#include "setprojectdlg.h"
#include "ui_setprojectdlg.h"

extern QMutex send_group_leaser;
extern QMutex send_group_robot;

setprojectDlg::setprojectDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setprojectDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;

    for(int n=0;n<ROBOTTCPNUM;n++)
    {
        QString msg="TCP: "+QString::number(n);
        ui->movetcpcombo->addItem(msg);
        ui->scantcpcombo->addItem(msg);
        ui->ctlmovetcpcombo->addItem(msg);
    }

    for(int n=0;n<TRACE_EDIT_ID_TOTAL_NUM;n++)
    {
        QString msg=Trace_edit_mode_toQString((Trace_edit_mode)n);
        ui->traceeditcombo->addItem(msg);
    }

    adoubleValidator_speed = new QDoubleValidator(ROBOT_SPEED_DECIMAL_BOTTOM,ROBOT_SPEED_DECIMAL_TOP,ROBOT_SPEED_DECIMAL_PLACE,this);//限制3位小数
    ui->movespeed->setValidator(adoubleValidator_speed);
    ui->tracespeed->setValidator(adoubleValidator_speed);
    ui->scanspeed->setValidator(adoubleValidator_speed);
    ui->ctlmovespeed->setValidator(adoubleValidator_speed);

    edittext=new edittextDlg();
    setmovec=new setmovecDlg(mcs);
    othercmd=new othercmdDlg(mcs);
    arrvepointtest=new arrvepointtestDlg(mcs);
    keycam=new keycamDlg(mcs);
    keycreat=new keycreatDlg(mcs);
    keydelay=new keydelayDlg(mcs);
    keyio=new keyioDlg(mcs);
    keyplc=new keyplcDlg(mcs);
    keymov=new keymovDlg(mcs);
    keyscan=new keyscanDlg(mcs);
    keytrace=new keytraceDlg(mcs);
    keyweld=new keyweldDlg(mcs);
    keysearch=new keysearchDlg(mcs);
    keycoord=new keycoordDlg(mcs);
    keygetpos=new keygetposDlg(mcs);
    keysmov=new keysmovDlg(mcs);
    keysscan=new keysscanDlg(mcs);
    keysearchend=new keysearchendDlg(mcs);
    keysample=new keysampleDlg(mcs);
    keytracing=new keytracingDlg(mcs);
    keytraceadd=new keytraceaddDlg(mcs);
    keygoweld=new keygoweldDlg(mcs);
    keywave=new keywaveDlg(mcs);
    keyaout=new keyaoutDlg(mcs);
    keycreatp=new keycreatpDlg(mcs);
    keysetpose=new keysetposeDlg(mcs);
    keygettcppos=new keygettcpposDlg(mcs);
    keycreatf=new keycreatfDlg(mcs);
    keyplotpos=new keyplotposDlg(mcs);
    keycreatc=new keycreatcDlg(mcs);
    keygettcppos2=new keygettcppos2Dlg(mcs);
    keytracecontinue=new keytracecontinueDlg(mcs);
    keyfilter=new keyfilterDlg(mcs);
    keygoto=new keygotoDlg(mcs);
    traceedit0=new traceedit0Dlg(mcs);
    traceedit1=new traceedit1Dlg(mcs);
    traceedit2=new traceedit2Dlg(mcs); 

    thread1=NULL;
}

setprojectDlg::~setprojectDlg()
{
    delete edittext;
    delete setmovec;
    delete othercmd;
    delete arrvepointtest;
    delete keycam;
    delete keycreat;
    delete keydelay;
    delete keyio;
    delete keyplc;
    delete keymov;
    delete keyscan;
    delete keytrace;
    delete keyweld;
    delete keysearch;
    delete keycoord;
    delete keygetpos;
    delete keysmov;
    delete keysscan;
    delete keysearchend;
    delete keysample;
    delete keytracing;
    delete keytraceadd;
    delete keygoweld;
    delete keywave;
    delete keyaout;
    delete keycreatp;
    delete keysetpose;
    delete keygettcppos;
    delete keycreatf;
    delete keyplotpos;
    delete keycreatc;
    delete keygettcppos2;
    delete keytracecontinue;
    delete keyfilter;
    delete keygoto;
    delete traceedit0;
    delete traceedit1;
    delete traceedit2;
    delete adoubleValidator_speed;
    delete ui;
}

void setprojectDlg::init_dlg_show()
{
    if(m_mcs->mainDlg_robotEnable==true)
        ui->ctlrobotEncheckBox->setCheckState(Qt::Checked);
    else
        ui->ctlrobotEncheckBox->setCheckState(Qt::Unchecked);
    ui->ctlmovespeed->setText(QString::number(m_mcs->e2proomdata.maindlg_movespeed,'f',ROBOT_SPEED_DECIMAL_PLACE));
    ui->ctlmovetcpcombo->setCurrentIndex(m_mcs->e2proomdata.maindlg_movetcp);
    now_cmdline=m_mcs->project->project_cmdlist.size()-1;
    updatacmdlistUi();
}

void setprojectDlg::close_dlg_show()
{
    if(thread1!=NULL)
    {
        thread1->Stop();
        thread1->quit();
        thread1->wait();
        delete thread1;
        thread1=NULL;

        if(m_mcs->resultdata.link_param_state==true)
        {
            u_int16_t tab_reg[1];
            tab_reg[0]=0;
            modbus_write_registers(m_mcs->resultdata.ctx_param,ALS_SHOW_STEP_REG_ADD,1,tab_reg);
        }
        if(m_mcs->resultdata.link_param_state==true)
        {
            modbus_close(m_mcs->resultdata.ctx_param);
            modbus_free(m_mcs->resultdata.ctx_param);
            m_mcs->resultdata.link_param_state=false;
            QString msg=QString::number(PORT_ALS_PARAMETER);
            ui->record->append(msg+QString::fromLocal8Bit("端口关闭"));
        }

        m_mcs->cam->sop_cam[0].DisConnect();
        ui->ConnectCamBtn->setText(QString::fromLocal8Bit("连接相机"));
    }
}

void setprojectDlg::on_moveaddBtn_clicked()//插入移动指令
{
    int tcp=ui->movetcpcombo->currentIndex();
    if(tcp<0||tcp>ui->movetcpcombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择一个tcp"));
        return;
    }
    if(m_mcs->rob->b_link_ctx_posget==true)
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
        usleep(ROB_WORK_DELAY);
        int num=0;
        while(m_mcs->rob->b_send_group_robot==false)
        {
            if(num>10)
            {
                break;
            }
            usleep(ROB_WORK_DELAY_STEP);
            num++;
        }
        if(m_mcs->rob->b_send_group_robot==false)
        {
            ui->record->append(QString::fromLocal8Bit("机器人TCP设置异常"));
            return;
        }
        usleep(ROB_WORK_DELAY);//等待服务器获取到机器人坐标
        num=0;
        m_mcs->rob->TCPpos.nEn=false;
        while (m_mcs->rob->TCPpos.nEn==false)
        {
            if(num>10)
            {
                break;
            }
            usleep(ROB_WORK_DELAY_STEP);
            num++;
        }
        if(m_mcs->rob->TCPpos.nEn==false)
        {
            ui->record->append(QString::fromLocal8Bit("获取机器人坐标失败"));
            return;
        }
        Robmovemodel movemodel=(Robmovemodel)ui->movemodecombo->currentIndex();
        bool rc;
        float speed=ui->movespeed->text().toFloat(&rc);
        RobPos robpos=m_mcs->rob->TCPpos;
        my_cmd cmd;
        QString msg;
        if(ui->movespeed->text().isEmpty())
        {
            ui->record->append(QString::fromLocal8Bit("请填写移动速度"));
            return;
        }
        if(rc==false)
        {
            ui->record->append(QString::fromLocal8Bit("移动速度格式出错"));
            return;
        }
        switch(movemodel)
        {
            case MOVEL:
            case MOVEJ:
            {
                msg=cmd.cmd_move(robpos,movemodel,speed,tcp);
            }
            break;
            case MOVEC:
            {
                setmovec->init_dlg_show();
                setmovec->setWindowTitle(QString::fromLocal8Bit("圆弧移动设置"));
                setmovec->set_arrive_param(speed,tcp);
                int rc=setmovec->exec();
                setmovec->close_dlg_show();
                if(rc!=0)//确定
                {
                    msg=cmd.cmd_moveC(setmovec->pos_st,setmovec->pos_center,setmovec->pos_ed,movemodel,speed,tcp);
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消圆弧移动设置"));
                    return;
                }
            }
            break;
        }
        if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
        {
            m_mcs->project->project_cmdlist.push_back(msg);
        }
        else
        {
            m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
        }
        ui->record->append(QString::fromLocal8Bit("插入移动指令成功"));
        now_cmdline++;
        updatacmdlistUi();
    }
    else
    {
        ui->record->append(QString::fromLocal8Bit("与机器人的连接异常"));
    }
}


void setprojectDlg::on_delaytimeBtn_clicked()//插入延时指令
{
    bool rc;
    int time=ui->delaytime->text().toInt(&rc);
    my_cmd cmd;
    QString msg=cmd.cmd_delay(time);
    if(ui->delaytime->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写延时时间"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("延时时间格式出错"));
        return;
    }
    if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
    {
        m_mcs->project->project_cmdlist.push_back(msg);
    }
    else
    {
        m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
    }
    ui->record->append(QString::fromLocal8Bit("插入延时指令成功"));
    now_cmdline++;
    updatacmdlistUi();
}


void setprojectDlg::on_leaseropenBtn_clicked()//插入开激光指令
{
    bool rc;
    int task=ui->leasertasknum->text().toInt(&rc);
    my_cmd cmd;
    QString msg=cmd.cmd_cam(task,1);
    if(ui->leasertasknum->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写任务号"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("任务号格式出错"));
        return;
    }
    if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
    {
        m_mcs->project->project_cmdlist.push_back(msg);
    }
    else
    {
        m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
    }
    ui->record->append(QString::fromLocal8Bit("插入开激光指令成功"));
    now_cmdline++;
    updatacmdlistUi();
}


void setprojectDlg::on_leasercloseBtn_clicked()//插入关激光指令
{
    my_cmd cmd;
    QString msg=cmd.cmd_cam_work(0);
    if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
    {
        m_mcs->project->project_cmdlist.push_back(msg);
    }
    else
    {
        m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
    }
    ui->record->append(QString::fromLocal8Bit("插入关激光指令成功"));
    now_cmdline++;
    updatacmdlistUi();
}


