#include "qtweldingdlg.h"
#include "ui_qtweldingdlg.h"
#include<QGridLayout>

QMutex send_group_leaser;
QMutex send_group_robot;
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
#endif
    m_mcs=m_mcs->Get();

    b_init_show_ui_list=true;
    b_init_sent_leaser=true;
    b_init_show_robpos_list=true;
    b_init_set_robtask=true;
    b_init_show_record_list=true;

    b_RunAlgCamer=false;

    qtmysunny=new qtmysunnyDlg(m_mcs);
    demarcate=new demarcateDlg(m_mcs);
    robotset=new robotsetDlg(m_mcs);
    weldset=new weldsetDlg(m_mcs);
    editproject=new editprojectDlg(m_mcs);
    newproject=new newprojectDlg(m_mcs);
    setproject=new setprojectDlg(m_mcs);
    editcraft=new editcraftDlg(m_mcs);
    newcraft=new newcraftDlg(m_mcs);
    setcraft0=new setcraft0Dlg(m_mcs);
    setcraft1=new setcraft1Dlg(m_mcs);
    setcraft2=new setcraft2Dlg(m_mcs);

    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint        //显示关闭
                  |Qt::WindowMinMaxButtonsHint);    //显示最大最小化

    ui->project_name->setText(QString::fromLocal8Bit("未命名"));
    ui->project_Id->setText(QString::fromLocal8Bit("无"));
    ui->project_scannum->setText(QString::fromLocal8Bit("0/0"));
    ui->project_weldnum->setText(QString::fromLocal8Bit("0/0"));
    ui->robot_model->setText(m_mcs->rob->robot_model_toQString(m_mcs->rob->robot_model));
    ui->robot_ip_port->setText(QString::fromLocal8Bit("0.0.0.0"));
    ui->robot_state->setText(m_mcs->rob->robot_state_toQString(m_mcs->rob->robot_state));
    ui->robot_speed->setText(QString::number(m_mcs->rob->robot_speed,'f',3));
    ui->robot_pos_x->setText(QString::number(m_mcs->rob->TCPpos.X,'f',ROBOT_POSE_DECIMAL_PLACE));
    ui->robot_pos_y->setText(QString::number(m_mcs->rob->TCPpos.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
    ui->robot_pos_z->setText(QString::number(m_mcs->rob->TCPpos.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
    ui->robot_pos_rx->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    ui->robot_pos_ry->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    ui->robot_pos_rz->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    ui->leaser_ip->setText(QString::fromLocal8Bit("0.0.0.0"));
    ui->leaser_pos_y->setText(QString::fromLocal8Bit("0.000"));
    ui->leaser_pos_z->setText(QString::fromLocal8Bit("0.000"));
    ui->leaser_state->setText(QString::fromLocal8Bit("未链接"));
    ui->leaser_tasknum->setText(QString::fromLocal8Bit("0"));
    ui->leaser_time->setText(QString::fromLocal8Bit("00:00:00:000"));
    ui->leaser_timestamp->setText(QString::fromLocal8Bit("00:00:00:000"));
    ui->leaser_camera_fps->setText(QString::fromLocal8Bit("0.00"));
    ui->leaser_result_fps->setText(QString::fromLocal8Bit("0.00"));
    ui->weld_model->setText(m_mcs->rob->weld_model_toQString(m_mcs->rob->weld_model));
    ui->weld_ip_port->setText(QString::fromLocal8Bit("0.0.0.0"));
    ui->weld_state->setText(QString::fromLocal8Bit("待机"));
    ui->weld_current->setText(QString::fromLocal8Bit("0.000"));
    ui->weld_process->setText(QString::fromLocal8Bit("平焊"));
    ui->weld_alternating->setText(QString::fromLocal8Bit("直流"));

    if(m_mcs->e2proomdata.maindlg_SaveDatacheckBox==0)
    {
        ui->SaveDatacheckBox->setCheckState(Qt::Unchecked);
    }
    else
    {
        ui->SaveDatacheckBox->setCheckState(Qt::Checked);
    }
    ui->weld_eled->setText(QString::number(m_mcs->e2proomdata.maindlg_Weldeled,'f',3));
    ui->weld_elem->setCurrentIndex(m_mcs->e2proomdata.maindlg_Weldelem);

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

    ConnectCamer();//连接相机
    ConnectRobot();//连接机器人

    b_thread1=true;
    thread1->start();

    b_thread2=true;
    thread2->start();

    b_thread3=true;
    thread3->start();

    UpdataUi();

    ui->record->append(QString::fromLocal8Bit("系统启动成功"));
}

qtweldingDlg::~qtweldingDlg()
{
#ifdef USE_MYROBOT_CONTROL      //是否使用程序自带接口
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

    DisconnectCamer();
    DisconnectRobot();

    delete thread1;
    delete thread2;
    delete thread3;
    delete qtmysunny;
    delete demarcate;
    delete robotset;
    delete weldset;
    delete editproject;
    delete newproject;
    delete setproject;
    delete editcraft;
    delete newcraft;
    delete setcraft0;
    delete setcraft1;
    delete setcraft2;
    delete ui;
}

void qtweldingDlg::UpdataUi()
{
    if(m_mcs->process->b_processrun==false)
    {
        ui->runprojectBtn->setText(QString::fromLocal8Bit("运行工程"));
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
    }
    else
    {
        ui->runprojectBtn->setText(QString::fromLocal8Bit("停止工程"));
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
    }
    if(m_mcs->process->b_processpaused==false)
    {
        ui->runpausedBtn->setText(QString::fromLocal8Bit("暂停工程"));
    }
    else
    {
        ui->runpausedBtn->setText(QString::fromLocal8Bit("继续工程"));
    }
}

void qtweldingDlg::on_importprojectBtn_clicked()//导入工程
{
    QString fileName = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("请选择要导入的工程文件"), "./DATA/", "JSON(*.json)");
#if _MSC_VER
    QTextCodec *code = QTextCodec::codecForName("GBK");
#else
    QTextCodec *code = QTextCodec::codecForName("UTF-8");
#endif
    std::string name = code->fromUnicode(fileName).data();
    if(name.size()>0)
    {
        if(0!=m_mcs->project->LoadProject((char*)name.c_str()))
        {
            ui->record->append(QString::fromLocal8Bit("工程文件读取失败"));
        }
        else
        {
            QString path=QString::fromLocal8Bit(name.c_str());
            QString msg=QString::fromLocal8Bit("已经成功加载工程文件:")+path;
            ui->record->append(msg);
            msg=QString::fromLocal8Bit("工程名称:")+m_mcs->project->project_name;
            ui->record->append(msg);
        }
    }
}

void qtweldingDlg::on_runprojectBtn_clicked()//运行工程
{
    if(m_mcs->process->b_processrun==false)
    {
        if(m_mcs->rob->b_connect==false)
        {
            ui->record->append(QString::fromLocal8Bit("机器人未链接成功"));
        }
        else if(m_mcs->resultdata.link_result_state==false)
        {
            ui->record->append(QString::fromLocal8Bit("激光头未链接成功"));
        }
        else
        {
            m_mcs->process->init_start_process();
            ui->record->append(QString::fromLocal8Bit("开始运行工程"));
        }
    }
    else
    {
        m_mcs->process->stop_process();

        ui->record->append(QString::fromLocal8Bit("停止运行工程"));
    }
    UpdataUi();
}

void qtweldingDlg::on_runpausedBtn_clicked()//暂停工程
{
    if(m_mcs->process->b_processpaused==false)
    {
        m_mcs->process->paused_process();
        ui->record->append(QString::fromLocal8Bit("暂停运行工程"));
    }
    else
    {
        m_mcs->process->continue_process();
        ui->record->append(QString::fromLocal8Bit("继续运行工程"));
    }
    UpdataUi();
}

void qtweldingDlg::on_editprojectBtn_clicked()//工程编辑
{
    int rc;
    editproject->init_dlg_show();
    editproject->setWindowTitle(QString::fromLocal8Bit("工程编辑"));
    rc=editproject->exec();
    editproject->close_dlg_show();
    switch(rc)
    {
        case EDITPROJECTDLG_BTN1:   //新建工程
        {
            int rc2;           
            newproject->init_dlg_show();
            newproject->setWindowTitle(QString::fromLocal8Bit("新建工程"));
            rc2=newproject->exec();
            newproject->close_dlg_show();
            if(rc2!=0)//保存成功返回
            {
                QString msg=QString::fromLocal8Bit("工程名称: ")+m_mcs->project->project_name+
                            QString::fromLocal8Bit(" 工程类型")+QString::number(m_mcs->project->project_Id)+": "
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
                ui->record->append(QString::fromLocal8Bit("取消新建工程"));
            }
        }
        break;
        case EDITPROJECTDLG_BTN2:   //编辑当前工程
        {
            QString msg=QString::fromLocal8Bit("工程名称: ")+m_mcs->project->project_name+
                        QString::fromLocal8Bit(" 工程类型")+QString::number(m_mcs->project->project_Id)+": "
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
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("机器人未连接成功"));
        return;
    }
    int rc;
    editcraft->init_dlg_show();
    editcraft->setWindowTitle(QString::fromLocal8Bit("焊接工艺设置"));
    rc=editcraft->exec();
    editcraft->close_dlg_show();
    switch(rc)
    {
        case EDITCRAFTDLG_BTN1:   //新建工艺
        {
            int rc2;
            newcraft->init_dlg_show();
            newcraft->setWindowTitle(QString::fromLocal8Bit("新建工艺"));
            rc2=newcraft->exec();
            newcraft->close_dlg_show();
            if(rc2!=0)//保存成功返回
            {
                QString msg=QString::fromLocal8Bit(" 工艺类型")+QString::number(m_mcs->craft->craft_id)+": "
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
                }
            }
            else
            {
                ui->record->append(QString::fromLocal8Bit("取消新建工艺"));
            }
        }
        break;
        case EDITCRAFTDLG_BTN2:   //选择现有工艺
        {
            QString fileName = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("请选择要编辑的工艺文件"), "./CRAFT/", "CRAFT(*.craft)");
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
                    ui->record->append(QString::fromLocal8Bit("工艺文件内容格式错误,读取失败"));
                }
                else
                {
                    m_mcs->craft->craft_path=fileName;
                    QString msg=QString::fromLocal8Bit(" 工艺类型")+QString::number(m_mcs->craft->craft_id)+": "
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
                    }
                }
            }
        }
        break;
    }
}

