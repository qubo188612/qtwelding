#include "process1_scanbeforetrace.h"
#include <sys/socket.h>

Process1_scanbeforetrace *Process1_scanbeforetrace::Get(my_parameters *mcs)
{
    static Process1_scanbeforetrace fun;
    fun.m_mcs=mcs;
    return &fun;
}

Process1_scanbeforetrace::Process1_scanbeforetrace()
{
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
    buildline=0;    //从首命令启动
    m_mcs->tosendbuffer->cmd_clear_elec_work(); //清除当前起弧状态
    m_mcs->tosendbuffer->cmd_lock(false);   //解锁机器人
    m_mcs->tosendbuffer->cmd_cam(0,0);      //关相机
    usleep(ROB_WORK_DELAY);
    b_thread=true;
    thread->start();
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
}

void Process1_scanbeforetrace::paused_process()
{
    m_mcs->tosendbuffer->cmdlist_stopbuild();
    if(b_thread==true)
    {
        thread->Stop();
        thread->quit();
        thread->wait();
    }
}

void Process1_scanbeforetrace::continue_process()
{
    m_mcs->tosendbuffer->cmd_lock(false);   //解锁机器人
    usleep(ROB_WORK_DELAY);
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
        rc=_p->m_mcs->tosendbuffer->cmdlist_build(_p->buildline);
        if(rc!=0)
        {
            goto OUT_THREAD_ERROR;
        }
        break;
    }    
OUT_THREAD_ERROR:
    _p->thread->quit();
    _p->thread->wait();
    _p->b_thread=false;
}

void Process1Thread::Stop()
{
  while(_p->b_thread==true)
  {
    sleep(0);
  }
}

