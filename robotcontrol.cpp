#include "robotcontrol.h"

QMutex mutexsend_buf_group;
QMutex mutextotalcontrol_buf_group;
extern QMutex main_record;

Robotcontrol *Robotcontrol::Get(my_parameters *mcs)
{
    static Robotcontrol fun;
    fun.m_mcs=mcs;
    return &fun;
}

Robotcontrol::Robotcontrol()
{
    server_state=false;
    b_stop_server_state=false;
    link_state=false;
    b_stop_link_state=false;
    b_client=false;
    b_rcv_thread=false;
    b_stop_rcv_thread=false;
    rcv_buf=NULL;
    b_sendent=false;
    b_send_thread=false;
    b_stop_send_thread=false;
    b_sendrcv_thread=false;
    b_stop_sendrcv_thread=false;
    sendrcv_buf=NULL;
    b_totalcontrolent=false;
    b_totalcontrol_Thread=false;
    b_stop_totalcontrol_Thread=false;
    totalcontrolrcv_buf=NULL;
    b_totalcontrolrcv_Thread=false;
    b_stop_totalcontrolrcv_Thread=false;

    rob_mod=ROBOT_MODEL_NULL;
}

Robotcontrol::~Robotcontrol()
{

}

void Robotcontrol::Creat_control_modbus()
{
    if(server_state==false)
    {
        thread1 = new RobotcontrolThread1(this);
        mb_mapping = modbus_mapping_new(0, 0,ROB_CONTROL_REG_TOTALNUM, 0);
        server_state=true;
        thread1->start();

        linkthread=new RobotlinkThread(this);
        link_state=true;
        linkthread->start();

        rcv_thread = new RobotrcvThread(this);
        send_Thread = new RobotsendThread(this);
        sendrcv_Thread = new RobotsendrcvThread(this);
        totalcontrol_Thread = new RobottotalcontrolThread(this);
        totalcontrolrcv_Thread = new RobottotalcontrolrcvThread(this);

        usleep(100000);
    }
}

void Robotcontrol::Close_control_modbus()
{
    if(server_state==true)
    {
        b_stop_link_state=false;
        link_state=false;
        while (b_stop_link_state==false)
        {
          sleep(0);
        }

        if(b_client==true)
        {
            rcv_thread->Stop();
            rcv_thread->quit();
            rcv_thread->wait();
            m_client.Close();
            if(rcv_buf!=NULL)
            {
                delete []rcv_buf;
                rcv_buf=NULL;
            }
            b_client=false;
        }
        if(b_sendent==true)
        {
            send_Thread->Stop();
            send_Thread->quit();
            send_Thread->wait();
        #ifdef OPEN_SHOW_ROBOTSOCKDATA
            sendrcv_Thread->Stop();
            sendrcv_Thread->quit();
            sendrcv_Thread->wait();
        #endif
            m_sendent.Close();
        #ifdef OPEN_SHOW_ROBOTSOCKDATA
            if(sendrcv_buf!=NULL)
            {
                delete []sendrcv_buf;
                sendrcv_buf=NULL;
            }
        #endif
            b_sendent=false;
        }
        if(b_totalcontrolent==true)
        {
            totalcontrol_Thread->Stop();
            totalcontrol_Thread->quit();
            totalcontrol_Thread->wait();
        #ifdef OPEN_SHOW_ROBOTSOCKDATA
            totalcontrolrcv_Thread->Stop();
            totalcontrolrcv_Thread->quit();
            totalcontrolrcv_Thread->wait();
        #endif
            m_totalcontrolent.Close();
        #ifdef OPEN_SHOW_ROBOTSOCKDATA
            if(totalcontrolrcv_buf!=NULL)
            {
                delete []totalcontrolrcv_buf;
                totalcontrolrcv_buf=NULL;
            }
        #endif
            b_totalcontrolent=false;
        }

        linkthread->quit();
        linkthread->wait();

        b_stop_server_state=false;
        server_state=false;
        while (b_stop_server_state==false)
        {
          sleep(0);
        }
        thread1->quit();
        thread1->wait();
        close(sock);

        delete rcv_thread;
        delete send_Thread;
        delete sendrcv_Thread;
        delete totalcontrol_Thread;
        delete totalcontrolrcv_Thread;
        delete linkthread;
        delete thread1;

    }
}