void qtweldingDlg::on_setlaserheadBtn_clicked()//激光头设置
{
    thread1->Stop();
    thread1->quit();
    thread1->wait();
    DisconnectCamer();
    qtmysunny->init_dlg_show();
    qtmysunny->setWindowTitle(QString::fromLocal8Bit("激光头设置"));
    qtmysunny->exec();
    qtmysunny->close_dlg_show();
    ConnectCamer();
    b_thread1=true;
    thread1->start();
}


void qtweldingDlg::on_setrobotBtn_clicked()//机器人设置
{
    thread2->Stop();
    thread2->quit();
    thread2->wait();
    DisconnectRobot();
    robotset->init_dlg_show();
    robotset->setWindowTitle(QString::fromLocal8Bit("机器人设置"));
    robotset->exec();
    robotset->close_dlg_show();
    ConnectRobot();
    b_thread2=true;
    thread2->start();
}


void qtweldingDlg::on_setweldBtn_clicked()//焊机设置
{
    thread2->Stop();
    thread2->quit();
    thread2->wait();
    DisconnectRobot();
    weldset->init_dlg_show();
    weldset->setWindowTitle(QString::fromLocal8Bit("焊机设置"));
    weldset->exec();
    weldset->close_dlg_show();
    ConnectRobot();
    b_thread2=true;
    thread2->start();
}

