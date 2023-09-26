#include "qtweldingdlg.h"
#include "ui_qtweldingdlg.h"
#include<QGridLayout>

QMutex send_group_leaser;
QMutex send_group_robot;
QMutex send_group_plc;
QMutex main_record;

qtweldingDlg::qtweldingDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::qtweldingDlg)
{
#if _MSC_VER
    QString filePath;
    filePath =  "./DATA";
    QDir dir(filePath);
    if(!dir.exists())
        QDir().mkdir(filePath);
    filePath =  "./CRAFT";
    QDir dir(filePath);
    if(!dir.exists())
        QDir().mkdir(filePath);
    filePath =  "./log";
    QDir dir(filePath);
    if(!dir.exists())
        QDir().mkdir(filePath);
    filePath =  "./temp";
    QDir dir(filePath);
    if(!dir.exists())
        QDir().mkdir(filePath);
#else
    std::string dir;
    dir = "./DATA";
    if (access(dir.c_str(), 0) == -1)
    {
      mkdir("./DATA",S_IRWXU);
    }
    dir = "./CRAFT";
    if (access(dir.c_str(), 0) == -1)
    {
      mkdir("./CRAFT",S_IRWXU);
    }
    dir = "./log";
    if (access(dir.c_str(), 0) == -1)
    {
      mkdir("./log",S_IRWXU);
    }
    dir = "./temp";
    if (access(dir.c_str(), 0) == -1)
    {
      mkdir("./temp",S_IRWXU);
    }
#endif
    m_mcs=m_mcs->Get();

    b_init_show_ui_list=true;
    b_init_sent_leaser=true;
    b_init_show_robpos_list=true;
    b_init_set_robtask=true;
//  b_init_set_plctask=true;
    b_init_show_record_list=true;

    b_RunAlgCamer=false;

    m_mcs->mainDlg_robmovestate=MAINDLG_STATIC;

    sndata=new sndataDlg(m_mcs);
    info=new infoDlg(m_mcs);
    qtmysunny=new qtmysunnyDlg(m_mcs);
    demarcate=new demarcateDlg(m_mcs);
    robotset=new robotsetDlg(m_mcs);
    weldset=new weldsetDlg(m_mcs);
    plcset=new plcsetDlg(m_mcs);
    editproject=new editprojectDlg(m_mcs);
    newproject=new newprojectDlg(m_mcs);
    setproject=new setprojectDlg(m_mcs);
    editcraft=new editcraftDlg(m_mcs);
    newcraft=new newcraftDlg(m_mcs);
    checkscandata=new checkscandataDlg(m_mcs);
    setcraft0=new setcraft0Dlg(m_mcs);
    setcraft1=new setcraft1Dlg(m_mcs);
    setcraft2=new setcraft2Dlg(m_mcs);
    setcraft3=new setcraft3Dlg(m_mcs);

    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint        //显示关闭
                  |Qt::WindowMinMaxButtonsHint);    //显示最大最小化

    ui->project_name->setText(QStringLiteral("未命名"));
    ui->project_Id->setText(QStringLiteral("无"));
    ui->project_buildstep->setText(QStringLiteral("0/0"));
    ui->robot_model->setText(m_mcs->rob->robot_model_toQString(m_mcs->rob->robot_model));
    ui->robot_ip_port->setText(QStringLiteral("0.0.0.0"));
    ui->robot_state->setText(m_mcs->rob->robot_state_toQString(m_mcs->rob->robot_state));
    ui->robot_speed->setText(QString::number(m_mcs->rob->robot_speed,'f',3));
    ui->robot_pos_x->setText(QString::number(m_mcs->rob->TCPpos.X,'f',ROBOT_POSE_DECIMAL_PLACE));
    ui->robot_pos_y->setText(QString::number(m_mcs->rob->TCPpos.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
    ui->robot_pos_z->setText(QString::number(m_mcs->rob->TCPpos.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
    ui->robot_pos_rx->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    ui->robot_pos_ry->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    ui->robot_pos_rz->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    ui->leaser_ip->setText(QStringLiteral("0.0.0.0"));
    ui->leaser_pos_y->setText(QStringLiteral("0.000"));
    ui->leaser_pos_z->setText(QStringLiteral("0.000"));
    ui->leaser_state->setText(QStringLiteral("未链接"));
    ui->leaser_tasknum->setText(QStringLiteral("0"));
    ui->leaser_time->setText(QStringLiteral("00:00:00:000"));
    ui->leaser_timestamp->setText(QStringLiteral("00:00:00:000"));
    ui->leaser_camera_fps->setText(QStringLiteral("0.00"));
    ui->leaser_result_fps->setText(QStringLiteral("0.00"));
    ui->weld_model->setText(m_mcs->rob->weld_model_toQString(m_mcs->rob->weld_model));
    ui->weld_ip_port->setText(QStringLiteral("0.0.0.0"));
    ui->weld_state->setText(QStringLiteral("待机"));
    ui->weld_current->setText(QStringLiteral("0.000"));
    ui->weld_process->setText(QStringLiteral("平焊"));
    ui->weld_alternating->setText(QStringLiteral("直流"));
//  ui->robotEncheckBox->setCheckState(Qt::Unchecked);
//  m_mcs->mainDlg_robotEnable=false;

    if(m_mcs->e2proomdata.maindlg_SaveDatacheckBox==0)
    {
        ui->SaveDatacheckBox->setCheckState(Qt::Unchecked);
    }
    else
    {
        ui->SaveDatacheckBox->setCheckState(Qt::Checked);
    }
    if(m_mcs->e2proomdata.maindlg_circlerun==0)
    {
        ui->CircleRuncheckBox->setCheckState(Qt::Unchecked);
    }
    else
    {
        ui->CircleRuncheckBox->setCheckState(Qt::Checked);
    }
    ui->weld_eled->setText(QString::number(m_mcs->e2proomdata.maindlg_Weldeled,'f',3));
    ui->weld_elev->setText(QString::number(m_mcs->e2proomdata.maindlg_Weldelev,'f',3));
    ui->weld_elem->setCurrentIndex(m_mcs->e2proomdata.maindlg_Weldelem);


    adoubleValidator_speed = new QDoubleValidator(ROBOT_SPEED_DECIMAL_BOTTOM,ROBOT_SPEED_DECIMAL_TOP,ROBOT_SPEED_DECIMAL_PLACE,this);//限制3位小数
    ui->movespeed->setValidator(adoubleValidator_speed);
    ui->movespeed->setText(QString::number(m_mcs->e2proomdata.maindlg_movespeed,'f',ROBOT_SPEED_DECIMAL_PLACE));
#ifdef USE_SN_DATA
    //检测序列号
    if(0!=m_mcs->sn_data.checkSN())
    {
        QMessageBox:: StandardButton result= QMessageBox::information(this, QStringLiteral("提示信息"),
                                                                      QStringLiteral("硬件信息出错，程序将退出"),
                                                                      QMessageBox::Yes,
                                                                      QMessageBox::Yes
                                                                      );
        exit(1);
    }
    //查看软件剩余时间
    if(m_mcs->sn_data.nLeftHours==0)//剩余时间无
    {
        sndata->init_dlg_show(QStringLiteral("剩余使用时长0,请输入序列号激活:"));
        sndata->setWindowTitle(QStringLiteral("软件激活"));
        int rc=sndata->exec();
        sndata->close_dlg_show();
        if(rc!=0)//确定
        {

        }
        else
        {
            exit(1);
        }
    }
#endif

    /************************************************/

#ifdef USE_MYROBOT_CONTROL      //是否使用程序自带接口
    m_mcs->robotcontrol->Creat_control_modbus();//创建自带接口
#endif
    /*************************************************/

    thread1 = new qtweldingThread(this);
    connect(thread1, SIGNAL(Send_show_ui_list()), this, SLOT(init_show_ui_list()));
    connect(thread1, SIGNAL(Send_sent_leaser()), this, SLOT(init_sent_leaser()));

    thread2 = new qtgetrobThread(this);
    connect(thread2, SIGNAL(Send_show_robpos_list()), this, SLOT(init_show_robpos_list()));
    connect(thread2, SIGNAL(Send_set_robtask()), this, SLOT(init_set_robtask()));

    thread3 = new qtrecordThread(this);
    connect(thread3, SIGNAL(Send_show_record_list(QString)), this, SLOT(init_show_record_list(QString)));

//  thread4 = new qtplcThread(this);
//  connect(thread4, SIGNAL(Send_set_plctask()), this, SLOT(init_set_plctask()));

#ifdef USE_SN_DATA
    SN_timer = new QTimer();
    connect(SN_timer,&QTimer::timeout,this,&qtweldingDlg::init_SNtimeoutShow);
#endif

 /*****************/

    ConnectCamer();//连接相机
    ConnectRobot();//连接机器人
    ConnectPLC();//连接PLC

    b_thread1=true;
    thread1->start();

    b_thread2=true;
    thread2->start();

    b_thread3=true;
    thread3->start();

//  b_thread4=true;
//  thread4->start();
#ifdef USE_SN_DATA
    SN_timer->start(60000);//60秒进一次终端
#endif
    for(int n=0;n<ROBOTTCPNUM;n++)
    {
        QString msg="TCP: "+QString::number(n);
        ui->movetcpcombo->addItem(msg);
    }

    ui->movetcpcombo->setCurrentIndex(m_mcs->e2proomdata.maindlg_movetcp);

    connect(ui->movetcpcombo,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[=](){
        if(m_mcs->rob->b_link_ctx_posget==true)
        {
            int tcp=ui->movetcpcombo->currentIndex();
            m_mcs->tosendbuffer->cmd_settcp(tcp);
            ui->record->append(QStringLiteral("切换tcp"));
        }
        else
        {
            ui->record->append(QStringLiteral("与机器人的连接异常"));
        }
        UpdataUi();
    });

    UpdataUi();

    ui->record->append(QStringLiteral("系统启动成功"));
}

qtweldingDlg::~qtweldingDlg()
{
#ifdef USE_MYROBOT_CONTROL      //是否使用程序自带接口
    m_mcs->robotcontrol->RobotDisOPEN_ELE();    //机器人下使能
    usleep(ROB_WORK_DELAY);
    m_mcs->robotcontrol->Close_control_modbus();//创建自带接口
#endif

    if(b_thread1==true)
    {
        thread1->Stop();
        thread1->quit();
        thread1->wait();
    }
    if(b_thread2==true)
    {
        thread2->Stop();
        thread2->quit();
        thread2->wait();
    }
    if(b_thread3==true)
    {
        thread3->Stop();
        thread3->quit();
        thread3->wait();
    }
    /*
    if(b_thread4==true)
    {
        thread4->Stop();
        thread4->quit();
        thread4->wait();
    }
    */
    DisconnectCamer();
    DisconnectRobot();
    DisconnectPLC();

    delete thread1;
    delete thread2;
    delete thread3;
//  delete thread4;
    delete sndata;
    delete info;
    delete qtmysunny;
    delete demarcate;
    delete robotset;
    delete weldset;
    delete plcset;
    delete editproject;
    delete newproject;
    delete setproject;
    delete editcraft;
    delete newcraft;
    delete checkscandata;
    delete setcraft0;
    delete setcraft1;
    delete setcraft2;
    delete setcraft3;
    delete adoubleValidator_speed;
    delete ui;
}

void qtweldingDlg::UpdataUi()
{
    if(m_mcs->process->b_processrun==false)
    {
        ui->runprojectBtn->setText(QStringLiteral("运行工程"));
        ui->demarcateBtn->setDisabled(false);
        ui->editprojectBtn->setDisabled(false);
        ui->editweldprocessBtn->setDisabled(false);
        ui->importprojectBtn->setDisabled(false);
        ui->runpausedBtn->setDisabled(true);
        ui->setlaserheadBtn->setDisabled(false);
        ui->setrobotBtn->setDisabled(false);
        ui->weld_fireBtn->setDisabled(false);
        ui->weld_gassBtn->setDisabled(false);
        ui->weld_windBtn->setDisabled(false);
        ui->weld_rewindBtn->setDisabled(false);
        ui->setplcBtn->setDisabled(false);
        ui->setweldBtn->setDisabled(false);
        ui->posXsubBtn->setDisabled(false);
        ui->posXaddBtn->setDisabled(false);
        ui->posYsubBtn->setDisabled(false);
        ui->posYaddBtn->setDisabled(false);
        ui->posZsubBtn->setDisabled(false);
        ui->posZaddBtn->setDisabled(false);
        ui->posRXsubBtn->setDisabled(false);
        ui->posRXaddBtn->setDisabled(false);
        ui->posRYsubBtn->setDisabled(false);
        ui->posRYaddBtn->setDisabled(false);
        ui->posRZsubBtn->setDisabled(false);
        ui->posRZaddBtn->setDisabled(false);
        ui->posOut1subBtn->setDisabled(false);
        ui->posOut1addBtn->setDisabled(false);
        ui->posOut2subBtn->setDisabled(false);
        ui->posOut2addBtn->setDisabled(false);
        ui->posOut3subBtn->setDisabled(false);
        ui->posOut3addBtn->setDisabled(false);
        ui->projectskiprunBtn->setDisabled(false);
        ui->projectcheckdataBtn->setDisabled(false);
        ui->robotEncheckBox->setDisabled(false);
    }
    else
    {
        ui->runprojectBtn->setText(QStringLiteral("停止工程"));
        ui->demarcateBtn->setDisabled(true);
        ui->editprojectBtn->setDisabled(true);
        ui->editweldprocessBtn->setDisabled(true);
        ui->importprojectBtn->setDisabled(true);
        ui->runpausedBtn->setDisabled(false);
        ui->setlaserheadBtn->setDisabled(true);
        ui->setrobotBtn->setDisabled(true);
        ui->weld_fireBtn->setDisabled(true);
        ui->weld_gassBtn->setDisabled(true);
        ui->weld_windBtn->setDisabled(true);
        ui->weld_rewindBtn->setDisabled(true);
        ui->setplcBtn->setDisabled(true);
        ui->setweldBtn->setDisabled(true);
        ui->posXsubBtn->setDisabled(true);
        ui->posXaddBtn->setDisabled(true);
        ui->posYsubBtn->setDisabled(true);
        ui->posYaddBtn->setDisabled(true);
        ui->posZsubBtn->setDisabled(true);
        ui->posZaddBtn->setDisabled(true);
        ui->posRXsubBtn->setDisabled(true);
        ui->posRXaddBtn->setDisabled(true);
        ui->posRYsubBtn->setDisabled(true);
        ui->posRYaddBtn->setDisabled(true);
        ui->posRZsubBtn->setDisabled(true);
        ui->posRZaddBtn->setDisabled(true);
        ui->posOut1subBtn->setDisabled(true);
        ui->posOut1addBtn->setDisabled(true);
        ui->posOut2subBtn->setDisabled(true);
        ui->posOut2addBtn->setDisabled(true);
        ui->posOut3subBtn->setDisabled(true);
        ui->posOut3addBtn->setDisabled(true);
        ui->projectskiprunBtn->setDisabled(true);
        ui->projectcheckdataBtn->setDisabled(true);
        ui->robotEncheckBox->setDisabled(true);
    }
    if(m_mcs->process->b_processpaused==false)
    {
        ui->runpausedBtn->setText(QStringLiteral("暂停工程"));
    }
    else
    {
        ui->runpausedBtn->setText(QStringLiteral("继续工程"));
    }
}

void qtweldingDlg::on_importprojectBtn_clicked()//导入工程
{
#ifdef USE_SN_DATA
    m_mcs->sn_data.save();
#endif
    QString fileName = QFileDialog::getOpenFileName(this, QStringLiteral("请选择要导入的工程文件"), "./DATA/", "JSON(*.json)");
    if(fileName.size()>0)
    {
        if(0!=m_mcs->project->LoadProject((char*)fileName.toStdString().c_str()))
        {
            ui->record->append(QStringLiteral("工程文件读取失败"));
        }
        else
        {
            QString path=fileName;
            QString msg=QStringLiteral("已经成功加载工程文件:")+path;
            ui->record->append(msg);
            msg=QStringLiteral("工程名称:")+m_mcs->project->project_name;
            ui->record->append(msg);
        }
    }
}

void qtweldingDlg::on_runprojectBtn_clicked()//运行工程
{
#ifdef USE_SN_DATA
    m_mcs->sn_data.save();
#endif
    if(m_mcs->process->b_processrun==false)
    {
        if(m_mcs->rob->b_connect==false)
        {
            ui->record->append(QStringLiteral("机器人未链接成功"));
        }
        else if(m_mcs->resultdata.link_result_state==false)
        {
            ui->record->append(QStringLiteral("激光头未链接成功"));
        }
        else if(m_mcs->mainDlg_robotEnable==false)
        {
            ui->record->append(QStringLiteral("请打勾机器人使能"));
        }
        else if(m_mcs->rob->b_link_ctx_posget_remote==false)
        {
            ui->record->append(QStringLiteral("机器人远端未链接成功"));
        }
        else
        {
            m_mcs->process->init_start_process();
            ui->record->append(QStringLiteral("开始运行工程"));
        }
    }
    else
    {
        m_mcs->process->stop_process();

        ui->record->append(QStringLiteral("停止运行工程"));
    }
    UpdataUi();
}

void qtweldingDlg::on_projectskiprunBtn_clicked()//从第N行开始运行工程
{
#ifdef USE_SN_DATA
    m_mcs->sn_data.save();
#endif
    if(m_mcs->process->b_processrun==false)
    {
        bool rc;
        int skipline=ui->projectskiplineEdit->text().toInt(&rc);
        if(rc==false)
        {
            ui->record->append(QStringLiteral("行数格式错误"));
            return;
        }
        if(ui->projectskiplineEdit->text().isEmpty())
        {
            ui->record->append(QStringLiteral("请填入程序的开始行数"));
            return;
        }
        if(skipline<0)
        {
            ui->record->append(QStringLiteral("程序的开始行数必须大于等于0"));
            return;
        }
        if(skipline>=m_mcs->project->project_cmdlist.size())
        {
            ui->record->append(QStringLiteral("程序的开始行数必须小于程序总行数"));
            return;
        }
        //开始执行
        if(m_mcs->rob->b_connect==false)
        {
            ui->record->append(QStringLiteral("机器人未链接成功"));
        }
        else if(m_mcs->resultdata.link_result_state==false)
        {
            ui->record->append(QStringLiteral("激光头未链接成功"));
        }
        else if(m_mcs->mainDlg_robotEnable==false)
        {
            ui->record->append(QStringLiteral("请打勾机器人使能"));
        }
        else if(m_mcs->rob->b_link_ctx_posget_remote==false)
        {
            ui->record->append(QStringLiteral("机器人远端未链接成功"));
        }
        else
        {
            m_mcs->process->init_skip_start_process(skipline);
            ui->record->append(QStringLiteral("开始运行工程"));
        }
    }
    else
    {
        ui->record->append(QStringLiteral("请先停止运行工程"));
    }
    UpdataUi();
}

//查看扫描数据
void qtweldingDlg::on_projectcheckdataBtn_clicked()
{
    m_mcs->process->check_data_process();
    checkscandata->init_dlg_show();
    checkscandata->setWindowTitle(QStringLiteral("查看变量数据"));
    checkscandata->exec();
    checkscandata->close_dlg_show();
}

void qtweldingDlg::on_runpausedBtn_clicked()//暂停工程
{
#ifdef USE_SN_DATA
    m_mcs->sn_data.save();
#endif
    if(m_mcs->process->b_processpaused==false)
    {
        m_mcs->process->paused_process();
        ui->record->append(QStringLiteral("暂停运行工程"));
    }
    else
    {
        m_mcs->process->continue_process();
        ui->record->append(QStringLiteral("继续运行工程"));
    }
    UpdataUi();
}

void qtweldingDlg::on_editprojectBtn_clicked()//工程编辑
{
#ifdef USE_SN_DATA
    m_mcs->sn_data.save();
#endif
    int rc;
    editproject->init_dlg_show();
    editproject->setWindowTitle(QStringLiteral("工程编辑"));
    rc=editproject->exec();
    editproject->close_dlg_show();
    switch(rc)
    {
        case EDITPROJECTDLG_BTN1:   //新建工程
        {
            int rc2;           
            newproject->init_dlg_show();
            newproject->setWindowTitle(QStringLiteral("新建工程"));
            rc2=newproject->exec();
            newproject->close_dlg_show();
            if(rc2!=0)//保存成功返回
            {
                QString msg=QStringLiteral("工程名称: ")+m_mcs->project->project_name+
                            QStringLiteral(" 工程类型")+QString::number(m_mcs->project->project_Id)+": "
                            +m_mcs->project->project_Id_toQString(m_mcs->project->project_Id);
                if(m_mcs->resultdata.link_result_state==true)
                {
                    m_mcs->cam->sop_cam[0].DisConnect();
                }
                switch(m_mcs->project->project_Id)
                {
                    case PROGECT_ID_TEACH_SCAN:
                    {
                        setproject->init_dlg_show();
                        setproject->setWindowTitle(msg);
                        setproject->exec();
                        setproject->close_dlg_show();
                    }
                    break;
                }
                if(m_mcs->resultdata.link_result_state==true)
                {
                    m_mcs->cam->sop_cam[0].InitConnect();
                }
            }
            else
            {
                ui->record->append(QStringLiteral("取消新建工程"));
            }
        }
        break;
        case EDITPROJECTDLG_BTN2:   //编辑当前工程
        {
            QString msg=QStringLiteral("工程名称: ")+m_mcs->project->project_name+
                        QStringLiteral(" 工程类型")+QString::number(m_mcs->project->project_Id)+": "
                        +m_mcs->project->project_Id_toQString(m_mcs->project->project_Id);
            if(m_mcs->resultdata.link_result_state==true)
            {
                m_mcs->cam->sop_cam[0].DisConnect();
            }
            switch(m_mcs->project->project_Id)
            {
                case PROGECT_ID_TEACH_SCAN:
                {
                    setproject->init_dlg_show();
                    setproject->setWindowTitle(msg);
                    setproject->exec();
                    setproject->close_dlg_show();
                }
                break;
            }
            if(m_mcs->resultdata.link_result_state==true)
            {
                m_mcs->cam->sop_cam[0].InitConnect();
            }
        }
        break;
    }
}

void qtweldingDlg::on_editweldprocessBtn_clicked()//焊接工艺设置
{
#ifdef USE_SN_DATA
    m_mcs->sn_data.save();
#endif
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("机器人未连接成功"));
        return;
    }
    int rc;
    editcraft->init_dlg_show();
    editcraft->setWindowTitle(QStringLiteral("焊接工艺设置"));
    rc=editcraft->exec();
    editcraft->close_dlg_show();
    switch(rc)
    {
        case EDITCRAFTDLG_BTN1:   //新建工艺
        {
            int rc2;
            newcraft->init_dlg_show();
            newcraft->setWindowTitle(QStringLiteral("新建工艺"));
            rc2=newcraft->exec();
            newcraft->close_dlg_show();
            if(rc2!=0)//保存成功返回
            {
                QString msg=QStringLiteral(" 工艺类型")+QString::number(m_mcs->craft->craft_id)+": "
                            +m_mcs->craft->craft_Id_toQString(m_mcs->craft->craft_id);
                switch(m_mcs->craft->craft_id)
                {
                    case CRAFT_ID_FIXED_POSTURE://固定焊接姿态
                    {
                        setcraft0->init_dlg_show();
                        setcraft0->setWindowTitle(msg);
                        setcraft0->exec();
                        setcraft0->close_dlg_show();
                    }
                    break;
                    case CRAFT_ID_STARTENDCHANGE_POSTURE://起终点变姿态
                    {
                        setcraft1->init_dlg_show();
                        setcraft1->setWindowTitle(msg);
                        setcraft1->exec();
                        setcraft1->close_dlg_show();
                    }
                    break;
                    case CRAFT_ID_LASERNORMAL_POSTURE: //激光器测量法线姿态
                    {
                        setcraft2->init_dlg_show();
                        setcraft2->setWindowTitle(msg);
                        setcraft2->exec();
                        setcraft2->close_dlg_show();
                    }
                    break;
                    case CRAFT_ID_CORRUGATED_POSTURE: //波纹板变姿态
                    {
                        setcraft3->init_dlg_show();
                        setcraft3->setWindowTitle(msg);
                        setcraft3->exec();
                        setcraft3->close_dlg_show();
                    }
                    break;
                }
            }
            else
            {
                ui->record->append(QStringLiteral("取消新建工艺"));
            }
        }
        break;
        case EDITCRAFTDLG_BTN2:   //选择现有工艺
        {
            QString fileName = QFileDialog::getOpenFileName(this, QStringLiteral("请选择要编辑的工艺文件"), "./CRAFT/", "CRAFT(*.craft)");
            if(fileName.size()>0)
            {
                if(0!=m_mcs->craft->LoadCraft(fileName))
                {
                    ui->record->append(QStringLiteral("工艺文件内容格式错误,读取失败"));
                }
                else
                {
                    m_mcs->craft->craft_path=fileName;
                    QString msg=QStringLiteral(" 工艺类型")+QString::number(m_mcs->craft->craft_id)+": "
                                +m_mcs->craft->craft_Id_toQString(m_mcs->craft->craft_id);
                    switch(m_mcs->craft->craft_id)
                    {
                        case CRAFT_ID_FIXED_POSTURE://固定焊接姿态
                        {
                            setcraft0->init_dlg_show();
                            setcraft0->setWindowTitle(msg);
                            setcraft0->exec();
                            setcraft0->close_dlg_show();
                        }
                        break;
                        case CRAFT_ID_STARTENDCHANGE_POSTURE://起终点变姿态
                        {
                            setcraft1->init_dlg_show();
                            setcraft1->setWindowTitle(msg);
                            setcraft1->exec();
                            setcraft1->close_dlg_show();
                        }
                        break;
                        case CRAFT_ID_LASERNORMAL_POSTURE: //激光器测量法线姿态
                        {
                            setcraft2->init_dlg_show();
                            setcraft2->setWindowTitle(msg);
                            setcraft2->exec();
                            setcraft2->close_dlg_show();
                        }
                        break;
                        case CRAFT_ID_CORRUGATED_POSTURE: //波纹板变姿态
                        {
                            setcraft3->init_dlg_show();
                            setcraft3->setWindowTitle(msg);
                            setcraft3->exec();
                            setcraft3->close_dlg_show();
                        }
                        break;
                    }
                }
            }
        }
        break;
    }
}

void qtweldingDlg::on_setlaserheadBtn_clicked()//激光头设置
{
#ifdef USE_SN_DATA
    m_mcs->sn_data.save();
#endif
    thread1->Stop();
    thread1->quit();
    thread1->wait();
    DisconnectCamer();
    qtmysunny->init_dlg_show();
    qtmysunny->setWindowTitle(QStringLiteral("激光头设置"));
    qtmysunny->exec();
    qtmysunny->close_dlg_show();
    ConnectCamer();
    b_thread1=true;
    thread1->start();
}


void qtweldingDlg::on_setrobotBtn_clicked()//机器人设置
{
#ifdef USE_SN_DATA
    m_mcs->sn_data.save();
#endif
    thread2->Stop();
    thread2->quit();
    thread2->wait();
    DisconnectRobot();
    robotset->init_dlg_show();
    robotset->setWindowTitle(QStringLiteral("机器人设置"));
    robotset->exec();
    robotset->close_dlg_show();
    ConnectRobot();
    b_thread2=true;
    thread2->start();
}


void qtweldingDlg::on_setweldBtn_clicked()//焊机设置
{
#ifdef USE_SN_DATA
    m_mcs->sn_data.save();
#endif
    thread2->Stop();
    thread2->quit();
    thread2->wait();
    DisconnectRobot();
    weldset->init_dlg_show();
    weldset->setWindowTitle(QStringLiteral("焊机设置"));
    weldset->exec();
    weldset->close_dlg_show();
    ConnectRobot();
    b_thread2=true;
    thread2->start();
}

void qtweldingDlg::on_setplcBtn_clicked()//PLC设置
{
#ifdef USE_SN_DATA
    m_mcs->sn_data.save();
#endif
    DisconnectPLC();
    plcset->init_dlg_show();
    plcset->setWindowTitle(QStringLiteral("PLC设置"));
    plcset->exec();
    plcset->close_dlg_show();
    ConnectPLC();
}

void qtweldingDlg::on_demarcateBtn_clicked()//标定设置
{
#ifdef USE_SN_DATA
    m_mcs->sn_data.save();
#endif
    if(m_mcs->resultdata.link_result_state==false)
    {
        ui->record->append(QStringLiteral("激光头未连接成功"));
    }
    else if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("机器人未连接成功"));
    }
    else
    {
        sent_info_leaser sentdata;
        send_group_leaser.lock();
        sentdata.ctx=m_mcs->resultdata.ctx_result;
        sentdata.addr=ALS_OPEN_REG_ADD;
        sentdata.data={0xff};
        m_mcs->resultdata.b_send_group_leaser=false;
        m_mcs->resultdata.send_group_leaser.push_back(sentdata);
        m_mcs->resultdata.ctx_result_dosomeing=DO_WRITE_TASK;
        send_group_leaser.unlock();

        m_mcs->cam->sop_cam[0].DisConnect();

        demarcate->init_dlg_show();
        demarcate->setWindowTitle(QStringLiteral("标定设置"));
        demarcate->exec();
        demarcate->close_dlg_show();

        m_mcs->cam->sop_cam[0].InitConnect();
        if(m_mcs->cam->sop_cam[0].b_connect==true)
        {
            ui->record->append(QStringLiteral("激光头连接成功"));
        }
        else if(m_mcs->cam->sop_cam[0].b_connect==false)
        {
            ui->record->append(QStringLiteral("激光头连接失败"));
        }

        send_group_leaser.lock();
        sentdata.data={0x00};
        m_mcs->resultdata.b_send_group_leaser=false;
        m_mcs->resultdata.send_group_leaser.push_back(sentdata);
        m_mcs->resultdata.ctx_result_dosomeing=DO_WRITE_TASK;
        send_group_leaser.unlock();
    }
}