void setprojectDlg::on_welderarcingBtn_clicked()//插入起弧指令
{
    bool rc;
    float eled=ui->weldercurrent->text().toFloat(&rc);
    float elev=ui->weldercurrent_v->text().toFloat(&rc);
    Alternatingcurrent elem=(Alternatingcurrent)ui->weldermodelcombo->currentIndex();
    my_cmd cmd;
    QString msg=cmd.cmd_elec(eled,elev,elem,1);
    if(ui->weldercurrent->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写电流值"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("电流值格式出错"));
        return;
    }
    if(ui->weldercurrent_v->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写电压值"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("电压值格式出错"));
        return;
    }
    if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
    {
        m_mcs->project->project_cmdlist.push_back(msg);
    }
    else
    {
        m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
    }
    ui->record->append(QString::fromLocal8Bit("插入起弧指令成功"));
    now_cmdline++;
    updatacmdlistUi();
}


void setprojectDlg::on_welderarcoutBtn_clicked()//插入息弧指令
{
    my_cmd cmd;
    QString msg=cmd.cmd_elec_work(0);
    if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
    {
        m_mcs->project->project_cmdlist.push_back(msg);
    }
    else
    {
        m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
    }
    ui->record->append(QString::fromLocal8Bit("插入息弧指令成功"));
    now_cmdline++;
    updatacmdlistUi();
}


void setprojectDlg::on_scanaddBtn_clicked()//插入采集数据指令
{
    int tcp=ui->scantcpcombo->currentIndex();
    if(tcp<0||tcp>ui->scantcpcombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择一个tcp"));
        return;
    }
    if(m_mcs->rob->b_link_ctx_posget==true)
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
        usleep(ROB_WORK_DELAY);
        int num=0;
        while(m_mcs->rob->b_send_group_robot==false)
        {
            if(num>10)
            {
                break;
            }
            usleep(ROB_WORK_DELAY_STEP);
            num++;
        }
        if(m_mcs->rob->b_send_group_robot==false)
        {
            ui->record->append(QString::fromLocal8Bit("机器人TCP设置异常"));
            return;
        }
        usleep(ROB_WORK_DELAY);//等待服务器获取到机器人坐标
        num=0;
        m_mcs->rob->TCPpos.nEn=false;
        while (m_mcs->rob->TCPpos.nEn==false)
        {
            if(num>10)
            {
                break;
            }
            usleep(ROB_WORK_DELAY_STEP);
            num++;
        }
        if(m_mcs->rob->TCPpos.nEn==false)
        {
            ui->record->append(QString::fromLocal8Bit("获取机器人坐标失败"));
            return;
        }
        bool rc;
        float speed=ui->scanspeed->text().toFloat(&rc);
        RobPos robpos=m_mcs->rob->TCPpos;
        Robmovemodel movemodel=(Robmovemodel)ui->scanmovemodecombo->currentIndex();
        my_cmd cmd;
        QString name=ui->scanname->text();
        if(ui->scanspeed->text().isEmpty())
        {
            ui->record->append(QString::fromLocal8Bit("请填写采集速度"));
            return;
        }
        if(rc==false)
        {
            ui->record->append(QString::fromLocal8Bit("采集速度格式出错"));
            return;
        }
        if(ui->scanname->text().isEmpty())
        {
            ui->record->append(QString::fromLocal8Bit("请填写轨迹名称"));
            return;
        }
        QString msg;

        switch(movemodel)
        {
            case MOVEL:
            case MOVEJ:
            {
                msg=cmd.cmd_scan(robpos,movemodel,speed,tcp,name);
            }
            break;
            case MOVEC:
            {
                setmovec->init_dlg_show();
                setmovec->setWindowTitle(QString::fromLocal8Bit("圆弧采集设置"));
                setmovec->set_arrive_param(speed,tcp);
                int rc=setmovec->exec();
                setmovec->close_dlg_show();
                if(rc!=0)//确定
                {
                    msg=cmd.cmd_scanC(setmovec->pos_st,setmovec->pos_center,setmovec->pos_ed,movemodel,speed,tcp,name);
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消圆弧采集设置"));
                    return;
                }
            }
            break;
        }
        std::vector<QString> err_msg;
        m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg);
        for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
        {
            if(name==m_mcs->project->project_scan_trace[n].name)
            {
                ui->record->append(QString::fromLocal8Bit("扫描轨迹与已有的轨迹重名"));
                return;
            }
        }
        if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
        {
            m_mcs->project->project_cmdlist.push_back(msg);
        }
        else
        {
            m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
        }
        ui->record->append(QString::fromLocal8Bit("插入采集指令成功"));
        now_cmdline++;
        updatacmdlistUi();
    }
    else
    {
        ui->record->append(QString::fromLocal8Bit("与机器人的连接异常"));
    }
}


void setprojectDlg::on_tracecmdaddBtn_clicked()//插入跟踪轨迹指令
{
    bool rc;
    int route=ui->tracetrackcombo->currentIndex();
    QString name_in=ui->tracetrackcombo->currentText();
    QString name_out=ui->tracefilename->text();
    float speed=ui->tracespeed->text().toFloat(&rc);
    if(route<0||route>ui->tracetrackcombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择要插入的轨迹名字"));
        return;
    }
    else if(ui->tracespeed->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写跟踪速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("跟踪速度格式出错"));
        return;
    }
    if(name_out.isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写跟踪轨迹工艺名字"));
        return;
    }
    my_cmd cmd;
    QString filepath=ui->tracefilepath->text();//工艺包路径
    //请选择工艺包路径
    if(filepath.size()==0)
    {
        ui->record->append(QString::fromLocal8Bit("工艺包路径为空"));
        return;
    }
    else
    {
        //这里判断工艺包是否格式正确
    #if _MSC_VER
        QTextCodec *code = QTextCodec::codecForName("GBK");
    #else
        QTextCodec *code = QTextCodec::codecForName("UTF-8");
    #endif
        std::string fname = code->fromUnicode(filepath).data();
        int rc=m_mcs->craft->LoadCraft((char*)fname.c_str());
        if(rc==1)
        {
            ui->record->append(QString::fromLocal8Bit("该路径找不到工艺包文件"));
            return;
        }
        else if(rc==2)
        {
            ui->record->append(QString::fromLocal8Bit("工艺包文件格式错误"));
            return;
        }
    }
    QString msg=cmd.cmd_trace(name_in,speed,filepath,name_out);
    if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
    {
        m_mcs->project->project_cmdlist.push_back(msg);
    }
    else
    {
        m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
    }
    ui->record->append(QString::fromLocal8Bit("插入跟踪轨迹指令成功"));
    now_cmdline++;
    updatacmdlistUi();
}


void setprojectDlg::on_traceeditBtn_clicked()//编辑生成跟踪轨迹
{
    std::vector<QString> err_msg;
    if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
    {
        std::vector<QString> err_msg;
        QString name=ui->tracename->text();
        if(name.size()==0)
        {
            ui->record->append(QString::fromLocal8Bit("请输入要插入的轨迹名字"));
            return;
        }
        for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
        {
            if(name==m_mcs->project->project_weld_trace[n].name)
            {
                ui->record->append(QString::fromLocal8Bit("跟踪轨迹与已有的轨迹重名"));
                return;
            }
        }
        m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(now_cmdline+1,err_msg);
        if(m_mcs->project->project_scan_trace.size()<=0)
        {
            ui->record->append(QString::fromLocal8Bit("当前指令位置没有可用的扫描轨迹"));
            return;
        }
        Trace_edit_mode trace_edit_mode=(Trace_edit_mode)ui->traceeditcombo->currentIndex();
        switch(trace_edit_mode)
        {
            case TRACE_EDIT_MODE_ONE_TO_ONE://单扫对单轨道模式
            {
                traceedit0->init_dlg_show();
                traceedit0->setWindowTitle(QString::fromLocal8Bit("生成跟踪轨迹(单扫对单轨模式)"));
                int rc=traceedit0->exec();
                traceedit0->close_dlg_show();
                if(rc!=0)//确定保存生成轨迹
                {
                    my_cmd cmd;
                    std::vector<QString> scanname(1);

                    scanname[0]=traceedit0->name;
                    QString msg=cmd.cmd_creat(trace_edit_mode,scanname,name);
                    if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                    {
                        m_mcs->project->project_cmdlist.push_back(msg);
                    }
                    else
                    {
                        m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                    }
                    ui->record->append(QString::fromLocal8Bit("插入生成跟踪轨迹指令成功"));
                    now_cmdline++;
                    updatacmdlistUi();
                }
            }
            break;
            case TRACE_EDIT_MODE_THREE_TO_ONE:  //三直线交点模式
            {
                traceedit1->init_dlg_show();
                traceedit1->setWindowTitle(QString::fromLocal8Bit("生成跟踪轨迹(三直线交点模式)"));
                int rc=traceedit1->exec();
                traceedit1->close_dlg_show();
                if(rc!=0)//确定保存生成轨迹
                {
                    my_cmd cmd;
                    std::vector<QString> scanname(3);

                    scanname[0]=traceedit1->name0;
                    scanname[1]=traceedit1->name1;
                    scanname[2]=traceedit1->name2;
                    QString msg=cmd.cmd_creat(trace_edit_mode,scanname,name);
                    if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                    {
                        m_mcs->project->project_cmdlist.push_back(msg);
                    }
                    else
                    {
                        m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                    }
                    ui->record->append(QString::fromLocal8Bit("插入生成跟踪轨迹指令成功"));
                    now_cmdline++;
                    updatacmdlistUi();
                }
            }
            break;
            case TRACE_EDIT_MODE_TOWPOINT_THREE_TO_ONE: //两端点三直线交点模式
            {
                traceedit2->init_dlg_show();
                traceedit2->setWindowTitle(QString::fromLocal8Bit("生成跟踪轨迹(两端点三直线交点模式)"));
                int rc=traceedit2->exec();
                traceedit2->close_dlg_show();
                if(rc!=0)//确定保存生成轨迹
                {
                    my_cmd cmd;
                    std::vector<QString> scanname(5);

                    scanname[0]=traceedit2->name0;
                    scanname[1]=traceedit2->name1;
                    scanname[2]=traceedit2->name2;
                    scanname[3]=traceedit2->name3;
                    scanname[4]=traceedit2->name4;

                    QString msg=cmd.cmd_creat(trace_edit_mode,scanname,name);
                    if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                    {
                        m_mcs->project->project_cmdlist.push_back(msg);
                    }
                    else
                    {
                        m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                    }
                    ui->record->append(QString::fromLocal8Bit("插入生成跟踪轨迹指令成功"));
                    now_cmdline++;
                    updatacmdlistUi();
                }
            }
            break;
        }
    }
    else
    {
        QString msg=QString::fromLocal8Bit("程序有错误，请先排除:");
        ui->record->append(msg);
        for(int i=0;i<err_msg.size();i++)
        {
            ui->record->append(err_msg[i]);
        }
    }
}