void qtweldingDlg::on_demarcateBtn_clicked()//标定设置
{
    if(m_mcs->resultdata.link_result_state==false)
    {
        ui->record->append(QString::fromLocal8Bit("激光头未连接成功"));
    }
    else if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("机器人未连接成功"));
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
        demarcate->setWindowTitle(QString::fromLocal8Bit("标定设置"));
        demarcate->exec();
        demarcate->close_dlg_show();

        m_mcs->cam->sop_cam[0].InitConnect();
        if(m_mcs->cam->sop_cam[0].b_connect==true)
        {
            ui->record->append(QString::fromLocal8Bit("激光头连接成功"));
        }
        else if(m_mcs->cam->sop_cam[0].b_connect==false)
        {
            ui->record->append(QString::fromLocal8Bit("激光头连接失败"));
        }

        send_group_leaser.lock();
        sentdata.data={0x00};
        m_mcs->resultdata.b_send_group_leaser=false;
        m_mcs->resultdata.send_group_leaser.push_back(sentdata);
        m_mcs->resultdata.ctx_result_dosomeing=DO_WRITE_TASK;
        send_group_leaser.unlock();
    }
}

void qtweldingDlg::on_weld_windBtn_pressed()    //送丝按下
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("焊机未连接成功"));
        return;
    }
    m_mcs->tosendbuffer->cmd_elec(WIND);
    ui->record->append(QString::fromLocal8Bit("开始送丝"));
}