void qtweldingDlg::on_infoBtn_clicked() //软件信息
{
#ifdef USE_SN_DATA
    m_mcs->sn_data.save();
#endif
    info->init_dlg_show();
    info->setWindowTitle(QStringLiteral("软件信息"));
    info->exec();
    info->close_dlg_show();
}

void qtweldingDlg::on_weld_windBtn_pressed()    //送丝按下
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("焊机未连接成功"));
        return;
    }
    m_mcs->tosendbuffer->cmd_elec(WIND);
    ui->record->append(QStringLiteral("开始送丝"));
}


void qtweldingDlg::on_weld_windBtn_released()   //送丝抬起
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("焊机未连接成功"));
        return;
    }
    m_mcs->tosendbuffer->cmd_elec(STATIC);
    ui->record->append(QStringLiteral("停止送丝"));
}


void qtweldingDlg::on_weld_rewindBtn_pressed()  //退丝按下
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("焊机未连接成功"));
        return;
    }
    m_mcs->tosendbuffer->cmd_elec(REWIND);
    ui->record->append(QStringLiteral("开始退丝"));
}


void qtweldingDlg::on_weld_rewindBtn_released()  //退丝抬起
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("焊机未连接成功"));
        return;
    }
    m_mcs->tosendbuffer->cmd_elec(STATIC);
    ui->record->append(QStringLiteral("停止退丝"));
}