void setprojectDlg::on_customaddBtn_clicked()//插入自定义指令
{
    std::vector<QString> err_msg;
    QString msg;
    QString key;
    my_cmd cmd;
    if(0>=cmd.decodecmd(ui->customcmd->text(),msg,key))
    {   
        m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg);
        if(key==CMD_SCAN_KEY)
        {
            QString name=cmd.cmd_scan_name;
            for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
            {
                if(name==m_mcs->project->project_scan_trace[n].name)
                {
                    ui->record->append(QString::fromLocal8Bit("扫描轨迹与已有的轨迹重名"));
                    return;
                }
            }
        }
        else if(key==CMD_CREAT_KEY)
        {
            QString name=cmd.cmd_creat_name;
            for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
            {
                if(name==m_mcs->project->project_weld_trace[n].name)
                {
                    ui->record->append(QString::fromLocal8Bit("从跟踪轨迹与已有的轨迹重名"));
                    return;
                }
            }
        }
        else if(key==CMD_TRACE_KEY)
        {
            QString filepath=cmd.cmd_trace_craftfilepath;
        #if _MSC_VER
            QTextCodec *code = QTextCodec::codecForName("GBK");
        #else
            QTextCodec *code = QTextCodec::codecForName("UTF-8");
        #endif
            std::string fname = code->fromUnicode(filepath).data();
            int rc=m_mcs->craft->LoadCraft((char*)fname.c_str());
            if(rc==1)
            {
                ui->record->append(QString::fromLocal8Bit("该路径找不到工艺包文件"));
                return;
            }
            else if(rc==2)
            {
                ui->record->append(QString::fromLocal8Bit("工艺包文件格式错误"));
                return;
            }
        }
        //解码成功
        if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
        {
            m_mcs->project->project_cmdlist.push_back(ui->customcmd->text());
        }
        else
        {
            m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,ui->customcmd->text());
        }
        ui->record->append(QString::fromLocal8Bit("插入自定义指令成功"));
        now_cmdline++;
        updatacmdlistUi();
    }
    else
    {
        ui->record->append(msg);
    }
}

void setprojectDlg::on_customreplaceBtn_clicked()//替换自定义指令
{
    std::vector<QString> err_msg;
    QString msg;
    QString key;
    my_cmd cmd;
    if(0>=cmd.decodecmd(ui->customcmd->text(),msg,key))
    {
        //解码成功
        if(key==CMD_TRACE_KEY)
        {
            QString filepath=cmd.cmd_trace_craftfilepath;
        #if _MSC_VER
            QTextCodec *code = QTextCodec::codecForName("GBK");
        #else
            QTextCodec *code = QTextCodec::codecForName("UTF-8");
        #endif
            std::string fname = code->fromUnicode(filepath).data();
            int rc=m_mcs->craft->LoadCraft((char*)fname.c_str());
            if(rc==1)
            {
                ui->record->append(QString::fromLocal8Bit("该路径找不到工艺包文件"));
                return;
            }
            else if(rc==2)
            {
                ui->record->append(QString::fromLocal8Bit("工艺包文件格式错误"));
                return;
            }
        }
        if(now_cmdline>=0&&m_mcs->project->project_cmdlist.size()>now_cmdline)
        {
            m_mcs->project->project_cmdlist[now_cmdline]=ui->customcmd->text();
            if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
            {
                ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
            }
            else
            {
                for(int n=0;n<err_msg.size();n++)
                {
                    ui->record->append(err_msg[n]);
                }
            }
            updatacmdlistUi();
        }
        else
        {
            ui->record->append(QString::fromLocal8Bit("请先选中要替换的指令"));
        }
    }
    else
    {
        ui->record->append(msg);
    }
}

