#include "tosendbuffer.h"

extern QMutex send_group_leaser;
extern QMutex send_group_robot;
extern QMutex main_record;

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
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 扫描轨迹与已有的轨迹重名");
                    m_mcs->main_record.push_back(return_msg);
                    errmsg.push_back(return_msg);
                    main_record.unlock();
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
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 跟踪轨迹与已有的轨迹重名");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    errmsg.push_back(return_msg);
                }
                else
                {
                    switch(mode)
                    {
                        case TRACE_EDIT_MODE_ONE_TO_ONE:    //单扫单轨道模式
                        {
                            if(scannames.size()!=1)
                            {
                                err=1;
                                main_record.lock();
                                return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": ")+
                                           QString::fromLocal8Bit(CMD_MODE)+QString::fromLocal8Bit("值为")+QString::number(TRACE_EDIT_MODE_ONE_TO_ONE)+
                                           QString::fromLocal8Bit("时,")+QString::fromLocal8Bit(CMD_SCAN)+QString::fromLocal8Bit("项的参数只能有1个");
                                m_mcs->main_record.push_back(return_msg);
                                main_record.unlock();
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
                                main_record.lock();
                                return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+scannames[m]+QString::fromLocal8Bit("的扫描轨道");
                                m_mcs->main_record.push_back(return_msg);
                                main_record.unlock();
                                errmsg.push_back(return_msg);
                                break;
                            }
                        }
                        break;
                        case TRACE_EDIT_MODE_THREE_TO_ONE:  //三直线交点模式
                        {
                            if(scannames.size()!=3)
                            {
                                err=1;
                                main_record.lock();
                                return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": ")+
                                           QString::fromLocal8Bit(CMD_MODE)+QString::fromLocal8Bit("值为")+QString::number(TRACE_EDIT_MODE_THREE_TO_ONE)+
                                           QString::fromLocal8Bit("时,")+QString::fromLocal8Bit(CMD_SCAN)+QString::fromLocal8Bit("项的参数只能有3个");
                                m_mcs->main_record.push_back(return_msg);
                                main_record.unlock();
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
                                main_record.lock();
                                return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+scannames[m]+QString::fromLocal8Bit("的扫描轨道");
                                m_mcs->main_record.push_back(return_msg);
                                main_record.unlock();
                                errmsg.push_back(return_msg);
                                break;
                            }
                        }
                        break;
                        default:
                        {
                            err=1;
                            main_record.lock();
                            return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 不支持当前轨道生成模式");
                            m_mcs->main_record.push_back(return_msg);
                            main_record.unlock();
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
            main_record.lock();
            return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 语法出错(")+msg+QString::fromLocal8Bit(")");
            m_mcs->main_record.push_back(return_msg);
            main_record.unlock();
        }
        if(key==CMD_TRACE_KEY)//跟踪命令时查看是否能找到工艺路径
        {
            QString craftfilepath=cmd.cmd_trace_craftfilepath;//获取到工艺包的文件路径
        #if _MSC_VER
            QTextCodec *code = QTextCodec::codecForName("GBK");
        #else
            QTextCodec *code = QTextCodec::codecForName("UTF-8");
        #endif
            std::string fname = code->fromUnicode(craftfilepath).data();
            int rc=m_mcs->craft->LoadCraft((char*)fname.c_str());
            if(rc!=0)
            {
                err=1;
                main_record.lock();
                return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 焊接工艺参数文件格式出错");
                m_mcs->main_record.push_back(return_msg);
                main_record.unlock();
            }
        }
    }
    if(0!=cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))//查看是否有重名轨迹
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
        main_record.lock();
        return_msg=QString::fromLocal8Bit("已经执行完全部命令");
        m_mcs->main_record.push_back(return_msg);
        main_record.unlock();
        return 1;
    }
    while(m_mcs->rob->robot_state!=ROBOT_STATE_IDLE)//等待机器人空闲才能执行之后的命令
    {
        if(b_cmdlist_build==false)     //手动停止
        {
            main_record.lock();
            return_msg=QString::fromLocal8Bit("手动停止进程");
            m_mcs->main_record.push_back(return_msg);
            main_record.unlock();
            if(line>0)
            {
                line=line-1;//恢复到上一条指令
            }
            cmd_lock(1);
            return 1;
        }
        usleep(ROB_WORK_DELAY_STEP);
    }
    b_cmdlist_build=true;
    for(int n=line;n<m_mcs->project->project_cmdlist.size();n++)
    {
        QString msg,key;
        my_cmd cmd;
        main_record.lock();
        return_msg=QString::fromLocal8Bit("执行Line")+QString::number(n)+QString::fromLocal8Bit(": ")+m_mcs->project->project_cmdlist[n];
        m_mcs->main_record.push_back(return_msg);
        main_record.unlock();
        int rc=cmd.decodecmd(m_mcs->project->project_cmdlist[n],msg,key);
        if(rc>0)
        {
            //语法出错
            main_record.lock();
            return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 语法出错(")+msg+QString::fromLocal8Bit(")");
            m_mcs->main_record.push_back(return_msg);
            main_record.unlock();
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
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("手动停止进程");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    cmd_lock(1);
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
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("手动停止进程");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    cmd_lock(1);
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
            if(m_mcs->e2proomdata.maindlg_SaveDatacheckBox!=0)//保存扫描轨迹
            {
                QString dir="./log/";
                QString key=SAVELOGFILE_SCANNAME_HEAD;
                QString time;
                std::string s_time;
                TimeFunction to;
                to.get_time_ms(&s_time);
                time=QString::fromStdString(s_time);
                dir=dir+time+key+name;
                savelog_scan(dir,m_mcs->project->project_scan_trace[scan_trace_num].point);
            }
        }
        else if(key==CMD_CREAT_KEY)//生成轨迹命令
        {
            //这里添加轨迹生成
            QString name=cmd.cmd_creat_name;//获取到的生成的轨迹名字
            Trace_edit_mode mode=cmd.cmd_creat_mode;//获取到的轨迹生成模式
            std::vector<QString> scanname=cmd.cmd_creat_scanname;//获取到生成轨迹所需要的轨迹名字
            int weld_trace_num;//搜索到的焊接轨道序号
            std::vector<RobPos> weld;//轨道
            for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
            {
                if(name==m_mcs->project->project_weld_trace[n].name)
                {
                    weld_trace_num=n;//找到要储存的焊接轨道下标
                    break;
                }
            }

            switch(mode)
            {
                case TRACE_EDIT_MODE_ONE_TO_ONE://单扫单轨
                {
                    int scan_trace_num;//搜索到的扫描轨道序号
                    QString scan_trace_name=scanname[0];//要搜索的扫描轨道名字
                    for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
                    {
                        if(scan_trace_name==m_mcs->project->project_scan_trace[n].name)
                        {
                            scan_trace_num=n;
                            break;
                        }
                    }
                    //这里添加轨迹拟合
                    std::vector<Scan_trace_line> scan_trace=m_mcs->project->project_scan_trace[scan_trace_num].point;
                    std::vector<Eigen::Vector3d> weld_trace;
                    if(false==m_mcs->synchronous->Scantrace_to_Weldtrace(scan_trace,weld_trace))
                    {
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 轨迹坐标拟合出错");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        line=n;
                        return 1;
                    }
                    weld.resize(weld_trace.size());
                    for(int n=0;n<weld.size();n++)
                    {
                        weld[n].X=weld_trace[n].x();
                        weld[n].Y=weld_trace[n].y();
                        weld[n].Z=weld_trace[n].z();
                    }
                }
                break;
                case TRACE_EDIT_MODE_THREE_TO_ONE:  //三直线交点模式
                {
                    int scan_trace_num_0,scan_trace_num_1,scan_trace_num_2;//搜索到的扫描轨道序号
                    for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
                    {
                        if(scanname[0]==m_mcs->project->project_scan_trace[n].name)
                        {
                            scan_trace_num_0=n;
                            break;
                        }
                    }
                    for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
                    {
                        if(scanname[1]==m_mcs->project->project_scan_trace[n].name)
                        {
                            scan_trace_num_1=n;
                            break;
                        }
                    }
                    for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
                    {
                        if(scanname[2]==m_mcs->project->project_scan_trace[n].name)
                        {
                            scan_trace_num_2=n;
                            break;
                        }
                    }
                    //这里添加轨迹拟合
                    std::vector<Scan_trace_line> scan_trace0,scan_trace1,scan_trace2;
                    std::vector<Eigen::Vector3d> weld_trace0,weld_trace1,weld_trace2;
                    scan_trace0=m_mcs->project->project_scan_trace[scan_trace_num_0].point;
                    if(false==m_mcs->synchronous->Scantrace_to_Weldtrace(scan_trace0,weld_trace0)||
                       false==m_mcs->synchronous->Scantrace_to_Weldtrace(scan_trace1,weld_trace1)||
                       false==m_mcs->synchronous->Scantrace_to_Weldtrace(scan_trace2,weld_trace2))
                    {
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 轨迹坐标拟合出错");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        line=n;
                        return 1;
                    }
                    std::vector<Eigen::VectorXd> linePoints,SidePoints;
                    for(int n=0;n<weld_trace0.size();n++)
                    {
                        Eigen::Vector3d sing_linepoint;
                        sing_linepoint(0)=weld_trace0[n].x();
                        sing_linepoint(1)=weld_trace0[n].y();
                        sing_linepoint(2)=weld_trace0[n].z();
                        linePoints.push_back(sing_linepoint);
                    }
                    for(int n=0;n<weld_trace1.size();n++)
                    {
                        Eigen::Vector3d sing_linepoint;
                        sing_linepoint(0)=weld_trace1[n].x();
                        sing_linepoint(1)=weld_trace1[n].y();
                        sing_linepoint(2)=weld_trace1[n].z();
                        SidePoints.push_back(sing_linepoint);
                    }
                    for(int n=0;n<weld_trace2.size();n++)
                    {
                        Eigen::Vector3d sing_linepoint;
                        sing_linepoint(0)=weld_trace2[n].x();
                        sing_linepoint(1)=weld_trace2[n].y();
                        sing_linepoint(2)=weld_trace2[n].z();
                        SidePoints.push_back(sing_linepoint);
                    }
                    if(linePoints.size()<=2&&SidePoints.size()<=4)
                    {
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 轨迹坐标数据太少");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        line=n;
                        return 1;
                    }
                    FitlineSide fitlineside;
                    Eigen::Vector3d endpoint=fitlineside.computePointOfLineAndSurface(linePoints,SidePoints);//交点
                    float f_headdis=(weld_trace0[0].x()-endpoint.x())*(weld_trace0[0].x()-endpoint.x())+
                                    (weld_trace0[0].y()-endpoint.y())*(weld_trace0[0].y()-endpoint.y())+
                                    (weld_trace0[0].z()-endpoint.z())*(weld_trace0[0].z()-endpoint.z());
                    float f_tiledis=(weld_trace0[weld_trace0.size()-1].x()-endpoint.x())*(weld_trace0[weld_trace0.size()-1].x()-endpoint.x())+
                                    (weld_trace0[weld_trace0.size()-1].y()-endpoint.y())*(weld_trace0[weld_trace0.size()-1].y()-endpoint.y())+
                                    (weld_trace0[weld_trace0.size()-1].z()-endpoint.z())*(weld_trace0[weld_trace0.size()-1].z()-endpoint.z());
                    if(f_tiledis>f_headdis)//距离平面越来越远,说明交点放在头部
                    {
                        weld_trace0.insert(weld_trace0.begin(),endpoint);
                    }
                    else//距离平面越来越近，说明交点放在尾部
                    {
                        weld_trace0.push_back(endpoint);
                    }

                }
                break;
            }
            //规划后的轨道
            m_mcs->project->project_weld_trace[weld_trace_num].point=weld;

            if(m_mcs->e2proomdata.maindlg_SaveDatacheckBox!=0)//保存焊接轨迹
            {
                QString dir="./log/";
                QString key=SAVELOGFILE_CREATNAME_HEAD;
                QString time;
                std::string s_time;
                TimeFunction to;
                to.get_time_ms(&s_time);
                time=QString::fromStdString(s_time);
                dir=dir+time+key+name;
                savelog_creat(dir,m_mcs->project->project_weld_trace[weld_trace_num].point);
            }
        }
        else if(key==CMD_TRACE_KEY)//跟踪命令
        {
            QString name=cmd.cmd_trace_name;//获取到跟踪轨迹序号
            float speed=cmd.cmd_trace_speed;//获取到的跟踪速度
            int tcp=cmd.cmd_trace_tcp;//获取到跟踪TCP
            QString craftfilepath=cmd.cmd_trace_craftfilepath;//获取到工艺包的文件路径
            int weld_trace_num;//搜索到的焊接轨道序号
            std::vector<RobPos> weld;//轨道

            //这里添加移动命令
            for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
            {
                if(name==m_mcs->project->project_weld_trace[n].name)
                {
                    weld_trace_num=n;//找到要储存的焊接轨道下标
                    break;
                }
            }
        #if _MSC_VER
            QTextCodec *code = QTextCodec::codecForName("GBK");
        #else
            QTextCodec *code = QTextCodec::codecForName("UTF-8");
        #endif
            std::string fname = code->fromUnicode(craftfilepath).data();
            m_mcs->craft->LoadCraft((char*)fname.c_str());
            weld=m_mcs->project->project_weld_trace[weld_trace_num].point;
            //这里添加姿态
            switch(m_mcs->craft->craft_id)
            {
                case CRAFT_ID_FIXED_POSTURE://固定焊接姿态
                {
                    for(int n=0;n<weld.size();n++)
                    {
                        weld[n].X=weld[n].X+m_mcs->craft->posturelist[0].X;
                        weld[n].Y=weld[n].Y+m_mcs->craft->posturelist[0].Y;
                        weld[n].Z=weld[n].Z+m_mcs->craft->posturelist[0].Z;
                        weld[n].RX=m_mcs->craft->posturelist[0].RX;
                        weld[n].RY=m_mcs->craft->posturelist[0].RY;
                        weld[n].RZ=m_mcs->craft->posturelist[0].RZ;
                        weld[n].nEn=true;
                    }
                }
                break;
                case CRAFT_ID_STARTENDCHANGE_POSTURE://起终点变姿态
                {

                }
                break;
            }

            switch(m_mcs->craft->pendulum_mode)
            {
                case PENDULUM_ID_FLAT://平焊
                {
                }
                break;
            }

            m_mcs->project->project_weld_trace[weld_trace_num].point=weld;

            if(m_mcs->e2proomdata.maindlg_SaveDatacheckBox!=0)//保存焊接轨迹
            {
                QString dir="./log/";
                QString key=SAVELOGFILE_TRACENAME_HEAD;
                QString time;
                std::string s_time;
                TimeFunction to;
                to.get_time_ms(&s_time);
                time=QString::fromStdString(s_time);
                dir=dir+time+key+name;
                savelog_trace(dir,m_mcs->project->project_weld_trace[weld_trace_num].point);
            }
            for(int n=0;n<m_mcs->project->project_weld_trace[weld_trace_num].point.size();n++)
            {
                RobPos pos=m_mcs->project->project_weld_trace[weld_trace_num].point[n];
                cmd_move(pos,MOVEL,speed,tcp);//这里考虑暂停如何加
            }
            usleep(ROB_WORK_DELAY);
            while(m_mcs->rob->robot_state!=ROBOT_STATE_IDLE)//等待移动到位
            {
                if(b_cmdlist_build==false)     //停止
                {
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("手动停止进程");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    cmd_lock(1);
                    line=n;
                    return 1;
                }
                usleep(ROB_WORK_DELAY_STEP);
            }
        }
        if(b_cmdlist_build==false)//流程停止或暂停了
        {
            main_record.lock();
            return_msg=QString::fromLocal8Bit("手动停止进程");
            m_mcs->main_record.push_back(return_msg);
            main_record.unlock();
            line=n;
            return 1;
        }
    }
    b_cmdlist_build=false;
    u16data_elec_work=0;
    main_record.lock();
    return_msg=QString::fromLocal8Bit("指令执行结束");
    m_mcs->main_record.push_back(return_msg);
    main_record.unlock();
    line=m_mcs->project->project_cmdlist.size();
    return 0;
}

