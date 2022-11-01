#include "qtweldingdlg.h"
#include "ui_qtweldingdlg.h"
#include<QGridLayout>

qtweldingDlg::qtweldingDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::qtweldingDlg)
{
#if _MSC_VER
    QString filePath =  "./DATA";
    QDir dir(filePath);
    if(!dir.exists())
        QDir().mkdir(filePath);
#else
    std::string dir = "./DATA";
    if (access(dir.c_str(), 0) == -1)
    {
      mkdir("./DATA",S_IRWXU);
    }
#endif
    m_mcs=m_mcs->Get();

    b_init_show_ui_list=true;
    b_init_sent_leaser=true;
    b_init_show_robpos_list=true;
    b_init_set_robtask=true;

    b_RunAlgCamer=false;

    qtmysunny=new qtmysunnyDlg(m_mcs);
    demarcate=new demarcateDlg(m_mcs);
    robotset=new robotsetDlg(m_mcs);
    editproject=new editprojectDlg(m_mcs);
    newproject=new newprojectDlg(m_mcs);
    setproject=new setprojectDlg(m_mcs);

    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint        //显示关闭
                  |Qt::WindowMinMaxButtonsHint);    //显示最大最小化

    ui->project_name->setText(QString::fromLocal8Bit("未命名"));
    ui->project_Id->setText(QString::fromLocal8Bit("无"));
    ui->project_scannum->setText(QString::fromLocal8Bit("0/0"));
    ui->project_weldnum->setText(QString::fromLocal8Bit("0/0"));
    ui->robot_model->setText(m_mcs->rob->robot_model_toQString());
    ui->robot_ip_port->setText(QString::fromLocal8Bit("0.0.0.0"));
    ui->robot_state->setText(m_mcs->rob->robot_state_toQString());
    ui->robot_speed->setText(QString::number(m_mcs->rob->robot_speed,'f',3));
    ui->robot_pos_x->setText(QString::number(m_mcs->rob->TCPpos.X,'f',3));
    ui->robot_pos_y->setText(QString::number(m_mcs->rob->TCPpos.Y,'f',3));
    ui->robot_pos_z->setText(QString::number(m_mcs->rob->TCPpos.Z,'f',3));
    ui->robot_pos_rx->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',3));
    ui->robot_pos_ry->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',3));
    ui->robot_pos_rz->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',3));
    ui->leaser_ip->setText(QString::fromLocal8Bit("0.0.0.0"));
    ui->leaser_pos_y->setText(QString::fromLocal8Bit("0.000"));
    ui->leaser_pos_z->setText(QString::fromLocal8Bit("0.000"));
    ui->leaser_state->setText(QString::fromLocal8Bit("未链接"));
    ui->leaser_tasknum->setText(QString::fromLocal8Bit("0"));
    ui->leaser_time->setText(QString::fromLocal8Bit("00:00:00:000"));
    ui->leaser_timestamp->setText(QString::fromLocal8Bit("00:00:00:000"));
    ui->leaser_camera_fps->setText(QString::fromLocal8Bit("0.00"));
    ui->leaser_result_fps->setText(QString::fromLocal8Bit("0.00"));
    ui->weld_state->setText(QString::fromLocal8Bit("待机"));
    ui->weld_current->setText(QString::fromLocal8Bit("0.000"));
    ui->weld_process->setText(QString::fromLocal8Bit("平焊"));
    ui->weld_alternating->setText(QString::fromLocal8Bit("直流"));

    thread1 = new qtweldingThread(this);
    connect(thread1, SIGNAL(Send_show_ui_list()), this, SLOT(init_show_ui_list()));
    connect(thread1, SIGNAL(Send_sent_leaser()), this, SLOT(init_sent_leaser()));

    thread2 = new qtgetrobThread(this);
    connect(thread2, SIGNAL(Send_show_robpos_list()), this, SLOT(init_show_robpos_list()));
    connect(thread2, SIGNAL(Send_set_robtask()), this, SLOT(init_set_robtask()));

    ConnectCamer();//连接相机
    ConnectRobot();//连接机器人

    b_thread1=true;
    thread1->start();

    b_thread2=true;
    thread2->start();

    UpdataUi();

    ui->record->append(QString::fromLocal8Bit("系统启动成功"));
}