void setprojectDlg::on_customcheckBtn_clicked()//指令表查看
{
    if(now_cmdline>=0&&m_mcs->project->project_cmdlist.size()>now_cmdline)
    {
        QString cmdlist=m_mcs->project->project_cmdlist[now_cmdline];
        std::vector<QString> err_msg;
        QString msg;
        QString key;
        my_cmd cmd;
        int rc=cmd.decodecmd(cmdlist,msg,key);
        if(rc==0)
        {
            //解码成功
            if(key==CMD_MOV_KEY)
            {
                keymov->init_dlg_show(cmdlist);
                keymov->setWindowTitle(othercmd->cmdname);
                keymov->setbutton(1);
                int rc=keymov->exec();
                keymov->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg=keymov->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_SMOV_KEY)
            {
                keysmov->init_dlg_show(cmdlist);
                keysmov->setWindowTitle(othercmd->cmdname);
                keysmov->setbutton(1);
                int rc=keysmov->exec();
                keysmov->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg=keysmov->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
                    {
                        ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    }
                    else
                    {
                        for(int n=0;n<err_msg.size();n++)
                        {
                            ui->record->append(err_msg[n]);
                        }
                    }
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_DELAY_KEY)
            {
                keydelay->init_dlg_show(cmdlist);
                keydelay->setWindowTitle(othercmd->cmdname);
                keydelay->setbutton(1);
                int rc=keydelay->exec();
                keydelay->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg=keydelay->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_CAM_KEY)
            {
                keycam->init_dlg_show(cmdlist);
                keycam->setWindowTitle(othercmd->cmdname);
                keycam->setbutton(1);
                int rc=keycam->exec();
                keycam->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg=keycam->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_WELD_KEY)
            {
                keyweld->init_dlg_show(cmdlist);
                keyweld->setWindowTitle(othercmd->cmdname);
                keyweld->setbutton(1);
                int rc=keyweld->exec();
                keyweld->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg=keyweld->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_SCAN_KEY)
            {
                keyscan->init_dlg_show(cmdlist);
                keyscan->setWindowTitle(othercmd->cmdname);
                keyscan->setbutton(1);
                int rc=keyscan->exec();
                keyscan->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg=keyscan->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
                    {
                        ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    }
                    else
                    {
                        for(int n=0;n<err_msg.size();n++)
                        {
                            ui->record->append(err_msg[n]);
                        }
                    }
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_SSCAN_KEY)
            {
                keysscan->init_dlg_show(cmdlist);
                keysscan->setWindowTitle(othercmd->cmdname);
                keysscan->setbutton(1);
                int rc=keysscan->exec();
                keysscan->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg=keysscan->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
                    {
                        ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    }
                    else
                    {
                        for(int n=0;n<err_msg.size();n++)
                        {
                            ui->record->append(err_msg[n]);
                        }
                    }
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_TRACE_KEY)
            {
                keytrace->init_dlg_show(cmdlist);
                keytrace->setWindowTitle(othercmd->cmdname);
                keytrace->setbutton(1);
                int rc=keytrace->exec();
                keytrace->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg;
                    QString key;
                    my_cmd cmd;
                    if(0>=cmd.decodecmd(keytrace->cmd_msg,msg,key))
                    {
                        //解码成功
                        if(key==CMD_TRACE_KEY)
                        {
                            QString filepath=cmd.cmd_trace_craftfilepath;
                        #if _MSC_VER
                            QTextCodec *code = QTextCodec::codecForName("GBK");
                        #else
                            QTextCodec *code = QTextCodec::codecForName("UTF-8");
                        #endif
                            std::string fname = code->fromUnicode(filepath).data();
                            int rc=m_mcs->craft->LoadCraft((char*)fname.c_str());
                            if(rc==1)
                            {
                                ui->record->append(QString::fromLocal8Bit("该路径找不到工艺包文件"));
                                return;
                            }
                            else if(rc==2)
                            {
                                ui->record->append(QString::fromLocal8Bit("工艺包文件格式错误"));
                                return;
                            }
                        }
                    }
                    m_mcs->project->project_cmdlist[now_cmdline]=keytrace->cmd_msg;
                    if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
                    {
                        ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    }
                    else
                    {
                        for(int n=0;n<err_msg.size();n++)
                        {
                            ui->record->append(err_msg[n]);
                        }
                    }
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_CREAT_KEY)
            {
                keycreat->init_dlg_show(cmdlist);
                keycreat->setWindowTitle(othercmd->cmdname);
                keycreat->setbutton(1);
                keycreat->set_now_cmdline(now_cmdline);
                int rc=keycreat->exec();
                keycreat->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg=keycreat->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
                    {
                        ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    }
                    else
                    {
                        for(int n=0;n<err_msg.size();n++)
                        {
                            ui->record->append(err_msg[n]);
                        }
                    }
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_IO_KEY)
            {
                keyio->init_dlg_show(cmdlist);
                keyio->setWindowTitle(othercmd->cmdname);
                keyio->setbutton(1);
                int rc=keyio->exec();
                keyio->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg=keyio->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_PLC_KEY)
            {
                keyplc->init_dlg_show(cmdlist);
                keyplc->setWindowTitle(othercmd->cmdname);
                keyplc->setbutton(1);
                int rc=keyplc->exec();
                keyplc->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg=keyplc->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_SEARCH_KEY)//寻位指令
            {
                keysearch->init_dlg_show(cmdlist);
                keysearch->setWindowTitle(othercmd->cmdname);
                keysearch->setbutton(1);
                int rc=keysearch->exec();
                keysearch->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg=keysearch->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
                    {
                        ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    }
                    else
                    {
                        for(int n=0;n<err_msg.size();n++)
                        {
                            ui->record->append(err_msg[n]);
                        }
                    }
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_SEARCHEND_KEY)//寻位指令
            {
                keysearchend->init_dlg_show(cmdlist);
                keysearchend->setWindowTitle(othercmd->cmdname);
                keysearchend->setbutton(1);
                int rc=keysearchend->exec();
                keysearchend->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg=keysearchend->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
                    {
                        ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    }
                    else
                    {
                        for(int n=0;n<err_msg.size();n++)
                        {
                            ui->record->append(err_msg[n]);
                        }
                    }
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_COORD_KEY)//定位指令
            {
                keycoord->init_dlg_show(cmdlist);
                keycoord->setWindowTitle(othercmd->cmdname);
                keycoord->setbutton(1);
                int rc=keycoord->exec();
                keycoord->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg=keycoord->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
                    {
                        ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    }
                    else
                    {
                        for(int n=0;n<err_msg.size();n++)
                        {
                            ui->record->append(err_msg[n]);
                        }
                    }
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_GETPOS_KEY)
            {
                keygetpos->init_dlg_show(cmdlist);
                keygetpos->setWindowTitle(othercmd->cmdname);
                keygetpos->setbutton(1);
                int rc=keygetpos->exec();
                keygetpos->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg=keygetpos->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
                    {
                        ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    }
                    else
                    {
                        for(int n=0;n<err_msg.size();n++)
                        {
                            ui->record->append(err_msg[n]);
                        }
                    }
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_SAMPLE_KEY)
            {
                keysample->init_dlg_show(cmdlist);
                keysample->setWindowTitle(othercmd->cmdname);
                keysample->setbutton(1);
                int rc=keysample->exec();
                keysample->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg=keysample->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
                    {
                        ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    }
                    else
                    {
                        for(int n=0;n<err_msg.size();n++)
                        {
                            ui->record->append(err_msg[n]);
                        }
                    }
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_TRACING_KEY)
            {
                keytracing->init_dlg_show(cmdlist);
                keytracing->setWindowTitle(othercmd->cmdname);
                keytracing->setbutton(1);
                int rc=keytracing->exec();
                keytracing->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg=keytracing->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
                    {
                        ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    }
                    else
                    {
                        for(int n=0;n<err_msg.size();n++)
                        {
                            ui->record->append(err_msg[n]);
                        }
                    }
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_TRACEADD_KEY)
            {
                keytraceadd->init_dlg_show(cmdlist);
                keytraceadd->setWindowTitle(othercmd->cmdname);
                keytraceadd->setbutton(1);
                int rc=keytraceadd->exec();
                keytraceadd->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg=keytraceadd->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
                    {
                        ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    }
                    else
                    {
                        for(int n=0;n<err_msg.size();n++)
                        {
                            ui->record->append(err_msg[n]);
                        }
                    }
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_GOWELD_KEY)
            {
                keygoweld->init_dlg_show(cmdlist);
                keygoweld->setWindowTitle(othercmd->cmdname);
                keygoweld->setbutton(1);
                int rc=keygoweld->exec();
                keygoweld->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg=keygoweld->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
                    {
                        ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    }
                    else
                    {
                        for(int n=0;n<err_msg.size();n++)
                        {
                            ui->record->append(err_msg[n]);
                        }
                    }
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_WAVE_KEY)
            {
                keywave->init_dlg_show(cmdlist);
                keywave->setWindowTitle(othercmd->cmdname);
                keywave->setbutton(1);
                int rc=keywave->exec();
                keywave->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg=keywave->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
                    {
                        ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    }
                    else
                    {
                        for(int n=0;n<err_msg.size();n++)
                        {
                            ui->record->append(err_msg[n]);
                        }
                    }
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_AOUT_KEY)
            {
                keyaout->init_dlg_show(cmdlist);
                keyaout->setWindowTitle(othercmd->cmdname);
                keyaout->setbutton(1);
                int rc=keyaout->exec();
                keyaout->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg=keyaout->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
                    {
                        ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    }
                    else
                    {
                        for(int n=0;n<err_msg.size();n++)
                        {
                            ui->record->append(err_msg[n]);
                        }
                    }
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_CREATP_KEY)
            {
                keycreatp->init_dlg_show(cmdlist);
                keycreatp->setWindowTitle(othercmd->cmdname);
                keycreatp->setbutton(1);
                int rc=keycreatp->exec();
                keycreatp->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg=keycreatp->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
                    {
                        ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    }
                    else
                    {
                        for(int n=0;n<err_msg.size();n++)
                        {
                            ui->record->append(err_msg[n]);
                        }
                    }
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_SETPOSE_KEY)
            {
                keysetpose->init_dlg_show(cmdlist);
                keysetpose->setWindowTitle(othercmd->cmdname);
                keysetpose->setbutton(1);
                int rc=keysetpose->exec();
                keysetpose->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg=keysetpose->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
                    {
                        ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    }
                    else
                    {
                        for(int n=0;n<err_msg.size();n++)
                        {
                            ui->record->append(err_msg[n]);
                        }
                    }
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_GETTCPPOS_KEY)
            {
                keygettcppos->init_dlg_show(cmdlist);
                keygettcppos->setWindowTitle(othercmd->cmdname);
                keygettcppos->setbutton(1);
                int rc=keygettcppos->exec();
                keygettcppos->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg=keygettcppos->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
                    {
                        ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    }
                    else
                    {
                        for(int n=0;n<err_msg.size();n++)
                        {
                            ui->record->append(err_msg[n]);
                        }
                    }
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_CREATF_KEY)
            {
                keycreatf->init_dlg_show(cmdlist);
                keycreatf->setWindowTitle(othercmd->cmdname);
                keycreatf->setbutton(1);
                int rc=keycreatf->exec();
                keycreatf->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg;
                    QString key;
                    my_cmd cmd;
                    if(0>=cmd.decodecmd(keycreatf->cmd_msg,msg,key))
                    {
                        //解码成功
                        if(key==CMD_CREATF_KEY)
                        {
                            QString filename=cmd.cmd_creatf_filename;
                            std::vector<RobPos> weld;
                            int rc=m_mcs->tosendbuffer->loadlog_creat(filename,weld);
                            if(rc==1)
                            {
                                ui->record->append(QString::fromLocal8Bit("该路径找不到跟踪轨迹文件"));
                                return;
                            }
                            else
                            {
                                ui->record->append(QString::fromLocal8Bit("跟踪轨迹文件格式错误"));
                                return;
                            }
                        }
                    }
                    msg=keycreatf->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
                    {
                        ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    }
                    else
                    {
                        for(int n=0;n<err_msg.size();n++)
                        {
                            ui->record->append(err_msg[n]);
                        }
                    }
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_PLOTPOS_KEY)
            {
                keyplotpos->init_dlg_show(cmdlist);
                keyplotpos->setWindowTitle(othercmd->cmdname);
                keyplotpos->setbutton(1);
                keyplotpos->set_now_cmdline(now_cmdline);
                int rc=keyplotpos->exec();
                keyplotpos->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg=keyplotpos->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
                    {
                        ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    }
                    else
                    {
                        for(int n=0;n<err_msg.size();n++)
                        {
                            ui->record->append(err_msg[n]);
                        }
                    }
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_CREATC_KEY)//寻位指令
            {
                keycreatc->init_dlg_show(cmdlist);
                keycreatc->setWindowTitle(othercmd->cmdname);
                keycreatc->setbutton(1);
                int rc=keycreatc->exec();
                keycreatc->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg=keycreatc->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
                    {
                        ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    }
                    else
                    {
                        for(int n=0;n<err_msg.size();n++)
                        {
                            ui->record->append(err_msg[n]);
                        }
                    }
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_GETTCPPOS2_KEY)
            {
                keygettcppos2->init_dlg_show(cmdlist);
                keygettcppos2->setWindowTitle(othercmd->cmdname);
                keygettcppos2->setbutton(1);
                int rc=keygettcppos2->exec();
                keygettcppos2->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg=keygettcppos2->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
                    {
                        ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    }
                    else
                    {
                        for(int n=0;n<err_msg.size();n++)
                        {
                            ui->record->append(err_msg[n]);
                        }
                    }
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_TRACECONTINUE_KEY)
            {
                keytracecontinue->init_dlg_show(cmdlist);
                keytracecontinue->setWindowTitle(othercmd->cmdname);
                keytracecontinue->setbutton(1);
                int rc=keytracecontinue->exec();
                keytracecontinue->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg=keytracecontinue->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
                    {
                        ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    }
                    else
                    {
                        for(int n=0;n<err_msg.size();n++)
                        {
                            ui->record->append(err_msg[n]);
                        }
                    }
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_FILTER_KEY)
            {
                keyfilter->init_dlg_show(cmdlist);
                keyfilter->setWindowTitle(othercmd->cmdname);
                keyfilter->setbutton(1);
                int rc=keyfilter->exec();
                keyfilter->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg=keyfilter->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
                    {
                        ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    }
                    else
                    {
                        for(int n=0;n<err_msg.size();n++)
                        {
                            ui->record->append(err_msg[n]);
                        }
                    }
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_GOTO_KEY)
            {
                keygoto->init_dlg_show(cmdlist);
                keygoto->setWindowTitle(othercmd->cmdname);
                keygoto->setbutton(1);
                int rc=keygoto->exec();
                keygoto->close_dlg_show();
                if(rc!=0)//确定
                {
                    QString msg=keygoto->cmd_msg;
                    m_mcs->project->project_cmdlist[now_cmdline]=msg;
                    if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
                    {
                        ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                    }
                    else
                    {
                        for(int n=0;n<err_msg.size();n++)
                        {
                            ui->record->append(err_msg[n]);
                        }
                    }
                    updatacmdlistUi();
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消替换自定义指令"));
                    return;
                }
            }
            else if(key==CMD_STOP_KEY)
            {
                my_cmd cmd;
                QString msg=cmd.cmd_stop();
                m_mcs->project->project_cmdlist[now_cmdline]=msg;
                if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
                {
                    ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
                }
                else
                {
                    for(int n=0;n<err_msg.size();n++)
                    {
                        ui->record->append(err_msg[n]);
                    }
                }
                updatacmdlistUi();
            }
        }
        else if(rc==-1)
        {
            ui->record->append(QString::fromLocal8Bit("这是注释的指令行"));
        }
        else
        {
            ui->record->append(msg);
        }
    }
    else
    {
        ui->record->append(QString::fromLocal8Bit("请先选中要查看的指令"));
    }
}