void toSendbuffer::cmdlist_stopbuild()
{
    b_cmdlist_build=false;
    u16data_elec_work=0;
}

void toSendbuffer::cmd_lock(int lock)
{
    send_group_robot.lock();
    sent_info_robot sendrob;
    sendrob.addr=ROB_STOP_REG_ADD;
    sendrob.ctx=m_mcs->rob->ctx_posget;
    sendrob.data.resize(1);
    sendrob.data[0]=lock;
    m_mcs->rob->b_send_group_robot=false;
    m_mcs->rob->send_group_robot.push_back(sendrob);
    m_mcs->rob->ctx_robot_dosomeing=DO_WRITE_TASK;
    send_group_robot.unlock();
}

void toSendbuffer::cmd_move(RobPos pos,Robmovemodel movemodel,float speed,int tcp)
{
    send_group_robot.lock();
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
    send_group_robot.unlock();
}

void toSendbuffer::cmd_delay(int time)
{
    int mtime=time*1000;//获取到延时时间转ms
    usleep(mtime);
}

void toSendbuffer::cmd_cam(int task,int work)
{
    send_group_leaser.lock();
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
    send_group_leaser.unlock();
}

void toSendbuffer::cmd_elec(float eled,Alternatingcurrent elem,int work)
{
    send_group_robot.lock();
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
    send_group_robot.unlock();
}