void qtweldingDlg::on_weld_windBtn_released()   //送丝抬起
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("焊机未连接成功"));
        return;
    }
    m_mcs->tosendbuffer->cmd_elec(STATIC);
    ui->record->append(QString::fromLocal8Bit("停止送丝"));
}


void qtweldingDlg::on_weld_rewindBtn_pressed()  //退丝按下
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("焊机未连接成功"));
        return;
    }
    m_mcs->tosendbuffer->cmd_elec(REWIND);
    ui->record->append(QString::fromLocal8Bit("开始退丝"));
}


void qtweldingDlg::on_weld_rewindBtn_released()  //退丝抬起
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("焊机未连接成功"));
        return;
    }
    m_mcs->tosendbuffer->cmd_elec(STATIC);
    ui->record->append(QString::fromLocal8Bit("停止退丝"));
}


void qtweldingDlg::on_weld_gassBtn_pressed()    //送气按下
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("焊机未连接成功"));
        return;
    }
    m_mcs->tosendbuffer->cmd_elec(GASS);
    ui->record->append(QString::fromLocal8Bit("开始送气"));
}


void qtweldingDlg::on_weld_gassBtn_released()    //送气抬起
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("焊机未连接成功"));
        return;
    }
    m_mcs->tosendbuffer->cmd_elec(STATIC);
    ui->record->append(QString::fromLocal8Bit("停止送气"));
}


void qtweldingDlg::on_weld_fireBtn_pressed()    //点焊按下
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("焊机未连接成功"));
        return;
    }
    Alternatingcurrent_ID elem=(Alternatingcurrent_ID)ui->weld_elem->currentIndex();
    float eled=ui->weld_eled->text().toFloat();
    m_mcs->tosendbuffer->cmd_elec(eled,elem,FIRE);
    ui->record->append(QString::fromLocal8Bit("开始点焊"));
}


void qtweldingDlg::on_weld_fireBtn_released()   //点焊抬起
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("焊机未连接成功"));
        return;
    }
    m_mcs->tosendbuffer->cmd_elec(STATIC);
    ui->record->append(QString::fromLocal8Bit("停止点焊"));
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

void qtweldingDlg::on_SaveDatacheckBox_stateChanged(int arg1)//保存数据
{
    if(arg1==0)
    {
        m_mcs->e2proomdata.maindlg_SaveDatacheckBox=0;
        ui->record->append(QString::fromLocal8Bit("关闭轨迹数据保存功能"));
    }
    else
    {
        m_mcs->e2proomdata.maindlg_SaveDatacheckBox=1;
        ui->record->append(QString::fromLocal8Bit("开启轨迹数据保存功能"));
    }
    m_mcs->e2proomdata.write_maindlg_para();
}


