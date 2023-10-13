#include "soptocameratcpip.h"

#if _MSC_VER||WINDOWS_TCP



QMutex mutex_IfAlgorhmitcloud;

Soptocameratcpip::Soptocameratcpip()
{
    ipaddress="192.168.1.2";

    b_connect=false;
    luzhi=false;
    b_updataimage_finish=false;
    b_updatacloud_finish=false;
    b_int_show_image_inlab=false;
    callbacknumber=0;
    rcv_buf=new uchar[RECVBUFFER_MAX+1];
    cloud_buf=new uchar[RECVBUFFER_MAX+1];

    b_rcv_thread=false;
    b_stop_rcv_thread=false;
    rcv_thread = new tcprcvThread(this);

    b_cloud_thread=false;
    b_stop_cloud_thread=false;
    cloud_thread = new tcpcloudThread(this);

    ros_line=new IFAlgorhmitcloud;
}

Soptocameratcpip::~Soptocameratcpip()
{
    delete []rcv_buf;
    delete []cloud_buf;
    delete rcv_thread;
    delete cloud_thread;
    delete ros_line;
}

void Soptocameratcpip::InitConnect(PictureBox *lab_show,QString hostName, int port)
{
    if(b_connect==false)
    {
        m_lab_show=lab_show;
        std::string str = hostName.toStdString();
        const char* ch = str.c_str();
        m_client.CreateSocket();
        m_client.Connect(ch,port);
        m_client.SetBlock(0);
        if(0!=m_client.SetRcvBufferlong(RECVBUFFER_MAX))
        {
            printf("m_client setRcvBufferlong false");
        }   
/*
        m_ftp.CreateSocket();
        m_ftp.Connect(ch,PORT_ALSTCP_FTP);
        m_ftp.SetBlock(0);
        if(0!=m_ftp.SetRcvBufferlong(RECVBUFFER_MAX))
        {
            printf("m_ftp setRcvBufferlong false");
        }
*/
        b_rcv_thread=true;
        rcv_thread->start();
        connect_mod=0;
        b_connect=true;

        char data[1]={1};
        m_client.Send(data,1);
    }
}

void Soptocameratcpip::InitConnect_cloud(QString hostName,int port)
{
    if(b_connect==false)
    {
        std::string str = hostName.toStdString();
        const char* ch = str.c_str();
        m_cloud.CreateSocket();
        m_cloud.Connect(ch,port);
        m_cloud.SetBlock(0);
        if(0!=m_cloud.SetRcvBufferlong(RECVBUFFER_MAX))
        {
            printf("SetRcvBufferlong false");
        }
/*
        m_ftp.CreateSocket();
        m_ftp.Connect(ch,PORT_ALSTCP_FTP);
        m_ftp.SetBlock(0);
        if(0!=m_ftp.SetRcvBufferlong(RECVBUFFER_MAX))
        {
            printf("m_ftp setRcvBufferlong false");
        }
*/
        b_cloud_thread=true;
        cloud_thread->start();
        connect_mod=1;
        b_connect=true;

        char data[1]={1};
        m_cloud.Send(data,1);
    }
}

void Soptocameratcpip::InitConnect_all(PictureBox *lab_show,QString hostName,int img_port,int cloud_port)
{
    if(b_connect==false)
    {
        m_lab_show=lab_show;
        std::string str = hostName.toStdString();
        const char* ch = str.c_str();

        m_client.CreateSocket();
        m_client.Connect(ch,img_port);
        m_client.SetBlock(0);
        if(0!=m_client.SetRcvBufferlong(RECVBUFFER_MAX))
        {
            printf("m_client setRcvBufferlong false");
        }

        m_cloud.CreateSocket();
        m_cloud.Connect(ch,cloud_port);
        m_cloud.SetBlock(0);
        if(0!=m_cloud.SetRcvBufferlong(RECVBUFFER_MAX))
        {
            printf("SetRcvBufferlong false");
        }
/*
        m_ftp.CreateSocket();
        m_ftp.Connect(ch,PORT_ALSTCP_FTP);
        m_ftp.SetBlock(0);
        if(0!=m_ftp.SetRcvBufferlong(RECVBUFFER_MAX))
        {
            printf("m_ftp setRcvBufferlong false");
        }
*/
        b_rcv_thread=true;
        rcv_thread->start();

        b_cloud_thread=true;
        cloud_thread->start();

        connect_mod=2;
        b_connect=true;

        char data[1]={1};
        m_client.Send(data,1);
        m_cloud.Send(data,1);
    }
}

