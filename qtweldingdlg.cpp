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
    b_RunAlgCamer=false;
    ctx_result_dosomeing=DO_NOTHING;

    qtmysunny=new qtmysunnyDlg(m_mcs);
    demarcate=new demarcateDlg(m_mcs);

    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint        //显示关闭
                  |Qt::WindowMinMaxButtonsHint);    //显示最大最小化

    ui->project_name->setText(QString::fromLocal8Bit("未命名"));
    ui->project_Id->setText(QString::fromLocal8Bit("无"));
    ui->project_scannum->setText(QString::fromLocal8Bit("0/0"));
    ui->project_weldnum->setText(QString::fromLocal8Bit("0/0"));
    ui->robot_model->setText(QString::fromLocal8Bit("无"));
    ui->robot_ip_port->setText(QString::fromLocal8Bit("0.0.0.0"));
    ui->robot_state->setText(QString::fromLocal8Bit("未链接"));
    ui->robot_speed->setText(QString::fromLocal8Bit("0.000"));
    ui->robot_pos_x->setText(QString::fromLocal8Bit("0.000"));
    ui->robot_pos_y->setText(QString::fromLocal8Bit("0.000"));
    ui->robot_pos_z->setText(QString::fromLocal8Bit("0.000"));
    ui->robot_pos_rx->setText(QString::fromLocal8Bit("0.000"));
    ui->robot_pos_ry->setText(QString::fromLocal8Bit("0.000"));
    ui->robot_pos_rz->setText(QString::fromLocal8Bit("0.000"));
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

    thread = new qtweldingThread(this);
    connect(thread, SIGNAL(Send_show_ui_list()), this, SLOT(init_show_ui_list()));
    connect(thread, SIGNAL(Send_sent_leaser()), this, SLOT(init_sent_leaser()));

    ConnectCamer();//连接相机

    b_thread=true;
    thread->start();

    ui->record->append(QString::fromLocal8Bit("系统启动成功"));
}

qtweldingDlg::~qtweldingDlg()
{
    thread->Stop();
    thread->quit();
    thread->wait();

    DisconnectCamer();

    delete thread;
    delete qtmysunny;
    delete demarcate;
    delete ui;
}

void qtweldingDlg::on_importprojectBtn_clicked()//导入工程
{
    QString fileName = QFileDialog::getOpenFileName(this, "open project", "./DATA/", "Project File(*.json)");
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
            QString path=name.c_str();
            QString msg="已经成功加载工程文件:"+path;
            ui->record->append(msg);
            msg="工程名称:"+m_mcs->project->project_name;
            ui->record->append(msg);
        }
    }
}

void qtweldingDlg::on_runprojectBtn_clicked()//运行工程
{

}

void qtweldingDlg::on_editprojectBtn_clicked()//工程编辑
{

}


void qtweldingDlg::on_editweldprocessBtn_clicked()//焊接工艺编辑
{

}


void qtweldingDlg::on_setlaserheadBtn_clicked()//激光头设置
{
    thread->Stop();
    thread->quit();
    thread->wait();
    DisconnectCamer();
    qtmysunny->init_dlg_show();
    qtmysunny->setWindowTitle(QString::fromLocal8Bit("激光头设置"));
    qtmysunny->exec();
    qtmysunny->close_dlg_show();
    ConnectCamer();
    b_thread=true;
    thread->start();
}


void qtweldingDlg::on_setrobotBtn_clicked()//机器人设置
{

}

void qtweldingDlg::on_setwelderBtn_clicked()//焊机设置
{

}

void qtweldingDlg::on_demarcateBtn_clicked()//标定设置
{
    if(m_mcs->resultdata.link_result_state==false)
    {
        ui->record->append(QString::fromLocal8Bit("激光头未连接成功"));
    }
    /*
    else if()
    {
        ui->record->append(QString::fromLocal8Bit("机器人未连接成功"));
    }
    */
    else
    {
        sent_info_leaser sentdata;
        sentdata.ctx=m_mcs->resultdata.ctx_result;
        sentdata.addr=0x101;
        sentdata.data={0xff};
        send_group_leaser.push_back(sentdata);
        ctx_result_dosomeing=DO_WRITE_TASK;

        demarcate->init_dlg_show();
        demarcate->setWindowTitle(QString::fromLocal8Bit("标定设置"));
        demarcate->exec();
        demarcate->close_dlg_show();

        sentdata.data={0x00};
        send_group_leaser.push_back(sentdata);
        ctx_result_dosomeing=DO_WRITE_TASK;
    }
}

void qtweldingDlg::ConnectCamer()
{
    int real_readnum;
    if(m_mcs->resultdata.link_result_state==false)
    {
        QString server_ip=m_mcs->ip->camer_ip[0].ip;
        QString server_port2="1502";
        m_mcs->resultdata.ctx_result = modbus_new_tcp(server_ip.toUtf8(), server_port2.toInt());
        if (modbus_connect(m_mcs->resultdata.ctx_result) == -1)
        {
            ui->record->append(QString::fromLocal8Bit("激光头1502端口连接失败"));
            modbus_free(m_mcs->resultdata.ctx_result);
            return;
        }
        m_mcs->resultdata.link_result_state=true;
        ui->record->append(QString::fromLocal8Bit("激光头1502端口连接成功"));
    //  RunAlgCamer();
    }

    u_int16_t task;
    real_readnum=modbus_read_registers(m_mcs->resultdata.ctx_result,0x102,1,&task);
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
        ui->record->append(QString::fromLocal8Bit("1502端口关闭"));
    }
}