void qtweldingDlg::on_weld_gassBtn_pressed()    //送气按下
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("焊机未连接成功"));
        return;
    }
    m_mcs->tosendbuffer->cmd_elec(GASS);
    ui->record->append(QStringLiteral("开始送气"));
}


void qtweldingDlg::on_weld_gassBtn_released()    //送气抬起
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("焊机未连接成功"));
        return;
    }
    m_mcs->tosendbuffer->cmd_elec(STATIC);
    ui->record->append(QStringLiteral("停止送气"));
}


void qtweldingDlg::on_weld_fireBtn_pressed()    //点焊按下
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("焊机未连接成功"));
        return;
    }
    Alternatingcurrent_ID elem=(Alternatingcurrent_ID)ui->weld_elem->currentIndex();
    float eled=ui->weld_eled->text().toFloat();
    float elev=ui->weld_elev->text().toFloat();
    m_mcs->tosendbuffer->cmd_elec(eled,elev,elem,FIRE);
    ui->record->append(QStringLiteral("开始点焊"));
}


void qtweldingDlg::on_weld_fireBtn_released()   //点焊抬起
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("焊机未连接成功"));
        return;
    }
    m_mcs->tosendbuffer->cmd_elec(STATIC);
    ui->record->append(QStringLiteral("停止点焊"));
}

void qtweldingDlg::on_weld_elem_currentIndexChanged(int index)
{
    m_mcs->e2proomdata.maindlg_Weldelem=(Alternatingcurrent_ID)index;
    m_mcs->e2proomdata.write_maindlg_para();
}


void qtweldingDlg::on_weld_eled_editingFinished()
{
    m_mcs->e2proomdata.maindlg_Weldeled=ui->weld_eled->text().toFloat();
    m_mcs->e2proomdata.write_maindlg_para();
}

void qtweldingDlg::on_weld_elev_editingFinished()
{
    m_mcs->e2proomdata.maindlg_Weldelev=ui->weld_elev->text().toFloat();
    m_mcs->e2proomdata.write_maindlg_para();
}

void qtweldingDlg::on_SaveDatacheckBox_stateChanged(int arg1)//保存数据
{
    if(arg1==0)
    {
        m_mcs->e2proomdata.maindlg_SaveDatacheckBox=0;
        ui->record->append(QStringLiteral("关闭轨迹数据保存功能"));
    }
    else
    {
        m_mcs->e2proomdata.maindlg_SaveDatacheckBox=1;
        ui->record->append(QStringLiteral("开启轨迹数据保存功能"));
    }
    m_mcs->e2proomdata.write_maindlg_para();
}

