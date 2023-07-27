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
    m_mcs->project->projecr_robpos_trace.clear();
    m_mcs->project->projecr_coord_matrix4d.clear();
    m_mcs->project->project_interweld_trace.clear();
    for(int n=0;n<beforeline;n++)
    {
        QString msg,key;
        my_cmd cmd;
        int rc=cmd.decodecmd(m_mcs->project->project_cmdlist[n],msg,key);
        if(rc<=0)
        {
            if(key==CMD_MOV_KEY)//移动指令
            {
                QString change=cmd.cmd_move_change;//获取变换矩阵名字
                if(!change.isEmpty())//有变化矩阵
                {
                    bool b_find=false;
                    for(int t=0;t<m_mcs->project->projecr_coord_matrix4d.size();t++)
                    {
                        if(m_mcs->project->projecr_coord_matrix4d[t].name==change)
                        {
                            b_find=1;
                            break;
                        }
                    }
                    if(b_find==false)
                    {
                        err=1;
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+change+QString::fromLocal8Bit("的矩阵");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        errmsg.push_back(return_msg);
                    }
                }
            }
            else if(key==CMD_SMOV_KEY)//点位移动指令
            {
                QString change=cmd.cmd_smove_change;//获取变换矩阵名字
                Robmovemodel movemod=cmd.cmd_smove_movemod;//获取到的点位移动模式
                bool b_find=false;
                if(!change.isEmpty())//有变化矩阵
                {
                    bool b_find=false;
                    for(int t=0;t<m_mcs->project->projecr_coord_matrix4d.size();t++)
                    {
                        if(m_mcs->project->projecr_coord_matrix4d[t].name==change)
                        {
                            b_find=1;
                            break;
                        }
                    }
                    if(b_find==false)
                    {
                        err=1;
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+change+QString::fromLocal8Bit("的矩阵");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        errmsg.push_back(return_msg);
                    }
                }
                switch(movemod)
                {
                    case MOVEL:
                    case MOVEJ:
                    {
                        QString s_pos=cmd.cmd_smove_pos;//获取到移动坐标
                        b_find=false;
                        for(int t=0;t<m_mcs->project->projecr_robpos_trace.size();t++)
                        {
                            if(s_pos==m_mcs->project->projecr_robpos_trace[t].name)
                            {
                                b_find=true;
                                break;
                            }
                        }
                        if(b_find==false)//没找到s_pointx这个名字的扫描轨道
                        {
                            err=1;
                            main_record.lock();
                            return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+s_pos+QString::fromLocal8Bit("的坐标点");
                            m_mcs->main_record.push_back(return_msg);
                            main_record.unlock();
                            errmsg.push_back(return_msg);
                            break;
                        }
                    }
                    break;
                    case MOVEC:
                    {
                        QString s_pos1=cmd.cmd_smove_pos1;//获取到移动坐标
                        QString s_pos2=cmd.cmd_smove_pos2;//获取到移动坐标
                        QString s_pos3=cmd.cmd_smove_pos3;//获取到移动坐标
                        b_find=false;
                        for(int t=0;t<m_mcs->project->projecr_robpos_trace.size();t++)
                        {
                            if(s_pos1==m_mcs->project->projecr_robpos_trace[t].name)
                            {
                                b_find=true;
                                break;
                            }
                        }
                        if(b_find==false)//没找到s_pointx这个名字的扫描轨道
                        {
                            err=1;
                            main_record.lock();
                            return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+s_pos1+QString::fromLocal8Bit("的坐标点");
                            m_mcs->main_record.push_back(return_msg);
                            main_record.unlock();
                            errmsg.push_back(return_msg);
                            break;
                        }
                        b_find=false;
                        for(int t=0;t<m_mcs->project->projecr_robpos_trace.size();t++)
                        {
                            if(s_pos2==m_mcs->project->projecr_robpos_trace[t].name)
                            {
                                b_find=true;
                                break;
                            }
                        }
                        if(b_find==false)//没找到s_pointx这个名字的扫描轨道
                        {
                            err=1;
                            main_record.lock();
                            return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+s_pos2+QString::fromLocal8Bit("的坐标点");
                            m_mcs->main_record.push_back(return_msg);
                            main_record.unlock();
                            errmsg.push_back(return_msg);
                            break;
                        }
                        b_find=false;
                        for(int t=0;t<m_mcs->project->projecr_robpos_trace.size();t++)
                        {
                            if(s_pos3==m_mcs->project->projecr_robpos_trace[t].name)
                            {
                                b_find=true;
                                break;
                            }
                        }
                        if(b_find==false)//没找到s_pointx这个名字的扫描轨道
                        {
                            err=1;
                            main_record.lock();
                            return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+s_pos3+QString::fromLocal8Bit("的坐标点");
                            m_mcs->main_record.push_back(return_msg);
                            main_record.unlock();
                            errmsg.push_back(return_msg);
                            break;
                        }
                    }
                    break;
                }
            }
            else if(key==CMD_SCAN_KEY)//采集指令
            {
                QString name=cmd.cmd_scan_name;//获取到的扫描名字
                QString change=cmd.cmd_scan_change;//获取变换矩阵名字
                bool b_find=0;
                if(!change.isEmpty())//有变化矩阵
                {
                    bool b_find=false;
                    for(int t=0;t<m_mcs->project->projecr_coord_matrix4d.size();t++)
                    {
                        if(m_mcs->project->projecr_coord_matrix4d[t].name==change)
                        {
                            b_find=1;
                            break;
                        }
                    }
                    if(b_find==false)
                    {
                        err=1;
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+change+QString::fromLocal8Bit("的矩阵");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        errmsg.push_back(return_msg);
                    }
                }
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
            else if(key==CMD_SSCAN_KEY)//点位采集指令
            {
                QString name=cmd.cmd_sscan_name;//获取到的点位扫描名字
                QString change=cmd.cmd_sscan_change;//获取点位变换矩阵名字
                bool b_find=0;
                if(!change.isEmpty())//有变化矩阵
                {
                    bool b_find=false;
                    for(int t=0;t<m_mcs->project->projecr_coord_matrix4d.size();t++)
                    {
                        if(m_mcs->project->projecr_coord_matrix4d[t].name==change)
                        {
                            b_find=1;
                            break;
                        }
                    }
                    if(b_find==false)
                    {
                        err=1;
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+change+QString::fromLocal8Bit("的矩阵");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        errmsg.push_back(return_msg);
                    }
                }
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
                            else
                            {
                                Weld_trace_result trace;
                                trace.name=name;
                                m_mcs->project->project_weld_trace.push_back(trace);
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
                            else
                            {
                                Weld_trace_result trace;
                                trace.name=name;
                                m_mcs->project->project_weld_trace.push_back(trace);
                            }
                        }
                        break;
                        case TRACE_EDIT_MODE_TOWPOINT_THREE_TO_ONE: //两端点三直线交点模式
                        {
                            if(scannames.size()!=5)
                            {
                                err=1;
                                main_record.lock();
                                return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": ")+
                                           QString::fromLocal8Bit(CMD_MODE)+QString::fromLocal8Bit("值为")+QString::number(TRACE_EDIT_MODE_TOWPOINT_THREE_TO_ONE)+
                                           QString::fromLocal8Bit("时,")+QString::fromLocal8Bit(CMD_SCAN)+QString::fromLocal8Bit("项的参数只能有5个");
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
                            else
                            {
                                Weld_trace_result trace;
                                trace.name=name;
                                m_mcs->project->project_weld_trace.push_back(trace);
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
            else if(key==CMD_SEARCH_KEY)//起点寻位指令
            {
                QString name=cmd.cmd_search_name;//获取到的寻位点名字
                bool b_find=0;
                for(int t=0;t<m_mcs->project->projecr_robpos_trace.size();t++)
                {
                    if(m_mcs->project->projecr_robpos_trace[t].name==name)
                    {
                        b_find=1;
                        break;
                    }
                }
                if(b_find==1)
                {
                    err=1;
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 寻位点与已有的点重名");
                    m_mcs->main_record.push_back(return_msg);
                    errmsg.push_back(return_msg);
                    main_record.unlock();
                }
                else
                {
                    Point_robpos_result point;
                    point.name=name;
                    m_mcs->project->projecr_robpos_trace.push_back(point);
                }
            }
            else if(key==CMD_SEARCHEND_KEY)//终点寻位指令
            {
                QString name=cmd.cmd_searchend_name;//获取到的寻位点名字
                bool b_find=0;
                for(int t=0;t<m_mcs->project->projecr_robpos_trace.size();t++)
                {
                    if(m_mcs->project->projecr_robpos_trace[t].name==name)
                    {
                        b_find=1;
                        break;
                    }
                }
                if(b_find==1)
                {
                    err=1;
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 寻位点与已有的点重名");
                    m_mcs->main_record.push_back(return_msg);
                    errmsg.push_back(return_msg);
                    main_record.unlock();
                }
                else
                {
                    Point_robpos_result point;
                    point.name=name;
                    m_mcs->project->projecr_robpos_trace.push_back(point);
                }
            }
            else if(key==CMD_COORD_KEY)//工件零点定位矩阵指令
            {
                QString name=cmd.cmd_coord_name;//获取到的矩阵名字
                QString s_pointx=cmd.cmd_coord_pointx;
                QString s_pointo=cmd.cmd_coord_pointo;
                QString s_cpointx=cmd.cmd_coord_cpointx;
                QString s_cpointo=cmd.cmd_coord_cpointo;
                bool b_find=0;
                for(int t=0;t<m_mcs->project->projecr_coord_matrix4d.size();t++)
                {
                    if(m_mcs->project->projecr_coord_matrix4d[t].name==name)
                    {
                        b_find=1;
                        break;
                    }
                }
                if(b_find==1)
                {
                    err=1;
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 变换矩阵与已有的矩阵重名");
                    m_mcs->main_record.push_back(return_msg);
                    errmsg.push_back(return_msg);
                    main_record.unlock();
                }
                else
                {
                    b_find=false;
                    for(int t=0;t<m_mcs->project->projecr_robpos_trace.size();t++)
                    {
                        if(s_pointx==m_mcs->project->projecr_robpos_trace[t].name)
                        {
                            b_find=true;
                            break;
                        }
                    }
                    if(b_find==false)//没找到s_pointx这个名字的扫描轨道
                    {
                        err=1;
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+s_pointx+QString::fromLocal8Bit("的坐标点");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        errmsg.push_back(return_msg);
                        break;
                    }
                    b_find=false;
                    for(int t=0;t<m_mcs->project->projecr_robpos_trace.size();t++)
                    {
                        if(s_pointo==m_mcs->project->projecr_robpos_trace[t].name)
                        {
                            b_find=true;
                            break;
                        }
                    }
                    if(b_find==false)//没找到s_pointo这个名字的扫描轨道
                    {
                        err=1;
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+s_pointo+QString::fromLocal8Bit("的坐标点");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        errmsg.push_back(return_msg);
                        break;
                    }
                    b_find=false;
                    for(int t=0;t<m_mcs->project->projecr_robpos_trace.size();t++)
                    {
                        if(s_cpointx==m_mcs->project->projecr_robpos_trace[t].name)
                        {
                            b_find=true;
                            break;
                        }
                    }
                    if(b_find==false)//没找到s_cpointx这个名字的扫描轨道
                    {
                        err=1;
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+s_cpointx+QString::fromLocal8Bit("的坐标点");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        errmsg.push_back(return_msg);
                        break;
                    }
                    b_find=false;
                    for(int t=0;t<m_mcs->project->projecr_robpos_trace.size();t++)
                    {
                        if(s_cpointo==m_mcs->project->projecr_robpos_trace[t].name)
                        {
                            b_find=true;
                            break;
                        }
                    }
                    if(b_find==false)//没找到s_cpointo这个名字的扫描轨道
                    {
                        err=1;
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+s_cpointo+QString::fromLocal8Bit("的坐标点");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        errmsg.push_back(return_msg);
                        break;
                    }
                    if(s_pointx==s_pointo)
                    {
                        err=1;
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": ")+CMD_POINTX+QString::fromLocal8Bit("的参数项与")+CMD_POINTO+QString::fromLocal8Bit("的参数项相同");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        errmsg.push_back(return_msg);
                        break;
                    }
                    if(s_cpointx==s_cpointo)
                    {
                        err=1;
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": ")+CMD_CPOINTX+QString::fromLocal8Bit("的参数项与")+CMD_CPOINTO+QString::fromLocal8Bit("的参数项相同");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        errmsg.push_back(return_msg);
                        break;
                    }
                    Coord_Matrix4d_result matrix;
                    matrix.name=name;
                    m_mcs->project->projecr_coord_matrix4d.push_back(matrix);
                }
            }
            else if(key==CMD_GETPOS_KEY)
            {
                QString name=cmd.cmd_getpos_name;//获取到的点名字
                bool b_find=0;
                for(int t=0;t<m_mcs->project->projecr_robpos_trace.size();t++)
                {
                    if(m_mcs->project->projecr_robpos_trace[t].name==name)
                    {
                        b_find=1;
                        break;
                    }
                }
                if(b_find==1)
                {
                    err=1;
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 获取到的点与已有的点重名");
                    m_mcs->main_record.push_back(return_msg);
                    errmsg.push_back(return_msg);
                    main_record.unlock();
                }
                else
                {
                    Point_robpos_result point;
                    point.name=name;
                    m_mcs->project->projecr_robpos_trace.push_back(point);
                }
            }
            else if(key==CMD_SAMPLE_KEY)
            {
                QString creatname=cmd.cmd_sample_creatname;//获取到的要采样的轨迹名字
                QString name=cmd.cmd_sample_name;//获取到的采样结果名字
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
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 采样结果轨迹与已有的轨迹重名");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    errmsg.push_back(return_msg);
                }
                else
                {
                    b_find=0;
                    for(int t=0;t<m_mcs->project->project_weld_trace.size();t++)
                    {
                        if(creatname==m_mcs->project->project_weld_trace[t].name)
                        {
                            b_find=1;
                            break;
                        }
                    }
                    if(b_find==0)//没找到creatname这个名字的轨迹
                    {
                        err=1;
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+creatname+QString::fromLocal8Bit("的焊接轨迹");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        errmsg.push_back(return_msg);
                        break;
                    }
                    else
                    {
                        Weld_trace_result trace;
                        trace.name=name;
                        m_mcs->project->project_weld_trace.push_back(trace);
                    }
                }
            }
            else if(key==CMD_TRACE_KEY)
            {
                QString name_in=cmd.cmd_trace_name_in;//获取到跟踪轨迹序号
                QString name_out=cmd.cmd_trace_name_out;//获取到生成的跟踪轨迹序号
                bool b_find=0;

                b_find=false;
                for(int t=0;t<m_mcs->project->project_weld_trace.size();t++)
                {
                    if(name_in==m_mcs->project->project_weld_trace[t].name)
                    {
                        b_find=true;
                        break;
                    }
                }
                if(b_find==false)//没找到name_in这个名字的跟踪轨迹序号
                {
                    err=1;
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+name_in+QString::fromLocal8Bit("的跟踪轨迹");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    errmsg.push_back(return_msg);
                    break;
                }
                else
                {
                    bool b_find=0;
                    for(int t=0;t<m_mcs->project->project_interweld_trace.size();t++)
                    {
                        if(m_mcs->project->project_interweld_trace[t].name==name_out)
                        {
                            b_find=1;
                            break;
                        }
                    }
                    if(b_find==1)
                    {
                        err=1;
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 跟踪工艺轨迹与已有的轨迹重名");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        errmsg.push_back(return_msg);
                    }
                    else
                    {
                        Weld_tracing_result trace;
                        trace.name=name_out;
                        m_mcs->project->project_interweld_trace.push_back(trace);
                    }
                }
            }
            else if(key==CMD_TRACING_KEY)
            {
                QString name=cmd.cmd_tracing_name;//获取跟踪轨迹工艺名字
                bool b_find=false;
                for(int t=0;t<m_mcs->project->project_interweld_trace.size();t++)
                {
                    if(m_mcs->project->project_interweld_trace[t].name==name)
                    {
                        b_find=1;
                        break;
                    }
                }
                if(b_find==false)
                {
                    err=1;
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+name+QString::fromLocal8Bit("的跟踪轨迹工艺");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    errmsg.push_back(return_msg);
                }
            }
            else if(key==CMD_TRACEADD_KEY)
            {
                QString name1=cmd.cmd_traceadd_name1;//获取跟踪轨迹工艺1名字
                QString name2=cmd.cmd_traceadd_name2;//获取跟踪轨迹工艺2名字
                QString name_out=cmd.cmd_traceadd_nameout;
                bool b_find=false;
                for(int t=0;t<m_mcs->project->project_interweld_trace.size();t++)
                {
                    if(m_mcs->project->project_interweld_trace[t].name==name1)
                    {
                        b_find=1;
                        break;
                    }
                }
                if(b_find==false)
                {
                    err=1;
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+name1+QString::fromLocal8Bit("的跟踪轨迹工艺");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    errmsg.push_back(return_msg);
                }
                else
                {
                    bool b_find=false;
                    for(int t=0;t<m_mcs->project->project_interweld_trace.size();t++)
                    {
                        if(m_mcs->project->project_interweld_trace[t].name==name2)
                        {
                            b_find=1;
                            break;
                        }
                    }
                    if(b_find==false)
                    {
                        err=1;
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+name2+QString::fromLocal8Bit("的跟踪轨迹工艺");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        errmsg.push_back(return_msg);
                    }
                    else
                    {
                        b_find=0;
                        for(int t=0;t<m_mcs->project->project_interweld_trace.size();t++)
                        {
                            if(m_mcs->project->project_interweld_trace[t].name==name_out)
                            {
                                b_find=1;
                                break;
                            }
                        }
                        if(b_find==1)
                        {
                            err=1;
                            main_record.lock();
                            return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 跟踪工艺轨迹与已有的轨迹重名");
                            m_mcs->main_record.push_back(return_msg);
                            main_record.unlock();
                            errmsg.push_back(return_msg);
                        }
                        else
                        {
                            Weld_tracing_result trace;
                            trace.name=name_out;
                            m_mcs->project->project_interweld_trace.push_back(trace);
                        }
                    }
                }
            }
            else if(key==CMD_GOWELD_KEY)
            {
                QString name=cmd.cmd_goweld_name;//获取跟踪轨迹工艺名字
                bool b_find=false;
                for(int t=0;t<m_mcs->project->project_interweld_trace.size();t++)
                {
                    if(m_mcs->project->project_interweld_trace[t].name==name)
                    {
                        b_find=1;
                        break;
                    }
                }
                if(b_find==false)
                {
                    err=1;
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+name+QString::fromLocal8Bit("的跟踪轨迹工艺");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    errmsg.push_back(return_msg);
                }
            }
            else if(key==CMD_WAVE_KEY)
            {
                QString name_in=cmd.cmd_wave_namein;//获取跟踪轨迹工艺名字
                QString name_out=cmd.cmd_wave_nameout;//获取摆焊跟踪轨迹工艺名字
                bool b_find=false;
                for(int t=0;t<m_mcs->project->project_interweld_trace.size();t++)
                {
                    if(m_mcs->project->project_interweld_trace[t].name==name_in)
                    {
                        b_find=1;
                        break;
                    }
                }
                if(b_find==false)
                {
                    err=1;
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+name_in+QString::fromLocal8Bit("的跟踪轨迹工艺");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    errmsg.push_back(return_msg);
                }
                else
                {
                    b_find=0;
                    for(int t=0;t<m_mcs->project->project_interweld_trace.size();t++)
                    {
                        if(m_mcs->project->project_interweld_trace[t].name==name_out)
                        {
                            b_find=1;
                            break;
                        }
                    }
                    if(b_find==1)
                    {
                        err=1;
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 跟踪工艺轨迹与已有的轨迹重名");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        errmsg.push_back(return_msg);
                    }
                    else
                    {
                        Weld_tracing_result trace;
                        trace.name=name_out;
                        m_mcs->project->project_interweld_trace.push_back(trace);
                    }
                }
            }
            else if(key==CMD_CREATP_KEY)//用点生成轨迹指令
            {
                QString name=cmd.cmd_creatp_name;//获取到的生成的轨迹名字
                std::vector<QString> pointsname=cmd.cmd_creatp_pointsname;//获取到要点的轨道名字
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
                    if(pointsname.size()<1)
                    {
                        err=1;
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 点的参数个数至少要大于等于1个");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        errmsg.push_back(return_msg);
                    }
                    else
                    {
                        bool b_ok=true;
                        for(int n=0;n<pointsname.size();n++)
                        {
                            QString pointname=pointsname[n];
                            bool b_find=false;
                            for(int t=0;t<m_mcs->project->projecr_robpos_trace.size();t++)
                            {
                                if(m_mcs->project->projecr_robpos_trace[t].name==pointname)
                                {
                                    b_find=1;
                                    break;
                                }
                            }
                            if(b_find==false)
                            {
                                b_ok=false;
                                err=1;
                                main_record.lock();
                                return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+pointname+QString::fromLocal8Bit("的坐标点");
                                m_mcs->main_record.push_back(return_msg);
                                main_record.unlock();
                                errmsg.push_back(return_msg);
                            }
                        }
                        if(b_ok==true)
                        {
                            Weld_trace_result trace;
                            trace.name=name;
                            m_mcs->project->project_weld_trace.push_back(trace);
                        }
                    }
                }
            }
            else if(key==CMD_SETPOSE_KEY)
            {
                QString name_in=cmd.cmd_setpose_namein;//获取到的点坐标的名字
                QString name_out=cmd.cmd_setpose_nameout;//获取到的生成的点坐标的名字
                bool b_find=0;
                for(int t=0;t<m_mcs->project->projecr_robpos_trace.size();t++)
                {
                    if(m_mcs->project->projecr_robpos_trace[t].name==name_out)
                    {
                        b_find=1;
                        break;
                    }
                }
                if(b_find==1)
                {
                    err=1;
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 获取到的点与已有的点重名");
                    m_mcs->main_record.push_back(return_msg);
                    errmsg.push_back(return_msg);
                    main_record.unlock();
                }
                else
                {
                    b_find=false;
                    for(int t=0;t<m_mcs->project->projecr_robpos_trace.size();t++)
                    {
                        if(name_in==m_mcs->project->projecr_robpos_trace[t].name)
                        {
                            b_find=true;
                            break;
                        }
                    }
                    if(b_find==false)//没找到s_pointx这个名字的扫描轨道
                    {
                        err=1;
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+name_in+QString::fromLocal8Bit("的坐标点");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        errmsg.push_back(return_msg);
                        break;
                    }
                    else
                    {
                        Point_robpos_result point;
                        point.name=name_out;
                        m_mcs->project->projecr_robpos_trace.push_back(point);
                    }
                }
            }
            else if(key==CMD_GETTCPPOS_KEY)
            {
                QString name=cmd.cmd_gettcppos_name;//获取到的点名字
                bool b_find=0;
                for(int t=0;t<m_mcs->project->projecr_robpos_trace.size();t++)
                {
                    if(m_mcs->project->projecr_robpos_trace[t].name==name)
                    {
                        b_find=1;
                        break;
                    }
                }
                if(b_find==1)
                {
                    err=1;
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 获取到的点与已有的点重名");
                    m_mcs->main_record.push_back(return_msg);
                    errmsg.push_back(return_msg);
                    main_record.unlock();
                }
                else
                {
                    Point_robpos_result point;
                    point.name=name;
                    m_mcs->project->projecr_robpos_trace.push_back(point);
                }
            }
            else if(key==CMD_GETTCPPOS2_KEY)
            {
                QString name=cmd.cmd_gettcppos2_name;//获取到的点名字
                bool b_find=0;
                for(int t=0;t<m_mcs->project->projecr_robpos_trace.size();t++)
                {
                    if(m_mcs->project->projecr_robpos_trace[t].name==name)
                    {
                        b_find=1;
                        break;
                    }
                }
                if(b_find==1)
                {
                    err=1;
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 获取到的点与已有的点重名");
                    m_mcs->main_record.push_back(return_msg);
                    errmsg.push_back(return_msg);
                    main_record.unlock();
                }
                else
                {
                    Point_robpos_result point;
                    point.name=name;
                    m_mcs->project->projecr_robpos_trace.push_back(point);
                }
            }
            else if(key==CMD_CREATF_KEY)
            {
                QString name=cmd.cmd_creatf_name;//获取到生成的跟踪轨迹序号
                bool b_find=0;
                for(int t=0;t<m_mcs->project->project_interweld_trace.size();t++)
                {
                    if(m_mcs->project->project_interweld_trace[t].name==name)
                    {
                        b_find=1;
                        break;
                    }
                }
                if(b_find==1)
                {
                    err=1;
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 跟踪工艺轨迹与已有的轨迹重名");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    errmsg.push_back(return_msg);
                }
                else
                {
                    Weld_tracing_result trace;
                    trace.name=name;
                    m_mcs->project->project_interweld_trace.push_back(trace);
                }
            }
            else if(key==CMD_PLOTPOS_KEY)
            {
                QString name=cmd.cmd_plotpos_name;//获取到的生成的点名字
                std::vector<QString> creatsnames=cmd.cmd_plotpos_creatname;//获取到要计算用的轨道名字
                std::vector<QString> pointsnames=cmd.cmd_plotpos_pointsname;//获得到要计算用的点的名字
                Plotpos_edit_mode mode=cmd.cmd_plotpos_mode;//获取到的计算模式
                bool b_find=0;
                for(int t=0;t<m_mcs->project->projecr_robpos_trace.size();t++)
                {
                    if(m_mcs->project->projecr_robpos_trace[t].name==name)
                    {
                        b_find=1;
                        break;
                    }
                }
                if(b_find==1)
                {
                    err=1;
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 获取到的点与已有的点重名");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    errmsg.push_back(return_msg);
                }
                else
                {
                    switch(mode)
                    {
                        case PLOTPOS_EDIT_MODE_THREE_TO_ONE:    //三直线交点模式
                        {
                            if(creatsnames.size()!=3)
                            {
                                err=1;
                                main_record.lock();
                                return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": ")+
                                           QString::fromLocal8Bit(CMD_MODE)+QString::fromLocal8Bit("值为")+QString::number(PLOTPOS_EDIT_MODE_THREE_TO_ONE)+
                                           QString::fromLocal8Bit("时,")+QString::fromLocal8Bit(CMD_CREATS)+QString::fromLocal8Bit("项的参数只能有3个");
                                m_mcs->main_record.push_back(return_msg);
                                main_record.unlock();
                                errmsg.push_back(return_msg);
                                break;
                            }
                            bool b_find=false;
                            int m=0;
                            for(m=0;m<creatsnames.size();m++)
                            {
                                b_find=false;
                                for(int t=0;t<m_mcs->project->project_weld_trace.size();t++)
                                {
                                    if(creatsnames[m]==m_mcs->project->project_weld_trace[t].name)
                                    {
                                        b_find=true;
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
                                return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+creatsnames[m]+QString::fromLocal8Bit("的跟踪轨道");
                                m_mcs->main_record.push_back(return_msg);
                                main_record.unlock();
                                errmsg.push_back(return_msg);
                                break;
                            }
                            else
                            {
                                Point_robpos_result point;
                                point.name=name;
                                m_mcs->project->projecr_robpos_trace.push_back(point);
                            }
                        }
                        break;
                        case PLOTPOS_EDIT_MODE_FIVEPOINTS_TO_ONE:
                        {
                            if(pointsnames.size()!=5)
                            {
                                err=1;
                                main_record.lock();
                                return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": ")+
                                           QString::fromLocal8Bit(CMD_MODE)+QString::fromLocal8Bit("值为")+QString::number(PLOTPOS_EDIT_MODE_FIVEPOINTS_TO_ONE)+
                                           QString::fromLocal8Bit("时,")+QString::fromLocal8Bit(CMD_POINTS)+QString::fromLocal8Bit("项的参数只能有5个");
                                m_mcs->main_record.push_back(return_msg);
                                main_record.unlock();
                                errmsg.push_back(return_msg);
                                break;
                            }
                            bool b_find=false;
                            int m=0;
                            for(m=0;m<pointsnames.size();m++)
                            {
                                b_find=false;
                                for(int t=0;t<m_mcs->project->projecr_robpos_trace.size();t++)
                                {
                                    if(pointsnames[m]==m_mcs->project->projecr_robpos_trace[t].name)
                                    {
                                        b_find=true;
                                        break;
                                    }
                                }
                                if(b_find==false)//没找到这个名字的点坐标
                                {
                                    break;
                                }
                            }
                            if(b_find==false)
                            {
                                err=1;
                                main_record.lock();
                                return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+pointsnames[m]+QString::fromLocal8Bit("的点坐标");
                                m_mcs->main_record.push_back(return_msg);
                                main_record.unlock();
                                errmsg.push_back(return_msg);
                                break;
                            }
                            else
                            {
                                Point_robpos_result point;
                                point.name=name;
                                m_mcs->project->projecr_robpos_trace.push_back(point);
                            }
                        }
                        break;
                        default:
                        {
                            err=1;
                            main_record.lock();
                            return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 不支持当前点的生成模式");
                            m_mcs->main_record.push_back(return_msg);
                            main_record.unlock();
                            errmsg.push_back(return_msg);
                        }
                        break;
                    }
                }
            }
            else if(key==CMD_CREATC_KEY)//用点生成圆弧轨迹指令
            {
                QString name=cmd.cmd_creatc_name;//获取到的生成的轨迹名字
                std::vector<QString> pointsname=cmd.cmd_creatc_pointsname;//获取到要生成的轨道所用的点名字
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
                    if(pointsname.size()!=3)
                    {
                        err=1;
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 点的参数个数有且只有3个");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        errmsg.push_back(return_msg);
                    }
                    else
                    {
                        bool b_ok=true;
                        for(int n=0;n<pointsname.size();n++)
                        {
                            QString pointname=pointsname[n];
                            bool b_find=false;
                            for(int t=0;t<m_mcs->project->projecr_robpos_trace.size();t++)
                            {
                                if(m_mcs->project->projecr_robpos_trace[t].name==pointname)
                                {
                                    b_find=1;
                                    break;
                                }
                            }
                            if(b_find==false)
                            {
                                b_ok=false;
                                err=1;
                                main_record.lock();
                                return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+pointname+QString::fromLocal8Bit("的坐标点");
                                m_mcs->main_record.push_back(return_msg);
                                main_record.unlock();
                                errmsg.push_back(return_msg);
                            }
                        }
                        if(b_ok==true)
                        {
                            Weld_trace_result trace;
                            trace.name=name;
                            m_mcs->project->project_weld_trace.push_back(trace);
                        }
                    }
                }
            }
            else if(key==CMD_TRACECONTINUE_KEY)
            {
                QString name_in=cmd.cmd_tracecontinue_namein;//获取到的生成的轨迹名字
                QString name_out=cmd.cmd_tracecontinue_nameout;
                bool b_find=false;
                for(int t=0;t<m_mcs->project->project_interweld_trace.size();t++)
                {
                    if(m_mcs->project->project_interweld_trace[t].name==name_in)
                    {
                        b_find=1;
                        break;
                    }
                }
                if(b_find==false)
                {
                    err=1;
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+name_in+QString::fromLocal8Bit("的跟踪轨迹工艺");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    errmsg.push_back(return_msg);
                }
                else
                {
                    b_find=0;
                    for(int t=0;t<m_mcs->project->project_interweld_trace.size();t++)
                    {
                        if(m_mcs->project->project_interweld_trace[t].name==name_out)
                        {
                            b_find=1;
                            break;
                        }
                    }
                    if(b_find==1)
                    {
                        err=1;
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 跟踪工艺轨迹与已有的轨迹重名");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        errmsg.push_back(return_msg);
                    }
                    else
                    {
                        Weld_tracing_result trace;
                        trace.name=name_out;
                        m_mcs->project->project_interweld_trace.push_back(trace);
                    }
                }
            }
            else if(key==CMD_FILTER_KEY)
            {
                QString name_in=cmd.cmd_filter_namein;//获取到的滤波的轨迹名字
                QString name_out=cmd.cmd_filter_nameout;
                bool b_find=0;
                for(int t=0;t<m_mcs->project->project_weld_trace.size();t++)
                {
                    if(m_mcs->project->project_weld_trace[t].name==name_out)
                    {
                        b_find=1;
                        break;
                    }
                }
                if(b_find==1)
                {
                    err=1;
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 滤波结果轨迹与已有的轨迹重名");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    errmsg.push_back(return_msg);
                }
                else
                {
                    b_find=0;
                    for(int t=0;t<m_mcs->project->project_weld_trace.size();t++)
                    {
                        if(name_in==m_mcs->project->project_weld_trace[t].name)
                        {
                            b_find=1;
                            break;
                        }
                    }
                    if(b_find==0)//没找到name_in这个名字的轨迹
                    {
                        err=1;
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+name_in+QString::fromLocal8Bit("的焊接轨迹");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        errmsg.push_back(return_msg);
                        break;
                    }
                    else
                    {
                        Weld_trace_result trace;
                        trace.name=name_out;
                        m_mcs->project->project_weld_trace.push_back(trace);
                    }
                }
            }
            else if(key==CMD_GOTO_KEY)
            {
                int goto_line=cmd.cmd_goto_line;
                if(goto_line==n)
                {
                    err=1;
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+QString::fromLocal8Bit("无法跳转到本身所在行");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    errmsg.push_back(return_msg);
                    break;
                }
                else if(goto_line>=beforeline)
                {
                    err=1;
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+QString::fromLocal8Bit("跳转行数超过总行数");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    break;
                }
            }
            else if(key==CMD_CREATADD_KEY)
            {
                QString name=cmd.cmd_creatadd_nameout;//获取到的生成的轨迹名字
                std::vector<QString> creatsnames=cmd.cmd_creatadd_names;//获取到要计算用的轨道名字
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
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 获取到的轨迹与已有的轨迹重名");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    errmsg.push_back(return_msg);
                }
                else
                {
                    bool b_find=false;
                    int m=0;
                    for(m=0;m<creatsnames.size();m++)
                    {
                        b_find=false;
                        for(int t=0;t<m_mcs->project->project_weld_trace.size();t++)
                        {
                            if(creatsnames[m]==m_mcs->project->project_weld_trace[t].name)
                            {
                                b_find=true;
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
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+creatsnames[m]+QString::fromLocal8Bit("的跟踪轨道");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        errmsg.push_back(return_msg);
                        break;
                    }
                    else
                    {
                        Weld_trace_result weld;
                        weld.name=name;
                        m_mcs->project->project_weld_trace.push_back(weld);
                    }
                }
            }
            else if(key==CMD_CREATADDP_KEY)
            {
                QString name=cmd.cmd_creataddp_nameout;//获取到的生成的轨迹名字
                QString weldname=cmd.cmd_creataddp_weldname;//获取到的轨迹名字
                QString pointname=cmd.cmd_creataddp_pointname;//获取到的点名字
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
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 获取到的轨迹与已有的轨迹重名");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    errmsg.push_back(return_msg);
                }
                else
                {
                    bool b_find=false;
                    for(int t=0;t<m_mcs->project->project_weld_trace.size();t++)
                    {
                        if(weldname==m_mcs->project->project_weld_trace[t].name)
                        {
                            b_find=true;
                            break;
                        }
                    }
                    if(b_find==false)
                    {
                        err=1;
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+weldname+QString::fromLocal8Bit("的跟踪轨道");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        errmsg.push_back(return_msg);
                        break;
                    }
                    b_find=false;
                    for(int t=0;t<m_mcs->project->projecr_robpos_trace.size();t++)
                    {
                        if(pointname==m_mcs->project->projecr_robpos_trace[t].name)
                        {
                            b_find=true;
                            break;
                        }
                    }
                    if(b_find==false)
                    {
                        err=1;
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 前面没有名为")+pointname+QString::fromLocal8Bit("的跟踪轨道");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        errmsg.push_back(return_msg);
                        break;
                    }
                    Weld_trace_result weld;
                    weld.name=name;
                    m_mcs->project->project_weld_trace.push_back(weld);
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
                if(rc==1)
                {
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 该路径找不到焊接工艺参数文件");
                }
                else
                {
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 焊接工艺参数文件格式出错");
                }
                m_mcs->main_record.push_back(return_msg);
                main_record.unlock();
            }
        }
        if(key==CMD_MOV_KEY)
        {
            int tcp=cmd.cmd_move_tcp;//获取到移动TCP
            float speed=cmd.cmd_move_speed;//获取到速度值
            Robmovemodel movemod=cmd.cmd_move_movemod;//获取到的移动模式
        #ifdef USE_MYMOVEC_CONTROL
            if(movemod==MOVEC)
            {
                CWeldTarject tarjectMath;
                std::vector<RobPos> interpolatPos;
                RobPos pos1=cmd.cmd_move_pos1;//获取到移动坐标
                RobPos pos2=cmd.cmd_move_pos2;//获取到移动坐标
                RobPos pos3=cmd.cmd_move_pos3;//获取到移动坐标
                if(false==tarjectMath.pos_circle(m_mcs->rob->cal_posture_model,pos1,pos2,pos3,interpolatPos,ROBOT_POSE_MOVEC_STEP,16,speed))
                {
                    err=1;
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 圆弧三点轨迹拟合出错");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                }
            }
        #endif
        }
        if(key==CMD_CREATF_KEY)
        {
            std::vector<RobPos> pos;
            QString filename=cmd.cmd_creatf_filename;//获取到轨迹的文件路径
            int rc=loadlog_creat(filename,pos);
            if(rc!=0)
            {
                err=1;
                main_record.lock();
                if(rc==1)
                {
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 该路径找不到轨迹文件");
                }
                else
                {
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 轨迹的文件格式出错");
                }
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

int toSendbuffer::cmdlist_skip(int stline)
{
    QString return_msg;
    for(int n=0;n<=stline;n++)
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
            return 1;
        }
        else if(rc<0)//注释行
        {
            continue;
        }
        if(key==CMD_SEARCH_KEY)//获取到的寻位
        {
            QString name=cmd.cmd_search_name;//获取到的寻位名字
            int robpos_trace_num;
            for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
            {
                if(name==m_mcs->project->projecr_robpos_trace[n].name)
                {
                    robpos_trace_num=n;//找到要储存的寻位点下标
                    break;
                }
            }
            if(0!=loadtemp_pos(m_mcs->project->projecr_robpos_trace[robpos_trace_num]))
            {
                main_record.lock();
                return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+name+QString::fromLocal8Bit("没有获取到有效值");
                m_mcs->main_record.push_back(return_msg);
                main_record.unlock();
                return 1;
            }
        }
        else if(key==CMD_SEARCHEND_KEY)//获取到的寻位终点
        {
            QString name=cmd.cmd_searchend_name;//获取到的寻位名字
            int robpos_trace_num;
            for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
            {
                if(name==m_mcs->project->projecr_robpos_trace[n].name)
                {
                    robpos_trace_num=n;//找到要储存的寻位点下标
                    break;
                }
            }
            if(0!=loadtemp_pos(m_mcs->project->projecr_robpos_trace[robpos_trace_num]))
            {
                main_record.lock();
                return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+name+QString::fromLocal8Bit("没有获取到有效值");
                m_mcs->main_record.push_back(return_msg);
                main_record.unlock();
                return 1;
            }
        }
        else if(key==CMD_GETPOS_KEY)
        {
            QString name=cmd.cmd_getpos_name;//获取到的寻位名字
            int robpos_trace_num;
            for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
            {
                if(name==m_mcs->project->projecr_robpos_trace[n].name)
                {
                    robpos_trace_num=n;//找到要储存的寻位点下标
                    break;
                }
            }
            if(0!=loadtemp_pos(m_mcs->project->projecr_robpos_trace[robpos_trace_num]))
            {
                main_record.lock();
                return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+name+QString::fromLocal8Bit("没有获取到有效值");
                m_mcs->main_record.push_back(return_msg);
                main_record.unlock();
                return 1;
            }
        }
        else if(key==CMD_SCAN_KEY)
        {
            QString name=cmd.cmd_scan_name;//获取到的寻位名字
            int scan_trace_num;
            for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
            {
                if(name==m_mcs->project->project_scan_trace[n].name)
                {
                    scan_trace_num=n;//找到要储存的寻位点下标
                    break;
                }
            }
            if(0!=loadtemp_scan(m_mcs->project->project_scan_trace[scan_trace_num]))
            {
                main_record.lock();
                return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+name+QString::fromLocal8Bit("没有获取到有效值");
                m_mcs->main_record.push_back(return_msg);
                main_record.unlock();
                return 1;
            }
        }
        else if(key==CMD_SSCAN_KEY)
        {
            QString name=cmd.cmd_sscan_name;//获取到的寻位名字
            int scan_trace_num;
            for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
            {
                if(name==m_mcs->project->project_scan_trace[n].name)
                {
                    scan_trace_num=n;//找到要储存的寻位点下标
                    break;
                }
            }
            if(0!=loadtemp_scan(m_mcs->project->project_scan_trace[scan_trace_num]))
            {
                main_record.lock();
                return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+name+QString::fromLocal8Bit("没有获取到有效值");
                m_mcs->main_record.push_back(return_msg);
                main_record.unlock();
                return 1;
            }
        }
        else if(key==CMD_GETTCPPOS_KEY)
        {
            QString name=cmd.cmd_gettcppos_name;//获取到的寻位名字
            int robpos_trace_num;
            for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
            {
                if(name==m_mcs->project->projecr_robpos_trace[n].name)
                {
                    robpos_trace_num=n;//找到要储存的寻位点下标
                    break;
                }
            }
            if(0!=loadtemp_pos(m_mcs->project->projecr_robpos_trace[robpos_trace_num]))
            {
                main_record.lock();
                return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+name+QString::fromLocal8Bit("没有获取到有效值");
                m_mcs->main_record.push_back(return_msg);
                main_record.unlock();
                return 1;
            }
        }
        else if(key==CMD_GOTO_KEY)
        {
            int goto_line=cmd.cmd_goto_line;
            n=goto_line;
        }
        else if(key==CMD_STOP_KEY)//流程停止了
        {
            main_record.lock();
            return_msg=QString::fromLocal8Bit("代码中停止进程");
            m_mcs->main_record.push_back(return_msg);
            main_record.unlock();
            return 1;
        }
        //其他指令正常计算
        else
        {
            if(0!=slopbuild(m_mcs->project->project_cmdlist[n],n,return_msg))
            {
                main_record.lock();
                m_mcs->main_record.push_back(return_msg);
                main_record.unlock();
                return 1;
            }
        }
        //其他控制指令忽略
    }
    return 0;
}

int toSendbuffer::slopbuild(QString list,int n,QString &return_msg)
{
    QString msg,key;
    my_cmd cmd;
    int rc=cmd.decodecmd(list,msg,key);

    if(key==CMD_TRACE_KEY)
    {
        QString name_in=cmd.cmd_trace_name_in;//获取到跟踪轨迹序号
        QString name_out=cmd.cmd_trace_name_out;//获取到生成的跟踪轨迹序号
        float speed=cmd.cmd_trace_speed;//获取到的跟踪速度
        QString craftfilepath=cmd.cmd_trace_craftfilepath;//获取到工艺包的文件路径
        int weld_trace_num_in;//搜索到的焊接轨道序号
        int weld_trace_num_out;//搜索到的焊接轨道序号
        std::vector<RobPos> interpolatweld;//轨道
        bool Sample;

        //这里添加移动命令
        for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
        {
            if(name_in==m_mcs->project->project_weld_trace[n].name)
            {
                weld_trace_num_in=n;//找到焊接轨道下标
                break;
            }
        }
        for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
        {
            if(name_out==m_mcs->project->project_interweld_trace[n].name)
            {
                weld_trace_num_out=n;//找到要储存的焊接工艺焊接轨道下标
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
        interpolatweld=m_mcs->project->project_weld_trace[weld_trace_num_in].point;
        Sample=m_mcs->project->project_weld_trace[weld_trace_num_in].Sample;
        //这里添加姿态
        switch(m_mcs->craft->craft_id)
        {
            case CRAFT_ID_FIXED_POSTURE://固定焊接姿态
            {
                for(int n=0;n<interpolatweld.size();n++)
                {
                    interpolatweld[n].X=interpolatweld[n].X+m_mcs->craft->posturelist[0].Variable.X;
                    interpolatweld[n].Y=interpolatweld[n].Y+m_mcs->craft->posturelist[0].Variable.Y;
                    interpolatweld[n].Z=interpolatweld[n].Z+m_mcs->craft->posturelist[0].Variable.Z;
                    interpolatweld[n].RX=m_mcs->craft->posturelist[0].posture.RX;
                    interpolatweld[n].RY=m_mcs->craft->posturelist[0].posture.RY;
                    interpolatweld[n].RZ=m_mcs->craft->posturelist[0].posture.RZ;
                    interpolatweld[n].nEn=true;
                }


                /*********************************/
                /*
                //只保留起终点，为了库卡演示好看
                std::vector<RobPos> temp_interpolatweld(2);
                if(interpolatweld.size()>=10)
                {
                   temp_interpolatweld[0]=interpolatweld[5];
                   temp_interpolatweld[1]=interpolatweld[interpolatweld.size()-5];
                }
                interpolatweld=temp_interpolatweld;
                */
                /********************************/
            }
            break;
            case CRAFT_ID_STARTENDCHANGE_POSTURE://起终点变姿态
            {
                std::vector<ChangeRobPosVariable> tempposturelist;
                QString msg;
                //整理姿态位置
                if(0!=m_mcs->craft->tidyup_posturelist(m_mcs->craft->posturelist,tempposturelist,msg))
                {
                    main_record.lock();
                    return_msg=msg;
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    return 1;
                }
                if(interpolatweld.size()==1)
                {
                    interpolatweld[0].X=interpolatweld[0].X+tempposturelist[0].Variable.X;
                    interpolatweld[0].Y=interpolatweld[0].Y+tempposturelist[0].Variable.Y;
                    interpolatweld[0].Z=interpolatweld[0].Z+tempposturelist[0].Variable.Z;
                    interpolatweld[0].RX=tempposturelist[0].posture.RX;
                    interpolatweld[0].RY=tempposturelist[0].posture.RY;
                    interpolatweld[0].RZ=tempposturelist[0].posture.RZ;
                    interpolatweld[0].nEn=true;
                }
                else if(interpolatweld.size()>=2)
                {
                    Eigen::Vector3d RealpointST(interpolatweld[0].X,interpolatweld[0].Y,interpolatweld[0].Z);
                    Eigen::Vector3d RealpointED(interpolatweld[interpolatweld.size()-1].X,interpolatweld[interpolatweld.size()-1].Y,interpolatweld[interpolatweld.size()-1].Z);
                    Eigen::Vector3d Realpointvector=RealpointED-RealpointST;//实际焊缝方向
                    double Realdistance=Realpointvector.norm();//实际焊缝长度

                    Eigen::Vector3d PointST(tempposturelist[0].posture.X,tempposturelist[0].posture.Y,tempposturelist[0].posture.Z);
                    Eigen::Vector3d PointED(tempposturelist[tempposturelist.size()-1].posture.X,tempposturelist[tempposturelist.size()-1].posture.Y,tempposturelist[tempposturelist.size()-1].posture.Z);
                    Eigen::Vector3d Pointvector=PointED-PointST;//示教姿态方向
                    double Distance=Pointvector.norm();//示教姿态长度

                    if(fabs(Realdistance-Distance)>STARTENDCHANGE_POSTURE_ALLOWERROR)
                    {
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 工艺的变姿态轨迹长度与实际测得的轨迹长度误差大于")+QString::number(STARTENDCHANGE_POSTURE_ALLOWERROR);
                        main_record.unlock();
                        return 1;
                    }

                    if(Distance!=0)
                    {
                        int N=1;
                        int lastN=0;
                        std::vector<RobPos> tempinterpolatweld(interpolatweld.size());
                        for(int t=1;t<tempposturelist.size()-1;t++)
                        {
                            Eigen::Vector3d pointcenter(tempposturelist[t].posture.X,tempposturelist[t].posture.Y,tempposturelist[t].posture.Z);
                            Eigen::Vector3d pointcentervector=pointcenter-PointST;
                            double distance=pointcentervector.norm();
                            double specific=distance/Distance;//比例
                            double realdistance=Realdistance*specific;
                            for(;N<interpolatweld.size()-1;N++)
                            {
                                Eigen::Vector3d realpointcenter(interpolatweld[N].X,interpolatweld[N].Y,interpolatweld[N].Z);
                                Eigen::Vector3d realpointcentervector=realpointcenter-RealpointST;
                                double temprealdistance=realpointcentervector.norm();
                                if(temprealdistance>realdistance)//定位成功
                                {
                                    Eigen::Vector3d st(interpolatweld[lastN].X+tempposturelist[t-1].Variable.X,interpolatweld[lastN].Y+tempposturelist[t-1].Variable.Y,interpolatweld[lastN].Z+tempposturelist[t-1].Variable.Z);//起点
                                    Eigen::Vector3d ed(interpolatweld[N].X+tempposturelist[t].Variable.X,interpolatweld[N].Y+tempposturelist[t].Variable.Y,interpolatweld[N].Z+tempposturelist[t].Variable.Z);//终点
                                    Eigen::Vector3d stepdif=(ed-st)/(N-lastN);//变姿态补偿
                                    Eigen::Vector3d stR(tempposturelist[t-1].posture.RX,tempposturelist[t-1].posture.RY,tempposturelist[t-1].posture.RZ);
                                    Eigen::Vector3d edR(tempposturelist[t].posture.RX,tempposturelist[t].posture.RY,tempposturelist[t].posture.RZ);
                                    std::vector<Eigen::Vector3d> posR=Calibration::Attitudedifference(m_mcs->rob->cal_posture_model,stR,edR,N-lastN+1);
                                    for(int m=lastN;m<=N;m++)
                                    {
                                        Eigen::Vector3d p=stepdif*(m-lastN)+st;
                                        tempinterpolatweld[m].X=p.x();
                                        tempinterpolatweld[m].Y=p.y();
                                        tempinterpolatweld[m].Z=p.z();
                                        tempinterpolatweld[m].RX=posR[m-lastN][0];
                                        tempinterpolatweld[m].RY=posR[m-lastN][1];
                                        tempinterpolatweld[m].RZ=posR[m-lastN][2];
                                    }
                                    lastN=N;
                                    break;
                                }
                            }
                        }
                        //加上终点姿态
                        Eigen::Vector3d st(interpolatweld[lastN].X+tempposturelist[tempposturelist.size()-2].Variable.X,interpolatweld[lastN].Y+tempposturelist[tempposturelist.size()-2].Variable.Y,interpolatweld[lastN].Z+tempposturelist[tempposturelist.size()-2].Variable.Z);//起点
                        Eigen::Vector3d ed(interpolatweld[interpolatweld.size()-1].X+tempposturelist[tempposturelist.size()-1].Variable.X,interpolatweld[interpolatweld.size()-1].Y+tempposturelist[tempposturelist.size()-1].Variable.Y,interpolatweld[interpolatweld.size()-1].Z+tempposturelist[tempposturelist.size()-1].Variable.Z);//终点
                        Eigen::Vector3d stepdif=(ed-st)/(interpolatweld.size()-1-lastN);//变姿态补偿
                        Eigen::Vector3d stR(tempposturelist[tempposturelist.size()-2].posture.RX,tempposturelist[tempposturelist.size()-2].posture.RY,tempposturelist[tempposturelist.size()-2].posture.RZ);
                        Eigen::Vector3d edR(tempposturelist[tempposturelist.size()-1].posture.RX,tempposturelist[tempposturelist.size()-1].posture.RY,tempposturelist[tempposturelist.size()-1].posture.RZ);
                        std::vector<Eigen::Vector3d> posR=Calibration::Attitudedifference(m_mcs->rob->cal_posture_model,stR,edR,interpolatweld.size()-lastN);
                        for(int m=lastN;m<interpolatweld.size();m++)
                        {
                            Eigen::Vector3d p=stepdif*(m-lastN)+st;
                            tempinterpolatweld[m].X=p.x();
                            tempinterpolatweld[m].Y=p.y();
                            tempinterpolatweld[m].Z=p.z();
                            tempinterpolatweld[m].RX=posR[m-lastN][0];
                            tempinterpolatweld[m].RY=posR[m-lastN][1];
                            tempinterpolatweld[m].RZ=posR[m-lastN][2];
                        }
                        interpolatweld=tempinterpolatweld;
                    }
                    else
                    {
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 轨迹姿态插值出错");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        return 1;
                    }
                }
            }
            break;
            case CRAFT_ID_LASERNORMAL_POSTURE: //激光器测量法线姿态
            {
                for(int n=0;n<interpolatweld.size();n++)
                {
                    interpolatweld[n].X=interpolatweld[n].X+m_mcs->craft->posturelist[0].Variable.X;
                    interpolatweld[n].Y=interpolatweld[n].Y+m_mcs->craft->posturelist[0].Variable.Y;
                    interpolatweld[n].Z=interpolatweld[n].Z+m_mcs->craft->posturelist[0].Variable.Z;
                    interpolatweld[n].nEn=true;
                }
            }
            break;
            case CRAFT_ID_CORRUGATED_POSTURE: //波纹板变姿态
            {
                //整理波纹板平坡和上坡数据
                cv::Mat dif=cv::Mat::zeros(1,interpolatweld.size(),CV_32FC1);
                float *f_dif=dif.ptr<float>(0);
                switch(m_mcs->craft->weld_direction)
                {
                    case WELD_DIRECTION_X:
                    {
                        for(int j = 0;j < interpolatweld.size();j ++)
                        {
                            f_dif[j]=interpolatweld[j].Y;
                        }
                    }
                    break;
                    case WELD_DIRECTION_Y:
                    {
                        for(int j = 0;j < interpolatweld.size();j ++)
                        {
                            f_dif[j]=interpolatweld[j].X;
                        }
                    }
                    break;
                }
                //卷积窗
                if(interpolatweld.size()>40)
                {
                    cv::Mat kern = (cv::Mat_<float>(1,31) << 1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,-1,-1,-1,-1);
                    cv::Mat dstImage;
                    filter2D(dif,dstImage,CV_32F,kern);
                    float *f_res=dstImage.ptr<float>(0);
                    double d_value=0;
                    int n_d_value=0;
                    for(int j = 20;j < interpolatweld.size()-20;j ++)
                    {
                        d_value=abs(f_res[j])+d_value;
                        n_d_value++;
                    }
                    d_value=d_value/n_d_value;

                    //切换使用三种姿态
                    cv::Mat difmask=cv::Mat::zeros(1,interpolatweld.size(),CV_8UC1);
                    Uint8 *u8_difmask=difmask.ptr<uchar>(0);
                    for(int j = 20;j < interpolatweld.size()-20;j ++)
                    {
                        if(abs(f_res[j])>d_value&&f_res[j]>0)//上坡
                        {
                            u8_difmask[j]=255;
                        }
                    }
                    cv::Mat element=getStructuringElement(cv::MORPH_RECT,cv::Size(50,1));
                    cv::Mat elementdel=getStructuringElement(cv::MORPH_RECT,cv::Size(101,1));

                    cv::Mat difmask2=difmask.clone();
                    morphologyEx(difmask2, difmask2, cv::MORPH_CLOSE, element);
                    morphologyEx(difmask2, difmask2, cv::MORPH_OPEN, elementdel);
                    Uint8 *u8_difmaskUp=difmask2.ptr<uchar>(0);

                    difmask=cv::Mat::zeros(1,interpolatweld.size(),CV_8UC1);
                    for(int j = 20;j < interpolatweld.size()-20;j ++)
                    {
                        if(abs(f_res[j])>d_value&&f_res[j]<0)//下坡
                        {
                            u8_difmask[j]=255;
                        }
                    }

                    cv::Mat difmask3=difmask.clone();
                    morphologyEx(difmask3, difmask3, cv::MORPH_CLOSE, element);
                    morphologyEx(difmask3, difmask3, cv::MORPH_OPEN, elementdel);
                    Uint8 *u8_difmaskDown=difmask3.ptr<uchar>(0);

                    std::vector<int> s_upst;//平坡与上坡交界
                    std::vector<int> up_s_st;//上坡与平坡交界
                    std::vector<int> s_downst;//平坡与下坡交界
                    std::vector<int> down_s_st;//下坡与平坡交界

                    int n_state=0;//上下坡的状态
                    int f_state=0;//第一个姿态
                    int e_state=0;//最后一个姿态

                    for(int j = 20;j < interpolatweld.size()-20;j ++)
                    {
                        //if(abs(f_res[j])>d_value&&f_res[j]>0)//上坡
                        if(u8_difmaskUp[j]!=0)
                        {
                        //	tempWeldin.weldTar.push_back(tempWeld.weldTar[j]);
                            if(n_state!=2)
                            {
                                if(n_state==0)
                                {
                                    f_state=2;
                                }
                                n_state=2;
                                s_upst.push_back(j);
                            }
                            interpolatweld[j].X=interpolatweld[j].X+m_mcs->craft->posturelist[1].Variable.X;
                            interpolatweld[j].Y=interpolatweld[j].Y+m_mcs->craft->posturelist[1].Variable.Y;
                            interpolatweld[j].Z=interpolatweld[j].Z+m_mcs->craft->posturelist[1].Variable.Z;
                            interpolatweld[j].RX=m_mcs->craft->posturelist[1].posture.RX;
                            interpolatweld[j].RY=m_mcs->craft->posturelist[1].posture.RY;
                            interpolatweld[j].RZ=m_mcs->craft->posturelist[1].posture.RZ;
                        }
                        //else if(abs(f_res[j])>d_value&&f_res[j]<0)//下坡
                        else if(u8_difmaskDown[j]!=0)
                        {
                        //	tempWeldin.weldTar.push_back(tempWeld.weldTar[j]);
                            if(n_state!=3)
                            {
                                if(n_state==0)
                                {
                                    f_state=3;
                                }
                                n_state=3;
                                s_downst.push_back(j);
                            }
                            interpolatweld[j].X=interpolatweld[j].X+m_mcs->craft->posturelist[2].Variable.X;
                            interpolatweld[j].Y=interpolatweld[j].Y+m_mcs->craft->posturelist[2].Variable.Y;
                            interpolatweld[j].Z=interpolatweld[j].Z+m_mcs->craft->posturelist[2].Variable.Z;
                            interpolatweld[j].RX=m_mcs->craft->posturelist[2].posture.RX;
                            interpolatweld[j].RY=m_mcs->craft->posturelist[2].posture.RY;
                            interpolatweld[j].RZ=m_mcs->craft->posturelist[2].posture.RZ;
                        }
                        else
                        {
                            if(n_state!=1)
                            {
                                if(n_state==0)
                                {
                                    f_state=1;
                                }
                                if(n_state==2)
                                {
                                    up_s_st.push_back(j);
                                }
                                else if(n_state==3)
                                {
                                    down_s_st.push_back(j);
                                }
                                n_state=1;
                            }
                            interpolatweld[j].X=interpolatweld[j].X+m_mcs->craft->posturelist[0].Variable.X;
                            interpolatweld[j].Y=interpolatweld[j].Y+m_mcs->craft->posturelist[0].Variable.Y;
                            interpolatweld[j].Z=interpolatweld[j].Z+m_mcs->craft->posturelist[0].Variable.Z;
                            interpolatweld[j].RX=m_mcs->craft->posturelist[0].posture.RX;
                            interpolatweld[j].RY=m_mcs->craft->posturelist[0].posture.RY;
                            interpolatweld[j].RZ=m_mcs->craft->posturelist[0].posture.RZ;
                        }
                    }
                    e_state=n_state;
                    for(int j=0;j<20;j++)
                    {
                        if(j<interpolatweld.size())
                        {
                            switch(f_state)
                            {
                            case 1:
                                interpolatweld[j].X=interpolatweld[j].X+m_mcs->craft->posturelist[0].Variable.X;
                                interpolatweld[j].Y=interpolatweld[j].Y+m_mcs->craft->posturelist[0].Variable.Y;
                                interpolatweld[j].Z=interpolatweld[j].Z+m_mcs->craft->posturelist[0].Variable.Z;
                                interpolatweld[j].RX=m_mcs->craft->posturelist[0].posture.RX;
                                interpolatweld[j].RY=m_mcs->craft->posturelist[0].posture.RY;
                                interpolatweld[j].RZ=m_mcs->craft->posturelist[0].posture.RZ;
                            break;
                            case 2:
                                interpolatweld[j].X=interpolatweld[j].X+m_mcs->craft->posturelist[1].Variable.X;
                                interpolatweld[j].Y=interpolatweld[j].Y+m_mcs->craft->posturelist[1].Variable.Y;
                                interpolatweld[j].Z=interpolatweld[j].Z+m_mcs->craft->posturelist[1].Variable.Z;
                                interpolatweld[j].RX=m_mcs->craft->posturelist[1].posture.RX;
                                interpolatweld[j].RY=m_mcs->craft->posturelist[1].posture.RY;
                                interpolatweld[j].RZ=m_mcs->craft->posturelist[1].posture.RZ;
                            break;
                            case 3:
                                interpolatweld[j].X=interpolatweld[j].X+m_mcs->craft->posturelist[2].Variable.X;
                                interpolatweld[j].Y=interpolatweld[j].Y+m_mcs->craft->posturelist[2].Variable.Y;
                                interpolatweld[j].Z=interpolatweld[j].Z+m_mcs->craft->posturelist[2].Variable.Z;
                                interpolatweld[j].RX=m_mcs->craft->posturelist[2].posture.RX;
                                interpolatweld[j].RY=m_mcs->craft->posturelist[2].posture.RY;
                                interpolatweld[j].RZ=m_mcs->craft->posturelist[2].posture.RZ;
                            break;
                            default:
                                interpolatweld[j].X=interpolatweld[j].X+m_mcs->craft->posturelist[0].Variable.X;
                                interpolatweld[j].Y=interpolatweld[j].Y+m_mcs->craft->posturelist[0].Variable.Y;
                                interpolatweld[j].Z=interpolatweld[j].Z+m_mcs->craft->posturelist[0].Variable.Z;
                                interpolatweld[j].RX=m_mcs->craft->posturelist[0].posture.RX;
                                interpolatweld[j].RY=m_mcs->craft->posturelist[0].posture.RY;
                                interpolatweld[j].RZ=m_mcs->craft->posturelist[0].posture.RZ;
                            break;
                            }
                        }
                    }
                    for(int j=interpolatweld.size()-20;j<interpolatweld.size();j++)
                    {
                        switch(e_state)
                        {
                        case 1:
                            interpolatweld[j].X=interpolatweld[j].X+m_mcs->craft->posturelist[0].Variable.X;
                            interpolatweld[j].Y=interpolatweld[j].Y+m_mcs->craft->posturelist[0].Variable.Y;
                            interpolatweld[j].Z=interpolatweld[j].Z+m_mcs->craft->posturelist[0].Variable.Z;
                            interpolatweld[j].RX=m_mcs->craft->posturelist[0].posture.RX;
                            interpolatweld[j].RY=m_mcs->craft->posturelist[0].posture.RY;
                            interpolatweld[j].RZ=m_mcs->craft->posturelist[0].posture.RZ;
                        break;
                        case 2:
                            interpolatweld[j].X=interpolatweld[j].X+m_mcs->craft->posturelist[1].Variable.X;
                            interpolatweld[j].Y=interpolatweld[j].Y+m_mcs->craft->posturelist[1].Variable.Y;
                            interpolatweld[j].Z=interpolatweld[j].Z+m_mcs->craft->posturelist[1].Variable.Z;
                            interpolatweld[j].RX=m_mcs->craft->posturelist[1].posture.RX;
                            interpolatweld[j].RY=m_mcs->craft->posturelist[1].posture.RY;
                            interpolatweld[j].RZ=m_mcs->craft->posturelist[1].posture.RZ;
                        break;
                        case 3:
                            interpolatweld[j].X=interpolatweld[j].X+m_mcs->craft->posturelist[2].Variable.X;
                            interpolatweld[j].Y=interpolatweld[j].Y+m_mcs->craft->posturelist[2].Variable.Y;
                            interpolatweld[j].Z=interpolatweld[j].Z+m_mcs->craft->posturelist[2].Variable.Z;
                            interpolatweld[j].RX=m_mcs->craft->posturelist[2].posture.RX;
                            interpolatweld[j].RY=m_mcs->craft->posturelist[2].posture.RY;
                            interpolatweld[j].RZ=m_mcs->craft->posturelist[2].posture.RZ;
                        break;
                        default:
                            interpolatweld[j].X=interpolatweld[j].X+m_mcs->craft->posturelist[0].Variable.X;
                            interpolatweld[j].Y=interpolatweld[j].Y+m_mcs->craft->posturelist[0].Variable.Y;
                            interpolatweld[j].Z=interpolatweld[j].Z+m_mcs->craft->posturelist[0].Variable.Z;
                            interpolatweld[j].RX=m_mcs->craft->posturelist[0].posture.RX;
                            interpolatweld[j].RY=m_mcs->craft->posturelist[0].posture.RY;
                            interpolatweld[j].RZ=m_mcs->craft->posturelist[0].posture.RZ;
                        break;
                        }
                    }

                    //开始变姿态插值
                    Eigen::Vector3d PosR_st;
                    Eigen::Vector3d PosR_ed;
                    std::vector<Eigen::Vector3d> resaddPos;
                    int differenceNum=m_mcs->craft->posture_distance*2+1;

                    //平坡到上坡插值
                    PosR_st[0]=m_mcs->craft->posturelist[0].posture.RX;
                    PosR_st[1]=m_mcs->craft->posturelist[0].posture.RY;
                    PosR_st[2]=m_mcs->craft->posturelist[0].posture.RZ;
                    PosR_ed[0]=m_mcs->craft->posturelist[1].posture.RX;
                    PosR_ed[1]=m_mcs->craft->posturelist[1].posture.RY;
                    PosR_ed[2]=m_mcs->craft->posturelist[1].posture.RZ;
                    resaddPos=Calibration::Attitudedifference(m_mcs->rob->cal_posture_model,PosR_st,PosR_ed,differenceNum);
                    for(int i=0;i<s_upst.size();i++)
                    {
                        int t=0;
                        int j=s_upst[i];
                        for(int k=j-m_mcs->craft->posture_distance;k<=j+m_mcs->craft->posture_distance;k++)
                        {
                            if(k>=0&&k<interpolatweld.size())
                            {
                                interpolatweld[k].RX=resaddPos[t][0];
                                interpolatweld[k].RY=resaddPos[t][1];
                                interpolatweld[k].RZ=resaddPos[t][2];
                            }
                            t++;
                        }
                    }
                    //上坡到平坡插值
                    PosR_st[0]=m_mcs->craft->posturelist[1].posture.RX;
                    PosR_st[1]=m_mcs->craft->posturelist[1].posture.RY;
                    PosR_st[2]=m_mcs->craft->posturelist[1].posture.RZ;
                    PosR_ed[0]=m_mcs->craft->posturelist[0].posture.RX;
                    PosR_ed[1]=m_mcs->craft->posturelist[0].posture.RY;
                    PosR_ed[2]=m_mcs->craft->posturelist[0].posture.RZ;

                    resaddPos=Calibration::Attitudedifference(m_mcs->rob->cal_posture_model,PosR_st,PosR_ed,differenceNum);
                    for(int i=0;i<up_s_st.size();i++)
                    {
                        int t=0;
                        int j=up_s_st[i];
                        for(int k=j-m_mcs->craft->posture_distance;k<=j+m_mcs->craft->posture_distance;k++)
                        {
                            if(k>=0&&k<interpolatweld.size())
                            {
                                interpolatweld[k].RX=resaddPos[t][0];
                                interpolatweld[k].RY=resaddPos[t][1];
                                interpolatweld[k].RZ=resaddPos[t][2];
                            }
                            t++;
                        }
                    }

                    //平坡到下坡插值
                    PosR_st[0]=m_mcs->craft->posturelist[0].posture.RX;
                    PosR_st[1]=m_mcs->craft->posturelist[0].posture.RY;
                    PosR_st[2]=m_mcs->craft->posturelist[0].posture.RZ;
                    PosR_ed[0]=m_mcs->craft->posturelist[2].posture.RX;
                    PosR_ed[1]=m_mcs->craft->posturelist[2].posture.RY;
                    PosR_ed[2]=m_mcs->craft->posturelist[2].posture.RZ;

                    resaddPos=Calibration::Attitudedifference(m_mcs->rob->cal_posture_model,PosR_st,PosR_ed,differenceNum);
                    for(int i=0;i<s_downst.size();i++)
                    {
                        int t=0;
                        int j=s_downst[i];
                        for(int k=j-m_mcs->craft->posture_distance;k<=j+m_mcs->craft->posture_distance;k++)
                        {
                            if(k>=0&&k<interpolatweld.size())
                            {
                                interpolatweld[k].RX=resaddPos[t][0];
                                interpolatweld[k].RY=resaddPos[t][1];
                                interpolatweld[k].RZ=resaddPos[t][2];
                            }
                            t++;
                        }
                    }

                    //下坡到平坡插值
                    PosR_st[0]=m_mcs->craft->posturelist[2].posture.RX;
                    PosR_st[1]=m_mcs->craft->posturelist[2].posture.RY;
                    PosR_st[2]=m_mcs->craft->posturelist[2].posture.RZ;
                    PosR_ed[0]=m_mcs->craft->posturelist[0].posture.RX;
                    PosR_ed[1]=m_mcs->craft->posturelist[0].posture.RY;
                    PosR_ed[2]=m_mcs->craft->posturelist[0].posture.RZ;

                    resaddPos=Calibration::Attitudedifference(m_mcs->rob->cal_posture_model,PosR_st,PosR_ed,differenceNum);
                    for(int i=0;i<down_s_st.size();i++)
                    {
                        int t=0;
                        int j=down_s_st[i];
                        for(int k=j-m_mcs->craft->posture_distance;k<=j+m_mcs->craft->posture_distance;k++)
                        {
                            if(k>=0&&k<interpolatweld.size())
                            {
                                interpolatweld[k].RX=resaddPos[t][0];
                                interpolatweld[k].RY=resaddPos[t][1];
                                interpolatweld[k].RZ=resaddPos[t][2];
                            }
                            t++;
                        }
                    }

                    /*********************************/
                    //滤波轨道,只保留拐点，为了展会好看
                    /*
                    if(interpolatweld.size()>0)
                    {
                        std::vector<RobPos> interpolatweld_media;//滤波轨道
                        interpolatweld_media.push_back(interpolatweld[0]);
                        static float oldRX=interpolatweld[0].RX,oldRY=interpolatweld[0].RY,oldRZ=interpolatweld[0].RZ;
                        for(int i=0;i<interpolatweld.size();i++)
                        {
                            if(fabs(interpolatweld[i].RX-oldRX)>0.0001||
                               fabs(interpolatweld[i].RY-oldRY)>0.0001||
                               fabs(interpolatweld[i].RZ-oldRZ)>0.0001)
                            {
                                oldRX=interpolatweld[i].RX;
                                oldRY=interpolatweld[i].RY;
                                oldRZ=interpolatweld[i].RZ;
                                interpolatweld_media.push_back(interpolatweld[i]);
                            }
                        }
                        interpolatweld_media.push_back(interpolatweld[interpolatweld.size()-1]);
                        interpolatweld=interpolatweld_media;
                    }
                    */
                    /********************************/
                }
                else
                {
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 焊缝搜索异常");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    return 1;
                }
            }
            break;
        }
        Weld_trace_onec trace_onec;
        trace_onec.Sample=Sample;
        trace_onec.point=interpolatweld;
        trace_onec.speed=speed;
        m_mcs->project->project_interweld_trace[weld_trace_num_out].trace.push_back(trace_onec);

        if(m_mcs->e2proomdata.maindlg_SaveDatacheckBox!=0)//保存焊接轨迹
        {
            QString dir="./log/";
            QString key=SAVELOGFILE_TRACENAME_HEAD;
            QString time;
            std::string s_time;
            TimeFunction to;
            to.get_time_ms(&s_time);
            time=QString::fromStdString(s_time);
            dir=dir+time+key+name_out;
            savelog_trace(dir,m_mcs->project->project_interweld_trace[weld_trace_num_out].trace);
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
                std::vector<RobPos> weld_trace;
                if(false==m_mcs->synchronous->Scantrace_to_Weldtrace(scan_trace,weld_trace))
                {
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 轨迹坐标拟合出错");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    return 1;
                }
                weld=weld_trace;
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
                std::vector<RobPos> weld_trace0,weld_trace1,weld_trace2;
                scan_trace0=m_mcs->project->project_scan_trace[scan_trace_num_0].point;
                if(false==m_mcs->synchronous->Scantrace_to_Weldtrace(scan_trace0,weld_trace0)||
                   false==m_mcs->synchronous->Scantrace_to_Weldtrace(scan_trace1,weld_trace1)||
                   false==m_mcs->synchronous->Scantrace_to_Weldtrace(scan_trace2,weld_trace2))
                {
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 轨迹坐标拟合出错");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    return 1;
                }
                std::vector<Eigen::VectorXd> linePoints,SidePoints;
                for(int n=0;n<weld_trace0.size();n++)
                {
                    Eigen::Vector3d sing_linepoint;
                    sing_linepoint(0)=weld_trace0[n].X;
                    sing_linepoint(1)=weld_trace0[n].Y;
                    sing_linepoint(2)=weld_trace0[n].Z;
                    linePoints.push_back(sing_linepoint);
                }
                for(int n=0;n<weld_trace1.size();n++)
                {
                    Eigen::Vector3d sing_linepoint;
                    sing_linepoint(0)=weld_trace1[n].X;
                    sing_linepoint(1)=weld_trace1[n].Y;
                    sing_linepoint(2)=weld_trace1[n].Z;
                    SidePoints.push_back(sing_linepoint);
                }
                for(int n=0;n<weld_trace2.size();n++)
                {
                    Eigen::Vector3d sing_linepoint;
                    sing_linepoint(0)=weld_trace2[n].X;
                    sing_linepoint(1)=weld_trace2[n].Y;
                    sing_linepoint(2)=weld_trace2[n].Z;
                    SidePoints.push_back(sing_linepoint);
                }
                if(linePoints.size()<=2&&SidePoints.size()<=4)
                {
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 轨迹坐标数据太少");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    return 1;
                }
                FitlineSide fitlineside;
                Eigen::Vector3d endpoint=fitlineside.computePointOfLineAndSurface(linePoints,SidePoints);//交点
                RobPos Endpoint;
                float f_headdis=(weld_trace0[0].X-endpoint.x())*(weld_trace0[0].X-endpoint.x())+
                                (weld_trace0[0].Y-endpoint.y())*(weld_trace0[0].Y-endpoint.y())+
                                (weld_trace0[0].Z-endpoint.z())*(weld_trace0[0].Z-endpoint.z());
                float f_tiledis=(weld_trace0[weld_trace0.size()-1].X-endpoint.x())*(weld_trace0[weld_trace0.size()-1].X-endpoint.x())+
                                (weld_trace0[weld_trace0.size()-1].Y-endpoint.y())*(weld_trace0[weld_trace0.size()-1].Y-endpoint.y())+
                                (weld_trace0[weld_trace0.size()-1].Z-endpoint.z())*(weld_trace0[weld_trace0.size()-1].Z-endpoint.z());
                if(f_tiledis>f_headdis)//距离平面越来越远,说明交点放在头部
                {
                    Endpoint=weld_trace0[0];
                    Endpoint.X=endpoint.x();
                    Endpoint.Y=endpoint.y();
                    Endpoint.Z=endpoint.z();
                    weld_trace0.insert(weld_trace0.begin(),Endpoint);
                }
                else//距离平面越来越近，说明交点放在尾部
                {
                    Endpoint=weld_trace0[weld_trace0.size()-1];
                    Endpoint.X=endpoint.x();
                    Endpoint.Y=endpoint.y();
                    Endpoint.Z=endpoint.z();
                    weld_trace0.push_back(Endpoint);
                }
                weld=weld_trace0;
            }
            break;
            case TRACE_EDIT_MODE_TOWPOINT_THREE_TO_ONE: //两端点三直线交点模式
            {
                int scan_trace_num_0,scan_trace_num_1,scan_trace_num_2,scan_trace_num_3,scan_trace_num_4;//搜索到的扫描轨道序号
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
                for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
                {
                    if(scanname[3]==m_mcs->project->project_scan_trace[n].name)
                    {
                        scan_trace_num_3=n;
                        break;
                    }
                }
                for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
                {
                    if(scanname[4]==m_mcs->project->project_scan_trace[n].name)
                    {
                        scan_trace_num_4=n;
                        break;
                    }
                }
                //这里添加轨迹拟合
                std::vector<Scan_trace_line> scan_trace0,scan_trace1,scan_trace2,scan_trace3,scan_trace4;
                std::vector<RobPos> weld_trace0,weld_trace1,weld_trace2,weld_trace3,weld_trace4;
                scan_trace0=m_mcs->project->project_scan_trace[scan_trace_num_0].point;
                if(false==m_mcs->synchronous->Scantrace_to_Weldtrace(scan_trace0,weld_trace0)||
                   false==m_mcs->synchronous->Scantrace_to_Weldtrace(scan_trace1,weld_trace1)||
                   false==m_mcs->synchronous->Scantrace_to_Weldtrace(scan_trace2,weld_trace2)||
                   false==m_mcs->synchronous->Scantrace_to_Weldtrace(scan_trace3,weld_trace3)||
                   false==m_mcs->synchronous->Scantrace_to_Weldtrace(scan_trace4,weld_trace4))
                {
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 轨迹坐标拟合出错");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    return 1;
                }
                std::vector<Eigen::VectorXd> linePoints,SidePoints1,SidePoints2;
                for(int n=0;n<weld_trace0.size();n++)
                {
                    Eigen::Vector3d sing_linepoint;
                    sing_linepoint(0)=weld_trace0[n].X;
                    sing_linepoint(1)=weld_trace0[n].Y;
                    sing_linepoint(2)=weld_trace0[n].Z;
                    linePoints.push_back(sing_linepoint);
                }
                for(int n=0;n<weld_trace1.size();n++)
                {
                    Eigen::Vector3d sing_linepoint;
                    sing_linepoint(0)=weld_trace1[n].X;
                    sing_linepoint(1)=weld_trace1[n].Y;
                    sing_linepoint(2)=weld_trace1[n].Z;
                    SidePoints1.push_back(sing_linepoint);
                }
                for(int n=0;n<weld_trace2.size();n++)
                {
                    Eigen::Vector3d sing_linepoint;
                    sing_linepoint(0)=weld_trace2[n].X;
                    sing_linepoint(1)=weld_trace2[n].Y;
                    sing_linepoint(2)=weld_trace2[n].Z;
                    SidePoints1.push_back(sing_linepoint);
                }
                for(int n=0;n<weld_trace3.size();n++)
                {
                    Eigen::Vector3d sing_linepoint;
                    sing_linepoint(0)=weld_trace3[n].X;
                    sing_linepoint(1)=weld_trace3[n].Y;
                    sing_linepoint(2)=weld_trace3[n].Z;
                    SidePoints2.push_back(sing_linepoint);
                }
                for(int n=0;n<weld_trace4.size();n++)
                {
                    Eigen::Vector3d sing_linepoint;
                    sing_linepoint(0)=weld_trace2[n].X;
                    sing_linepoint(1)=weld_trace2[n].Y;
                    sing_linepoint(2)=weld_trace2[n].Z;
                    SidePoints2.push_back(sing_linepoint);
                }
                if(linePoints.size()<=2&&SidePoints1.size()<=4)
                {
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 轨迹端点1的坐标数据太少");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    return 1;
                }
                if(linePoints.size()<=2&&SidePoints2.size()<=4)
                {
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 轨迹端点2的坐标数据太少");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    return 1;
                }
                FitlineSide fitlineside;
                Eigen::Vector3d endpoint1=fitlineside.computePointOfLineAndSurface(linePoints,SidePoints1);//交点1
                Eigen::Vector3d endpoint2=fitlineside.computePointOfLineAndSurface(linePoints,SidePoints2);//交点2
                RobPos Endpoint1,Endpoint2;


                float f_headdis1=(weld_trace0[0].X-endpoint1.x())*(weld_trace0[0].X-endpoint1.x())+
                                (weld_trace0[0].Y-endpoint1.y())*(weld_trace0[0].Y-endpoint1.y())+
                                (weld_trace0[0].Z-endpoint1.z())*(weld_trace0[0].Z-endpoint1.z());
                float f_tiledis1=(weld_trace0[weld_trace0.size()-1].X-endpoint1.x())*(weld_trace0[weld_trace0.size()-1].X-endpoint1.x())+
                                (weld_trace0[weld_trace0.size()-1].Y-endpoint1.y())*(weld_trace0[weld_trace0.size()-1].Y-endpoint1.y())+
                                (weld_trace0[weld_trace0.size()-1].Z-endpoint1.z())*(weld_trace0[weld_trace0.size()-1].Z-endpoint1.z());
                float f_headdis2=(weld_trace0[0].X-endpoint2.x())*(weld_trace0[0].Y-endpoint2.x())+
                                (weld_trace0[0].Y-endpoint2.y())*(weld_trace0[0].Y-endpoint2.y())+
                                (weld_trace0[0].Z-endpoint2.z())*(weld_trace0[0].Z-endpoint2.z());
                float f_tiledis2=(weld_trace0[weld_trace0.size()-1].X-endpoint2.x())*(weld_trace0[weld_trace0.size()-1].X-endpoint2.x())+
                                (weld_trace0[weld_trace0.size()-1].Y-endpoint2.y())*(weld_trace0[weld_trace0.size()-1].Y-endpoint2.y())+
                                (weld_trace0[weld_trace0.size()-1].Z-endpoint2.z())*(weld_trace0[weld_trace0.size()-1].Z-endpoint2.z());

                if(f_tiledis1>f_headdis1)//距离平面越来越远,说明交点放在头部
                {
                    Endpoint1=weld_trace0[0];
                    Endpoint1.X=endpoint1.x();
                    Endpoint1.Y=endpoint1.y();
                    Endpoint1.Z=endpoint1.z();
                    weld_trace0.insert(weld_trace0.begin(),Endpoint1);
                }
                else//距离平面越来越近，说明交点放在尾部
                {
                    Endpoint1=weld_trace0[weld_trace0.size()-1];
                    Endpoint1.X=endpoint1.x();
                    Endpoint1.Y=endpoint1.y();
                    Endpoint1.Z=endpoint1.z();
                    weld_trace0.push_back(Endpoint1);
                }
                if(f_tiledis2>f_headdis2)//距离平面越来越远,说明交点放在头部
                {
                    Endpoint2=weld_trace0[0];
                    Endpoint2.X=endpoint2.x();
                    Endpoint2.Y=endpoint2.y();
                    Endpoint2.Z=endpoint2.z();
                    weld_trace0.insert(weld_trace0.begin(),Endpoint2);
                }
                else//距离平面越来越近，说明交点放在尾部
                {
                    Endpoint2=weld_trace0[weld_trace0.size()-1];
                    Endpoint2.X=endpoint2.x();
                    Endpoint2.Y=endpoint2.y();
                    Endpoint2.Z=endpoint2.z();
                    weld_trace0.push_back(Endpoint2);
                }
                weld=weld_trace0;
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
    else if(key==CMD_COORD_KEY)//零位矩阵指令
    {
        QString s_pointx=cmd.cmd_coord_pointx;//实际零位矩阵零点的X方向基准点
        QString s_pointo=cmd.cmd_coord_pointo;//实际零位矩阵零点基准点
        QString s_cpointx=cmd.cmd_coord_cpointx;
        QString s_cpointo=cmd.cmd_coord_cpointo;
        QString name=cmd.cmd_coord_name;//获取到的矩阵名字
        int coord_trace_num;//要储存的矩阵数据下标
        int pointx_trace_num;//实际零位矩阵零点的X方向基准点所在的点下标
        int pointo_trace_num;//实际零位矩阵零点的O方向基准点所在的点下标
        int cpointx_trace_num;//基准零位矩阵零点的X方向基准点所在的点下标
        int cpointo_trace_num;//基准零位矩阵零点的O方向基准点所在的点下标
        for(int n=0;n<m_mcs->project->projecr_coord_matrix4d.size();n++)
        {
            if(name==m_mcs->project->projecr_coord_matrix4d[n].name)
            {
                coord_trace_num=n;//找到要储存的扫描轨迹下标
                break;
            }
        }
        for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
        {
            if(s_pointx==m_mcs->project->projecr_robpos_trace[n].name)
            {
                pointx_trace_num=n;//找到要储存的s_pointx点下标
                break;
            }
        }
        if(m_mcs->project->projecr_robpos_trace[pointx_trace_num].nEn!=true)
        {
            //点无效
            main_record.lock();
            return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+s_pointx+QString::fromLocal8Bit("点没有获取到坐标值");
            m_mcs->main_record.push_back(return_msg);
            main_record.unlock();
            return 1;
        }
        for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
        {
            if(s_pointo==m_mcs->project->projecr_robpos_trace[n].name)
            {
                pointo_trace_num=n;//找到要储存的s_pointx点下标
                break;
            }
        }
        if(m_mcs->project->projecr_robpos_trace[pointo_trace_num].nEn!=true)
        {
            //点无效
            main_record.lock();
            return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+s_pointo+QString::fromLocal8Bit("点没有获取到坐标值");
            m_mcs->main_record.push_back(return_msg);
            main_record.unlock();
            return 1;
        }
        for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
        {
            if(s_cpointx==m_mcs->project->projecr_robpos_trace[n].name)
            {
                cpointx_trace_num=n;//找到要储存的s_cpointx点下标
                break;
            }
        }
        if(m_mcs->project->projecr_robpos_trace[cpointx_trace_num].nEn!=true)
        {
            //点无效
            main_record.lock();
            return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+s_cpointx+QString::fromLocal8Bit("点没有获取到坐标值");
            m_mcs->main_record.push_back(return_msg);
            main_record.unlock();
            return 1;
        }
        for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
        {
            if(s_cpointo==m_mcs->project->projecr_robpos_trace[n].name)
            {
                cpointo_trace_num=n;//找到要储存的s_cpointo点下标
                break;
            }
        }
        if(m_mcs->project->projecr_robpos_trace[cpointo_trace_num].nEn!=true)
        {
            //点无效
            main_record.lock();
            return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+s_cpointo+QString::fromLocal8Bit("点没有获取到坐标值");
            m_mcs->main_record.push_back(return_msg);
            main_record.unlock();
            return 1;
        }
        /*************/
        //开始计算零点变化矩阵
        Eigen::Vector3d org;
        Eigen::Vector3d org_x,org_y;
        Eigen::Vector3d corg;
        Eigen::Vector3d corg_x,corg_y;
        Eigen::Matrix3d R;          //旋转矩阵
        Eigen::Vector3d T;          //平移矩阵(零点坐标)
        Eigen::Matrix3d R1;          //旋转矩阵
        Eigen::Vector3d T1;          //平移矩阵(零点坐标)
        org.x()=m_mcs->project->projecr_robpos_trace[pointo_trace_num].robotpos.X;
        org.y()=m_mcs->project->projecr_robpos_trace[pointo_trace_num].robotpos.Y;
        org.z()=m_mcs->project->projecr_robpos_trace[pointo_trace_num].robotpos.Z;
        org_x.x()=m_mcs->project->projecr_robpos_trace[pointx_trace_num].robotpos.X;
        org_x.y()=m_mcs->project->projecr_robpos_trace[pointx_trace_num].robotpos.Y;
        org_x.z()=m_mcs->project->projecr_robpos_trace[pointx_trace_num].robotpos.Z;
        org_y.x()=org_x.x();
        org_y.y()=org_x.y()+100;
        org_y.z()=org_x.z();

        corg.x()=m_mcs->project->projecr_robpos_trace[cpointo_trace_num].robotpos.X;
        corg.y()=m_mcs->project->projecr_robpos_trace[cpointo_trace_num].robotpos.Y;
        corg.z()=m_mcs->project->projecr_robpos_trace[cpointo_trace_num].robotpos.Z;
        corg_x.x()=m_mcs->project->projecr_robpos_trace[cpointx_trace_num].robotpos.X;
        corg_x.y()=m_mcs->project->projecr_robpos_trace[cpointx_trace_num].robotpos.Y;
        corg_x.z()=m_mcs->project->projecr_robpos_trace[cpointx_trace_num].robotpos.Z;
        corg_y.x()=corg_x.x();
        corg_y.y()=corg_x.y()+100;
        corg_y.z()=corg_x.z();

        CCoordChange::_1coord2RT(corg,corg_x,corg_y,&R1,&T1);
        CCoordChange::coord2RT(org,org_x,org_y,&R,&T);

        m_mcs->project->projecr_coord_matrix4d[coord_trace_num].R1=R1;
        m_mcs->project->projecr_coord_matrix4d[coord_trace_num].T1=T1;

        m_mcs->project->projecr_coord_matrix4d[coord_trace_num].R=R;
        m_mcs->project->projecr_coord_matrix4d[coord_trace_num].T=T;

        m_mcs->project->projecr_coord_matrix4d[coord_trace_num].nEn=true;
    }
    else if(key==CMD_SAMPLE_KEY)//采样命令
    {
        QString creatname=cmd.cmd_sample_creatname;//获取到的要采样的轨迹名称
        QString name=cmd.cmd_sample_name;//获取到的采样结果点名称
        float speed=cmd.cmd_sample_speed;//获取到的采样时轨迹的速度
        int time=cmd.cmd_sample_time;//获取到的采样点之间的时间单位ms
        int weld_trace_creatnum;//搜索到的要采样的轨迹序号
        int weld_trace_num;//搜索到的采样结果的轨迹序号
        std::vector<RobPos> weld,interpolatweld;//轨道

        for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
        {
            if(name==m_mcs->project->project_weld_trace[n].name)
            {
                weld_trace_num=n;//找到要储存的焊接轨道下标
                break;
            }
        }
        for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
        {
            if(creatname==m_mcs->project->project_weld_trace[n].name)
            {
                weld_trace_creatnum=n;//找到要采样的焊接轨道下标
                break;
            }
        }
        weld=m_mcs->project->project_weld_trace[weld_trace_creatnum].point;
        CWeldTarject tarjectMath;
        if(!tarjectMath.pos_interpolation(m_mcs->rob->cal_posture_model,weld,interpolatweld,time,speed))
        {
            main_record.lock();
            return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 轨迹采样出错");
            m_mcs->main_record.push_back(return_msg);
            main_record.unlock();
            return 1;
        }
        m_mcs->project->project_weld_trace[weld_trace_num].point=interpolatweld;
        m_mcs->project->project_weld_trace[weld_trace_num].Sample=true;
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
    else if(key==CMD_TRACEADD_KEY)
    {
        QString name1=cmd.cmd_traceadd_name1;//获取跟踪轨迹工艺1名字
        QString name2=cmd.cmd_traceadd_name2;//获取跟踪轨迹工艺2名字
        QString name_out=cmd.cmd_traceadd_nameout;
        bool b_link=cmd.cmd_traceadd_samplelink;//连接处采样
        float speed=cmd.cmd_traceadd_speed;
        float samplespeed=cmd.cmd_traceadd_samplespeed;
        int time=cmd.cmd_traceadd_time;
        int weld_tracing_num1;
        int weld_tracing_num2;
        int weld_tracing_num_out;
        for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
        {
            if(name1==m_mcs->project->project_interweld_trace[n].name)
            {
                weld_tracing_num1=n;//找到焊接轨道下标
                break;
            }
        }
        for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
        {
            if(name2==m_mcs->project->project_interweld_trace[n].name)
            {
                weld_tracing_num2=n;//找到焊接轨道下标
                break;
            }
        }
        for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
        {
            if(name_out==m_mcs->project->project_interweld_trace[n].name)
            {
                weld_tracing_num_out=n;//找到焊接轨道下标
                break;
            }
        }
        std::vector<Weld_trace_onec> trace1=m_mcs->project->project_interweld_trace[weld_tracing_num1].trace;
        std::vector<Weld_trace_onec> trace2=m_mcs->project->project_interweld_trace[weld_tracing_num2].trace;
        std::vector<Weld_trace_onec> traceout=m_mcs->project->project_interweld_trace[weld_tracing_num_out].trace;
        traceout.insert(traceout.end(),trace1.begin(),trace1.end());
        if(b_link==true)//中间采样连接
        {          
            if(trace1.size()>0&&trace2.size()>0)
            {
                if(trace1[trace1.size()-1].point.size()>0&&trace2[trace2.size()-1].point.size()>0)
                {
                    Weld_trace_onec tracelink;
                    RobPos headpoint=trace1[trace1.size()-1].point[trace1[trace1.size()-1].point.size()-1];
                    RobPos lastpoint=trace2[0].point[0];
                    std::vector<RobPos> weld(2),interpolatweld;
                    weld[0]=headpoint;
                    weld[1]=lastpoint;
                    CWeldTarject tarjectMath;
                    if(!tarjectMath.pos_interpolation(m_mcs->rob->cal_posture_model,weld,interpolatweld,time,samplespeed))
                    {
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 轨迹采样出错");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        return 1;
                    }
                    tracelink.point=interpolatweld;
                    tracelink.Sample=true;
                    tracelink.speed=speed;
                    traceout.push_back(tracelink);
                }
            }
        }
        traceout.insert(traceout.end(),trace2.begin(),trace2.end());
        m_mcs->project->project_interweld_trace[weld_tracing_num_out].trace=traceout;
        if(m_mcs->e2proomdata.maindlg_SaveDatacheckBox!=0)//保存轨迹
        {
            QString dir="./log/";
            QString key=SAVELOGFILE_TRACENAME_HEAD;
            QString time;
            std::string s_time;
            TimeFunction to;
            to.get_time_ms(&s_time);
            time=QString::fromStdString(s_time);
            dir=dir+time+key+name_out;
            savelog_trace(dir,m_mcs->project->project_interweld_trace[weld_tracing_num_out].trace);
        }
    }
    else if(key==CMD_WAVE_KEY)
    {
        QString name_in=cmd.cmd_wave_namein;
        QString name_out=cmd.cmd_wave_nameout;
        wWAVEParam wave_info=cmd.cmd_wave_info;
        int weld_tracing_num_in;
        int weld_tracing_num_out;
        for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
        {
            if(name_in==m_mcs->project->project_interweld_trace[n].name)
            {
                weld_tracing_num_in=n;//找到焊接轨道下标
                break;
            }
        }
        for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
        {
            if(name_out==m_mcs->project->project_interweld_trace[n].name)
            {
                weld_tracing_num_out=n;//找到焊接轨道下标
                break;
            }
        }
        std::vector<Weld_trace_onec> trace_in=m_mcs->project->project_interweld_trace[weld_tracing_num_in].trace;
        std::vector<Weld_trace_onec> trace_out(trace_in.size());
        double wavet_in,wavet_out;
        for(int n=0;n<trace_in.size();n++)
        {
            Weld_trace_onec trace=trace_in[n];
            Weld_trace_onec wavetrace;
            /***************/
            //摆焊工艺
            CWeldTarject tarjectMath;
            if(n==0)
            {
                if(0!=tarjectMath.creat_wave(m_mcs->rob->cal_posture_model,trace.point,wave_info,&wavetrace.point,&wavet_out))
                {
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 摆焊轨迹计算异常");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    return 1;
                }
            }
            else
            {
                wavet_in=wavet_out;
                if(0!=tarjectMath.creat_wave_continue(m_mcs->rob->cal_posture_model,trace.point,wave_info,&wavetrace.point,wavet_in,&wavet_out))
                {
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 摆焊轨迹计算异常");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    return 1;
                }
            }
            wavetrace.speed=trace.speed;
            wavetrace.Sample=trace.Sample;
            trace_out[n]=wavetrace;
        }
        m_mcs->project->project_interweld_trace[weld_tracing_num_out].trace=trace_out;
        if(m_mcs->e2proomdata.maindlg_SaveDatacheckBox!=0)//保存轨迹
        {
            QString dir="./log/";
            QString key=SAVELOGFILE_TRACENAME_HEAD;
            QString time;
            std::string s_time;
            TimeFunction to;
            to.get_time_ms(&s_time);
            time=QString::fromStdString(s_time);
            dir=dir+time+key+name_out;
            savelog_trace(dir,m_mcs->project->project_interweld_trace[weld_tracing_num_out].trace);
        }
    }
    else if(key==CMD_CREATP_KEY)
    {
        QString name=cmd.cmd_creatp_name;//获取到的生成的轨迹名字
        std::vector<QString> pointsname=cmd.cmd_creatp_pointsname;//获取到要点的轨道名字
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
        std::vector<int> weld_pointsname_num(pointsname.size());
        for(int m=0;m<pointsname.size();m++)
        {
            QString pointname=pointsname[m];
            for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
            {
                if(pointname==m_mcs->project->projecr_robpos_trace[n].name)
                {
                    weld_pointsname_num[m]=n;//找到要储存的焊接点下标
                    break;
                }
            }
        }
        //把这些点按顺序连接
        for(int n=0;n<weld_pointsname_num.size();n++)
        {
            int weld_point_num=weld_pointsname_num[n];
            if(m_mcs->project->projecr_robpos_trace[weld_point_num].nEn!=true)//点无效
            {
                main_record.lock();
                return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+m_mcs->project->projecr_robpos_trace[weld_point_num].name+QString::fromLocal8Bit("点没有获取到坐标值");
                m_mcs->main_record.push_back(return_msg);
                main_record.unlock();
                return 1;
            }
            RobPos pos=m_mcs->project->projecr_robpos_trace[weld_point_num].robotpos;
            weld.push_back(pos);
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
    else if(key==CMD_SETPOSE_KEY)
    {
        QString namein=cmd.cmd_setpose_namein;
        QString nameout=cmd.cmd_setpose_nameout;
        std::vector<float> pose=cmd.cmd_setpose_pose;
        std::vector<float> add=cmd.cmd_setpose_add;
        int weld_namein_num;
        int weld_nameout_num;
        for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
        {
            if(namein==m_mcs->project->projecr_robpos_trace[n].name)
            {
                weld_namein_num=n;//找到要储存的点下标
                break;
            }
        }
        for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
        {
            if(nameout==m_mcs->project->projecr_robpos_trace[n].name)
            {
                weld_nameout_num=n;//找到要储存的点下标
                break;
            }
        }
        if(m_mcs->project->projecr_robpos_trace[weld_namein_num].nEn!=true)//点无效
        {
            main_record.lock();
            return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+namein+QString::fromLocal8Bit("点没有获取到坐标值");
            m_mcs->main_record.push_back(return_msg);
            main_record.unlock();
            return 1;
        }
        RobPos pos=m_mcs->project->projecr_robpos_trace[weld_namein_num].robotpos;
        pos.RX=pose[0];
        pos.RY=pose[1];
        pos.RZ=pose[2];
        pos.X=pos.X+add[0];
        pos.Y=pos.Y+add[1];
        pos.Z=pos.Z+add[2];
        m_mcs->project->projecr_robpos_trace[weld_nameout_num].robotpos=pos;
        m_mcs->project->projecr_robpos_trace[weld_nameout_num].nEn=true;
    }
    else if(key==CMD_CREATF_KEY)
    {
        QString filename=cmd.cmd_creatf_filename;//获取到要轨迹文件名字
        QString name=cmd.cmd_creatf_name;//获取到的生成的轨迹名字
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
        if(0!=loadlog_creat(filename,weld))
        {
            main_record.lock();
            return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 轨迹的文件格式出错");
            m_mcs->main_record.push_back(return_msg);
            main_record.unlock();
            return 1;
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
    else if(key==CMD_PLOTPOS_KEY)//生成点命令
    {
        QString name=cmd.cmd_plotpos_name;//获取到的生成的点名字
        Plotpos_edit_mode mode=cmd.cmd_plotpos_mode;//获取到的轨迹生成模式
        std::vector<QString> creatname=cmd.cmd_plotpos_creatname;//获取到生成点所需要的轨迹名字
        std::vector<QString> pointsname=cmd.cmd_plotpos_pointsname;//获取到生成点所需要的点名字
        int robpos_trace_num;//搜索到的点序号
        RobPos Endpoint;
        for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
        {
            if(name==m_mcs->project->projecr_robpos_trace[n].name)
            {
                robpos_trace_num=n;//找到要储存的点下标
                break;
            }
        }

        switch(mode)
        {
            case PLOTPOS_EDIT_MODE_THREE_TO_ONE://三直线交点模式
            {
                int scan_trace_num_0,scan_trace_num_1,scan_trace_num_2;//搜索到的扫描轨道序号
                for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
                {
                    if(creatname[0]==m_mcs->project->project_weld_trace[n].name)
                    {
                        scan_trace_num_0=n;
                        break;
                    }
                }
                for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
                {
                    if(creatname[1]==m_mcs->project->project_weld_trace[n].name)
                    {
                        scan_trace_num_1=n;
                        break;
                    }
                }
                for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
                {
                    if(creatname[2]==m_mcs->project->project_weld_trace[n].name)
                    {
                        scan_trace_num_2=n;
                        break;
                    }
                }
                //这里添加轨迹拟合
                std::vector<RobPos> weld_trace0,weld_trace1,weld_trace2;
                weld_trace0=m_mcs->project->project_weld_trace[scan_trace_num_0].point;
                weld_trace1=m_mcs->project->project_weld_trace[scan_trace_num_1].point;
                weld_trace2=m_mcs->project->project_weld_trace[scan_trace_num_2].point;

                std::vector<Eigen::VectorXd> linePoints,SidePoints;
                for(int n=0;n<weld_trace0.size();n++)
                {
                    Eigen::Vector3d sing_linepoint;
                    sing_linepoint(0)=weld_trace0[n].X;
                    sing_linepoint(1)=weld_trace0[n].Y;
                    sing_linepoint(2)=weld_trace0[n].Z;
                    linePoints.push_back(sing_linepoint);
                }
                for(int n=0;n<weld_trace1.size();n++)
                {
                    Eigen::Vector3d sing_linepoint;
                    sing_linepoint(0)=weld_trace1[n].X;
                    sing_linepoint(1)=weld_trace1[n].Y;
                    sing_linepoint(2)=weld_trace1[n].Z;
                    SidePoints.push_back(sing_linepoint);
                }
                for(int n=0;n<weld_trace2.size();n++)
                {
                    Eigen::Vector3d sing_linepoint;
                    sing_linepoint(0)=weld_trace2[n].X;
                    sing_linepoint(1)=weld_trace2[n].Y;
                    sing_linepoint(2)=weld_trace2[n].Z;
                    SidePoints.push_back(sing_linepoint);
                }
                if(linePoints.size()<=2&&SidePoints.size()<=4)
                {
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 轨迹坐标数据太少");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    return 1;
                }
                FitlineSide fitlineside;
                Eigen::Vector3d endpoint=fitlineside.computePointOfLineAndSurface(linePoints,SidePoints);//交点

                float f_headdis=(weld_trace0[0].X-endpoint.x())*(weld_trace0[0].X-endpoint.x())+
                                (weld_trace0[0].Y-endpoint.y())*(weld_trace0[0].Y-endpoint.y())+
                                (weld_trace0[0].Z-endpoint.z())*(weld_trace0[0].Z-endpoint.z());
                float f_tiledis=(weld_trace0[weld_trace0.size()-1].X-endpoint.x())*(weld_trace0[weld_trace0.size()-1].X-endpoint.x())+
                                (weld_trace0[weld_trace0.size()-1].Y-endpoint.y())*(weld_trace0[weld_trace0.size()-1].Y-endpoint.y())+
                                (weld_trace0[weld_trace0.size()-1].Z-endpoint.z())*(weld_trace0[weld_trace0.size()-1].Z-endpoint.z());
                if(f_tiledis>f_headdis)//距离平面越来越远,说明交点放在头部
                {
                    Endpoint=weld_trace0[0];
                    Endpoint.X=endpoint.x();
                    Endpoint.Y=endpoint.y();
                    Endpoint.Z=endpoint.z();
                }
                else//距离平面越来越近，说明交点放在尾部
                {
                    Endpoint=weld_trace0[weld_trace0.size()-1];
                    Endpoint.X=endpoint.x();
                    Endpoint.Y=endpoint.y();
                    Endpoint.Z=endpoint.z();
                }
            }
            break;
            case PLOTPOS_EDIT_MODE_FIVEPOINTS_TO_ONE:
            {
                int point_trace_num_0,point_trace_num_1;
                int point_trace_num_2,point_trace_num_3,point_trace_num_4;//搜索到的扫描轨道序号
                for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
                {
                    if(pointsname[0]==m_mcs->project->projecr_robpos_trace[n].name)
                    {
                        point_trace_num_0=n;
                        break;
                    }
                }
                for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
                {
                    if(pointsname[1]==m_mcs->project->projecr_robpos_trace[n].name)
                    {
                        point_trace_num_1=n;
                        break;
                    }
                }
                for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
                {
                    if(pointsname[2]==m_mcs->project->projecr_robpos_trace[n].name)
                    {
                        point_trace_num_2=n;
                        break;
                    }
                }
                for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
                {
                    if(pointsname[3]==m_mcs->project->projecr_robpos_trace[n].name)
                    {
                        point_trace_num_3=n;
                        break;
                    }
                }
                for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
                {
                    if(pointsname[4]==m_mcs->project->projecr_robpos_trace[n].name)
                    {
                        point_trace_num_4=n;
                        break;
                    }
                }
                if(m_mcs->project->projecr_robpos_trace[point_trace_num_0].nEn==false)
                {
                    //点无效
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+pointsname[0]+QString::fromLocal8Bit("点没有获取到坐标值");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    return 1;
                }
                if(m_mcs->project->projecr_robpos_trace[point_trace_num_1].nEn==false)
                {
                    //点无效
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+pointsname[1]+QString::fromLocal8Bit("点没有获取到坐标值");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    return 1;
                }
                if(m_mcs->project->projecr_robpos_trace[point_trace_num_2].nEn==false)
                {
                    //点无效
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+pointsname[2]+QString::fromLocal8Bit("点没有获取到坐标值");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    return 1;
                }
                if(m_mcs->project->projecr_robpos_trace[point_trace_num_3].nEn==false)
                {
                    //点无效
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+pointsname[3]+QString::fromLocal8Bit("点没有获取到坐标值");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    return 1;
                }
                if(m_mcs->project->projecr_robpos_trace[point_trace_num_4].nEn==false)
                {
                    //点无效
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+pointsname[4]+QString::fromLocal8Bit("点没有获取到坐标值");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    return 1;
                }


                std::vector<RobPos> weld_trace0;
                weld_trace0.push_back(m_mcs->project->projecr_robpos_trace[point_trace_num_0].robotpos);
                weld_trace0.push_back(m_mcs->project->projecr_robpos_trace[point_trace_num_1].robotpos);
                Eigen::Vector3d a,b,c,d,e;
                a.x()=m_mcs->project->projecr_robpos_trace[point_trace_num_2].robotpos.X;
                a.y()=m_mcs->project->projecr_robpos_trace[point_trace_num_2].robotpos.Y;
                a.z()=m_mcs->project->projecr_robpos_trace[point_trace_num_2].robotpos.Z;
                b.x()=m_mcs->project->projecr_robpos_trace[point_trace_num_3].robotpos.X;
                b.y()=m_mcs->project->projecr_robpos_trace[point_trace_num_3].robotpos.Y;
                b.z()=m_mcs->project->projecr_robpos_trace[point_trace_num_3].robotpos.Z;
                c.x()=m_mcs->project->projecr_robpos_trace[point_trace_num_4].robotpos.X;
                c.y()=m_mcs->project->projecr_robpos_trace[point_trace_num_4].robotpos.Y;
                c.z()=m_mcs->project->projecr_robpos_trace[point_trace_num_4].robotpos.Z;

                d.x()=m_mcs->project->projecr_robpos_trace[point_trace_num_0].robotpos.X;
                d.y()=m_mcs->project->projecr_robpos_trace[point_trace_num_0].robotpos.Y;
                d.z()=m_mcs->project->projecr_robpos_trace[point_trace_num_0].robotpos.Z;
                e.x()=m_mcs->project->projecr_robpos_trace[point_trace_num_1].robotpos.X;
                e.y()=m_mcs->project->projecr_robpos_trace[point_trace_num_1].robotpos.Y;
                e.z()=m_mcs->project->projecr_robpos_trace[point_trace_num_1].robotpos.Z;

                Eigen::Vector3d ab=b-a;
                Eigen::Vector3d ac=c-a;
                Eigen::Vector3d nn=ab.cross(ac);

                Eigen::Vector3d v=e-d;

                double dot_product=nn.dot(v);
                double t=-(nn.dot(d-a))/dot_product;
                Eigen::Vector3d endpoint=d+t*v;

                float f_headdis=(weld_trace0[0].X-endpoint.x())*(weld_trace0[0].X-endpoint.x())+
                                (weld_trace0[0].Y-endpoint.y())*(weld_trace0[0].Y-endpoint.y())+
                                (weld_trace0[0].Z-endpoint.z())*(weld_trace0[0].Z-endpoint.z());
                float f_tiledis=(weld_trace0[weld_trace0.size()-1].X-endpoint.x())*(weld_trace0[weld_trace0.size()-1].X-endpoint.x())+
                                (weld_trace0[weld_trace0.size()-1].Y-endpoint.y())*(weld_trace0[weld_trace0.size()-1].Y-endpoint.y())+
                                (weld_trace0[weld_trace0.size()-1].Z-endpoint.z())*(weld_trace0[weld_trace0.size()-1].Z-endpoint.z());
                if(f_tiledis>f_headdis)//距离平面越来越远,说明交点放在头部
                {
                    Endpoint=weld_trace0[0];
                    Endpoint.X=endpoint.x();
                    Endpoint.Y=endpoint.y();
                    Endpoint.Z=endpoint.z();
                }
                else//距离平面越来越近，说明交点放在尾部
                {
                    Endpoint=weld_trace0[weld_trace0.size()-1];
                    Endpoint.X=endpoint.x();
                    Endpoint.Y=endpoint.y();
                    Endpoint.Z=endpoint.z();
                }
            }
            break;
        }
        m_mcs->project->projecr_robpos_trace[robpos_trace_num].robotpos=Endpoint;
        m_mcs->project->projecr_robpos_trace[robpos_trace_num].nEn=true;
    }
    else if(key==CMD_CREATC_KEY)
    {
        QString name=cmd.cmd_creatc_name;//获取到的生成的轨迹名字
        std::vector<QString> pointsname=cmd.cmd_creatc_pointsname;//获取到要点的轨道名字
        float speed=cmd.cmd_creatc_speed;
        int time=cmd.cmd_creatc_time;
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
        std::vector<int> weld_pointsname_num(pointsname.size());
        for(int m=0;m<pointsname.size();m++)
        {
            QString pointname=pointsname[m];
            for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
            {
                if(pointname==m_mcs->project->projecr_robpos_trace[n].name)
                {
                    weld_pointsname_num[m]=n;//找到要储存的焊接点下标
                    break;
                }
            }
        }
        //把这些点按顺序连接
        CWeldTarject tarjectMath;
        std::vector<RobPos> interpolatPos;
        RobPos pos1=m_mcs->project->projecr_robpos_trace[weld_pointsname_num[0]].robotpos;//获取到移动坐标
        RobPos pos2=m_mcs->project->projecr_robpos_trace[weld_pointsname_num[1]].robotpos;//获取到移动坐标
        RobPos pos3=m_mcs->project->projecr_robpos_trace[weld_pointsname_num[2]].robotpos;//获取到移动坐标
        if(false==tarjectMath.pos_circle(m_mcs->rob->cal_posture_model,pos1,pos2,pos3,interpolatPos,ROBOT_POSE_MOVEC_STEP,time,speed))
        {
            main_record.lock();
            return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 圆弧三点轨迹拟合出错");
            m_mcs->main_record.push_back(return_msg);
            main_record.unlock();
            return 1;
        }
        //规划后的轨道
        m_mcs->project->project_weld_trace[weld_trace_num].point=interpolatPos;
        m_mcs->project->project_weld_trace[weld_trace_num].Sample=true;
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
    else if(key==CMD_GETTCPPOS2_KEY)
    {
        QString name=cmd.cmd_gettcppos2_name;//获取到要保存tcp的点名字
        RobPos pos=cmd.cmd_gettcppos2_pos;
        int robpos_trace_num;//获取要储存的tcp点下标
        for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
        {
            if(name==m_mcs->project->projecr_robpos_trace[n].name)
            {
                robpos_trace_num=n;//找到要储存的扫描点下标
                break;
            }
        }
        m_mcs->project->projecr_robpos_trace[robpos_trace_num].robotpos=pos;
        m_mcs->project->projecr_robpos_trace[robpos_trace_num].nEn=true;
    }
    else if(key==CMD_TRACECONTINUE_KEY)
    {
        QString name_in=cmd.cmd_tracecontinue_namein;//获取到的生成的轨迹名字
        QString name_out=cmd.cmd_tracecontinue_nameout;
        RobPos pos=cmd.cmd_tracecontinue_pos;
        int weld_tracing_num_in;
        int weld_tracing_num_out;
        for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
        {
            if(name_in==m_mcs->project->project_interweld_trace[n].name)
            {
                weld_tracing_num_in=n;//找到焊接轨道下标
                break;
            }
        }
        for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
        {
            if(name_out==m_mcs->project->project_interweld_trace[n].name)
            {
                weld_tracing_num_out=n;//找到焊接轨道下标
                break;
            }
        }
        float min_dis=FLT_MAX;
        int min_n=0;
        int min_m=0;
        int num=0;

        std::vector<Weld_trace_onec> weld=m_mcs->project->project_interweld_trace[weld_tracing_num_in].trace;

        for(int n=0;n<weld.size();n++)
        {
            num=num+weld[n].point.size();
        }
        if(num==0)
        {
            main_record.lock();
            return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 轨迹生成出错");
            m_mcs->main_record.push_back(return_msg);
            main_record.unlock();
            return 1;
        }
        Eigen::Vector3d point_st;
        point_st.x()=pos.X;
        point_st.y()=pos.Y;
        point_st.z()=pos.Z;

        for(int n=0;n<weld.size();n++)
        {
            Weld_trace_onec trace=weld[n];
            for(int m=0;m<trace.point.size();m++)
            {
                Eigen::Vector3d point;
                point.x()=trace.point[m].X;
                point.y()=trace.point[m].Y;
                point.z()=trace.point[m].Z;
                float dis=(point-point_st).norm();
                if(dis<min_dis)
                {
                    min_dis=dis;
                    min_n=n;
                    min_m=m;
                }
            }
        }
        std::vector<Weld_trace_onec> newweld(weld.size()-(min_n+1)+1);
        Weld_trace_onec sing;
        std::vector<RobPos> posgroup(weld[min_n].point.size()-min_m);
        for(int m=min_m;m<weld[min_n].point.size();m++)
        {
            posgroup[m-min_m]=weld[min_n].point[m];
        }
        sing.point=posgroup;
        sing.speed=weld[min_n].speed;
        newweld[0]=sing;
        for(int n=min_n+1;n<weld.size();n++)
        {
            newweld[n-min_n]=weld[n];
        }
        m_mcs->project->project_interweld_trace[weld_tracing_num_out].trace=newweld;

        if(m_mcs->e2proomdata.maindlg_SaveDatacheckBox!=0)//保存焊接轨迹
        {
            QString dir="./log/";
            QString key=SAVELOGFILE_TRACENAME_HEAD;
            QString time;
            std::string s_time;
            TimeFunction to;
            to.get_time_ms(&s_time);
            time=QString::fromStdString(s_time);
            dir=dir+time+key+name_in;
            savelog_trace(dir,m_mcs->project->project_interweld_trace[weld_tracing_num_out].trace);
        }
    }
    else if(key==CMD_FILTER_KEY)
    {
        QString namein=cmd.cmd_filter_namein;//获取到的要滤波的轨迹名称
        QString nameout=cmd.cmd_filter_nameout;//获取到的滤波结果名称
        Filter_mode mode=cmd.cmd_filter_mode;
        filterParam filters=cmd.cmd_filters;//滤波参数
        int weld_trace_creatnum,weld_trace_num;
        std::vector<RobPos> weld,interpolatweld;//轨道

        for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
        {
            if(namein==m_mcs->project->project_weld_trace[n].name)
            {
                weld_trace_creatnum=n;//找到要储存的焊接轨道下标
                break;
            }
        }
        for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
        {
            if(nameout==m_mcs->project->project_weld_trace[n].name)
            {
                weld_trace_num=n;//找到要储存的焊接轨道下标
                break;
            }
        }

        weld=m_mcs->project->project_weld_trace[weld_trace_creatnum].point;

        switch(mode)
        {
        #if USE_PLC_FILTER==1
            case FILTER_MLS://中值滤波
            {
                if(0!=Mypcl::Moving_Least_Squares(weld,interpolatweld,filters.msl_poly))
                {
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 滤波结果出错");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    return 1;
                }
            }
            break;
            case FILTER_SOR://曲线拟合
            {
                if(0!=Mypcl::Statistical_Outlier_Removal(weld,interpolatweld,filters.sor_nearpoint_num,filters.sor_standard_deviation))
                {
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 滤波结果出错");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    return 1;
                }
            }
            break;
        #endif
            case FILTER_SVD://SVD滤波
            {
                if(0!=Mypcl::Singular_Value_Decomposition(weld,interpolatweld,filters.svd_Degree,filters.svd_WindowSize,filters.svd_SingularThreshold))
                {
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 滤波结果出错");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    return 1;
                }
            }
            break;
            case FILTER_GAUSSIAN://高斯滤波
            {
                if(0!=Mypcl::Gaussian(weld,interpolatweld,filters.gaussian_SmoothingRadius,filters.gaussian_SmoothingSigma))
                {
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 滤波结果出错");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    return 1;
                }
            }
            break;
        }

        m_mcs->project->project_weld_trace[weld_trace_num].point=interpolatweld;
        m_mcs->project->project_weld_trace[weld_trace_num].Sample=m_mcs->project->project_weld_trace[weld_trace_creatnum].Sample;
        if(m_mcs->e2proomdata.maindlg_SaveDatacheckBox!=0)//保存焊接轨迹
        {
            QString dir="./log/";
            QString key=SAVELOGFILE_CREATNAME_HEAD;
            QString time;
            std::string s_time;
            TimeFunction to;
            to.get_time_ms(&s_time);
            time=QString::fromStdString(s_time);
            dir=dir+time+key+nameout;
            savelog_creat(dir,m_mcs->project->project_weld_trace[weld_trace_num].point);
        }
    }  
    else if(key==CMD_CREATADD_KEY)
    {
        QString name=cmd.cmd_creatadd_nameout;//获取到的生成的轨迹名字
        std::vector<QString> creatsname=cmd.cmd_creatadd_names;//获取到的轨道名字
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
        std::vector<int> weld_creatsname_num(creatsname.size());
        for(int m=0;m<creatsname.size();m++)
        {
            QString pointname=creatsname[m];
            for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
            {
                if(pointname==m_mcs->project->project_weld_trace[n].name)
                {
                    weld_creatsname_num[m]=n;//找到要储存的焊接点下标
                    break;
                }
            }
        }
        //把这些点按顺序连接
        std::vector<RobPos> interpolatPos;
        bool Sample=true;
        for(int m=0;m<weld_creatsname_num.size();m++)
        {
            std::vector<RobPos> weld=m_mcs->project->project_weld_trace[weld_creatsname_num[m]].point;//获取到移动坐标
            if(m_mcs->project->project_weld_trace[weld_creatsname_num[m]].Sample==false)
            {
                Sample=false;//有一组不是连续采样则整体就不是连续采样
            }
            interpolatPos.insert(interpolatPos.end(),weld.begin(),weld.end());
        }

        //规划后的轨道
        m_mcs->project->project_weld_trace[weld_trace_num].point=interpolatPos;
        m_mcs->project->project_weld_trace[weld_trace_num].Sample=Sample;
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
    else if(key==CMD_CREATADDP_KEY)
    {
        QString name=cmd.cmd_creataddp_nameout;//获取到的生成的轨迹名字
        QString weldname=cmd.cmd_creataddp_weldname;//获取到的轨道名字
        QString pointname=cmd.cmd_creataddp_pointname;//获取到的点名字
        Creataddp_edit_mode mode=cmd.cmd_creataddp_mode;//加入的位置
        int weld_trace_num;//搜索到的焊接轨道序号
        int weldin_trace_num;//搜索到的输入焊接轨道序号
        int point_trace_num;//搜索到的点序号
        std::vector<RobPos> weld;//轨道
        RobPos point;
        for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
        {
            if(name==m_mcs->project->project_weld_trace[n].name)
            {
                weld_trace_num=n;//找到要储存的焊接轨道下标
                break;
            }
        }
        for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
        {
            if(weldname==m_mcs->project->project_weld_trace[n].name)
            {
                weldin_trace_num=n;//找到输入焊接轨道下标
                break;
            }
        }
        for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
        {
            if(pointname==m_mcs->project->projecr_robpos_trace[n].name)
            {
                point_trace_num=n;//找到要储存的点下标
                break;
            }
        }
        if(m_mcs->project->projecr_robpos_trace[point_trace_num].nEn!=true)
        {
            //点无效
            main_record.lock();
            return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+pointname+QString::fromLocal8Bit("点没有获取到坐标值");
            m_mcs->main_record.push_back(return_msg);
            main_record.unlock();
            return 1;
        }
        point=m_mcs->project->projecr_robpos_trace[point_trace_num].robotpos;
        weld=m_mcs->project->project_weld_trace[weldin_trace_num].point;
        std::vector<RobPos> interpolatPos=weld;
        switch(mode)
        {
            case CREATADDP_EDIT_MODE_HEAD:     //点插入在轨迹头部
            {
                interpolatPos.insert(interpolatPos.begin(),point);
            }
            break;
            case CREATADDP_EDIT_MODE_TAIL:     //点插入在轨迹尾部
            {
                interpolatPos.push_back(point);
            }
            break;
        }
        //规划后的轨道
        m_mcs->project->project_weld_trace[weld_trace_num].point=interpolatPos;
        m_mcs->project->project_weld_trace[weld_trace_num].Sample=false;
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
    return 0;
}

int toSendbuffer::cmdlist_build(volatile int &line)
{
    QString return_msg;
    if(line<0||line>=m_mcs->project->project_cmdlist.size())
    {
    #ifdef USE_MYROBOT_CONTROL
        m_mcs->robotcontrol->clear_movepoint_buffer();
        m_mcs->robotcontrol->pause_movepoint_buffer.clear();
    #endif
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
            cmd_lock(1);
            return 1;
        }
        sleep(0);
    //  usleep(ROB_WORK_DELAY_STEP);
    }
#ifdef USE_MYROBOT_CONTROL
    m_mcs->robotcontrol->clear_movepoint_buffer();
#endif
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
            float speed=cmd.cmd_move_speed;//获取到速度值
            Robmovemodel movemod=cmd.cmd_move_movemod;//获取到的移动模式
            QString change=cmd.cmd_move_change;
            int matrix4d_trace_num;
            Eigen::Matrix3d R1;          //旋转矩阵
            Eigen::Vector3d T1;          //平移矩阵(零点坐标)
            Eigen::Matrix3d R;          //旋转矩阵
            Eigen::Vector3d T;          //平移矩阵(零点坐标)
            if(!change.isEmpty())//需要变换矩阵
            {
                for(int n=0;n<m_mcs->project->projecr_coord_matrix4d.size();n++)
                {
                    if(change==m_mcs->project->projecr_coord_matrix4d[n].name)
                    {
                        matrix4d_trace_num=n;//找到变换矩阵下标
                        break;
                    }
                }
                if(m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].nEn!=true)
                {
                    //矩阵无效
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+change+QString::fromLocal8Bit("矩阵没有获取到有效值");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    line=n;
                    return 1;
                }
                R1=m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].R1;
                T1=m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].T1;
                R=m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].R;
                T=m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].T;
            }
            switch(movemod)
            {
                case MOVEL:
                case MOVEJ:
                {
                    RobPos pos=cmd.cmd_move_pos;//获取到移动坐标
                    if(!change.isEmpty())//需要变换矩阵
                    {
                        Eigen::Vector3d tempin,tempout;
                        tempin.x()=pos.X;
                        tempin.y()=pos.Y;
                        tempin.z()=pos.Z;
                        tempout=CCoordChange::_1point2point(tempin,R1,T1);
                        tempout=CCoordChange::point2point(tempout,R,T);
                        pos.X=tempout.x();
                        pos.Y=tempout.y();
                        pos.Z=tempout.z();
                        /************/
                        //姿态自适应变化
                        std::array<double,3> posture={pos.RX,pos.RY,pos.RZ};
                        std::array<double,3> posture_map=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R1,posture);
                        std::array<double,3> posture_out=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R,posture_map);
                        pos.RX=posture_out[0];
                        pos.RY=posture_out[1];
                        pos.RZ=posture_out[2];
                        /************/
                    }
                    cmd_move(pos,movemod,speed,tcp);//移动
                }
                break;
                case MOVEC:
                {
                    RobPos pos1=cmd.cmd_move_pos1;//获取到移动坐标
                    RobPos pos2=cmd.cmd_move_pos2;//获取到移动坐标
                    RobPos pos3=cmd.cmd_move_pos3;//获取到移动坐标
                    if(!change.isEmpty())//需要变换矩阵
                    {
                        Eigen::Vector3d tempin,tempout;
                        tempin.x()=pos1.X;
                        tempin.y()=pos1.Y;
                        tempin.z()=pos1.Z;
                        tempout=CCoordChange::_1point2point(tempin,R1,T1);
                        tempout=CCoordChange::point2point(tempout,R,T);
                        pos1.X=tempout.x();
                        pos1.Y=tempout.y();
                        pos1.Z=tempout.z();
                        /************/
                        //姿态自适应变化
                        std::array<double,3> posture1={pos1.RX,pos1.RY,pos1.RZ};
                        std::array<double,3> posture_map1=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R1,posture1);
                        std::array<double,3> posture_out1=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R,posture_map1);
                        pos1.RX=posture_out1[0];
                        pos1.RY=posture_out1[1];
                        pos1.RZ=posture_out1[2];
                        /************/
                        tempin.x()=pos2.X;
                        tempin.y()=pos2.Y;
                        tempin.z()=pos2.Z;
                        tempout=CCoordChange::_1point2point(tempin,R1,T1);
                        tempout=CCoordChange::point2point(tempout,R,T);
                        pos2.X=tempout.x();
                        pos2.Y=tempout.y();
                        pos2.Z=tempout.z();
                        /************/
                        //姿态自适应变化
                        std::array<double,3> posture2={pos2.RX,pos2.RY,pos2.RZ};
                        std::array<double,3> posture_map2=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R1,posture2);
                        std::array<double,3> posture_out2=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R,posture_map2);
                        pos2.RX=posture_out2[0];
                        pos2.RY=posture_out2[1];
                        pos2.RZ=posture_out2[2];
                        /************/
                        tempin.x()=pos3.X;
                        tempin.y()=pos3.Y;
                        tempin.z()=pos3.Z;
                        tempout=CCoordChange::_1point2point(tempin,R1,T1);
                        tempout=CCoordChange::point2point(tempout,R,T);
                        pos3.X=tempout.x();
                        pos3.Y=tempout.y();
                        pos3.Z=tempout.z();
                        /************/
                        //姿态自适应变化
                        std::array<double,3> posture3={pos3.RX,pos3.RY,pos3.RZ};
                        std::array<double,3> posture_map3=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R1,posture3);
                        std::array<double,3> posture_out3=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R,posture_map3);
                        pos3.RX=posture_out3[0];
                        pos3.RY=posture_out3[1];
                        pos3.RZ=posture_out3[2];
                        /************/
                    }
                #ifdef USE_MYMOVEC_CONTROL
                    CWeldTarject tarjectMath;
                    std::vector<RobPos> interpolatPos;
                    if(false==tarjectMath.pos_circle(m_mcs->rob->cal_posture_model,pos1,pos2,pos3,interpolatPos,ROBOT_POSE_MOVEC_STEP,16,speed))
                    {
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 圆弧三点轨迹拟合出错");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        line=n;
                        return 1;
                    }
                    for(int n=0;n<interpolatPos.size();n++)
                    {
                        cmd_move(interpolatPos[n],MOVEL,speed,tcp);
                    }
                #else
                    cmd_move(pos1,MOVEL,speed,tcp);
                    cmd_moveC(pos2,pos3,MOVEC,speed,tcp);
                #endif
                }
                break;
            }
            //判断下一句如果不是MOVE类型，那就必须等这句移动完
            bool b_continuemove=false;//紧接着移动
            for(int next=n+1;next<m_mcs->project->project_cmdlist.size();next++)
            {
                int rc=cmd.decodecmd(m_mcs->project->project_cmdlist[next],msg,key);
                if(rc>0)
                {
                    //语法出错
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n+1)+QString::fromLocal8Bit(": 语法出错(")+msg+QString::fromLocal8Bit(")");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    line=n;
                    return 1;
                }
                else if(rc<0)//注释行
                {
                    continue;
                }
                else if(rc==0)
                {
                    //下一句还是移动，可以紧接着动
                    if(key==CMD_SMOV_KEY||
                       key==CMD_MOV_KEY)
                    {
                        b_continuemove=true;//
                    }
                    break;
                }
            }
            if(b_continuemove==false)
            {
                usleep(ROB_WORK_DELAY);
                while(m_mcs->rob->robot_state!=ROBOT_STATE_IDLE)//等待移动到位
                {
                    if(b_cmdlist_build==false)     //停止
                    {
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("手动停止进程");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        paused_key=key;
                        cmd_lock(1);
                        line=n;
                        return 1;
                    }
                    sleep(0);
                //  usleep(ROB_WORK_DELAY_STEP);
                }
            #ifdef USE_MYROBOT_CONTROL
                m_mcs->robotcontrol->clear_movepoint_buffer();
            #endif
            }
        }
        else if(key==CMD_SMOV_KEY)//点位移动指令
        {
            int tcp=cmd.cmd_smove_tcp;//获取到移动TCP
            float speed=cmd.cmd_smove_speed;//获取到速度值
            Robmovemodel movemod=cmd.cmd_smove_movemod;//获取到的移动模式
            QString change=cmd.cmd_smove_change;//找到变换矩阵名字
            QString s_pos=cmd.cmd_smove_pos;//找到的点位名字
            QString s_pos1=cmd.cmd_smove_pos1;//找到的点位名字
            QString s_pos2=cmd.cmd_smove_pos2;//找到的点位名字
            QString s_pos3=cmd.cmd_smove_pos3;//找到的点位名字
            int matrix4d_trace_num;//找到变换矩阵下标
            int point_trace_num;//找到点位下标
            int point1_trace_num;//找到点位下标
            int point2_trace_num;//找到点位下标
            int point3_trace_num;//找到点位下标
            Eigen::Matrix3d R;          //旋转矩阵
            Eigen::Vector3d T;          //平移矩阵(零点坐标)
            Eigen::Matrix3d R1;          //旋转矩阵
            Eigen::Vector3d T1;          //平移矩阵(零点坐标)
            if(!change.isEmpty())//需要变换矩阵
            {
                for(int n=0;n<m_mcs->project->projecr_coord_matrix4d.size();n++)
                {
                    if(change==m_mcs->project->projecr_coord_matrix4d[n].name)
                    {
                        matrix4d_trace_num=n;//找到变换矩阵下标
                        break;
                    }
                }
                if(m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].nEn!=true)
                {
                    //矩阵无效
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+change+QString::fromLocal8Bit("矩阵没有获取到有效值");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    line=n;
                    return 1;
                }
                R=m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].R;
                T=m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].T;
                R1=m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].R1;
                T1=m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].T1;
            }
            switch(movemod)
            {
                case MOVEL:
                case MOVEJ:
                {
                    for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
                    {
                        if(s_pos==m_mcs->project->projecr_robpos_trace[n].name)
                        {
                            point_trace_num=n;//找到要储存的寻位点下标
                            break;
                        }
                    }
                    if(m_mcs->project->projecr_robpos_trace[point_trace_num].nEn!=true)
                    {
                        //点无效
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+s_pos+QString::fromLocal8Bit("点没有获取到坐标值");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        line=n;
                        return 1;
                    }
                    RobPos pos=m_mcs->project->projecr_robpos_trace[point_trace_num].robotpos;//获取到移动坐标
                    if(!change.isEmpty())//需要变换矩阵
                    {
                        Eigen::Vector3d tempin,tempout;
                        tempin.x()=pos.X;
                        tempin.y()=pos.Y;
                        tempin.z()=pos.Z;
                        tempout=CCoordChange::_1point2point(tempin,R1,T1);
                        tempout=CCoordChange::point2point(tempout,R,T);
                        pos.X=tempout.x();
                        pos.Y=tempout.y();
                        pos.Z=tempout.z();
                        /************/
                        //姿态自适应变化
                        std::array<double,3> posture={pos.RX,pos.RY,pos.RZ};
                        std::array<double,3> posture_map=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R1,posture);
                        std::array<double,3> posture_out=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R,posture_map);
                        pos.RX=posture_out[0];
                        pos.RY=posture_out[1];
                        pos.RZ=posture_out[2];
                        /************/
                    }
                    cmd_move(pos,movemod,speed,tcp);//移动
                }
                break;
                case MOVEC:
                {
                    for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
                    {
                        if(s_pos1==m_mcs->project->projecr_robpos_trace[n].name)
                        {
                            point1_trace_num=n;//找到要储存的寻位点下标
                            break;
                        }
                    }
                    if(m_mcs->project->projecr_robpos_trace[point1_trace_num].nEn!=true)
                    {
                        //点无效
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+s_pos1+QString::fromLocal8Bit("点没有获取到坐标值");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        line=n;
                        return 1;
                    }
                    for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
                    {
                        if(s_pos2==m_mcs->project->projecr_robpos_trace[n].name)
                        {
                            point2_trace_num=n;//找到要储存的寻位点下标
                            break;
                        }
                    }
                    if(m_mcs->project->projecr_robpos_trace[point2_trace_num].nEn!=true)
                    {
                        //点无效
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+s_pos2+QString::fromLocal8Bit("点没有获取到坐标值");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        line=n;
                        return 1;
                    }
                    for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
                    {
                        if(s_pos3==m_mcs->project->projecr_robpos_trace[n].name)
                        {
                            point3_trace_num=n;//找到要储存的寻位点下标
                            break;
                        }
                    }
                    if(m_mcs->project->projecr_robpos_trace[point3_trace_num].nEn!=true)
                    {
                        //点无效
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+s_pos3+QString::fromLocal8Bit("点没有获取到坐标值");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        line=n;
                        return 1;
                    }
                    RobPos pos1=m_mcs->project->projecr_robpos_trace[point1_trace_num].robotpos;//获取到移动坐标
                    RobPos pos2=m_mcs->project->projecr_robpos_trace[point2_trace_num].robotpos;//获取到移动坐标
                    RobPos pos3=m_mcs->project->projecr_robpos_trace[point3_trace_num].robotpos;//获取到移动坐标
                    if(!change.isEmpty())//需要变换矩阵
                    {
                        Eigen::Vector3d tempin,tempout;
                        tempin.x()=pos1.X;
                        tempin.y()=pos1.Y;
                        tempin.z()=pos1.Z;
                        tempout=CCoordChange::_1point2point(tempin,R1,T1);
                        tempout=CCoordChange::point2point(tempout,R,T);
                        pos1.X=tempout.x();
                        pos1.Y=tempout.y();
                        pos1.Z=tempout.z();
                        /************/
                        //姿态自适应变化
                        std::array<double,3> posture1={pos1.RX,pos1.RY,pos1.RZ};
                        std::array<double,3> posture_map1=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R1,posture1);
                        std::array<double,3> posture_out1=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R,posture_map1);
                        pos1.RX=posture_out1[0];
                        pos1.RY=posture_out1[1];
                        pos1.RZ=posture_out1[2];
                        /************/
                        tempin.x()=pos2.X;
                        tempin.y()=pos2.Y;
                        tempin.z()=pos2.Z;
                        tempout=CCoordChange::_1point2point(tempin,R1,T1);
                        tempout=CCoordChange::point2point(tempout,R,T);
                        pos2.X=tempout.x();
                        pos2.Y=tempout.y();
                        pos2.Z=tempout.z();
                        /************/
                        //姿态自适应变化
                        std::array<double,3> posture2={pos2.RX,pos2.RY,pos2.RZ};
                        std::array<double,3> posture_map2=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R1,posture2);
                        std::array<double,3> posture_out2=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R,posture_map2);
                        pos2.RX=posture_out2[0];
                        pos2.RY=posture_out2[1];
                        pos2.RZ=posture_out2[2];
                        /************/
                        tempin.x()=pos3.X;
                        tempin.y()=pos3.Y;
                        tempin.z()=pos3.Z;
                        tempout=CCoordChange::_1point2point(tempin,R1,T1);
                        tempout=CCoordChange::point2point(tempout,R,T);
                        pos3.X=tempout.x();
                        pos3.Y=tempout.y();
                        pos3.Z=tempout.z();
                        /************/
                        //姿态自适应变化
                        std::array<double,3> posture3={pos3.RX,pos3.RY,pos3.RZ};
                        std::array<double,3> posture_map3=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R1,posture3);
                        std::array<double,3> posture_out3=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R,posture_map3);
                        pos3.RX=posture_out3[0];
                        pos3.RY=posture_out3[1];
                        pos3.RZ=posture_out3[2];
                        /************/
                    }
                #ifdef USE_MYMOVEC_CONTROL
                    CWeldTarject tarjectMath;
                    std::vector<RobPos> interpolatPos;
                    if(false==tarjectMath.pos_circle(m_mcs->rob->cal_posture_model,pos1,pos2,pos3,interpolatPos,ROBOT_POSE_MOVEC_STEP,16,speed))
                    {
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 圆弧三点轨迹拟合出错");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        line=n;
                        return 1;
                    }
                    for(int n=0;n<interpolatPos.size();n++)
                    {
                        cmd_move(interpolatPos[n],MOVEL,speed,tcp);
                    }
                #else
                    cmd_move(pos1,MOVEL,speed,tcp);
                    cmd_moveC(pos2,pos3,MOVEC,speed,tcp);
                #endif
                }
                break;
            }
            //判断下一句如果不是MOVE类型，那就必须等这句移动完
            bool b_continuemove=false;//紧接着移动
            for(int next=n+1;next<m_mcs->project->project_cmdlist.size();next++)
            {
                int rc=cmd.decodecmd(m_mcs->project->project_cmdlist[next],msg,key);
                if(rc>0)
                {
                    //语法出错
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n+1)+QString::fromLocal8Bit(": 语法出错(")+msg+QString::fromLocal8Bit(")");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    line=n;
                    return 1;
                }
                else if(rc<0)//注释行
                {
                    continue;
                }
                else if(rc==0)
                {
                    //下一句还是移动，可以紧接着动
                    if(key==CMD_SMOV_KEY||
                       key==CMD_MOV_KEY)
                    {
                        b_continuemove=true;//
                    }
                    break;
                }
            }
            if(b_continuemove==false)
            {
                usleep(ROB_WORK_DELAY);
                while(m_mcs->rob->robot_state!=ROBOT_STATE_IDLE)//等待移动到位
                {
                    if(b_cmdlist_build==false)     //停止
                    {
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("手动停止进程");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        paused_key=key;
                        cmd_lock(1);
                        line=n;
                        return 1;
                    }
                    sleep(0);
                //  usleep(ROB_WORK_DELAY_STEP);
                }
            #ifdef USE_MYROBOT_CONTROL
                m_mcs->robotcontrol->clear_movepoint_buffer();
            #endif
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
            Weldworkmodel_ID work=(Weldworkmodel_ID)cmd.cmd_elec_work_d;//获取到焊机启停
            Alternatingcurrent elem=cmd.cmd_elec_elem;  //获取到焊机交变电流模式
            float eled=cmd.cmd_elec_eled; //获取到焊机电流
            float elev=cmd.cmd_elec_elev; //获取到焊机电压
            cmd_elec(eled,elev,elem,work);
            usleep(ROB_WORK_DELAY);//确保焊机设置完成
        }
        else if(key==CMD_IO_KEY)//IO指令
        {
            switch(cmd.cmd_io_workmod)
            {
                case OUT://输出IO
                {
                    std::vector<int> io_out=cmd.cmd_io_output;
                    cmd_ioout(io_out);
                }
                break;
                case WAITIN://等待IO输入
                {
                    std::vector<int> io_in=cmd.cmd_io_input;
                    bool b_io=true;
                    for(int n=0;n<ROBOTINPUTNUM;n++)
                    {
                        if(io_in[n]!=m_mcs->rob->robioinput[n])
                        {
                            b_io=false;
                            break;
                        }
                    }
                    while(b_io==false)
                    {
                        b_io=true;
                        for(int n=0;n<ROBOTINPUTNUM;n++)
                        {
                            if(io_in[n]!=m_mcs->rob->robioinput[n])
                            {
                                b_io=false;
                                break;
                            }
                        }
                        if(b_cmdlist_build==false)     //停止
                        {
                            main_record.lock();
                            return_msg=QString::fromLocal8Bit("手动停止进程");
                            m_mcs->main_record.push_back(return_msg);
                            main_record.unlock();
                            paused_key=key;
                            cmd_lock(1);
                            line=n;
                            return 1;
                        }
                        sleep(0);
                    }
                }
                break;
            }
        }
        else if(key==CMD_PLC_KEY)//PLC指令
        {
            if(m_mcs->rob->b_link_ctx_plc==false)
            {
                main_record.lock();
                return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 没有与PLC建立通信");
                m_mcs->main_record.push_back(return_msg);
                main_record.unlock();
                line=n;
                return 1;
            }
            switch(cmd.cmd_plc_mod)
            {
                case PLC_WRITE://输出plc
                {
                    int add=cmd.cmd_plc_register_add;
                    uint16_t data=(uint16_t)cmd.cmd_plc_register_data;
                    if(1!=modbus_write_registers(m_mcs->rob->ctx_plc,add,1,&data))
                    {
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 与PLC通信出错");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        line=n;
                        return 1;
                    }
                }
                break;
                case PLC_WAIT://等待PLC输入
                {
                    int add=cmd.cmd_plc_register_add;
                    int data=cmd.cmd_plc_register_data;
                    bool b_plc=false;
                    uint16_t plc_rcv_data;
                    while(b_plc==false)
                    {
                        b_plc=true;
                        if(1==modbus_read_registers(m_mcs->rob->ctx_plc,add,1,&plc_rcv_data))
                        {
                            if(plc_rcv_data==data)
                            {
                                break;
                            }
                        }
                        else
                        {
                            main_record.lock();
                            return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 与PLC通信出错");
                            m_mcs->main_record.push_back(return_msg);
                            main_record.unlock();
                            line=n;
                            return 1;
                        }
                        if(b_cmdlist_build==false)     //停止
                        {
                            main_record.lock();
                            return_msg=QString::fromLocal8Bit("手动停止进程");
                            m_mcs->main_record.push_back(return_msg);
                            main_record.unlock();
                            paused_key=key;
                            cmd_lock(1);
                            line=n;
                            return 1;
                        }
                        sleep(0);
                    }
                }
                break;
            }
        }
        else if(key==CMD_SEARCH_KEY)//起点寻位指令
        {
            bool b_find=false;//成功找到
            float speed=cmd.cmd_search_speed;//获取到的寻位速度
            int tcp=cmd.cmd_search_tcp;//获取到寻位TCP
            Robmovemodel movemod=cmd.cmd_search_movemod;//获取到的寻位模式
            QString name=cmd.cmd_search_name;//获取到的寻位名字
            int side=cmd.cmd_search_side;//获取到左右两侧是否还要寻位
            std::vector<float> sidemove=cmd.cmd_search_sidemove;//获取每次两侧寻位的移动向量
            float sidespeed=cmd.cmd_search_sidespeed;//获取每次两侧寻位的空闲移动速度
            int robpos_trace_num;//要储存的寻位点下标
            QString change=cmd.cmd_search_change;//变换矩阵
            int matrix4d_trace_num;//要变换矩阵下标
            Eigen::Matrix3d R;          //旋转矩阵
            Eigen::Vector3d T;          //平移矩阵(零点坐标)
            Eigen::Matrix3d R1;          //旋转矩阵
            Eigen::Vector3d T1;          //平移矩阵(零点坐标)
            if(!change.isEmpty())//需要变换矩阵
            {
                for(int n=0;n<m_mcs->project->projecr_coord_matrix4d.size();n++)
                {
                    if(change==m_mcs->project->projecr_coord_matrix4d[n].name)
                    {
                        matrix4d_trace_num=n;//找到变换矩阵下标
                        break;
                    }
                }
                if(m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].nEn!=true)
                {
                    //矩阵无效
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+change+QString::fromLocal8Bit("矩阵没有获取到有效值");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    line=n;
                    return 1;
                }
                R=m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].R;
                T=m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].T;
                R1=m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].R1;
                T1=m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].T1;
            }
            for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
            {
                if(name==m_mcs->project->projecr_robpos_trace[n].name)
                {
                    robpos_trace_num=n;//找到要储存的寻位点下标
                    break;
                }
            }
            //先记录下当前位置
            RobPos oldpos=m_mcs->rob->TCPpos;
            RobPos pos=cmd.cmd_search_pos,pos1=cmd.cmd_search_pos1,pos2=cmd.cmd_search_pos2,pos3=cmd.cmd_search_pos3;

            switch(movemod)
            {
                case MOVEL:
                case MOVEJ:
                {
                    if(!change.isEmpty())//需要变换矩阵
                    {
                        Eigen::Vector3d tempin,tempout;
                        tempin.x()=pos.X;
                        tempin.y()=pos.Y;
                        tempin.z()=pos.Z;
                        tempout=CCoordChange::_1point2point(tempin,R1,T1);
                        tempout=CCoordChange::point2point(tempout,R,T);
                        pos.X=tempout.x();
                        pos.Y=tempout.y();
                        pos.Z=tempout.z();
                        /************/
                        //姿态自适应变化
                        std::array<double,3> posture={pos.RX,pos.RY,pos.RZ};
                        std::array<double,3> posture_map=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R1,posture);
                        std::array<double,3> posture_out=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R,posture_map);
                        pos.RX=posture_out[0];
                        pos.RY=posture_out[1];
                        pos.RZ=posture_out[2];
                        /************/
                    }
                }
                break;
                case MOVEC:
                {
                    if(!change.isEmpty())//需要变换矩阵
                    {
                        Eigen::Vector3d tempin,tempout;
                        tempin.x()=pos1.X;
                        tempin.y()=pos1.Y;
                        tempin.z()=pos1.Z;
                        tempout=CCoordChange::_1point2point(tempin,R1,T1);
                        tempout=CCoordChange::point2point(tempout,R,T);
                        pos1.X=tempout.x();
                        pos1.Y=tempout.y();
                        pos1.Z=tempout.z();
                        /************/
                        //姿态自适应变化
                        std::array<double,3> posture1={pos1.RX,pos1.RY,pos1.RZ};
                        std::array<double,3> posture_map1=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R1,posture1);
                        std::array<double,3> posture_out1=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R,posture_map1);
                        pos1.RX=posture_out1[0];
                        pos1.RY=posture_out1[1];
                        pos1.RZ=posture_out1[2];
                        /************/
                        tempin.x()=pos2.X;
                        tempin.y()=pos2.Y;
                        tempin.z()=pos2.Z;
                        tempout=CCoordChange::_1point2point(tempin,R1,T1);
                        tempout=CCoordChange::point2point(tempout,R,T);
                        pos2.X=tempout.x();
                        pos2.Y=tempout.y();
                        pos2.Z=tempout.z();
                        /************/
                        //姿态自适应变化
                        std::array<double,3> posture2={pos2.RX,pos2.RY,pos2.RZ};
                        std::array<double,3> posture_map2=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R1,posture2);
                        std::array<double,3> posture_out2=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R,posture_map2);
                        pos2.RX=posture_out2[0];
                        pos2.RY=posture_out2[1];
                        pos2.RZ=posture_out2[2];
                        /************/
                        tempin.x()=pos3.X;
                        tempin.y()=pos3.Y;
                        tempin.z()=pos3.Z;
                        tempout=CCoordChange::_1point2point(tempin,R1,T1);
                        tempout=CCoordChange::point2point(tempout,R,T);
                        pos3.X=tempout.x();
                        pos3.Y=tempout.y();
                        pos3.Z=tempout.z();
                        /************/
                        //姿态自适应变化
                        std::array<double,3> posture3={pos3.RX,pos3.RY,pos3.RZ};
                        std::array<double,3> posture_map3=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R1,posture3);
                        std::array<double,3> posture_out3=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R,posture_map3);
                        pos3.RX=posture_out3[0];
                        pos3.RY=posture_out3[1];
                        pos3.RZ=posture_out3[2];
                        /************/
                    }
                }
                break;
            }

            Eigen::Vector3d st,ed,ed1,ed2,ed3;

            st.x()=oldpos.X;
            st.y()=oldpos.Y;
            st.z()=oldpos.Z;
            ed.x()=pos.X;
            ed.y()=pos.Y;
            ed.z()=pos.Z;
            ed1.x()=pos1.X;
            ed1.y()=pos1.Y;
            ed1.z()=pos1.Z;
            ed2.x()=pos2.X;
            ed2.y()=pos2.Y;
            ed2.z()=pos2.Z;
            ed3.x()=pos3.X;
            ed3.y()=pos3.Y;
            ed3.z()=pos3.Z;

            for(int nside=0;nside<=side*2;nside++)
            {
                //清空检测状态
                m_mcs->cam->sop_cam[0].b_updatacloud_finish=false;
                m_mcs->cam->sop_cam[0].b_ros_lineEn=false;
                //打开激光器
                switch(movemod)
                {
                    case MOVEL:
                    case MOVEJ:
                    {
                        cmd_move(pos,movemod,speed,tcp);
                    }
                    break;
                    case MOVEC:
                    {
                    #ifdef USE_MYMOVEC_CONTROL
                        CWeldTarject tarjectMath;
                        std::vector<RobPos> interpolatPos;
                        if(false==tarjectMath.pos_circle(m_mcs->rob->cal_posture_model,pos1,pos2,pos3,interpolatPos,ROBOT_POSE_MOVEC_STEP,16,speed))
                        {
                            main_record.lock();
                            return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 圆弧三点轨迹拟合出错");
                            m_mcs->main_record.push_back(return_msg);
                            main_record.unlock();
                            line=n;
                            return 1;
                        }
                        for(int n=0;n<interpolatPos.size();n++)
                        {
                            cmd_move(interpolatPos[n],MOVEL,speed,tcp);
                        }
                    #else
                        cmd_move(pos1,MOVEL,speed,tcp);
                        cmd_moveC(pos2,pos3,MOVEC,speed,tcp);
                    #endif
                    }
                    break;
                }
                usleep(ROB_WORK_DELAY);
                while(m_mcs->rob->robot_state!=ROBOT_STATE_IDLE)//等待寻位到位
                {
                    if(b_cmdlist_build==false)     //停止
                    {
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("手动停止进程");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        paused_key=key;
                        cmd_lock(1);
                        line=n;
                        return 1;
                    }
                    //这里添加扫描数据
                    if(m_mcs->cam->sop_cam[0].b_updatacloud_finish==true)//有中断数据
                    {
                        if(m_mcs->cam->sop_cam[0].b_ros_lineEn==true)//检测有正确结果
                        {
                            std::vector<Scan_trace_line> scan_trace(1);
                            std::vector<RobPos> weld_trace;
                            scan_trace[0].robottime=m_mcs->rob->robtime;
                            scan_trace[0].ros_line=*(m_mcs->cam->sop_cam[0].ros_line);
                        #if OPEN_TIMESTAMP==1
                            scan_trace[0].robotpos.X=scan_trace[0].ros_line.robpos.posx;
                            scan_trace[0].robotpos.Y=scan_trace[0].ros_line.robpos.posy;
                            scan_trace[0].robotpos.Z=scan_trace[0].ros_line.robpos.posz;
                            scan_trace[0].robotpos.RX=scan_trace[0].ros_line.robpos.posrx;
                            scan_trace[0].robotpos.RY=scan_trace[0].ros_line.robpos.posry;
                            scan_trace[0].robotpos.RZ=scan_trace[0].ros_line.robpos.posrz;
                            scan_trace[0].robotpos.out_1=scan_trace[0].ros_line.robpos.posout1;
                            scan_trace[0].robotpos.out_2=scan_trace[0].ros_line.robpos.posout2;
                            scan_trace[0].robotpos.out_3=scan_trace[0].ros_line.robpos.posout3;
                            scan_trace[0].robotpos.nEn=1;
                        #else
                            scan_trace[0].robotpos=m_mcs->rob->TCPpos;
                        #endif
                            if(false==m_mcs->synchronous->Scantrace_to_Weldtrace(scan_trace,weld_trace))
                            {
                                main_record.lock();
                                return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 寻位计算结果出错");
                                m_mcs->main_record.push_back(return_msg);
                                main_record.unlock();
                                line=n;
                                return 1;
                            }
                            m_mcs->project->projecr_robpos_trace[robpos_trace_num].robotpos=scan_trace[0].robotpos;
                            m_mcs->project->projecr_robpos_trace[robpos_trace_num].robotpos.X=weld_trace[0].X;
                            m_mcs->project->projecr_robpos_trace[robpos_trace_num].robotpos.Y=weld_trace[0].Y;
                            m_mcs->project->projecr_robpos_trace[robpos_trace_num].robotpos.Z=weld_trace[0].Z;
                            m_mcs->project->projecr_robpos_trace[robpos_trace_num].nEn=true;
                            b_find=true;
                        }
                        m_mcs->cam->sop_cam[0].b_updatacloud_finish=false;
                    }
                    if(b_find==true)//成功找到了点
                    {
                        break;
                    }
                    //开始采集检测数据
                    usleep(0);
                }
            #ifdef USE_MYROBOT_CONTROL
                m_mcs->robotcontrol->clear_movepoint_buffer();
            #endif
                if(b_find==true)//成功找到了点
                {
                    cmd_lock(0);    //让机器人先停下当前的寻位运动
                    break;
                }
                else    //移动到下一个点
                {
                    if(nside<=side*2-1)
                    {
                        Eigen::Vector3d V;
                        Eigen::Vector3d nextst;
                        Eigen::Vector3d nexted,nexted1,nexted2,nexted3;
                        int times=nside+1;
                        int ns=(times-1)/2+1;

                        if((times%2)==0)//偶数
                        {
                            //计算pos;
                            V.x()=ns*cmd.cmd_search_sidemove[0];
                            V.y()=ns*cmd.cmd_search_sidemove[1];
                            V.z()=ns*cmd.cmd_search_sidemove[2];
                        }
                        else//奇数
                        {
                            //计算pos;
                            V.x()=-ns*cmd.cmd_search_sidemove[0];
                            V.y()=-ns*cmd.cmd_search_sidemove[1];
                            V.z()=-ns*cmd.cmd_search_sidemove[2];
                        }
                        nextst=st+V;
                        switch(movemod)
                        {
                            case MOVEL:
                            case MOVEJ:
                            {
                                nexted=ed+V;
                                pos.X=nexted.x();
                                pos.Y=nexted.y();
                                pos.Z=nexted.z();
                            }
                            break;
                            case MOVEC:
                            {
                                nexted1=ed1+V;
                                nexted2=ed2+V;
                                nexted3=ed3+V;
                                pos1.X=nexted1.x();
                                pos1.Y=nexted1.y();
                                pos1.Z=nexted1.z();
                                pos2.X=nexted2.x();
                                pos2.Y=nexted2.y();
                                pos2.Z=nexted2.z();
                                pos3.X=nexted3.x();
                                pos3.Y=nexted3.y();
                                pos3.Z=nexted3.z();
                            }
                            break;
                        }
                        //移动到下一个起点
                        RobPos Nextpos=oldpos;
                        Nextpos.X=nextst.x();
                        Nextpos.Y=nextst.y();
                        Nextpos.Z=nextst.z();
                        cmd_move(Nextpos,MOVEJ,sidespeed,tcp);

                        usleep(ROB_WORK_DELAY);
                        while(m_mcs->rob->robot_state!=ROBOT_STATE_IDLE)//等待移动到位
                        {
                            if(b_cmdlist_build==false)     //停止
                            {
                                main_record.lock();
                                return_msg=QString::fromLocal8Bit("手动停止进程");
                                m_mcs->main_record.push_back(return_msg);
                                main_record.unlock();
                                paused_key=key;
                                cmd_lock(1);
                                line=n;
                                return 1;
                            }
                            sleep(0);
                        //  usleep(ROB_WORK_DELAY_STEP);
                        }
                    #ifdef USE_MYROBOT_CONTROL
                        m_mcs->robotcontrol->clear_movepoint_buffer();
                    #endif
                    }
                    else//没有找到寻位点
                    {
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 寻位点找不到");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        line=n;
                        return 1;
                    }
                }
            }
            /*********************/
            //掉电保存
            savetemp_pos(m_mcs->project->projecr_robpos_trace[robpos_trace_num]);
        }
        else if(key==CMD_SEARCHEND_KEY)//终点寻位指令
        {
            bool b_find=false;//成功找到
            float speed=cmd.cmd_searchend_speed;//获取到的寻位速度
            int tcp=cmd.cmd_searchend_tcp;//获取到寻位TCP
            Robmovemodel movemod=cmd.cmd_searchend_movemod;//获取到的寻位模式
            QString name=cmd.cmd_searchend_name;//获取到的寻位名字
            int robpos_trace_num;//要储存的寻位点下标
            QString change=cmd.cmd_searchend_change;//变换矩阵
            int matrix4d_trace_num;//要变换矩阵下标
            Eigen::Matrix3d R;          //旋转矩阵
            Eigen::Vector3d T;          //平移矩阵(零点坐标)
            Eigen::Matrix3d R1;          //旋转矩阵
            Eigen::Vector3d T1;          //平移矩阵(零点坐标)
            if(!change.isEmpty())//需要变换矩阵
            {
                for(int n=0;n<m_mcs->project->projecr_coord_matrix4d.size();n++)
                {
                    if(change==m_mcs->project->projecr_coord_matrix4d[n].name)
                    {
                        matrix4d_trace_num=n;//找到变换矩阵下标
                        break;
                    }
                }
                if(m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].nEn!=true)
                {
                    //矩阵无效
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+change+QString::fromLocal8Bit("矩阵没有获取到有效值");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    line=n;
                    return 1;
                }
                R=m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].R;
                T=m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].T;
                R1=m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].R1;
                T1=m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].T1;
            }
            for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
            {
                if(name==m_mcs->project->projecr_robpos_trace[n].name)
                {
                    robpos_trace_num=n;//找到要储存的寻位点下标
                    break;
                }
            }
            //先记录下当前位置
            RobPos oldpos=m_mcs->rob->TCPpos;
            RobPos pos=cmd.cmd_searchend_pos,pos1=cmd.cmd_searchend_pos1,pos2=cmd.cmd_searchend_pos2,pos3=cmd.cmd_searchend_pos3;

            switch(movemod)
            {
                case MOVEL:
                case MOVEJ:
                {
                    if(!change.isEmpty())//需要变换矩阵
                    {
                        Eigen::Vector3d tempin,tempout;
                        tempin.x()=pos.X;
                        tempin.y()=pos.Y;
                        tempin.z()=pos.Z;
                        tempout=CCoordChange::_1point2point(tempin,R1,T1);
                        tempout=CCoordChange::point2point(tempout,R,T);
                        pos.X=tempout.x();
                        pos.Y=tempout.y();
                        pos.Z=tempout.z();
                        /************/
                        //姿态自适应变化
                        std::array<double,3> posture={pos.RX,pos.RY,pos.RZ};
                        std::array<double,3> posture_map=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R1,posture);
                        std::array<double,3> posture_out=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R,posture_map);
                        pos.RX=posture_out[0];
                        pos.RY=posture_out[1];
                        pos.RZ=posture_out[2];
                        /************/
                    }
                }
                break;
                case MOVEC:
                {
                    if(!change.isEmpty())//需要变换矩阵
                    {
                        Eigen::Vector3d tempin,tempout;
                        tempin.x()=pos1.X;
                        tempin.y()=pos1.Y;
                        tempin.z()=pos1.Z;
                        tempout=CCoordChange::_1point2point(tempin,R1,T1);
                        tempout=CCoordChange::point2point(tempout,R,T);
                        pos1.X=tempout.x();
                        pos1.Y=tempout.y();
                        pos1.Z=tempout.z();
                        /************/
                        //姿态自适应变化
                        std::array<double,3> posture1={pos1.RX,pos1.RY,pos1.RZ};
                        std::array<double,3> posture_map1=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R1,posture1);
                        std::array<double,3> posture_out1=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R,posture_map1);
                        pos1.RX=posture_out1[0];
                        pos1.RY=posture_out1[1];
                        pos1.RZ=posture_out1[2];
                        /************/
                        tempin.x()=pos2.X;
                        tempin.y()=pos2.Y;
                        tempin.z()=pos2.Z;
                        tempout=CCoordChange::_1point2point(tempin,R1,T1);
                        tempout=CCoordChange::point2point(tempout,R,T);
                        pos2.X=tempout.x();
                        pos2.Y=tempout.y();
                        pos2.Z=tempout.z();
                        /************/
                        //姿态自适应变化
                        std::array<double,3> posture2={pos2.RX,pos2.RY,pos2.RZ};
                        std::array<double,3> posture_map2=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R1,posture2);
                        std::array<double,3> posture_out2=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R,posture_map2);
                        pos2.RX=posture_out2[0];
                        pos2.RY=posture_out2[1];
                        pos2.RZ=posture_out2[2];
                        /************/
                        tempin.x()=pos3.X;
                        tempin.y()=pos3.Y;
                        tempin.z()=pos3.Z;
                        tempout=CCoordChange::_1point2point(tempin,R1,T1);
                        tempout=CCoordChange::point2point(tempout,R,T);
                        pos3.X=tempout.x();
                        pos3.Y=tempout.y();
                        pos3.Z=tempout.z();
                        /************/
                        //姿态自适应变化
                        std::array<double,3> posture3={pos3.RX,pos3.RY,pos3.RZ};
                        std::array<double,3> posture_map3=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R1,posture3);
                        std::array<double,3> posture_out3=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R,posture_map3);
                        pos3.RX=posture_out3[0];
                        pos3.RY=posture_out3[1];
                        pos3.RZ=posture_out3[2];
                        /************/
                    }
                }
                break;
            }

            Eigen::Vector3d st,ed,ed1,ed2,ed3;

            st.x()=oldpos.X;
            st.y()=oldpos.Y;
            st.z()=oldpos.Z;
            ed.x()=pos.X;
            ed.y()=pos.Y;
            ed.z()=pos.Z;
            ed1.x()=pos1.X;
            ed1.y()=pos1.Y;
            ed1.z()=pos1.Z;
            ed2.x()=pos2.X;
            ed2.y()=pos2.Y;
            ed2.z()=pos2.Z;
            ed3.x()=pos3.X;
            ed3.y()=pos3.Y;
            ed3.z()=pos3.Z;

            //清空检测状态
            m_mcs->cam->sop_cam[0].b_updatacloud_finish=false;
            m_mcs->cam->sop_cam[0].b_ros_lineEn=false;

            //打开激光器
            //找第一个点
            while(m_mcs->cam->sop_cam[0].b_updatacloud_finish==false)//等待寻位到位
            {
                if(m_mcs->cam->sop_cam[0].b_ros_lineEn==true)
                {
                    std::vector<Scan_trace_line> scan_trace(1);
                    std::vector<RobPos> weld_trace;
                    scan_trace[0].robottime=m_mcs->rob->robtime;
                    scan_trace[0].ros_line=*(m_mcs->cam->sop_cam[0].ros_line);
                #if OPEN_TIMESTAMP==1
                    scan_trace[0].robotpos.X=scan_trace[0].ros_line.robpos.posx;
                    scan_trace[0].robotpos.Y=scan_trace[0].ros_line.robpos.posy;
                    scan_trace[0].robotpos.Z=scan_trace[0].ros_line.robpos.posz;
                    scan_trace[0].robotpos.RX=scan_trace[0].ros_line.robpos.posrx;
                    scan_trace[0].robotpos.RY=scan_trace[0].ros_line.robpos.posry;
                    scan_trace[0].robotpos.RZ=scan_trace[0].ros_line.robpos.posrz;
                    scan_trace[0].robotpos.out_1=scan_trace[0].ros_line.robpos.posout1;
                    scan_trace[0].robotpos.out_2=scan_trace[0].ros_line.robpos.posout2;
                    scan_trace[0].robotpos.out_3=scan_trace[0].ros_line.robpos.posout3;
                    scan_trace[0].robotpos.nEn=1;
                #else
                    scan_trace[0].robotpos=m_mcs->rob->TCPpos;
                #endif
                    if(false==m_mcs->synchronous->Scantrace_to_Weldtrace(scan_trace,weld_trace))
                    {
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 寻位计算结果出错");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        line=n;
                        return 1;
                    }
                    m_mcs->project->projecr_robpos_trace[robpos_trace_num].robotpos=scan_trace[0].robotpos;
                    m_mcs->project->projecr_robpos_trace[robpos_trace_num].robotpos.X=weld_trace[0].X;
                    m_mcs->project->projecr_robpos_trace[robpos_trace_num].robotpos.Y=weld_trace[0].Y;
                    m_mcs->project->projecr_robpos_trace[robpos_trace_num].robotpos.Z=weld_trace[0].Z;
                    m_mcs->project->projecr_robpos_trace[robpos_trace_num].nEn=true;
                    break;
                }
                else//第一个点寻找失败
                {
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 起点处找不到焊缝");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    line=n;
                    return 1;
                }
            }
            //清空检测状态
            m_mcs->cam->sop_cam[0].b_updatacloud_finish=false;
            m_mcs->cam->sop_cam[0].b_ros_lineEn=false;

            switch(movemod)
            {
                case MOVEL:
                case MOVEJ:
                {
                    cmd_move(pos,movemod,speed,tcp);
                }
                break;
                case MOVEC:
                {
                #ifdef USE_MYMOVEC_CONTROL
                    CWeldTarject tarjectMath;
                    std::vector<RobPos> interpolatPos;
                    if(false==tarjectMath.pos_circle(m_mcs->rob->cal_posture_model,pos1,pos2,pos3,interpolatPos,ROBOT_POSE_MOVEC_STEP,16,speed))
                    {
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 圆弧三点轨迹拟合出错");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        line=n;
                        return 1;
                    }
                    for(int n=0;n<interpolatPos.size();n++)
                    {
                        cmd_move(interpolatPos[n],MOVEL,speed,tcp);
                    }
                #else
                    cmd_move(pos1,MOVEL,speed,tcp);
                    cmd_moveC(pos2,pos3,MOVEC,speed,tcp);
                #endif
                }
                break;
            }
            int unfindnum=0;
            while(m_mcs->rob->robot_state!=ROBOT_STATE_IDLE)//等待寻位到位
            {
                if(b_cmdlist_build==false)     //停止
                {
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("手动停止进程");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    paused_key=key;
                    cmd_lock(1);
                    line=n;
                    return 1;
                }
                //这里添加扫描数据
                if(m_mcs->cam->sop_cam[0].b_updatacloud_finish==true)//有中断数据
                {
                    if(m_mcs->cam->sop_cam[0].b_ros_lineEn==false)//没有有正确结果
                    {
                        unfindnum++;
                        if(unfindnum>=3)//连续3次没有焊缝表示找到终点
                        {
                            b_find=true;
                            break;
                        }
                    }
                    else
                    {
                        std::vector<Scan_trace_line> scan_trace(1);
                        std::vector<RobPos> weld_trace;
                        scan_trace[0].robottime=m_mcs->rob->robtime;
                        scan_trace[0].ros_line=*(m_mcs->cam->sop_cam[0].ros_line);
                    #if OPEN_TIMESTAMP==1
                        scan_trace[0].robotpos.X=scan_trace[0].ros_line.robpos.posx;
                        scan_trace[0].robotpos.Y=scan_trace[0].ros_line.robpos.posy;
                        scan_trace[0].robotpos.Z=scan_trace[0].ros_line.robpos.posz;
                        scan_trace[0].robotpos.RX=scan_trace[0].ros_line.robpos.posrx;
                        scan_trace[0].robotpos.RY=scan_trace[0].ros_line.robpos.posry;
                        scan_trace[0].robotpos.RZ=scan_trace[0].ros_line.robpos.posrz;
                        scan_trace[0].robotpos.out_1=scan_trace[0].ros_line.robpos.posout1;
                        scan_trace[0].robotpos.out_2=scan_trace[0].ros_line.robpos.posout2;
                        scan_trace[0].robotpos.out_3=scan_trace[0].ros_line.robpos.posout3;
                        scan_trace[0].robotpos.nEn=1;
                    #else
                        scan_trace[0].robotpos=m_mcs->rob->TCPpos;
                    #endif
                        if(false==m_mcs->synchronous->Scantrace_to_Weldtrace(scan_trace,weld_trace))
                        {
                            main_record.lock();
                            return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 寻位计算结果出错");
                            m_mcs->main_record.push_back(return_msg);
                            main_record.unlock();
                            line=n;
                            return 1;
                        }
                        m_mcs->project->projecr_robpos_trace[robpos_trace_num].robotpos=scan_trace[0].robotpos;
                        m_mcs->project->projecr_robpos_trace[robpos_trace_num].robotpos.X=weld_trace[0].X;
                        m_mcs->project->projecr_robpos_trace[robpos_trace_num].robotpos.Y=weld_trace[0].Y;
                        m_mcs->project->projecr_robpos_trace[robpos_trace_num].robotpos.Z=weld_trace[0].Z;
                        m_mcs->project->projecr_robpos_trace[robpos_trace_num].nEn=true;
                        unfindnum=0;    //统计清零
                    }
                    m_mcs->cam->sop_cam[0].b_updatacloud_finish=false;
                }
            }
        #ifdef USE_MYROBOT_CONTROL
            m_mcs->robotcontrol->clear_movepoint_buffer();
        #endif
            if(b_find==false)//没有找到终点
            {
                main_record.lock();
                return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 寻位点找不到");
                m_mcs->main_record.push_back(return_msg);
                main_record.unlock();
                line=n;
                return 1;
            }
            /*********************/
            //掉电保存
            savetemp_pos(m_mcs->project->projecr_robpos_trace[robpos_trace_num]);
        }

        else if(key==CMD_GETPOS_KEY)//获取扫描的焊缝坐标指令
        {
            bool b_find=false;//成功找到
            QString name=cmd.cmd_getpos_name;//获取到要保存扫描的点名字
            int time=cmd.cmd_getpos_time;//获取到等待扫描结果的时间
            std::vector<float> add=cmd.cmd_getpos_add;
            int robpos_trace_num;//获取要储存的扫描点下标
            for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
            {
                if(name==m_mcs->project->projecr_robpos_trace[n].name)
                {
                    robpos_trace_num=n;//找到要储存的扫描点下标
                    break;
                }
            }
            //清空检测状态
            m_mcs->cam->sop_cam[0].b_updatacloud_finish=false;
            m_mcs->cam->sop_cam[0].b_ros_lineEn=false;
            //这里添加扫描数据
            QElapsedTimer qtime;
            qtime.start();
            while(!qtime.hasExpired(time))
            {
                if(m_mcs->cam->sop_cam[0].b_updatacloud_finish==true)//有中断数据
                {
                    if(m_mcs->cam->sop_cam[0].b_ros_lineEn==true)//检测有正确结果
                    {
                        std::vector<Scan_trace_line> scan_trace(1);
                        std::vector<RobPos> weld_trace;
                        scan_trace[0].robottime=m_mcs->rob->robtime;
                        scan_trace[0].ros_line=*(m_mcs->cam->sop_cam[0].ros_line);
                    #if OPEN_TIMESTAMP==1
                        scan_trace[0].robotpos.X=scan_trace[0].ros_line.robpos.posx;
                        scan_trace[0].robotpos.Y=scan_trace[0].ros_line.robpos.posy;
                        scan_trace[0].robotpos.Z=scan_trace[0].ros_line.robpos.posz;
                        scan_trace[0].robotpos.RX=scan_trace[0].ros_line.robpos.posrx;
                        scan_trace[0].robotpos.RY=scan_trace[0].ros_line.robpos.posry;
                        scan_trace[0].robotpos.RZ=scan_trace[0].ros_line.robpos.posrz;
                        scan_trace[0].robotpos.out_1=scan_trace[0].ros_line.robpos.posout1;
                        scan_trace[0].robotpos.out_2=scan_trace[0].ros_line.robpos.posout2;
                        scan_trace[0].robotpos.out_3=scan_trace[0].ros_line.robpos.posout3;
                        scan_trace[0].robotpos.nEn=1;
                    #else
                        scan_trace[0].robotpos=m_mcs->rob->TCPpos;
                    #endif
                        if(false==m_mcs->synchronous->Scantrace_to_Weldtrace(scan_trace,weld_trace))
                        {
                            main_record.lock();
                            return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 扫描计算结果出错");
                            m_mcs->main_record.push_back(return_msg);
                            main_record.unlock();
                            line=n;
                            return 1;
                        }
                        m_mcs->project->projecr_robpos_trace[robpos_trace_num].robotpos=scan_trace[0].robotpos;
                        m_mcs->project->projecr_robpos_trace[robpos_trace_num].robotpos.X=weld_trace[0].X+add[0];
                        m_mcs->project->projecr_robpos_trace[robpos_trace_num].robotpos.Y=weld_trace[0].Y+add[1];
                        m_mcs->project->projecr_robpos_trace[robpos_trace_num].robotpos.Z=weld_trace[0].Z+add[2];
                        m_mcs->project->projecr_robpos_trace[robpos_trace_num].nEn=true;
                        b_find=true;
                    }
                    m_mcs->cam->sop_cam[0].b_updatacloud_finish=false;
                }
                if(b_find==true)//成功找到了点
                {
                    break;
                }
            }
            if(b_find==false)//没有找到点
            {
                main_record.lock();
                return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 扫描点找不到");
                m_mcs->main_record.push_back(return_msg);
                main_record.unlock();
                line=n;
                return 1;
            }
            /*********************/
            //掉电保存
            savetemp_pos(m_mcs->project->projecr_robpos_trace[robpos_trace_num]);
        }
        else if(key==CMD_SCAN_KEY)//采集指令
        {    
            float speed=cmd.cmd_scan_speed;//获取到的扫描速度
            int tcp=cmd.cmd_scan_tcp;//获取到扫描TCP
            Robmovemodel movemod=cmd.cmd_scan_movemod;//获取到的扫描模式
            QString name=cmd.cmd_scan_name;//获取到的扫描名字
            int scan_trace_num;//要储存的轨道数据下标
            QString change=cmd.cmd_scan_change;
            int matrix4d_trace_num;
            Eigen::Matrix3d R;          //旋转矩阵
            Eigen::Vector3d T;          //平移矩阵(零点坐标)
            Eigen::Matrix3d R1;          //旋转矩阵
            Eigen::Vector3d T1;          //平移矩阵(零点坐标)
            if(!change.isEmpty())//需要变换矩阵
            {
                for(int n=0;n<m_mcs->project->projecr_coord_matrix4d.size();n++)
                {
                    if(change==m_mcs->project->projecr_coord_matrix4d[n].name)
                    {
                        matrix4d_trace_num=n;//找到变换矩阵下标
                        break;
                    }
                }
                if(m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].nEn!=true)
                {
                    //矩阵无效
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+change+QString::fromLocal8Bit("矩阵没有获取到有效值");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    line=n;
                    return 1;
                }
                R=m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].R;
                T=m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].T;
                R1=m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].R1;
                T1=m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].T1;
            }

            for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
            {
                if(name==m_mcs->project->project_scan_trace[n].name)
                {
                    scan_trace_num=n;//找到要储存的扫描轨迹下标
                    break;
                }
            }
            switch(movemod)
            {
                case MOVEL:
                case MOVEJ:
                {
                    RobPos pos=cmd.cmd_scan_pos;//获取到扫描终点坐标
                    if(!change.isEmpty())//需要变换矩阵
                    {
                        Eigen::Vector3d tempin,tempout;
                        tempin.x()=pos.X;
                        tempin.y()=pos.Y;
                        tempin.z()=pos.Z;
                        tempout=CCoordChange::_1point2point(tempin,R1,T1);
                        tempout=CCoordChange::point2point(tempout,R,T);
                        pos.X=tempout.x();
                        pos.Y=tempout.y();
                        pos.Z=tempout.z();
                        /************/
                        //姿态自适应变化
                        std::array<double,3> posture={pos.RX,pos.RY,pos.RZ};
                        std::array<double,3> posture_map=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R1,posture);
                        std::array<double,3> posture_out=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R,posture_map);
                        pos.RX=posture_out[0];
                        pos.RY=posture_out[1];
                        pos.RZ=posture_out[2];
                        /************/
                    }
                    cmd_move(pos,movemod,speed,tcp);
                }
                break;
                case MOVEC:
                {
                    RobPos pos1=cmd.cmd_scan_pos1;//获取到移动坐标
                    RobPos pos2=cmd.cmd_scan_pos2;//获取到移动坐标
                    RobPos pos3=cmd.cmd_scan_pos3;//获取到移动坐标
                    if(!change.isEmpty())//需要变换矩阵
                    {
                        Eigen::Vector3d tempin,tempout;
                        tempin.x()=pos1.X;
                        tempin.y()=pos1.Y;
                        tempin.z()=pos1.Z;
                        tempout=CCoordChange::_1point2point(tempin,R1,T1);
                        tempout=CCoordChange::point2point(tempout,R,T);
                        pos1.X=tempout.x();
                        pos1.Y=tempout.y();
                        pos1.Z=tempout.z();
                        /************/
                        //姿态自适应变化
                        std::array<double,3> posture1={pos1.RX,pos1.RY,pos1.RZ};
                        std::array<double,3> posture_map1=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R1,posture1);
                        std::array<double,3> posture_out1=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R,posture_map1);
                        pos1.RX=posture_out1[0];
                        pos1.RY=posture_out1[1];
                        pos1.RZ=posture_out1[2];
                        /************/
                        tempin.x()=pos2.X;
                        tempin.y()=pos2.Y;
                        tempin.z()=pos2.Z;
                        tempout=CCoordChange::_1point2point(tempin,R1,T1);
                        tempout=CCoordChange::point2point(tempout,R,T);
                        pos2.X=tempout.x();
                        pos2.Y=tempout.y();
                        pos2.Z=tempout.z();
                        /************/
                        //姿态自适应变化
                        std::array<double,3> posture2={pos2.RX,pos2.RY,pos2.RZ};
                        std::array<double,3> posture_map2=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R1,posture2);
                        std::array<double,3> posture_out2=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R,posture_map2);
                        pos2.RX=posture_out2[0];
                        pos2.RY=posture_out2[1];
                        pos2.RZ=posture_out2[2];
                        /************/
                        tempin.x()=pos3.X;
                        tempin.y()=pos3.Y;
                        tempin.z()=pos3.Z;
                        tempout=CCoordChange::_1point2point(tempin,R1,T1);
                        tempout=CCoordChange::point2point(tempout,R,T);
                        pos3.X=tempout.x();
                        pos3.Y=tempout.y();
                        pos3.Z=tempout.z();
                        /************/
                        //姿态自适应变化
                        std::array<double,3> posture3={pos3.RX,pos3.RY,pos3.RZ};
                        std::array<double,3> posture_map3=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R1,posture3);
                        std::array<double,3> posture_out3=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R,posture_map3);
                        pos3.RX=posture_out3[0];
                        pos3.RY=posture_out3[1];
                        pos3.RZ=posture_out3[2];
                        /************/
                    }
                #ifdef USE_MYMOVEC_CONTROL
                    CWeldTarject tarjectMath;
                    std::vector<RobPos> interpolatPos;
                    if(false==tarjectMath.pos_circle(m_mcs->rob->cal_posture_model,pos1,pos2,pos3,interpolatPos,ROBOT_POSE_MOVEC_STEP,16,speed))
                    {
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 圆弧三点轨迹拟合出错");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        line=n;
                        return 1;
                    }
                    for(int n=0;n<interpolatPos.size();n++)
                    {
                        cmd_move(interpolatPos[n],MOVEL,speed,tcp);
                    }
                #else
                    cmd_move(pos1,MOVEL,speed,tcp);
                    cmd_moveC(pos2,pos3,MOVEC,speed,tcp);
                #endif
                }
                break;
            }
            usleep(ROB_WORK_DELAY);
            while(m_mcs->rob->robot_state!=ROBOT_STATE_IDLE)//等待扫描到位
            {
                if(b_cmdlist_build==false)     //停止
                {
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("手动停止进程");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    paused_key=key;
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
                        res.robottime=m_mcs->rob->robtime;
                        res.ros_line=*(m_mcs->cam->sop_cam[0].ros_line);
                    #if OPEN_TIMESTAMP==1
                        res.robotpos.X=res.ros_line.robpos.posx;
                        res.robotpos.Y=res.ros_line.robpos.posy;
                        res.robotpos.Z=res.ros_line.robpos.posz;
                        res.robotpos.RX=res.ros_line.robpos.posrx;
                        res.robotpos.RY=res.ros_line.robpos.posry;
                        res.robotpos.RZ=res.ros_line.robpos.posrz;
                        res.robotpos.out_1=res.ros_line.robpos.posout1;
                        res.robotpos.out_2=res.ros_line.robpos.posout2;
                        res.robotpos.out_3=res.ros_line.robpos.posout3;
                        res.robotpos.nEn=1;
                    #else
                        res.robotpos=m_mcs->rob->TCPpos;
                    #endif
                        m_mcs->project->project_scan_trace[scan_trace_num].point.push_back(res);
                    }
                    m_mcs->cam->sop_cam[0].b_updatacloud_finish=false;
                }
                //开始采集检测数据
                usleep(0);
            }
        #ifdef USE_MYROBOT_CONTROL
            m_mcs->robotcontrol->clear_movepoint_buffer();
        #endif
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
            /*********************/
            //掉电保存
            savetemp_scan(m_mcs->project->project_scan_trace[scan_trace_num]);
        }
        else if(key==CMD_SSCAN_KEY)//点位采集指令
        {
            float speed=cmd.cmd_sscan_speed;//获取到的扫描速度
            int tcp=cmd.cmd_sscan_tcp;//获取到扫描TCP
            Robmovemodel movemod=cmd.cmd_sscan_movemod;//获取到的扫描模式
            QString name=cmd.cmd_sscan_name;//获取到的扫描名字
            int scan_trace_num;//要储存的轨道数据下标
            QString change=cmd.cmd_sscan_change;
            QString s_pos=cmd.cmd_sscan_pos;
            QString s_pos1=cmd.cmd_sscan_pos1;
            QString s_pos2=cmd.cmd_sscan_pos2;
            QString s_pos3=cmd.cmd_sscan_pos3;
            int matrix4d_trace_num;
            int point_trace_num;//找到寻位点下标
            int point1_trace_num;//找到寻位点下标
            int point2_trace_num;//找到寻位点下标
            int point3_trace_num;//找到寻位点下标
            Eigen::Matrix3d R;          //旋转矩阵
            Eigen::Vector3d T;          //平移矩阵(零点坐标)
            Eigen::Matrix3d R1;          //旋转矩阵
            Eigen::Vector3d T1;          //平移矩阵(零点坐标)
            if(!change.isEmpty())//需要变换矩阵
            {
                for(int n=0;n<m_mcs->project->projecr_coord_matrix4d.size();n++)
                {
                    if(change==m_mcs->project->projecr_coord_matrix4d[n].name)
                    {
                        matrix4d_trace_num=n;//找到变换矩阵下标
                        break;
                    }
                }
                if(m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].nEn!=true)
                {
                    //矩阵无效
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+change+QString::fromLocal8Bit("矩阵没有获取到有效值");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    line=n;
                    return 1;
                }
                R=m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].R;
                T=m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].T;
                R1=m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].R1;
                T1=m_mcs->project->projecr_coord_matrix4d[matrix4d_trace_num].T1;
            }

            for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
            {
                if(name==m_mcs->project->project_scan_trace[n].name)
                {
                    scan_trace_num=n;//找到要储存的扫描轨迹下标
                    break;
                }
            }
            switch(movemod)
            {
                case MOVEL:
                case MOVEJ:
                {
                    for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
                    {
                        if(s_pos==m_mcs->project->projecr_robpos_trace[n].name)
                        {
                            point_trace_num=n;//找到要储存的寻位点下标
                            break;
                        }
                    }
                    if(m_mcs->project->projecr_robpos_trace[point_trace_num].nEn!=true)
                    {
                        //点无效
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+s_pos+QString::fromLocal8Bit("点没有获取到坐标值");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        line=n;
                        return 1;
                    }
                    RobPos pos=m_mcs->project->projecr_robpos_trace[point_trace_num].robotpos;//获取到移动坐标
                    if(!change.isEmpty())//需要变换矩阵
                    {
                        Eigen::Vector3d tempin,tempout;
                        tempin.x()=pos.X;
                        tempin.y()=pos.Y;
                        tempin.z()=pos.Z;
                        tempout=CCoordChange::_1point2point(tempin,R1,T1);
                        tempout=CCoordChange::point2point(tempout,R,T);
                        pos.X=tempout.x();
                        pos.Y=tempout.y();
                        pos.Z=tempout.z();
                        /************/
                        //姿态自适应变化
                        std::array<double,3> posture={pos.RX,pos.RY,pos.RZ};
                        std::array<double,3> posture_map=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R1,posture);
                        std::array<double,3> posture_out=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R,posture_map);
                        pos.RX=posture_out[0];
                        pos.RY=posture_out[1];
                        pos.RZ=posture_out[2];
                        /************/
                    }
                    cmd_move(pos,movemod,speed,tcp);
                }
                break;
                case MOVEC:
                {
                    for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
                    {
                        if(s_pos1==m_mcs->project->projecr_robpos_trace[n].name)
                        {
                            point1_trace_num=n;//找到要储存的寻位点下标
                            break;
                        }
                    }
                    if(m_mcs->project->projecr_robpos_trace[point1_trace_num].nEn!=true)
                    {
                        //点无效
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+s_pos1+QString::fromLocal8Bit("点没有获取到坐标值");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        line=n;
                        return 1;
                    }
                    for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
                    {
                        if(s_pos2==m_mcs->project->projecr_robpos_trace[n].name)
                        {
                            point2_trace_num=n;//找到要储存的寻位点下标
                            break;
                        }
                    }
                    if(m_mcs->project->projecr_robpos_trace[point2_trace_num].nEn!=true)
                    {
                        //点无效
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+s_pos2+QString::fromLocal8Bit("点没有获取到坐标值");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        line=n;
                        return 1;
                    }
                    for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
                    {
                        if(s_pos3==m_mcs->project->projecr_robpos_trace[n].name)
                        {
                            point3_trace_num=n;//找到要储存的寻位点下标
                            break;
                        }
                    }
                    if(m_mcs->project->projecr_robpos_trace[point3_trace_num].nEn!=true)
                    {
                        //点无效
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+": "+s_pos3+QString::fromLocal8Bit("点没有获取到坐标值");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        line=n;
                        return 1;
                    }
                    RobPos pos1=m_mcs->project->projecr_robpos_trace[point1_trace_num].robotpos;//获取到移动坐标
                    RobPos pos2=m_mcs->project->projecr_robpos_trace[point2_trace_num].robotpos;//获取到移动坐标
                    RobPos pos3=m_mcs->project->projecr_robpos_trace[point3_trace_num].robotpos;//获取到移动坐标
                    if(!change.isEmpty())//需要变换矩阵
                    {
                        Eigen::Vector3d tempin,tempout;
                        tempin.x()=pos1.X;
                        tempin.y()=pos1.Y;
                        tempin.z()=pos1.Z;
                        tempout=CCoordChange::_1point2point(tempin,R1,T1);
                        tempout=CCoordChange::point2point(tempout,R,T);
                        pos1.X=tempout.x();
                        pos1.Y=tempout.y();
                        pos1.Z=tempout.z();
                        /************/
                        //姿态自适应变化
                        std::array<double,3> posture1={pos1.RX,pos1.RY,pos1.RZ};
                        std::array<double,3> posture_map1=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R1,posture1);
                        std::array<double,3> posture_out1=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R,posture_map1);
                        pos1.RX=posture_out1[0];
                        pos1.RY=posture_out1[1];
                        pos1.RZ=posture_out1[2];
                        /************/
                        tempin.x()=pos2.X;
                        tempin.y()=pos2.Y;
                        tempin.z()=pos2.Z;
                        tempout=CCoordChange::_1point2point(tempin,R1,T1);
                        tempout=CCoordChange::point2point(tempout,R,T);
                        pos2.X=tempout.x();
                        pos2.Y=tempout.y();
                        pos2.Z=tempout.z();
                        /************/
                        //姿态自适应变化
                        std::array<double,3> posture2={pos2.RX,pos2.RY,pos2.RZ};
                        std::array<double,3> posture_map2=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R1,posture2);
                        std::array<double,3> posture_out2=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R,posture_map2);
                        pos2.RX=posture_out2[0];
                        pos2.RY=posture_out2[1];
                        pos2.RZ=posture_out2[2];
                        /************/
                        tempin.x()=pos3.X;
                        tempin.y()=pos3.Y;
                        tempin.z()=pos3.Z;
                        tempout=CCoordChange::_1point2point(tempin,R1,T1);
                        tempout=CCoordChange::point2point(tempout,R,T);
                        pos3.X=tempout.x();
                        pos3.Y=tempout.y();
                        pos3.Z=tempout.z();
                        /************/
                        //姿态自适应变化
                        std::array<double,3> posture3={pos3.RX,pos3.RY,pos3.RZ};
                        std::array<double,3> posture_map3=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R1,posture3);
                        std::array<double,3> posture_out3=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R,posture_map3);
                        pos3.RX=posture_out3[0];
                        pos3.RY=posture_out3[1];
                        pos3.RZ=posture_out3[2];
                        /************/
                    }
                #ifdef USE_MYMOVEC_CONTROL
                    CWeldTarject tarjectMath;
                    std::vector<RobPos> interpolatPos;
                    if(false==tarjectMath.pos_circle(m_mcs->rob->cal_posture_model,pos1,pos2,pos3,interpolatPos,ROBOT_POSE_MOVEC_STEP,16,speed))
                    {
                        main_record.lock();
                        return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 圆弧三点轨迹拟合出错");
                        m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        line=n;
                        return 1;
                    }
                    for(int n=0;n<interpolatPos.size();n++)
                    {
                        cmd_move(interpolatPos[n],MOVEL,speed,tcp);
                    }
                #else
                    cmd_move(pos1,MOVEL,speed,tcp);
                    cmd_moveC(pos2,pos3,MOVEC,speed,tcp);
                #endif
                }
                break;
            }
            usleep(ROB_WORK_DELAY);
            while(m_mcs->rob->robot_state!=ROBOT_STATE_IDLE)//等待扫描到位
            {
                if(b_cmdlist_build==false)     //停止
                {
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("手动停止进程");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    paused_key=key;
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
                        res.robottime=m_mcs->rob->robtime;
                        res.ros_line=*(m_mcs->cam->sop_cam[0].ros_line);
                    #if OPEN_TIMESTAMP==1
                        res.robotpos.X=res.ros_line.robpos.posx;
                        res.robotpos.Y=res.ros_line.robpos.posy;
                        res.robotpos.Z=res.ros_line.robpos.posz;
                        res.robotpos.RX=res.ros_line.robpos.posrx;
                        res.robotpos.RY=res.ros_line.robpos.posry;
                        res.robotpos.RZ=res.ros_line.robpos.posrz;
                        res.robotpos.out_1=res.ros_line.robpos.posout1;
                        res.robotpos.out_2=res.ros_line.robpos.posout2;
                        res.robotpos.out_3=res.ros_line.robpos.posout3;
                        res.robotpos.nEn=1;
                    #else
                        res.robotpos=m_mcs->rob->TCPpos;
                    #endif
                        m_mcs->project->project_scan_trace[scan_trace_num].point.push_back(res);
                    }
                    m_mcs->cam->sop_cam[0].b_updatacloud_finish=false;
                }
                //开始采集检测数据
                usleep(0);
            }
        #ifdef USE_MYROBOT_CONTROL
            m_mcs->robotcontrol->clear_movepoint_buffer();
        #endif
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
            /*********************/
            //掉电保存
            savetemp_scan(m_mcs->project->project_scan_trace[scan_trace_num]);
        }



        else if(key==CMD_TRACING_KEY)
        {
            QString name=cmd.cmd_tracing_name;
            int tcp=cmd.cmd_tracing_tcp;
            int weld_tracing_num;
            int time=cmd.cmd_tracing_time;
            for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
            {
                if(name==m_mcs->project->project_interweld_trace[n].name)
                {
                    weld_tracing_num=n;//找到焊接轨道下标
                    break;
                }
            }
            //这里添加移动指令

            for(int n=0;n<m_mcs->project->project_interweld_trace[weld_tracing_num].trace.size();n++)
            {
                Weld_trace_onec trace=m_mcs->project->project_interweld_trace[weld_tracing_num].trace[n];
                for(int m=0;m<trace.point.size();m++)
                {
                    RobPos pos=trace.point[m];
                    if(m==0)
                    {
                        cmd_move(pos,MOVEL,trace.speed,tcp);//这里考虑暂停如何加
                    }
                    else
                    {
                        if(trace.Sample==false)
                            cmd_move(pos,MOVEL,trace.speed,tcp);//稀疏轨迹
                        else
                        {
                            cmd_move(pos,MOVEP,trace.speed,tcp);//密集轨迹
                            if(b_cmdlist_build==false)     //停止
                            {
                                main_record.lock();
                                return_msg=QString::fromLocal8Bit("手动停止进程");
                                m_mcs->main_record.push_back(return_msg);
                                main_record.unlock();
                                paused_key=key;
                                cmd_lock(1);
                                line=n;
                                return 1;
                            }
                        }
                    }
                    usleep(time*1000);
                }
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
                    paused_key=key;
                    cmd_lock(1);
                    line=n;
                    return 1;
                }
                sleep(0);
            //  usleep(ROB_WORK_DELAY_STEP);
            }
        #ifdef USE_MYROBOT_CONTROL
            m_mcs->robotcontrol->clear_movepoint_buffer();
        #endif
        }

        else if(key==CMD_GOWELD_KEY)
        {
            QString name=cmd.cmd_goweld_name;
            int tcp=cmd.cmd_goweld_tcp;
            float speed=cmd.cmd_goweld_speed;
            int weld_tracing_num;
            for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
            {
                if(name==m_mcs->project->project_interweld_trace[n].name)
                {
                    weld_tracing_num=n;//找到焊接轨道下标
                    break;
                }
            }
            //这里添加移动指令
            if(m_mcs->project->project_interweld_trace[weld_tracing_num].trace.size()>0)
            {
                if(m_mcs->project->project_interweld_trace[weld_tracing_num].trace[0].point.size()>0)
                {
                    RobPos pos=m_mcs->project->project_interweld_trace[weld_tracing_num].trace[0].point[0];
                    cmd_move(pos,MOVEL,speed,tcp);//这里考虑暂停如何加
                }
                else
                {
                    main_record.lock();
                    return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 焊缝搜索异常");
                    m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    line=n;
                    return 1;
                }
            }
            else
            {
                main_record.lock();
                return_msg=QString::fromLocal8Bit("Line")+QString::number(n)+QString::fromLocal8Bit(": 焊缝搜索异常");
                m_mcs->main_record.push_back(return_msg);
                main_record.unlock();
                line=n;
                return 1;
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
                    paused_key=key;
                    cmd_lock(1);
                    line=n;
                    return 1;
                }
                sleep(0);
            //  usleep(ROB_WORK_DELAY_STEP);
            }
        #ifdef USE_MYROBOT_CONTROL
            m_mcs->robotcontrol->clear_movepoint_buffer();
        #endif
        }
        else if(key==CMD_AOUT_KEY)
        {
            std::vector<float> aout=cmd.cmd_aout_output;
            cmd_aout(aout);
        }
        else if(key==CMD_GETTCPPOS_KEY)
        {
            QString name=cmd.cmd_gettcppos_name;//获取到要保存tcp的点名字
            std::vector<float> add=cmd.cmd_gettcppos_add;
            int robpos_trace_num;//获取要储存的tcp点下标
            for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
            {
                if(name==m_mcs->project->projecr_robpos_trace[n].name)
                {
                    robpos_trace_num=n;//找到要储存的扫描点下标
                    break;
                }
            }
            RobPos pos=m_mcs->rob->TCPpos;
            pos.X=pos.X+add[0];
            pos.Y=pos.Y+add[1];
            pos.Z=pos.Z+add[2];
            m_mcs->project->projecr_robpos_trace[robpos_trace_num].robotpos=pos;
            m_mcs->project->projecr_robpos_trace[robpos_trace_num].nEn=true;
            /*********************/
            //掉电保存
            savetemp_pos(m_mcs->project->projecr_robpos_trace[robpos_trace_num]);
        }
        else if(key==CMD_GOTO_KEY)
        {
            int goto_line=cmd.cmd_goto_line;
            n=goto_line;
        }
        else if(key==CMD_STOP_KEY)//流程停止了
        {
            main_record.lock();
            return_msg=QString::fromLocal8Bit("代码中停止进程");
            m_mcs->main_record.push_back(return_msg);
            main_record.unlock();
            line=n;
            return 1;
        }
        else//计算类
        {
            if(0!=slopbuild(m_mcs->project->project_cmdlist[n],n,return_msg))
            {
                main_record.lock();
                m_mcs->main_record.push_back(return_msg);
                main_record.unlock();
                line=n;
                return 1;
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
    cmd_elec(STATIC);
    main_record.lock();
    return_msg=QString::fromLocal8Bit("指令执行结束");
    m_mcs->main_record.push_back(return_msg);
    main_record.unlock();
    line=m_mcs->project->project_cmdlist.size();
#ifdef USE_MYROBOT_CONTROL
    m_mcs->robotcontrol->clear_movepoint_buffer();
    m_mcs->robotcontrol->pause_movepoint_buffer.clear();
#endif
    return 0;
}

void toSendbuffer::cmdlist_stopbuild()
{
    b_cmdlist_build=false;
    u16data_elec_work=0;
    cmd_elec(STATIC);
}

void toSendbuffer::cmdlist_pausedbuild()
{
    b_cmdlist_build=false;
    cmd_elec(STATIC);
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

void toSendbuffer::cmd_settcp(int tcp)
{
    send_group_robot.lock();
    sent_info_robot sendrob;
    sendrob.addr=ROB_TCP_NUM_REG_ADD;
    sendrob.ctx=m_mcs->rob->ctx_posget;
    sendrob.data.resize(1);
    sendrob.data[0]=tcp;
    m_mcs->rob->b_send_group_robot=false;
    m_mcs->rob->send_group_robot.push_back(sendrob);
    m_mcs->rob->ctx_robot_dosomeing=DO_WRITE_TASK;
    send_group_robot.unlock();
}

void toSendbuffer::cmd_speed(float speed)
{
    send_group_robot.lock();
    sent_info_robot sendrob;
    sendrob.addr=ROB_MOVESPEED_FH_REG_ADD;
    sendrob.ctx=m_mcs->rob->ctx_posget;
    sendrob.data.resize(2);
    sendrob.data[0]=*((u_int16_t*)&speed);
    sendrob.data[1]=*((u_int16_t*)&speed+1);
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
    sendrob.data.resize(41);
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

    sendrob.data[17]=0;
    sendrob.data[18]=0;
    sendrob.data[19]=0;
    sendrob.data[20]=0;
    sendrob.data[21]=0;
    sendrob.data[22]=0;
    sendrob.data[23]=0;
    sendrob.data[24]=0;
    sendrob.data[25]=0;
    sendrob.data[26]=0;
    sendrob.data[27]=0;
    sendrob.data[28]=0;

    sendrob.data[29]=*((u_int16_t*)&pos.out_1);
    sendrob.data[30]=*((u_int16_t*)&pos.out_1+1);
    sendrob.data[31]=*((u_int16_t*)&pos.out_2);
    sendrob.data[32]=*((u_int16_t*)&pos.out_2+1);
    sendrob.data[33]=*((u_int16_t*)&pos.out_3);
    sendrob.data[34]=*((u_int16_t*)&pos.out_3+1);

    sendrob.data[35]=0;
    sendrob.data[36]=0;
    sendrob.data[37]=0;
    sendrob.data[38]=0;
    sendrob.data[39]=0;
    sendrob.data[40]=0;

    m_mcs->rob->b_send_group_robot=false; 
    m_mcs->rob->send_group_robot.push_back(sendrob);
    m_mcs->rob->ctx_robot_dosomeing=DO_WRITE_TASK;
    send_group_robot.unlock();
}

void toSendbuffer::cmd_moveC(RobPos pos1,RobPos pos2,Robmovemodel movemodel,float speed,int tcp)
{
    send_group_robot.lock();
    sent_info_robot sendrob;
    sendrob.addr=ROB_TCP_NUM_REG_ADD;
    sendrob.ctx=m_mcs->rob->ctx_posget;
    sendrob.data.resize(41);
    sendrob.data[0]=tcp;
    sendrob.data[1]=movemodel;
    sendrob.data[2]=u16data_elec_work;
    sendrob.data[3]=*((u_int16_t*)&speed);
    sendrob.data[4]=*((u_int16_t*)&speed+1);
    sendrob.data[5]=*((u_int16_t*)&pos1.X);
    sendrob.data[6]=*((u_int16_t*)&pos1.X+1);
    sendrob.data[7]=*((u_int16_t*)&pos1.Y);
    sendrob.data[8]=*((u_int16_t*)&pos1.Y+1);
    sendrob.data[9]=*((u_int16_t*)&pos1.Z);
    sendrob.data[10]=*((u_int16_t*)&pos1.Z+1);
    sendrob.data[11]=*((u_int16_t*)&pos1.RX);
    sendrob.data[12]=*((u_int16_t*)&pos1.RX+1);
    sendrob.data[13]=*((u_int16_t*)&pos1.RY);
    sendrob.data[14]=*((u_int16_t*)&pos1.RY+1);
    sendrob.data[15]=*((u_int16_t*)&pos1.RZ);
    sendrob.data[16]=*((u_int16_t*)&pos1.RZ+1);
    sendrob.data[17]=*((u_int16_t*)&pos2.X);
    sendrob.data[18]=*((u_int16_t*)&pos2.X+1);
    sendrob.data[19]=*((u_int16_t*)&pos2.Y);
    sendrob.data[20]=*((u_int16_t*)&pos2.Y+1);
    sendrob.data[21]=*((u_int16_t*)&pos2.Z);
    sendrob.data[22]=*((u_int16_t*)&pos2.Z+1);
    sendrob.data[23]=*((u_int16_t*)&pos2.RX);
    sendrob.data[24]=*((u_int16_t*)&pos2.RX+1);
    sendrob.data[25]=*((u_int16_t*)&pos2.RY);
    sendrob.data[26]=*((u_int16_t*)&pos2.RY+1);
    sendrob.data[27]=*((u_int16_t*)&pos2.RZ);
    sendrob.data[28]=*((u_int16_t*)&pos2.RZ+1);

    sendrob.data[29]=*((u_int16_t*)&pos1.out_1);
    sendrob.data[30]=*((u_int16_t*)&pos1.out_1+1);
    sendrob.data[31]=*((u_int16_t*)&pos1.out_2);
    sendrob.data[32]=*((u_int16_t*)&pos1.out_2+1);
    sendrob.data[33]=*((u_int16_t*)&pos1.out_3);
    sendrob.data[34]=*((u_int16_t*)&pos1.out_3+1);

    sendrob.data[35]=*((u_int16_t*)&pos2.out_1);
    sendrob.data[36]=*((u_int16_t*)&pos2.out_1+1);
    sendrob.data[37]=*((u_int16_t*)&pos2.out_2);
    sendrob.data[38]=*((u_int16_t*)&pos2.out_2+1);
    sendrob.data[39]=*((u_int16_t*)&pos2.out_3);
    sendrob.data[40]=*((u_int16_t*)&pos2.out_3+1);

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

void toSendbuffer::cmd_elec(Weldworkmodel work)
{
    send_group_robot.lock();
    sent_info_robot sendrob;
    sendrob.addr=ROB_MOVEFIER_REG_ADD;
    sendrob.ctx=m_mcs->rob->ctx_posget;
    sendrob.data.resize(1);
    sendrob.data[0]=work;
    m_mcs->rob->b_send_group_robot=false;
    m_mcs->rob->send_group_robot.push_back(sendrob);
    m_mcs->rob->ctx_robot_dosomeing=DO_WRITE_TASK;
    m_mcs->rob->weld_state=(Weldworkmodel)work;
    u16data_elec_work=work;
    send_group_robot.unlock();
}

void toSendbuffer::cmd_elec(float eled,float elev,Alternatingcurrent elem,Weldworkmodel work)
{
    send_group_robot.lock();
    sent_info_robot sendrob;
    sendrob.addr=ROB_WELD_CURRENT_FH_REG_ADD;
    sendrob.ctx=m_mcs->rob->ctx_posget;
    sendrob.data.resize(5);
    sendrob.data[0]=*((u_int16_t*)&eled);
    sendrob.data[1]=*((u_int16_t*)&eled+1);
    sendrob.data[2]=elem;
    sendrob.data[3]=*((u_int16_t*)&elev);
    sendrob.data[4]=*((u_int16_t*)&elev+1);
    m_mcs->rob->b_send_group_robot=false;
    m_mcs->rob->send_group_robot.push_back(sendrob);
    u16data_elec_work=work;
    sendrob.addr=ROB_MOVEFIER_REG_ADD;
    sendrob.data.resize(1);
    sendrob.data[0]=work;
    m_mcs->rob->send_group_robot.push_back(sendrob);
    m_mcs->rob->ctx_robot_dosomeing=DO_WRITE_TASK;
    m_mcs->rob->weld_state=(Weldworkmodel)work;
    m_mcs->rob->weld_eled=eled;
    m_mcs->rob->weld_elem=elem;
    m_mcs->rob->weld_elev=elev;
    send_group_robot.unlock();
}

void toSendbuffer::cmd_ioout(std::vector<int> io)
{
    send_group_robot.lock();
    sent_info_robot sendrob;
    sendrob.addr=ROB_IO_OUTPUT1_REG_ADD;
    sendrob.ctx=m_mcs->rob->ctx_posget;
    sendrob.data.resize(io.size());
    for(int n=0;n<io.size();n++)
    {
        sendrob.data[n]=io[n];
    }
    m_mcs->rob->b_send_group_robot=false;
    m_mcs->rob->send_group_robot.push_back(sendrob);
    m_mcs->rob->ctx_robot_dosomeing=DO_WRITE_TASK;
    send_group_robot.unlock();
}

void toSendbuffer::cmd_aout(std::vector<float> a)
{
    send_group_robot.lock();
    sent_info_robot sendrob;
    sendrob.addr=ROB_A_OUTPUT1_FH_REG_ADD;
    sendrob.ctx=m_mcs->rob->ctx_posget;
    sendrob.data.resize(a.size()*2);
    for(int n=0;n<a.size();n++)
    {
        sendrob.data[n*2]=*((u_int16_t*)&a[n]);
        sendrob.data[n*2+1]=*((u_int16_t*)&a[n]+1);
    }
    m_mcs->rob->b_send_group_robot=false;
    m_mcs->rob->send_group_robot.push_back(sendrob);
    m_mcs->rob->ctx_robot_dosomeing=DO_WRITE_TASK;
    send_group_robot.unlock();
}

void toSendbuffer::cmd_totalcontrol(bool nEn)
{
    send_group_robot.lock();
    sent_info_robot sendrob;
    sendrob.ctx=m_mcs->rob->ctx_posget;
    sendrob.addr=ROB_TOLTAL_CONTROL_REG_ADD;
    sendrob.data.resize(1);
    if(nEn==true)
        sendrob.data[0]=1;
    else
        sendrob.data[0]=0;
    m_mcs->rob->b_send_group_robot=false;
    m_mcs->rob->send_group_robot.push_back(sendrob);
    m_mcs->rob->ctx_robot_dosomeing=DO_WRITE_TASK;
    send_group_robot.unlock();
}

int toSendbuffer::loadlog_scan_json(QString filename,std::vector<Scan_trace_line> &trace)
{
    std::vector<Scan_trace_line> tra;
    QString format=".txt";
    QString filenamescan=filename+format;
    QJsonDocument jsonDoc;
    if(0!=Loadjsonfile((char*)filenamescan.toStdString().c_str(),jsonDoc))
    {
        return -1;
    }
    QJsonObject rootObj = jsonDoc.object();

    tra.resize(rootObj.size());
    for(int i=0;i<rootObj.size();i++)//遍历Key
    {
        QJsonObject singpos=rootObj[QString::number(i)].toObject();
        QJsonArray arrData=singpos["RobPos"].toArray();
        tra[i].robotpos.X=arrData[0].toDouble();
        tra[i].robotpos.Y=arrData[1].toDouble();
        tra[i].robotpos.Z=arrData[2].toDouble();
        tra[i].robotpos.RX=arrData[3].toDouble();
        tra[i].robotpos.RY=arrData[4].toDouble();
        tra[i].robotpos.RZ=arrData[5].toDouble();
        tra[i].robotpos.out_1=arrData[6].toInt();
        tra[i].robotpos.out_2=arrData[7].toInt();
        tra[i].robotpos.out_3=arrData[8].toInt();
        tra[i].robotpos.nEn=arrData[9].toBool();
        QJsonArray arrTime=singpos["robottime"].toArray();
        tra[i].robottime.hour=arrTime[0].toInt();
        tra[i].robottime.min=arrTime[1].toInt();
        tra[i].robottime.sec=arrTime[2].toInt();
        tra[i].robottime.msec=arrTime[3].toInt();
        QJsonObject ros_line=singpos["ros_line"].toObject();
        QJsonObject header=ros_line["header"].toObject();
        tra[i].ros_line.header.frame_id=header["frame_id"].toString().toStdString();
        tra[i].ros_line.header.stamp.sec=header["sec"].toInt();
        tra[i].ros_line.header.stamp.nanosec=header["nanosec"].toInt();
        QJsonArray lasertrackoutcloud=ros_line["lasertrackoutcloud"].toArray();
        tra[i].ros_line.lasertrackoutcloud.resize(lasertrackoutcloud.size()/4);
        for(int j=0;j<lasertrackoutcloud.size()/4;j++)
        {
            tra[i].ros_line.lasertrackoutcloud[j].u=lasertrackoutcloud[j*4].toInt();
            tra[i].ros_line.lasertrackoutcloud[j].v=lasertrackoutcloud[j*4+1].toInt();
            tra[i].ros_line.lasertrackoutcloud[j].x=lasertrackoutcloud[j*4+2].toDouble();
            tra[i].ros_line.lasertrackoutcloud[j].y=lasertrackoutcloud[j*4+3].toDouble();
        }
        QJsonArray targetpointoutcloud=ros_line["targetpointoutcloud"].toArray();
        tra[i].ros_line.targetpointoutcloud.resize(targetpointoutcloud.size()/5);
        for(int j=0;j<targetpointoutcloud.size()/5;j++)
        {
            tra[i].ros_line.targetpointoutcloud[j].u=targetpointoutcloud[j*5].toInt();
            tra[i].ros_line.targetpointoutcloud[j].v=targetpointoutcloud[j*5+1].toInt();
            tra[i].ros_line.targetpointoutcloud[j].x=targetpointoutcloud[j*5+2].toDouble();
            tra[i].ros_line.targetpointoutcloud[j].y=targetpointoutcloud[j*5+3].toDouble();
            tra[i].ros_line.targetpointoutcloud[j].name=targetpointoutcloud[j*5+4].toString().toStdString();
        }
        tra[i].ros_line.solderjoints=ros_line["solderjoints"].toBool();
    }
    trace=tra;
    return 0;
}

int toSendbuffer::savelog_scan_json(QString filename,std::vector<Scan_trace_line> trace)
{
    QString format=".txt";
    QString filenamescan=filename+format;
    QVariantHash arr;
    for (int i=0;i<trace.size();i++)
    {
        QVariantHash singpos;
        QVariantHash ros_line;
        QVariantHash header;
        QJsonArray arrData;
        QJsonArray arrTime;
        QJsonArray lasertrackoutcloud;
        QJsonArray targetpointoutcloud;
        arrData.append(trace[i].robotpos.X);
        arrData.append(trace[i].robotpos.Y);
        arrData.append(trace[i].robotpos.Z);
        arrData.append(trace[i].robotpos.RX);
        arrData.append(trace[i].robotpos.RY);
        arrData.append(trace[i].robotpos.RZ);
        arrData.append(trace[i].robotpos.out_1);
        arrData.append(trace[i].robotpos.out_2);
        arrData.append(trace[i].robotpos.out_3);
        arrData.append(trace[i].robotpos.nEn);
        singpos.insert("RobPos",arrData);
        arrTime.append(trace[i].robottime.hour);
        arrTime.append(trace[i].robottime.min);
        arrTime.append(trace[i].robottime.sec);
        arrTime.append(trace[i].robottime.msec);
        singpos.insert("robottime",arrTime);

        header.insert("frame_id",QString::fromStdString(trace[i].ros_line.header.frame_id));
        header.insert("sec",trace[i].ros_line.header.stamp.sec);
        header.insert("nanosec",trace[i].ros_line.header.stamp.nanosec);
        ros_line.insert("header",header);
        for(int j=0;j<trace[i].ros_line.lasertrackoutcloud.size();j++)
        {
            lasertrackoutcloud.append(trace[i].ros_line.lasertrackoutcloud[j].u);
            lasertrackoutcloud.append(trace[i].ros_line.lasertrackoutcloud[j].v);
            lasertrackoutcloud.append(trace[i].ros_line.lasertrackoutcloud[j].x);
            lasertrackoutcloud.append(trace[i].ros_line.lasertrackoutcloud[j].y);
        }
        ros_line.insert("lasertrackoutcloud",lasertrackoutcloud);
        for(int j=0;j<trace[i].ros_line.targetpointoutcloud.size();j++)
        {
            targetpointoutcloud.append(trace[i].ros_line.targetpointoutcloud[j].u);
            targetpointoutcloud.append(trace[i].ros_line.targetpointoutcloud[j].v);
            targetpointoutcloud.append(trace[i].ros_line.targetpointoutcloud[j].x);
            targetpointoutcloud.append(trace[i].ros_line.targetpointoutcloud[j].y);
            targetpointoutcloud.append(QString::fromStdString(trace[i].ros_line.targetpointoutcloud[j].name));
        }
        ros_line.insert("targetpointoutcloud",targetpointoutcloud);
        ros_line.insert("solderjoints",trace[i].ros_line.solderjoints);
        singpos.insert("ros_line",ros_line);
        arr.insert(QString::number(i),singpos);
    }
    Savejsonfile((char*)filenamescan.toStdString().c_str(),arr);
    return 0;
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
        msg="Line"+QString::number(n)+": "+pointdata+"\r\n";
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
        msg="Line"+QString::number(n)+": "+pointdata+"\r\n";
        fp2.write(msg.toStdString().c_str());
    }
    fp2.close();

    QString filenamepointlinecloud=filename+linecloud+format;
    QFile fp3(filenamepointlinecloud);
    if(!fp3.open(QIODevice::WriteOnly))
        return -1;

    for(int n=0;n<trace.size();n++)
    {
        msg="Line"+QString::number(n)+": \r\n";
        fp3.write(msg.toStdString().c_str());
        for(int m=0;m<trace[n].ros_line.lasertrackoutcloud.size();m++)
        {
            int u=trace[n].ros_line.lasertrackoutcloud[m].u;
            int v=trace[n].ros_line.lasertrackoutcloud[m].v;
            float Y=trace[n].ros_line.lasertrackoutcloud[m].x;
            float Z=trace[n].ros_line.lasertrackoutcloud[m].y;
            QString linedata="U="+QString::number(u)+",V="+QString::number(v)+",Y="+QString::number(Y,'f',ROBOT_POSE_DECIMAL_PLACE)+",Z="+QString::number(Z,'f',ROBOT_POSE_DECIMAL_PLACE)+"\r\n";
            fp3.write(linedata.toStdString().c_str());
        }
    }
    fp3.close();

    return 0;
}

int toSendbuffer::savelog_point(QString filename,RobPos point)
{
    QString msg;
    QString format=".txt";
    QString filenamepoint=filename+format;
    QFile fp(filenamepoint);
    if(!fp.open(QIODevice::WriteOnly))
        return -1;
    if(point.nEn==false)
    {
        msg="nEn=0\r\n";
    }
    else
    {
        msg="nEn=1\r\n";
    }
    fp.write(msg.toStdString().c_str());
    msg="pos("+QString::number(point.X,'f',ROBOT_POSE_DECIMAL_PLACE)+","+QString::number(point.Y,'f',ROBOT_POSE_DECIMAL_PLACE)+","+QString::number(point.Z,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
               QString::number(point.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+QString::number(point.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+QString::number(point.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
               QString::number(point.out_1)+","+QString::number(point.out_2)+","+QString::number(point.out_3)+")\r\n";
    fp.write(msg.toStdString().c_str());
    fp.close();
    return 0;
}

int toSendbuffer::loadlog_point(QString filename,RobPos &point)
{
    QString format=".txt";
    QString filenamepoint=filename+format;
    bool rc;
    RobPos pos;
    QFile fp(filenamepoint);
    if(!fp.open(QIODevice::ReadOnly))
        return 1;
    QString line;
    QTextStream in(&fp);  //用文件构造流
    line = in.readLine();//读取一行放到字符串里
    if(!line.isNull())
    {
        QString str = line;
        QStringList posgroup = str.split("=");
        if(posgroup.size()!=2)
        {
            fp.close();
            return -1;
        }
        if(posgroup[0]!="nEn")
        {
            fp.close();
            return -1;
        }
        int data=posgroup[1].toInt(&rc);
        if(rc==false)
        {
            fp.close();
            return -1;
        }
        if(data==0)
        {
            pos.nEn=0;
        }
        else
        {
            pos.nEn=1;
        }
        line = in.readLine();//读取一行放到字符串里
        if(!line.isNull())
        {
            QString str = line;
            QString smid;
            str.chop(str.size()-str.lastIndexOf(")"));
            smid=str.right(str.size()-str.indexOf("(")-1);
            QStringList posgroup = smid.split(",");
            if(posgroup.size()!=9)
            {
                fp.close();
                return -1;
            }
            bool ok;
            pos.X=posgroup[0].toFloat(&ok);
            if(ok==false)
            {
                fp.close();
                return -1;
            }
            pos.Y=posgroup[1].toFloat(&ok);
            if(ok==false)
            {
                fp.close();
                return -1;
            }
            pos.Z=posgroup[2].toFloat(&ok);
            if(ok==false)
            {
                fp.close();
                return -1;
            }
            pos.RX=posgroup[3].toFloat(&ok);
            if(ok==false)
            {
                return -1;
            }
            pos.RY=posgroup[4].toFloat(&ok);
            if(ok==false)
            {
                fp.close();
                return -1;
            }
            pos.RZ=posgroup[5].toFloat(&ok);
            if(ok==false)
            {
                fp.close();
                return -1;
            }
            pos.out_1=posgroup[6].toInt(&ok);
            if(ok==false)
            {
                fp.close();
                return -1;
            }
            pos.out_2=posgroup[7].toInt(&ok);
            if(ok==false)
            {
                fp.close();
                return -1;
            }
            pos.out_3=posgroup[8].toInt(&ok);
            if(ok==false)
            {
                fp.close();
                return -1;
            }
        }
        else
        {
            fp.close();
            return -1;
        }
    }
    point=pos;
    fp.close();
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
        msg="Num"+QString::number(n)+": ("+QString::number(trace[n].X,'f',ROBOT_POSE_DECIMAL_PLACE)+","+QString::number(trace[n].Y,'f',ROBOT_POSE_DECIMAL_PLACE)+","+QString::number(trace[n].Z,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                           QString::number(trace[n].RX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+QString::number(trace[n].RY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+QString::number(trace[n].RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                           QString::number(trace[n].out_1)+","+QString::number(trace[n].out_2)+","+QString::number(trace[n].out_3)+")\r\n";
        fp.write(msg.toStdString().c_str());
    }
    fp.close();
    return 0;
}

int toSendbuffer::loadlog_creat(QString filename,std::vector<RobPos> &trace)
{
    trace.clear();
    QFile fp(filename);
    if(!fp.open(QIODevice::ReadOnly))
        return 1;
    QString line;
    QTextStream in(&fp);  //用文件构造流
    line = in.readLine();//读取一行放到字符串里
    while(!line.isNull())//字符串有内容
    {
        QString str = line;
        QString smid;
        RobPos pos;
        str.chop(str.size()-str.lastIndexOf(")"));
        smid=str.right(str.size()-str.indexOf("(")-1);
        QStringList posgroup = smid.split(",");
        if(posgroup.size()!=9)
        {
            fp.close();
            return -1;
        }
        bool ok;
        pos.X=posgroup[0].toFloat(&ok);
        if(ok==false)
        {
            fp.close();
            return -1;
        }
        pos.Y=posgroup[1].toFloat(&ok);
        if(ok==false)
        {
            fp.close();
            return -1;
        }
        pos.Z=posgroup[2].toFloat(&ok);
        if(ok==false)
        {
            fp.close();
            return -1;
        }
        pos.RX=posgroup[3].toFloat(&ok);
        if(ok==false)
        {
            fp.close();
            return -1;
        }
        pos.RY=posgroup[4].toFloat(&ok);
        if(ok==false)
        {
            fp.close();
            return -1;
        }
        pos.RZ=posgroup[5].toFloat(&ok);
        if(ok==false)
        {
            fp.close();
            return -1;
        }
        pos.out_1=posgroup[6].toInt(&ok);
        if(ok==false)
        {
            fp.close();
            return -1;
        }
        pos.out_2=posgroup[7].toInt(&ok);
        if(ok==false)
        {
            fp.close();
            return -1;
        }
        pos.out_3=posgroup[8].toInt(&ok);
        if(ok==false)
        {
            fp.close();
            return -1;
        }
        trace.push_back(pos);
        line=in.readLine();//循环读取下行
    }
    fp.close();
    return 0;
}

int toSendbuffer::savelog_trace(QString filename,std::vector<Weld_trace_onec> trace)
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
        for(int m=0;m<trace[n].point.size();m++)
        {
            msg="Num"+QString::number(n)+","+QString::number(m)+": ("+QString::number(trace[n].point[m].X,'f',ROBOT_POSE_DECIMAL_PLACE)+","+QString::number(trace[n].point[m].Y,'f',ROBOT_POSE_DECIMAL_PLACE)+","+QString::number(trace[n].point[m].Z,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                      QString::number(trace[n].point[m].RX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+QString::number(trace[n].point[m].RY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+QString::number(trace[n].point[m].RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                      QString::number(trace[n].point[m].out_1)+","+QString::number(trace[n].point[m].out_2)+","+QString::number(trace[n].point[m].out_3)+")\r\n";
            fp.write(msg.toStdString().c_str());
        }
    }
    fp.close();
    return 0;
}

int toSendbuffer::savetemp_pos(Point_robpos_result pos)
{
    int rc;
    QString dir="./temp/";
    QString key=SAVELOGFILE_POSNAME_HEAD;
    dir=dir+key+pos.name;
    pos.robotpos.nEn=pos.nEn;
    rc=savelog_point(dir,pos.robotpos);
    return rc;
}

int toSendbuffer::loadtemp_pos(Point_robpos_result &pos)
{
    int rc;
    QString dir="./temp/";
    QString key=SAVELOGFILE_POSNAME_HEAD;
    dir=dir+key+pos.name;
    rc=loadlog_point(dir,pos.robotpos);
    pos.nEn=pos.robotpos.nEn;
    return rc;
}

int toSendbuffer::savetemp_scan(Scan_trace_result trace)
{
    int rc;
    QString dir="./temp/";
    QString key=SAVELOGFILE_SCANNAME_HEAD;
    dir=dir+key+trace.name;
    rc=savelog_scan_json(dir,trace.point);
    return rc;
}

int toSendbuffer::loadtemp_scan(Scan_trace_result &trace)
{
    int rc;
    QString dir="./temp/";
    QString key=SAVELOGFILE_SCANNAME_HEAD;
    dir=dir+key+trace.name;
    rc=loadlog_scan_json(dir,trace.point);
    return rc;
}

int toSendbuffer::Loadjsonfile(char* filename,QJsonDocument &jsonDoc)
{
    QFile loadFile(filename);

    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Unable to load JSON file";
        return 1;
    }

    QByteArray allData = loadFile.readAll();
    loadFile.close();

    QJsonParseError json_error;
    jsonDoc=QJsonDocument::fromJson(allData, &json_error);

    if (json_error.error != QJsonParseError::NoError)
    {
        qDebug() << "JSON error!";
        return 1;
    }

    return 0;
}

int toSendbuffer::Savejsonfile(char* filename,QVariantHash data)
{
    QJsonObject rootObj = QJsonObject::fromVariantHash(data);
    QJsonDocument document;
    document.setObject(rootObj);

    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    QString json_str(byte_array);
    //根据实际填写路径
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "file error!";
        return 1;
    }
    QTextStream in(&file);
    in << json_str;

    file.close();   // 关闭file
    return 0;
}