qtweldingDlg::~qtweldingDlg()
{
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

    DisconnectCamer();
    DisconnectRobot();

    delete thread1;
    delete thread2;
    delete qtmysunny;
    delete demarcate;
    delete robotset;
    delete editproject;
    delete newproject;
    delete setproject;
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

void qtweldingDlg::on_weldersetBtn_clicked()//焊机设置
{

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
        sentdata.ctx=m_mcs->resultdata.ctx_result;
        sentdata.addr=0x101;
        sentdata.data={0xff};
        m_mcs->resultdata.send_group_leaser.push_back(sentdata);
        m_mcs->resultdata.ctx_result_dosomeing=DO_WRITE_TASK;

        demarcate->init_dlg_show();
        demarcate->setWindowTitle(QString::fromLocal8Bit("标定设置"));
        demarcate->exec();
        demarcate->close_dlg_show();

        sentdata.data={0x00};
        m_mcs->resultdata.send_group_leaser.push_back(sentdata);
        m_mcs->resultdata.ctx_result_dosomeing=DO_WRITE_TASK;
    }
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
    //  RunAlgCamer();
    }

    u_int16_t task;
    real_readnum=modbus_read_registers(m_mcs->resultdata.ctx_result,ALS_TASKNUM_REG_ADD,1,&task);
    if(real_readnum<0)
    {
        ui->record->append(QString::fromLocal8Bit("激光头获取当前任务号信息失败"));
    }
    else
    {
        ui->leaser_tasknum->setText(QString::number(task));
        QString msg=QString::fromLocal8Bit("激光头获取当前内部任务号:")+QString::number(task);
        ui->record->append(msg);
    }
}

void qtweldingDlg::DisconnectCamer()
{
    if(m_mcs->resultdata.link_result_state==true)
    {
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

    ui->leaser_camera_fps->setText(QString::number(m_mcs->resultdata.fps,'f',2));
    ui->leaser_result_fps->setText(QString::number(m_mcs->resultdata.camfps,'f',2));

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

    ui->robot_ip_port->setText(m_mcs->ip->robot_ip[0].remote_ip.ip);
    ui->robot_model->setText(m_mcs->rob->robot_model_toQString());
    ui->robot_state->setText(m_mcs->rob->robot_state_toQString());

    ui->robot_speed->setText(QString::number(m_mcs->rob->robot_speed,'f',3));
    ui->robot_pos_x->setText(QString::number(m_mcs->rob->TCPpos.X,'f',3));
    ui->robot_pos_y->setText(QString::number(m_mcs->rob->TCPpos.Y,'f',3));
    ui->robot_pos_z->setText(QString::number(m_mcs->rob->TCPpos.Z,'f',3));
    ui->robot_pos_rx->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',3));
    ui->robot_pos_ry->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',3));
    ui->robot_pos_rz->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',3));

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
                    if(_p->m_mcs->resultdata.send_group_leaser.size()!=0)
                    {
                        sent_info_leaser sentdata=_p->m_mcs->resultdata.send_group_leaser[0];
                        std::vector<sent_info_leaser>::iterator it = _p->m_mcs->resultdata.send_group_leaser.begin();
                        _p->m_mcs->resultdata.send_group_leaser.erase(it);
                        int rc=modbus_write_registers(sentdata.ctx,sentdata.addr,sentdata.data.size(),sentdata.data.data());
                        if(rc!=1)
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
                }
                else if(_p->m_mcs->resultdata.ctx_result_dosomeing==DO_NOTHING)
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
                        _p->m_mcs->resultdata.pos2.nEn=true;
                        _p->m_mcs->resultdata.pos3.Y=Y3;
                        _p->m_mcs->resultdata.pos3.Z=Z3;
                        _p->m_mcs->resultdata.pos3.nEn=true;
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
                    if(_p->m_mcs->rob->send_group_robot.size()!=0)
                    {
                        sent_info_robot sentdata=_p->m_mcs->rob->send_group_robot[0];
                        std::vector<sent_info_robot>::iterator it = _p->m_mcs->rob->send_group_robot.begin();
                        _p->m_mcs->rob->send_group_robot.erase(it);
                        int rc=modbus_write_registers(sentdata.ctx,sentdata.addr,sentdata.data.size(),sentdata.data.data());
                        if(rc!=1)
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
                }
                else if(_p->m_mcs->rob->ctx_robot_dosomeing==DO_NOTHING)
                {
                //访问机器人坐标通信
                    if(0<=modbus_read_registers(_p->m_mcs->rob->ctx_posget,ROB_X_POS_FH_REG_ADD,14,_p->robotpos_rcv_data))
                    {
                        _p->m_mcs->rob->TCPpos.X=*((float*)&_p->robotpos_rcv_data[0]);
                        _p->m_mcs->rob->TCPpos.Y=*((float*)&_p->robotpos_rcv_data[2]);
                        _p->m_mcs->rob->TCPpos.Z=*((float*)&_p->robotpos_rcv_data[4]);
                        _p->m_mcs->rob->TCPpos.RX=*((float*)&_p->robotpos_rcv_data[6]);
                        _p->m_mcs->rob->TCPpos.RY=*((float*)&_p->robotpos_rcv_data[8]);
                        _p->m_mcs->rob->TCPpos.RZ=*((float*)&_p->robotpos_rcv_data[10]);
                        _p->m_mcs->rob->TCPpos.nEn=true;
                        _p->m_mcs->rob->robot_speed=*((float*)&_p->robotpos_rcv_data[12]);
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





