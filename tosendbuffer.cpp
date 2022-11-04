#include "tosendbuffer.h"

toSendbuffer *toSendbuffer::Get(my_parameters *mcs)
{
    static toSendbuffer fun;
    fun.m_mcs=mcs;
    return &fun;
}

toSendbuffer::toSendbuffer()
{
    b_cmdlist_build=false;
    u16data_elec_work=0;
}

toSendbuffer::~toSendbuffer()
{

}

void toSendbuffer::cmd_clear_elec_work()
{
    u16data_elec_work=0;
}

int toSendbuffer::cmdlist_creat_tracename_mem(int beforeline,std::vector<QString> &errmsg)
{
    errmsg.clear();
    int err=0;
    QString return_msg;
    m_mcs->project->project_scan_trace.clear();
    m_mcs->project->project_weld_trace.clear();
    for(int n=0;n<beforeline;n++)
    {
        QString msg,key;
        my_cmd cmd;
        int rc=cmd.decodecmd(m_mcs->project->project_cmdlist[n],msg,key);
        if(rc<=0)
        {
            if(key==CMD_SCAN_KEY)//采集指令
            {
                QString name=cmd.cmd_scan_name;//获取到的扫描名字
                bool b_find=0;
                for(int t=0;t<m_mcs->project->project_scan_trace.size();t++)
                {
                    if(m_mcs->project->project_scan_trace[t].name==name)
                    {
                        b_find=1;
                        break;
                    }
                }
                if(b_find==1)
                {
                    err=1;
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 扫描轨迹与已有的轨迹重名");
                    m_mcs->main_record.push_back(return_msg);
                    errmsg.push_back(return_msg);
                }
                else
                {
                    Scan_trace_result trace;
                    trace.name=name;
                    m_mcs->project->project_scan_trace.push_back(trace);
                }
            }
            else if(key==CMD_CREAT_KEY)//生成轨迹指令
            {
                QString name=cmd.cmd_creat_name;//获取到的生成的轨迹名字
                std::vector<QString> scannames=cmd.cmd_creat_scanname;//获取到要扫描的轨道名字
                Trace_edit_mode mode=cmd.cmd_creat_mode;//获取到的扫描模式
                bool b_find=0;
                for(int t=0;t<m_mcs->project->project_weld_trace.size();t++)
                {
                    if(m_mcs->project->project_weld_trace[t].name==name)
                    {
                        b_find=1;
                        break;
                    }
                }
                if(b_find==1)
                {
                    err=1;
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 跟踪轨迹与已有的轨迹重名");
                    m_mcs->main_record.push_back(return_msg);
                    errmsg.push_back(return_msg);
                }
                else
                {
                    switch(mode)
                    {
                        case TRACE_EDIT_MODE_ONE_TO_ONE:
                        {
                            if(scannames.size()!=1)
                            {
                                err=1;
                                return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": ")+
                                           QString::fromLocal8Bit(CMD_MODE)+QString::fromLocal8Bit("值为")+QString::number(TRACE_EDIT_MODE_ONE_TO_ONE)+
                                           QString::fromLocal8Bit("时,")+QString::fromLocal8Bit(CMD_SCAN)+QString::fromLocal8Bit("项的参数只能有1个");
                                m_mcs->main_record.push_back(return_msg);
                                errmsg.push_back(return_msg);
                                break;
                            }
                            bool b_find=false;
                            int m=0;
                            for(m=0;m<scannames.size();m++)
                            {
                                b_find=false;
                                for(int t=0;t<m_mcs->project->project_scan_trace.size();t++)
                                {
                                    if(scannames[m]==m_mcs->project->project_scan_trace[t].name)
                                    {
                                        b_find=true;

                                        Weld_trace_result trace;
                                        trace.name=name;
                                        m_mcs->project->project_weld_trace.push_back(trace);

                                        break;
                                    }
                                }
                                if(b_find==false)//没找到这个名字的扫描轨道
                                {
                                    break;
                                }
                            }
                            if(b_find==false)
                            {
                                err=1;
                                return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+scannames[m]+QString::fromLocal8Bit("的扫描轨道");
                                m_mcs->main_record.push_back(return_msg);
                                errmsg.push_back(return_msg);
                                break;
                            }
                        }
                        break;
                    default:
                        {
                            err=1;
                            return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 不支持当前轨道生成模式");
                            m_mcs->main_record.push_back(return_msg);
                            errmsg.push_back(return_msg);
                        }
                        break;
                    }
                }
            }
        }
    }
    return err;
}