void qtweldingDlg::on_CircleRuncheckBox_stateChanged(int arg1)
{
    if(arg1==0)
    {
        m_mcs->e2proomdata.maindlg_circlerun=0;
        ui->record->append(QStringLiteral("关闭循环运行功能"));
    }
    else
    {
        m_mcs->e2proomdata.maindlg_circlerun=1;
        ui->record->append(QStringLiteral("开启循环运行功能"));
    }
    m_mcs->e2proomdata.write_maindlg_para();
}

//X-按下
void qtweldingDlg::on_posXsubBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("机器人未连接成功"));
        return;
    }
    int tcp=ui->movetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->movespeed->text().toFloat(&rc);//获取到速度值
    if(ui->movespeed->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QStringLiteral("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QStringLiteral("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_XSUB;
    tcp=m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QStringLiteral("机器人X轴负移动"));
    return;
}

//X-抬起
void qtweldingDlg::on_posXsubBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QStringLiteral("机器人停止移动"));
}

//X+按下
void qtweldingDlg::on_posXaddBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("机器人未连接成功"));
        return;
    }
    int tcp=ui->movetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->movespeed->text().toFloat(&rc);//获取到速度值
    if(ui->movespeed->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QStringLiteral("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QStringLiteral("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_XADD;
    tcp=m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QStringLiteral("机器人X轴正移动"));
}

//X+抬起
void qtweldingDlg::on_posXaddBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QStringLiteral("机器人停止移动"));
}

//Y-按下
void qtweldingDlg::on_posYsubBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("机器人未连接成功"));
        return;
    }
    int tcp=ui->movetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->movespeed->text().toFloat(&rc);//获取到速度值
    if(ui->movespeed->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QStringLiteral("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QStringLiteral("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_YSUB;
    tcp=m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QStringLiteral("机器人Y轴负移动"));
    return;
}

//Y-抬起
void qtweldingDlg::on_posYsubBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QStringLiteral("机器人停止移动"));
}

//Y+按下
void qtweldingDlg::on_posYaddBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("机器人未连接成功"));
        return;
    }
    int tcp=ui->movetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->movespeed->text().toFloat(&rc);//获取到速度值
    if(ui->movespeed->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QStringLiteral("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QStringLiteral("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_YADD;
    tcp=m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QStringLiteral("机器人Y轴正移动"));
    return;
}

//Y+抬起
void qtweldingDlg::on_posYaddBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QStringLiteral("机器人停止移动"));
}

//Z-按下
void qtweldingDlg::on_posZsubBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("机器人未连接成功"));
        return;
    }
    int tcp=ui->movetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->movespeed->text().toFloat(&rc);//获取到速度值
    if(ui->movespeed->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QStringLiteral("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QStringLiteral("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_ZSUB;
    tcp=m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QStringLiteral("机器人Z轴负移动"));
    return;
}

//Z-抬起
void qtweldingDlg::on_posZsubBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QStringLiteral("机器人停止移动"));
}

//Z+按下
void qtweldingDlg::on_posZaddBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("机器人未连接成功"));
        return;
    }
    int tcp=ui->movetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->movespeed->text().toFloat(&rc);//获取到速度值
    if(ui->movespeed->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QStringLiteral("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QStringLiteral("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_ZADD;
    tcp=m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QStringLiteral("机器人Z轴正移动"));
    return;
}

//Z+抬起
void qtweldingDlg::on_posZaddBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QStringLiteral("机器人停止移动"));
}

//RX-按下
void qtweldingDlg::on_posRXsubBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("机器人未连接成功"));
        return;
    }
    int tcp=ui->movetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->movespeed->text().toFloat(&rc);//获取到速度值
    if(ui->movespeed->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QStringLiteral("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QStringLiteral("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_RXSUB;
    tcp=m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QStringLiteral("机器人RX轴负移动"));
    return;
}

//RX-抬起
void qtweldingDlg::on_posRXsubBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QStringLiteral("机器人停止移动"));
}

//RX+按下
void qtweldingDlg::on_posRXaddBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("机器人未连接成功"));
        return;
    }
    int tcp=ui->movetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->movespeed->text().toFloat(&rc);//获取到速度值
    if(ui->movespeed->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QStringLiteral("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QStringLiteral("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_RXADD;
    tcp=m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QStringLiteral("机器人RX轴正移动"));
    return;
}

//RX+抬起
void qtweldingDlg::on_posRXaddBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QStringLiteral("机器人停止移动"));
}

//RY-按下
void qtweldingDlg::on_posRYsubBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("机器人未连接成功"));
        return;
    }
    int tcp=ui->movetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->movespeed->text().toFloat(&rc);//获取到速度值
    if(ui->movespeed->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QStringLiteral("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QStringLiteral("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_RYSUB;
    tcp=m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QStringLiteral("机器人RY轴负移动"));
    return;
}

//RY-抬起
void qtweldingDlg::on_posRYsubBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QStringLiteral("机器人停止移动"));
}

//RY+按下
void qtweldingDlg::on_posRYaddBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("机器人未连接成功"));
        return;
    }
    int tcp=ui->movetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->movespeed->text().toFloat(&rc);//获取到速度值
    if(ui->movespeed->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QStringLiteral("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QStringLiteral("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_RYADD;
    tcp=m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QStringLiteral("机器人RY轴正移动"));
    return;
}

//RY+抬起
void qtweldingDlg::on_posRYaddBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QStringLiteral("机器人停止移动"));
}

//RZ-按下
void qtweldingDlg::on_posRZsubBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("机器人未连接成功"));
        return;
    }
    int tcp=ui->movetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->movespeed->text().toFloat(&rc);//获取到速度值
    if(ui->movespeed->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QStringLiteral("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QStringLiteral("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_RZSUB;
    tcp=m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QStringLiteral("机器人RZ轴负移动"));
    return;
}

//RZ-抬起
void qtweldingDlg::on_posRZsubBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QStringLiteral("机器人停止移动"));
}

//RZ+按下
void qtweldingDlg::on_posRZaddBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("机器人未连接成功"));
        return;
    }
    int tcp=ui->movetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->movespeed->text().toFloat(&rc);//获取到速度值
    if(ui->movespeed->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QStringLiteral("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QStringLiteral("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_RZADD;
    tcp=m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QStringLiteral("机器人RZ轴正移动"));
    return;
}

//RZ+抬起
void qtweldingDlg::on_posRZaddBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QStringLiteral("机器人停止移动"));
}

//OUT1-按下
void qtweldingDlg::on_posOut1subBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("机器人未连接成功"));
        return;
    }
    int tcp=ui->movetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->movespeed->text().toFloat(&rc);//获取到速度值
    if(ui->movespeed->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QStringLiteral("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QStringLiteral("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_OUT1SUB;
    tcp=m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QStringLiteral("机器人OUT1轴负移动"));
    return;
}

//OUT1-抬起
void qtweldingDlg::on_posOut1subBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QStringLiteral("机器人停止移动"));
}

//OUT1+按下
void qtweldingDlg::on_posOut1addBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("机器人未连接成功"));
        return;
    }
    int tcp=ui->movetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->movespeed->text().toFloat(&rc);//获取到速度值
    if(ui->movespeed->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QStringLiteral("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QStringLiteral("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_OUT1ADD;
    tcp=m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QStringLiteral("机器人OUT1轴正移动"));
    return;
}

//OUT1+抬起
void qtweldingDlg::on_posOut1addBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QStringLiteral("机器人停止移动"));
}

//OUT2-按下
void qtweldingDlg::on_posOut2subBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("机器人未连接成功"));
        return;
    }
    int tcp=ui->movetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->movespeed->text().toFloat(&rc);//获取到速度值
    if(ui->movespeed->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QStringLiteral("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QStringLiteral("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_OUT2SUB;
    tcp=m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QStringLiteral("机器人OUT2轴负移动"));
    return;
}

//OUT2-抬起
void qtweldingDlg::on_posOut2subBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QStringLiteral("机器人停止移动"));
}

//OUT2+按下
void qtweldingDlg::on_posOut2addBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("机器人未连接成功"));
        return;
    }
    int tcp=ui->movetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->movespeed->text().toFloat(&rc);//获取到速度值
    if(ui->movespeed->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QStringLiteral("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QStringLiteral("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_OUT2ADD;
    tcp=m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QStringLiteral("机器人OUT2轴正移动"));
    return;
}

//OUT2+抬起
void qtweldingDlg::on_posOut2addBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QStringLiteral("机器人停止移动"));
}

//OUT3-按下
void qtweldingDlg::on_posOut3subBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("机器人未连接成功"));
        return;
    }
    int tcp=ui->movetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->movespeed->text().toFloat(&rc);//获取到速度值
    if(ui->movespeed->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QStringLiteral("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QStringLiteral("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_OUT3SUB;
    tcp=m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QStringLiteral("机器人OUT3轴负移动"));
    return;
}

//OUT3-抬起
void qtweldingDlg::on_posOut3subBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QStringLiteral("机器人停止移动"));
}

//OUT3+按下
void qtweldingDlg::on_posOut3addBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("机器人未连接成功"));
        return;
    }
    int tcp=ui->movetcpcombo->currentIndex();//获取到移动TCP
    bool rc;
    float f_speed=ui->movespeed->text().toFloat(&rc);//获取到速度值
    if(ui->movespeed->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QStringLiteral("移动速度格式出错"));
        return;
    }
    if(m_mcs->mainDlg_robotEnable==false)
    {
        ui->record->append(QStringLiteral("请打勾机器人使能"));
        return;
    }
    m_mcs->e2proomdata.maindlg_movespeed=f_speed;
    m_mcs->e2proomdata.maindlg_movetcp=tcp;
    m_mcs->e2proomdata.write_maindlg_para();
    m_mcs->mainDlg_robmovestate=MAINDLG_OUT3ADD;
    tcp=m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
    if(0!=task_setmaindlgtcp(tcp))
        return;
    ui->record->append(QStringLiteral("机器人OUT3轴正移动"));
    return;
}

//OUT3+抬起
void qtweldingDlg::on_posOut3addBtn_released()
{
    m_mcs->mainDlg_robmovestate=MAINDLG_NOTMOVE;
    ui->record->append(QStringLiteral("机器人停止移动"));
}