void setprojectDlg::on_cmdlistdeleteBtn_clicked()//删除指令
{
    if(now_cmdline>=0&&m_mcs->project->project_cmdlist.size()>now_cmdline)
    {
        m_mcs->project->project_cmdlist.erase(m_mcs->project->project_cmdlist.begin()+now_cmdline);
        ui->record->append(QString::fromLocal8Bit("删除指令"));
        if(now_cmdline>=m_mcs->project->project_cmdlist.size())
            now_cmdline=now_cmdline-1;
        updatacmdlistUi();
    }
    else
    {
        ui->record->append(QString::fromLocal8Bit("请先选中要删除的指令"));
    }
}


void setprojectDlg::on_cmdlistclearBtn_clicked()//清空指令
{
    QMessageBox:: StandardButton result= QMessageBox::information(this, QString::fromLocal8Bit("提示信息"),
                                                                  QString::fromLocal8Bit("确认清空全部指令？"),
                                                                  QMessageBox::Yes|QMessageBox::No,
                                                                  QMessageBox::No
                                                                  );
    switch (result)
    {
        case QMessageBox::Yes:
        {
            m_mcs->project->project_cmdlist.clear();
            now_cmdline=m_mcs->project->project_cmdlist.size()-1;
            updatacmdlistUi();
            ui->record->append(QString::fromLocal8Bit("清空指令完成"));
        }
        break;
        case QMessageBox::No:
        {
            ui->record->append(QString::fromLocal8Bit("取消清空指令操作"));
        }
        break;
    }
}

void setprojectDlg::on_cmdlist_itemClicked(QListWidgetItem *item)//选择值令
{
    now_cmdline=ui->cmdlist->currentRow();
    updatacmdlistUi();
    ui->customcmd->setText(m_mcs->project->project_cmdlist[now_cmdline]);
}

void setprojectDlg::on_SaveAsBtn_clicked()//另存为
{
#ifdef USE_SN_DATA
    m_mcs->sn_data.save();
#endif
    QString project_name;
    edittext->init_dlg_show(QString::fromLocal8Bit("另存为工程名称:"));
    edittext->setWindowTitle(QString::fromLocal8Bit("工程另存为"));
    int rc=edittext->exec();
    edittext->close_dlg_show();
    if(rc!=0)//确定
    {
        project_name=edittext->msg_edit;
        QString fileName = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("请选择要保存的新工程路径"), "./DATA/.json", "JSON(*.json)");
        if(fileName.size()>0)
        {
            m_mcs->project->project_name=project_name;
            QString msg=fileName;
            if(fileName.size()>=5)
            {
                QString tem1=".json";
                QString tem2=".JSON";
                QString tem=fileName.mid(fileName.size()-5,5);
                if(tem!=tem1&&tem!=tem2)//文件名末尾不是".json"或".JSON"
                {
                    msg=msg+".json";
                }
            }
            m_mcs->project->project_path=msg;
            m_mcs->project->SaveProject((char*)msg.toStdString().c_str());
            ui->record->append(QString::fromLocal8Bit("另存成功，当前新工程路径为:")+msg);
        }
        else
        {
            ui->record->append(QString::fromLocal8Bit("保存操作未完成，请重新选择路径"));
        }
    }
    else
    {
        ui->record->append(QString::fromLocal8Bit("取消另存为操作"));
    }
}

void setprojectDlg::on_OKBtn_clicked()//保存指令
{
#ifdef USE_SN_DATA
    m_mcs->sn_data.save();
#endif
    m_mcs->project->SaveProject((char*)m_mcs->project->project_path.toStdString().c_str());
    ui->record->append(QString::fromLocal8Bit("已经保存当前操作"));
}

void setprojectDlg::on_tracefilepathBtn_clicked()//修改路径
{
#ifdef USE_SN_DATA
    m_mcs->sn_data.save();
#endif
    QString fileName = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("请选择要导入的焊接工艺文件"), "./CRAFT/", "CRAFT(*.craft)");
#if _MSC_VER
    QTextCodec *code = QTextCodec::codecForName("GBK");
#else
    QTextCodec *code = QTextCodec::codecForName("UTF-8");
#endif
    std::string name = code->fromUnicode(fileName).data();
    if(name.size()>0)
    {
        if(0!=m_mcs->craft->LoadCraft((char*)name.c_str()))
        {
            ui->record->append(QString::fromLocal8Bit("工艺文件读取失败"));
            return;
        }
        ui->record->append(QString::fromLocal8Bit("工艺文件读取成功"));
        ui->tracefilepath->setText(fileName);
    }
}

void setprojectDlg::on_ConnectCamBtn_clicked()//连接相机
{
    if(m_mcs->resultdata.link_result_state==false)
    {
        ui->record->append(QString::fromLocal8Bit("激光头未连接成功"));
    }
    else
    {
        if(thread1==NULL)
        {
            b_init_show_setproject_inlab_finish=true;
            thread1 = new setprojectThread(this);
            connect(thread1, SIGNAL(Send_show_setproject_inlab(cv::Mat)), this, SLOT(init_show_setproject_inlab(cv::Mat)));
            b_thread1=true;
            thread1->start();

            m_mcs->cam->sop_cam[0].InitConnect(ui->widget,2);

            m_mcs->tosendbuffer->cmd_cam(m_mcs->resultdata.task,1);

            if(m_mcs->resultdata.link_param_state==false)
            {
                QString server_ip=m_mcs->ip->camer_ip->ip;
                QString server_port1=QString::number(PORT_ALS_PARAMETER);
                m_mcs->resultdata.ctx_param = modbus_new_tcp(server_ip.toUtf8(), server_port1.toInt());
                if (modbus_connect(m_mcs->resultdata.ctx_param) == -1)
                {
                    ui->record->append(server_port1+QString::fromLocal8Bit("端口连接失败"));
                    modbus_free(m_mcs->resultdata.ctx_param);
                    return;
                }
                m_mcs->resultdata.link_param_state=true;
                ui->record->append(server_port1+QString::fromLocal8Bit("端口连接成功"));
            }

            u_int16_t tab_reg[1];
            tab_reg[0]=1;
            modbus_write_registers(m_mcs->resultdata.ctx_param,ALS_SHOW_STEP_REG_ADD,1,tab_reg);
            ui->ConnectCamBtn->setText(QString::fromLocal8Bit("断开相机"));
            ui->Camtask->setText(QString::number(m_mcs->resultdata.task));
        }
        else
        {
            thread1->Stop();
            thread1->quit();
            thread1->wait();
            delete thread1;
            thread1=NULL;

            m_mcs->tosendbuffer->cmd_cam(m_mcs->resultdata.task,0);

            if(m_mcs->resultdata.link_param_state==true)
            {
                u_int16_t tab_reg[1];
                tab_reg[0]=0;
                modbus_write_registers(m_mcs->resultdata.ctx_param,ALS_SHOW_STEP_REG_ADD,1,tab_reg);
            }
            if(m_mcs->resultdata.link_param_state==true)
            {
                modbus_close(m_mcs->resultdata.ctx_param);
                modbus_free(m_mcs->resultdata.ctx_param);
                m_mcs->resultdata.link_param_state=false;
                QString msg=QString::number(PORT_ALS_PARAMETER);
                ui->record->append(msg+QString::fromLocal8Bit("端口关闭"));
            }

            m_mcs->cam->sop_cam[0].DisConnect();
            ui->ConnectCamBtn->setText(QString::fromLocal8Bit("连接相机"));
        }
    }
}

void setprojectDlg::on_setCamtaskBtn_clicked()//设置任务号
{
    if(thread1==NULL)
    {
        ui->record->append(QString::fromLocal8Bit("请连接相机后再设置任务号"));
    }
    else
    {
        if(m_mcs->resultdata.link_result_state==true)
        {
            bool b_task;
            uint16_t task=ui->Camtask->text().toInt(&b_task);
            if(b_task==false)
            {
                ui->record->append(QString::fromLocal8Bit("任务号格式错误"));
            }
            else
            {
                m_mcs->tosendbuffer->cmd_cam(task,1);
                ui->record->append(QString::fromLocal8Bit("更新任务号成功"));
            }
        }
        else
        {
            ui->record->append(QString::fromLocal8Bit("请连接相机后再设置任务号"));
        }
    }
}

void setprojectDlg::on_IOoutputBtn_clicked()//输出IO口指令
{
    my_cmd cmd;
    std::vector<int> io(ROBOTOUTPUTNUM);
    QString msg;
    if(ui->checkIO_1->isChecked())
    {
        io[0]=1;
    }
    else
    {
        io[0]=0;
    }
    if(ui->checkIO_2->isChecked())
    {
        io[1]=1;
    }
    else
    {
        io[1]=0;
    }
    if(ui->checkIO_3->isChecked())
    {
        io[2]=1;
    }
    else
    {
        io[2]=0;
    }
    if(ui->checkIO_4->isChecked())
    {
        io[3]=1;
    }
    else
    {
        io[3]=0;
    }
    if(ui->checkIO_5->isChecked())
    {
        io[4]=1;
    }
    else
    {
        io[4]=0;
    }
    if(ui->checkIO_6->isChecked())
    {
        io[5]=1;
    }
    else
    {
        io[5]=0;
    }
    if(ui->checkIO_7->isChecked())
    {
        io[6]=1;
    }
    else
    {
        io[6]=0;
    }
    if(ui->checkIO_8->isChecked())
    {
        io[7]=1;
    }
    else
    {
        io[7]=0;
    }
    msg=cmd.cmd_ioout(io);
    if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
    {
        m_mcs->project->project_cmdlist.push_back(msg);
    }
    else
    {
        m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
    }
    ui->record->append(QString::fromLocal8Bit("插入IO口输出指令成功"));
    now_cmdline++;
    updatacmdlistUi();
}

void setprojectDlg::on_IOinputBtn_clicked()//等待输入IO口指令
{
    my_cmd cmd;
    std::vector<int> io(ROBOTINPUTNUM);
    QString msg;
    if(ui->checkIO_1->isChecked())
    {
        io[0]=1;
    }
    else
    {
        io[0]=0;
    }
    if(ui->checkIO_2->isChecked())
    {
        io[1]=1;
    }
    else
    {
        io[1]=0;
    }
    if(ui->checkIO_3->isChecked())
    {
        io[2]=1;
    }
    else
    {
        io[2]=0;
    }
    if(ui->checkIO_4->isChecked())
    {
        io[3]=1;
    }
    else
    {
        io[3]=0;
    }
    if(ui->checkIO_5->isChecked())
    {
        io[4]=1;
    }
    else
    {
        io[4]=0;
    }
    if(ui->checkIO_6->isChecked())
    {
        io[5]=1;
    }
    else
    {
        io[5]=0;
    }
    if(ui->checkIO_7->isChecked())
    {
        io[6]=1;
    }
    else
    {
        io[6]=0;
    }
    if(ui->checkIO_8->isChecked())
    {
        io[7]=1;
    }
    else
    {
        io[7]=0;
    }
    msg=cmd.cmd_iowaitin(io);
    if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
    {
        m_mcs->project->project_cmdlist.push_back(msg);
    }
    else
    {
        m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
    }
    ui->record->append(QString::fromLocal8Bit("插入等待IO口输入指令成功"));
    now_cmdline++;
    updatacmdlistUi();
}

