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
    cmd_creat_mode=TRACE_EDIT_MODE_ONE_TO_ONE;//获取到轨迹生成模式
    cmd_search_speed=0;//获取到的寻位速度
    cmd_search_tcp=0;//获取到寻位TCP
    cmd_search_movemod=MOVEL;//获取到的寻位模式
    cmd_search_side=0;//寻位寻找两侧的范围
    cmd_search_sidespeed=0;//寻位寻找两侧的空闲移动速度
    cmd_getpos_time=0;//获取坐标时的等待时间
    cmd_traceadd_samplelink=false;//是否要在连接处采样
    cmd_traceadd_samplespeed=0;//获取到的连接处的采样点移动速度mm/s
    cmd_traceadd_speed=0;//获取到的连接处的焊接移动速度mm/s
    cmd_traceadd_time=0;//获取到的连接处的采样点之间的时间间隔ms
}

QString my_cmd::cmd_move(RobPos pos,Robmovemodel movemodel,float speed,int tcp,QString change)
{
    QString msg;
    QString msg1;

    msg=QString(CMD_MOV_KEY)+" "+
            rc_move(pos,movemodel)+" "+
            rc_speed(speed)+" "+
            rc_tcp(tcp);
    if(!change.isEmpty())
    {
        msg=msg+" "+rc_change(change);
    }
    return msg;
}

QString my_cmd::cmd_moveC(RobPos pos1,RobPos pos2,RobPos pos3,Robmovemodel movemodel,float speed,int tcp,QString change)
{
    QString msg;
    QString msg1;

    msg=QString(CMD_MOV_KEY)+" "+
            rc_moveC(pos1,pos2,pos3,movemodel)+" "+
            rc_speed(speed)+" "+
            rc_tcp(tcp);
    if(!change.isEmpty())
    {
        msg=msg+" "+rc_change(change);
    }
    return msg;
}

QString my_cmd::cmd_smove(QString s_pos,Robmovemodel movemodel,float speed,int tcp,QString change)
{
    QString msg;
    QString msg1;

    msg=QString(CMD_SMOV_KEY)+" "+
            rc_smove(s_pos,movemodel)+" "+
            rc_speed(speed)+" "+
            rc_tcp(tcp);
    if(!change.isEmpty())
    {
        msg=msg+" "+rc_change(change);
    }
    return msg;
}