void Soptocameratcpip::DisConnect()
{
    if(b_connect==true)
    {
        char data[1]={0};
        if(connect_mod==0)
        {
            m_client.Send(data,1);
        }
        else if(connect_mod==1)
        {
            m_cloud.Send(data,1);
        }
        else if(connect_mod==2)
        {
            m_client.Send(data,1);
            m_cloud.Send(data,1);
        }
#if _MSC_VER
        _sleep(50);
#else
        usleep(50000);
#endif

        if(connect_mod==0)
        {
            rcv_thread->Stop();
            rcv_thread->quit();
            rcv_thread->wait();
            m_client.Close();
        }
        else if(connect_mod==1)
        {
            cloud_thread->Stop();
            cloud_thread->quit();
            cloud_thread->wait();
            m_cloud.Close();
        }
        else if(connect_mod==2)
        {
            rcv_thread->Stop();
            rcv_thread->quit();
            rcv_thread->wait();
            cloud_thread->Stop();
            cloud_thread->quit();
            cloud_thread->wait();

            m_client.Close();
            m_cloud.Close();
        }
//      m_ftp.Close();
        b_connect=false;
    #if _MSC_VER
        _sleep(1000);   //需要等下位机点云线程完全退出
    #elif WINDOWS_TCP
        sleep(1);   //需要等下位机点云线程完全退出
    #endif
    }
}

void Soptocameratcpip::StartRecord(QString filename)
{
    bool isColor = (cv_image.type()==CV_8UC3);
    double fps     = Getfps();
    int frameW  = cv_image.cols;
    int frameH  = cv_image.rows;
    int codec=cv::VideoWriter::fourcc('M','J','P','G');
    writer.open(filename.toStdString(),codec,fps,cv::Size(frameW,frameH),isColor);
    luzhi=true;
}

void Soptocameratcpip::StopRecord()
{
    luzhi=false;
    writer.release();
}

double Soptocameratcpip::Getfps()
{
    return 50.0;
}

void Soptocameratcpip::ros_set_homography_matrix(Params ros_Params,QTcpSocket *m_ftp)
{
    QJsonObject json;
    QJsonObject js;
    QJsonArray jarry;
    for(int i=0;i<ros_Params.homography_matrix.size();i++)
    {
        jarry.append(ros_Params.homography_matrix[i]);
    }
    json["homography_matrix"]=jarry;
    js["echo"]=json;
    QString msg=JsonToQstring(js);
    QByteArray arry=msg.toUtf8();
    arry.push_back('\0');
    m_ftp->write(arry);
}

QJsonObject Soptocameratcpip::QstringToJson(QString jsonString)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toLocal8Bit().data());
    if(jsonDocument.isNull())
    {
    //  qDebug()<< "String NULL"<< jsonString.toLocal8Bit().data();
    }
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}

QString Soptocameratcpip::JsonToQstring(QJsonObject jsonObject)
{
    return QString(QJsonDocument(jsonObject).toJson());
}

tcprcvThread::tcprcvThread(Soptocameratcpip *statci_p)
{
    _p=statci_p;
}

void tcprcvThread::run()
{
    while (1)
    {
        if(_p->b_rcv_thread==true)
        {
            int rcvnum=_p->m_client.Recv((char*)_p->rcv_buf,RECVBUFFER_MAX);
            if(rcvnum>0)
            {
                std::vector<uchar> decode;
                decode.insert(decode.end(),_p->rcv_buf,_p->rcv_buf+rcvnum);
                _p->cv_image = cv::imdecode(decode, CV_LOAD_IMAGE_COLOR);//图像解码

                if(_p->b_int_show_image_inlab==false&&_p->b_updataimage_finish==false)
                {
                    if(!_p->cv_image.empty())
                    {
                        _p->b_int_show_image_inlab=true;
                        _p->b_updataimage_finish=true;
                        _p->callbacknumber++;
                        if(_p->luzhi==true)
                        {
                            _p->writer << _p->cv_image;
                        }
                    }
                }
            }
        }
        else
        {
            _p->b_stop_rcv_thread=true;
            break;
        }
        sleep(0);
    }
}

void tcprcvThread::Stop()
{
    if(_p->b_rcv_thread==true)
    {
        _p->b_stop_rcv_thread=false;
        _p->b_rcv_thread=false;
        while (_p->b_stop_rcv_thread==false)
        {
            sleep(0);
        }
    }
}

tcpcloudThread::tcpcloudThread(Soptocameratcpip *statci_p)
{
    _p=statci_p;
}