int toSendbuffer::savelog_scan(QString filename,std::vector<Scan_trace_line> trace)
{
    QString msg;
    QString format=".txt";
    QString pointUV="_pointUV";     //UV点
    QString pointYZ="_pointYZ";     //结果点
    QString linecloud="_linecloud"; //激光线点
    std::string name;

    QString filenamepointUV=filename+pointUV+format;
    QFile fp1(filenamepointUV);
    if(!fp1.open(QIODevice::WriteOnly))
        return -1;
    for(int n=0;n<trace.size();n++)
    {
        QString pointdata;
        for(int m=0;m<trace[n].ros_line.targetpointoutcloud.size();m++)
        {
            int u=trace[n].ros_line.targetpointoutcloud[m].u;
            int v=trace[n].ros_line.targetpointoutcloud[m].v;
            pointdata="pointUV"+QString::number(m)+"("+QString::number(u)+","+QString::number(v)+")";
        }
        msg="Line"+QString::number(n)+": "+pointdata+"\n";
        fp1.write(msg.toStdString().c_str());
    }
    fp1.close();

    QString filenamepointYZ=filename+pointYZ+format;
    QFile fp2(filenamepointYZ);
    if(!fp2.open(QIODevice::WriteOnly))
        return -1;
    for(int n=0;n<trace.size();n++)
    {
        QString pointdata;
        for(int m=0;m<trace[n].ros_line.targetpointoutcloud.size();m++)
        {
            float Y=trace[n].ros_line.targetpointoutcloud[m].x;
            float Z=trace[n].ros_line.targetpointoutcloud[m].y;
            pointdata="pointYZ"+QString::number(m)+"("+QString::number(Y,'f',ROBOT_POSE_DECIMAL_PLACE)+","+QString::number(Z,'f',ROBOT_POSE_DECIMAL_PLACE)+")";
        }
        msg="Line"+QString::number(n)+": "+pointdata+"\n";
        fp2.write(msg.toStdString().c_str());
    }
    fp2.close();

    QString filenamepointlinecloud=filename+linecloud+format;
    QFile fp3(filenamepointlinecloud);
    if(!fp3.open(QIODevice::WriteOnly))
        return -1;
    for(int n=0;n<trace.size();n++)
    {
        msg="Line"+QString::number(n)+":\n";
        fp3.write(msg.toStdString().c_str());
        for(int m=0;m<trace[n].ros_line.lasertrackoutcloud.size();m++)
        {
            int u=trace[n].ros_line.lasertrackoutcloud[m].u;
            int v=trace[n].ros_line.lasertrackoutcloud[m].v;
            float Y=trace[n].ros_line.lasertrackoutcloud[m].x;
            float Z=trace[n].ros_line.lasertrackoutcloud[m].y;
            QString linedata="U="+QString::number(u)+",V="+QString::number(v)+",Y="+QString::number(Y,'f',ROBOT_POSE_DECIMAL_PLACE)+",Z="+QString::number(Z,'f',ROBOT_POSE_DECIMAL_PLACE)+"\n";
            fp3.write(linedata.toStdString().c_str());
        }
    }
    fp3.close();;

    return 0;
}

