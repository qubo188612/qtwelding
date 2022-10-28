#include "my_cmd.h"

my_cmd::my_cmd()
{

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

QString my_cmd::cmd_elec(float eled,int elem,int work)
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

QString my_cmd::cmd_scan(RobPos pos,float speed,int tcp)
{
    QString msg;
    QString msg1;

    msg=QString(CMD_SCAN_KEY)+" "+
            rc_move(pos,MOVEL)+" "+
            rc_speed(speed)+" "+
            rc_tcp(tcp);
    return msg;
}

QString my_cmd::cmd_trace(int route,float speed,int tcp)
{
    QString msg;
    QString msg1;

    msg=QString(CMD_TRACE_KEY)+" "+
            rc_route(route)+" "+
            rc_speed(speed)+" "+
            rc_tcp(tcp);
    return msg;
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
    msg=msg1+"["+QString::number(pos.X,'f',3)+","+
                 QString::number(pos.Y,'f',3)+","+
                 QString::number(pos.Z,'f',3)+","+
                 QString::number(pos.RX,'f',3)+","+
                 QString::number(pos.RY,'f',3)+","+
                 QString::number(pos.RZ,'f',3)+"]";
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

QString my_cmd::rc_elem(int elem)
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

QString my_cmd::rc_route(int route)
{
    QString msg;
    msg=QString(CMD_ROUTE)+"["+QString::number(route,'f',3)+"]";
    return msg;
}