//插入其他指令
void setprojectDlg::on_othercmdaddBtn_clicked()
{
    othercmd->init_dlg_show();
    othercmd->setWindowTitle(QString::fromLocal8Bit("全部指令"));
    int rc=othercmd->exec();
    othercmd->close_dlg_show();
    if(rc!=0)//确定
    {
        //开启参数设置页面
        QString key=othercmd->cmdkey;
        if(key==CMD_MOV_KEY)
        {
            keymov->init_dlg_show();
            keymov->setWindowTitle(othercmd->cmdname);
            keymov->setbutton(0);
            int rc=keymov->exec();
            keymov->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keymov->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入移动指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消移动指令设置"));
                return;
            }
        }
        else if(key==CMD_SMOV_KEY)
        {
            keysmov->init_dlg_show();
            keysmov->setWindowTitle(othercmd->cmdname);
            keysmov->setbutton(0);
            int rc=keysmov->exec();
            keysmov->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keysmov->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入点位移动指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消点位移动指令设置"));
                return;
            }
        }
        else if(key==CMD_DELAY_KEY)
        {
            keydelay->init_dlg_show();
            keydelay->setWindowTitle(othercmd->cmdname);
            keydelay->setbutton(0);
            int rc=keydelay->exec();
            keydelay->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keydelay->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入延时指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消延时指令设置"));
                return;
            }
        }
        else if(key==CMD_CAM_KEY)
        {
            keycam->init_dlg_show();
            keycam->setWindowTitle(othercmd->cmdname);
            keycam->setbutton(0);
            int rc=keycam->exec();
            keycam->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keycam->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入激光器指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消激光器指令设置"));
                return;
            }
        }
        else if(key==CMD_WELD_KEY)
        {
            keyweld->init_dlg_show();
            keyweld->setWindowTitle(othercmd->cmdname);
            keyweld->setbutton(0);
            int rc=keyweld->exec();
            keyweld->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keyweld->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入弧光指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消弧光指令设置"));
                return;
            }
        }
        else if(key==CMD_SCAN_KEY)
        {
            keyscan->init_dlg_show();
            keyscan->setWindowTitle(othercmd->cmdname);
            keyscan->setbutton(0);
            int rc=keyscan->exec();
            keyscan->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keyscan->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入采集激光数据指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消采集激光数据指令设置"));
                return;
            }
        }
        else if(key==CMD_SSCAN_KEY)
        {
            keysscan->init_dlg_show();
            keysscan->setWindowTitle(othercmd->cmdname);
            keysscan->setbutton(0);
            int rc=keysscan->exec();
            keysscan->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keysscan->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入采集激光数据指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消采集激光数据指令设置"));
                return;
            }
        }
        else if(key==CMD_TRACE_KEY)
        {
            keytrace->init_dlg_show();
            keytrace->setWindowTitle(othercmd->cmdname);
            keytrace->setbutton(0);
            int rc=keytrace->exec();
            keytrace->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keytrace->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入跟踪轨迹指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消跟踪轨迹指令设置"));
                return;
            }
        }
        else if(key==CMD_CREAT_KEY)
        {
            keycreat->init_dlg_show();
            keycreat->setWindowTitle(othercmd->cmdname);
            keycreat->setbutton(0);
            keycreat->set_now_cmdline(now_cmdline);
            int rc=keycreat->exec();
            keycreat->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keycreat->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入生成跟踪轨迹指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消生成跟踪轨迹指令设置"));
                return;
            }
        }
        else if(key==CMD_IO_KEY)
        {
            keyio->init_dlg_show();
            keyio->setWindowTitle(othercmd->cmdname);
            keyio->setbutton(0);
            int rc=keyio->exec();
            keyio->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keyio->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入IO口指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消IO口指令设置"));
                return;
            }
        }
        else if(key==CMD_PLC_KEY)
        {
            keyplc->init_dlg_show();
            keyplc->setWindowTitle(othercmd->cmdname);
            keyplc->setbutton(0);
            int rc=keyplc->exec();
            keyplc->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keyplc->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入PLC指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消PLC指令设置"));
                return;
            }
        }
        else if(key==CMD_SEARCH_KEY)//寻位指令
        {
            keysearch->init_dlg_show();
            keysearch->setWindowTitle(othercmd->cmdname);
            keysearch->setbutton(0);
            int rc=keysearch->exec();
            keysearch->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keysearch->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入寻位指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消寻位指令设置"));
                return;
            }
        }
        else if(key==CMD_SEARCHEND_KEY)//终点寻位指令
        {
            keysearchend->init_dlg_show();
            keysearchend->setWindowTitle(othercmd->cmdname);
            keysearchend->setbutton(0);
            int rc=keysearchend->exec();
            keysearchend->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keysearchend->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入寻位指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消寻位指令设置"));
                return;
            }
        }
        else if(key==CMD_COORD_KEY)//定位指令
        {
            keycoord->init_dlg_show();
            keycoord->setWindowTitle(othercmd->cmdname);
            keycoord->setbutton(0);
            int rc=keycoord->exec();
            keycoord->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keycoord->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入定位指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消定位指令设置"));
                return;
            }
        }
        else if(key==CMD_GETPOS_KEY)
        {
            keygetpos->init_dlg_show();
            keygetpos->setWindowTitle(othercmd->cmdname);
            keygetpos->setbutton(0);
            int rc=keygetpos->exec();
            keygetpos->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keygetpos->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入获取扫描的焊缝坐标值指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消获取扫描的焊缝坐标值指令设置"));
                return;
            }
        }
        else if(key==CMD_SAMPLE_KEY)
        {
            keysample->init_dlg_show();
            keysample->setWindowTitle(othercmd->cmdname);
            keysample->setbutton(0);
            int rc=keysample->exec();
            keysample->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keysample->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入采样轨迹指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消采样轨迹指令设置"));
                return;
            }
        }
        else if(key==CMD_TRACING_KEY)
        {
            keytracing->init_dlg_show();
            keytracing->setWindowTitle(othercmd->cmdname);
            keytracing->setbutton(0);
            int rc=keytracing->exec();
            keytracing->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keytracing->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入跟踪轨迹工艺指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消跟踪轨迹工艺指令设置"));
                return;
            }
        }
        else if(key==CMD_TRACEADD_KEY)
        {
            keytraceadd->init_dlg_show();
            keytraceadd->setWindowTitle(othercmd->cmdname);
            keytraceadd->setbutton(0);
            int rc=keytraceadd->exec();
            keytraceadd->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keytraceadd->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入跟踪轨迹工艺相加指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消跟踪轨迹工艺相加指令设置"));
                return;
            }
        }
        else if(key==CMD_GOWELD_KEY)
        {
            keygoweld->init_dlg_show();
            keygoweld->setWindowTitle(othercmd->cmdname);
            keygoweld->setbutton(0);
            int rc=keygoweld->exec();
            keygoweld->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keygoweld->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入前往起弧点指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消前往起弧点指令设置"));
                return;
            }
        }
        else if(key==CMD_WAVE_KEY)
        {
            keywave->init_dlg_show();
            keywave->setWindowTitle(othercmd->cmdname);
            keywave->setbutton(0);
            int rc=keywave->exec();
            keywave->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keywave->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入生成摆焊指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消生成摆焊指令设置"));
                return;
            }
        }
        else if(key==CMD_AOUT_KEY)
        {
            keyaout->init_dlg_show();
            keyaout->setWindowTitle(othercmd->cmdname);
            keyaout->setbutton(0);
            int rc=keyaout->exec();
            keyaout->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keyaout->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入模拟量输出指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消模拟量输出指令设置"));
                return;
            }
        }
        else if(key==CMD_CREATP_KEY)
        {
            keycreatp->init_dlg_show();
            keycreatp->setWindowTitle(othercmd->cmdname);
            keycreatp->setbutton(0);
            int rc=keycreatp->exec();
            keycreatp->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keycreatp->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入点坐标生成轨迹指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消点坐标生成轨迹指令设置"));
                return;
            }
        }
        else if(key==CMD_SETPOSE_KEY)
        {
            keysetpose->init_dlg_show();
            keysetpose->setWindowTitle(othercmd->cmdname);
            keysetpose->setbutton(0);
            int rc=keysetpose->exec();
            keysetpose->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keysetpose->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入点坐标姿态指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消点坐标姿态指令设置"));
                return;
            }
        }
        else if(key==CMD_GETTCPPOS_KEY)
        {
            keygettcppos->init_dlg_show();
            keygettcppos->setWindowTitle(othercmd->cmdname);
            keygettcppos->setbutton(0);
            int rc=keygettcppos->exec();
            keygettcppos->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keygettcppos->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入获取点坐标指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消获取点坐标指令设置"));
                return;
            }
        }
        else if(key==CMD_CREATF_KEY)
        {
            keycreatf->init_dlg_show();
            keycreatf->setWindowTitle(othercmd->cmdname);
            keycreatf->setbutton(0);
            int rc=keycreatf->exec();
            keycreatf->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keycreatf->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入跟踪轨迹指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消跟踪轨迹指令设置"));
                return;
            }
        }
        else if(key==CMD_PLOTPOS_KEY)
        {
            keyplotpos->init_dlg_show();
            keyplotpos->setWindowTitle(othercmd->cmdname);
            keyplotpos->setbutton(0);
            keyplotpos->set_now_cmdline(now_cmdline);
            int rc=keyplotpos->exec();
            keyplotpos->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keyplotpos->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入计算点坐标指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消计算点坐标指令设置"));
                return;
            }
        }
        else if(key==CMD_CREATC_KEY)
        {
            keycreatc->init_dlg_show();
            keycreatc->setWindowTitle(othercmd->cmdname);
            keycreatc->setbutton(0);
            int rc=keycreatc->exec();
            keycreatc->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keycreatc->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入圆弧跟踪轨迹指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消圆弧跟踪轨迹指令设置"));
                return;
            }
        }
        else if(key==CMD_GETTCPPOS2_KEY)
        {
            keygettcppos2->init_dlg_show();
            keygettcppos2->setWindowTitle(othercmd->cmdname);
            keygettcppos2->setbutton(0);
            int rc=keygettcppos2->exec();
            keygettcppos2->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keygettcppos2->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入获取点坐标指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消获取点坐标指令设置"));
                return;
            }
        }
        else if(key==CMD_TRACECONTINUE_KEY)
        {
            keytracecontinue->init_dlg_show();
            keytracecontinue->setWindowTitle(othercmd->cmdname);
            keytracecontinue->setbutton(0);
            int rc=keytracecontinue->exec();
            keytracecontinue->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keytracecontinue->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入继续跟踪轨迹指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消继续跟踪轨迹指令设置"));
                return;
            }
        }
        else if(key==CMD_FILTER_KEY)
        {
            keyfilter->init_dlg_show();
            keyfilter->setWindowTitle(othercmd->cmdname);
            keyfilter->setbutton(0);
            int rc=keyfilter->exec();
            keyfilter->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keyfilter->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入扫描轨迹滤波指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消扫描轨迹滤波指令设置"));
                return;
            }
        }
        else if(key==CMD_GOTO_KEY)
        {
            keygoto->init_dlg_show();
            keygoto->setWindowTitle(othercmd->cmdname);
            keygoto->setbutton(0);
            int rc=keygoto->exec();
            keygoto->close_dlg_show();
            if(rc!=0)//确定
            {
                QString msg=keygoto->cmd_msg;
                if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                {
                    m_mcs->project->project_cmdlist.push_back(msg);
                }
                else
                {
                    m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                }
                ui->record->append(QString::fromLocal8Bit("插入跳转行数指令成功"));
                now_cmdline++;
                updatacmdlistUi();
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消跳转行数指令设置"));
                return;
            }
        }
        else if(key==CMD_STOP_KEY)
        {
            my_cmd cmd;
            QString msg=cmd.cmd_stop();
            if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
            {
                m_mcs->project->project_cmdlist.push_back(msg);
            }
            else
            {
                m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
            }
            ui->record->append(QString::fromLocal8Bit("插入停止程序指令成功"));
            now_cmdline++;
            updatacmdlistUi();
        }
    }
    else
    {
        ui->record->append(QString::fromLocal8Bit("取消指令设置"));
        return;
    }
}