QString my_cmd::cmd_smoveC(QString s_pos1,QString s_pos2,QString s_pos3,Robmovemodel movemodel,float speed,int tcp,QString change)
{
    QString msg;
    QString msg1;

    msg=QString(CMD_SMOV_KEY)+" "+
            rc_smoveC(s_pos1,s_pos2,s_pos3,movemodel)+" "+
            rc_speed(speed)+" "+
            rc_tcp(tcp);
    if(!change.isEmpty())
    {
        msg=msg+" "+rc_change(change);
    }
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

QString my_cmd::cmd_elec(float eled,float elev,Alternatingcurrent elem,int work)
{
    QString msg;
    msg=QString(CMD_WELD_KEY)+" "+
            rc_eled(eled)+" "+
            rc_elev(elev)+" "+
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

QString my_cmd::cmd_scan(RobPos pos,Robmovemodel movemodel,float speed,int tcp,QString name,QString change)
{
    QString msg;
    QString msg1;

    msg=QString(CMD_SCAN_KEY)+" "+
            rc_move(pos,movemodel)+" "+
            rc_speed(speed)+" "+
            rc_tcp(tcp)+" "+
            rc_name(name);
    if(!change.isEmpty())
    {
        msg=msg+" "+rc_change(change);
    }
    return msg;
}

QString my_cmd::cmd_scanC(RobPos pos1,RobPos pos2,RobPos pos3,Robmovemodel movemodel,float speed,int tcp,QString name,QString change)
{
    QString msg;
    QString msg1;

    msg=QString(CMD_SCAN_KEY)+" "+
            rc_moveC(pos1,pos2,pos3,movemodel)+" "+
            rc_speed(speed)+" "+
            rc_tcp(tcp)+" "+
            rc_name(name);
    if(!change.isEmpty())
    {
        msg=msg+" "+rc_change(change);
    }
    return msg;
}

QString my_cmd::cmd_sscan(QString s_pos,Robmovemodel movemodel,float speed,int tcp,QString name,QString change)
{
    QString msg;
    QString msg1;

    msg=QString(CMD_SSCAN_KEY)+" "+
            rc_smove(s_pos,movemodel)+" "+
            rc_speed(speed)+" "+
            rc_tcp(tcp)+" "+
            rc_name(name);
    if(!change.isEmpty())
    {
        msg=msg+" "+rc_change(change);
    }
    return msg;
}

QString my_cmd::cmd_sscanC(QString s_pos1,QString s_pos2,QString s_pos3,Robmovemodel movemodel,float speed,int tcp,QString name,QString change)
{
    QString msg;
    QString msg1;

    msg=QString(CMD_SSCAN_KEY)+" "+
            rc_smoveC(s_pos1,s_pos2,s_pos3,movemodel)+" "+
            rc_speed(speed)+" "+
            rc_tcp(tcp)+" "+
            rc_name(name);
    if(!change.isEmpty())
    {
        msg=msg+" "+rc_change(change);
    }
    return msg;
}

QString my_cmd::cmd_search(RobPos pos,Robmovemodel movemodel,float speed,int tcp,int side,std::vector<float> sidemove,float sidespeed,QString name,QString change)
{
    QString msg;
    QString msg1;

    msg=QString(CMD_SEARCH_KEY)+" "+
            rc_move(pos,movemodel)+" "+
            rc_speed(speed)+" "+
            rc_tcp(tcp)+" "+
            rc_side(side)+" "+
            rc_sidemove(sidemove)+" "+
            rc_sidespeed(sidespeed)+" "+
            rc_name(name);
    if(!change.isEmpty())
    {
        msg=msg+" "+rc_change(change);
    }
    return msg;
}

QString my_cmd::cmd_searchC(RobPos pos1,RobPos pos2,RobPos pos3,Robmovemodel movemodel,float speed,int tcp,int side,std::vector<float> sidemove,float sidespeed,QString name,QString change)
{
    QString msg;
    QString msg1;

    msg=QString(CMD_SEARCH_KEY)+" "+
            rc_moveC(pos1,pos2,pos3,movemodel)+" "+
            rc_speed(speed)+" "+
            rc_tcp(tcp)+" "+
            rc_side(side)+" "+
            rc_sidemove(sidemove)+" "+
            rc_sidespeed(sidespeed)+" "+
            rc_name(name);
    if(!change.isEmpty())
    {
        msg=msg+" "+rc_change(change);
    }
    return msg;
}

QString my_cmd::cmd_searchend(RobPos pos,Robmovemodel movemodel,float speed,int tcp,QString name,QString change)
{
    QString msg;
    QString msg1;

    msg=QString(CMD_SEARCHEND_KEY)+" "+
            rc_move(pos,movemodel)+" "+
            rc_speed(speed)+" "+
            rc_tcp(tcp)+" "+
            rc_name(name);
    if(!change.isEmpty())
    {
        msg=msg+" "+rc_change(change);
    }
    return msg;
}

QString my_cmd::cmd_searchendC(RobPos pos1,RobPos pos2,RobPos pos3,Robmovemodel movemodel,float speed,int tcp,QString name,QString change)
{
    QString msg;
    QString msg1;

    msg=QString(CMD_SEARCHEND_KEY)+" "+
            rc_moveC(pos1,pos2,pos3,movemodel)+" "+
            rc_speed(speed)+" "+
            rc_tcp(tcp)+" "+
            rc_name(name);
    if(!change.isEmpty())
    {
        msg=msg+" "+rc_change(change);
    }
    return msg;
}

QString my_cmd::cmd_trace(QString name_in,float speed,QString craftfilepath,QString name_out,QString change)
{
    QString msg;

    msg=QString(CMD_TRACE_KEY)+" "+
            rc_creat(name_in)+" "+
            rc_speed(speed)+" "+
            rc_craft(craftfilepath)+" "+
            rc_name(name_out);
    if(!change.isEmpty())
    {
        msg=msg+" "+rc_change(change);
    }
    return msg;
}

QString my_cmd::cmd_trace2(QString name_in,float speed,QString crafts,QString name_out,QString change)
{
    QString msg;

    msg=QString(CMD_TRACE2_KEY)+" "+
          rc_creat(name_in)+" "+
          rc_speed(speed)+" "+
          rc_crafts(crafts)+" "+
          rc_name(name_out);
    if(!change.isEmpty())
    {
        msg=msg+" "+rc_change(change);
    }
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

QString my_cmd::cmd_ioout(std::vector<int> io)
{
    QString msg;
    msg=QString(CMD_IO_KEY)+" "+
        rc_ioout(io);
    return msg;
}

QString my_cmd::cmd_iowaitin(std::vector<int> io)
{
    QString msg;
    msg=QString(CMD_IO_KEY)+" "+
        rc_iowaitin(io);
    return msg;
}

QString my_cmd::cmd_plcwait(int register_add,int16_t register_data)
{
    QString msg;
    msg=QString(CMD_PLC_KEY)+" "+
        rc_plcwait(register_add)+" "+
        rc_16data(register_data);
    return msg;
}

QString my_cmd::cmd_plcwrite(int register_add,int16_t register_data)
{
    QString msg;
    msg=QString(CMD_PLC_KEY)+" "+
        rc_plcwrite(register_add)+" "+
        rc_16data(register_data);
    return msg;
}

QString my_cmd::cmd_coord(QString s_pointX,QString s_pointO,QString s_cpointX,QString s_cpointO,QString name)
{
    QString msg;
    msg=QString(CMD_COORD_KEY)+" "+
        rc_pointX(s_pointX)+" "+
        rc_pointO(s_pointO)+" "+
        rc_cpointX(s_cpointX)+" "+
        rc_cpointO(s_cpointO)+" "+
        rc_name(name);
    return msg;
}

QString my_cmd::cmd_getpos(int time,QString name,std::vector<float> add)
{
    QString msg;
    msg=QString(CMD_GETPOS_KEY)+" "+
        rc_time(time)+" "+
        rc_name(name)+" "+
        rc_add(add);
    return msg;
}

QString my_cmd::cmd_sample(QString name_in,float speed,int time,QString name_out)
{
    QString msg;
    msg=QString(CMD_SAMPLE_KEY)+" "+
        rc_creat(name_in)+" "+
        rc_speed(speed)+" "+
        rc_time(time)+" "+
        rc_name(name_out);
    return msg;
}

QString my_cmd::cmd_traceadd(QString name1,QString name2,QString name_out,bool b_link,float linkspeed,float linksamplespeed,int linktime)
{
    QString msg;
    if(b_link==false)
    {
        msg=QString(CMD_TRACEADD_KEY)+" "+
            rc_traceadd(name1,name2)+" "+
            rc_name(name_out);
    }
    else
    {
        msg=QString(CMD_TRACEADD_KEY)+" "+
            rc_traceadd(name1,name2)+" "+
            rc_speed(linkspeed)+" "+
            rc_samplespeed(linksamplespeed)+" "+
            rc_time(linktime)+" "+
            rc_name(name_out);
    }
    return msg;
}

QString my_cmd::cmd_tracing(QString name,int tcp,int time)
{
    QString msg;
    msg=QString(CMD_TRACING_KEY)+" "+
        rc_name(name)+" "+
        rc_tcp(tcp)+" "+
        rc_time(time);
    return msg;
}

QString my_cmd::cmd_goweld(int tcp,float speed,QString name)
{
    QString msg;
    msg=QString(CMD_GOWELD_KEY)+" "+
        rc_tcp(tcp)+" "+
        rc_speed(speed)+" "+
        rc_name(name);
    return msg;
}

QString my_cmd::cmd_wave(QString name_in,wWAVEParam cmd_wave_info,QString name_out)
{
    QString msg;
    msg=QString(CMD_WAVE_KEY)+" "+
        rc_trace(name_in)+" "+
        rc_wave(cmd_wave_info)+" "+
        rc_name(name_out);
    return msg;
}

QString my_cmd::cmd_aout(std::vector<float> a)
{
    QString msg;
    msg=QString(CMD_AOUT_KEY)+" "+
        rc_aout(a);
    return msg;
}

QString my_cmd::cmd_creatp(std::vector<QString> pointsname,QString name)
{
    QString msg;
    msg=QString(CMD_CREATP_KEY)+" "+
        rc_points(pointsname)+" "+
        rc_name(name);
    return msg;
}

QString my_cmd::cmd_setpose(QString name_in,std::vector<float> pose,std::vector<float> add,QString name_out)
{
    QString msg;
    msg=QString(CMD_SETPOSE_KEY)+" "+
        rc_point(name_in)+" "+
        rc_pose(pose)+" "+
        rc_add(add)+" "+
        rc_name(name_out);
    return msg;
}

QString my_cmd::cmd_gettcppos(QString name,std::vector<float> add)
{
    QString msg;
    msg=QString(CMD_GETTCPPOS_KEY)+" "+
        rc_name(name)+" "+
        rc_add(add);
    return msg;
}

QString my_cmd::cmd_creatf(QString filename,QString name)
{
    QString msg;
    msg=QString(CMD_CREATF_KEY)+" "+
        rc_file(filename)+" "+
        rc_name(name);
    return msg;
}

QString my_cmd::cmd_plotpos(Plotpos_edit_mode mode,std::vector<QString> weldname,std::vector<QString> pointname,QString posname)
{
    QString msg;
    msg=QString(CMD_PLOTPOS_KEY)+" "+
        rc_mode(mode)+" "+
        rc_plot(mode,weldname,pointname)+" "+
        rc_name(posname);
    return msg;
}

QString my_cmd::cmd_creatc(std::vector<QString> pointsname,float speed,int time,QString name)
{
    QString msg;
    msg=QString(CMD_CREATC_KEY)+" "+
        rc_points(pointsname)+" "+
        rc_speed(speed)+" "+
        rc_time(time)+" "+
        rc_name(name);
    return msg;
}

QString my_cmd::cmd_gettcppos2(RobPos pos,QString name)
{
    QString msg;
    msg=QString(CMD_GETTCPPOS2_KEY)+" "+
        rc_pos(pos)+" "+
        rc_name(name);
    return msg;
}

QString my_cmd::cmd_tracecontinue(QString name_in,RobPos pos,QString name_out)
{
    QString msg;
    msg=QString(CMD_TRACECONTINUE_KEY)+" "+
        rc_trace(name_in)+" "+
        rc_pos(pos)+" "+
        rc_name(name_out);
    return msg;
}

QString my_cmd::cmd_filter(QString name_in,Filter_mode mode,filterParam filters,QString name_out)
{
    QString msg;
    msg=QString(CMD_FILTER_KEY)+" "+
        rc_creat(name_in)+" "+
        rc_mode(mode)+" "+
        rc_filters(filters,mode)+" "+
        rc_name(name_out);
    return msg;
}

QString my_cmd::cmd_goto(int line)
{
    QString msg;
    msg=QString(CMD_GOTO_KEY)+" "+
        rc_line(line);
    return msg;
}

QString my_cmd::cmd_stop()
{
    QString msg;
    msg=QString(CMD_STOP_KEY);
    return msg;
}

QString my_cmd::cmd_creatadd(std::vector<QString> names,QString name_out)
{
    QString msg;
    msg=QString(CMD_CREATADD_KEY)+" "+
        rc_creats(names)+" "+
        rc_name(name_out);
    return msg;
}

QString my_cmd::cmd_creataddp(QString weldname,QString pointname,Creataddp_edit_mode mode,QString name_out)
{
    QString msg;
    msg=QString(CMD_CREATADDP_KEY)+" "+
        rc_creat(weldname)+" "+
        rc_point(pointname)+" "+
        rc_mode(mode)+" "+
        rc_name(name_out);
    return msg;
}

QString my_cmd::cmd_crafts(Craft_ID craft_id,std::vector<ChangeRobPosVariable> posturelist,std::vector<float> params,QString name)
{
    QString msg;
    if(params.size()>0)
    {
        msg=QString(CMD_CRAFTS_KEY)+" "+
              rc_mode(craft_id)+" "+
              rc_pointadds(posturelist)+" "+
              rc_f32datas(params)+" "+
              rc_name(name);
    }
    else
    {
        msg=QString(CMD_CRAFTS_KEY)+" "+
              rc_mode(craft_id)+" "+
              rc_pointadds(posturelist)+" "+
              rc_name(name);
    }
    return msg;
}

QString my_cmd::cmd_savepcd(std::vector<QString> scanname,std::vector<QString> pointsname,std::vector<QString> creatsname,std::vector<QString> tracesname,Savepcd_edit_mode mode,QString name)
{
    QString msg;
    msg=QString(CMD_SAVEPCD_KEY)+" ";
    if(scanname.size()>0)
    {
        msg=msg+rc_scan(scanname)+" ";
    }
    if(pointsname.size()>0)
    {
        msg=msg+rc_points(pointsname)+" ";
    }
    if(creatsname.size()>0)
    {
        msg=msg+rc_creats(creatsname)+" ";
    }
    if(tracesname.size()>0)
    {
        msg=msg+rc_traces(tracesname)+" ";
    }
    msg=msg+rc_mode(mode)+" "+
            rc_name(name);
    return msg;
}

int my_cmd::getkey(QString msg,QString &return_msg,QString &return_key)
{
    if(msg.isEmpty())
    {
        return_msg=QStringLiteral("指令为空");
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
        return_msg=QStringLiteral("指令中只能出现一个':'");
        return 1;
    }
    QString key=list[0]+":";
    return_key=key;
    return 0;
}

int my_cmd::decodecmd(QString msg,QString &return_msg,QString &return_key)
{
    if(msg.isEmpty())
    {
        return_msg=QStringLiteral("指令为空");
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
        return_msg=QStringLiteral("指令中只能出现一个':'");
        return 1;
    }
    QString key=list[0]+":";
    if(key==CMD_MOV_KEY)
    {
        int pn=0;
        bool b_SPEED=false;
        bool b_MOVE=false;
        bool b_TCP=false;
        bool b_CHANGE=false;

        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
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
                        if(cmd_move_speed<=0)
                        {
                            return_msg=paramname+QStringLiteral("项参数只能大于0");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
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
                        return_msg=QStringLiteral("只能有一个MOV类型项");
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
                        return_msg=QStringLiteral("只能有一个MOV类型项");
                        return 1;
                    }
                }
                else if(paramname==CMD_MOVC)
                {
                    if(b_MOVE==false)
                    {
                        b_MOVE=true;
                        if(0!=de_robposP(paramname,param[n],data_fpos,data_bpos,cmd_move_pos1,cmd_move_pos2,cmd_move_pos3,return_msg))
                        {
                            return 1;
                        }
                        cmd_move_movemod=MOVEC;
                    }
                    else
                    {
                        return_msg=QStringLiteral("只能有一个MOV类型项");
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
                            return_msg=QStringLiteral("TCP的值超出设置范围");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_CHANGE)
                {
                    if(b_CHANGE==false)
                    {
                        b_CHANGE=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_move_change,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_SPEED==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_SPEED+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_MOVE==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_MOVL+QStringLiteral("'或'")+CMD_MOVJ+QStringLiteral("'或'")+CMD_MOVC+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_TCP==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_TCP+QStringLiteral("'项参数");
            return 1;
        }

        if(b_CHANGE==false)
        {
            cmd_move_change.clear();
        }
    }
    else if(key==CMD_SMOV_KEY)
    {
        int pn=0;
        bool b_SPEED=false;
        bool b_MOVE=false;
        bool b_TCP=false;
        bool b_CHANGE=false;

        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
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
                        if(0!=de_float(paramname,param[n],data_fpos,data_bpos,cmd_smove_speed,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_smove_speed<=0)
                        {
                            return_msg=paramname+QStringLiteral("项参数只能大于0");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_SMOVL)
                {
                    if(b_MOVE==false)
                    {
                        b_MOVE=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_smove_pos,return_msg))
                        {
                            return 1;
                        }
                        cmd_smove_movemod=MOVEL;
                    }
                    else
                    {
                        return_msg=QStringLiteral("只能有一个SMOV类型项");
                        return 1;
                    }
                }
                else if(paramname==CMD_SMOVJ)
                {
                    if(b_MOVE==false)
                    {
                        b_MOVE=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_smove_pos,return_msg))
                        {
                            return 1;
                        }
                        cmd_smove_movemod=MOVEJ;
                    }
                    else
                    {
                        return_msg=QStringLiteral("只能有一个SMOV类型项");
                        return 1;
                    }
                }
                else if(paramname==CMD_SMOVC)
                {
                    if(b_MOVE==false)
                    {
                        std::vector<QString> posgroup;
                        b_MOVE=true;
                        if(0!=de_vector_QString(paramname,param[n],data_fpos,data_bpos,posgroup,return_msg))
                        {
                            return 1;
                        }
                        if(posgroup.size()!=3)
                        {
                            return_msg=paramname+QStringLiteral("项参数有且只有3个");
                            return 1;
                        }
                        cmd_smove_pos1=posgroup[0];
                        cmd_smove_pos2=posgroup[1];
                        cmd_smove_pos3=posgroup[2];
                        cmd_smove_movemod=MOVEC;
                    }
                    else
                    {
                        return_msg=QStringLiteral("只能有一个SMOV类型项");
                        return 1;
                    }
                }
                else if(paramname==CMD_TCP)
                {
                    if(b_TCP==false)
                    {
                        b_TCP=true;
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,cmd_smove_tcp,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_smove_tcp<0||cmd_smove_tcp>=ROBOTTCPNUM)
                        {
                            return_msg=QStringLiteral("TCP的值超出设置范围");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_CHANGE)
                {
                    if(b_CHANGE==false)
                    {
                        b_CHANGE=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_smove_change,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_SPEED==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_SPEED+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_MOVE==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_SMOVL+QStringLiteral("'或'")+CMD_SMOVJ+QStringLiteral("'或'")+CMD_SMOVC+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_TCP==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_TCP+QStringLiteral("'项参数");
            return 1;
        }

        if(b_CHANGE==false)
        {
            cmd_smove_change.clear();
        }
    }
    else if(key==CMD_DELAY_KEY)
    {
        int pn=0;
        bool b_TIME=false;
        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
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
                        if(cmd_delay_time<0)
                        {
                            return_msg=paramname+QStringLiteral("项参数只能大于等于0");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_TIME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_TIME+QStringLiteral("'项参数");
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
            if(param[n].size()!=0)
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
                            return_msg=paramname+QStringLiteral("项参数只能为0或1");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
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
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_WORK==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_WORK+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_TASK==false)
        {
            if(cmd_cam_work_d==1)
            {
                return_msg=key+QStringLiteral("指令在")+CMD_WORK+QStringLiteral("值等于1时还需要设置'")+CMD_TASK+QStringLiteral("'项参数");
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
        bool b_ELEV=false;
        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
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
                            return_msg=paramname+QStringLiteral("项参数只能为0或1");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
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
                        return_msg=paramname+QStringLiteral("项参数重复设置");
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
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_ELEV)
                {
                    if(b_ELEV==false)
                    {
                        b_ELEV=true;
                        if(0!=de_float(paramname,param[n],data_fpos,data_bpos,cmd_elec_elev,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_WORK==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_WORK+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_ELEM==false)
        {
            if(cmd_elec_work_d==1)
            {
                return_msg=key+QStringLiteral("指令在")+CMD_WORK+QStringLiteral("值等于1时还需要设置'")+CMD_ELEM+QStringLiteral("'项参数");
                return 1;
            }
        }
        else if(b_ELED==false)
        {
            if(cmd_elec_work_d==1)
            {
                return_msg=key+QStringLiteral("指令在")+CMD_WORK+QStringLiteral("值等于1时还需要设置'")+CMD_ELED+QStringLiteral("'项参数");
                return 1;
            }
        }
        else if(b_ELEV==false)
        {
            if(cmd_elec_work_d==1)
            {
                return_msg=key+QStringLiteral("指令在")+CMD_WORK+QStringLiteral("值等于1时还需要设置'")+CMD_ELEV+QStringLiteral("'项参数");
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
        bool b_CHANGE=false;
        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
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
                        if(cmd_scan_speed<=0)
                        {
                            return_msg=paramname+QStringLiteral("项参数只能大于0");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
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
                        return_msg=QStringLiteral("只能有一个MOV类型项");
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
                        return_msg=QStringLiteral("只能有一个MOV类型项");
                        return 1;
                    }
                }
                else if(paramname==CMD_MOVC)
                {
                    if(b_MOVE==false)
                    {
                        b_MOVE=true;
                        if(0!=de_robposP(paramname,param[n],data_fpos,data_bpos,cmd_scan_pos1,cmd_scan_pos2,cmd_scan_pos3,return_msg))
                        {
                            return 1;
                        }
                        cmd_scan_movemod=MOVEC;
                    }
                    else
                    {
                        return_msg=QStringLiteral("只能有一个MOV类型项");
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
                            return_msg=QStringLiteral("TCP的值超出设置范围");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
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
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_CHANGE)
                {
                    if(b_CHANGE==false)
                    {
                        b_CHANGE=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_scan_change,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_SPEED==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_SPEED+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_MOVE==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_MOVL+QStringLiteral("'或'")+CMD_MOVJ+QStringLiteral("'或'")+CMD_MOVC+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_TCP==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_TCP+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_NAME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_NAME+QStringLiteral("'项参数");
            return 1;
        }

        if(b_CHANGE==false)
        {
            cmd_scan_change.clear();
        }
    }
    else if(key==CMD_SSCAN_KEY)
    {
        int pn=0;
        bool b_SPEED=false;
        bool b_MOVE=false;
        bool b_TCP=false;
        bool b_NAME=false;
        bool b_CHANGE=false;
        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
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
                        if(0!=de_float(paramname,param[n],data_fpos,data_bpos,cmd_sscan_speed,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_sscan_speed<=0)
                        {
                            return_msg=paramname+QStringLiteral("项参数只能大于0");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_SMOVL)
                {
                    if(b_MOVE==false)
                    {
                        b_MOVE=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_sscan_pos,return_msg))
                        {
                            return 1;
                        }
                        cmd_sscan_movemod=MOVEL;
                    }
                    else
                    {
                        return_msg=QStringLiteral("只能有一个MOV类型项");
                        return 1;
                    }
                }
                else if(paramname==CMD_SMOVJ)
                {
                    if(b_MOVE==false)
                    {
                        b_MOVE=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_sscan_pos,return_msg))
                        {
                            return 1;
                        }
                        cmd_sscan_movemod=MOVEJ;
                    }
                    else
                    {
                        return_msg=QStringLiteral("只能有一个MOV类型项");
                        return 1;
                    }
                }
                else if(paramname==CMD_SMOVC)
                {
                    if(b_MOVE==false)
                    {
                        std::vector<QString> posgroup;
                        b_MOVE=true;
                        if(0!=de_vector_QString(paramname,param[n],data_fpos,data_bpos,posgroup,return_msg))
                        {
                            return 1;
                        }
                        if(posgroup.size()!=3)
                        {
                            return_msg=paramname+QStringLiteral("项参数有且只有3个");
                            return 1;
                        }
                        cmd_sscan_pos1=posgroup[0];
                        cmd_sscan_pos2=posgroup[1];
                        cmd_sscan_pos3=posgroup[2];
                        cmd_sscan_movemod=MOVEC;
                    }
                    else
                    {
                        return_msg=QStringLiteral("只能有一个MOV类型项");
                        return 1;
                    }
                }
                else if(paramname==CMD_TCP)
                {
                    if(b_TCP==false)
                    {
                        b_TCP=true;
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,cmd_sscan_tcp,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_sscan_tcp<0||cmd_sscan_tcp>=ROBOTTCPNUM)
                        {
                            return_msg=QStringLiteral("TCP的值超出设置范围");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_NAME)
                {
                    if(b_NAME==false)
                    {
                        b_NAME=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_sscan_name,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_CHANGE)
                {
                    if(b_CHANGE==false)
                    {
                        b_CHANGE=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_sscan_change,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_SPEED==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_SPEED+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_MOVE==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_SMOVL+QStringLiteral("'或'")+CMD_SMOVJ+QStringLiteral("'或'")+CMD_SMOVC+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_TCP==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_TCP+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_NAME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_NAME+QStringLiteral("'项参数");
            return 1;
        }

        if(b_CHANGE==false)
        {
            cmd_sscan_change.clear();
        }
    }
    else if(key==CMD_TRACE_KEY)
    {
        int pn=0;
        bool b_CREAT=false;
        bool b_SPEED=false;
        bool b_NAME=false;
        bool b_CRAFT=false;
        bool b_CHANGE=false;

        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
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
                        if(cmd_trace_speed<=0)
                        {
                            return_msg=paramname+QStringLiteral("项参数只能大于0");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_CREAT)
                {
                    if(b_CREAT==false)
                    {
                        b_CREAT=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_trace_name_in,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_NAME)
                {
                    if(b_NAME==false)
                    {
                        b_NAME=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_trace_name_out,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
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
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_CHANGE)
                {
                    if(b_CHANGE==false)
                    {
                        b_CHANGE=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_trace_change,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_SPEED==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_SPEED+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_CREAT==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_CREAT+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_NAME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_NAME+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_CRAFT==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_CRAFT+QStringLiteral("'项参数");
            return 1;
        }
        if(b_CHANGE==false)
        {
            cmd_trace_change.clear();
        }
    }
    else if(key==CMD_TRACE2_KEY)
    {
        int pn=0;
        bool b_CREAT=false;
        bool b_SPEED=false;
        bool b_NAME=false;
        bool b_CRAFTS=false;
        bool b_CHANGE=false;

        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
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
                        if(0!=de_float(paramname,param[n],data_fpos,data_bpos,cmd_trace2_speed,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_trace2_speed<=0)
                        {
                            return_msg=paramname+QStringLiteral("项参数只能大于0");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_CREAT)
                {
                    if(b_CREAT==false)
                    {
                        b_CREAT=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_trace2_name_in,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_NAME)
                {
                    if(b_NAME==false)
                    {
                        b_NAME=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_trace2_name_out,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_CRAFTS)
                {
                    if(b_CRAFTS==false)
                    {
                        b_CRAFTS=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_trace2_crafts,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_CHANGE)
                {
                    if(b_CHANGE==false)
                    {
                        b_CHANGE=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_trace2_change,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_SPEED==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_SPEED+QStringLiteral("'项参数");
                         return 1;
        }
        else if(b_CREAT==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_CREAT+QStringLiteral("'项参数");
                         return 1;
        }
        else if(b_NAME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_NAME+QStringLiteral("'项参数");
                         return 1;
        }
        else if(b_CRAFTS==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_CRAFTS+QStringLiteral("'项参数");
                         return 1;
        }
        if(b_CHANGE==false)
        {
            cmd_trace2_change.clear();
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
            if(param[n].size()!=0)
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
                        return_msg=paramname+QStringLiteral("项参数重复设置");
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
                        return_msg=paramname+QStringLiteral("项参数重复设置");
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
                        if(false==b_nosame_vector_QString(cmd_creat_scanname))
                        {
                            return_msg=paramname+QStringLiteral("项参数里不能有同名项");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_NAME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_NAME+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_MODE==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_MODE+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_SCAN==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_SCAN+QStringLiteral("'项参数");
            return 1;
        }
    }
    else if(key==CMD_IO_KEY)
    {
        int pn=0;
        bool b_IO=false;
        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
            {
                QString paramname;
                int data_fpos,data_bpos;
                if(0!=de_param(++pn,param[n],paramname,data_fpos,data_bpos,return_msg))
                {
                    return 1;
                }
                if(paramname==CMD_OUT)
                {
                    if(b_IO==false)
                    {
                        b_IO=true;
                        if(0!=de_vector_int(paramname,param[n],data_fpos,data_bpos,cmd_io_output,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_io_output.size()!=ROBOTOUTPUTNUM)
                        {
                            return_msg=paramname+QStringLiteral("项参数有且只有")+QString::number(ROBOTOUTPUTNUM)+QStringLiteral("个");
                            return 1;
                        }
                        cmd_io_workmod=OUT;
                    }
                    else
                    {
                        return_msg=CMD_OUT+QStringLiteral("项和")+CMD_WAITIN+QStringLiteral("项只能有一个");
                        return 1;
                    }
                }
                else if(paramname==CMD_WAITIN)
                {
                    if(b_IO==false)
                    {
                        b_IO=true;
                        if(0!=de_vector_int(paramname,param[n],data_fpos,data_bpos,cmd_io_input,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_io_input.size()!=ROBOTINPUTNUM)
                        {
                            return_msg=paramname+QStringLiteral("项参数有且只有")+QString::number(ROBOTINPUTNUM)+QStringLiteral("个");
                            return 1;
                        }
                        cmd_io_workmod=WAITIN;
                    }
                    else
                    {
                        return_msg=CMD_OUT+QStringLiteral("项和")+CMD_WAITIN+QStringLiteral("项只能有一个");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_IO==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_OUT+QStringLiteral("'或'")+CMD_WAITIN+QStringLiteral("'项参数");
            return 1;
        }
    }
    else if(key==CMD_PLC_KEY)
    {
        int pn=0;
        bool b_register_add=false;
        bool b_data=false;

        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
            {
                QString paramname;
                int data_fpos,data_bpos;
                if(0!=de_param(++pn,param[n],paramname,data_fpos,data_bpos,return_msg))
                {
                    return 1;
                }
                if(paramname==CMD_WAITPLC)
                {
                    if(b_register_add==false)
                    {
                        b_register_add=true;
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,cmd_plc_register_add,return_msg))
                        {
                            return 1;
                        }
                        cmd_plc_mod=PLC_WAIT;
                    }
                    else
                    {
                        return_msg=CMD_WAITPLC+QStringLiteral("项和")+CMD_WRITEPLC+QStringLiteral("项只能有一个");
                        return 1;
                    }
                }
                else if(paramname==CMD_WRITEPLC)
                {
                    if(b_register_add==false)
                    {
                        b_register_add=true;
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,cmd_plc_register_add,return_msg))
                        {
                            return 1;
                        }
                        cmd_plc_mod=PLC_WRITE;
                    }
                    else
                    {
                        return_msg=CMD_WAITPLC+QStringLiteral("项和")+CMD_WRITEPLC+QStringLiteral("项只能有一个");
                        return 1;
                    }
                }
                else if(paramname==CMD_DATA16)
                {
                    if(b_data==false)
                    {
                        b_data=true;
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,cmd_plc_register_data,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_register_add==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_WRITEPLC+QStringLiteral("'或'")+CMD_WAITPLC+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_data==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_DATA16+QStringLiteral("'项参数");
            return 1;
        }
    }
    else if(key==CMD_SEARCH_KEY)
    {
        int pn=0;
        bool b_SPEED=false;
        bool b_MOVE=false;
        bool b_TCP=false;
        bool b_NAME=false;
        bool b_SIDE=false;
        bool b_SIDEMOVE=false;
        bool b_SIDESPEED=false;
        bool b_CHANGE=false;
        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
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
                        if(0!=de_float(paramname,param[n],data_fpos,data_bpos,cmd_search_speed,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_search_speed<=0)
                        {
                            return_msg=paramname+QStringLiteral("项参数只能大于0");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_MOVL)
                {
                    if(b_MOVE==false)
                    {
                        b_MOVE=true;
                        if(0!=de_robpos(paramname,param[n],data_fpos,data_bpos,cmd_search_pos,return_msg))
                        {
                            return 1;
                        }
                        cmd_search_movemod=MOVEL;
                    }
                    else
                    {
                        return_msg=QStringLiteral("只能有一个MOV类型项");
                        return 1;
                    }
                }
                else if(paramname==CMD_MOVJ)
                {
                    if(b_MOVE==false)
                    {
                        b_MOVE=true;
                        if(0!=de_robpos(paramname,param[n],data_fpos,data_bpos,cmd_search_pos,return_msg))
                        {
                            return 1;
                        }
                        cmd_search_movemod=MOVEJ;
                    }
                    else
                    {
                        return_msg=QStringLiteral("只能有一个MOV类型项");
                        return 1;
                    }
                }
                else if(paramname==CMD_MOVC)
                {
                    if(b_MOVE==false)
                    {
                        b_MOVE=true;
                        if(0!=de_robposP(paramname,param[n],data_fpos,data_bpos,cmd_search_pos1,cmd_search_pos2,cmd_search_pos3,return_msg))
                        {
                            return 1;
                        }
                        cmd_search_movemod=MOVEC;
                    }
                    else
                    {
                        return_msg=QStringLiteral("只能有一个MOV类型项");
                        return 1;
                    }
                }
                else if(paramname==CMD_TCP)
                {
                    if(b_TCP==false)
                    {
                        b_TCP=true;
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,cmd_search_tcp,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_search_tcp<0||cmd_search_tcp>=ROBOTTCPNUM)
                        {
                            return_msg=paramname+QStringLiteral("的值超出设置范围");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_NAME)
                {
                    if(b_NAME==false)
                    {
                        b_NAME=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_search_name,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_CHANGE)
                {
                    if(b_CHANGE==false)
                    {
                        b_CHANGE=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_search_change,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_SIDE)
                {
                    if(b_SIDE==false)
                    {
                        b_SIDE=true;
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,cmd_search_side,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_search_side<0)
                        {
                            return_msg=paramname+QStringLiteral("的值必须大于等于0");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_SIDEMOVE)
                {
                    if(b_SIDEMOVE==false)
                    {
                        b_SIDEMOVE=true;
                        if(0!=de_vector_float(paramname,param[n],data_fpos,data_bpos,cmd_search_sidemove,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_search_sidemove.size()!=3)
                        {
                            return_msg=paramname+QStringLiteral("项只能有3个参数");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_SIDESPEED)
                {
                    if(b_SIDESPEED==false)
                    {
                        b_SIDESPEED=true;
                        if(0!=de_float(paramname,param[n],data_fpos,data_bpos,cmd_search_sidespeed,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_SPEED==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_SPEED+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_MOVE==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_MOVL+QStringLiteral("'或'")+CMD_MOVJ+QStringLiteral("'或'")+CMD_MOVC+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_TCP==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_TCP+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_SIDE==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_SIDE+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_SIDEMOVE==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_SIDEMOVE+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_SIDESPEED==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_SIDESPEED+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_NAME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_NAME+QStringLiteral("'项参数");
            return 1;
        }

        if(b_CHANGE==false)
        {
            cmd_search_change.clear();
        }
    }
    else if(key==CMD_SEARCHEND_KEY)
    {
        int pn=0;
        bool b_SPEED=false;
        bool b_MOVE=false;
        bool b_TCP=false;
        bool b_NAME=false;
        bool b_CHANGE=false;
        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
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
                        if(0!=de_float(paramname,param[n],data_fpos,data_bpos,cmd_searchend_speed,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_searchend_speed<=0)
                        {
                            return_msg=paramname+QStringLiteral("项参数只能大于0");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_MOVL)
                {
                    if(b_MOVE==false)
                    {
                        b_MOVE=true;
                        if(0!=de_robpos(paramname,param[n],data_fpos,data_bpos,cmd_searchend_pos,return_msg))
                        {
                            return 1;
                        }
                        cmd_searchend_movemod=MOVEL;
                    }
                    else
                    {
                        return_msg=QStringLiteral("只能有一个MOV类型项");
                        return 1;
                    }
                }
                else if(paramname==CMD_MOVJ)
                {
                    if(b_MOVE==false)
                    {
                        b_MOVE=true;
                        if(0!=de_robpos(paramname,param[n],data_fpos,data_bpos,cmd_searchend_pos,return_msg))
                        {
                            return 1;
                        }
                        cmd_searchend_movemod=MOVEJ;
                    }
                    else
                    {
                        return_msg=QStringLiteral("只能有一个MOV类型项");
                        return 1;
                    }
                }
                else if(paramname==CMD_MOVC)
                {
                    if(b_MOVE==false)
                    {
                        b_MOVE=true;
                        if(0!=de_robposP(paramname,param[n],data_fpos,data_bpos,cmd_searchend_pos1,cmd_searchend_pos2,cmd_searchend_pos3,return_msg))
                        {
                            return 1;
                        }
                        cmd_searchend_movemod=MOVEC;
                    }
                    else
                    {
                        return_msg=QStringLiteral("只能有一个MOV类型项");
                        return 1;
                    }
                }
                else if(paramname==CMD_TCP)
                {
                    if(b_TCP==false)
                    {
                        b_TCP=true;
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,cmd_searchend_tcp,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_searchend_tcp<0||cmd_searchend_tcp>=ROBOTTCPNUM)
                        {
                            return_msg=paramname+QStringLiteral("的值超出设置范围");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_NAME)
                {
                    if(b_NAME==false)
                    {
                        b_NAME=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_searchend_name,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_CHANGE)
                {
                    if(b_CHANGE==false)
                    {
                        b_CHANGE=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_searchend_change,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }

                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_SPEED==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_SPEED+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_MOVE==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_MOVL+QStringLiteral("'或'")+CMD_MOVJ+QStringLiteral("'或'")+CMD_MOVC+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_TCP==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_TCP+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_NAME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_NAME+QStringLiteral("'项参数");
            return 1;
        }

        if(b_CHANGE==false)
        {
            cmd_search_change.clear();
        }
    }
    else if(key==CMD_COORD_KEY)//定位零点
    {
        int pn=0;
        bool b_POINTX=false;
        bool b_POINTO=false;
        bool b_CPOINTX=false;
        bool b_CPOINTO=false;
        bool b_NAME=false;
        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
            {
                QString paramname;
                int data_fpos,data_bpos;
                if(0!=de_param(++pn,param[n],paramname,data_fpos,data_bpos,return_msg))
                {
                    return 1;
                }
                if(paramname==CMD_POINTX)
                {
                    if(b_POINTX==false)
                    {
                        b_POINTX=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_coord_pointx,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_POINTO)
                {
                    if(b_POINTO==false)
                    {
                        b_POINTO=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_coord_pointo,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_CPOINTX)
                {
                    if(b_CPOINTX==false)
                    {
                        b_CPOINTX=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_coord_cpointx,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_CPOINTO)
                {
                    if(b_CPOINTO==false)
                    {
                        b_CPOINTO=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_coord_cpointo,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_NAME)
                {
                    if(b_NAME==false)
                    {
                        b_NAME=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_coord_name,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_POINTX==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_POINTX+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_POINTO==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_POINTO+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_CPOINTX==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_CPOINTX+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_CPOINTO==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_CPOINTO+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_NAME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_NAME+QStringLiteral("'项参数");
            return 1;
        }
    }
    else if(key==CMD_GETPOS_KEY)
    {
        int pn=0;
        bool b_TIME=false;
        bool b_NAME=false;
        bool b_ADD=false;
        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
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
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,cmd_getpos_time,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_getpos_time<0)
                        {
                            return_msg=paramname+QStringLiteral("项参数只能大于等于0");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_NAME)
                {
                    if(b_NAME==false)
                    {
                        b_NAME=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_getpos_name,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_ADD)
                {
                    if(b_ADD==false)
                    {
                        b_ADD=true;
                        if(0!=de_vector_float(paramname,param[n],data_fpos,data_bpos,cmd_getpos_add,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_getpos_add.size()!=3)
                        {
                            return_msg=paramname+QStringLiteral("项参数有且只有")+QString::number(3)+QStringLiteral("个");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_TIME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_TIME+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_NAME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_NAME+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_ADD==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_ADD+QStringLiteral("'项参数");
            return 1;
        }
    }
    else if(key==CMD_SAMPLE_KEY)
    {
        int pn=0;
        bool b_CREAT=false;
        bool b_SPEED=false;
        bool b_TIME=false;
        bool b_NAME=false;
        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
            {
                QString paramname;
                int data_fpos,data_bpos;
                if(0!=de_param(++pn,param[n],paramname,data_fpos,data_bpos,return_msg))
                {
                    return 1;
                }
                if(paramname==CMD_CREAT)
                {
                    if(b_CREAT==false)
                    {
                        b_CREAT=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_sample_creatname,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_SPEED)
                {
                    if(b_SPEED==false)
                    {
                        b_SPEED=true;
                        if(0!=de_float(paramname,param[n],data_fpos,data_bpos,cmd_sample_speed,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_sample_speed<=0)
                        {
                            return_msg=paramname+QStringLiteral("项参数只能大于0");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_TIME)
                {
                    if(b_TIME==false)
                    {
                        b_TIME=true;
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,cmd_sample_time,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_sample_time<0)
                        {
                            return_msg=paramname+QStringLiteral("项参数只能大于等于0");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_NAME)
                {
                    if(b_NAME==false)
                    {
                        b_NAME=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_sample_name,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_CREAT==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_CREAT+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_TIME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_TIME+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_SPEED==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_SPEED+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_NAME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_NAME+QStringLiteral("'项参数");
            return 1;
        }
        else if(cmd_sample_name==cmd_sample_creatname)
        {
            return_msg=key+"'"+CMD_NAME+QStringLiteral("'项参数和'")+CMD_CREAT+QStringLiteral("'项参数不能同名");
            return 1;
        }
    }
    else if(key==CMD_TRACING_KEY)
    {
        int pn=0;
        bool b_TCP=false;
        bool b_NAME=false;
        bool b_TIME=false;
        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
            {
                QString paramname;
                int data_fpos,data_bpos;
                if(0!=de_param(++pn,param[n],paramname,data_fpos,data_bpos,return_msg))
                {
                    return 1;
                }
                if(paramname==CMD_TCP)
                {
                    if(b_TCP==false)
                    {
                        b_TCP=true;
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,cmd_tracing_tcp,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_tracing_tcp<0||cmd_tracing_tcp>=ROBOTTCPNUM)
                        {
                            return_msg=paramname+QStringLiteral("的值超出设置范围");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_TIME)
                {
                    if(b_TIME==false)
                    {
                        b_TIME=true;
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,cmd_tracing_time,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_tracing_time<0)
                        {
                            return_msg=paramname+QStringLiteral("项参数只能大于等于0");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_NAME)
                {
                    if(b_NAME==false)
                    {
                        b_NAME=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_tracing_name,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_TCP==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_TCP+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_NAME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_NAME+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_TIME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_TIME+QStringLiteral("'项参数");
            return 1;
        }
    }
    else if(key==CMD_TRACEADD_KEY)
    {
        int pn=0;
        bool b_TRACEADD=false;
        bool b_NAME=false;
        bool b_SPEED=false;
        bool b_SAMPLESPEED=false;
        bool b_TIME=false;
        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
            {
                QString paramname;
                int data_fpos,data_bpos;
                if(0!=de_param(++pn,param[n],paramname,data_fpos,data_bpos,return_msg))
                {
                    return 1;
                }
                if(paramname==CMD_TRACEADD)
                {
                    if(b_TRACEADD==false)
                    {
                        std::vector<QString> namegroup;
                        b_TRACEADD=true;
                        if(0!=de_vector_QString(paramname,param[n],data_fpos,data_bpos,namegroup,return_msg))
                        {
                            return 1;
                        }
                        if(namegroup.size()!=2)
                        {
                            return_msg=paramname+QStringLiteral("项参数有且只有2个");
                            return 1;
                        }
                        cmd_traceadd_name1=namegroup[0];
                        cmd_traceadd_name2=namegroup[1];
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_NAME)
                {
                    if(b_NAME==false)
                    {
                        b_NAME=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_traceadd_nameout,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_SPEED)
                {
                    if(b_SPEED==false)
                    {
                        b_SPEED=true;
                        if(0!=de_float(paramname,param[n],data_fpos,data_bpos,cmd_traceadd_speed,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_traceadd_speed<=0)
                        {
                            return_msg=paramname+QStringLiteral("项参数只能大于0");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_SAMPLESPEED)
                {
                    if(b_SAMPLESPEED==false)
                    {
                        b_SAMPLESPEED=true;
                        if(0!=de_float(paramname,param[n],data_fpos,data_bpos,cmd_traceadd_samplespeed,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_traceadd_samplespeed<=0)
                        {
                            return_msg=paramname+QStringLiteral("项参数只能大于0");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_TIME)
                {
                    if(b_TIME==false)
                    {
                        b_TIME=true;
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,cmd_traceadd_time,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_traceadd_time<0)
                        {
                            return_msg=paramname+QStringLiteral("项参数只能大于等于0");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_TRACEADD==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_TRACEADD+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_NAME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_NAME+QStringLiteral("'项参数");
            return 1;
        }

        if(b_TIME==false&&b_SPEED==false&&b_SAMPLESPEED==false)
        {
            cmd_traceadd_samplelink=false;
        }
        else if(b_TIME==true&&b_SPEED==true&&b_SAMPLESPEED==true)
        {
            cmd_traceadd_samplelink=true;
        }
        else if(b_TIME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_TIME+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_SPEED==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_SPEED+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_SAMPLESPEED==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_SAMPLESPEED+QStringLiteral("'项参数");
            return 1;
        }
    }
    else if(key==CMD_GOWELD_KEY)
    {
        int pn=0;
        bool b_SPEED=false;
        bool b_TCP=false;
        bool b_NAME=false;
        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
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
                        if(0!=de_float(paramname,param[n],data_fpos,data_bpos,cmd_goweld_speed,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_goweld_speed<=0)
                        {
                            return_msg=paramname+QStringLiteral("项参数只能大于0");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_TCP)
                {
                    if(b_TCP==false)
                    {
                        b_TCP=true;
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,cmd_goweld_tcp,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_goweld_tcp<0||cmd_goweld_tcp>=ROBOTTCPNUM)
                        {
                            return_msg=paramname+QStringLiteral("的值超出设置范围");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_NAME)
                {
                    if(b_NAME==false)
                    {
                        b_NAME=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_goweld_name,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_SPEED==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_SPEED+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_TCP==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_TCP+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_NAME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_NAME+QStringLiteral("'项参数");
            return 1;
        }
    }
    else if(key==CMD_WAVE_KEY)
    {
        int pn=0;
        bool b_TRACE=false;
        bool b_NAME=false;
        bool b_WAVE=false;
        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
            {
                QString paramname;
                int data_fpos,data_bpos;
                if(0!=de_param(++pn,param[n],paramname,data_fpos,data_bpos,return_msg))
                {
                    return 1;
                }
                if(paramname==CMD_TRACE)
                {
                    if(b_TRACE==false)
                    {
                        b_TRACE=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_wave_namein,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_NAME)
                {
                    if(b_NAME==false)
                    {
                        b_NAME=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_wave_nameout,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_WAVE)
                {
                    if(b_WAVE==false)
                    {
                        std::vector<float> f_datagroup;
                        b_WAVE=true;
                        if(0!=de_vector_float(paramname,param[n],data_fpos,data_bpos,f_datagroup,return_msg))
                        {
                            return 1;
                        }
                        if(f_datagroup.size()!=20)
                        {
                            return_msg=paramname+QStringLiteral("项参数有且只有20个");
                            return 1;
                        }
                        cmd_wave_info.period=f_datagroup[0];
                        cmd_wave_info.leftAmp=f_datagroup[1];
                        cmd_wave_info.rightAmp=f_datagroup[2];
                        cmd_wave_info.leftAmp_z=f_datagroup[3];
                        cmd_wave_info.rightAmp_z=f_datagroup[4];
                        cmd_wave_info.leftStopTime=f_datagroup[5];
                        cmd_wave_info.rightStopTime=f_datagroup[6];
                        cmd_wave_info.anglex=f_datagroup[7];
                        cmd_wave_info.angley=f_datagroup[8];
                        cmd_wave_info.startPos=(int)f_datagroup[9];
                        cmd_wave_info.order=(int)f_datagroup[10];
                        cmd_wave_info.pendulum_mode=(Pendulum_mode)((int)f_datagroup[11]);
                        cmd_wave_info.timeGap=(int)f_datagroup[12];
                        cmd_wave_info.trend_mode=(Trend_mode)((int)f_datagroup[13]);
                        cmd_wave_info.leftAddRX=f_datagroup[14];
                        cmd_wave_info.leftAddRY=f_datagroup[15];
                        cmd_wave_info.leftAddRZ=f_datagroup[16];
                        cmd_wave_info.rightAddRX=f_datagroup[17];
                        cmd_wave_info.rightAddRY=f_datagroup[18];
                        cmd_wave_info.rightAddRZ=f_datagroup[19];

                        if(f_datagroup[0]<=0)
                        {
                            return_msg=paramname+QStringLiteral("的摆焊周期(第1个参数)必须大于0");
                            return 1;
                        }
                        if(f_datagroup[1]<=0)
                        {
                            return_msg=paramname+QStringLiteral("的左摆焊幅度(第2个参数)必须大于0");
                            return 1;
                        }
                        if(f_datagroup[2]<=0)
                        {
                            return_msg=paramname+QStringLiteral("的右摆焊幅度(第3个参数)必须大于0");
                            return 1;
                        }
                        if(f_datagroup[5]<0)
                        {
                            return_msg=paramname+QStringLiteral("的左摆停留时间(第6个参数)必须大于等于0");
                            return 1;
                        }
                        if(f_datagroup[6]<0)
                        {
                            return_msg=paramname+QStringLiteral("的右摆停留时间(第7个参数)必须大于等于0");
                            return 1;
                        }
                        if((int)f_datagroup[9]<0)
                        {
                            return_msg=paramname+QStringLiteral("的起摆位置(第9个参数)必须大于等于0");
                            return 1;
                        }
                        if((int)f_datagroup[10]<0||(int)f_datagroup[10]>1)
                        {
                            return_msg=paramname+QStringLiteral("的起摆位置(第10个参数)超出设置范围");
                            return 1;
                        }
                        if((int)f_datagroup[11]<0||(int)f_datagroup[11]>=PENDULUM_ID_TOTAL_NUM)
                        {
                            return_msg=paramname+QStringLiteral("的摆焊模式(第12个参数)超出设置范围");
                            return 1;
                        }
                        if((int)f_datagroup[12]<=0)
                        {
                            return_msg=paramname+QStringLiteral("的采样时间(第13个参数)必须大于0");
                            return 1;
                        }
                        if((int)f_datagroup[13]<0||(int)f_datagroup[13]>=TREND_ID_TOTAL_NUM)
                        {
                            return_msg=paramname+QStringLiteral("的摆幅方向(第14个参数)超出设置范围");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_TRACE==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_TRACE+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_NAME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_NAME+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_WAVE==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_WAVE+QStringLiteral("'项参数");
            return 1;
        }
    }
    else if(key==CMD_AOUT_KEY)
    {
        int pn=0;
        bool b_AOUT=false;
        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
            {
                QString paramname;
                int data_fpos,data_bpos;
                if(0!=de_param(++pn,param[n],paramname,data_fpos,data_bpos,return_msg))
                {
                    return 1;
                }
                if(paramname==CMD_AOUT)
                {
                    if(b_AOUT==false)
                    {
                        b_AOUT=true;
                        if(0!=de_vector_float(paramname,param[n],data_fpos,data_bpos,cmd_aout_output,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_aout_output.size()!=ROBOTAOUTPUTNUM)
                        {
                            return_msg=paramname+QStringLiteral("项参数有且只有")+QString::number(ROBOTAOUTPUTNUM)+QStringLiteral("个");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_AOUT==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_AOUT+QStringLiteral("'项参数");
            return 1;
        }
    }
    else if(key==CMD_CREATP_KEY)
    {
        int pn=0;
        bool b_NAME=false;
        bool b_POINTS=false;

        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
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
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_creatp_name,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_POINTS)
                {
                    if(b_POINTS==false)
                    {
                        b_POINTS=true;
                        if(0!=de_vector_QString(paramname,param[n],data_fpos,data_bpos,cmd_creatp_pointsname,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_creatp_pointsname.size()<1)
                        {
                            return_msg=paramname+QStringLiteral("项参数个数至少大于等于1个");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_NAME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_NAME+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_POINTS==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_POINTS+QStringLiteral("'项参数");
            return 1;
        }
    }
    else if(key==CMD_SETPOSE_KEY)
    {
        int pn=0;
        bool b_NAME=false;
        bool b_POINT=false;
        bool b_POSE=false;
        bool b_ADD=false;

        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
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
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_setpose_nameout,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_POINT)
                {
                    if(b_POINT==false)
                    {
                        b_POINT=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_setpose_namein,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_POSE)
                {
                    if(b_POSE==false)
                    {
                        b_POSE=true;
                        if(0!=de_vector_float(paramname,param[n],data_fpos,data_bpos,cmd_setpose_pose,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_setpose_pose.size()!=3)
                        {
                            return_msg=paramname+QStringLiteral("项参数有且只有3个");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_ADD)
                {
                    if(b_ADD==false)
                    {
                        b_ADD=true;
                        if(0!=de_vector_float(paramname,param[n],data_fpos,data_bpos,cmd_setpose_add,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_setpose_add.size()!=3)
                        {
                            return_msg=paramname+QStringLiteral("项参数有且只有3个");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_NAME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_NAME+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_POINT==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_POINT+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_POSE==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_POSE+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_ADD==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_ADD+QStringLiteral("'项参数");
            return 1;
        }

        if(cmd_setpose_namein==cmd_setpose_nameout)
        {
            return_msg=key+"'"+CMD_NAME+QStringLiteral("'项参数和'")+CMD_POINT+QStringLiteral("'项参数不能同名");
            return 1;
        }
    }
    else if(key==CMD_GETTCPPOS_KEY)
    {
        int pn=0;
        bool b_NAME=false;
        bool b_ADD=false;
        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
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
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_gettcppos_name,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_ADD)
                {
                    if(b_ADD==false)
                    {
                        b_ADD=true;
                        if(0!=de_vector_float(paramname,param[n],data_fpos,data_bpos,cmd_gettcppos_add,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_gettcppos_add.size()!=3)
                        {
                            return_msg=paramname+QStringLiteral("项参数有且只有")+QString::number(3)+QStringLiteral("个");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_NAME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_NAME+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_ADD==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_ADD+QStringLiteral("'项参数");
            return 1;
        }
    }
    else if(key==CMD_CREATF_KEY)
    {
        int pn=0;
        bool b_NAME=false;
        bool b_FILE=false;
        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
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
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_creatf_name,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_FILE)
                {
                    if(b_FILE==false)
                    {
                        b_FILE=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_creatf_filename,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_NAME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_NAME+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_FILE==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_FILE+QStringLiteral("'项参数");
            return 1;
        }
    }
    else if(key==CMD_PLOTPOS_KEY)
    {
        int pn=0;
        bool b_NAME=false;
        bool b_MODE=false;
        bool b_PARAM=false;
        bool b_POINTS=false;
        bool b_CREATS=false;

        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
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
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_plotpos_name,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
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
                        cmd_plotpos_mode=(Plotpos_edit_mode)data;
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_CREATS)
                {
                    if(b_CREATS==false)
                    {
                        b_PARAM=true;
                        b_CREATS=true;
                        if(0!=de_vector_QString(paramname,param[n],data_fpos,data_bpos,cmd_plotpos_creatname,return_msg))
                        {
                            return 1;
                        }
                        if(false==b_nosame_vector_QString(cmd_plotpos_creatname))
                        {
                            return_msg=paramname+QStringLiteral("项参数里不能有同名项");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_POINTS)
                {
                    if(b_POINTS==false)
                    {
                        b_PARAM=true;
                        b_POINTS=true;
                        if(0!=de_vector_QString(paramname,param[n],data_fpos,data_bpos,cmd_plotpos_pointsname,return_msg))
                        {
                            return 1;
                        }
                        if(false==b_nosame_vector_QString(cmd_plotpos_pointsname))
                        {
                            return_msg=paramname+QStringLiteral("项参数里不能有同名项");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_NAME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_NAME+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_MODE==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_MODE+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_PARAM==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_CREATS+QStringLiteral("'或'")+CMD_POINTS+QStringLiteral("'项参数");
            return 1;
        }
    }
    else if(key==CMD_CREATC_KEY)
    {
        int pn=0;
        bool b_NAME=false;
        bool b_POINTS=false;
        bool b_SPEED=false;
        bool b_TIME=false;
        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
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
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_creatc_name,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_POINTS)
                {
                    if(b_POINTS==false)
                    {
                        b_POINTS=true;
                        if(0!=de_vector_QString(paramname,param[n],data_fpos,data_bpos,cmd_creatc_pointsname,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_creatc_pointsname.size()!=3)
                        {
                            return_msg=paramname+QStringLiteral("项参数有且只有3个");
                            return 1;
                        }
                        if(false==b_nosame_vector_QString(cmd_creatc_pointsname))
                        {
                            return_msg=paramname+QStringLiteral("项参数里不能有同名项");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_SPEED)
                {
                    if(b_SPEED==false)
                    {
                        b_SPEED=true;
                        if(0!=de_float(paramname,param[n],data_fpos,data_bpos,cmd_creatc_speed,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_creatc_speed<=0)
                        {
                            return_msg=paramname+QStringLiteral("项参数只能大于0");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_TIME)
                {
                    if(b_TIME==false)
                    {
                        b_TIME=true;
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,cmd_creatc_time,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_creatc_time<0)
                        {
                            return_msg=paramname+QStringLiteral("项参数只能大于等于0");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_NAME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_NAME+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_POINTS==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_POINTS+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_TIME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_TIME+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_SPEED==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_SPEED+QStringLiteral("'项参数");
            return 1;
        }
    }
    else if(key==CMD_GETTCPPOS2_KEY)
    {
        int pn=0;
        bool b_NAME=false;
        bool b_POS=false;
        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
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
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_gettcppos2_name,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_POS)
                {
                    if(b_POS==false)
                    {
                        b_POS=true;
                        if(0!=de_robpos(paramname,param[n],data_fpos,data_bpos,cmd_gettcppos2_pos,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_NAME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_NAME+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_POS==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_POS+QStringLiteral("'项参数");
            return 1;
        }
    }
    else if(key==CMD_TRACECONTINUE_KEY)
    {
        int pn=0;
        bool b_TRACE=false;
        bool b_NAME=false;
        bool b_POS=false;
        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
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
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_tracecontinue_nameout,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_TRACE)
                {
                    if(b_TRACE==false)
                    {
                        b_TRACE=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_tracecontinue_namein,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_POS)
                {
                    if(b_POS==false)
                    {
                        b_POS=true;
                        if(0!=de_robpos(paramname,param[n],data_fpos,data_bpos,cmd_tracecontinue_pos,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_NAME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_NAME+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_TRACE==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_TRACE+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_POS==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_POS+QStringLiteral("'项参数");
            return 1;
        }
    }
    else if(key==CMD_FILTER_KEY)
    {
        int pn=0;
        bool b_CREAT=false;
        bool b_NAME=false;
        bool b_MODE=false;
        bool b_FILTERS=false;
        std::vector<float> f_datagroup;
        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
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
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_filter_nameout,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_CREAT)
                {
                    if(b_CREAT==false)
                    {
                        b_CREAT=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_filter_namein,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
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
                        cmd_filter_mode=(Filter_mode)data;
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_FILTERS)
                {
                    if(b_FILTERS==false)
                    {
                        b_FILTERS=true;
                        if(0!=de_vector_float(paramname,param[n],data_fpos,data_bpos,f_datagroup,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_NAME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_NAME+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_CREAT==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_CREAT+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_MODE==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_MODE+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_FILTERS==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_FILTERS+QStringLiteral("'项参数");
            return 1;
        }

        //判断滤波参数是否合理
        switch(cmd_filter_mode)
        {
        #if USE_PLC_FILTER==1
            case FILTER_MLS:
            {
                if(f_datagroup.size()!=1)
                {
                    return_msg=CMD_MODE+QStringLiteral("项参数为")+QString::number(cmd_filter_mode)+QStringLiteral("时,")+CMD_FILTERS+QStringLiteral("项参数有且只有1个");
                    return 1;
                }
                if(f_datagroup[0]<0)
                {
                    return_msg=Filter_mode_toQString(cmd_filter_mode)+CMD_FILTERS+QStringLiteral("项参数的拟合阶次(第1个参数)必须大于等于0");
                    return 1;
                }
                cmd_filters.msl_poly=f_datagroup[0];                 //拟合阶次,0为平滑，1为一项线性曲线拟合，2为二项线性曲线拟合    //设置上采样步长
            }
            break;
            case FILTER_SOR:
            {
                if(f_datagroup.size()!=2)
                {
                    return_msg=CMD_MODE+QStringLiteral("项参数为")+QString::number(cmd_filter_mode)+QStringLiteral("时,")+CMD_FILTERS+QStringLiteral("项参数有且只有2个");
                    return 1;
                }
                if(f_datagroup[0]<0)
                {
                    return_msg=Filter_mode_toQString(cmd_filter_mode)+CMD_FILTERS+QStringLiteral("项参数的邻域点数量(第1个参数)必须大于等于0");
                    return 1;
                }
                if(f_datagroup[1]<0)
                {
                    return_msg=Filter_mode_toQString(cmd_filter_mode)+CMD_FILTERS+QStringLiteral("项参数的标准差(第2个参数)必须大于等于0");
                    return 1;
                }

                cmd_filters.sor_nearpoint_num=f_datagroup[0];                //每个点参考的邻域点数量
                cmd_filters.sor_standard_deviation=f_datagroup[1];       //标准差
            }
            break;
        #endif
            case FILTER_SVD:
            {
                if(f_datagroup.size()!=3)
                {
                    return_msg=CMD_MODE+QStringLiteral("项参数为")+QString::number(cmd_filter_mode)+QStringLiteral("时,")+CMD_FILTERS+QStringLiteral("项参数有且只有3个");
                    return 1;
                }
                if(f_datagroup[0]<0)
                {
                    return_msg=Filter_mode_toQString(cmd_filter_mode)+CMD_FILTERS+QStringLiteral("项参数的邻域点数量(第1个参数)必须大于等于0");
                    return 1;
                }
                if(f_datagroup[1]<0)
                {
                    return_msg=Filter_mode_toQString(cmd_filter_mode)+CMD_FILTERS+QStringLiteral("项参数的标准差(第2个参数)必须大于等于0");
                    return 1;
                }
                if(f_datagroup[2]<0)
                {
                    return_msg=Filter_mode_toQString(cmd_filter_mode)+CMD_FILTERS+QStringLiteral("项参数的标准差(第3个参数)必须大于等于0");
                    return 1;
                }

                cmd_filters.svd_Degree=f_datagroup[0];
                cmd_filters.svd_WindowSize=f_datagroup[1];
                cmd_filters.svd_SingularThreshold=f_datagroup[2];
            }
            break;
            case FILTER_GAUSSIAN:
            {
                if(f_datagroup.size()!=2)
                {
                    return_msg=CMD_MODE+QStringLiteral("项参数为")+QString::number(cmd_filter_mode)+QStringLiteral("时,")+CMD_FILTERS+QStringLiteral("项参数有且只有2个");
                    return 1;
                }
                if(f_datagroup[0]<0)
                {
                    return_msg=Filter_mode_toQString(cmd_filter_mode)+CMD_FILTERS+QStringLiteral("项参数的邻域点数量(第1个参数)必须大于等于0");
                    return 1;
                }
                if(f_datagroup[1]<0)
                {
                    return_msg=Filter_mode_toQString(cmd_filter_mode)+CMD_FILTERS+QStringLiteral("项参数的标准差(第2个参数)必须大于等于0");
                    return 1;
                }
                cmd_filters.gaussian_SmoothingRadius=f_datagroup[0];
                cmd_filters.gaussian_SmoothingSigma=f_datagroup[1];
            }
            break;
            default:
            break;
        }
    }
    else if(key==CMD_GOTO_KEY)
    {
        int pn=0;
        bool b_LINE=false;
        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
            {
                QString paramname;
                int data_fpos,data_bpos;
                if(0!=de_param(++pn,param[n],paramname,data_fpos,data_bpos,return_msg))
                {
                    return 1;
                }
                if(paramname==CMD_LINE)
                {
                    if(b_LINE==false)
                    {
                        b_LINE=true;
                        if(0!=de_int(paramname,param[n],data_fpos,data_bpos,cmd_goto_line,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_goto_line<0)
                        {
                            return_msg=paramname+QStringLiteral("项参数只能大于等于0");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_LINE==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_LINE+QStringLiteral("'项参数");
            return 1;
        }
    }
    else if(key==CMD_CREATADD_KEY)
    {
        int pn=0;
        bool b_NAME=false;
        bool b_CREATS=false;

        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
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
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_creatadd_nameout,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_CREATS)
                {
                    if(b_CREATS==false)
                    {
                        b_CREATS=true;
                        if(0!=de_vector_QString(paramname,param[n],data_fpos,data_bpos,cmd_creatadd_names,return_msg))
                        {
                            return 1;
                        }
                        if(false==b_nosame_vector_QString(cmd_creatadd_names))
                        {
                            return_msg=paramname+QStringLiteral("项参数里不能有同名项");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=QStringLiteral("只能有一个'")+CMD_CREATS+QStringLiteral("'项或'")+CMD_POINTS+QStringLiteral("'项参数");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_NAME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_NAME+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_CREATS==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_CREATS+QStringLiteral("'项参数");
            return 1;
        }
    }
    else if(key==CMD_CREATADDP_KEY)
    {
        int pn=0;
        bool b_NAME=false;
        bool b_CREAT=false;
        bool b_MODE=false;
        bool b_POINT=false;

        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
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
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_creataddp_nameout,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_CREAT)
                {
                    if(b_CREAT==false)
                    {
                        b_CREAT=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_creataddp_weldname,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=QStringLiteral("只能有一个'")+CMD_CREAT+QStringLiteral("'项或'")+CMD_POINTS+QStringLiteral("'项参数");
                        return 1;
                    }
                }
                else if(paramname==CMD_POINT)
                {
                    if(b_POINT==false)
                    {
                        b_POINT=true;
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_creataddp_pointname,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
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
                        cmd_creataddp_mode=(Creataddp_edit_mode)data;
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_NAME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_NAME+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_CREAT==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_CREAT+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_POINT==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_POINT+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_MODE==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_MODE+QStringLiteral("'项参数");
            return 1;
        }
    }
    else if(key==CMD_CRAFTS_KEY)
    {
        int pn=0;
        bool b_NAME=false;
        bool b_MODE=false;
        bool b_POINTADDS=false;
        bool b_DATA32FS=false;
        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
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
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_crafts_name,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_POINTADDS)
                {
                    if(b_POINTADDS==false)
                    {
                        b_POINTADDS=true;
                        std::vector<float> f_datagroup;
                        if(0!=de_vector_float(paramname,param[n],data_fpos,data_bpos,f_datagroup,return_msg))
                        {
                            return 1;
                        }
                        int jump=12;
                        if(f_datagroup.size()%jump!=0)
                        {
                            return_msg=paramname+QStringLiteral("项参数的个数不合理");
                            return 1;
                        }
                        int num=f_datagroup.size()/jump;
                        cmd_crafts_posturelist.resize(num);
                        for(int n=0;n<num;n++)
                        {
                            cmd_crafts_posturelist[n].posture.X=f_datagroup[n*jump+0];
                            cmd_crafts_posturelist[n].posture.Y=f_datagroup[n*jump+1];
                            cmd_crafts_posturelist[n].posture.Z=f_datagroup[n*jump+2];
                            cmd_crafts_posturelist[n].posture.RX=f_datagroup[n*jump+3];
                            cmd_crafts_posturelist[n].posture.RY=f_datagroup[n*jump+4];
                            cmd_crafts_posturelist[n].posture.RZ=f_datagroup[n*jump+5];
                            cmd_crafts_posturelist[n].posture.out_1=f_datagroup[n*jump+6];
                            cmd_crafts_posturelist[n].posture.out_2=f_datagroup[n*jump+7];
                            cmd_crafts_posturelist[n].posture.out_3=f_datagroup[n*jump+8];
                            cmd_crafts_posturelist[n].Variable.X=f_datagroup[n*jump+9];
                            cmd_crafts_posturelist[n].Variable.Y=f_datagroup[n*jump+10];
                            cmd_crafts_posturelist[n].Variable.Z=f_datagroup[n*jump+11];
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_DATA32FS)
                {
                    if(b_DATA32FS==false)
                    {
                        b_DATA32FS=true;
                        if(0!=de_vector_float(paramname,param[n],data_fpos,data_bpos,cmd_crafts_params,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
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
                        cmd_crafts_craft_id=(Craft_ID)data;
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_NAME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_NAME+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_POINTADDS==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_POINTADDS+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_MODE==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_MODE+QStringLiteral("'项参数");
            return 1;
        }
        Craft *craft=craft->Get();
        if(0!=craft->posturelistcheck(cmd_crafts_craft_id,cmd_crafts_posturelist))
        {
            return_msg=QStringLiteral("当'")+CMD_MODE+QStringLiteral("'项参数等于")+
                        QString::number(cmd_crafts_craft_id)+QStringLiteral("时'")+CMD_POINTADDS+QStringLiteral("'项参数不合理");
            return 1;
        }
        switch(cmd_crafts_craft_id)
        {
            case CRAFT_ID_FIXED_POSTURE:    //固定焊接姿态
            {

            }
            break;
            case CRAFT_ID_STARTENDCHANGE_POSTURE:   //起终点变姿态
            {

            }
            break;
            case CRAFT_ID_LASERNORMAL_POSTURE:   //激光器测量法线姿态
            {

            }
            break;
            case CRAFT_ID_CORRUGATED_POSTURE: //波纹板变姿态
            {
                if(b_DATA32FS==false)
                {
                    return_msg=QStringLiteral("当'")+CMD_MODE+QStringLiteral("'项参数等于")+
                                QString::number(cmd_crafts_craft_id)+QStringLiteral("时,指令还需要设置'")+
                                CMD_DATA32FS+QStringLiteral("'项参数");
                    return 1;
                }
                else if(cmd_crafts_params.size()!=2)
                {
                    return_msg=QStringLiteral("当'")+CMD_MODE+QStringLiteral("'项参数等于")+
                                QString::number(cmd_crafts_craft_id)+QStringLiteral("时'")+CMD_DATA32FS+QStringLiteral("'项参数只能有2个");
                    return 1;
                }

            }
            break;
        }
    }
    else if(key==CMD_SAVEPCD_KEY)
    {
        int pn=0;
        bool b_NAME=false;
        bool b_POINTS=false;
        bool b_SCANS=false;
        bool b_CREATS=false;
        bool b_TRACES=false;
        bool b_MODE=false;

        QStringList param = list[1].split(" ");
        for(int n=0;n<param.size();n++)
        {
            if(param[n].size()!=0)
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
                        if(0!=de_QString(paramname,param[n],data_fpos,data_bpos,cmd_savepcd_name,return_msg))
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_POINTS)
                {
                    if(b_POINTS==false)
                    {
                        b_POINTS=true;
                        if(0!=de_vector_QString(paramname,param[n],data_fpos,data_bpos,cmd_savepcd_pointsname,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_savepcd_pointsname.size()<1)
                        {
                            return_msg=paramname+QStringLiteral("项参数个数至少大于等于1个");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_SCAN)
                {
                    if(b_SCANS==false)
                    {
                        b_SCANS=true;
                        if(0!=de_vector_QString(paramname,param[n],data_fpos,data_bpos,cmd_savepcd_scanname,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_savepcd_scanname.size()<1)
                        {
                            return_msg=paramname+QStringLiteral("项参数个数至少大于等于1个");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_CREATS)
                {
                    if(b_CREATS==false)
                    {
                        b_CREATS=true;
                        if(0!=de_vector_QString(paramname,param[n],data_fpos,data_bpos,cmd_savepcd_creatsname,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_savepcd_creatsname.size()<1)
                        {
                            return_msg=paramname+QStringLiteral("项参数个数至少大于等于1个");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else if(paramname==CMD_TRACES)
                {
                    if(b_TRACES==false)
                    {
                        b_TRACES=true;
                        if(0!=de_vector_QString(paramname,param[n],data_fpos,data_bpos,cmd_savepcd_tracesname,return_msg))
                        {
                            return 1;
                        }
                        if(cmd_savepcd_tracesname.size()<1)
                        {
                            return_msg=paramname+QStringLiteral("项参数个数至少大于等于1个");
                            return 1;
                        }
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
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
                        cmd_savepcd_mode=(Savepcd_edit_mode)data;
                    }
                    else
                    {
                        return_msg=paramname+QStringLiteral("项参数重复设置");
                        return 1;
                    }
                }
                else
                {
                    return_msg=key+QStringLiteral("指令里没有这个'")+paramname+QStringLiteral("'参数名称");
                    return 1;
                }
            }
        }
        if(b_NAME==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_NAME+QStringLiteral("'项参数");
            return 1;
        }
        else if(b_MODE==false)
        {
            return_msg=key+QStringLiteral("指令还需要设置'")+CMD_MODE+QStringLiteral("'项参数");
            return 1;
        }
    }
    else if(key==CMD_STOP_KEY)
    {
        //不需要处理
    }
    else
    {
        return_msg=QStringLiteral("指令集中没有'")+key+QStringLiteral("'类型的指令，请查看支持的指令表");
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
                 QString::number(pos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                 QString::number(pos.out_1)+","+
                 QString::number(pos.out_2)+","+
                 QString::number(pos.out_3)+
              "]";
    return msg;
}

QString my_cmd::rc_moveC(RobPos pos1,RobPos pos2,RobPos pos3,Robmovemodel movemodel)
{
    QString msg;
    QString msg1;
    switch(movemodel)
    {
        case MOVEC:
        {
            msg1=QString(CMD_MOVC);
        }
        break;
    }
    msg=msg1+"["+QString::number(pos1.X,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                 QString::number(pos1.Y,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                 QString::number(pos1.Z,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                 QString::number(pos1.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                 QString::number(pos1.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                 QString::number(pos1.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                 QString::number(pos1.out_1)+","+
                 QString::number(pos1.out_2)+","+
                 QString::number(pos1.out_3)+","+

                 QString::number(pos2.X,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                 QString::number(pos2.Y,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                 QString::number(pos2.Z,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                 QString::number(pos2.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                 QString::number(pos2.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                 QString::number(pos2.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                 QString::number(pos2.out_1)+","+
                 QString::number(pos2.out_2)+","+
                 QString::number(pos2.out_3)+","+

                 QString::number(pos3.X,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                 QString::number(pos3.Y,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                 QString::number(pos3.Z,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                 QString::number(pos3.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                 QString::number(pos3.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                 QString::number(pos3.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                 QString::number(pos3.out_1)+","+
                 QString::number(pos3.out_2)+","+
                 QString::number(pos3.out_3)+
            "]";
    return msg;
}

QString my_cmd::rc_smove(QString spos,Robmovemodel movemodel)
{
    QString msg;
    QString msg1;
    switch(movemodel)
    {
        case MOVEL:
        {
            msg1=QString(CMD_SMOVL);
        }
        break;
        case MOVEJ:
        {
            msg1=QString(CMD_SMOVJ);
        }
        break;
    }
    msg=msg1+"["+spos+"]";
    return msg;
}

QString my_cmd::rc_smoveC(QString s_pos1,QString s_pos2,QString s_pos3,Robmovemodel movemodel)
{
    QString msg;
    QString msg1;
    switch(movemodel)
    {
        case MOVEC:
        {
            msg1=QString(CMD_SMOVC);
        }
        break;
    }
    msg=msg1+"["+s_pos1+","+s_pos2+","+s_pos3+"]";
    return msg;
}


QString my_cmd::rc_change(QString name)
{
    QString msg;
    msg=QString(CMD_CHANGE)+"["+name+"]";
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

QString my_cmd::rc_elev(float elev)
{
    QString msg;
    msg=QString(CMD_ELEV)+"["+QString::number(elev,'f',3)+"]";
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

QString my_cmd::rc_crafts(QString crafts)
{
    QString msg;
    msg=QString(CMD_CRAFTS)+"["+crafts+"]";
    return msg;
}

QString my_cmd::rc_ioout(std::vector<int> io)
{
    QString msg;
    QString data;
    for(int i=0;i<io.size();i++)
    {
        data=data+QString::number(io[i]);
        if(i<io.size()-1)
        {
            data=data+",";
        }
    }
    msg=QString(CMD_OUT)+"["+data+"]";
    return msg;
}

QString my_cmd::rc_iowaitin(std::vector<int> io)
{
    QString msg;
    QString data;
    for(int i=0;i<io.size();i++)
    {
        data=data+QString::number(io[i]);
        if(i<io.size()-1)
        {
            data=data+",";
        }
    }
    msg=QString(CMD_WAITIN)+"["+data+"]";
    return msg;
}

QString my_cmd::rc_plcwait(int register_add)
{
    QString msg;
    msg=QString(CMD_WAITPLC)+"["+QString::number(register_add)+"]";
    return msg;
}

QString my_cmd::rc_plcwrite(int register_add)
{
    QString msg;
    msg=QString(CMD_WRITEPLC)+"["+QString::number(register_add)+"]";
    return msg;
}

QString my_cmd::rc_16data(int16_t i16_data)
{
    QString msg;
    msg=QString(CMD_DATA16)+"["+QString::number(i16_data)+"]";
    return msg;
}

QString my_cmd::rc_f32datas(std::vector<float> f32_datas)
{
    QString msg;
    QString data;
    for(int i=0;i<f32_datas.size();i++)
    {
        data=data+QString::number(f32_datas[i],'f',3);
        if(i<f32_datas.size()-1)
        {
            data=data+",";
        }
    }
    msg=QString(CMD_DATA32FS)+"["+data+"]";
    return msg;
}

QString my_cmd::rc_pointadds(std::vector<ChangeRobPosVariable> posturelist)
{
    QString msg;
    QString data;
    for(int i=0;i<posturelist.size();i++)
    {
        QString temp;
        temp=QString::number(posturelist[i].posture.X,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
             QString::number(posturelist[i].posture.Y,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
             QString::number(posturelist[i].posture.Z,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
             QString::number(posturelist[i].posture.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
             QString::number(posturelist[i].posture.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
             QString::number(posturelist[i].posture.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
             QString::number(posturelist[i].posture.out_1)+","+
             QString::number(posturelist[i].posture.out_2)+","+
             QString::number(posturelist[i].posture.out_3)+","+
             QString::number(posturelist[i].Variable.X,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
             QString::number(posturelist[i].Variable.Y,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
             QString::number(posturelist[i].Variable.Z,'f',ROBOT_POSE_DECIMAL_PLACE);
        data=data+temp;
        if(i<posturelist.size()-1)
        {
            data=data+",";
        }
    }
    msg=QString(CMD_POINTADDS)+"["+data+"]";
    return msg;
}

QString my_cmd::rc_side(int side)
{
    QString msg;
    msg=QString(CMD_SIDE)+"["+QString::number(side)+"]";
    return msg;
}

QString my_cmd::rc_sidemove(std::vector<float> sidemove)
{
    QString msg,s_data;
    for(int i=0;i<sidemove.size();i++)
    {
        s_data=s_data+QString::number(sidemove[i],'f',ROBOT_POSE_DECIMAL_PLACE);
        if(i!=sidemove.size()-1)
        {
          s_data=s_data+",";
        }
    }
    msg=QString(CMD_SIDEMOVE)+"["+s_data+"]";
    return msg;
}

QString my_cmd::rc_sidespeed(float sidespeed)
{
    QString msg;
    msg=QString(CMD_SIDESPEED)+"["+QString::number(sidespeed,'f',3)+"]";
    return msg;
}

QString my_cmd::rc_pointX(QString s_pointX)
{
    QString msg;
    msg=QString(CMD_POINTX)+"["+s_pointX+"]";
    return msg;
}

QString my_cmd::rc_pointO(QString s_pointO)
{
    QString msg;
    msg=QString(CMD_POINTO)+"["+s_pointO+"]";
    return msg;
}

QString my_cmd::rc_cpointX(QString s_cpointX)
{
    QString msg;
    msg=QString(CMD_CPOINTX)+"["+s_cpointX+"]";
    return msg;
}

QString my_cmd::rc_cpointO(QString s_cpointO)
{
    QString msg;
    msg=QString(CMD_CPOINTO)+"["+s_cpointO+"]";
    return msg;
}

QString my_cmd::rc_creat(QString names)
{
    QString msg;
    msg=QString(CMD_CREAT)+"["+names+"]";
    return msg;
}

QString my_cmd::rc_traceadd(QString name1, QString name2)
{
    QString msg;
    msg=QString(CMD_TRACEADD)+"["+name1+","+name2+"]";
    return msg;
}

QString my_cmd::rc_trace(QString name)
{
    QString msg;
    msg=QString(CMD_TRACE)+"["+name+"]";
    return msg;
}

QString my_cmd::rc_traces(std::vector<QString> names)
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
    msg=QString(CMD_TRACES)+"["+msg1+"]";
    return msg;
}

QString my_cmd::rc_add(std::vector<float> add)
{
    QString msg,s_data;
    for(int i=0;i<add.size();i++)
    {
        s_data=s_data+QString::number(add[i],'f',ROBOT_POSE_DECIMAL_PLACE);
        if(i!=add.size()-1)
        {
          s_data=s_data+",";
        }
    }
    msg=QString(CMD_ADD)+"["+s_data+"]";
    return msg;
}

QString my_cmd::rc_wave(wWAVEParam cmd_wave_info)
{
    QString msg;
    msg=QString(CMD_WAVE)+"["+
        QString::number(cmd_wave_info.period,'f',3)+","+
        QString::number(cmd_wave_info.leftAmp,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
        QString::number(cmd_wave_info.rightAmp,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
        QString::number(cmd_wave_info.leftAmp_z,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
        QString::number(cmd_wave_info.rightAmp_z,'f',ROBOT_POSE_DECIMAL_PLACE)+","+       
        QString::number(cmd_wave_info.leftStopTime,'f',3)+","+
        QString::number(cmd_wave_info.rightStopTime,'f',3)+","+
        QString::number(cmd_wave_info.anglex,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
        QString::number(cmd_wave_info.angley,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
        QString::number(cmd_wave_info.startPos)+","+
        QString::number(cmd_wave_info.order)+","+
        QString::number(cmd_wave_info.pendulum_mode)+","+
        QString::number(cmd_wave_info.timeGap)+","+
        QString::number(cmd_wave_info.trend_mode)+","+
        QString::number(cmd_wave_info.leftAddRX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
        QString::number(cmd_wave_info.leftAddRY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
        QString::number(cmd_wave_info.leftAddRZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
        QString::number(cmd_wave_info.rightAddRX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
        QString::number(cmd_wave_info.rightAddRY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
        QString::number(cmd_wave_info.rightAddRZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+"]";
    return msg;
}

QString my_cmd::rc_aout(std::vector<float> a)
{
    QString msg;
    QString data;
    for(int i=0;i<a.size();i++)
    {
        data=data+QString::number(a[i],'f',3);
        if(i<a.size()-1)
        {
            data=data+",";
        }
    }
    msg=QString(CMD_AOUT)+"["+data+"]";
    return msg;
}

QString my_cmd::rc_points(std::vector<QString> pointsname)
{
    QString msg;
    QString data;
    for(int i=0;i<pointsname.size();i++)
    {
        data=data+pointsname[i];
        if(i<pointsname.size()-1)
        {
            data=data+",";
        }
    }
    msg=QString(CMD_POINTS)+"["+data+"]";
    return msg;
}

QString my_cmd::rc_point(QString name)
{
    QString msg;
    msg=QString(CMD_POINT)+"["+name+"]";
    return msg;
}

QString my_cmd::rc_pose(std::vector<float> pose)
{
    QString msg;
    QString data;
    for(int i=0;i<pose.size();i++)
    {
        data=data+QString::number(pose[i],'f',3);
        if(i<pose.size()-1)
        {
            data=data+",";
        }
    }
    msg=QString(CMD_POSE)+"["+data+"]";
    return msg;
}

QString my_cmd::rc_file(QString filename)
{
    QString msg;
    msg=QString(CMD_FILE)+"["+filename+"]";
    return msg;
}

QString my_cmd::rc_creats(std::vector<QString> names)
{
    QString msg;
    QString data;
    for(int i=0;i<names.size();i++)
    {
        data=data+names[i];
        if(i<names.size()-1)
        {
            data=data+",";
        }
    }
    msg=QString(CMD_CREATS)+"["+data+"]";
    return msg;
}

QString my_cmd::rc_plot(Plotpos_edit_mode mode,std::vector<QString> weldnames,std::vector<QString> pointnames)
{
    QString msg;
    switch(mode)
    {
        case PLOTPOS_EDIT_MODE_THREE_TO_ONE:
        {
            msg=rc_creats(weldnames);
        }
        break;
        case PLOTPOS_EDIT_MODE_FIVEPOINTS_TO_ONE:
        {
            msg=rc_points(pointnames);
        }
        break;
        case PLOTPOS_EDIT_MODE_LINE_THREEPOINTS_TO_ONE:
        {
            msg=rc_creats(weldnames)+" "+
                rc_points(pointnames);
        }
        break;
        default:
        {
            msg=rc_creats(weldnames);
        }
        break;
    }
    return msg;
}

QString my_cmd::rc_pos(RobPos pos)
{
    QString msg;
    QString msg1=CMD_POS;
    msg=msg1+"["+QString::number(pos.X,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                 QString::number(pos.Y,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                 QString::number(pos.Z,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                 QString::number(pos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                 QString::number(pos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                 QString::number(pos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                 QString::number(pos.out_1)+","+
                 QString::number(pos.out_2)+","+
                 QString::number(pos.out_3)+
              "]";
    return msg;
}

QString my_cmd::rc_filters(filterParam filters,Filter_mode mode)
{
    QString msg;
    QString data;

    switch(mode)
    {
    #if USE_PLC_FILTER==1
        case FILTER_MLS:
        {
            msg=QString(CMD_FILTERS)+"["+
                QString::number(filters.msl_poly)+"]";
        }
        break;
        case FILTER_SOR:
        {
            msg=QString(CMD_FILTERS)+"["+
                QString::number(filters.sor_nearpoint_num)+","+
                QString::number(filters.sor_standard_deviation,'f',3)+"]";
        }
        break;
    #endif
        case FILTER_SVD:
        {
            msg=QString(CMD_FILTERS)+"["+
                QString::number(filters.svd_Degree)+","+
                QString::number(filters.svd_WindowSize)+","+
                QString::number(filters.svd_SingularThreshold,'f',ROBOT_POSE_DECIMAL_PLACE)+"]";
        }
        break;
        case FILTER_GAUSSIAN:
        {
            msg=QString(CMD_FILTERS)+"["+
                QString::number(filters.gaussian_SmoothingRadius,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                QString::number(filters.gaussian_SmoothingSigma,'f',3)+"]";
        }
        break;
    }
    return msg;
}

QString my_cmd::rc_samplespeed(float speed)
{
    QString msg;
    msg=QString(CMD_SAMPLESPEED)+"["+QString::number(speed,'f',3)+"]";
    return msg;
}

QString my_cmd::rc_line(int line)
{
    QString msg;
    msg=QString(CMD_LINE)+"["+QString::number(line)+"]";
    return msg;
}

int my_cmd::de_param(int param_n,QString msg,QString &paramname,int &data_fpos,int &data_bpos,QString &return_msg)
{
    data_fpos = msg.indexOf("[");
    data_bpos = msg.indexOf("]");
    if(data_fpos<0||data_bpos<0||data_bpos<data_fpos||data_bpos!=msg.size()-1)
    {
        return_msg=QStringLiteral("第")+QString::number(param_n)+QStringLiteral("项参数格式错误");
        return 1;
    }
    paramname=msg.mid(0,data_fpos);
    if(paramname.size()==0)
    {
        return_msg=QStringLiteral("第")+QString::number(param_n)+QStringLiteral("项参数没有参数名称");
        return 1;
    }
    return 0;
}

int my_cmd::de_int(QString parakey,QString msg,int data_fpos,int data_bpos,int &intdata,QString &return_msg)
{
    QString paramdata=msg.mid(data_fpos+1,data_bpos-data_fpos-1);
    if(paramdata.size()==0)
    {
        return_msg=parakey+QStringLiteral("项参数数据为空");
        return 1;
    }
    bool ok;
    intdata=paramdata.toInt(&ok);//获取到值
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项参数数据格式错误");
        return 1;
    }
    return 0;
}

int my_cmd::de_float(QString parakey,QString msg,int data_fpos,int data_bpos,float &floatdata,QString &return_msg)
{
    QString paramdata=msg.mid(data_fpos+1,data_bpos-data_fpos-1);
    if(paramdata.size()==0)
    {
        return_msg=parakey+QStringLiteral("项参数数据为空");
        return 1;
    }
    bool ok;
    floatdata=paramdata.toFloat(&ok);//获取到值
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项参数数据格式错误");
        return 1;
    }
    return 0;
}

int my_cmd::de_robpos(QString parakey,QString msg,int data_fpos,int data_bpos,RobPos &pos,QString &return_msg)
{
    QString paramdata=msg.mid(data_fpos+1,data_bpos-data_fpos-1);
    if(paramdata.size()==0)
    {
        return_msg=parakey+QStringLiteral("项参数数据为空");
        return 1;
    }
    QStringList posgroup = paramdata.split(",");
    if(posgroup.size()!=9)
    {
        return_msg=parakey+QStringLiteral("项参数数据有且只有9个");
        return 1;
    }
    bool ok;
    pos.X=posgroup[0].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的X项参数数据格式错误");
        return 1;
    }
    pos.Y=posgroup[1].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的Y项参数数据格式错误");
        return 1;
    }
    pos.Z=posgroup[2].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的Z项参数数据格式错误");
        return 1;
    }
    pos.RX=posgroup[3].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的RX项参数数据格式错误");
        return 1;
    }
    pos.RY=posgroup[4].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的RY项参数数据格式错误");
        return 1;
    }
    pos.RZ=posgroup[5].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的RZ项参数数据格式错误");
        return 1;
    }
    pos.out_1=posgroup[6].toInt(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的out_1项参数数据格式错误");
        return 1;
    }
    pos.out_2=posgroup[7].toInt(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的out_2项参数数据格式错误");
        return 1;
    }
    pos.out_3=posgroup[8].toInt(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的out_3项参数数据格式错误");
        return 1;
    }
    return 0;
}

int my_cmd::de_robposP(QString parakey,QString msg,int data_fpos,int data_bpos,RobPos &pos1,RobPos &pos2,RobPos &pos3,QString &return_msg)
{
    QString paramdata=msg.mid(data_fpos+1,data_bpos-data_fpos-1);
    if(paramdata.size()==0)
    {
        return_msg=parakey+QStringLiteral("项参数数据为空");
        return 1;
    }
    QStringList posgroup = paramdata.split(",");
    if(posgroup.size()!=27)
    {
        return_msg=parakey+QStringLiteral("项参数数据有且只有18个");
        return 1;
    }
    bool ok;
    pos1.X=posgroup[0].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的X1项参数数据格式错误");
        return 1;
    }
    pos1.Y=posgroup[1].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的Y1项参数数据格式错误");
        return 1;
    }
    pos1.Z=posgroup[2].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的Z1项参数数据格式错误");
        return 1;
    }
    pos1.RX=posgroup[3].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的RX1项参数数据格式错误");
        return 1;
    }
    pos1.RY=posgroup[4].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的RY1项参数数据格式错误");
        return 1;
    }
    pos1.RZ=posgroup[5].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的RZ1项参数数据格式错误");
        return 1;
    }
    pos1.out_1=posgroup[6].toInt(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的out_1项参数数据格式错误");
        return 1;
    }
    pos1.out_2=posgroup[7].toInt(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的out_2项参数数据格式错误");
        return 1;
    }
    pos1.out_3=posgroup[8].toInt(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的out_3项参数数据格式错误");
        return 1;
    }

    pos2.X=posgroup[9].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的X2项参数数据格式错误");
        return 1;
    }
    pos2.Y=posgroup[10].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的Y2项参数数据格式错误");
        return 1;
    }
    pos2.Z=posgroup[11].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的Z2项参数数据格式错误");
        return 1;
    }
    pos2.RX=posgroup[12].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的RX2项参数数据格式错误");
        return 1;
    }
    pos2.RY=posgroup[13].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的RY2项参数数据格式错误");
        return 1;
    }
    pos2.RZ=posgroup[14].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的RZ2项参数数据格式错误");
        return 1;
    }
    pos2.out_1=posgroup[15].toInt(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的out_1项参数数据格式错误");
        return 1;
    }
    pos2.out_2=posgroup[16].toInt(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的out_2项参数数据格式错误");
        return 1;
    }
    pos2.out_3=posgroup[17].toInt(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的out_3项参数数据格式错误");
        return 1;
    }

    pos3.X=posgroup[18].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的X3项参数数据格式错误");
        return 1;
    }
    pos3.Y=posgroup[19].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的Y3项参数数据格式错误");
        return 1;
    }
    pos3.Z=posgroup[20].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的Z3项参数数据格式错误");
        return 1;
    }
    pos3.RX=posgroup[21].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的RX3项参数数据格式错误");
        return 1;
    }
    pos3.RY=posgroup[22].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的RY3项参数数据格式错误");
        return 1;
    }
    pos3.RZ=posgroup[23].toFloat(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的RZ3项参数数据格式错误");
        return 1;
    }
    pos3.out_1=posgroup[24].toInt(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的out_1项参数数据格式错误");
        return 1;
    }
    pos3.out_2=posgroup[25].toInt(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的out_2项参数数据格式错误");
        return 1;
    }
    pos3.out_3=posgroup[26].toInt(&ok);
    if(ok==false)
    {
        return_msg=parakey+QStringLiteral("项的out_3项参数数据格式错误");
        return 1;
    }
    return 0;
}

int my_cmd::de_QString(QString parakey,QString msg,int data_fpos,int data_bpos,QString &QStringdata,QString &return_msg)
{
    QString paramdata=msg.mid(data_fpos+1,data_bpos-data_fpos-1);
    if(paramdata.size()==0)
    {
        return_msg=parakey+QStringLiteral("项参数数据为空");
        return 1;
    }
    QStringdata=paramdata;//获取到值
    if(QStringdata.size()==0)
    {
        return_msg=parakey+QStringLiteral("项参数数据格式错误");
        return 1;
    }
    if(QStringdata.indexOf(",")!=-1)
    {
        return_msg=parakey+QStringLiteral("','符号是保留字符，不能用于自定义字符");
        return 1;
    }
    return 0;
}

int my_cmd::de_vector_int(QString parakey,QString msg,int data_fpos,int data_bpos,std::vector<int> &vector_intdata,QString &return_msg)
{
    bool ok;
    QString paramdata=msg.mid(data_fpos+1,data_bpos-data_fpos-1);
    if(paramdata.size()==0)
    {
        return_msg=parakey+QStringLiteral("项参数数据为空");
        return 1;
    }
    QStringList posgroup = paramdata.split(",");
    vector_intdata.resize(posgroup.size());
    for(int n=0;n<posgroup.size();n++)
    {
        vector_intdata[n]=posgroup[n].toInt(&ok);
        if(ok==false)
        {
            return_msg=parakey+QStringLiteral("项的第")+QString::number(n+1)+QStringLiteral("个参数数据格式错误");
            return 1;
        }
    }
    return 0;
}

int my_cmd::de_vector_float(QString parakey,QString msg,int data_fpos,int data_bpos,std::vector<float> &vector_floatdata,QString &return_msg)
{
    bool ok;
    QString paramdata=msg.mid(data_fpos+1,data_bpos-data_fpos-1);
    if(paramdata.size()==0)
    {
        return_msg=parakey+QStringLiteral("项参数数据为空");
        return 1;
    }
    QStringList posgroup = paramdata.split(",");
    vector_floatdata.resize(posgroup.size());
    for(int n=0;n<posgroup.size();n++)
    {
        vector_floatdata[n]=posgroup[n].toFloat(&ok);
        if(ok==false)
        {
            return_msg=parakey+QStringLiteral("项的第")+QString::number(n+1)+QStringLiteral("个参数数据格式错误");
            return 1;
        }
    }
    return 0;
}

int my_cmd::de_vector_QString(QString parakey,QString msg,int data_fpos,int data_bpos,std::vector<QString> &vector_QStringdata,QString &return_msg)
{
    QString paramdata=msg.mid(data_fpos+1,data_bpos-data_fpos-1);
    if(paramdata.size()==0)
    {
        return_msg=parakey+QStringLiteral("项参数数据为空");
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