void tcpcloudThread::run()
{
    while (1)
    {
        if(_p->b_cloud_thread==true)
        {
            int rcvnum=_p->m_cloud.Recv((char*)_p->cloud_buf,RECVBUFFER_MAX);
            if(rcvnum>0)
            {
                if(rcvnum>=1)
                {
                    int n=rcvnum;
                    static std::string s_rcvmsg;
                    if(_p->cloud_buf[n-1]!='\0')
                    {
                        _p->cloud_buf[n]='\0';
                        std::string rcvmsg=(char*)_p->cloud_buf;
                        s_rcvmsg=s_rcvmsg+rcvmsg;
                    }
                    else
                    {
                        std::string rcvmsg=(char*)_p->cloud_buf;
                        s_rcvmsg=s_rcvmsg+rcvmsg;

                        mutex_IfAlgorhmitcloud.lock();//锁住线程，防止在修改IfAlgorhmitcloud内存时被其他线程调用

                        QString msg= QString::fromStdString(s_rcvmsg);
                        s_rcvmsg.clear();
                        QJsonObject json=_p->QstringToJson(msg);
                        QJsonObject header=json["header"].toObject();
                        QJsonObject stamp=header["stamp"].toObject();
                        _p->ros_line->header.stamp.sec=stamp["sec"].toInt();
                        _p->ros_line->header.stamp.nanosec=stamp["nanosec"].toInt();
                        _p->ros_line->header.frame_id=header["frame_id"].toString().toStdString();
                        QJsonArray lasertrackoutcloud=json["lasertrackoutcloud"].toArray();
                        _p->ros_line->lasertrackoutcloud.resize(lasertrackoutcloud.size());
                        for(int i=0;i<lasertrackoutcloud.size();i++)
                        {
                            QJsonObject qtask=lasertrackoutcloud[i].toObject();
                            _p->ros_line->lasertrackoutcloud[i].x=qtask["x"].toDouble();
                            _p->ros_line->lasertrackoutcloud[i].y=qtask["y"].toDouble();
                            _p->ros_line->lasertrackoutcloud[i].u=qtask["u"].toInt();
                            _p->ros_line->lasertrackoutcloud[i].v=qtask["v"].toInt();
                        }
                        QJsonArray targetpointoutcloud=json["targetpointoutcloud"].toArray();
                        _p->ros_line->targetpointoutcloud.resize(targetpointoutcloud.size());
                        for(int i=0;i<targetpointoutcloud.size();i++)
                        {
                            QJsonObject qtask=targetpointoutcloud[i].toObject();
                            _p->ros_line->targetpointoutcloud[i].x=qtask["x"].toDouble();
                            _p->ros_line->targetpointoutcloud[i].y=qtask["y"].toDouble();
                            _p->ros_line->targetpointoutcloud[i].u=qtask["u"].toInt();
                            _p->ros_line->targetpointoutcloud[i].v=qtask["v"].toInt();
                            _p->ros_line->targetpointoutcloud[i].name=qtask["name"].toString().toStdString();
                        }
                        _p->ros_line->solderjoints=json["solderjoints"].toBool();

                        QJsonObject robpos=json["robpos"].toObject();
                        QJsonObject robposheader=robpos["header"].toObject();
                        QJsonObject robposstamp=robposheader["stamp"].toObject();
                        _p->ros_line->robpos.header.stamp.sec=robposstamp["sec"].toInt();
                        _p->ros_line->robpos.header.stamp.nanosec=robposstamp["nanosec"].toInt();
                        _p->ros_line->robpos.header.frame_id=robposheader["frame_id"].toString().toStdString();
                        _p->ros_line->robpos.posx=robpos["posx"].toDouble();
                        _p->ros_line->robpos.posy=robpos["posy"].toDouble();
                        _p->ros_line->robpos.posz=robpos["posz"].toDouble();
                        _p->ros_line->robpos.posrx=robpos["posrx"].toDouble();
                        _p->ros_line->robpos.posry=robpos["posry"].toDouble();
                        _p->ros_line->robpos.posrz=robpos["posrz"].toDouble();
                        _p->ros_line->robpos.posout1=robpos["posout1"].toDouble();
                        _p->ros_line->robpos.posout2=robpos["posout2"].toDouble();
                        _p->ros_line->robpos.posout3=robpos["posout3"].toDouble();
                        _p->ros_line->robpos.toolid=robpos["toolid"].toInt();
                        _p->ros_line->robpos.tcpid=robpos["tcpid"].toInt();
                        _p->ros_line->robpos.usertcpid=robpos["usertcpid"].toInt();

                        if(lasertrackoutcloud.size()>0)
                        {
                            _p->b_ros_lineEn=true;
                        }
                        else
                        {
                            _p->b_ros_lineEn=false;
                        }

                        _p->b_updatacloud_finish=true;
                        mutex_IfAlgorhmitcloud.unlock();//解锁线程

                        s_rcvmsg.clear();
                    }
                }


            }
        }
        else
        {
            _p->b_stop_cloud_thread=true;
            break;
        }
        sleep(0);
    }
}

void tcpcloudThread::Stop()
{
    if(_p->b_cloud_thread==true)
    {
        _p->b_stop_cloud_thread=false;
        _p->b_cloud_thread=false;
        while (_p->b_stop_cloud_thread==false)
        {
            sleep(0);
        }
    }
}
#endif