void qtweldingDlg::ConnectCamer()
{
    int real_readnum;
    if(m_mcs->resultdata.link_result_state==false)
    {
        QString server_ip=m_mcs->ip->camer_ip[0].ip;
        QString server_port2=QString::number(PORT_ALS_RESULT);
        m_mcs->resultdata.ctx_result = modbus_new_tcp(server_ip.toUtf8(), server_port2.toInt());
        if (modbus_connect(m_mcs->resultdata.ctx_result) == -1)
        {
            ui->record->append(server_port2+QString::fromLocal8Bit("端口连接失败"));
            modbus_free(m_mcs->resultdata.ctx_result);
            return;
        }
        m_mcs->resultdata.link_result_state=true;
        ui->record->append(server_port2+QString::fromLocal8Bit("端口连接成功"));
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
        ui->record->append(msg+QString::fromLocal8Bit("端口关闭"));
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
            ui->record->append(QString::fromLocal8Bit("激光头相机启动设置失败"));
        }
        else
        {
            ui->record->append(QString::fromLocal8Bit("激光头相机启动设置成功"));
            if(b_RunAlgCamer==false)
            {
                m_mcs->cam->sop_cam[0].InitConnect();

                if(m_mcs->cam->sop_cam[0].b_connect==true)
                {
                    ui->record->append(QString::fromLocal8Bit("激光头连接成功"));
                    b_RunAlgCamer=true;
                }
                else if(m_mcs->cam->sop_cam[0].b_connect==false)
                {
                    ui->record->append(QString::fromLocal8Bit("激光头连接失败"));
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
            ui->record->append(QString::fromLocal8Bit("相机关闭"));
            b_RunAlgCamer=false;
        }
        uint16_t tab_reg[1];
        tab_reg[0]=0;
        int rc=modbus_write_registers(m_mcs->resultdata.ctx_result,ALS_OPEN_REG_ADD,1,tab_reg);
        if(rc!=1)
        {
            ui->record->append(QString::fromLocal8Bit("激光头相机关闭设置失败"));
        }
        else
        {
            ui->record->append(QString::fromLocal8Bit("激光头相机关闭设置成功"));
        }
    }
}

void qtweldingDlg::ConnectRobot()
{
    if(0==m_mcs->rob->ConnectRobot(m_mcs->ip->robot_ip[0].robot_ip.ip,m_mcs->ip->robot_ip[0].robot_ip.port))
    {
        ui->record->append(QString::fromLocal8Bit("与机器人连接成功"));
        //写入机器人型号和远程地址
        uint16_t u16_data[5];
        u16_data[0]=m_mcs->rob->robot_model;
        int rc=modbus_write_registers(m_mcs->rob->ctx_posget,ROB_MODEL_REG_ADD,1,u16_data);
        if(rc!=1)
        {
            ui->record->append(QString::fromLocal8Bit("机器人型号设置失败"));
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
                ui->record->append(QString::fromLocal8Bit("机器人远程IP地址设置失败"));
            }
        }
        else
        {
            ui->record->append(QString::fromLocal8Bit("机器人远程IP地址格式错误"));
        }
        u16_data[0]=0;
        rc=modbus_write_registers(m_mcs->rob->ctx_posget,ROB_STOP_REG_ADD,1,u16_data);
        if(rc!=1)
        {
            ui->record->append(QString::fromLocal8Bit("机器人启停设置失败"));
        }
        //写入焊机型号和远程地址
        u16_data[0]=m_mcs->rob->weld_model;
        rc=modbus_write_registers(m_mcs->rob->ctx_posget,ROB_WELD_MODEL_REG_ADD,1,u16_data);
        if(rc!=1)
        {
            ui->record->append(QString::fromLocal8Bit("焊机型号设置失败"));
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
                ui->record->append(QString::fromLocal8Bit("焊机远程IP地址设置失败"));
            }
        }
        else
        {
            ui->record->append(QString::fromLocal8Bit("焊机远程IP地址格式错误"));
        }
    }
    else
    {
        ui->record->append(QString::fromLocal8Bit("与机器人连接失败"));
    }
}

void qtweldingDlg::DisconnectRobot()
{
    m_mcs->rob->DisconnectRobot();
    ui->record->append(QString::fromLocal8Bit("与机器人断开连接"));
}

void qtweldingDlg::init_show_ui_list()//界面刷新
{
    QString msg;
    //工程信息
    ui->project_name->setText(m_mcs->project->project_name);
    ui->project_Id->setText(m_mcs->project->project_Id_toQString(m_mcs->project->project_Id));

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
        ui->record->append(QString::fromLocal8Bit("激光头通信异常"));
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
        ui->robot_ip_port->setText(QString::fromLocal8Bit("无"));
    }
    else
    {
        ui->robot_ip_port->setText(m_mcs->ip->robot_ip[0].remote_ip.ip);
    }
    if(m_mcs->rob->weld_model==WELD_MODEL_NULL||m_mcs->rob->weld_model==WELD_MODEL_ROBOT_LINK)
    {
        ui->weld_ip_port->setText(QString::fromLocal8Bit("无"));
    }
    else
    {
        ui->weld_ip_port->setText(m_mcs->ip->robot_ip[0].weld_ip.ip);
    }
    ui->weld_current->setText(QString::number(m_mcs->rob->weld_eled,'f',3));
    ui->weld_alternating->setText(m_mcs->rob->weldwork_elem_toQString(m_mcs->rob->weld_elem));
    ui->weld_state->setText(m_mcs->rob->weldwork_model_toQString(m_mcs->rob->weld_state));
    b_init_show_robpos_list=true;
}