//机器人使能

void setprojectDlg::on_ctlrobotEncheckBox_clicked()
{
    bool arg1=ui->ctlrobotEncheckBox->isChecked();
    if(arg1==0)
    {
        if(m_mcs->rob->b_connect==false)
        {
            ui->record->append(QString::fromLocal8Bit("机器人通信异常"));
            return;
        }
        else
        {
            m_mcs->tosendbuffer->cmd_totalcontrol(false);
        }
        /*
    #ifdef USE_MYROBOT_CONTROL
        m_mcs->robotcontrol->RobotDisOPEN_ELE();
    #endif
        */
        m_mcs->mainDlg_robotEnable=false;
        ui->record->append(QString::fromLocal8Bit("机器人关闭使能"));
    }
    else
    {
        if(m_mcs->rob->b_connect==false)
        {
            ui->record->append(QString::fromLocal8Bit("机器人通信异常"));
            return;
        }
        else
        {
            m_mcs->tosendbuffer->cmd_totalcontrol(true);
        }
    /*
    #ifdef USE_MYROBOT_CONTROL
        m_mcs->robotcontrol->RobotOPEN_ELE(false);
    #endif
    */
        m_mcs->mainDlg_robotEnable=true;
        ui->record->append(QString::fromLocal8Bit("机器人开启使能"));
    }
}

void setprojectDlg::on_ctlmovetcpcombo_currentIndexChanged(int index)
{
    if(m_mcs->rob->b_link_ctx_posget==true)
    {
        int tcp=index;
        m_mcs->tosendbuffer->cmd_settcp(tcp);
        ui->record->append(QString::fromLocal8Bit("切换tcp"));
    }
    else
    {
        ui->record->append(QString::fromLocal8Bit("与机器人的连接异常"));
    }
}

//到点测试
void setprojectDlg::on_arrvepointtestBtn_clicked()
{
    arrvepointtest->init_dlg_show();
    arrvepointtest->setWindowTitle("到点测试");
    arrvepointtest->exec();
    arrvepointtest->close_dlg_show();
}

int setprojectDlg::task_setmaindlgtcp(int tcp)
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
    usleep(ROB_WORK_DELAY);
    int num=0;
    while(m_mcs->rob->b_send_group_robot==false)
    {
        if(num>10)
        {
            break;
        }
        usleep(ROB_WORK_DELAY_STEP);
        num++;
    }
    if(m_mcs->rob->b_send_group_robot==false)
    {
        ui->record->append(QString::fromLocal8Bit("机器人TCP设置异常"));
        return 1;
    }
    usleep(ROB_WORK_DELAY);//等待服务器获取到机器人坐标
    num=0;
    m_mcs->rob->TCPpos.nEn=false;
    while (m_mcs->rob->TCPpos.nEn==false)
    {
        if(num>10)
        {
            break;
        }
        usleep(ROB_WORK_DELAY_STEP);
        num++;
    }
    if(m_mcs->rob->TCPpos.nEn==false)
    {
        ui->record->append(QString::fromLocal8Bit("获取机器人坐标失败"));
        return 1;
    }
    return 0;
}

//机器人X-按下
void setprojectDlg::on_ctlposXsubBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("机器人未连接成功"));
        return;
    }
    int tcp=ui->ctlmovetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->ctlmovespeed->text().toFloat(&rc);//获取到速度值
    if(tcp<0||tcp>ui->ctlmovetcpcombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择一个tcp"));
        return;
    }
    if(ui->ctlmovespeed->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QString::fromLocal8Bit("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_XSUB;
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QString::fromLocal8Bit("机器人X轴负移动"));
    return;
}

//机器人X-抬起
void setprojectDlg::on_ctlposXsubBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QString::fromLocal8Bit("机器人停止移动"));
}

//机器人X+按下
void setprojectDlg::on_ctlposXaddBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("机器人未连接成功"));
        return;
    }
    int tcp=ui->ctlmovetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->ctlmovespeed->text().toFloat(&rc);//获取到速度值
    if(tcp<0||tcp>ui->ctlmovetcpcombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择一个tcp"));
        return;
    }
    if(ui->ctlmovespeed->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QString::fromLocal8Bit("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_XADD;
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QString::fromLocal8Bit("机器人X轴正移动"));
}

//机器人X+抬起
void setprojectDlg::on_ctlposXaddBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QString::fromLocal8Bit("机器人停止移动"));
}

//机器人Y-按下
void setprojectDlg::on_ctlposYsubBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("机器人未连接成功"));
        return;
    }
    int tcp=ui->ctlmovetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->ctlmovespeed->text().toFloat(&rc);//获取到速度值
    if(tcp<0||tcp>ui->ctlmovetcpcombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择一个tcp"));
        return;
    }
    if(ui->ctlmovespeed->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QString::fromLocal8Bit("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_YSUB;
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QString::fromLocal8Bit("机器人Y轴负移动"));
    return;
}

//机器人Y-抬起
void setprojectDlg::on_ctlposYsubBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QString::fromLocal8Bit("机器人停止移动"));
}

//机器人Y+按下
void setprojectDlg::on_ctlposYaddBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("机器人未连接成功"));
        return;
    }
    int tcp=ui->ctlmovetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->ctlmovespeed->text().toFloat(&rc);//获取到速度值
    if(tcp<0||tcp>ui->ctlmovetcpcombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择一个tcp"));
        return;
    }
    if(ui->ctlmovespeed->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QString::fromLocal8Bit("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_YADD;
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QString::fromLocal8Bit("机器人Y轴正移动"));
    return;
}

//机器人Y+抬起
void setprojectDlg::on_ctlposYaddBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QString::fromLocal8Bit("机器人停止移动"));
}

//机器人Z-按下
void setprojectDlg::on_ctlposZsubBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("机器人未连接成功"));
        return;
    }
    int tcp=ui->ctlmovetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->ctlmovespeed->text().toFloat(&rc);//获取到速度值
    if(tcp<0||tcp>ui->ctlmovetcpcombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择一个tcp"));
        return;
    }
    if(ui->ctlmovespeed->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QString::fromLocal8Bit("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_ZSUB;
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QString::fromLocal8Bit("机器人Z轴负移动"));
    return;
}

//机器人Z-抬起
void setprojectDlg::on_ctlposZsubBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QString::fromLocal8Bit("机器人停止移动"));
}

//机器人Z+按下
void setprojectDlg::on_ctlposZaddBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("机器人未连接成功"));
        return;
    }
    int tcp=ui->ctlmovetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->ctlmovespeed->text().toFloat(&rc);//获取到速度值
    if(tcp<0||tcp>ui->ctlmovetcpcombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择一个tcp"));
        return;
    }
    if(ui->ctlmovespeed->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QString::fromLocal8Bit("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_ZADD;
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QString::fromLocal8Bit("机器人Z轴正移动"));
    return;
}

//机器人Z+抬起
void setprojectDlg::on_ctlposZaddBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QString::fromLocal8Bit("机器人停止移动"));
}

//机器人RX-按下
void setprojectDlg::on_ctlposRXsubBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("机器人未连接成功"));
        return;
    }
    int tcp=ui->ctlmovetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->ctlmovespeed->text().toFloat(&rc);//获取到速度值
    if(tcp<0||tcp>ui->ctlmovetcpcombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择一个tcp"));
        return;
    }
    if(ui->ctlmovespeed->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QString::fromLocal8Bit("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_RXSUB;
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QString::fromLocal8Bit("机器人RX轴负移动"));
    return;
}

//机器人RX-抬起
void setprojectDlg::on_ctlposRXsubBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QString::fromLocal8Bit("机器人停止移动"));
}