void Robotcontrol:: RobotInit()//机器人初始化
{
    switch(rob_mod)
    {
        case ROBOT_MODEL_NULL://无机器人
        {

        }
        break;
        case ROBOT_MODEL_EMERGEN://智昌机器人
        {

        }
        break;
        case ROBOT_MODEL_DOBOT://越彊机器人
        {
            if(b_totalcontrol_Thread==true)
            {
                mutextotalcontrol_buf_group.lock();
                QString msg="CP(1)";    //机器人上电使能
                std::string str=msg.toStdString();
                totalcontrol_buf_group.push_back(str);
                mutextotalcontrol_buf_group.unlock();
            }
        }
        break;
    }
}

void Robotcontrol::RobotOPEN_ELE()
{
    switch(rob_mod)
    {
        case ROBOT_MODEL_NULL://无机器人
        {

        }
        break;
        case ROBOT_MODEL_EMERGEN://智昌机器人
        {

        }
        break;
        case ROBOT_MODEL_DOBOT://越彊机器人
        {
            if(b_totalcontrol_Thread==true)
            {
                mutexsend_buf_group.lock();
                QString msg="EnableRobot()";    //机器人上电使能
                std::string str=msg.toStdString();
                totalcontrol_buf_group.push_back(str);
                mutexsend_buf_group.unlock();
                sleep(3);
            }
        }
        break;
    }
}

void Robotcontrol::RobotCLOSE_ELE()
{
    switch(rob_mod)
    {
        case ROBOT_MODEL_NULL://无机器人
        {

        }
        break;
        case ROBOT_MODEL_EMERGEN://智昌机器人
        {

        }
        break;
        case ROBOT_MODEL_DOBOT://越彊机器人
        {
            if(b_totalcontrol_Thread==true)
            {
                mutexsend_buf_group.lock();
                QString msg="ResetRobot()";    //机器人停止
                std::string str=msg.toStdString();
                totalcontrol_buf_group.push_back(str);
                mutexsend_buf_group.unlock();
                sleep(3);
            }
        }
        break;
    }
}

RobotcontrolThread1::RobotcontrolThread1(Robotcontrol *statci_p)
{
    _p=statci_p;
}

