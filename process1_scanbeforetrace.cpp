#include "process1_scanbeforetrace.h"
#include "my_parameters.h"
#if _MSC_VER
#include <synchapi.h>
#else
#include <sys/socket.h>
#endif

Process1_scanbeforetrace *Process1_scanbeforetrace::Get(my_parameters *mcs)
{
    static Process1_scanbeforetrace fun;
    fun.m_mcs=mcs;
    return &fun;
}

Process1_scanbeforetrace::Process1_scanbeforetrace()
{
    b_skip=false;
    buildline=0;
    b_thread=false;
    thread = new Process1Thread(this);
}

Process1_scanbeforetrace::~Process1_scanbeforetrace()
{
    if(b_thread==true)
    {
        thread->Stop();
        thread->quit();
        thread->wait();
    }
    delete thread;
}

void Process1_scanbeforetrace::init_start_process()
{
    b_skip=false;
    buildline=0;    //从首命令启动
    m_mcs->tosendbuffer->cmd_clear_elec_work(); //清除当前起弧状态
    m_mcs->tosendbuffer->cmd_lock(0);   //解锁机器人
    m_mcs->tosendbuffer->cmd_cam(0,0);      //关相机
    usleep(ROB_WORK_DELAY);
    b_thread=true;
    thread->start();
}

void Process1_scanbeforetrace::init_skip_start_process(int stline)
{
    /*****************/
    //载入上一次数据
    b_skip=true;
    /*****************/
    buildline=stline;
    m_mcs->tosendbuffer->cmd_clear_elec_work(); //清除当前起弧状态
    m_mcs->tosendbuffer->cmd_lock(0);   //解锁机器人
    m_mcs->tosendbuffer->cmd_cam(0,0);      //关相机
    usleep(ROB_WORK_DELAY);
    b_thread=true;
    thread->start();
}

void Process1_scanbeforetrace::check_data_process()
{
    int rc;
    rc=m_mcs->tosendbuffer->cmdlist_check();//检查指令
    if(rc!=0)
    {
        return;
    }
    m_mcs->tosendbuffer->cmdlist_skip(m_mcs->project->project_cmdlist.size()-1);
}

void Process1_scanbeforetrace::stop_process()
{
    m_mcs->tosendbuffer->cmdlist_stopbuild();
    if(b_thread==true)
    {
        thread->Stop();
        thread->quit();
        thread->wait();
    }
#ifdef USE_MYROBOT_CONTROL
    m_mcs->robotcontrol->clear_movepoint_buffer();
#endif
    b_skip=false;
}

void Process1_scanbeforetrace::paused_process()
{
    m_mcs->tosendbuffer->cmdlist_pausedbuild();
    if(b_thread==true)
    {
        thread->Stop();
        thread->quit();
        thread->wait();
    }
    b_skip=false;
}

void Process1_scanbeforetrace::continue_process()
{
    b_skip=false;
    m_mcs->tosendbuffer->cmd_lock(2);   //解锁机器人
    usleep(ROB_WORK_DELAY);
    if(buildline<m_mcs->project->project_cmdlist.size())
    {
        if(m_mcs->tosendbuffer->paused_key==CMD_TRACING_KEY)//已经存在下位机了
        {
            buildline=buildline+1;//此处越界1没问题，之后会判断
        }
    }
    b_thread=true;
    thread->start();//继续从当前命令往下走
}

Process1Thread::Process1Thread(Process1_scanbeforetrace *statci_p)
{
    _p=statci_p;
}

void Process1Thread::run()
{
    QString msg;
    while(_p->b_thread==true)
    {
        int rc;
        rc=_p->m_mcs->tosendbuffer->cmdlist_check();//检查指令
        if(rc!=0)
        {
            goto OUT_THREAD_ERROR;
        }
        if(_p->b_thread==false)
        {
            goto OUT_THREAD_ERROR;
        }
    #ifdef USE_MYROBOT_CONTROL
        _p->m_mcs->robotcontrol->RobotOPEN_ELE();//机器人上电
    #endif
        if(_p->b_thread==false)
        {
            goto OUT_THREAD_ERROR;
        }
        if(_p->b_skip==true)//跳过执行之前的程序
        {
            rc=_p->m_mcs->tosendbuffer->cmdlist_skip(_p->buildline-1<0?0:_p->buildline-1);
            if(rc!=0)
            {
                goto OUT_THREAD_ERROR;
            }
        }
        rc=_p->m_mcs->tosendbuffer->cmdlist_build(_p->buildline);
        if(rc!=0)
        {
            goto OUT_THREAD_ERROR;
        }
        if(_p->m_mcs->e2proomdata.maindlg_circlerun==0)
        {
            break;
        }
        else
        {
            _p->buildline=0;
        }
    }    
OUT_THREAD_ERROR:
    _p->thread->quit();
    _p->thread->wait();
    _p->b_thread=false;
    _p->m_mcs->tosendbuffer->cmd_lock(0);//机器人停止
 // 以后用停止
}

void Process1Thread::Stop()
{
    _p->b_thread=false;
/*
  while(_p->b_thread==true)
  {
    sleep(0);
  }
  */
}