void qtweldingDlg::RunAlgCamer()
{
    if(m_mcs->resultdata.link_result_state==true)
    {
        uint16_t tab_reg[1];
        tab_reg[0]=0xff;
        int rc=modbus_write_registers(m_mcs->resultdata.ctx_result,0x101,1,tab_reg);
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
        int rc=modbus_write_registers(m_mcs->resultdata.ctx_result,0x101,1,tab_reg);
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

void qtweldingDlg::init_show_ui_list()//界面刷新
{
    QString msg;
    //工程信息
    ui->project_name->setText(m_mcs->project->project_name);
    switch(m_mcs->project->project_Id)
    {
        case PROGECT_ID_TEACH_SCAN:
            ui->project_Id->setText(QString::fromLocal8Bit("示教扫描类型"));
        break;
    }

    //机器人信息
    msg=m_mcs->ip->robot_ip[0].ip+":"+QString::number(m_mcs->ip->robot_ip[0].port);
    ui->robot_ip_port->setText(msg);

    //相机信息
    ui->leaser_ip->setText(m_mcs->ip->camer_ip[0].ip);


    float Y=(int16_t)leaser_rcv_data[1]/100.0;
    float Z=(int16_t)leaser_rcv_data[2]/100.0;
    float Y2=(int16_t)leaser_rcv_data2[0]/100.0;
    float Z2=(int16_t)leaser_rcv_data2[1]/100.0;
    float Y3=(int16_t)leaser_rcv_data2[2]/100.0;
    float Z3=(int16_t)leaser_rcv_data2[3]/100.0;

    u_int16_t hour=(int16_t)leaser_rcv_data[5];
    u_int16_t min=(int16_t)leaser_rcv_data[6];
    u_int16_t sec=(int16_t)leaser_rcv_data[7];
    u_int16_t msec=(int16_t)leaser_rcv_data[8];

    u_int16_t hour2=(int16_t)leaser_rcv_data[11];
    u_int16_t min2=(int16_t)leaser_rcv_data[12];
    u_int16_t sec2=(int16_t)leaser_rcv_data[13];
    u_int16_t msec2=(int16_t)leaser_rcv_data[14];
    float fps=(float)leaser_rcv_data[9]/100.0;
    float camfps=(float)leaser_rcv_data[10]/100.0;

    ui->leaser_state->setText("0x"+QString::number(leaser_rcv_data[0],16));
    ui->leaser_pos_y->setText(QString::number(Y,'f',2));
    ui->leaser_pos_z->setText(QString::number(Z,'f',2));

    m_mcs->resultdata.pos1.Y=Y;
    m_mcs->resultdata.pos1.Z=Z;
    m_mcs->resultdata.pos1.nEn=true;
    m_mcs->resultdata.pos2.Y=Y2;
    m_mcs->resultdata.pos2.Z=Z2;
    m_mcs->resultdata.pos2.nEn=true;
    m_mcs->resultdata.pos3.Y=Y3;
    m_mcs->resultdata.pos3.Z=Z3;
    m_mcs->resultdata.pos3.nEn=true;

    msg=QString::number(hour)+":"+QString::number(min)+":"+QString::number(sec)+":"+QString::number(msec);
    ui->leaser_timestamp->setText(msg);
    msg=QString::number(hour2)+":"+QString::number(min2)+":"+QString::number(sec2)+":"+QString::number(msec2);
    ui->leaser_time->setText(msg);

    ui->leaser_camera_fps->setText(QString::number(fps,'f',2));
    ui->leaser_result_fps->setText(QString::number(camfps,'f',2));

    b_init_show_ui_list=true;
}

void qtweldingDlg::init_sent_leaser()
{
    if(send_group_leaser.size()!=0)
    {
        sent_info_leaser sentdata=send_group_leaser[0];
        std::vector<sent_info_leaser>::iterator it = send_group_leaser.begin();
        send_group_leaser.erase(it);
        int rc=modbus_write_registers(sentdata.ctx,sentdata.addr,sentdata.data.size(),sentdata.data.data());
        if(rc!=1)
        {
            ui->record->append(QString::fromLocal8Bit("激光头通信异常"));
        }
        if(send_group_leaser.size()==0)
        {
            ctx_result_dosomeing=DO_NOTHING;
        }
    }
    else
    {
        ctx_result_dosomeing=DO_NOTHING;
    }
    b_init_sent_leaser=true;
}

qtweldingThread::qtweldingThread(qtweldingDlg *statci_p)
{
    _p=statci_p;
}

void qtweldingThread::run()
{
    while (1)
    {
        if(_p->b_thread==true)
        {
            if(_p->m_mcs->resultdata.link_result_state==true)
            {
                if(_p->ctx_result_dosomeing==DO_WRITE_TASK)
                {
                    if(_p->b_init_sent_leaser==true)
                    {
                        _p->b_init_sent_leaser=false;
                        emit Send_sent_leaser();
                    }
                }
                else if(_p->ctx_result_dosomeing==DO_NOTHING)
                {
                    modbus_read_registers(_p->m_mcs->resultdata.ctx_result,0x02,15,_p->leaser_rcv_data);
                    modbus_read_registers(_p->m_mcs->resultdata.ctx_result,0x50,4,_p->leaser_rcv_data2);
                }
            }
            if(_p->b_init_show_ui_list==true)
            {
                _p->b_init_show_ui_list=false;
                emit Send_show_ui_list();
            }
        }
        else
        {
            _p->b_stop_thread=true;
            break;
        }
    }
}

void qtweldingThread::Stop()
{
  if(_p->b_thread==true)
  {
    _p->b_stop_thread=false;
    _p->b_thread=false;
    while (_p->b_stop_thread==false)
    {
      sleep(0);
    }
  }
}