void qtweldingDlg::init_set_robtask()
{
    if(m_mcs->rob->b_send_group_robot==false)
    {
        ui->record->append(QString::fromLocal8Bit("机器人通信异常"));
    }
    b_init_set_robtask=true;
}

void qtweldingDlg::init_show_record_list(QString msg)
{
    ui->record->append(msg);
    b_init_show_record_list=true;
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
                    if(0<=modbus_read_registers(_p->m_mcs->rob->ctx_posget,ROB_X_POS_FH_REG_ADD,19+ROBOTINPUTNUM+ROBOTTCPPOSOUTNUM*2,_p->robotpos_rcv_data))
                    {
                        _p->m_mcs->rob->TCPpos.X=*((float*)&_p->robotpos_rcv_data[0]);
                        _p->m_mcs->rob->TCPpos.Y=*((float*)&_p->robotpos_rcv_data[2]);
                        _p->m_mcs->rob->TCPpos.Z=*((float*)&_p->robotpos_rcv_data[4]);
                        _p->m_mcs->rob->TCPpos.RX=*((float*)&_p->robotpos_rcv_data[6]);
                        _p->m_mcs->rob->TCPpos.RY=*((float*)&_p->robotpos_rcv_data[8]);
                        _p->m_mcs->rob->TCPpos.RZ=*((float*)&_p->robotpos_rcv_data[10]);
                        _p->m_mcs->rob->TCPpos.nEn=true;
                        _p->m_mcs->rob->robot_speed=*((float*)&_p->robotpos_rcv_data[12]);
                        _p->m_mcs->rob->robot_state=(ROBOT_STATE)_p->robotpos_rcv_data[14];
                        _p->m_mcs->rob->robtime.hour=(int16_t)_p->robotpos_rcv_data[15];
                        _p->m_mcs->rob->robtime.min=(int16_t)_p->robotpos_rcv_data[16];
                        _p->m_mcs->rob->robtime.sec=(int16_t)_p->robotpos_rcv_data[17];
                        _p->m_mcs->rob->robtime.msec=(int16_t)_p->robotpos_rcv_data[18];
                        for(int n=0;n<ROBOTINPUTNUM;n++)
                        {
                            _p->m_mcs->rob->robioinput[n]=(int16_t)_p->robotpos_rcv_data[19+n];
                        }
                        for(int n=0;n<ROBOTTCPPOSOUTNUM;n++)
                        {
                            _p->m_mcs->rob->robTCPposout[n]=*((int32_t*)&_p->robotpos_rcv_data[19+ROBOTINPUTNUM+n*2]);
                        }
                        //往激光器写入机器人实时坐标信息
                        if(_p->m_mcs->resultdata.link_result_state==true)
                        {
                            sent_info_leaser sentdata;
                            send_group_leaser.lock();
                            int32_t num=0;
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
                            *i32_data=_p->m_mcs->rob->robTCPposout[0];
                            sentdata.data.push_back(u16_data[0]);
                            sentdata.data.push_back(u16_data[1]);
                            *i32_data=_p->m_mcs->rob->robTCPposout[0];
                            sentdata.data.push_back(u16_data[0]);
                            sentdata.data.push_back(u16_data[1]);
                            *i32_data=_p->m_mcs->rob->robTCPposout[2];
                            sentdata.data.push_back(u16_data[0]);
                            sentdata.data.push_back(u16_data[1]);
                            //工具号、坐标系、用户坐标系全用0
                            sentdata.data.push_back(0);
                            sentdata.data.push_back(0);
                            sentdata.data.push_back(0);

                            _p->m_mcs->resultdata.b_send_group_leaser=false;
                            _p->m_mcs->resultdata.send_group_leaser.push_back(sentdata);
                            _p->m_mcs->resultdata.ctx_result_dosomeing=DO_WRITE_TASK;
                            send_group_leaser.unlock();
                        }
                    }
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