int toSendbuffer::savelog_creat(QString filename,std::vector<RobPos> trace)
{
    QString msg;
    QString format=".txt";
    QString rob="_RobXYZ";

    QString filenamerob=filename+rob+format;
    QFile fp(filenamerob);
    if(!fp.open(QIODevice::WriteOnly))
        return -1;
    for(int n=0;n<trace.size();n++)
    {
        msg="Num"+QString::number(n)+": ("+QString::number(trace[n].X,'f',ROBOT_POSE_DECIMAL_PLACE)+","+QString::number(trace[n].Y,'f',ROBOT_POSE_DECIMAL_PLACE)+","+QString::number(trace[n].Z,'f',ROBOT_POSE_DECIMAL_PLACE)+")\n";
        fp.write(msg.toStdString().c_str());
    }
    fp.close();
    return 0;
}

int toSendbuffer::savelog_trace(QString filename,std::vector<RobPos> trace)
{
    QString msg;
    QString format=".txt";
    QString rob="_RobXYZRXRYRZ";

    QString filenamerob=filename+rob+format;
    QFile fp(filenamerob);
    if(!fp.open(QIODevice::WriteOnly))
        return -1;
    for(int n=0;n<trace.size();n++)
    {
        msg="Num"+QString::number(n)+": ("+QString::number(trace[n].X,'f',ROBOT_POSE_DECIMAL_PLACE)+","+QString::number(trace[n].Y,'f',ROBOT_POSE_DECIMAL_PLACE)+","+QString::number(trace[n].Z,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                  QString::number(trace[n].RX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+QString::number(trace[n].RY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+QString::number(trace[n].RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+")\n";
        fp.write(msg.toStdString().c_str());
    }
    fp.close();
    return 0;
}