//机器人RX+按下
void setprojectDlg::on_ctlposRXaddBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("机器人未连接成功"));
        return;
    }
    int tcp=ui->ctlmovetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->ctlmovespeed->text().toFloat(&rc);//获取到速度值
    if(tcp<0||tcp>ui->ctlmovetcpcombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择一个tcp"));
        return;
    }
    if(ui->ctlmovespeed->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QString::fromLocal8Bit("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_RXADD;
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QString::fromLocal8Bit("机器人RX轴正移动"));
    return;
}

//机器人RX+抬起
void setprojectDlg::on_ctlposRXaddBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QString::fromLocal8Bit("机器人停止移动"));
}

//机器人RY-按下
void setprojectDlg::on_ctlposRYsubBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("机器人未连接成功"));
        return;
    }
    int tcp=ui->ctlmovetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->ctlmovespeed->text().toFloat(&rc);//获取到速度值
    if(tcp<0||tcp>ui->ctlmovetcpcombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择一个tcp"));
        return;
    }
    if(ui->ctlmovespeed->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QString::fromLocal8Bit("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_RYSUB;
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QString::fromLocal8Bit("机器人RY轴负移动"));
    return;
}

//机器人RY-抬起
void setprojectDlg::on_ctlposRYsubBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QString::fromLocal8Bit("机器人停止移动"));
}

//机器人RY+按下
void setprojectDlg::on_ctlposRYaddBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("机器人未连接成功"));
        return;
    }
    int tcp=ui->ctlmovetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->ctlmovespeed->text().toFloat(&rc);//获取到速度值
    if(tcp<0||tcp>ui->ctlmovetcpcombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择一个tcp"));
        return;
    }
    if(ui->ctlmovespeed->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QString::fromLocal8Bit("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_RYADD;
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QString::fromLocal8Bit("机器人RY轴正移动"));
    return;
}

//机器人RY+抬起
void setprojectDlg::on_ctlposRYaddBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QString::fromLocal8Bit("机器人停止移动"));
}

//机器人RZ-按下
void setprojectDlg::on_ctlposRZsubBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("机器人未连接成功"));
        return;
    }
    int tcp=ui->ctlmovetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->ctlmovespeed->text().toFloat(&rc);//获取到速度值
    if(tcp<0||tcp>ui->ctlmovetcpcombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择一个tcp"));
        return;
    }
    if(ui->ctlmovespeed->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QString::fromLocal8Bit("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_RZSUB;
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QString::fromLocal8Bit("机器人RZ轴负移动"));
    return;
}

//机器人RZ-抬起
void setprojectDlg::on_ctlposRZsubBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QString::fromLocal8Bit("机器人停止移动"));
}

//机器人RZ+按下
void setprojectDlg::on_ctlposRZaddBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("机器人未连接成功"));
        return;
    }
    int tcp=ui->ctlmovetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->ctlmovespeed->text().toFloat(&rc);//获取到速度值
    if(tcp<0||tcp>ui->ctlmovetcpcombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择一个tcp"));
        return;
    }
    if(ui->ctlmovespeed->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QString::fromLocal8Bit("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_RZADD;
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QString::fromLocal8Bit("机器人RZ轴正移动"));
    return;
}

//机器人RZ+抬起
void setprojectDlg::on_ctlposRZaddBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QString::fromLocal8Bit("机器人停止移动"));
}

//机器人OUT1-按下
void setprojectDlg::on_ctlposOut1subBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("机器人未连接成功"));
        return;
    }
    int tcp=ui->ctlmovetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->ctlmovespeed->text().toFloat(&rc);//获取到速度值
    if(tcp<0||tcp>ui->ctlmovetcpcombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择一个tcp"));
        return;
    }
    if(ui->ctlmovespeed->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QString::fromLocal8Bit("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_OUT1SUB;
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QString::fromLocal8Bit("机器人OUT1轴负移动"));
    return;
}

//机器人OUT1-抬起
void setprojectDlg::on_ctlposOut1subBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QString::fromLocal8Bit("机器人停止移动"));
}

//机器人OUT1+按下
void setprojectDlg::on_ctlposOut1addBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("机器人未连接成功"));
        return;
    }
    int tcp=ui->ctlmovetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->ctlmovespeed->text().toFloat(&rc);//获取到速度值
    if(tcp<0||tcp>ui->ctlmovetcpcombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择一个tcp"));
        return;
    }
    if(ui->ctlmovespeed->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QString::fromLocal8Bit("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_OUT1ADD;
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QString::fromLocal8Bit("机器人OUT1轴正移动"));
    return;
}

//机器人OUT1+抬起
void setprojectDlg::on_ctlposOut1addBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QString::fromLocal8Bit("机器人停止移动"));
}

//机器人OUT2-按下
void setprojectDlg::on_ctlposOut2subBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("机器人未连接成功"));
        return;
    }
    int tcp=ui->ctlmovetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->ctlmovespeed->text().toFloat(&rc);//获取到速度值
    if(tcp<0||tcp>ui->ctlmovetcpcombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择一个tcp"));
        return;
    }
    if(ui->ctlmovespeed->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QString::fromLocal8Bit("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_OUT2SUB;
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QString::fromLocal8Bit("机器人OUT2轴负移动"));
    return;
}

//机器人OUT2-抬起
void setprojectDlg::on_ctlposOut2subBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QString::fromLocal8Bit("机器人停止移动"));
}

//机器人OUT2+按下
void setprojectDlg::on_ctlposOut2addBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("机器人未连接成功"));
        return;
    }
    int tcp=ui->ctlmovetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->ctlmovespeed->text().toFloat(&rc);//获取到速度值
    if(tcp<0||tcp>ui->ctlmovetcpcombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择一个tcp"));
        return;
    }
    if(ui->ctlmovespeed->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QString::fromLocal8Bit("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_OUT2ADD;
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QString::fromLocal8Bit("机器人OUT2轴正移动"));
    return;
}

//机器人OUT2+抬起
void setprojectDlg::on_ctlposOut2addBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QString::fromLocal8Bit("机器人停止移动"));
}

//机器人OUT3-按下
void setprojectDlg::on_ctlposOut3subBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("机器人未连接成功"));
        return;
    }
    int tcp=ui->ctlmovetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->ctlmovespeed->text().toFloat(&rc);//获取到速度值
    if(tcp<0||tcp>ui->ctlmovetcpcombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择一个tcp"));
        return;
    }
    if(ui->ctlmovespeed->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QString::fromLocal8Bit("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_OUT3SUB;
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QString::fromLocal8Bit("机器人OUT3轴负移动"));
    return;
}

//机器人OUT3-抬起
void setprojectDlg::on_ctlposOut3subBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QString::fromLocal8Bit("机器人停止移动"));
}

//机器人OUT3+按下
void setprojectDlg::on_ctlposOut3addBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("机器人未连接成功"));
        return;
    }
    int tcp=ui->ctlmovetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->ctlmovespeed->text().toFloat(&rc);//获取到速度值
    if(tcp<0||tcp>ui->ctlmovetcpcombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择一个tcp"));
        return;
    }
    if(ui->ctlmovespeed->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QString::fromLocal8Bit("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_OUT3ADD;
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QString::fromLocal8Bit("机器人OUT3轴正移动"));
    return;
}

//机器人OUT3+抬起
void setprojectDlg::on_ctlposOut3addBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QString::fromLocal8Bit("机器人停止移动"));
}

void setprojectDlg::updatacmdlistUi()
{
    std::vector<QString> err_msg;

    ui->cmdlist->clear();
    for(int n=0;n<m_mcs->project->project_cmdlist.size();n++)
    {
        QString msg;
        msg="Line"+QString::number(n)+"  "+m_mcs->project->project_cmdlist[n];
        ui->cmdlist->addItem(msg);
    }
    if(m_mcs->project->project_cmdlist.size()>0)
    {
        ui->cmdlist->setCurrentRow(now_cmdline);
    }

    m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(now_cmdline+1,err_msg);
    ui->tracetrackcombo->clear();
    for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
    {
        ui->tracetrackcombo->addItem(m_mcs->project->project_weld_trace[n].name);
    }
}

void setprojectDlg::init_show_setproject_inlab(cv::Mat cvimg)
{
    if(!cvimg.empty())
    {
         if(cvimg.rows!=CAMBUILD_IMAGE_HEIGHT||
            cvimg.cols!=CAMBUILD_IMAGE_WIDTH)
            cv::resize(cvimg,cvimg,cv::Size(CAMBUILD_IMAGE_WIDTH,CAMBUILD_IMAGE_HEIGHT));
         if(cvimg.type()==CV_8UC1)
            cv::cvtColor(cvimg,cvimg,cv::COLOR_GRAY2BGR);

         QImage::Format format = QImage::Format_RGB888;
         switch (cvimg.type())
         {
         case CV_8UC1:
           format = QImage::Format_RGB888;
           cv::cvtColor(cvimg,cvimg,cv::COLOR_GRAY2BGR);
           break;
         case CV_8UC3:
           format = QImage::Format_RGB888;
           break;
         case CV_8UC4:
           format = QImage::Format_ARGB32;
           break;
         }
         QImage img = QImage((const uchar*)cvimg.data,
                                           cvimg.cols,
                                           cvimg.rows,
                                           cvimg.cols * cvimg.channels(), format);
         img = img.scaled(ui->widget->width(),ui->widget->height(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//图片自适应lab大小
         ui->widget->setImage(img);
    }
    b_init_show_setproject_inlab_finish=true;
    m_mcs->cam->sop_cam[0].b_int_show_image_inlab=false;
    m_mcs->cam->sop_cam[0].b_updataimage_finish=false;
}

setprojectThread::setprojectThread(setprojectDlg *statci_p)
{
    _p=statci_p;
}

void setprojectThread::run()
{
    while (1)
    {
        if(_p->b_thread1==true)
        {
            if(_p->b_init_show_setproject_inlab_finish==true)
            {
                _p->b_init_show_setproject_inlab_finish=false;
                qRegisterMetaType< cv::Mat >("cv::Mat"); //传递自定义类型信号时要添加注册
                emit Send_show_setproject_inlab(_p->m_mcs->cam->sop_cam[0].cv_image);
            }
            sleep(0);
        }
        else
        {
            _p->b_stop_thread1=true;
            break;
        }
    }
}

void setprojectThread::Stop()
{
  if(_p->b_thread1==true)
  {
    _p->b_stop_thread1=false;
    _p->b_thread1=false;
    while (_p->b_stop_thread1==false)
    {
      sleep(0);
    }
  }
}