int toSendbuffer::cmdlist_check()
{
    int err=0;
    QString return_msg;  
    std::vector<QString> err_msg;
    for(int n=0;n<m_mcs->project->project_cmdlist.size();n++)
    {
        QString msg,key;
        my_cmd cmd;
        int rc=cmd.decodecmd(m_mcs->project->project_cmdlist[n],msg,key);
        if(rc>0)
        {
            //语法出错
            err=1;
            return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 语法出错(")+msg+QString::fromLocal8Bit(")");
            m_mcs->main_record.push_back(return_msg);
        }
    }
    if(0!=cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
    {
        err=1;//轨道命名出错
    }
    return err;
}

int toSendbuffer::cmdlist_build(volatile int &line)
{
    QString return_msg;
    if(line<0||line>=m_mcs->project->project_cmdlist.size())
    {
        return_msg=QString::fromLocal8Bit("已经执行完全部命令");
        m_mcs->main_record.push_back(return_msg);
        return 1;
    }
    b_cmdlist_build=true;
    for(int n=line;n<m_mcs->project->project_cmdlist.size();n++)
    {
        QString msg,key;
        my_cmd cmd;
        return_msg=QString::fromLocal8Bit("执行Line")+QString::number(n)+QString::fromLocal8Bit(": ")+m_mcs->project->project_cmdlist[n];
        m_mcs->main_record.push_back(return_msg);
        int rc=cmd.decodecmd(m_mcs->project->project_cmdlist[n],msg,key);
        if(rc>0)
        {
            //语法出错
            return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 语法出错(")+msg+QString::fromLocal8Bit(")");
            m_mcs->main_record.push_back(return_msg);
            line=n;
            return 1;
        }
        else if(rc<0)//注释行
        {
            continue;
        }
        if(key==CMD_MOV_KEY)//移动指令
        {
            int tcp=cmd.cmd_move_tcp;//获取到移动TCP
            RobPos pos=cmd.cmd_move_pos;//获取到移动坐标
            float speed=cmd.cmd_move_speed;//获取到速度值
            Robmovemodel movemod=cmd.cmd_move_movemod;//获取到的移动模式
            cmd_move(pos,movemod,speed,tcp);//移动
            usleep(ROB_WORK_DELAY);
            while(m_mcs->rob->robot_state!=ROBOT_STATE_IDLE)//等待移动到位
            {
                if(b_cmdlist_build==false)     //停止
                {
                    return_msg=QString::fromLocal8Bit("手动停止进程");
                    m_mcs->main_record.push_back(return_msg);
                    cmd_lock(true);
                    line=n;
                    return 1;
                }
                usleep(ROB_WORK_DELAY_STEP);
            }
        }
        else if(key==CMD_DELAY_KEY)//延时指令
        {
            int time=cmd.cmd_delay_time;//获取到延时时间转ms
            cmd_delay(time);
        }
        else if(key==CMD_CAM_KEY)//激光头指令
        {
            int task=cmd.cmd_cam_task;
            int work=cmd.cmd_cam_work_d;
            cmd_cam(task,work);
        }
        else if(key==CMD_WELD_KEY)//起光弧指令
        {
            int work=cmd.cmd_elec_work_d;//获取到焊机启停
            Alternatingcurrent elem=cmd.cmd_elec_elem;  //获取到焊机交变电流模式
            float eled=cmd.cmd_elec_eled; //获取到焊机电流
            cmd_elec(eled,elem,work);
            usleep(ROB_WORK_DELAY);//确保焊机设置完成
        }
        else if(key==CMD_SCAN_KEY)//采集指令
        {
            RobPos pos=cmd.cmd_scan_pos;//获取到扫描终点坐标
            float speed=cmd.cmd_scan_speed;//获取到的扫描速度
            int tcp=cmd.cmd_scan_tcp;//获取到扫描TCP
            Robmovemodel movemod=cmd.cmd_scan_movemod;//获取到的扫描模式
            QString name=cmd.cmd_scan_name;//获取到的扫描名字
            int scan_trace_num;//要储存的轨道数据下标
            for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
            {
                if(name==m_mcs->project->project_scan_trace[n].name)
                {
                    scan_trace_num=n;//找到要储存的扫描轨迹下标
                    break;
                }
            }
            cmd_move(pos,movemod,speed,tcp);
            usleep(ROB_WORK_DELAY);
            while(m_mcs->rob->robot_state!=ROBOT_STATE_IDLE)//等待扫描到位
            {
                if(b_cmdlist_build==false)     //停止
                {
                    return_msg=QString::fromLocal8Bit("手动停止进程");
                    m_mcs->main_record.push_back(return_msg);
                    cmd_lock(true);
                    line=n;
                    return 1;
                }
                //这里添加扫描数据
                if(m_mcs->cam->sop_cam[0].b_updatacloud_finish==true)//有中断数据
                {
                    if(m_mcs->cam->sop_cam[0].b_ros_lineEn==true)//检测有正确结果
                    {
                        Scan_trace_line res;
                        res.robotpos=m_mcs->rob->TCPpos;
                        res.robottime=m_mcs->rob->robtime;
                        res.ros_line=*(m_mcs->cam->sop_cam[0].ros_line);
                        m_mcs->project->project_scan_trace[scan_trace_num].point.push_back(res);
                    }
                    m_mcs->cam->sop_cam[0].b_updatacloud_finish=false;
                }
                //开始采集检测数据
                usleep(0);
            }
        }
        else if(key==CMD_TRACE_KEY)//跟踪命令
        {
            QString name=cmd.cmd_trace_name;//获取到跟踪轨迹序号
            float speed=cmd.cmd_trace_speed;//获取到的跟踪速度
            int tcp=cmd.cmd_trace_tcp;//获取到跟踪TCP

            //这里添加轨迹生成
            //这里添加移动命令
        }
        if(b_cmdlist_build==false)//停止或暂停了
        {
            return_msg=QString::fromLocal8Bit("手动停止进程");
            m_mcs->main_record.push_back(return_msg);
            line=n;
            return 1;
        }
    }
    b_cmdlist_build=false;
    u16data_elec_work=0;
    return_msg=QString::fromLocal8Bit("指令执行结束");
    m_mcs->main_record.push_back(return_msg);
    line=m_mcs->project->project_cmdlist.size();
    return 0;
}

void toSendbuffer::cmdlist_stopbuild()
{
    b_cmdlist_build=false;
    u16data_elec_work=0;
}

void toSendbuffer::cmd_lock(bool lock)
{
    sent_info_robot sendrob;
    sendrob.addr=ROB_STOP_REG_ADD;
    sendrob.ctx=m_mcs->rob->ctx_posget;
    sendrob.data.resize(1);
    if(lock==true)
    {
        sendrob.data[0]=1;
    }
    else
    {
        sendrob.data[0]=0;
    }
    m_mcs->rob->b_send_group_robot=false;
    m_mcs->rob->send_group_robot.push_back(sendrob);
    m_mcs->rob->ctx_robot_dosomeing=DO_WRITE_TASK;
}

void toSendbuffer::cmd_move(RobPos pos,Robmovemodel movemodel,float speed,int tcp)
{
    sent_info_robot sendrob;
    sendrob.addr=ROB_TCP_NUM_REG_ADD;
    sendrob.ctx=m_mcs->rob->ctx_posget;
    sendrob.data.resize(17);
    sendrob.data[0]=tcp;
    sendrob.data[1]=movemodel;
    sendrob.data[2]=u16data_elec_work;
    sendrob.data[3]=*((u_int16_t*)&speed);
    sendrob.data[4]=*((u_int16_t*)&speed+1);
    sendrob.data[5]=*((u_int16_t*)&pos.X);
    sendrob.data[6]=*((u_int16_t*)&pos.X+1);
    sendrob.data[7]=*((u_int16_t*)&pos.Y);
    sendrob.data[8]=*((u_int16_t*)&pos.Y+1);
    sendrob.data[9]=*((u_int16_t*)&pos.Z);
    sendrob.data[10]=*((u_int16_t*)&pos.Z+1);
    sendrob.data[11]=*((u_int16_t*)&pos.RX);
    sendrob.data[12]=*((u_int16_t*)&pos.RX+1);
    sendrob.data[13]=*((u_int16_t*)&pos.RY);
    sendrob.data[14]=*((u_int16_t*)&pos.RY+1);
    sendrob.data[15]=*((u_int16_t*)&pos.RZ);
    sendrob.data[16]=*((u_int16_t*)&pos.RZ+1);

    m_mcs->rob->b_send_group_robot=false;
    m_mcs->rob->send_group_robot.push_back(sendrob);
    m_mcs->rob->ctx_robot_dosomeing=DO_WRITE_TASK;
}

void toSendbuffer::cmd_delay(int time)
{
    int mtime=time*1000;//获取到延时时间转ms
    usleep(mtime);
}

void toSendbuffer::cmd_cam(int task,int work)
{
    sent_info_leaser sentcam;
    sentcam.ctx=m_mcs->resultdata.ctx_result;
    sentcam.addr=ALS_OPEN_REG_ADD;
    sentcam.data.resize(2);
    if(work==0)
    {
        sentcam.data[0]=0;
    }
    else
    {
        sentcam.data[0]=0xff;
    }
    sentcam.data[1]=task;
    m_mcs->resultdata.b_send_group_leaser=false;
    m_mcs->resultdata.send_group_leaser.push_back(sentcam);
    m_mcs->resultdata.ctx_result_dosomeing=DO_WRITE_TASK;
}

void toSendbuffer::cmd_elec(float eled,Alternatingcurrent elem,int work)
{
    sent_info_robot sendrob;
    sendrob.addr=ROB_WELD_CURRENT_FH_REG_ADD;
    sendrob.ctx=m_mcs->rob->ctx_posget;
    sendrob.data.resize(3);
    sendrob.data[0]=*((u_int16_t*)&eled);
    sendrob.data[1]=*((u_int16_t*)&eled+1);
    sendrob.data[2]=elem;
    if(work==0)
    {
        u16data_elec_work=0;
    }
    else
    {
        u16data_elec_work=1;
    }
    m_mcs->rob->b_send_group_robot=false;
    m_mcs->rob->send_group_robot.push_back(sendrob);
    m_mcs->rob->ctx_robot_dosomeing=DO_WRITE_TASK;
}