void qtweldingDlg::on_robotEncheckBox_stateChanged(int arg1)
{
    if(arg1==0)
    {
        if(m_mcs->rob->b_connect==false)
        {
            ui->record->append(QStringLiteral("机器人通信异常"));
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
        ui->record->append(QStringLiteral("机器人关闭使能"));
    }
    else
    {
        if(m_mcs->rob->b_connect==false)
        {
            ui->record->append(QStringLiteral("机器人通信异常"));
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
        ui->record->append(QStringLiteral("机器人开启使能"));
    }
}

void qtweldingDlg::ConnectCamer()
{
    if(m_mcs->resultdata.link_result_state==false)
    {
        QString server_ip=m_mcs->ip->camer_ip[0].ip;
        QString server_port2=QString::number(PORT_ALS_RESULT);
        m_mcs->resultdata.ctx_result = modbus_new_tcp(server_ip.toUtf8(), server_port2.toInt());
        if (modbus_connect(m_mcs->resultdata.ctx_result) == -1)
        {
            ui->record->append(server_port2+QStringLiteral("端口连接失败"));
            modbus_free(m_mcs->resultdata.ctx_result);
            return;
        }
        m_mcs->resultdata.link_result_state=true;
        ui->record->append(server_port2+QStringLiteral("端口连接成功"));
        m_mcs->cam->sop_cam[0].InitConnect();
    //  RunAlgCamer();
    }
}

void qtweldingDlg::DisconnectCamer()
{
    if(m_mcs->resultdata.link_result_state==true)
    {
        m_mcs->cam->sop_cam[0].DisConnect();
        if(b_RunAlgCamer==true)
        {
            StopAlgCamer();
        }
        modbus_free(m_mcs->resultdata.ctx_result);
        m_mcs->resultdata.link_result_state=false;
        QString msg=QString::number(PORT_ALS_RESULT);
        ui->record->append(msg+QStringLiteral("端口关闭"));
    }
}

void qtweldingDlg::ConnectPLC()
{
    switch(m_mcs->rob->plc_model)
    {
        case PLC_MODEL_NULL:
        {
        }
        break;
        case PLC_MODEL_MODBUSTCP:
        {
            if(0==m_mcs->rob->ConnectPLC(m_mcs->ip->robot_ip[0].plc_ip.ip,m_mcs->ip->robot_ip[0].plc_ip.port))
            {
                ui->record->append(QStringLiteral("与PLC连接成功"));
            }
            else
            {
                ui->record->append(QStringLiteral("与PLC连接失败"));
            }
        }
        break;
    }
}

void qtweldingDlg::DisconnectPLC()
{
    switch(m_mcs->rob->plc_model)
    {
        case PLC_MODEL_NULL:
        {
        }
        break;
        case PLC_MODEL_MODBUSTCP:
        {
            m_mcs->rob->DisconnectPLC();
            ui->record->append(QStringLiteral("与PLC断开连接"));
        }
        break;
    }
}

void qtweldingDlg::RunAlgCamer()
{
    if(m_mcs->resultdata.link_result_state==true)
    {
        uint16_t tab_reg[1];
        tab_reg[0]=0xff;
        int rc=modbus_write_registers(m_mcs->resultdata.ctx_result,ALS_OPEN_REG_ADD,1,tab_reg);
        if(rc!=1)
        {
            ui->record->append(QStringLiteral("激光头相机启动设置失败"));
        }
        else
        {
            ui->record->append(QStringLiteral("激光头相机启动设置成功"));
            if(b_RunAlgCamer==false)
            {
                m_mcs->cam->sop_cam[0].InitConnect();

                if(m_mcs->cam->sop_cam[0].b_connect==true)
                {
                    ui->record->append(QStringLiteral("激光头连接成功"));
                    b_RunAlgCamer=true;
                }
                else if(m_mcs->cam->sop_cam[0].b_connect==false)
                {
                    ui->record->append(QStringLiteral("激光头连接失败"));
                }
            }
        }
    }
}

void qtweldingDlg::StopAlgCamer()
{
    if(m_mcs->resultdata.link_result_state==true)
    {
        if(b_RunAlgCamer==true)
        {
            m_mcs->cam->sop_cam[0].DisConnect();
            ui->record->append(QStringLiteral("相机关闭"));
            b_RunAlgCamer=false;
        }
        uint16_t tab_reg[1];
        tab_reg[0]=0;
        int rc=modbus_write_registers(m_mcs->resultdata.ctx_result,ALS_OPEN_REG_ADD,1,tab_reg);
        if(rc!=1)
        {
            ui->record->append(QStringLiteral("激光头相机关闭设置失败"));
        }
        else
        {
            ui->record->append(QStringLiteral("激光头相机关闭设置成功"));
        }
    }
}

void qtweldingDlg::ConnectRobot()
{
    if(0==m_mcs->rob->ConnectRobot(m_mcs->ip->robot_ip[0].robot_ip.ip,m_mcs->ip->robot_ip[0].robot_ip.port))
    {
        ui->record->append(QStringLiteral("与机器人连接成功"));
        //写入机器人型号和远程地址
        uint16_t u16_data[5];
        u16_data[0]=m_mcs->rob->robot_model;
        int rc=modbus_write_registers(m_mcs->rob->ctx_posget,ROB_MODEL_REG_ADD,1,u16_data);
        if(rc!=1)
        {
            ui->record->append(QStringLiteral("机器人型号设置失败"));
        }
        QStringList sections=m_mcs->ip->robot_ip[0].remote_ip.ip.split(".");
        if(sections.size()==4)
        {
            for(int n=0;n<sections.size();n++)
            {
                u16_data[n]=sections[n].toInt();
            }
            rc=modbus_write_registers(m_mcs->rob->ctx_posget,ROB_IPADDR_1_REG_ADD,4,u16_data);
            if(rc!=4)
            {
                ui->record->append(QStringLiteral("机器人远程IP地址设置失败"));
            }
        }
        else
        {
            ui->record->append(QStringLiteral("机器人远程IP地址格式错误"));
        }
        //写入TCP
        u16_data[0]=m_mcs->e2proomdata.maindlg_movetcp;
        rc=modbus_write_registers(m_mcs->rob->ctx_posget,ROB_TCP_NUM_REG_ADD,1,u16_data);
        if(rc!=1)
        {
            ui->record->append(QStringLiteral("机器人TCP设置失败"));
        }
        //写入外部轴数量
        u16_data[0]=m_mcs->rob->out_num;
        rc=modbus_write_registers(m_mcs->rob->ctx_posget,ROB_OUTNUM_REG_ADD,1,u16_data);
        if(rc!=1)
        {
            ui->record->append(QStringLiteral("机器人外部轴设置失败"));
        }
        u16_data[0]=0;
        rc=modbus_write_registers(m_mcs->rob->ctx_posget,ROB_STOP_REG_ADD,1,u16_data);
        if(rc!=1)
        {
            ui->record->append(QStringLiteral("机器人启停设置失败"));
        }
        //写入焊机型号和远程地址
        u16_data[0]=m_mcs->rob->weld_model;
        rc=modbus_write_registers(m_mcs->rob->ctx_posget,ROB_WELD_MODEL_REG_ADD,1,u16_data);
        if(rc!=1)
        {
            ui->record->append(QStringLiteral("焊机型号设置失败"));
        }
        sections=m_mcs->ip->robot_ip[0].weld_ip.ip.split(".");
        if(sections.size()==4)
        {
            for(int n=0;n<sections.size();n++)
            {
                u16_data[n]=sections[n].toInt();
            }
            rc=modbus_write_registers(m_mcs->rob->ctx_posget,ROB_WELD_IPADDR_1_REG_ADD,4,u16_data);
            if(rc!=4)
            {
                ui->record->append(QStringLiteral("焊机远程IP地址设置失败"));
            }
        }
        else
        {
            ui->record->append(QStringLiteral("焊机远程IP地址格式错误"));
        }
        /**********************/
        //启动时使能
        u16_data[0]=1;
        rc=modbus_write_registers(m_mcs->rob->ctx_posget,ROB_TOLTAL_CONTROL_REG_ADD,1,u16_data);
        if(rc!=1)
        {
            ui->record->append(QStringLiteral("机器人控制设置失败"));
        }
        m_mcs->mainDlg_robotEnable=true;
        ui->robotEncheckBox->setCheckState(Qt::Checked);
        /**********************/
    }
    else
    {
        ui->record->append(QStringLiteral("与机器人连接失败"));
    }
}

void qtweldingDlg::DisconnectRobot()
{
    m_mcs->rob->DisconnectRobot();
    ui->record->append(QStringLiteral("与机器人断开连接"));
}

void qtweldingDlg::init_show_ui_list()//界面刷新
{
    QString msg;
    //工程信息
    ui->project_name->setText(m_mcs->project->project_name);
    ui->project_Id->setText(m_mcs->project->project_Id_toQString(m_mcs->project->project_Id));

    switch(m_mcs->project->project_Id)
    {
        case PROGECT_ID_TEACH_SCAN:
            msg=QString::number(m_mcs->process->process1_scanbeforetrace->buildline)+"/"+QString::number(m_mcs->project->project_cmdlist.size());
            ui->project_buildstep->setText(msg);
        break;
    }

    //相机信息
    ui->leaser_ip->setText(m_mcs->ip->camer_ip[0].ip);

    ui->leaser_state->setText("0x"+QString::number(m_mcs->resultdata.state,16));
    ui->leaser_pos_y->setText(QString::number(m_mcs->resultdata.pos1.Y,'f',2));
    ui->leaser_pos_z->setText(QString::number(m_mcs->resultdata.pos1.Z,'f',2));


    msg=QString::number(m_mcs->resultdata.leaser_time.hour)+":"+
                QString::number(m_mcs->resultdata.leaser_time.min)+":"+
                QString::number(m_mcs->resultdata.leaser_time.sec)+":"+
                QString::number(m_mcs->resultdata.leaser_time.msec);
    ui->leaser_timestamp->setText(msg);
    msg=QString::number(m_mcs->resultdata.time_stamp.hour)+":"+
                QString::number(m_mcs->resultdata.time_stamp.min)+":"+
                QString::number(m_mcs->resultdata.time_stamp.sec)+":"+
                QString::number(m_mcs->resultdata.time_stamp.msec);
    ui->leaser_time->setText(msg);

    ui->leaser_camera_fps->setText(QString::number(m_mcs->resultdata.camfps,'f',2));
    ui->leaser_result_fps->setText(QString::number(m_mcs->resultdata.fps,'f',2));
    ui->leaser_tasknum->setText(QString::number(m_mcs->resultdata.task));

    b_init_show_ui_list=true;
}

void qtweldingDlg::init_sent_leaser()
{
    if(m_mcs->resultdata.b_send_group_leaser==false)
    {
        ui->record->append(QStringLiteral("激光头通信异常"));
    }
    b_init_sent_leaser=true;
}

void qtweldingDlg::init_show_robpos_list()
{
    //机器人信息
    QString msg;

    ui->robot_model->setText(m_mcs->rob->robot_model_toQString(m_mcs->rob->robot_model));
    ui->robot_state->setText(m_mcs->rob->robot_state_toQString(m_mcs->rob->robot_state));

    ui->robot_speed->setText(QString::number(m_mcs->rob->robot_speed,'f',3));
    ui->robot_pos_x->setText(QString::number(m_mcs->rob->TCPpos.X,'f',ROBOT_POSE_DECIMAL_PLACE));
    ui->robot_pos_y->setText(QString::number(m_mcs->rob->TCPpos.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
    ui->robot_pos_z->setText(QString::number(m_mcs->rob->TCPpos.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
    ui->robot_pos_rx->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    ui->robot_pos_ry->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    ui->robot_pos_rz->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    msg=QString::number(m_mcs->rob->robtime.hour)+":"+
                QString::number(m_mcs->rob->robtime.min)+":"+
                QString::number(m_mcs->rob->robtime.sec)+":"+
                QString::number(m_mcs->rob->robtime.msec);
    ui->robot_time->setText(msg);

    ui->weld_model->setText(m_mcs->rob->weld_model_toQString(m_mcs->rob->weld_model));


    if(m_mcs->rob->robot_model==ROBOT_MODEL_NULL)
    {
        ui->robot_ip_port->setText(QStringLiteral("无"));
    }
    else
    {
        ui->robot_ip_port->setText(m_mcs->ip->robot_ip[0].remote_ip.ip);
    }
    if(m_mcs->rob->weld_model==WELD_MODEL_NULL||m_mcs->rob->weld_model==WELD_MODEL_ROBOT_LINK)
    {
        ui->weld_ip_port->setText(QStringLiteral("无"));
    }
    else
    {
        ui->weld_ip_port->setText(m_mcs->ip->robot_ip[0].weld_ip.ip);
    }
    ui->weld_current->setText(QString::number(m_mcs->rob->weld_eled,'f',3));
    ui->weld_current_v->setText(QString::number(m_mcs->rob->weld_elev,'f',3));
    ui->weld_alternating->setText(m_mcs->rob->weldwork_elem_toQString(m_mcs->rob->weld_elem));
    ui->weld_state->setText(m_mcs->rob->weldwork_model_toQString(m_mcs->rob->weld_state));
    b_init_show_robpos_list=true;
}

void qtweldingDlg::init_set_robtask()
{
    if(m_mcs->rob->b_send_group_robot==false)
    {
        ui->record->append(QStringLiteral("机器人通信异常"));
    }
    b_init_set_robtask=true;
}
/*
void qtweldingDlg::init_set_plctask()
{
    if(m_mcs->rob->b_send_group_plc==false)
    {
        ui->record->append(QStringLiteral("PLC通信异常"));
    }
    b_init_set_plctask=true;
}
*/
void qtweldingDlg::init_show_record_list(QString msg)
{
    ui->record->append(msg);
    b_init_show_record_list=true;
}

#ifdef USE_SN_DATA
void qtweldingDlg::init_SNtimeoutShow()//每分钟进一次中断
{
    if(m_mcs->sn_data.nLeftHours!=SN_TIME_SN)
    {
        if(m_mcs->sn_data.nLeftHours>0)
        {
            m_mcs->sn_data.nLeftHours--;
        }
    /*
        static int times=0;
        times++;
        if(times==30)   //每30分钟自动存一次
        {
            times=0;
            m_mcs->sn_data.save();
        }
    */
    }
}
#endif

int qtweldingDlg::task_setmaindlgtcp(int tcp)
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
        ui->record->append(QStringLiteral("机器人TCP设置异常"));
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
        ui->record->append(QStringLiteral("获取机器人坐标失败"));
        return 1;
    }
    return 0;
}


qtweldingThread::qtweldingThread(qtweldingDlg *statci_p)
{
    _p=statci_p;
}

void qtweldingThread::run()
{
    while (1)
    {
        if(_p->b_thread1==true)
        {
            if(_p->m_mcs->resultdata.link_result_state==true)
            {
                if(_p->m_mcs->resultdata.ctx_result_dosomeing==DO_WRITE_TASK)
                {
                    send_group_leaser.lock();
                    if(_p->m_mcs->resultdata.send_group_leaser.size()!=0)
                    {
                        sent_info_leaser sentdata=_p->m_mcs->resultdata.send_group_leaser[0];
                        std::vector<sent_info_leaser>::iterator it = _p->m_mcs->resultdata.send_group_leaser.begin();
                        _p->m_mcs->resultdata.send_group_leaser.erase(it);
                        int rc=modbus_write_registers(sentdata.ctx,sentdata.addr,sentdata.data.size(),sentdata.data.data());
                        if(rc!=sentdata.data.size())
                        {
                            _p->m_mcs->resultdata.b_send_group_leaser=false;
                            if(_p->b_init_sent_leaser==true)
                            {
                                _p->b_init_sent_leaser=false;
                                emit Send_sent_leaser();
                            }
                        }
                        else
                        {
                            _p->m_mcs->resultdata.b_send_group_leaser=true;
                        }
                        if(_p->m_mcs->resultdata.send_group_leaser.size()==0)
                        {
                            _p->m_mcs->resultdata.ctx_result_dosomeing=DO_NOTHING;
                        }
                    }
                    else
                    {
                        _p->m_mcs->resultdata.ctx_result_dosomeing=DO_NOTHING;
                    }
                    send_group_leaser.unlock();
                }
                if(_p->m_mcs->resultdata.ctx_result_dosomeing==DO_NOTHING)
                {
                    if(0<=modbus_read_registers(_p->m_mcs->resultdata.ctx_result,ALS_STATE_REG_ADD,15,_p->leaser_rcv_data))
                    {
                        float Y=(int16_t)_p->leaser_rcv_data[1]/100.0;
                        float Z=(int16_t)_p->leaser_rcv_data[2]/100.0;
                        _p->m_mcs->resultdata.pos1.Y=Y;
                        _p->m_mcs->resultdata.pos1.Z=Z;
                        _p->m_mcs->resultdata.time_stamp.hour=(int16_t)_p->leaser_rcv_data[5];
                        _p->m_mcs->resultdata.time_stamp.min=(int16_t)_p->leaser_rcv_data[6];
                        _p->m_mcs->resultdata.time_stamp.sec=(int16_t)_p->leaser_rcv_data[7];
                        _p->m_mcs->resultdata.time_stamp.msec=(int16_t)_p->leaser_rcv_data[8];
                        _p->m_mcs->resultdata.leaser_time.hour=(int16_t)_p->leaser_rcv_data[11];
                        _p->m_mcs->resultdata.leaser_time.min=(int16_t)_p->leaser_rcv_data[12];
                        _p->m_mcs->resultdata.leaser_time.sec=(int16_t)_p->leaser_rcv_data[13];
                        _p->m_mcs->resultdata.leaser_time.msec=(int16_t)_p->leaser_rcv_data[14];
                        _p->m_mcs->resultdata.fps=(float)_p->leaser_rcv_data[9]/100.0;
                        _p->m_mcs->resultdata.camfps=(float)_p->leaser_rcv_data[10]/100.0;
                        _p->m_mcs->resultdata.state=_p->leaser_rcv_data[0];
                        if(_p->m_mcs->resultdata.state==0)
                        {
                            _p->m_mcs->resultdata.pos1.nEn=false;
                        }
                        else
                        {
                            _p->m_mcs->resultdata.pos1.nEn=true;
                        }
                    }
                    if(0<=modbus_read_registers(_p->m_mcs->resultdata.ctx_result,ALS_Y_POINT2_REG_ADD,4,_p->leaser_rcv_data2))
                    {
                        float Y2=(int16_t)_p->leaser_rcv_data2[0]/100.0;
                        float Z2=(int16_t)_p->leaser_rcv_data2[1]/100.0;
                        float Y3=(int16_t)_p->leaser_rcv_data2[2]/100.0;
                        float Z3=(int16_t)_p->leaser_rcv_data2[3]/100.0;
                        _p->m_mcs->resultdata.pos2.Y=Y2;
                        _p->m_mcs->resultdata.pos2.Z=Z2;
                        _p->m_mcs->resultdata.pos3.Y=Y3;
                        _p->m_mcs->resultdata.pos3.Z=Z3;
                        if(_p->m_mcs->resultdata.state==0)
                        {
                            _p->m_mcs->resultdata.pos2.nEn=false;
                            _p->m_mcs->resultdata.pos3.nEn=false;
                        }
                        else
                        {
                            _p->m_mcs->resultdata.pos2.nEn=true;
                            _p->m_mcs->resultdata.pos3.nEn=true;
                        }
                    }
                    if(0<=modbus_read_registers(_p->m_mcs->resultdata.ctx_result,ALS_TASKNUM_REG_ADD,1,_p->leaser_rcv_data3))
                    {
                        int tasknum=(int16_t)_p->leaser_rcv_data3[0];
                        _p->m_mcs->resultdata.task=tasknum;
                    }
                    /***************************/
                #if OPEN_TIMESTAMP==1
                    //往激光器写入机器人实时坐标信息
                    if(_p->m_mcs->rob->b_link_ctx_posget==true)
                    {
                        sent_info_leaser sentdata;
                        int32_t *i32_data;
                        uint16_t u16_data[2];
                        sentdata.ctx=_p->m_mcs->resultdata.ctx_result;
                        sentdata.addr=ALS_REALTIME_POSX_REG_ADD;
                        sentdata.data.reserve(0x15);
                        i32_data=(int32_t*)u16_data;
                        *i32_data=_p->m_mcs->rob->TCPpos.X*1000;
                        sentdata.data.push_back(u16_data[0]);
                        sentdata.data.push_back(u16_data[1]);
                        *i32_data=_p->m_mcs->rob->TCPpos.Y*1000;
                        sentdata.data.push_back(u16_data[0]);
                        sentdata.data.push_back(u16_data[1]);
                        *i32_data=_p->m_mcs->rob->TCPpos.Z*1000;
                        sentdata.data.push_back(u16_data[0]);
                        sentdata.data.push_back(u16_data[1]);
                        *i32_data=_p->m_mcs->rob->TCPpos.RX*10000;
                        sentdata.data.push_back(u16_data[0]);
                        sentdata.data.push_back(u16_data[1]);
                        *i32_data=_p->m_mcs->rob->TCPpos.RY*10000;
                        sentdata.data.push_back(u16_data[0]);
                        sentdata.data.push_back(u16_data[1]);
                        *i32_data=_p->m_mcs->rob->TCPpos.RZ*10000;
                        sentdata.data.push_back(u16_data[0]);
                        sentdata.data.push_back(u16_data[1]);
                        *i32_data=_p->m_mcs->rob->TCPpos.out_1;
                        sentdata.data.push_back(u16_data[0]);
                        sentdata.data.push_back(u16_data[1]);
                        *i32_data=_p->m_mcs->rob->TCPpos.out_2;
                        sentdata.data.push_back(u16_data[0]);
                        sentdata.data.push_back(u16_data[1]);
                        *i32_data=_p->m_mcs->rob->TCPpos.out_3;
                        sentdata.data.push_back(u16_data[0]);
                        sentdata.data.push_back(u16_data[1]);
                        //工具号、坐标系、用户坐标系全用0
                        sentdata.data.push_back(0);
                        sentdata.data.push_back(0);
                        sentdata.data.push_back(0);
                        int rc=modbus_write_registers(sentdata.ctx,sentdata.addr,sentdata.data.size(),sentdata.data.data());
                        if(rc!=sentdata.data.size())
                        {
                            _p->m_mcs->resultdata.b_send_group_leaser=false;
                            if(_p->b_init_sent_leaser==true)
                            {
                                _p->b_init_sent_leaser=false;
                                emit Send_sent_leaser();
                            }
                        }
                        else
                        {
                            _p->m_mcs->resultdata.b_send_group_leaser=true;
                        }
                    }
                #endif
                    /**********************************/
                }
            }
            if(_p->b_init_show_ui_list==true)
            {
                _p->b_init_show_ui_list=false;
                emit Send_show_ui_list();
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

void qtweldingThread::Stop()
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

qtgetrobThread::qtgetrobThread(qtweldingDlg *statci_p)
{
    _p=statci_p;
}

void qtgetrobThread::run()
{
    while (1)
    {
        if(_p->b_thread2==true)
        {
            if(_p->m_mcs->rob->b_connect==true)
            {
                if(_p->m_mcs->rob->ctx_robot_dosomeing==DO_WRITE_TASK)
                {
                    send_group_robot.lock();
                    if(_p->m_mcs->rob->send_group_robot.size()!=0)
                    {
                        sent_info_robot sentdata=_p->m_mcs->rob->send_group_robot[0];
                        std::vector<sent_info_robot>::iterator it = _p->m_mcs->rob->send_group_robot.begin();
                        _p->m_mcs->rob->send_group_robot.erase(it);
                        int rc=modbus_write_registers(sentdata.ctx,sentdata.addr,sentdata.data.size(),sentdata.data.data());
                        if(rc!=sentdata.data.size())
                        {
                            _p->m_mcs->rob->b_send_group_robot=false;
                            if(_p->b_init_set_robtask==true)
                            {
                                _p->b_init_set_robtask=false;
                                emit Send_set_robtask();
                            }
                        }
                        else
                        {
                            _p->m_mcs->rob->b_send_group_robot=true;
                        }
                        if(_p->m_mcs->rob->send_group_robot.size()==0)
                        {
                            _p->m_mcs->rob->ctx_robot_dosomeing=DO_NOTHING;
                        }
                    }
                    else
                    {
                        _p->m_mcs->rob->ctx_robot_dosomeing=DO_NOTHING;
                    }
                    send_group_robot.unlock();
                }
                if(_p->m_mcs->rob->ctx_robot_dosomeing==DO_NOTHING)
                {
                //访问机器人坐标通信
                    if(0<=modbus_read_registers(_p->m_mcs->rob->ctx_posget,ROB_CONNECT_STATE_REG_ADD,21+ROBOTINPUTNUM+ROBOTTCPPOSOUTNUM*2,_p->robotpos_rcv_data))
                    {
                        if(_p->robotpos_rcv_data[0]==0)
                            _p->m_mcs->rob->b_link_ctx_posget_remote=false;
                        else
                            _p->m_mcs->rob->b_link_ctx_posget_remote=true;
                        _p->m_mcs->rob->TCPpos.X=*((float*)&_p->robotpos_rcv_data[2]);
                        _p->m_mcs->rob->TCPpos.Y=*((float*)&_p->robotpos_rcv_data[4]);
                        _p->m_mcs->rob->TCPpos.Z=*((float*)&_p->robotpos_rcv_data[6]);
                        _p->m_mcs->rob->TCPpos.RX=*((float*)&_p->robotpos_rcv_data[8]);
                        _p->m_mcs->rob->TCPpos.RY=*((float*)&_p->robotpos_rcv_data[10]);
                        _p->m_mcs->rob->TCPpos.RZ=*((float*)&_p->robotpos_rcv_data[12]);
                        _p->m_mcs->rob->TCPpos.nEn=true;
                        _p->m_mcs->rob->robot_speed=*((float*)&_p->robotpos_rcv_data[14]);
                        _p->m_mcs->rob->robot_state=(ROBOT_STATE)_p->robotpos_rcv_data[16];
                        _p->m_mcs->rob->robtime.hour=(int16_t)_p->robotpos_rcv_data[17];
                        _p->m_mcs->rob->robtime.min=(int16_t)_p->robotpos_rcv_data[18];
                        _p->m_mcs->rob->robtime.sec=(int16_t)_p->robotpos_rcv_data[19];
                        _p->m_mcs->rob->robtime.msec=(int16_t)_p->robotpos_rcv_data[20];
                        for(int n=0;n<ROBOTINPUTNUM;n++)
                        {
                            _p->m_mcs->rob->robioinput[n]=(int16_t)_p->robotpos_rcv_data[21+n];
                        }
                        _p->m_mcs->rob->TCPpos.out_1=*((int32_t*)&_p->robotpos_rcv_data[21+ROBOTINPUTNUM]);
                        _p->m_mcs->rob->TCPpos.out_2=*((int32_t*)&_p->robotpos_rcv_data[21+ROBOTINPUTNUM+2]);
                        _p->m_mcs->rob->TCPpos.out_3=*((int32_t*)&_p->robotpos_rcv_data[21+ROBOTINPUTNUM+4]);
                    }

                    /*****************************/
                    //主页按钮是否要移动机器人
                    if(_p->m_mcs->mainDlg_robmovestate!=MAINDLG_STATIC)
                    {
                        static RobPos targetpos;//移动前往的目标点
                        static int tcp;
                        static float speed;
                        Robmovemodel movemod=MOVEJ;
                        if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_NOTMOVE)//按钮停止机器人
                        {
                        #ifdef USE_MYROBOT_CONTROL
                            _p->m_mcs->robotcontrol->clear_movepoint_buffer();//清空运动后续缓存
                        #endif
                            _p->m_mcs->tosendbuffer->cmd_lock(0);//停止运动
                            _p->m_mcs->mainDlg_robmovestate=MAINDLG_STATIC;
                        }
                        else
                        {
                            if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_XSUB)
                            {
                                tcp=_p->m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
                                speed=_p->m_mcs->e2proomdata.maindlg_movespeed;//获取到速度值
                                targetpos=_p->m_mcs->rob->TCPpos;
                                targetpos.X=targetpos.X-ROBOT_MAINDLG_MOVEPOS_STEP;
                                _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                _p->m_mcs->mainDlg_robmovestate=MAINDLG_XSUB_ING;
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_XADD)
                            {
                                tcp=_p->m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
                                speed=_p->m_mcs->e2proomdata.maindlg_movespeed;//获取到速度值
                                targetpos=_p->m_mcs->rob->TCPpos;
                                targetpos.X=targetpos.X+ROBOT_MAINDLG_MOVEPOS_STEP;
                                _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                _p->m_mcs->mainDlg_robmovestate=MAINDLG_XADD_ING;
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_YSUB)
                            {
                                tcp=_p->m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
                                speed=_p->m_mcs->e2proomdata.maindlg_movespeed;//获取到速度值
                                targetpos=_p->m_mcs->rob->TCPpos;
                                targetpos.Y=targetpos.Y-ROBOT_MAINDLG_MOVEPOS_STEP;
                                _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                _p->m_mcs->mainDlg_robmovestate=MAINDLG_YSUB_ING;
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_YADD)
                            {
                                tcp=_p->m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
                                speed=_p->m_mcs->e2proomdata.maindlg_movespeed;//获取到速度值
                                targetpos=_p->m_mcs->rob->TCPpos;
                                targetpos.Y=targetpos.Y+ROBOT_MAINDLG_MOVEPOS_STEP;
                                _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                _p->m_mcs->mainDlg_robmovestate=MAINDLG_YADD_ING;
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_ZSUB)
                            {
                                tcp=_p->m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
                                speed=_p->m_mcs->e2proomdata.maindlg_movespeed;//获取到速度值
                                targetpos=_p->m_mcs->rob->TCPpos;
                                targetpos.Z=targetpos.Z-ROBOT_MAINDLG_MOVEPOS_STEP;
                                _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                _p->m_mcs->mainDlg_robmovestate=MAINDLG_ZSUB_ING;
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_ZADD)
                            {
                                tcp=_p->m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
                                speed=_p->m_mcs->e2proomdata.maindlg_movespeed;//获取到速度值
                                targetpos=_p->m_mcs->rob->TCPpos;
                                targetpos.Z=targetpos.Z+ROBOT_MAINDLG_MOVEPOS_STEP;
                                _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                _p->m_mcs->mainDlg_robmovestate=MAINDLG_ZADD_ING;
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_RXSUB)
                            {
                                tcp=_p->m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
                                speed=_p->m_mcs->e2proomdata.maindlg_movespeed;//获取到速度值
                                targetpos=_p->m_mcs->rob->TCPpos;
                                targetpos.RX=targetpos.RX-ROBOT_MAINDLG_MOVEPOSTURE_STEP;
                                _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                _p->m_mcs->mainDlg_robmovestate=MAINDLG_RXSUB_ING;
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_RXADD)
                            {
                                tcp=_p->m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
                                speed=_p->m_mcs->e2proomdata.maindlg_movespeed;//获取到速度值
                                targetpos=_p->m_mcs->rob->TCPpos;
                                targetpos.RX=targetpos.RX+ROBOT_MAINDLG_MOVEPOSTURE_STEP;
                                _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                _p->m_mcs->mainDlg_robmovestate=MAINDLG_RXADD_ING;
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_RYSUB)
                            {
                                tcp=_p->m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
                                speed=_p->m_mcs->e2proomdata.maindlg_movespeed;//获取到速度值
                                targetpos=_p->m_mcs->rob->TCPpos;
                                targetpos.RY=targetpos.RY-ROBOT_MAINDLG_MOVEPOSTURE_STEP;
                                _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                _p->m_mcs->mainDlg_robmovestate=MAINDLG_RYSUB_ING;
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_RYADD)
                            {
                                tcp=_p->m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
                                speed=_p->m_mcs->e2proomdata.maindlg_movespeed;//获取到速度值
                                targetpos=_p->m_mcs->rob->TCPpos;
                                targetpos.RY=targetpos.RY+ROBOT_MAINDLG_MOVEPOSTURE_STEP;
                                _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                _p->m_mcs->mainDlg_robmovestate=MAINDLG_RYADD_ING;
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_RZSUB)
                            {
                                tcp=_p->m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
                                speed=_p->m_mcs->e2proomdata.maindlg_movespeed;//获取到速度值
                                targetpos=_p->m_mcs->rob->TCPpos;
                                targetpos.RZ=targetpos.RZ-ROBOT_MAINDLG_MOVEPOSTURE_STEP;
                                _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                _p->m_mcs->mainDlg_robmovestate=MAINDLG_RZSUB_ING;
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_RZADD)
                            {
                                tcp=_p->m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
                                speed=_p->m_mcs->e2proomdata.maindlg_movespeed;//获取到速度值
                                targetpos=_p->m_mcs->rob->TCPpos;
                                targetpos.RZ=targetpos.RZ+ROBOT_MAINDLG_MOVEPOSTURE_STEP;
                                _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                _p->m_mcs->mainDlg_robmovestate=MAINDLG_RZADD_ING;
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_OUT1SUB)
                            {
                                tcp=_p->m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
                                speed=_p->m_mcs->e2proomdata.maindlg_movespeed;//获取到速度值
                                targetpos=_p->m_mcs->rob->TCPpos;
                                targetpos.out_1=targetpos.out_1-ROBOT_MAINDLG_MOVEOUT_STEP;
                                _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                _p->m_mcs->mainDlg_robmovestate=MAINDLG_OUT1SUB_ING;
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_OUT1ADD)
                            {
                                tcp=_p->m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
                                speed=_p->m_mcs->e2proomdata.maindlg_movespeed;//获取到速度值
                                targetpos=_p->m_mcs->rob->TCPpos;
                                targetpos.out_1=targetpos.out_1+ROBOT_MAINDLG_MOVEOUT_STEP;
                                _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                _p->m_mcs->mainDlg_robmovestate=MAINDLG_OUT1ADD_ING;
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_OUT2SUB)
                            {
                                tcp=_p->m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
                                speed=_p->m_mcs->e2proomdata.maindlg_movespeed;//获取到速度值
                                targetpos=_p->m_mcs->rob->TCPpos;
                                targetpos.out_2=targetpos.out_2-ROBOT_MAINDLG_MOVEOUT_STEP;
                                _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                _p->m_mcs->mainDlg_robmovestate=MAINDLG_OUT2SUB_ING;
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_OUT2ADD)
                            {
                                tcp=_p->m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
                                speed=_p->m_mcs->e2proomdata.maindlg_movespeed;//获取到速度值
                                targetpos=_p->m_mcs->rob->TCPpos;
                                targetpos.out_2=targetpos.out_2+ROBOT_MAINDLG_MOVEOUT_STEP;
                                _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                _p->m_mcs->mainDlg_robmovestate=MAINDLG_OUT2ADD_ING;
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_OUT3SUB)
                            {
                                tcp=_p->m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
                                speed=_p->m_mcs->e2proomdata.maindlg_movespeed;//获取到速度值
                                targetpos=_p->m_mcs->rob->TCPpos;
                                targetpos.out_3=targetpos.out_3-ROBOT_MAINDLG_MOVEOUT_STEP;
                                _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                _p->m_mcs->mainDlg_robmovestate=MAINDLG_OUT3SUB_ING;
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_OUT3ADD)
                            {
                                tcp=_p->m_mcs->e2proomdata.maindlg_movetcp;//获取到移动TCP
                                speed=_p->m_mcs->e2proomdata.maindlg_movespeed;//获取到速度值
                                targetpos=_p->m_mcs->rob->TCPpos;
                                targetpos.out_3=targetpos.out_3+ROBOT_MAINDLG_MOVEOUT_STEP;
                                _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                _p->m_mcs->mainDlg_robmovestate=MAINDLG_OUT3ADD_ING;
                            }

                            //判段是否已经靠近移动目标，是的话就更新移动目标
                            if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_XSUB_ING)
                            {
                                if(_p->m_mcs->rob->TCPpos.X-targetpos.X<ROBOT_MAINDLG_MOVEPOS_STEPMIN)
                                {
                                    targetpos.X=targetpos.X-ROBOT_MAINDLG_MOVEPOS_STEP;
                                    _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                }
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_XADD_ING)
                            {
                                if(targetpos.X-_p->m_mcs->rob->TCPpos.X<ROBOT_MAINDLG_MOVEPOS_STEPMIN)
                                {
                                    targetpos.X=targetpos.X+ROBOT_MAINDLG_MOVEPOS_STEP;
                                    _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                }
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_YSUB_ING)
                            {
                                if(_p->m_mcs->rob->TCPpos.Y-targetpos.Y<ROBOT_MAINDLG_MOVEPOS_STEPMIN)
                                {
                                    targetpos.Y=targetpos.Y-ROBOT_MAINDLG_MOVEPOS_STEP;
                                    _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                }
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_YADD_ING)
                            {
                                if(targetpos.Y-_p->m_mcs->rob->TCPpos.Y<ROBOT_MAINDLG_MOVEPOS_STEPMIN)
                                {
                                    targetpos.Y=targetpos.Y+ROBOT_MAINDLG_MOVEPOS_STEP;
                                    _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                }
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_ZSUB_ING)
                            {
                                if(_p->m_mcs->rob->TCPpos.Z-targetpos.Z<ROBOT_MAINDLG_MOVEPOS_STEPMIN)
                                {
                                    targetpos.Z=targetpos.Z-ROBOT_MAINDLG_MOVEPOS_STEP;
                                    _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                }
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_ZADD_ING)
                            {
                                if(targetpos.Z-_p->m_mcs->rob->TCPpos.Z<ROBOT_MAINDLG_MOVEPOS_STEPMIN)
                                {
                                    targetpos.Z=targetpos.Z+ROBOT_MAINDLG_MOVEPOS_STEP;
                                    _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                }
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_RXSUB_ING)
                            {
                                if(_p->m_mcs->rob->TCPpos.RX-targetpos.RX<ROBOT_MAINDLG_MOVEPOSTURE_STEPMIN)
                                {
                                    targetpos.RX=targetpos.RX-ROBOT_MAINDLG_MOVEPOSTURE_STEP;
                                    _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                }
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_RXADD_ING)
                            {
                                if(targetpos.RX-_p->m_mcs->rob->TCPpos.RX<ROBOT_MAINDLG_MOVEPOSTURE_STEPMIN)
                                {
                                    targetpos.RX=targetpos.RX+ROBOT_MAINDLG_MOVEPOSTURE_STEP;
                                    _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                }
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_RYSUB_ING)
                            {
                                if(_p->m_mcs->rob->TCPpos.RY-targetpos.RY<ROBOT_MAINDLG_MOVEPOSTURE_STEPMIN)
                                {
                                    targetpos.RY=targetpos.RY-ROBOT_MAINDLG_MOVEPOSTURE_STEP;
                                    _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                }
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_RYADD_ING)
                            {
                                if(targetpos.RY-_p->m_mcs->rob->TCPpos.RY<ROBOT_MAINDLG_MOVEPOSTURE_STEPMIN)
                                {
                                    targetpos.RY=targetpos.RY+ROBOT_MAINDLG_MOVEPOSTURE_STEP;
                                    _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                }
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_RZSUB_ING)
                            {
                                if(_p->m_mcs->rob->TCPpos.RZ-targetpos.RZ<ROBOT_MAINDLG_MOVEPOSTURE_STEPMIN)
                                {
                                    targetpos.RZ=targetpos.RZ-ROBOT_MAINDLG_MOVEPOSTURE_STEP;
                                    _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                }
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_RZADD_ING)
                            {
                                if(targetpos.RZ-_p->m_mcs->rob->TCPpos.RZ<ROBOT_MAINDLG_MOVEPOSTURE_STEPMIN)
                                {
                                    targetpos.RZ=targetpos.RZ+ROBOT_MAINDLG_MOVEPOSTURE_STEP;
                                    _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                }
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_OUT1SUB_ING)
                            {
                                if(_p->m_mcs->rob->TCPpos.out_1-targetpos.out_1<ROBOT_MAINDLG_MOVEOUT_STEPMIN)
                                {
                                    targetpos.out_1=targetpos.out_1-ROBOT_MAINDLG_MOVEPOSTURE_STEP;
                                    _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                }
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_OUT1ADD_ING)
                            {
                                if(targetpos.out_1-_p->m_mcs->rob->TCPpos.out_1<ROBOT_MAINDLG_MOVEOUT_STEPMIN)
                                {
                                    targetpos.out_1=targetpos.out_1+ROBOT_MAINDLG_MOVEPOSTURE_STEP;
                                    _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                }
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_OUT2SUB_ING)
                            {
                                if(_p->m_mcs->rob->TCPpos.out_2-targetpos.out_2<ROBOT_MAINDLG_MOVEOUT_STEPMIN)
                                {
                                    targetpos.out_2=targetpos.out_2-ROBOT_MAINDLG_MOVEPOSTURE_STEP;
                                    _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                }
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_OUT2ADD_ING)
                            {
                                if(targetpos.out_2-_p->m_mcs->rob->TCPpos.out_2<ROBOT_MAINDLG_MOVEOUT_STEPMIN)
                                {
                                    targetpos.out_2=targetpos.out_2+ROBOT_MAINDLG_MOVEPOSTURE_STEP;
                                    _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                }
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_OUT3SUB_ING)
                            {
                                if(_p->m_mcs->rob->TCPpos.out_3-targetpos.out_3<ROBOT_MAINDLG_MOVEOUT_STEPMIN)
                                {
                                    targetpos.out_3=targetpos.out_3-ROBOT_MAINDLG_MOVEPOSTURE_STEP;
                                    _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                }
                            }
                            else if(_p->m_mcs->mainDlg_robmovestate==MAINDLG_OUT3ADD_ING)
                            {
                                if(targetpos.out_3-_p->m_mcs->rob->TCPpos.out_3<ROBOT_MAINDLG_MOVEOUT_STEPMIN)
                                {
                                    targetpos.out_3=targetpos.out_3+ROBOT_MAINDLG_MOVEPOSTURE_STEP;
                                    _p->m_mcs->tosendbuffer->cmd_move(targetpos,movemod,speed,tcp);//移动
                                }
                            }
                        }
                    }
                    /*******************************/
                } 
            }
            if(_p->b_init_show_robpos_list==true)
            {
                _p->b_init_show_robpos_list=false;
                emit Send_show_robpos_list();
            }
            sleep(0);
        }
        else
        {
            _p->b_stop_thread2=true;
            break;
        }
    }
}

void qtgetrobThread::Stop()
{
  if(_p->b_thread2==true)
  {
    _p->b_stop_thread2=false;
    _p->b_thread2=false;
    while (_p->b_stop_thread2==false)
    {
      sleep(0);
    }
  }
}

qtrecordThread::qtrecordThread(qtweldingDlg *statci_p)
{
    _p=statci_p;
}

void qtrecordThread::run()
{
    while (1)
    {
        if(_p->b_thread3==true)
        {
            main_record.lock();
            if(_p->m_mcs->main_record.size()!=0)
            {
                if(_p->b_init_show_record_list==true)
                {
                    QString msg=_p->m_mcs->main_record[0];
                    std::vector<QString>::iterator it = _p->m_mcs->main_record.begin();
                    _p->m_mcs->main_record.erase(it);
                    _p->b_init_show_record_list=false;
                    qRegisterMetaType< QString >("QString");
                    emit Send_show_record_list(msg);
                }
            }
            main_record.unlock();
        }
        else
        {
            _p->b_stop_thread3=true;
            break;
        }
        usleep(0);
    }
}

void qtrecordThread::Stop()
{
  if(_p->b_thread3==true)
  {
    _p->b_stop_thread3=false;
    _p->b_thread3=false;
    while (_p->b_stop_thread3==false)
    {
      sleep(0);
    }
  }
}

/*
qtplcThread::qtplcThread(qtweldingDlg *statci_p)
{
    _p=statci_p;
}

void qtplcThread::run()
{
    while (1)
    {
        if(_p->b_thread4==true)
        {
            if(_p->m_mcs->rob->b_connect_plc==true)
            {
                if(_p->m_mcs->rob->ctx_plc_dosomeing==DO_WRITE_TASK)
                {
                    send_group_plc.lock();
                    if(_p->m_mcs->rob->send_group_plc.size()!=0)
                    {
                        sent_info_robot sentdata=_p->m_mcs->rob->send_group_plc[0];
                        std::vector<sent_info_robot>::iterator it = _p->m_mcs->rob->send_group_plc.begin();
                        _p->m_mcs->rob->send_group_plc.erase(it);
                        int rc=modbus_write_registers(sentdata.ctx,sentdata.addr,sentdata.data.size(),sentdata.data.data());
                        if(rc!=sentdata.data.size())
                        {
                            _p->m_mcs->rob->b_send_group_plc=false;
                            if(_p->b_init_set_plctask==true)
                            {
                                _p->b_init_set_plctask=false;
                                emit Send_set_plctask();
                            }
                        }
                        else
                        {
                            _p->m_mcs->rob->b_send_group_plc=true;
                        }
                        if(_p->m_mcs->rob->send_group_plc.size()==0)
                        {
                            _p->m_mcs->rob->ctx_plc_dosomeing=DO_NOTHING;
                        }
                    }
                    else
                    {
                        _p->m_mcs->rob->ctx_plc_dosomeing=DO_NOTHING;
                    }
                    send_group_plc.unlock();
                }
                if(_p->m_mcs->rob->ctx_plc_dosomeing==DO_NOTHING)
                {
                //访问机器人坐标通信
                    if(0<=modbus_read_registers(_p->m_mcs->rob->ctx_plc,PLC_ST_REG_ADD,PLC_RED_NUM,_p->plc_rcv_data))
                    {

                    }
                }
            }
            sleep(0);
        }
        else
        {
            _p->b_stop_thread4=true;
            break;
        }
    }
}

void qtplcThread::Stop()
{
  if(_p->b_thread4==true)
  {
    _p->b_stop_thread4=false;
    _p->b_thread4=false;
    while (_p->b_stop_thread4==false)
    {
      sleep(0);
    }
  }
}

*/
























