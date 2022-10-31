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

int my_cmd::decodecmd(QString msg,QString &return_msg)
{
    if(msg.isEmpty())
    {
        return_msg=QString::fromLocal8Bit("指令为空");
        return 1;
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
        int tcp;//获取到的TCP
        RobPos pos;//获取到移动坐标
        float speed;//获取到速度值
        Robmovemodel movemod;//获取到的移动模式

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
                        if(0!=de_float(paramname,param[n],data_fpos,data_bpos,speed,return_msg))
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
                        if(0!=de_robpos(paramname,param[n],data_fpos,data_bpos,pos,return_msg))
                        {
                            return 1;
                        }
                        movemod=MOVEL;
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
                        if(0!=de_robpos(paramname,param[n],data_fpos,data_bpos,pos,return_msg))
                        {
                            return 1;
                        }
                        movemod=MOVEJ;
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
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,tcp,return_msg))
                        {
                            return 1;
                        }
                        if(tcp<0||tcp>=ROBOTTCPNUM)
                        {
                            return_msg=("TCP的值超出设置范围");
                            return 1;
                        }
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
            return_msg=key+QString::fromLocal8Bit("指令还需要设置'")+CMD_MOVL+"'或'"+CMD_MOVJ+QString::fromLocal8Bit("'项参数");
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
        int time;//获取到延时值
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
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,time,return_msg))
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
        int work;//获取到值
        int tasknum;//获取到任务号值
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
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,work,return_msg))
                        {
                            return 1;
                        }
                        if(work!=1&&work!=0)
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
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,tasknum,return_msg))
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
            if(work==1)
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
        int work;//获取到值
        Alternatingcurrent elem;//获取到的交变电流
        float eled;//获取到电流值
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
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,work,return_msg))
                        {
                            return 1;
                        }
                        if(work!=1&&work!=0)
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
                        elem=(Alternatingcurrent)data;
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
                        if(0!=de_float(paramname,param[n],data_fpos,data_bpos,eled,return_msg))
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
            if(work==1)
            {
                return_msg=key+QString::fromLocal8Bit("指令在")+CMD_WORK+QString::fromLocal8Bit("值等于1时还需要设置'")+CMD_ELEM+QString::fromLocal8Bit("'项参数");
                return 1;
            }
        }
        else if(b_ELED==false)
        {
            if(work==1)
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
        int tcp;//获取到的TCP
        RobPos pos;//获取到扫描坐标
        float speed;//获取到速度值
        Robmovemodel movemod;//获取到的移动模式

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
                        if(0!=de_float(paramname,param[n],data_fpos,data_bpos,speed,return_msg))
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
                        if(0!=de_robpos(paramname,param[n],data_fpos,data_bpos,pos,return_msg))
                        {
                            return 1;
                        }
                        movemod=MOVEL;
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
                        if(0!=de_robpos(paramname,param[n],data_fpos,data_bpos,pos,return_msg))
                        {
                            return 1;
                        }
                        movemod=MOVEJ;
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
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,tcp,return_msg))
                        {
                            return 1;
                        }
                        if(tcp<0||tcp>=ROBOTTCPNUM)
                        {
                            return_msg=("TCP的值超出设置范围");
                            return 1;
                        }
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
            return_msg=key+QString::fromLocal8Bit("指令还需要设置'")+CMD_MOVL+"'或'"+CMD_MOVJ+QString::fromLocal8Bit("'项参数");
            return 1;
        }
        else if(b_TCP==false)
        {
            return_msg=key+QString::fromLocal8Bit("指令还需要设置'")+CMD_TCP+QString::fromLocal8Bit("'项参数");
            return 1;
        }
    }
    else if(key==CMD_TRACE_KEY)
    {
        int pn=0;
        bool b_SPEED=false;
        bool b_ROUTE=false;
        bool b_TCP=false;
        int tcp;//获取到的TCP
        int route;//获取到
        float speed;//获取到跟踪速度值
        Robmovemodel movemod;//获取到的移动模式

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
                        if(0!=de_float(paramname,param[n],data_fpos,data_bpos,speed,return_msg))
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
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,tcp,return_msg))
                        {
                            return 1;
                        }
                        if(tcp<0||tcp>=ROBOTTCPNUM)
                        {
                            return_msg=("TCP的值超出设置范围");
                            return 1;
                        }
                    }
                }
                else if(paramname==CMD_ROUTE)
                {
                    if(b_ROUTE==false)
                    {
                        b_ROUTE=true;
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,route,return_msg))
                        {
                            return 1;
                        }
                        if(route<0)//这里还要添加判断上限
                        {
                            return_msg=("轨迹的选取超出设置范围");
                            return 1;
                        }
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
        else if(b_ROUTE==false)
        {
            return_msg=key+QString::fromLocal8Bit("指令还需要设置'")+CMD_ROUTE+QString::fromLocal8Bit("'项参数");
            return 1;
        }
        else if(b_TCP==false)
        {
            return_msg=key+QString::fromLocal8Bit("指令还需要设置'")+CMD_TCP+QString::fromLocal8Bit("'项参数");
            return 1;
        }
    }
    else
    {
        return_msg=QString::fromLocal8Bit("指令集中没有'")+key+QString::fromLocal8Bit("'类型的指令，请查看支持的指令表");
        return 1;
    }


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
    msg=QString(CMD_ROUTE)+"["+QString::number(route)+"]";
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