void RobotcontrolThread1::run() //接到上位机命令
{
    QString server_port=QString::number(_p->m_mcs->ip->robotmyselfcontrol_port[0]);
    _p->ctx_robotcontrol = modbus_new_tcp(NULL, server_port.toInt());
    _p->sock = modbus_tcp_listen(_p->ctx_robotcontrol, 2);//最大监听2路
    std::set<int> fds {_p->sock};
    fd_set refset;
    FD_ZERO(&refset);
    FD_SET(_p->sock, &refset);
    int fdmax = _p->sock;
    while(_p->server_state==true)
    {
        auto rdset = refset;
        timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 100000;
        int ret = select(fdmax + 1, &rdset, NULL, NULL, &tv);
        if(ret==-1)
        {
            main_record.lock();
            QString return_msg=QString::fromLocal8Bit("机器人控制服务器申请失败");
            _p->m_mcs->main_record.push_back(return_msg);
            main_record.unlock();
            continue;
        }
        else if(ret==0)
        {
            continue;//时间到了
        }
        auto fds_bak = fds;
        for (auto fd : fds_bak)
        {
            if (!FD_ISSET(fd, &rdset))
            {continue;}

            if (fd == _p->sock)
            {
                ret = modbus_tcp_accept(_p->ctx_robotcontrol, &_p->sock);
                if (ret != -1)
                {
                    FD_SET(ret, &refset);
                    fds.insert(fds.end(), ret);
                    fdmax = *fds.rbegin();
                }
                else
                {
                    main_record.lock();
                    QString return_msg=QString::fromLocal8Bit("机器人控制服务器建立连接失败");
                    _p->m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    break;
                }
            }
            else
            {
                ret = modbus_set_socket(_p->ctx_robotcontrol, fd);
                if (ret == -1)
                {
                    main_record.lock();
                    QString return_msg=QString::fromLocal8Bit("机器人控制服务器设置socket三失败");
                    _p->m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                    break;
                }
                uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
                ret = modbus_receive(_p->ctx_robotcontrol, query);
                if (ret == -1)
                {
                    // Connection closed by the client or error
                    close(fd);
                    FD_CLR(fd, &refset);
                    fds.erase(fd);
                    fdmax = *fds.rbegin();
                    ret = 0;
                }
                else if (ret > 0)
                {
                    _p->mb_mapping->tab_registers[ROB_MOVEMOD_REG_ADD]=65535;
                    _p->mb_mapping->tab_registers[ROB_STOP_REG_ADD]=65535;
                    int rc=modbus_reply(_p->ctx_robotcontrol, query, ret, _p->mb_mapping);
                    if(rc>=0)
                    {
                        if(_p->b_send_thread==true)//远端机器人发送命令已经启动
                        {
                            if(_p->mb_mapping->tab_registers[ROB_MOVEMOD_REG_ADD]!=65535)//机器人要移动
                            {
                                uint16_t tcp=_p->mb_mapping->tab_registers[ROB_TCP_NUM_REG_ADD];
                                Robmovemodel_ID movemod=(Robmovemodel)_p->mb_mapping->tab_registers[ROB_MOVEMOD_REG_ADD];
                                uint16_t u16data_elec_work=_p->mb_mapping->tab_registers[ROB_MOVEFIER_REG_ADD];//加起弧判断
                                float f_speed=*(float*)&_p->mb_mapping->tab_registers[ROB_MOVESPEED_FH_REG_ADD];
                                float f_movX=*(float*)&_p->mb_mapping->tab_registers[ROB_MOVE_X_POS_FH_REG_ADD];
                                float f_movY=*(float*)&_p->mb_mapping->tab_registers[ROB_MOVE_Y_POS_FH_REG_ADD];
                                float f_movZ=*(float*)&_p->mb_mapping->tab_registers[ROB_MOVE_Z_POS_FH_REG_ADD];
                                float f_movRX=*(float*)&_p->mb_mapping->tab_registers[ROB_MOVE_RX_POS_FH_REG_ADD];
                                float f_movRY=*(float*)&_p->mb_mapping->tab_registers[ROB_MOVE_RY_POS_FH_REG_ADD];
                                float f_movRZ=*(float*)&_p->mb_mapping->tab_registers[ROB_MOVE_RZ_POS_FH_REG_ADD];

                                switch(_p->rob_mod)
                                {
                                    case ROBOT_MODEL_NULL://无机器人
                                    {

                                    }
                                    break;
                                    case ROBOT_MODEL_EMERGEN://智昌机器人
                                    {

                                    }
                                    break;
                                    case ROBOT_MODEL_DOBOT://越彊机器人
                                    {
                                        switch(movemod)
                                        {
                                            case MOVEL:
                                            {
                                                mutexsend_buf_group.lock();
                                                QString msg="MovL("+QString::number(f_movX,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                    QString::number(f_movY,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                    QString::number(f_movZ,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                    QString::number(f_movRX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                                    QString::number(f_movRY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                                    QString::number(f_movRZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+",User=0,Tool="+
                                                                    QString::number(tcp)+",SpeedL="+
                                                                    QString::number((int)f_speed)+")";
                                                std::string str=msg.toStdString();
                                            //  std::string str="MovL(1.3,23,45,6,7,8,User=0,Tool=2,SpeedL=1.2)";
                                                _p->send_buf_group.push_back(str);
                                                mutexsend_buf_group.unlock();
                                            }
                                            break;
                                            case MOVEJ:
                                            {
                                                mutexsend_buf_group.lock();
                                                QString msg="MovJ("+QString::number(f_movX,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                    QString::number(f_movY,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                    QString::number(f_movZ,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                    QString::number(f_movRX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                                    QString::number(f_movRY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                                    QString::number(f_movRZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+",User=0,Tool="+
                                                                    QString::number(tcp)+")";
                                                std::string str=msg.toStdString();
                                            //  std::string str="MovJ(1.3,23,45,6,7,8,User=0,Tool=2)";
                                                _p->send_buf_group.push_back(str);
                                                mutexsend_buf_group.unlock();
                                            }
                                            break;
                                        }
                                    }
                                    break;
                                }
                            }
                            if(_p->mb_mapping->tab_registers[ROB_STOP_REG_ADD]!=65535)//机器人要暂停或继续运行
                            {
                                uint16_t stopreg=_p->mb_mapping->tab_registers[ROB_STOP_REG_ADD];
                                switch(stopreg)
                                {
                                    case 0://机器人可以清空运行缓存脚本,并准备运行
                                    {
                                        switch(_p->rob_mod)
                                        {
                                            case ROBOT_MODEL_NULL://无机器人
                                            {

                                            }
                                            break;
                                            case ROBOT_MODEL_EMERGEN://智昌机器人
                                            {

                                            }
                                            break;
                                            case ROBOT_MODEL_DOBOT://越彊机器人
                                            {
                                                mutexsend_buf_group.lock();
                                                QString msg="StopScript()";
                                                std::string str=msg.toStdString();
                                                _p->totalcontrol_buf_group.push_back(str);
                                                mutexsend_buf_group.unlock();
                                            }
                                            break;
                                        }
                                    }
                                    break;
                                    case 1://机器人暂停缓存脚本
                                    {
                                        switch(_p->rob_mod)
                                        {
                                            case ROBOT_MODEL_NULL://无机器人
                                            {

                                            }
                                            break;
                                            case ROBOT_MODEL_EMERGEN://智昌机器人
                                            {

                                            }
                                            break;
                                            case ROBOT_MODEL_DOBOT://越彊机器人
                                            {
                                                mutexsend_buf_group.lock();
                                                QString msg="PauseScript()";
                                                std::string str=msg.toStdString();
                                                _p->totalcontrol_buf_group.push_back(str);
                                                mutexsend_buf_group.unlock();
                                            }
                                            break;
                                        }
                                    }
                                    break;
                                    case 2://机器人继续缓存脚本
                                    {
                                        switch(_p->rob_mod)
                                        {
                                            case ROBOT_MODEL_NULL://无机器人
                                            {

                                            }
                                            break;
                                            case ROBOT_MODEL_EMERGEN://智昌机器人
                                            {

                                            }
                                            break;
                                            case ROBOT_MODEL_DOBOT://越彊机器人
                                            {
                                                mutexsend_buf_group.lock();
                                                QString msg="ContinueScript()";
                                                std::string str=msg.toStdString();
                                                _p->totalcontrol_buf_group.push_back(str);
                                                mutexsend_buf_group.unlock();
                                            }
                                            break;
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
                sleep(0);
            }
        }
    }
    close(_p->sock);
    modbus_close(_p->ctx_robotcontrol);
    modbus_free(_p->ctx_robotcontrol);
    _p->b_stop_server_state=true;
}

RobotlinkThread::RobotlinkThread(Robotcontrol *statci_p)
{
    _p=statci_p;
}

void RobotlinkThread::run() //连接机器人命令
{
    while(_p->link_state==true)
    {
        sleep(5);//每隔5秒查看一次机器人
        static QString old_rodb_ip="0.0.0.0";
        u_int16_t ip[4];
        ip[0]=_p->mb_mapping->tab_registers[ROB_IPADDR_1_REG_ADD];
        ip[1]=_p->mb_mapping->tab_registers[ROB_IPADDR_2_REG_ADD];
        ip[2]=_p->mb_mapping->tab_registers[ROB_IPADDR_3_REG_ADD];
        ip[3]=_p->mb_mapping->tab_registers[ROB_IPADDR_4_REG_ADD];
        QString rodb_ip=QString::number(ip[0])+"."+
                        QString::number(ip[1])+"."+
                        QString::number(ip[2])+"."+
                        QString::number(ip[3]);
        static ROBOT_MODEL old_rob_mod=ROBOT_MODEL_NULL;
        _p->rob_mod=(ROBOT_MODEL)_p->mb_mapping->tab_registers[ROB_MODEL_REG_ADD];

        if(old_rodb_ip!=rodb_ip||old_rob_mod!=_p->rob_mod)
        {
            if(_p->b_client==true)
            {
                _p->rcv_thread->Stop();
                _p->rcv_thread->quit();
                _p->rcv_thread->wait();
                _p->m_client.Close();
                if(_p->rcv_buf!=NULL)
                {
                    delete []_p->rcv_buf;
                    _p->rcv_buf=NULL;
                }
                _p->b_client=false;
            }
            if(_p->b_sendent==true)
            {
                _p->send_Thread->Stop();
                _p->send_Thread->quit();
                _p->send_Thread->wait();
            #ifdef OPEN_SHOW_ROBOTSOCKDATA
                _p->sendrcv_Thread->Stop();
                _p->sendrcv_Thread->quit();
                _p->sendrcv_Thread->wait();
            #endif

                _p->m_sendent.Close();
            #ifdef OPEN_SHOW_ROBOTSOCKDATA
                if(_p->sendrcv_buf!=NULL)
                {
                    delete []_p->sendrcv_buf;
                    _p->sendrcv_buf=NULL;
                }
            #endif
                _p->b_sendent=false;
            }
            if(_p->b_totalcontrolent==true)
            {
                _p->totalcontrol_Thread->Stop();
                _p->totalcontrol_Thread->quit();
                _p->totalcontrol_Thread->wait();
            #ifdef OPEN_SHOW_ROBOTSOCKDATA
                _p->totalcontrolrcv_Thread->Stop();
                _p->totalcontrolrcv_Thread->quit();
                _p->totalcontrolrcv_Thread->wait();
            #endif
                _p->m_totalcontrolent.Close();
            #ifdef OPEN_SHOW_ROBOTSOCKDATA
                if(_p->totalcontrolrcv_buf!=NULL)
                {
                    delete []_p->totalcontrolrcv_buf;
                    _p->totalcontrolrcv_buf=NULL;
                }
            #endif
                _p->b_totalcontrolent=false;
            }
            switch(_p->rob_mod)
            {
                case ROBOT_MODEL_NULL://无机器人
                {

                }
                break;
                case ROBOT_MODEL_EMERGEN://智昌机器人
                {

                }
                break;
                case ROBOT_MODEL_DOBOT://越彊机器人
                {
                    _p->m_client.CreateSocket();
                    if(false==_p->m_client.Connect(rodb_ip.toStdString().c_str(),ROBOT_DOBOT_INFO_PORT))
                    {
                        main_record.lock();
                        QString return_msg=QString::fromLocal8Bit("与远端机器人数据端口连接失败");
                        _p->m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        continue;
                    }
                    _p->m_client.SetBlock(0);
                    if(0!=_p->m_client.SetRcvBufferlong(ROBOT_DOBOT_INFO_RECVBUFFER_MAX*2))
                    {
                        main_record.lock();
                        QString return_msg=QString::fromLocal8Bit("接收远端机器人数据缓存申请失败");
                        _p->m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        continue;
                    }
                    _p->b_client=true;

                    _p->rcv_buf=new uint8_t[ROBOT_DOBOT_INFO_RECVBUFFER_MAX*2+1];
                    _p->b_rcv_thread=true;
                    _p->rcv_thread->start();

                    _p->m_sendent.CreateSocket();
                    if(false==_p->m_sendent.Connect(rodb_ip.toStdString().c_str(),ROBOT_DOBOT_SEND_PORT))
                    {
                        main_record.lock();
                        QString return_msg=QString::fromLocal8Bit("与远端机器人命令端口连接失败");
                        _p->m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        continue;
                    }
                    _p->m_sendent.SetBlock(0);
                #ifdef OPEN_SHOW_ROBOTSOCKDATA
                    _p->sendrcv_buf=new uint8_t[ROBOT_DOBOT_INFO_SENDRECVBUFFER_MAX*2+1];
                    if(0!=_p->m_sendent.SetRcvBufferlong(ROBOT_DOBOT_INFO_SENDRECVBUFFER_MAX*2))
                    {
                        main_record.lock();
                        QString return_msg=QString::fromLocal8Bit("接收远端机器人命令缓存申请失败");
                        _p->m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        continue;
                    }
                #endif
                    _p->send_buf_group.clear();
                    _p->b_sendent=true;
                    _p->b_send_thread=true;
                    _p->send_Thread->start();
                #ifdef OPEN_SHOW_ROBOTSOCKDATA
                    _p->b_sendrcv_thread=true;
                    _p->sendrcv_Thread->start();
                #endif
                    _p->m_totalcontrolent.CreateSocket();
                    if(false==_p->m_totalcontrolent.Connect(rodb_ip.toStdString().c_str(),ROBOT_DOBOT_TOTALCONTROL_RORT))
                    {
                        main_record.lock();
                        QString return_msg=QString::fromLocal8Bit("与远端机器人总控端口连接失败");
                        _p->m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        continue;
                    }
                    _p->m_totalcontrolent.SetBlock(0);
                #ifdef OPEN_SHOW_ROBOTSOCKDATA
                    _p->totalcontrolrcv_buf=new uint8_t[ROBOT_DOBOT_INFO_TOTALCONTROLCVBUFFER_MAX*2+1];
                    if(0!=_p->m_totalcontrolent.SetRcvBufferlong(ROBOT_DOBOT_INFO_TOTALCONTROLCVBUFFER_MAX*2))
                    {
                        main_record.lock();
                        QString return_msg=QString::fromLocal8Bit("接收远端机器人总控缓存申请失败");
                        _p->m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        continue;
                    }
                #endif
                    _p->totalcontrol_buf_group.clear();
                    _p->b_totalcontrolent=true;
                    _p->b_totalcontrol_Thread=true;
                    _p->totalcontrol_Thread->start();
                #ifdef OPEN_SHOW_ROBOTSOCKDATA
                    _p->b_totalcontrolrcv_Thread=true;
                    _p->totalcontrolrcv_Thread->start();
                #endif
                    old_rodb_ip=rodb_ip;
                    old_rob_mod=_p->rob_mod;

                }
                break;
            }
            _p->RobotInit();
            main_record.lock();
            QString return_msg=QString::fromLocal8Bit("机器人状态获取成功");
            _p->m_mcs->main_record.push_back(return_msg);
            main_record.unlock();
        }
    }
    _p->b_stop_link_state=true;
}

RobotrcvThread::RobotrcvThread(Robotcontrol *statci_p)
{
    _p=statci_p;
}

void RobotrcvThread::run()//获取机器人数据
{
    while (1)
    {
        if(_p->b_rcv_thread==true)
        {
            switch(_p->rob_mod)
            {
                case ROBOT_MODEL_NULL://无机器人
                {

                }
                break;
                case ROBOT_MODEL_EMERGEN://智昌机器人
                {

                }
                break;
                case ROBOT_MODEL_DOBOT://越彊机器人
                {
                    int rcvnum=_p->m_client.Recv((char*)_p->rcv_buf,ROBOT_DOBOT_INFO_RECVBUFFER_MAX*2);
                    if(rcvnum>0)
                    {
                        double d_robX=*(double*)(&_p->rcv_buf[624]);
                        double d_robY=*(double*)(&_p->rcv_buf[632]);
                        double d_robZ=*(double*)(&_p->rcv_buf[640]);
                        double d_robRX=*(double*)(&_p->rcv_buf[648]);
                        double d_robRY=*(double*)(&_p->rcv_buf[656]);
                        double d_robRZ=*(double*)(&_p->rcv_buf[664]);
                        double d_speed=*(double*)(&_p->rcv_buf[672]);
                        uint8_t state=_p->rcv_buf[1028];//运行状态 只有0停止和1运动

                        float f_robX=d_robX;
                        float f_robY=d_robY;
                        float f_robZ=d_robZ;
                        float f_robRX=d_robRX;
                        float f_robRY=d_robRY;
                        float f_robRZ=d_robRZ;
                        float f_speed=d_speed;


                        _p->mb_mapping->tab_registers[ROB_X_POS_FH_REG_ADD]=((uint16_t*)(&f_robX))[0];
                        _p->mb_mapping->tab_registers[ROB_X_POS_FL_REG_ADD]=((uint16_t*)(&f_robX))[1];
                        _p->mb_mapping->tab_registers[ROB_Y_POS_FH_REG_ADD]=((uint16_t*)(&f_robY))[0];
                        _p->mb_mapping->tab_registers[ROB_Y_POS_FL_REG_ADD]=((uint16_t*)(&f_robY))[1];
                        _p->mb_mapping->tab_registers[ROB_Z_POS_FH_REG_ADD]=((uint16_t*)(&f_robZ))[0];
                        _p->mb_mapping->tab_registers[ROB_Z_POS_FL_REG_ADD]=((uint16_t*)(&f_robZ))[1];
                        _p->mb_mapping->tab_registers[ROB_RX_POS_FH_REG_ADD]=((uint16_t*)(&f_robRX))[0];
                        _p->mb_mapping->tab_registers[ROB_RX_POS_FL_REG_ADD]=((uint16_t*)(&f_robRX))[1];
                        _p->mb_mapping->tab_registers[ROB_RY_POS_FH_REG_ADD]=((uint16_t*)(&f_robRY))[0];
                        _p->mb_mapping->tab_registers[ROB_RY_POS_FL_REG_ADD]=((uint16_t*)(&f_robRY))[1];
                        _p->mb_mapping->tab_registers[ROB_RZ_POS_FH_REG_ADD]=((uint16_t*)(&f_robRZ))[0];
                        _p->mb_mapping->tab_registers[ROB_RZ_POS_FL_REG_ADD]=((uint16_t*)(&f_robRZ))[1];
                        _p->mb_mapping->tab_registers[ROB_SPEED_FH_REG_ADD]=((uint16_t*)(&f_speed))[0];
                        _p->mb_mapping->tab_registers[ROB_SPEED_FL_REG_ADD]=((uint16_t*)(&f_speed))[1];
                        _p->mb_mapping->tab_registers[ROB_STATE_REG_ADD]=state;

                    }
                }
                break;
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

void RobotrcvThread::Stop()
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


RobotsendThread::RobotsendThread(Robotcontrol *statci_p)//发送给机器人命令
{
    _p=statci_p;
}

void RobotsendThread::run()
{
    while (1)
    {
        if(_p->b_send_thread==true)
        {
            mutexsend_buf_group.lock();
            if(_p->send_buf_group.size()>0)
            {
                std::string send_buf=_p->send_buf_group[0];
                _p->send_buf_group.erase(_p->send_buf_group.begin());
                if(send_buf.size()!=_p->m_sendent.Send(send_buf.c_str(),send_buf.size()))
                {
                    main_record.lock();
                    QString return_msg=QString::fromLocal8Bit("远端机器人发送命令失败");
                    _p->m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                }
            #ifdef OPEN_SHOW_ROBOTSOCKDATA
                main_record.lock();
                QString return_msg=send_buf.c_str();
                _p->m_mcs->main_record.push_back("SendtoRob:"+return_msg);
                main_record.unlock();
            #endif
            }
            mutexsend_buf_group.unlock();
        }
        else
        {
            _p->b_stop_send_thread=true;
            break;
        }
        sleep(0);
    }
}

void RobotsendThread::Stop()
{
  if(_p->b_send_thread==true)
  {
    _p->b_stop_send_thread=false;
    _p->b_send_thread=false;
    while (_p->b_stop_send_thread==false)
    {
      sleep(0);
    }
  }
}

RobotsendrcvThread::RobotsendrcvThread(Robotcontrol *statci_p)
{
    _p=statci_p;
}

void RobotsendrcvThread::run()//获取机器人命令回复数据
{
    while (1)
    {
        if(_p->b_sendrcv_thread==true)
        {
            switch(_p->rob_mod)
            {
                case ROBOT_MODEL_NULL://无机器人
                {

                }
                break;
                case ROBOT_MODEL_EMERGEN://智昌机器人
                {

                }
                break;
                case ROBOT_MODEL_DOBOT://越彊机器人
                {
                    int rcvnum=_p->m_sendent.Recv((char*)_p->sendrcv_buf,ROBOT_DOBOT_INFO_SENDRECVBUFFER_MAX*2);
                    if(rcvnum>0)
                    {
                        main_record.lock();
                        _p->sendrcv_buf[rcvnum]='\0';
                        QString return_msg=(char*)_p->sendrcv_buf;
                        _p->m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                    }
                }
                break;
            }
        }
        else
        {
            _p->b_stop_sendrcv_thread=true;
            break;
        }
        sleep(0);
    }
}

void RobotsendrcvThread::Stop()
{
  if(_p->b_sendrcv_thread==true)
  {
    _p->b_stop_sendrcv_thread=false;
    _p->b_sendrcv_thread=false;
    while (_p->b_stop_sendrcv_thread==false)
    {
      sleep(0);
    }
  }
}

RobottotalcontrolThread::RobottotalcontrolThread(Robotcontrol *statci_p)//发送给机器人命令
{
    _p=statci_p;
}

void RobottotalcontrolThread::run()
{
    while (1)
    {
        if(_p->b_totalcontrol_Thread==true)
        {
            mutextotalcontrol_buf_group.lock();
            if(_p->totalcontrol_buf_group.size()!=0)
            {
                std::string totalcontrolent_buf=_p->totalcontrol_buf_group[0];
                std::vector<std::string>::iterator it = _p->totalcontrol_buf_group.begin();
                _p->totalcontrol_buf_group.erase(it);
                if(totalcontrolent_buf.size()!=_p->m_totalcontrolent.Send(totalcontrolent_buf.c_str(),totalcontrolent_buf.size()))
                {
                    main_record.lock();
                    QString return_msg=QString::fromLocal8Bit("远端机器人发送总控命令失败");
                    _p->m_mcs->main_record.push_back(return_msg);
                    main_record.unlock();
                }
            #ifdef OPEN_SHOW_ROBOTSOCKDATA
                main_record.lock();
                QString return_msg=totalcontrolent_buf.c_str();
                _p->m_mcs->main_record.push_back("SendtoRob:"+return_msg);
                main_record.unlock();
            #endif
            }
            mutextotalcontrol_buf_group.unlock();
        }
        else
        {
            _p->b_stop_totalcontrol_Thread=true;
            break;
        }
        sleep(0);
    }
}

void RobottotalcontrolThread::Stop()
{
  if(_p->b_totalcontrol_Thread==true)
  {
    _p->b_stop_totalcontrol_Thread=false;
    _p->b_totalcontrol_Thread=false;
    while (_p->b_stop_totalcontrol_Thread==false)
    {
      sleep(0);
    }
  }
}

RobottotalcontrolrcvThread::RobottotalcontrolrcvThread(Robotcontrol *statci_p)
{
    _p=statci_p;
}

void RobottotalcontrolrcvThread::run()//获取机器人总控回复数据
{
    while (1)
    {
        if(_p->b_totalcontrolrcv_Thread==true)
        {
            switch(_p->rob_mod)
            {
                case ROBOT_MODEL_NULL://无机器人
                {

                }
                break;
                case ROBOT_MODEL_EMERGEN://智昌机器人
                {

                }
                break;
                case ROBOT_MODEL_DOBOT://越彊机器人
                {
                    int rcvnum=_p->m_totalcontrolent.Recv((char*)_p->totalcontrolrcv_buf,ROBOT_DOBOT_INFO_TOTALCONTROLCVBUFFER_MAX*2);
                    if(rcvnum>0)
                    {
                        main_record.lock();
                        _p->totalcontrolrcv_buf[rcvnum]='\0';
                        QString return_msg=(char*)_p->totalcontrolrcv_buf;
                        _p->m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                    }
                }
                break;
            }
        }
        else
        {
            _p->b_stop_sendrcv_thread=true;
            break;
        }
        sleep(0);
    }
}

void RobottotalcontrolrcvThread::Stop()
{
  if(_p->b_sendrcv_thread==true)
  {
    _p->b_stop_sendrcv_thread=false;
    _p->b_sendrcv_thread=false;
    while (_p->b_stop_sendrcv_thread==false)
    {
      sleep(0);
    }
  }
}

