#include "my_cmd.h"

my_cmd::my_cmd()
{
    cmd_move_tcp=0;//获取到移动TCP
    cmd_move_speed=0;//获取到速度值
    cmd_move_movemod=MOVEL;//获取到的移动模式
    cmd_delay_time=0;//获取到延时时间
    cmd_cam_task=0;//获取到相机任务号
    cmd_cam_work_d=0;//获取到相机启停
    cmd_elec_work_d=0;//获取到焊机启停
    cmd_elec_elem=DIRECT;  //获取到焊机交变电流模式
    cmd_elec_eled=0; //获取到焊机电流
    cmd_scan_speed=0;//获取到的扫描速度
    cmd_scan_tcp=0;//获取到扫描TCP
    cmd_scan_movemod=MOVEL;//获取到的扫描模式
    cmd_trace_speed=0;//获取到的跟踪速度
    cmd_trace_tcp=0;//获取到跟踪TCP
    cmd_creat_mode=TRACE_EDIT_MODE_ONE_TO_ONE;//获取到轨迹生成模式
}

QString my_cmd::cmd_move(RobPos pos,Robmovemodel movemodel,float speed,int tcp)
{
    QString msg;
    QString msg1;

    msg=QString(CMD_MOV_KEY)+" "+
            rc_move(pos,movemodel)+" "+
            rc_speed(speed)+" "+
            rc_tcp(tcp);
    return msg;
}

QString my_cmd::cmd_moveC(RobPos pos1,RobPos pos2,Robmovemodel movemodel,float speed,int tcp)
{
    QString msg;
    QString msg1;

    msg=QString(CMD_MOV_KEY)+" "+
            rc_moveC(pos1,pos2,movemodel)+" "+
            rc_speed(speed)+" "+
            rc_tcp(tcp);
    return msg;
}

QString my_cmd::cmd_delay(int time)
{
    QString msg;
    msg=QString(CMD_DELAY_KEY)+" "+
            rc_time(time);
    return msg;
}

QString my_cmd::cmd_cam(int task,int work)
{
    QString msg;
    msg=QString(CMD_CAM_KEY)+" "+
            rc_task(task)+" "+
            rc_work(work);
    return msg;
}

QString my_cmd::cmd_cam_work(int work)
{
    QString msg;
    msg=QString(CMD_CAM_KEY)+" "+
            rc_work(work);
    return msg;
}

QString my_cmd::cmd_elec(float eled,Alternatingcurrent elem,int work)
{
    QString msg;
    msg=QString(CMD_WELD_KEY)+" "+
            rc_eled(eled)+" "+
            rc_elem(elem)+" "+
            rc_work(work);
    return msg;
}

QString my_cmd::cmd_elec_work(int work)
{
    QString msg;
    msg=QString(CMD_WELD_KEY)+" "+
            rc_work(work);
    return msg;
}

QString my_cmd::cmd_scan(RobPos pos,Robmovemodel movemodel,float speed,int tcp,QString name)
{
    QString msg;
    QString msg1;

    msg=QString(CMD_SCAN_KEY)+" "+
            rc_move(pos,movemodel)+" "+
            rc_speed(speed)+" "+
            rc_tcp(tcp)+" "+
            rc_name(name);
    return msg;
}

QString my_cmd::cmd_scanP(RobPos pos1,RobPos pos2,Robmovemodel movemodel,float speed,int tcp,QString name)
{
    QString msg;
    QString msg1;

    msg=QString(CMD_SCAN_KEY)+" "+
            rc_moveC(pos1,pos2,movemodel)+" "+
            rc_speed(speed)+" "+
            rc_tcp(tcp)+" "+
            rc_name(name);
    return msg;
}

QString my_cmd::cmd_trace(float speed,int tcp,QString craftfilepath,QString name)
{
    QString msg;
    QString msg1;

    msg=QString(CMD_TRACE_KEY)+" "+
            rc_speed(speed)+" "+
            rc_tcp(tcp)+" "+
            rc_craft(craftfilepath)+" "+
            rc_name(name);
    return msg;
}

QString my_cmd::cmd_creat(Trace_edit_mode mode,std::vector<QString> scanname,QString name)
{
    QString msg;
    QString msg1;

    msg=QString(CMD_CREAT_KEY)+" "+
            rc_mode(mode)+" "+
            rc_scan(scanname)+" "+
            rc_name(name);
    return msg;
}

int my_cmd::decodecmd(QString msg,QString &return_msg,QString &return_key)
{
    if(msg.isEmpty())
    {
        return_msg=QString::fromLocal8Bit("指令为空");
        return 1;
    }
    int exegesis=msg.indexOf("#");//寻找注释
    if(exegesis>=0)//代码有注释
    {
        msg=msg.left(exegesis);//截取注释左侧代码
        if(msg.size()==0)
        {
            return -1;//是注释行
        }
    }
    QStringList list = msg.split(":");
    if(list.size()!=2)
    {
        return_msg=QString::fromLocal8Bit("指令中只能出现一个':'");
        return 1;
    }
    QString key=list[0]+":";
    if(key==CMD_MOV_KEY)
    {
        int pn=0;
        bool b_SPEED=false;
        bool b_MOVE=false;
        bool b_TCP=false;

        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n]!=0)
            {
                QString paramname;
                int data_fpos,data_bpos;
                if(0!=de_param(++pn,param[n],paramname,data_fpos,data_bpos,return_msg))
                {
                    return 1;
                }
                if(paramname==CMD_SPEED)
                {
                    if(b_SPEED==false)
                    {
                        b_SPEED=true;
                        if(0!=de_float(paramname,param[n],data_fpos,data_bpos,cmd_move_speed,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QString::fromLocal8Bit("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_MOVL)
                {
                    if(b_MOVE==false)
                    {
                        b_MOVE=true;
                        if(0!=de_robpos(paramname,param[n],data_fpos,data_bpos,cmd_move_pos,return_msg))
                        {
                            return 1;
                        }
                        cmd_move_movemod=MOVEL;
                    }
                    else
                    {
                        return_msg=("只能有一个MOV类型项");
                        return 1;
                    }
                }
                else if(paramname==CMD_MOVJ)
                {
                    if(b_MOVE==false)
                    {
                        b_MOVE=true;
                        if(0!=de_robpos(paramname,param[n],data_fpos,data_bpos,cmd_move_pos,return_msg))
                        {
                            return 1;
                        }
                        cmd_move_movemod=MOVEJ;
                    }
                    else
                    {
                        return_msg=("只能有一个MOV类型项");
                        return 1;
                    }
                }
                else if(paramname==CMD_MOVP)
                {
                    if(b_MOVE==false)
                    {
                        b_MOVE=true;
                        if(0!=de_robposP(paramname,param[n],data_fpos,data_bpos,cmd_move_pos1,cmd_move_pos2,return_msg))
                        {
                            return 1;
                        }
                        cmd_move_movemod=MOVEC;
                    }
                    else
                    {
                        return_msg=("只能有一个MOV类型项");
                        return 1;
                    }
                }
                else if(paramname==CMD_TCP)
                {
                    if(b_TCP==false)
                    {
                        b_TCP=true;
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,cmd_move_tcp,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_move_tcp<0||cmd_move_tcp>=ROBOTTCPNUM)
                        {
                            return_msg=("TCP的值超出设置范围");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QString::fromLocal8Bit("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QString::fromLocal8Bit("指令里没有这个'")+paramname+QString::fromLocal8Bit("'参数名称");
                    return 1;
                }
            }
        }
        if(b_SPEED==false)
        {
            return_msg=key+QString::fromLocal8Bit("指令还需要设置'")+CMD_SPEED+QString::fromLocal8Bit("'项参数");
            return 1;
        }
        else if(b_MOVE==false)
        {
            return_msg=key+QString::fromLocal8Bit("指令还需要设置'")+CMD_MOVL+"'或'"+CMD_MOVJ+"'或'"+CMD_MOVP+QString::fromLocal8Bit("'项参数");
            return 1;
        }
        else if(b_TCP==false)
        {
            return_msg=key+QString::fromLocal8Bit("指令还需要设置'")+CMD_TCP+QString::fromLocal8Bit("'项参数");
            return 1;
        }
    }
    else if(key==CMD_DELAY_KEY)
    {
        int pn=0;
        bool b_TIME=false;
        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n]!=0)
            {
                QString paramname;
                int data_fpos,data_bpos;
                if(0!=de_param(++pn,param[n],paramname,data_fpos,data_bpos,return_msg))
                {
                    return 1;
                }
                if(paramname==CMD_TIME)
                {
                    if(b_TIME==false)
                    {
                        b_TIME=true;
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,cmd_delay_time,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QString::fromLocal8Bit("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QString::fromLocal8Bit("指令里没有这个'")+paramname+QString::fromLocal8Bit("'参数名称");
                    return 1;
                }
            }
        }
        if(b_TIME==false)
        {
            return_msg=key+QString::fromLocal8Bit("指令还需要设置'")+CMD_TIME+QString::fromLocal8Bit("'项参数");
            return 1;
        }
    }
    else if(key==CMD_CAM_KEY)
    {
        int pn=0;
        bool b_WORK=false;
        bool b_TASK=false;
        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n]!=0)
            {
                QString paramname;
                int data_fpos,data_bpos;
                if(0!=de_param(++pn,param[n],paramname,data_fpos,data_bpos,return_msg))
                {
                    return 1;
                }
                if(paramname==CMD_WORK)
                {
                    if(b_WORK==false)
                    {
                        b_WORK=true;
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,cmd_cam_work_d,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_cam_work_d!=1&&cmd_cam_work_d!=0)
                        {
                            return_msg=paramname+QString::fromLocal8Bit("项参数只能为0或1");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QString::fromLocal8Bit("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_TASK)
                {
                    if(b_TASK==false)
                    {
                        b_TASK=true;
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,cmd_cam_task,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QString::fromLocal8Bit("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QString::fromLocal8Bit("指令里没有这个'")+paramname+QString::fromLocal8Bit("'参数名称");
                    return 1;
                }
            }
        }
        if(b_WORK==false)
        {
            return_msg=key+QString::fromLocal8Bit("指令还需要设置'")+CMD_WORK+QString::fromLocal8Bit("'项参数");
            return 1;
        }
        else if(b_TASK==false)
        {
            if(cmd_cam_work_d==1)
            {
                return_msg=key+QString::fromLocal8Bit("指令在")+CMD_WORK+QString::fromLocal8Bit("值等于1时还需要设置'")+CMD_TASK+QString::fromLocal8Bit("'项参数");
                return 1;
            }
        }

    }
    else if(key==CMD_WELD_KEY)
    {
        int pn=0;
        bool b_WORK=false;
        bool b_ELED=false;
        bool b_ELEM=false;
        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n]!=0)
            {
                QString paramname;
                int data_fpos,data_bpos;
                if(0!=de_param(++pn,param[n],paramname,data_fpos,data_bpos,return_msg))
                {
                    return 1;
                }
                if(paramname==CMD_WORK)
                {
                    if(b_WORK==false)
                    {
                        b_WORK=true;
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,cmd_elec_work_d,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_elec_work_d!=1&&cmd_elec_work_d!=0)
                        {
                            return_msg=paramname+QString::fromLocal8Bit("项参数只能为0或1");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QString::fromLocal8Bit("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_ELEM)
                {
                    if(b_ELEM==false)
                    {
                        b_ELEM=true;
                        int data;
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,data,return_msg))
                        {
                            return 1;
                        }
                        cmd_elec_elem=(Alternatingcurrent)data;
                    }
                    else
                    {
                        return_msg=paramname+QString::fromLocal8Bit("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_ELED)
                {
                    if(b_ELED==false)
                    {
                        b_ELED=true;
                        if(0!=de_float(paramname,param[n],data_fpos,data_bpos,cmd_elec_eled,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QString::fromLocal8Bit("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QString::fromLocal8Bit("指令里没有这个'")+paramname+QString::fromLocal8Bit("'参数名称");
                    return 1;
                }
            }
        }
        if(b_WORK==false)
        {
            return_msg=key+QString::fromLocal8Bit("指令还需要设置'")+CMD_WORK+QString::fromLocal8Bit("'项参数");
            return 1;
        }
        else if(b_ELEM==false)
        {
            if(cmd_elec_work_d==1)
            {
                return_msg=key+QString::fromLocal8Bit("指令在")+CMD_WORK+QString::fromLocal8Bit("值等于1时还需要设置'")+CMD_ELEM+QString::fromLocal8Bit("'项参数");
                return 1;
            }
        }
        else if(b_ELED==false)
        {
            if(cmd_elec_work_d==1)
            {
                return_msg=key+QString::fromLocal8Bit("指令在")+CMD_WORK+QString::fromLocal8Bit("值等于1时还需要设置'")+CMD_ELED+QString::fromLocal8Bit("'项参数");
                return 1;
            }
        }
    }
    else if(key==CMD_SCAN_KEY)
    {
        int pn=0;
        bool b_SPEED=false;
        bool b_MOVE=false;
        bool b_TCP=false;
        bool b_NAME=false;

        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n]!=0)
            {
                QString paramname;
                int data_fpos,data_bpos;
                if(0!=de_param(++pn,param[n],paramname,data_fpos,data_bpos,return_msg))
                {
                    return 1;
                }
                if(paramname==CMD_SPEED)
                {
                    if(b_SPEED==false)
                    {
                        b_SPEED=true;
                        if(0!=de_float(paramname,param[n],data_fpos,data_bpos,cmd_scan_speed,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QString::fromLocal8Bit("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_MOVL)
                {
                    if(b_MOVE==false)
                    {
                        b_MOVE=true;
                        if(0!=de_robpos(paramname,param[n],data_fpos,data_bpos,cmd_scan_pos,return_msg))
                        {
                            return 1;
                        }
                        cmd_scan_movemod=MOVEL;
                    }
                    else
                    {
                        return_msg=("只能有一个MOV类型项");
                        return 1;
                    }
                }
                else if(paramname==CMD_MOVJ)
                {
                    if(b_MOVE==false)
                    {
                        b_MOVE=true;
                        if(0!=de_robpos(paramname,param[n],data_fpos,data_bpos,cmd_scan_pos,return_msg))
                        {
                            return 1;
                        }
                        cmd_scan_movemod=MOVEJ;
                    }
                    else
                    {
                        return_msg=("只能有一个MOV类型项");
                        return 1;
                    }
                }
                else if(paramname==CMD_MOVP)
                {
                    if(b_MOVE==false)
                    {
                        b_MOVE=true;
                        if(0!=de_robposP(paramname,param[n],data_fpos,data_bpos,cmd_scan_pos1,cmd_scan_pos2,return_msg))
                        {
                            return 1;
                        }
                        cmd_scan_movemod=MOVEC;
                    }
                    else
                    {
                        return_msg=("只能有一个MOV类型项");
                        return 1;
                    }
                }
                else if(paramname==CMD_TCP)
                {
                    if(b_TCP==false)
                    {
                        b_TCP=true;
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,cmd_scan_tcp,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_scan_tcp<0||cmd_scan_tcp>=ROBOTTCPNUM)
                        {
                            return_msg=("TCP的值超出设置范围");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QString::fromLocal8Bit("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_NAME)
                {
                    if(b_NAME==false)
                    {
                        b_NAME=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_scan_name,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QString::fromLocal8Bit("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QString::fromLocal8Bit("指令里没有这个'")+paramname+QString::fromLocal8Bit("'参数名称");
                    return 1;
                }
            }
        }
        if(b_SPEED==false)
        {
            return_msg=key+QString::fromLocal8Bit("指令还需要设置'")+CMD_SPEED+QString::fromLocal8Bit("'项参数");
            return 1;
        }
        else if(b_MOVE==false)
        {
            return_msg=key+QString::fromLocal8Bit("指令还需要设置'")+CMD_MOVL+"'或'"+CMD_MOVJ+"'或'"+CMD_MOVP+QString::fromLocal8Bit("'项参数");
            return 1;
        }
        else if(b_TCP==false)
        {
            return_msg=key+QString::fromLocal8Bit("指令还需要设置'")+CMD_TCP+QString::fromLocal8Bit("'项参数");
            return 1;
        }
        else if(b_NAME==false)
        {
            return_msg=key+QString::fromLocal8Bit("指令还需要设置'")+CMD_NAME+QString::fromLocal8Bit("'项参数");
            return 1;
        }
    }
    else if(key==CMD_TRACE_KEY)
    {
        int pn=0;
        bool b_SPEED=false;
        bool b_NAME=false;
        bool b_TCP=false;
        bool b_CRAFT=false;

        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n]!=0)
            {
                QString paramname;
                int data_fpos,data_bpos;
                if(0!=de_param(++pn,param[n],paramname,data_fpos,data_bpos,return_msg))
                {
                    return 1;
                }
                if(paramname==CMD_SPEED)
                {
                    if(b_SPEED==false)
                    {
                        b_SPEED=true;
                        if(0!=de_float(paramname,param[n],data_fpos,data_bpos,cmd_trace_speed,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QString::fromLocal8Bit("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_TCP)
                {
                    if(b_TCP==false)
                    {
                        b_TCP=true;
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,cmd_trace_tcp,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_trace_tcp<0||cmd_trace_tcp>=ROBOTTCPNUM)
                        {
                            return_msg=("TCP的值超出设置范围");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QString::fromLocal8Bit("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_NAME)
                {
                    if(b_NAME==false)
                    {
                        b_NAME=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_trace_name,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QString::fromLocal8Bit("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_CRAFT)
                {
                    if(b_CRAFT==false)
                    {
                        b_CRAFT=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_trace_craftfilepath,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QString::fromLocal8Bit("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QString::fromLocal8Bit("指令里没有这个'")+paramname+QString::fromLocal8Bit("'参数名称");
                    return 1;
                }
            }
        }
        if(b_SPEED==false)
        {
            return_msg=key+QString::fromLocal8Bit("指令还需要设置'")+CMD_SPEED+QString::fromLocal8Bit("'项参数");
            return 1;
        }
        else if(b_NAME==false)
        {
            return_msg=key+QString::fromLocal8Bit("指令还需要设置'")+CMD_NAME+QString::fromLocal8Bit("'项参数");
            return 1;
        }
        else if(b_TCP==false)
        {
            return_msg=key+QString::fromLocal8Bit("指令还需要设置'")+CMD_TCP+QString::fromLocal8Bit("'项参数");
            return 1;
        }
        else if(b_CRAFT==false)
        {
            return_msg=key+QString::fromLocal8Bit("指令还需要设置'")+CMD_CRAFT+QString::fromLocal8Bit("'项参数");
            return 1;
        }
    }
    else if(key==CMD_CREAT_KEY)
    {
        int pn=0;
        bool b_NAME=false;
        bool b_MODE=false;
        bool b_SCAN=false;

        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n]!=0)
            {
                QString paramname;
                int data_fpos,data_bpos;
                if(0!=de_param(++pn,param[n],paramname,data_fpos,data_bpos,return_msg))
                {
                    return 1;
                }
                if(paramname==CMD_NAME)
                {
                    if(b_NAME==false)
                    {
                        b_NAME=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_creat_name,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QString::fromLocal8Bit("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_MODE)
                {
                    if(b_MODE==false)
                    {
                        b_MODE=true;
                        int data;
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,data,return_msg))
                        {
                            return 1;
                        }
                        cmd_creat_mode=(Trace_edit_mode)data;
                    }
                    else
                    {
                        return_msg=paramname+QString::fromLocal8Bit("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_SCAN)
                {
                    if(b_SCAN==false)
                    {
                        b_SCAN=true;
                        if(0!=de_vector_QString(paramname,param[n],data_fpos,data_bpos,cmd_creat_scanname,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QString::fromLocal8Bit("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QString::fromLocal8Bit("指令里没有这个'")+paramname+QString::fromLocal8Bit("'参数名称");
                    return 1;
                }
            }
        }
        if(b_NAME==false)
        {
            return_msg=key+QString::fromLocal8Bit("指令还需要设置'")+CMD_NAME+QString::fromLocal8Bit("'项参数");
            return 1;
        }
        else if(b_MODE==false)
        {
            return_msg=key+QString::fromLocal8Bit("指令还需要设置'")+CMD_MODE+QString::fromLocal8Bit("'项参数");
            return 1;
        }
        else if(b_SCAN==false)
        {
            return_msg=key+QString::fromLocal8Bit("指令还需要设置'")+CMD_SCAN+QString::fromLocal8Bit("'项参数");
            return 1;
        }
    }
    else
    {
        return_msg=QString::fromLocal8Bit("指令集中没有'")+key+QString::fromLocal8Bit("'类型的指令，请查看支持的指令表");
        return 1;
    }
    return_key=key;

    return 0;
}

QString my_cmd::rc_tcp(int tcp)
{
    QString msg;
    msg=QString(CMD_TCP)+"["+QString::number(tcp)+"]";
    return msg;
}

QString my_cmd::rc_speed(float speed)
{
    QString msg;
    msg=QString(CMD_SPEED)+"["+QString::number(speed,'f',3)+"]";
    return msg;
}

QString my_cmd::rc_move(RobPos pos,Robmovemodel movemodel)
{
    QString msg;
    QString msg1;
    switch(movemodel)
    {
        case MOVEL:
        {
            msg1=QString(CMD_MOVL);
        }
        break;
        case MOVEJ:
        {
            msg1=QString(CMD_MOVJ);
        }
        break;
    }
    msg=msg1+"["+QString::number(pos.X,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                 QString::number(pos.Y,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                 QString::number(pos.Z,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                 QString::number(pos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                 QString::number(pos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                 QString::number(pos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+"]";
    return msg;
}

QString my_cmd::rc_moveC(RobPos pos1,RobPos pos2,Robmovemodel movemodel)
{
    QString msg;
    QString msg1;
    switch(movemodel)
    {
        case MOVEC:
        {
            msg1=QString(CMD_MOVP);
        }
        break;
    }
    msg=msg1+"["+QString::number(pos1.X,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                 QString::number(pos1.Y,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                 QString::number(pos1.Z,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                 QString::number(pos1.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                 QString::number(pos1.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                 QString::number(pos1.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                 QString::number(pos2.X,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                 QString::number(pos2.Y,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                 QString::number(pos2.Z,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                 QString::number(pos2.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                 QString::number(pos2.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                 QString::number(pos2.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+"]";
    return msg;
}

QString my_cmd::rc_time(int time)
{
    QString msg;
    msg=QString(CMD_TIME)+"["+QString::number(time)+"]";
    return msg;
}

QString my_cmd::rc_task(int task)
{
    QString msg;
    msg=QString(CMD_TASK)+"["+QString::number(task)+"]";
    return msg;
}

QString my_cmd::rc_work(int work)
{
    QString msg;
    msg=QString(CMD_WORK)+"["+QString::number(work)+"]";
    return msg;
}

QString my_cmd::rc_elem(Alternatingcurrent elem)
{
    QString msg;
    msg=QString(CMD_ELEM)+"["+QString::number(elem)+"]";
    return msg;
}

QString my_cmd::rc_eled(float eled)
{
    QString msg;
    msg=QString(CMD_ELED)+"["+QString::number(eled,'f',3)+"]";
    return msg;
}

QString my_cmd::rc_name(QString name)
{
    QString msg;
    msg=QString(CMD_NAME)+"["+name+"]";
    return msg;
}

QString my_cmd::rc_mode(int mode)
{
    QString msg;
    msg=QString(CMD_MODE)+"["+QString::number(mode)+"]";
    return msg;
}

QString my_cmd::rc_scan(std::vector<QString> names)
{
    QString msg;
    QString msg1;
    for(int n=0;n<names.size();n++)
    {
        msg1=msg1+names[n];
        if(n!=names.size()-1)
        {
            msg1=msg1+",";
        }
    }
    msg=QString(CMD_SCAN)+"["+msg1+"]";
    return msg;
}

QString my_cmd::rc_craft(QString craftfilepath)
{
    QString msg;
    msg=QString(CMD_CRAFT)+"["+craftfilepath+"]";
    return msg;
}

int my_cmd::de_param(int param_n,QString msg,QString &paramname,int &data_fpos,int &data_bpos,QString &return_msg)
{
    data_fpos = msg.indexOf("[");
    data_bpos = msg.indexOf("]");
    if(data_fpos<0||data_bpos<0||data_bpos<data_fpos||data_bpos!=msg.size()-1)
    {
        return_msg=QString::fromLocal8Bit("第")+QString::number(param_n)+QString::fromLocal8Bit("项参数格式错误");
        return 1;
    }
    paramname=msg.mid(0,data_fpos);
    if(paramname.size()==0)
    {
        return_msg=QString::fromLocal8Bit("第")+QString::number(param_n)+QString::fromLocal8Bit("项参数没有参数名称");
        return 1;
    }
    return 0;
}

int my_cmd::de_int(QString parakey,QString msg,int data_fpos,int data_bpos,int &intdata,QString &return_msg)
{
    QString paramdata=msg.mid(data_fpos+1,data_bpos-data_fpos-1);
    if(paramdata.size()==0)
    {
        return_msg=parakey+QString::fromLocal8Bit("项参数数据为空");
        return 1;
    }
    bool ok;
    intdata=paramdata.toInt(&ok);//获取到值
    if(ok==false)
    {
        return_msg=parakey+QString::fromLocal8Bit("项参数数据格式错误");
        return 1;
    }
    return 0;
}

int my_cmd::de_float(QString parakey,QString msg,int data_fpos,int data_bpos,float &floatdata,QString &return_msg)
{
    QString paramdata=msg.mid(data_fpos+1,data_bpos-data_fpos-1);
    if(paramdata.size()==0)
    {
        return_msg=parakey+QString::fromLocal8Bit("项参数数据为空");
        return 1;
    }
    bool ok;
    floatdata=paramdata.toFloat(&ok);//获取到值
    if(ok==false)
    {
        return_msg=parakey+QString::fromLocal8Bit("项参数数据格式错误");
        return 1;
    }
    return 0;
}

int my_cmd::de_robpos(QString parakey,QString msg,int data_fpos,int data_bpos,RobPos &pos,QString &return_msg)
{
    QString paramdata=msg.mid(data_fpos+1,data_bpos-data_fpos-1);
    if(paramdata.size()==0)
    {
        return_msg=parakey+QString::fromLocal8Bit("项参数数据为空");
        return 1;
    }
    QStringList posgroup = paramdata.split(",");
    if(posgroup.size()!=6)
    {
        return_msg=parakey+QString::fromLocal8Bit("项参数数据有且只有6个");
        return 1;
    }
    bool ok;
    pos.X=posgroup[0].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QString::fromLocal8Bit("项的X项参数数据格式错误");
        return 1;
    }
    pos.Y=posgroup[1].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QString::fromLocal8Bit("项的Y项参数数据格式错误");
        return 1;
    }
    pos.Z=posgroup[2].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QString::fromLocal8Bit("项的Z项参数数据格式错误");
        return 1;
    }
    pos.RX=posgroup[3].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QString::fromLocal8Bit("项的RX项参数数据格式错误");
        return 1;
    }
    pos.RY=posgroup[4].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QString::fromLocal8Bit("项的RY项参数数据格式错误");
        return 1;
    }
    pos.RZ=posgroup[5].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QString::fromLocal8Bit("项的RZ项参数数据格式错误");
        return 1;
    }
    return 0;
}

int my_cmd::de_robposP(QString parakey,QString msg,int data_fpos,int data_bpos,RobPos &pos1,RobPos &pos2,QString &return_msg)
{
    QString paramdata=msg.mid(data_fpos+1,data_bpos-data_fpos-1);
    if(paramdata.size()==0)
    {
        return_msg=parakey+QString::fromLocal8Bit("项参数数据为空");
        return 1;
    }
    QStringList posgroup = paramdata.split(",");
    if(posgroup.size()!=12)
    {
        return_msg=parakey+QString::fromLocal8Bit("项参数数据有且只有12个");
        return 1;
    }
    bool ok;
    pos1.X=posgroup[0].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QString::fromLocal8Bit("项的X1项参数数据格式错误");
        return 1;
    }
    pos1.Y=posgroup[1].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QString::fromLocal8Bit("项的Y1项参数数据格式错误");
        return 1;
    }
    pos1.Z=posgroup[2].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QString::fromLocal8Bit("项的Z1项参数数据格式错误");
        return 1;
    }
    pos1.RX=posgroup[3].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QString::fromLocal8Bit("项的RX1项参数数据格式错误");
        return 1;
    }
    pos1.RY=posgroup[4].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QString::fromLocal8Bit("项的RY1项参数数据格式错误");
        return 1;
    }
    pos1.RZ=posgroup[5].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QString::fromLocal8Bit("项的RZ1项参数数据格式错误");
        return 1;
    }
    pos2.X=posgroup[6].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QString::fromLocal8Bit("项的X2项参数数据格式错误");
        return 1;
    }
    pos2.Y=posgroup[7].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QString::fromLocal8Bit("项的Y2项参数数据格式错误");
        return 1;
    }
    pos2.Z=posgroup[8].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QString::fromLocal8Bit("项的Z2项参数数据格式错误");
        return 1;
    }
    pos2.RX=posgroup[9].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QString::fromLocal8Bit("项的RX2项参数数据格式错误");
        return 1;
    }
    pos2.RY=posgroup[10].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QString::fromLocal8Bit("项的RY2项参数数据格式错误");
        return 1;
    }
    pos2.RZ=posgroup[11].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QString::fromLocal8Bit("项的RZ2项参数数据格式错误");
        return 1;
    }
    return 0;
}

int my_cmd::de_QString(QString parakey,QString msg,int data_fpos,int data_bpos,QString &QStringdata,QString &return_msg)
{
    QString paramdata=msg.mid(data_fpos+1,data_bpos-data_fpos-1);
    if(paramdata.size()==0)
    {
        return_msg=parakey+QString::fromLocal8Bit("项参数数据为空");
        return 1;
    }
    QStringdata=paramdata;//获取到值
    if(QStringdata.size()==0)
    {
        return_msg=parakey+QString::fromLocal8Bit("项参数数据格式错误");
        return 1;
    }
    if(QStringdata.indexOf(",")!=-1)
    {
        return_msg=parakey+QString::fromLocal8Bit("','符号是保留字符，不能用于自定义字符");
        return 1;
    }
    return 0;
}

int my_cmd::de_vector_QString(QString parakey,QString msg,int data_fpos,int data_bpos,std::vector<QString> &vector_QStringdata,QString &return_msg)
{
    QString paramdata=msg.mid(data_fpos+1,data_bpos-data_fpos-1);
    if(paramdata.size()==0)
    {
        return_msg=parakey+QString::fromLocal8Bit("项参数数据为空");
        return 1;
    }
    QStringList posgroup = paramdata.split(",");
    vector_QStringdata.resize(posgroup.size());
    for(int n=0;n<posgroup.size();n++)
    {
        vector_QStringdata[n]=posgroup[n];
    }
    return 0;
}
