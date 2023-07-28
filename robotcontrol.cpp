#include "robotcontrol.h"

QMutex mutexsend_buf_group;
QMutex mutextotalcontrol_buf_group;
QMutex mutexweldsend_buf_group;
QMutex mutexmovepoint_buffer_group;
extern QMutex main_record;

MovRobPos::MovRobPos()
{
    X=0;
    Y=0;
    Z=0;
    RX=0;
    RY=0;
    RZ=0;
    X1=0;
    Y1=0;
    Z1=0;
    RX1=0;
    RY1=0;
    RZ1=0;
    nEn=0;
}

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
    b_relink=false;
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

    b_startweld_init=false;   //刚起弧初始化变量标记

    b_welding=false;


    rob_mod=ROBOT_MODEL_NULL;
/******************************************/
    //以下焊机接口(非机器人直连)
    b_weldsendent=false;
    b_weldsendrcv_thread=false;
    b_stop_weldsendrcv_thread=false;
    weldsendrcv_buf=NULL;

    weld_mod=WELD_MODEL_NULL;
/*************************************/
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
        weldsend_Thread = new WeldsendThread(this);
        weldsendrcv_Thread = new WeldsendrcvThread(this);
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
        /*************************/
        //以下焊接线程(非机器人直连)
        if(b_weldsendent==true)
        {
            weldsend_Thread->Stop();
            weldsend_Thread->quit();
            weldsend_Thread->wait();
        #ifdef OPEN_SHOW_WELDSOCKDATA
            weldsendrcv_Thread->Stop();
            weldsendrcv_Thread->quit();
            weldsendrcv_Thread->wait();
        #endif
            m_weldsendent.Close();
        #ifdef OPEN_SHOW_WELDSOCKDATA
            if(weldsendrcv_buf!=NULL)
            {
                delete []weldsendrcv_buf;
                weldsendrcv_buf=NULL;
            }
        #endif
            b_weldsendent=false;
        }
        /************************/

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
        delete weldsend_Thread;
        delete weldsendrcv_Thread;
        delete linkthread;
        delete thread1;

    }
}

std::array<double, 3> Robotcontrol::Yaskawa_RotMatrixXYZ2Euler(Eigen::Matrix3d rot_matrix)
{
    double r11,r21,r31,r32,r33;
    r11 = rot_matrix(0,0);
    r21 = rot_matrix(1,0);
    r31 = rot_matrix(2,0);
    r32 = rot_matrix(2,1);
    r33 = rot_matrix(2,2);
    double rx = atan2(r32,r33);
    double ry = atan2(-r31,sqrt(r11*r11 + r21*r21));
    double rz = atan2(r21,r11);

    std::array<double,3> temp;
    temp[0]= rx;
    temp[1]= ry;
    temp[2]= rz;
    return temp;
}

Eigen::Matrix3d Robotcontrol::Yaskawa_Euler2RotMatrixXYZ(std::array<double,3> pst)
{
    double rx = pst[0],ry = pst[1],rz = pst[2];
    double c1 = cos(rx*CAL_RADIAN);
    double c2 = cos(ry*CAL_RADIAN);
    double c3 = cos(rz*CAL_RADIAN);
    double s1 = sin(rx*CAL_RADIAN);
    double s2 = sin(ry*CAL_RADIAN);
    double s3 = sin(rz*CAL_RADIAN);
    Eigen::Matrix3d rot;
    rot(0,0) = c2*c3;
    rot(0,1) = s1*s2*c3 - c1*s3;
    rot(0,2) = c1*s2*c3 + s1*s3;
    rot(1,0) = c2*s3;
    rot(1,1) = s1*s2*s3 + c1*c3;
    rot(1,2) = c1*s2*s3 - s1*c3;
    rot(2,0) = -s2;
    rot(2,1) = s1*c2;
    rot(2,2) = c1*c2;
    return rot;
}

void Robotcontrol::WeldInit()//焊机初始化(非机器人直连时有效)
{
    switch(weld_mod)
    {
        case WELD_MODEL_NULL:                      //无焊机
        {
        //不做处理,无效
        }
        break;
        case WELD_MODEL_ROBOT_LINK:                //机器人直连
        {
        //不做处理，无效
        }
        break;
    }
}

void Robotcontrol::clear_movepoint_buffer()
{
    mutexmovepoint_buffer_group.lock();
    movepoint_buffer.clear();
    mutexmovepoint_buffer_group.unlock();
}

void Robotcontrol::RobotInit(int tcp)//机器人初始化
{
    mutexmovepoint_buffer_group.lock();
    movepoint_buffer.clear();
    mutexmovepoint_buffer_group.unlock();
    switch(rob_mod)
    {
        case ROBOT_MODEL_NULL://无机器人
        {
        //不做处理
        }
        break;
        case ROBOT_MODEL_EMERGEN://智昌机器人
        {
            if(b_sendent==true)
            {
                mutexsend_buf_group.lock();
                QString msg="Tool,"+QString::number(tcp);
                std::string str=msg.toStdString();
                send_buf_group.push_back(str);
                mutexsend_buf_group.unlock();
            }
        }
        break;
        case ROBOT_MODEL_DOBOT://越彊机器人
        {
            if(b_totalcontrol_Thread==true)
            {
                mutextotalcontrol_buf_group.lock();
                QString msg="CP(100)";    //机器人上电使能
                std::string str=msg.toStdString();
                totalcontrol_buf_group.push_back(str);
                msg="EnableRobot()";    //机器人上电使能
                str=msg.toStdString();
                totalcontrol_buf_group.push_back(str);
                mutextotalcontrol_buf_group.unlock();
            }
            if(b_sendent==true)
            {
                mutexsend_buf_group.lock();
                QString msg="Tool("+QString::number(tcp)+")";
                std::string str=msg.toStdString();
                send_buf_group.push_back(str);
                mutexsend_buf_group.unlock();
            }
        }
        break;
        case ROBOT_MODEL_UR://优傲机器人
        {

        }
        break;
        case ROBOT_MODEL_KUKA://库卡机器人
        {
            if(b_sendent==true)
            {
                mutexsend_buf_group.lock();
                QDomDocument doc;
                QDomText s_data;
                QDomElement root_elem = doc.createElement("ROBOTCONTROL");
                doc.appendChild(root_elem);
                QDomElement obj;
                obj=doc.createElement("TCP");
                s_data=doc.createTextNode(QString::number(tcp));
                obj.appendChild(s_data);
                root_elem.appendChild(obj);
                QString msg=doc.toString();
                std::string str=msg.toStdString();
                send_buf_group.push_back(str);
                mutexsend_buf_group.unlock();
            }
        }
        break;
        case ROBOT_MODEL_KAWASAKI://川崎机器人
        {
            sleep(1);
            if(b_client==true)
            { 
                QString msg;
                std::string str;
                msg="as\r\n";    //进入as指令
                str=msg.toStdString();
                m_client.Send(str.c_str(),str.size());
                usleep(100000);
                msg="where 2\r\n";    //进入as指令
                str=msg.toStdString();
                m_client.Send(str.c_str(),str.size());
            }
            if(b_sendent==true)
            {
                sleep(1);
                mutexsend_buf_group.lock();
                QString msg="as\n";    //进入as指令
                std::string str=msg.toStdString();
                send_buf_group.push_back(str);
                mutexsend_buf_group.unlock();
            }
            sleep(1);
        }
        break;
        case ROBOT_MODEL_YASKAWA://安川机器人
        {

        }
        break;
    }
}

void Robotcontrol::RobotOPEN_ELE(bool b_wait)
{
    mutexmovepoint_buffer_group.lock();
    movepoint_buffer.clear();
    mutexmovepoint_buffer_group.unlock();
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
                QString msg="EnableRobot()";    //机器人上电使能
                std::string str=msg.toStdString();
                totalcontrol_buf_group.push_back(str);
                mutextotalcontrol_buf_group.unlock();
                if(b_wait==true)
                {
                    sleep(3);
                }
            }
        }
        break;
        case ROBOT_MODEL_UR://优傲机器人
        {
            if(b_totalcontrol_Thread==true)
            {
                mutextotalcontrol_buf_group.lock();
                QString msg="power on\r\n";    //机器人上电使能
                std::string str=msg.toStdString();
                totalcontrol_buf_group.push_back(str);
                mutextotalcontrol_buf_group.unlock();
                if(b_wait==true)
                {
                    sleep(3);
                }
            }
        }
        break;
        case ROBOT_MODEL_KUKA://库卡机器人
        {

        }
        break;
        case ROBOT_MODEL_KAWASAKI://川崎机器人
        {

        }
        break;
        case ROBOT_MODEL_YASKAWA://安川机器人
        {

        }
        break;
    }
}

void Robotcontrol::RobotDisOPEN_ELE()
{
    mutexmovepoint_buffer_group.lock();
    movepoint_buffer.clear();
    mutexmovepoint_buffer_group.unlock();
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
                QString msg="DisableRobot()";    //机器人下电使能
                std::string str=msg.toStdString();
                totalcontrol_buf_group.push_back(str);
                mutextotalcontrol_buf_group.unlock();
            }
        }
        break;
        case ROBOT_MODEL_UR://优傲机器人
        {
            if(b_totalcontrol_Thread==true)
            {
                mutextotalcontrol_buf_group.lock();
                QString msg="power off\r\n";    //机器人下电使能
                std::string str=msg.toStdString();
                totalcontrol_buf_group.push_back(str);
                mutextotalcontrol_buf_group.unlock();
            }
        }
        break;
        case ROBOT_MODEL_KUKA://库卡机器人
        {

        }
        break;
        case ROBOT_MODEL_KAWASAKI://川崎机器人
        {

        }
        break;
        case ROBOT_MODEL_YASKAWA://安川机器人
        {

        }
        break;
    }
}

void RobotcontrolThread1::RobotMove(float f_movX,float f_movY,float f_movZ,float f_movRX, float f_movRY,float f_movRZ,
                                    float f_movX1,float f_movY1,float f_movZ1,float f_movRX1, float f_movRY1,float f_movRZ1,
                                    int i_out1,int i_out2,int i_out3,int i_out1_1,int i_out1_2,int i_out1_3,
                                    Robmovemodel_ID movemod,uint16_t tcp,float f_speed)
{
    switch(_p->rob_mod)
    {
        case ROBOT_MODEL_NULL://无机器人
        {

        }
        break;
        case ROBOT_MODEL_EMERGEN://智昌机器人
        {
            mutexmovepoint_buffer_group.lock();
            Pause_PointInfo moveinfo;
            moveinfo.robpos.X=f_movX;
            moveinfo.robpos.Y=f_movY;
            moveinfo.robpos.Z=f_movZ;
            moveinfo.robpos.RX=f_movRX;
            moveinfo.robpos.RY=f_movRY;
            moveinfo.robpos.RZ=f_movRZ;
            moveinfo.robpos.out_1=i_out1;
            moveinfo.robpos.out_2=i_out2;
            moveinfo.robpos.out_3=i_out3;
            switch(movemod)
            {
                case MOVEL:
                case MOVEJ:
                case MOVEP:
                break;
                case MOVEC:
                {
                    moveinfo.robpos.X1=f_movX1;
                    moveinfo.robpos.Y1=f_movY1;
                    moveinfo.robpos.Z1=f_movZ1;
                    moveinfo.robpos.RX1=f_movRX1;
                    moveinfo.robpos.RY1=f_movRY1;
                    moveinfo.robpos.RZ1=f_movRZ1;
                    moveinfo.robpos.out1_1=i_out1;
                    moveinfo.robpos.out1_2=i_out2;
                    moveinfo.robpos.out1_3=i_out3;
                }
                break;
            }
            moveinfo.f_speed=f_speed;
            moveinfo.movemod=movemod;
            moveinfo.tcp=tcp;
            _p->movepoint_buffer.push_back(moveinfo);
            mutexmovepoint_buffer_group.unlock();

            /************/
            switch(movemod)
            {
                case MOVEL:
                case MOVEP:
                {
                    //转换成四元数
                    std::array<double,3> pst;
                    pst[0]=f_movRX;
                    pst[1]=f_movRY;
                    pst[2]=f_movRZ;
                    Eigen::Matrix3d posture=_p->Yaskawa_Euler2RotMatrixXYZ(pst);
                    Eigen::Quaterniond quater_posture;
                    quater_posture=posture;

                    mutexsend_buf_group.lock();
                    QString msg="MoveL,"+QString::number(f_movX,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                         QString::number(f_movY,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                         QString::number(f_movZ,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                         QString::number(quater_posture.w(),'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                         QString::number(quater_posture.x(),'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                         QString::number(quater_posture.y(),'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                         QString::number(quater_posture.z(),'f',ROBOT_POSTURE_DECIMAL_PLACE)+",User,0,Tool,"+
                                         QString::number(tcp)+",SpeedL,"+
                                         QString::number((int)f_speed);
                    std::string str=msg.toStdString();
                //  std::string str="MoveL,1.3,23,45,6,7,8,User,0,Tool,2,SpeedL,1";
                    _p->send_buf_group.push_back(str);
                    mutexsend_buf_group.unlock();
                }
                break;
                case MOVEJ:
                {
                    //转换成四元数
                    std::array<double,3> pst;
                    pst[0]=f_movRX;
                    pst[1]=f_movRY;
                    pst[2]=f_movRZ;
                    Eigen::Matrix3d posture=_p->Yaskawa_Euler2RotMatrixXYZ(pst);
                    Eigen::Quaterniond quater_posture;
                    quater_posture=posture;

                    mutexsend_buf_group.lock();
                    QString msg="MoveJ,"+QString::number(f_movX,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                         QString::number(f_movY,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                         QString::number(f_movZ,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                         QString::number(quater_posture.w(),'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                         QString::number(quater_posture.x(),'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                         QString::number(quater_posture.y(),'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                         QString::number(quater_posture.z(),'f',ROBOT_POSTURE_DECIMAL_PLACE)+",User,0,Tool,"+
                                         QString::number(tcp)+",SpeedJ,"+
                                         QString::number((int)f_speed);
                    std::string str=msg.toStdString();
                //  std::string str="MoveJ,1.3,23,45,6,7,8,User,0,Tool,2,SpeedJ,3";
                    _p->send_buf_group.push_back(str);
                    mutexsend_buf_group.unlock();
                }
                break;
                case MOVEC:
                {
                    //转换成四元数
                    std::array<double,3> pst,pst1;
                    pst[0]=f_movRX;
                    pst[1]=f_movRY;
                    pst[2]=f_movRZ;
                    pst1[0]=f_movRX1;
                    pst1[1]=f_movRY1;
                    pst1[2]=f_movRZ1;
                    Eigen::Matrix3d posture=_p->Yaskawa_Euler2RotMatrixXYZ(pst);
                    Eigen::Matrix3d posture1=_p->Yaskawa_Euler2RotMatrixXYZ(pst1);
                    Eigen::Quaterniond quater_posture,quater_posture1;
                    quater_posture=posture;
                    quater_posture1=posture1;
                    mutexsend_buf_group.lock();
                    QString msg="MoveC,"+QString::number(f_movX,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                         QString::number(f_movY,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                         QString::number(f_movZ,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                         QString::number(quater_posture.w(),'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                         QString::number(quater_posture.x(),'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                         QString::number(quater_posture.y(),'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                         QString::number(quater_posture.z(),'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                         QString::number(f_movX1,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                         QString::number(f_movY1,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                         QString::number(f_movZ1,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                         QString::number(quater_posture1.w(),'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                         QString::number(quater_posture1.x(),'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                         QString::number(quater_posture1.y(),'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                         QString::number(quater_posture1.z(),'f',ROBOT_POSTURE_DECIMAL_PLACE)+",User,0,Tool,"+
                                         QString::number(tcp)+",SpeedL,"+
                                         QString::number((int)f_speed);
                    std::string str=msg.toStdString();
                //  std::string str="MoveC,1.3,23,45,6,7,8,1.3,23,45,6,7,8,User,0,Tool,2,SpeedL,1.2";
                    _p->send_buf_group.push_back(str);
                    mutexsend_buf_group.unlock();
                }
                break;
            }
        }
        break;
        case ROBOT_MODEL_DOBOT://越彊机器人
        {
            mutexmovepoint_buffer_group.lock();
            Pause_PointInfo moveinfo;
            moveinfo.robpos.X=f_movX;
            moveinfo.robpos.Y=f_movY;
            moveinfo.robpos.Z=f_movZ;
            moveinfo.robpos.RX=f_movRX;
            moveinfo.robpos.RY=f_movRY;
            moveinfo.robpos.RZ=f_movRZ;
            moveinfo.robpos.out_1=i_out1;
            moveinfo.robpos.out_2=i_out2;
            moveinfo.robpos.out_3=i_out3;
            switch(movemod)
            {
                case MOVEL:
                case MOVEJ:
                case MOVEP:
                break;
                case MOVEC:
                {
                    moveinfo.robpos.X1=f_movX1;
                    moveinfo.robpos.Y1=f_movY1;
                    moveinfo.robpos.Z1=f_movZ1;
                    moveinfo.robpos.RX1=f_movRX1;
                    moveinfo.robpos.RY1=f_movRY1;
                    moveinfo.robpos.RZ1=f_movRZ1;
                    moveinfo.robpos.out1_1=i_out1;
                    moveinfo.robpos.out1_2=i_out2;
                    moveinfo.robpos.out1_3=i_out3;
                }
                break;
            }
            moveinfo.f_speed=f_speed;
            moveinfo.movemod=movemod;
            moveinfo.tcp=tcp;
            _p->movepoint_buffer.push_back(moveinfo);
            mutexmovepoint_buffer_group.unlock();

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
                    /*
                    QString msg="ServoP("+QString::number(f_movX,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                        QString::number(f_movY,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                        QString::number(f_movZ,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                        QString::number(f_movRX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                        QString::number(f_movRY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                        QString::number(f_movRZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+")";
                    */
                    std::string str=msg.toStdString();
                //  std::string str="MovL(1.3,23,45,6,7,8,User=0,Tool=2,SpeedL=1.2,CP=50)";
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
                                        QString::number(tcp)+",SpeedJ="+
                                        QString::number((int)f_speed)+")";
                    std::string str=msg.toStdString();
                //  std::string str="MovJ(1.3,23,45,6,7,8,User=0,Tool=2,CP=50)";
                    _p->send_buf_group.push_back(str);
                    mutexsend_buf_group.unlock();
                }
                break;
                case MOVEC:
                {
                    mutexsend_buf_group.lock();
                    QString msg="Arc("+QString::number(f_movX,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                        QString::number(f_movY,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                        QString::number(f_movZ,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                        QString::number(f_movRX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                        QString::number(f_movRY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                        QString::number(f_movRZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                        QString::number(f_movX1,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                        QString::number(f_movY1,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                        QString::number(f_movZ1,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                        QString::number(f_movRX1,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                        QString::number(f_movRY1,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                        QString::number(f_movRZ1,'f',ROBOT_POSTURE_DECIMAL_PLACE)+",User=0,Tool="+
                                        QString::number(tcp)+",SpeedL="+
                                        QString::number((int)f_speed)+")";
                    std::string str=msg.toStdString();
                //  std::string str="Arc(1.3,23,45,6,7,8,1.3,23,45,6,7,8,User=0,Tool=2,SpeedL=1.2)";
                    _p->send_buf_group.push_back(str);
                    mutexsend_buf_group.unlock();
                }
                break;
                case MOVEP:
                {
                    mutexsend_buf_group.lock();
                    QString msg="ServoP("+QString::number(f_movX,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                        QString::number(f_movY,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                        QString::number(f_movZ,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                        QString::number(f_movRX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                        QString::number(f_movRY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                        QString::number(f_movRZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+")";
                    std::string str=msg.toStdString();
                //  std::string str="MovL(1.3,23,45,6,7,8,User=0,Tool=2,SpeedL=1.2,CP=50)";
                    _p->send_buf_group.push_back(str);
                    mutexsend_buf_group.unlock();
                }
                break;
            }
        }
        break;
        case ROBOT_MODEL_UR://优傲机器人
        {
            mutexmovepoint_buffer_group.lock();
            Pause_PointInfo moveinfo;
            moveinfo.robpos.X=f_movX;
            moveinfo.robpos.Y=f_movY;
            moveinfo.robpos.Z=f_movZ;
            moveinfo.robpos.RX=f_movRX;
            moveinfo.robpos.RY=f_movRY;
            moveinfo.robpos.RZ=f_movRZ;
            moveinfo.robpos.out_1=i_out1;
            moveinfo.robpos.out_2=i_out2;
            moveinfo.robpos.out_3=i_out3;
            switch(movemod)
            {
                case MOVEL:
                case MOVEP:
                case MOVEJ:
                break;
                case MOVEC:
                {
                    moveinfo.robpos.X1=f_movX1;
                    moveinfo.robpos.Y1=f_movY1;
                    moveinfo.robpos.Z1=f_movZ1;
                    moveinfo.robpos.RX1=f_movRX1;
                    moveinfo.robpos.RY1=f_movRY1;
                    moveinfo.robpos.RZ1=f_movRZ1;
                    moveinfo.robpos.out1_1=i_out1;
                    moveinfo.robpos.out1_2=i_out2;
                    moveinfo.robpos.out1_3=i_out3;
                }
                break;
            }
            moveinfo.f_speed=f_speed;
            moveinfo.movemod=movemod;
            moveinfo.tcp=tcp;
            _p->movepoint_buffer.push_back(moveinfo);
            mutexmovepoint_buffer_group.unlock();

            switch(movemod)
            {
                case MOVEL:
                case MOVEP:
                {
                    //转换成旋转矩阵
                    std::array<double,3> pst;
                    pst[0]=f_movRX;
                    pst[1]=f_movRY;
                    pst[2]=f_movRZ;
                    Eigen::Matrix3d posture=_p->Yaskawa_Euler2RotMatrixXYZ(pst);
                    //旋转矩阵转旋转向量
                    Eigen::AngleAxisd rotation_vector;
                    rotation_vector.fromRotationMatrix(posture);
                    double noml=rotation_vector.angle();

                    mutexsend_buf_group.lock();
                    QString msg="movel(p["+QString::number(f_movX/1000.0,'f',ROBOT_POSE_DECIMAL_PLACE+3)+","+
                                        QString::number(f_movY/1000.0,'f',ROBOT_POSE_DECIMAL_PLACE+3)+","+
                                        QString::number(f_movZ/1000.0,'f',ROBOT_POSE_DECIMAL_PLACE+3)+","+
                                        QString::number(rotation_vector.axis().transpose().x()*noml,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                        QString::number(rotation_vector.axis().transpose().y()*noml,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                        QString::number(rotation_vector.axis().transpose().z()*noml,'f',ROBOT_POSTURE_DECIMAL_PLACE)+"],v="+
                                        QString::number(f_speed/1000.0)+")\r\n";
                    std::string str=msg.toStdString();
                    _p->send_buf_group.push_back(str);
                    mutexsend_buf_group.unlock();
                }
                break;
                case MOVEJ:
                {
                    //转换成旋转矩阵
                    std::array<double,3> pst;
                    pst[0]=f_movRX;
                    pst[1]=f_movRY;
                    pst[2]=f_movRZ;
                    Eigen::Matrix3d posture=_p->Yaskawa_Euler2RotMatrixXYZ(pst);
                    //旋转矩阵转旋转向量
                    Eigen::AngleAxisd rotation_vector;
                    rotation_vector.fromRotationMatrix(posture);
                    double noml=rotation_vector.angle();

                    mutexsend_buf_group.lock();
                    QString msg="movej(p["+QString::number(f_movX/1000.0,'f',ROBOT_POSE_DECIMAL_PLACE+3)+","+
                                        QString::number(f_movY/1000.0,'f',ROBOT_POSE_DECIMAL_PLACE+3)+","+
                                        QString::number(f_movZ/1000.0,'f',ROBOT_POSE_DECIMAL_PLACE+3)+","+
                                        QString::number(rotation_vector.axis().transpose().x()*noml,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                        QString::number(rotation_vector.axis().transpose().y()*noml,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                        QString::number(rotation_vector.axis().transpose().z()*noml,'f',ROBOT_POSTURE_DECIMAL_PLACE)+"],v="+
                                        QString::number(f_speed/1000.0)+")\r\n";
                    std::string str=msg.toStdString();
                    _p->send_buf_group.push_back(str);
                    mutexsend_buf_group.unlock();
                }
                break;
                case MOVEC:
                {
                    std::array<double,3> pst;
                    pst[0]=f_movRX;
                    pst[1]=f_movRY;
                    pst[2]=f_movRZ;
                    Eigen::Matrix3d posture=_p->Yaskawa_Euler2RotMatrixXYZ(pst);
                    //旋转矩阵转旋转向量
                    Eigen::AngleAxisd rotation_vector;
                    rotation_vector.fromRotationMatrix(posture);
                    double noml=rotation_vector.angle();

                    std::array<double,3> pst1;
                    pst1[0]=f_movRX1;
                    pst1[1]=f_movRY1;
                    pst1[2]=f_movRZ1;
                    Eigen::Matrix3d posture1=_p->Yaskawa_Euler2RotMatrixXYZ(pst1);
                    //旋转矩阵转旋转向量
                    Eigen::AngleAxisd rotation_vector1;
                    rotation_vector1.fromRotationMatrix(posture1);
                    double noml1=rotation_vector1.angle();

                    mutexsend_buf_group.lock();
                    QString msg="movec(p["+QString::number(f_movX/1000.0,'f',ROBOT_POSE_DECIMAL_PLACE+3)+","+
                                        QString::number(f_movY/1000.0,'f',ROBOT_POSE_DECIMAL_PLACE+3)+","+
                                        QString::number(f_movZ/1000.0,'f',ROBOT_POSE_DECIMAL_PLACE+3)+","+
                                        QString::number(rotation_vector.axis().transpose().x()*noml,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                        QString::number(rotation_vector.axis().transpose().y()*noml,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                        QString::number(rotation_vector.axis().transpose().z()*noml,'f',ROBOT_POSTURE_DECIMAL_PLACE)+"],p["+
                                        QString::number(f_movX1/1000.0,'f',ROBOT_POSE_DECIMAL_PLACE+3)+","+
                                        QString::number(f_movY1/1000.0,'f',ROBOT_POSE_DECIMAL_PLACE+3)+","+
                                        QString::number(f_movZ1/1000.0,'f',ROBOT_POSE_DECIMAL_PLACE+3)+","+
                                        QString::number(rotation_vector1.axis().transpose().x()*noml1,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                        QString::number(rotation_vector1.axis().transpose().y()*noml1,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                        QString::number(rotation_vector1.axis().transpose().z()*noml1,'f',ROBOT_POSTURE_DECIMAL_PLACE)+"],v="+
                                        QString::number(f_speed/1000.0)+")\r\n";
                    std::string str=msg.toStdString();
                    _p->send_buf_group.push_back(str);
                    mutexsend_buf_group.unlock();
                }
                break;
            }
        }
        break;
        case ROBOT_MODEL_KUKA://库卡机器人
        {
            mutexmovepoint_buffer_group.lock();
            Pause_PointInfo moveinfo;
            moveinfo.robpos.X=f_movX;
            moveinfo.robpos.Y=f_movY;
            moveinfo.robpos.Z=f_movZ;
            moveinfo.robpos.RX=f_movRX;
            moveinfo.robpos.RY=f_movRY;
            moveinfo.robpos.RZ=f_movRZ;
            moveinfo.robpos.out_1=i_out1;
            moveinfo.robpos.out_2=i_out2;
            moveinfo.robpos.out_3=i_out3;
            switch(movemod)
            {
                case MOVEL:
                case MOVEP:
                case MOVEJ:
                break;
                case MOVEC:
                {
                    moveinfo.robpos.X1=f_movX1;
                    moveinfo.robpos.Y1=f_movY1;
                    moveinfo.robpos.Z1=f_movZ1;
                    moveinfo.robpos.RX1=f_movRX1;
                    moveinfo.robpos.RY1=f_movRY1;
                    moveinfo.robpos.RZ1=f_movRZ1;
                    moveinfo.robpos.out1_1=i_out1;
                    moveinfo.robpos.out1_2=i_out2;
                    moveinfo.robpos.out1_3=i_out3;
                }
                break;
            }
            moveinfo.f_speed=f_speed;
            moveinfo.movemod=movemod;
            moveinfo.tcp=tcp;
            _p->movepoint_buffer.push_back(moveinfo);
            mutexmovepoint_buffer_group.unlock();
            switch(movemod)
            {
                case MOVEL:
                case MOVEP:
                case MOVEJ:
                {
                    mutexsend_buf_group.lock();
                    QDomDocument doc;
                    QDomText s_data;
                    QDomElement root_elem = doc.createElement("ROBOTCONTROL");
                    doc.appendChild(root_elem);
                    QDomElement obj;
                    obj=doc.createElement("TCP");
                    s_data=doc.createTextNode(QString::number(tcp));
                    obj.appendChild(s_data);
                    root_elem.appendChild(obj);
                    obj=doc.createElement("MOVEMODE");
                    s_data=doc.createTextNode(QString::number(movemod));
                    obj.appendChild(s_data);
                    root_elem.appendChild(obj);
                    obj=doc.createElement("MOVESPEED");
                    s_data=doc.createTextNode(QString::number(f_speed,'f',ROBOT_POSE_DECIMAL_PLACE));
                    obj.appendChild(s_data);
                    root_elem.appendChild(obj);
                    obj=doc.createElement("MOVEPOSE");
                    QDomElement objmin;
                    objmin=doc.createElement("X");
                    s_data=doc.createTextNode(QString::number(f_movX,'f',ROBOT_POSE_DECIMAL_PLACE));
                    objmin.appendChild(s_data);
                    obj.appendChild(objmin);
                    objmin=doc.createElement("Y");
                    s_data=doc.createTextNode(QString::number(f_movY,'f',ROBOT_POSE_DECIMAL_PLACE));
                    objmin.appendChild(s_data);
                    obj.appendChild(objmin);
                    objmin=doc.createElement("Z");
                    s_data=doc.createTextNode(QString::number(f_movZ,'f',ROBOT_POSE_DECIMAL_PLACE));
                    objmin.appendChild(s_data);
                    obj.appendChild(objmin);
                    objmin=doc.createElement("RX");
                    s_data=doc.createTextNode(QString::number(f_movRX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
                    objmin.appendChild(s_data);
                    obj.appendChild(objmin);
                    objmin=doc.createElement("RY");
                    s_data=doc.createTextNode(QString::number(f_movRY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
                    objmin.appendChild(s_data);
                    obj.appendChild(objmin);
                    objmin=doc.createElement("RZ");
                    s_data=doc.createTextNode(QString::number(f_movRZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
                    objmin.appendChild(s_data);
                    obj.appendChild(objmin);
                    objmin=doc.createElement("OUT1");
                    s_data=doc.createTextNode(QString::number(i_out1));
                    objmin.appendChild(s_data);
                    obj.appendChild(objmin);
                    objmin=doc.createElement("OUT2");
                    s_data=doc.createTextNode(QString::number(i_out2));
                    objmin.appendChild(s_data);
                    obj.appendChild(objmin);
                    objmin=doc.createElement("OUT3");
                    s_data=doc.createTextNode(QString::number(i_out3));
                    objmin.appendChild(s_data);
                    obj.appendChild(objmin);
                    root_elem.appendChild(obj);
                    QString msg=doc.toString();
                    std::string str=msg.toStdString();
                    _p->send_buf_group.push_back(str);
                    mutexsend_buf_group.unlock();
                }
                break;
                case MOVEC:
                {
                    mutexsend_buf_group.lock();
                    QDomDocument doc;
                    QDomText s_data;
                    QDomElement root_elem = doc.createElement("ROBOTCONTROL");
                    doc.appendChild(root_elem);
                    QDomElement obj;
                    obj=doc.createElement("TCP");
                    s_data=doc.createTextNode(QString::number(tcp));
                    obj.appendChild(s_data);
                    root_elem.appendChild(obj);
                    obj=doc.createElement("MOVEMODE");
                    s_data=doc.createTextNode(QString::number(movemod));
                    obj.appendChild(s_data);
                    root_elem.appendChild(obj);
                    obj=doc.createElement("MOVESPEED");
                    s_data=doc.createTextNode(QString::number(f_speed,'f',ROBOT_POSE_DECIMAL_PLACE));
                    obj.appendChild(s_data);
                    root_elem.appendChild(obj);
                    obj=doc.createElement("MOVEPOSE");
                    QDomElement objmin;
                    objmin=doc.createElement("X");
                    s_data=doc.createTextNode(QString::number(f_movX,'f',ROBOT_POSE_DECIMAL_PLACE));
                    objmin.appendChild(s_data);
                    obj.appendChild(objmin);
                    objmin=doc.createElement("Y");
                    s_data=doc.createTextNode(QString::number(f_movY,'f',ROBOT_POSE_DECIMAL_PLACE));
                    objmin.appendChild(s_data);
                    obj.appendChild(objmin);
                    objmin=doc.createElement("Z");
                    s_data=doc.createTextNode(QString::number(f_movZ,'f',ROBOT_POSE_DECIMAL_PLACE));
                    objmin.appendChild(s_data);
                    obj.appendChild(objmin);
                    objmin=doc.createElement("RX");
                    s_data=doc.createTextNode(QString::number(f_movRX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
                    objmin.appendChild(s_data);
                    obj.appendChild(objmin);
                    objmin=doc.createElement("RY");
                    s_data=doc.createTextNode(QString::number(f_movRY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
                    objmin.appendChild(s_data);
                    obj.appendChild(objmin);
                    objmin=doc.createElement("RZ");
                    s_data=doc.createTextNode(QString::number(f_movRZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
                    objmin.appendChild(s_data);
                    obj.appendChild(objmin);
                    objmin=doc.createElement("OUT1");
                    s_data=doc.createTextNode(QString::number(i_out1));
                    objmin.appendChild(s_data);
                    obj.appendChild(objmin);
                    objmin=doc.createElement("OUT2");
                    s_data=doc.createTextNode(QString::number(i_out2));
                    objmin.appendChild(s_data);
                    obj.appendChild(objmin);
                    objmin=doc.createElement("OUT3");
                    s_data=doc.createTextNode(QString::number(i_out3));
                    objmin.appendChild(s_data);
                    obj.appendChild(objmin);
                    objmin=doc.createElement("X1");
                    s_data=doc.createTextNode(QString::number(f_movX1,'f',ROBOT_POSE_DECIMAL_PLACE));
                    objmin.appendChild(s_data);
                    obj.appendChild(objmin);
                    objmin=doc.createElement("Y1");
                    s_data=doc.createTextNode(QString::number(f_movY1,'f',ROBOT_POSE_DECIMAL_PLACE));
                    objmin.appendChild(s_data);
                    obj.appendChild(objmin);
                    objmin=doc.createElement("Z1");
                    s_data=doc.createTextNode(QString::number(f_movZ1,'f',ROBOT_POSE_DECIMAL_PLACE));
                    objmin.appendChild(s_data);
                    obj.appendChild(objmin);
                    objmin=doc.createElement("RX1");
                    s_data=doc.createTextNode(QString::number(f_movRX1,'f',ROBOT_POSTURE_DECIMAL_PLACE));
                    objmin.appendChild(s_data);
                    obj.appendChild(objmin);
                    objmin=doc.createElement("RY1");
                    s_data=doc.createTextNode(QString::number(f_movRY1,'f',ROBOT_POSTURE_DECIMAL_PLACE));
                    objmin.appendChild(s_data);
                    obj.appendChild(objmin);
                    objmin=doc.createElement("RZ1");
                    s_data=doc.createTextNode(QString::number(f_movRZ1,'f',ROBOT_POSTURE_DECIMAL_PLACE));
                    objmin.appendChild(s_data);
                    obj.appendChild(objmin);
                    objmin=doc.createElement("OUT11");
                    s_data=doc.createTextNode(QString::number(i_out1_1));
                    objmin.appendChild(s_data);
                    obj.appendChild(objmin);
                    objmin=doc.createElement("OUT21");
                    s_data=doc.createTextNode(QString::number(i_out1_2));
                    objmin.appendChild(s_data);
                    obj.appendChild(objmin);
                    objmin=doc.createElement("OUT31");
                    s_data=doc.createTextNode(QString::number(i_out1_3));
                    objmin.appendChild(s_data);
                    obj.appendChild(objmin);
                    root_elem.appendChild(obj);
                    QString msg=doc.toString();
                    std::string str=msg.toStdString();
                    _p->send_buf_group.push_back(str);
                    mutexsend_buf_group.unlock();
                }
                break;
            }
        }
        break;
        case ROBOT_MODEL_KAWASAKI://川崎机器人
        {
            mutexmovepoint_buffer_group.lock();
            Pause_PointInfo moveinfo;
            moveinfo.robpos.X=f_movX;
            moveinfo.robpos.Y=f_movY;
            moveinfo.robpos.Z=f_movZ;
            moveinfo.robpos.RX=f_movRX;
            moveinfo.robpos.RY=f_movRY;
            moveinfo.robpos.RZ=f_movRZ;
            moveinfo.robpos.out_1=i_out1;
            moveinfo.robpos.out_2=i_out2;
            moveinfo.robpos.out_3=i_out3;
            switch(movemod)
            {
                case MOVEL:
                case MOVEP:
                case MOVEJ:
                break;
                case MOVEC:
                {
                    moveinfo.robpos.X1=f_movX1;
                    moveinfo.robpos.Y1=f_movY1;
                    moveinfo.robpos.Z1=f_movZ1;
                    moveinfo.robpos.RX1=f_movRX1;
                    moveinfo.robpos.RY1=f_movRY1;
                    moveinfo.robpos.RZ1=f_movRZ1;
                    moveinfo.robpos.out1_1=i_out1;
                    moveinfo.robpos.out1_2=i_out2;
                    moveinfo.robpos.out1_3=i_out3;
                }
                break;
            }
            moveinfo.f_speed=f_speed;
            moveinfo.movemod=movemod;
            moveinfo.tcp=tcp;
            _p->movepoint_buffer.push_back(moveinfo);
            mutexmovepoint_buffer_group.unlock();

            uint16_t u16data_elec_work=_p->mb_mapping->tab_registers[ROB_MOVEFIER_REG_ADD];//加起弧判断
            float eled=*(float*)&_p->mb_mapping->tab_registers[ROB_WELD_CURRENT_FH_REG_ADD];//电流
            float elev=*(float*)&_p->mb_mapping->tab_registers[ROB_WELD_VOLTAGE_FH_REG_ADD];//电压
            float weldwidth=*(float*)&_p->mb_mapping->tab_registers[ROB_WELD_SWINGWIDTH_FH_REG_ADD];
            float weldfrequency=*(float*)&_p->mb_mapping->tab_registers[ROB_WELD_SWINGFREQUENCY_FH_REG_ADD];
            uint16_t weldmod=_p->mb_mapping->tab_registers[ROB_WELD_SWINGMOD_REG_ADD];
            int num_out=_p->mb_mapping->tab_registers[ROB_OUTNUM_REG_ADD];

            switch(u16data_elec_work)
            {
                case FIRE:
                {
                    switch(movemod)
                    {
                        case MOVEL:
                        case MOVEP:
                        {
                            mutexsend_buf_group.lock();
                            QString msg;
                            QString out_msg;
                            std::string str;
                            if(_p->b_startweld_init==true)
                            {
                                msg="w1set 1="+QString::number((int)f_speed)+","
                                              +QString::number((int)eled)+","
                                              +QString::number((int)elev)+","
                                              +QString::number((int)weldwidth)+","
                                              +QString::number((int)weldfrequency)+","
                                              +QString::number((int)weldmod)+
                                              "\r\n";
                                str=msg.toStdString();
                                _p->send_buf_group.push_back(str);
                                switch(num_out)
                                {
                                case 0:

                                break;
                                case 1:
                                    out_msg=QString::number(i_out1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                                break;
                                case 2:
                                    out_msg=QString::number(i_out1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            QString::number(i_out2/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                                break;
                                case 3:
                                    out_msg=QString::number(i_out1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            QString::number(i_out2/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            QString::number(i_out3/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                                break;
                                }

                                msg="point pos=trans("+ QString::number(f_movX,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                QString::number(f_movY,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                QString::number(f_movZ,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                QString::number(f_movRX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                QString::number(f_movRY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                QString::number(f_movRZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                out_msg+
                                                ")\r\n";
                                str=msg.toStdString();
                                _p->send_buf_group.push_back(str);
                                str="\r\n";
                                _p->send_buf_group.push_back(str);
                                msg="do lws pos\r\n";
                                _p->b_startweld_init=false;
                            }
                            else
                            {
                                switch(num_out)
                                {
                                case 0:

                                break;
                                case 1:
                                    out_msg=QString::number(i_out1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                                break;
                                case 2:
                                    out_msg=QString::number(i_out1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            QString::number(i_out2/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                                break;
                                case 3:
                                    out_msg=QString::number(i_out1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            QString::number(i_out2/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            QString::number(i_out3/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                                break;
                                }
                                msg="point pos=trans("+ QString::number(f_movX,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                QString::number(f_movY,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                QString::number(f_movZ,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                QString::number(f_movRX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                QString::number(f_movRY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                QString::number(f_movRZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                out_msg+
                                                ")\r\n";
                                str=msg.toStdString();
                                _p->send_buf_group.push_back(str);
                                str="\r\n";
                                _p->send_buf_group.push_back(str);
                                msg="do lwc pos\r\n";
                                str=msg.toStdString();
                                _p->send_buf_group.push_back(str);
                            }
                            mutexsend_buf_group.unlock();
                        }
                        break;
                        case MOVEJ:
                        {
                            mutexsend_buf_group.lock();
                            QString out_msg;
                            QString msg;
                            std::string str;
                            if(_p->b_startweld_init==true)
                            {
                                msg="w1set 1="+QString::number((int)f_speed)+","
                                              +QString::number((int)eled)+","
                                              +QString::number((int)elev)+","
                                              +QString::number((int)weldwidth)+","
                                              +QString::number((int)weldfrequency)+","
                                              +QString::number((int)weldmod)+
                                              "\r\n";
                                str=msg.toStdString();
                                _p->send_buf_group.push_back(str);
                                switch(num_out)
                                {
                                case 0:

                                break;
                                case 1:
                                    out_msg=QString::number(i_out1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                                break;
                                case 2:
                                    out_msg=QString::number(i_out1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            QString::number(i_out2/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                                break;
                                case 3:
                                    out_msg=QString::number(i_out1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            QString::number(i_out2/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            QString::number(i_out3/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                                break;
                                }

                                msg="point pos=trans("+ QString::number(f_movX,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                QString::number(f_movY,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                QString::number(f_movZ,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                QString::number(f_movRX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                QString::number(f_movRY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                QString::number(f_movRZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                out_msg+
                                                ")\r\n";
                                str=msg.toStdString();
                                _p->send_buf_group.push_back(str);
                                str="\r\n";
                                _p->send_buf_group.push_back(str);

                                msg="do jws pos\r\n";
                                _p->b_startweld_init=false;
                            }
                            else
                            {
                                switch(num_out)
                                {
                                case 0:

                                break;
                                case 1:
                                    out_msg=QString::number(i_out1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                                break;
                                case 2:
                                    out_msg=QString::number(i_out1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            QString::number(i_out2/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                                break;
                                case 3:
                                    out_msg=QString::number(i_out1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            QString::number(i_out2/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            QString::number(i_out3/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                                break;
                                }
                                msg="point pos=trans("+ QString::number(f_movX,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                QString::number(f_movY,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                QString::number(f_movZ,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                QString::number(f_movRX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                QString::number(f_movRY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                QString::number(f_movRZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                out_msg+
                                                ")\r\n";
                                str=msg.toStdString();
                                _p->send_buf_group.push_back(str);
                                str="\r\n";
                                _p->send_buf_group.push_back(str);
                                msg="do jwc pos\r\n";
                                str=msg.toStdString();
                                _p->send_buf_group.push_back(str);
                            }
                            mutexsend_buf_group.unlock();
                        }
                        break;
                        case MOVEC:
                        {
                            mutexsend_buf_group.lock();
                            QString msg;
                            QString out_msg;
                            std::string str;
                            if(_p->b_startweld_init==true)
                            {
                                msg="w1set 1="+QString::number((int)f_speed)+","
                                              +QString::number((int)eled)+","
                                              +QString::number((int)elev)+","
                                              +QString::number((int)weldwidth)+","
                                              +QString::number((int)weldfrequency)+","
                                              +QString::number((int)weldmod)+
                                              "\r\n";
                                str=msg.toStdString();
                                _p->send_buf_group.push_back(str);
                                switch(num_out)
                                {
                                case 0:

                                break;
                                case 1:
                                    out_msg=QString::number(i_out1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                                break;
                                case 2:
                                    out_msg=QString::number(i_out1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            QString::number(i_out2/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                                break;
                                case 3:
                                    out_msg=QString::number(i_out1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            QString::number(i_out2/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            QString::number(i_out3/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                                break;
                                }
                                msg="point pos=trans("+ QString::number(f_movX,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                QString::number(f_movY,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                QString::number(f_movZ,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                QString::number(f_movRX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                QString::number(f_movRY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                QString::number(f_movRZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                out_msg+
                                                ")\r\n";
                                str=msg.toStdString();
                                _p->send_buf_group.push_back(str);
                                str="\r\n";
                                _p->send_buf_group.push_back(str);
                                msg="do jws pos\r\n";
                                _p->b_startweld_init=false;
                            }
                            else
                            {
                                switch(num_out)
                                {
                                case 0:

                                break;
                                case 1:
                                    out_msg=QString::number(i_out1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                                break;
                                case 2:
                                    out_msg=QString::number(i_out1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            QString::number(i_out2/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                                break;
                                case 3:
                                    out_msg=QString::number(i_out1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            QString::number(i_out2/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            QString::number(i_out3/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                                break;
                                }
                                msg="point pos1=trans("+ QString::number(f_movX,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                QString::number(f_movY,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                QString::number(f_movZ,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                QString::number(f_movRX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                QString::number(f_movRY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                QString::number(f_movRZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                out_msg+
                                                ")\r\n";
                                str=msg.toStdString();
                                _p->send_buf_group.push_back(str);
                                str="\r\n";
                                _p->send_buf_group.push_back(str);
                                switch(num_out)
                                {
                                case 0:

                                break;
                                case 1:
                                    out_msg=QString::number(i_out1_1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                                break;
                                case 2:
                                    out_msg=QString::number(i_out1_1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            QString::number(i_out1_2/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                                break;
                                case 3:
                                    out_msg=QString::number(i_out1_1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            QString::number(i_out1_2/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            QString::number(i_out1_3/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                                break;
                                }
                                msg="point pos2=trans("+ QString::number(f_movX1,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                QString::number(f_movY1,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                QString::number(f_movZ1,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                QString::number(f_movRX1,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                QString::number(f_movRY1,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                QString::number(f_movRZ1,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                out_msg+
                                                ")\r\n";
                                str=msg.toStdString();
                                _p->send_buf_group.push_back(str);
                                str="\r\n";
                                _p->send_buf_group.push_back(str);

                                msg="do c1wc pos1\r\n";
                                str=msg.toStdString();
                                _p->send_buf_group.push_back(str);
                                msg="do c2wc pos2\r\n";
                                str=msg.toStdString();
                                _p->send_buf_group.push_back(str);
                            }
                            mutexsend_buf_group.unlock();
                        }
                        break;
                    }
                }
                break;
                default:
                {
                    switch(movemod)
                    {
                        case MOVEL:
                        case MOVEP:
                        {
                            mutexsend_buf_group.lock();
                            QString msg;
                            QString out_msg;
                            std::string str;
                            msg="speed "+QString::number((int)f_speed)+"\r\n";
                            str=msg.toStdString();
                            _p->send_buf_group.push_back(str);
                            switch(num_out)
                            {
                            case 0:

                            break;
                            case 1:
                                out_msg=QString::number(i_out1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                            break;
                            case 2:
                                out_msg=QString::number(i_out1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                        QString::number(i_out2/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                            break;
                            case 3:
                                out_msg=QString::number(i_out1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                        QString::number(i_out2/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                        QString::number(i_out3/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                            break;
                            }
                            msg="point pos=trans("+ QString::number(f_movX,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                            QString::number(f_movY,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                            QString::number(f_movZ,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                            QString::number(f_movRX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            QString::number(f_movRY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            QString::number(f_movRZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            out_msg+
                                            ")\r\n";
                            str=msg.toStdString();
                            _p->send_buf_group.push_back(str);
                            str="\r\n";
                            _p->send_buf_group.push_back(str);
                            msg="do lmove pos\r\n";
                            str=msg.toStdString();
                            _p->send_buf_group.push_back(str);
                            mutexsend_buf_group.unlock();
                        }
                        break;
                        case MOVEJ:
                        {
                            mutexsend_buf_group.lock();
                            QString msg;
                            QString out_msg;
                            std::string str;
                            msg="speed "+QString::number((int)f_speed)+"\r\n";
                            str=msg.toStdString();
                            _p->send_buf_group.push_back(str);
                            switch(num_out)
                            {
                            case 0:

                            break;
                            case 1:
                                out_msg=QString::number(i_out1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                            break;
                            case 2:
                                out_msg=QString::number(i_out1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                        QString::number(i_out2/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                            break;
                            case 3:
                                out_msg=QString::number(i_out1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                        QString::number(i_out2/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                        QString::number(i_out3/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                            break;
                            }
                            msg="point pos=trans("+ QString::number(f_movX,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                            QString::number(f_movY,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                            QString::number(f_movZ,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                            QString::number(f_movRX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            QString::number(f_movRY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            QString::number(f_movRZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            out_msg+
                                            ")\r\n";
                            str=msg.toStdString();
                            _p->send_buf_group.push_back(str);
                            str="\r\n";
                            _p->send_buf_group.push_back(str);
                            msg="do jmove pos\r\n";
                            str=msg.toStdString();
                            _p->send_buf_group.push_back(str);
                            mutexsend_buf_group.unlock();
                        }
                        break;
                        case MOVEC:
                        {
                            mutexsend_buf_group.lock();
                            QString msg;
                            QString out_msg;
                            std::string str;
                            msg="speed "+QString::number(f_speed)+"\r\n";
                            str=msg.toStdString();
                            _p->send_buf_group.push_back(str);
                            switch(num_out)
                            {
                            case 0:

                            break;
                            case 1:
                                out_msg=QString::number(i_out1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                            break;
                            case 2:
                                out_msg=QString::number(i_out1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                        QString::number(i_out2/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                            break;
                            case 3:
                                out_msg=QString::number(i_out1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                        QString::number(i_out2/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                        QString::number(i_out3/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                            break;
                            }
                            msg="point pos1=trans("+ QString::number(f_movX,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                            QString::number(f_movY,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                            QString::number(f_movZ,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                            QString::number(f_movRX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            QString::number(f_movRY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            QString::number(f_movRZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+
                                            out_msg+
                                            ")\r\n";
                            str=msg.toStdString();
                            _p->send_buf_group.push_back(str);
                            str="\r\n";
                            _p->send_buf_group.push_back(str);
                            switch(num_out)
                            {
                            case 0:

                            break;
                            case 1:
                                out_msg=QString::number(i_out1_1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                            break;
                            case 2:
                                out_msg=QString::number(i_out1_1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                        QString::number(i_out1_2/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                            break;
                            case 3:
                                out_msg=QString::number(i_out1_1/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                        QString::number(i_out1_2/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                        QString::number(i_out1_3/1000.0,'f',ROBOT_POSTURE_DECIMAL_PLACE);
                            break;
                            }
                            msg="point pos2=trans("+ QString::number(f_movX1,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                            QString::number(f_movY1,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                            QString::number(f_movZ1,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                            QString::number(f_movRX1,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            QString::number(f_movRY1,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                            QString::number(f_movRZ1,'f',ROBOT_POSTURE_DECIMAL_PLACE)+
                                            out_msg+
                                            ")\r\n";
                            str=msg.toStdString();
                            _p->send_buf_group.push_back(str);
                            str="\r\n";
                            _p->send_buf_group.push_back(str);

                            msg="do c1move pos1\r\n";
                            str=msg.toStdString();
                            _p->send_buf_group.push_back(str);
                            msg="do c2move pos2\r\n";
                            str=msg.toStdString();
                            _p->send_buf_group.push_back(str);
                            mutexsend_buf_group.unlock();
                        }
                        break;
                    }
                }
                break;
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
                    QString return_msg=QString::fromLocal8Bit("机器人控制服务器设置socket连接失败");
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
                else if (ret > 0 && (query[7] == 0x10||query[7] == 0x04))
                {       
                    _p->mb_mapping->tab_registers[ROB_MOVEMOD_REG_ADD]=65535;
                //  _p->mb_mapping->tab_registers[ROB_TCP_NUM_REG_ADD]=65535;
                    _p->mb_mapping->tab_registers[ROB_STOP_REG_ADD]=65535;
                    _p->mb_mapping->tab_registers[ROB_MOVESPEED_FH_REG_ADD]=65535;
                    _p->mb_mapping->tab_registers[ROB_MOVESPEED_FL_REG_ADD]=65535;
                    _p->mb_mapping->tab_registers[ROB_MOVEFIER_REG_ADD]=65535;
                    _p->mb_mapping->tab_registers[ROB_IO_OUTPUT1_REG_ADD]=65535;
                    _p->mb_mapping->tab_registers[ROB_A_OUTPUT1_FH_REG_ADD]=65535;
                    _p->mb_mapping->tab_registers[ROB_A_OUTPUT1_FL_REG_ADD]=65535;
                    int rc=modbus_reply(_p->ctx_robotcontrol, query, ret, _p->mb_mapping);
                    if(rc>=0)
                    {
                        if(_p->b_send_thread==true)//远端机器人发送命令已经启动
                        {
                            if(_p->mb_mapping->tab_registers[ROB_MOVEFIER_REG_ADD]!=65535)//机器人要起弧或者息弧或者送丝送气
                            {
                                uint16_t u16data_elec_work=_p->mb_mapping->tab_registers[ROB_MOVEFIER_REG_ADD];//加起弧判断
                                float eled=*(float*)&_p->mb_mapping->tab_registers[ROB_WELD_CURRENT_FH_REG_ADD];//电流
                                Alternatingcurrent elem=(Alternatingcurrent)_p->mb_mapping->tab_registers[ROB_WELD_CURRENTMOD_REG_ADD];//交变电流模式
                                float elev=*(float*)&_p->mb_mapping->tab_registers[ROB_WELD_VOLTAGE_FH_REG_ADD];//电压
                                static uint16_t old_u16data_elec_work=65535;

                                if(old_u16data_elec_work!=u16data_elec_work)
                                {
                                    switch(_p->weld_mod)
                                    {
                                        case WELD_MODEL_NULL://无焊机
                                        {

                                        }
                                        break;
                                        case WELD_MODEL_ROBOT_LINK://机器人直连
                                        {
                                            switch(_p->rob_mod)
                                            {
                                                case ROBOT_MODEL_NULL://无机器人
                                                {
                                                    switch(u16data_elec_work)
                                                    {
                                                        case STATIC:    //空闲

                                                        break;
                                                        case FIRE:         //起弧

                                                        break;
                                                        case WIND:         //送丝

                                                        break;
                                                        case REWIND:       //退丝

                                                        break;
                                                        case GASS:         //出气

                                                        break;
                                                    }
                                                }
                                                break;
                                                case ROBOT_MODEL_EMERGEN://智昌机器人
                                                {
                                                    switch(u16data_elec_work)
                                                    {
                                                        case STATIC:    //空闲

                                                        break;
                                                        case FIRE:         //起弧

                                                        break;
                                                        case WIND:         //送丝

                                                        break;
                                                        case REWIND:       //退丝

                                                        break;
                                                        case GASS:         //出气

                                                        break;
                                                    }
                                                }
                                                break;
                                                case ROBOT_MODEL_DOBOT://越彊机器人
                                                {
                                                    mutexsend_buf_group.lock();
                                                    switch(u16data_elec_work)
                                                    {
                                                        case STATIC:    //空闲
                                                        {
                                                            QString msg="DOGroup(6,0,7,0,8,0)";
                                                            std::string str=msg.toStdString();
                                                            _p->send_buf_group.push_back(str);
                                                        }
                                                        break;
                                                        case FIRE:         //起弧
                                                        {
                                                            /*******/
                                                            //计算模拟量
                                                            float a1,a2;
                                                            float Vmax=50,Vmin=10,Amin=60,Amax=500;
                                                            float a1max=9.6,a1min=1.8,a2max=9.6,a2min=1;
                                                            float f1=(Vmax-Vmin)/(a1max-a1min);
                                                            float f2=(Amax-Amin)/(a2max-a2min);
                                                            a1=(elev-Vmin)/f1+a1min;
                                                            a2=(eled-Amin)/f2+a2min;
                                                            if(a1<a1min)
                                                                a1=a1min;
                                                            if(a1>a1max)
                                                                a1=a1max;
                                                            if(a2<a2min)
                                                                a2=a2min;
                                                            if(a2>a2max)
                                                                a2=a2max;
                                                            QString msg="Sync()";
                                                            std::string str=msg.toStdString();
                                                            _p->send_buf_group.push_back(str);
                                                            msg="AO(1,"+QString::number(a1,'f',3)+")";
                                                            str=msg.toStdString();
                                                            _p->send_buf_group.push_back(str);
                                                            msg="AO(2,"+QString::number(a2,'f',3)+")";
                                                            str=msg.toStdString();
                                                            _p->send_buf_group.push_back(str);
                                                            msg="DOGroup(6,0,7,1,8,0)";
                                                            str=msg.toStdString();
                                                            _p->send_buf_group.push_back(str);
                                                        }
                                                        break;
                                                        case WIND:         //送丝
                                                        {
                                                            QString msg="DOGroup(6,1,7,0,8,0)";
                                                            std::string str=msg.toStdString();
                                                            _p->send_buf_group.push_back(str);
                                                        }
                                                        break;
                                                        case REWIND:       //退丝
                                                        {
                                                            QString msg="DOGroup(6,0,7,0,8,1)";
                                                            std::string str=msg.toStdString();
                                                            _p->send_buf_group.push_back(str);
                                                        }
                                                        break;
                                                        case GASS:         //出气

                                                        break;
                                                    }
                                                    mutexsend_buf_group.unlock();
                                                }
                                                break;
                                                case ROBOT_MODEL_UR://优傲机器人
                                                {
                                                    mutexsend_buf_group.lock();
                                                    switch(u16data_elec_work)
                                                    {
                                                        case STATIC:    //空闲

                                                        break;
                                                        case FIRE:         //起弧

                                                        break;
                                                        case WIND:         //送丝

                                                        break;
                                                        case REWIND:       //退丝

                                                        break;
                                                        case GASS:         //出气

                                                        break;
                                                    }
                                                    mutexsend_buf_group.unlock();
                                                }
                                                break;
                                                case ROBOT_MODEL_KUKA://库卡机器人
                                                {
                                                    mutexsend_buf_group.lock();
                                                    QDomDocument doc;
                                                    QDomText s_data;
                                                    QDomElement root_elem = doc.createElement("ROBOTCONTROL");
                                                    doc.appendChild(root_elem);
                                                    QDomElement obj;
                                                    obj=doc.createElement("WELD");
                                                    s_data=doc.createTextNode(QString::number(u16data_elec_work));
                                                    obj.appendChild(s_data);
                                                    root_elem.appendChild(obj);
                                                    if(u16data_elec_work==FIRE)
                                                    {
                                                        obj=doc.createElement("ELED");
                                                        s_data=doc.createTextNode(QString::number(eled,'f',3));
                                                        obj.appendChild(s_data);
                                                        root_elem.appendChild(obj);
                                                        obj=doc.createElement("ELEM");
                                                        s_data=doc.createTextNode(QString::number(elem));
                                                        obj.appendChild(s_data);
                                                        root_elem.appendChild(obj);
                                                    }
                                                    QString msg=doc.toString();
                                                    std::string str=msg.toStdString();
                                                    _p->send_buf_group.push_back(str);
                                                    mutexsend_buf_group.unlock();
                                                }
                                                break;
                                                case ROBOT_MODEL_KAWASAKI://川崎机器人
                                                {
                                                    QString msg;
                                                    mutexsend_buf_group.lock();
                                                    switch(u16data_elec_work)
                                                    {
                                                        case STATIC:    //空闲
                                                            if(_p->b_welding==true)//从焊接过程中熄弧
                                                            {
                                                                _p->b_welding=false;
                                                                Robmovemodel_ID movemod=(Robmovemodel)_p->mb_mapping->tab_registers[ROB_MOVEMOD_REG_ADD];
                                                                float f_movX=*(float*)&_p->mb_mapping->tab_registers[ROB_MOVE_X_POS_FH_REG_ADD];
                                                                float f_movY=*(float*)&_p->mb_mapping->tab_registers[ROB_MOVE_Y_POS_FH_REG_ADD];
                                                                float f_movZ=*(float*)&_p->mb_mapping->tab_registers[ROB_MOVE_Z_POS_FH_REG_ADD];
                                                                float f_movRX=*(float*)&_p->mb_mapping->tab_registers[ROB_MOVE_RX_POS_FH_REG_ADD];
                                                                float f_movRY=*(float*)&_p->mb_mapping->tab_registers[ROB_MOVE_RY_POS_FH_REG_ADD];
                                                                float f_movRZ=*(float*)&_p->mb_mapping->tab_registers[ROB_MOVE_RZ_POS_FH_REG_ADD];
                                                                switch(movemod)
                                                                {
                                                                    case MOVEL:
                                                                    case MOVEP:
                                                                    {
                                                                        msg="do lwe #["+
                                                                                QString::number(f_movX,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                                QString::number(f_movY,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                                QString::number(f_movZ,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                                QString::number(f_movRX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                                                QString::number(f_movRY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                                                QString::number(f_movRZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+
                                                                                "],1,1\r\n";
                                                                    }
                                                                    break;
                                                                    case MOVEJ:
                                                                    {
                                                                        msg="do jwe #["+
                                                                            QString::number(f_movX,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                            QString::number(f_movY,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                            QString::number(f_movZ,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                            QString::number(f_movRX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                                            QString::number(f_movRY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                                            QString::number(f_movRZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+
                                                                            "],1,1\r\n";
                                                                    }
                                                                    break;
                                                                    case MOVEC:
                                                                    {
                                                                        //川崎没有单独的movec熄弧，用movej熄弧
                                                                        msg="do jwe #["+
                                                                            QString::number(f_movX,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                            QString::number(f_movY,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                            QString::number(f_movZ,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                            QString::number(f_movRX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                                            QString::number(f_movRY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                                            QString::number(f_movRZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+
                                                                            "],1,1\r\n";
                                                                    }
                                                                    break;
                                                                }
                                                            }
                                                        break;
                                                        case FIRE:         //起弧
                                                            _p->b_startweld_init=true;
                                                            _p->b_welding=true;
                                                        break;
                                                        case WIND:         //送丝

                                                        break;
                                                        case REWIND:       //退丝

                                                        break;
                                                        case GASS:         //出气

                                                        break;
                                                    }
                                                    std::string str=msg.toStdString();
                                                    _p->send_buf_group.push_back(str);
                                                    mutexsend_buf_group.unlock();
                                                }
                                                break;
                                            }
                                        }
                                        break;
                                        /*
                                        case SOMEONE:   //非直连控制指令
                                        {
                                            switch(u16data_elec_work)
                                            {
                                                case STATIC:    //空闲

                                                break;
                                                case FIRE:         //起弧

                                                break;
                                                case WIND:         //送丝

                                                break;
                                                case REWIND:       //退丝

                                                break;
                                                case GASS:         //出气

                                                break;
                                            }
                                        }
                                        break;
                                        */
                                    }
                                }
                                old_u16data_elec_work=u16data_elec_work;
                            }
                            if(_p->mb_mapping->tab_registers[ROB_MOVEMOD_REG_ADD]!=65535)//机器人要移动
                            {
                                uint16_t tcp=_p->mb_mapping->tab_registers[ROB_TCP_NUM_REG_ADD];
                                Robmovemodel_ID movemod=(Robmovemodel)_p->mb_mapping->tab_registers[ROB_MOVEMOD_REG_ADD];
                                float f_speed=*(float*)&_p->mb_mapping->tab_registers[ROB_MOVESPEED_FH_REG_ADD];
                                float f_movX=*(float*)&_p->mb_mapping->tab_registers[ROB_MOVE_X_POS_FH_REG_ADD];
                                float f_movY=*(float*)&_p->mb_mapping->tab_registers[ROB_MOVE_Y_POS_FH_REG_ADD];
                                float f_movZ=*(float*)&_p->mb_mapping->tab_registers[ROB_MOVE_Z_POS_FH_REG_ADD];
                                float f_movRX=*(float*)&_p->mb_mapping->tab_registers[ROB_MOVE_RX_POS_FH_REG_ADD];
                                float f_movRY=*(float*)&_p->mb_mapping->tab_registers[ROB_MOVE_RY_POS_FH_REG_ADD];
                                float f_movRZ=*(float*)&_p->mb_mapping->tab_registers[ROB_MOVE_RZ_POS_FH_REG_ADD];
                                float f_movX1=*(float*)&_p->mb_mapping->tab_registers[ROB_MOVE_X1_POS_FH_REG_ADD];
                                float f_movY1=*(float*)&_p->mb_mapping->tab_registers[ROB_MOVE_Y1_POS_FH_REG_ADD];
                                float f_movZ1=*(float*)&_p->mb_mapping->tab_registers[ROB_MOVE_Z1_POS_FH_REG_ADD];
                                float f_movRX1=*(float*)&_p->mb_mapping->tab_registers[ROB_MOVE_RX1_POS_FH_REG_ADD];
                                float f_movRY1=*(float*)&_p->mb_mapping->tab_registers[ROB_MOVE_RY1_POS_FH_REG_ADD];
                                float f_movRZ1=*(float*)&_p->mb_mapping->tab_registers[ROB_MOVE_RZ1_POS_FH_REG_ADD];
                                int i_out1=*(int*)&_p->mb_mapping->tab_registers[ROB_MOVE_OUT_1_POS_FH_REG_ADD];
                                int i_out2=*(int*)&_p->mb_mapping->tab_registers[ROB_MOVE_OUT_2_POS_FH_REG_ADD];
                                int i_out3=*(int*)&_p->mb_mapping->tab_registers[ROB_MOVE_OUT_3_POS_FH_REG_ADD];
                                int i_out1_1=*(int*)&_p->mb_mapping->tab_registers[ROB_MOVE_OUT1_1_POS_FH_REG_ADD];
                                int i_out1_2=*(int*)&_p->mb_mapping->tab_registers[ROB_MOVE_OUT1_2_POS_FH_REG_ADD];
                                int i_out1_3=*(int*)&_p->mb_mapping->tab_registers[ROB_MOVE_OUT1_3_POS_FH_REG_ADD];
                                RobotMove(f_movX,f_movY,f_movZ,f_movRX,f_movRY,f_movRZ,f_movX1,f_movY1,f_movZ1,f_movRX1,f_movRY1,f_movRZ1,
                                          i_out1,i_out2,i_out3,i_out1_1,i_out1_2,i_out1_3,
                                          movemod,tcp,f_speed);
                            }
                            //机器人只是单独改TCP
                            else if((query[7] == 0x10&&ROB_TCP_NUM_REG_ADD==(((uint16_t)query[8])<<8)+(uint16_t)query[9]&&1==(((uint16_t)query[10])<<8)+(uint16_t)query[11])
                                  ||(query[7] == 0x04&&ROB_TCP_NUM_REG_ADD==(((uint16_t)query[8])<<8)+(uint16_t)query[9]))

                            {
                                uint16_t tcp=_p->mb_mapping->tab_registers[ROB_TCP_NUM_REG_ADD];
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
                                        QString msg;    //机器人tcp口设置0
                                        msg="Tool("+QString::number(tcp)+")";
                                        std::string str=msg.toStdString();
                                        _p->send_buf_group.push_back(str);
                                        mutexsend_buf_group.unlock();
                                    }
                                    break;
                                    case ROBOT_MODEL_UR://优傲机器人
                                    {

                                    }
                                    break;
                                    case ROBOT_MODEL_KUKA://库卡机器人
                                    {     
                                        mutexsend_buf_group.lock();
                                        QDomDocument doc;
                                        QDomText s_data;
                                        QDomElement root_elem = doc.createElement("ROBOTCONTROL");
                                        doc.appendChild(root_elem);
                                        QDomElement obj;
                                        obj=doc.createElement("TCP");
                                        s_data=doc.createTextNode(QString::number(tcp));
                                        obj.appendChild(s_data);
                                        root_elem.appendChild(obj);
                                        QString msg=doc.toString();
                                        std::string str=msg.toStdString();
                                        _p->send_buf_group.push_back(str);
                                        mutexsend_buf_group.unlock();
                                    }
                                    break;
                                    case ROBOT_MODEL_KAWASAKI://川崎机器人
                                    {

                                    }
                                    break;
                                }
                            }
                            else if(_p->mb_mapping->tab_registers[ROB_MOVESPEED_FH_REG_ADD]!=65535||//机器人只是单独改移动速度
                                    _p->mb_mapping->tab_registers[ROB_MOVESPEED_FL_REG_ADD]!=65535)
                            {
                                float f_speed=*(float*)&_p->mb_mapping->tab_registers[ROB_MOVESPEED_FH_REG_ADD];
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
                                        QString msg;
                                        msg="SpeedL("+QString::number((int)f_speed)+")";
                                        std::string str=msg.toStdString();
                                        _p->send_buf_group.push_back(str);
                                        mutexsend_buf_group.unlock();
                                    }
                                    break;
                                    case ROBOT_MODEL_UR://优傲机器人
                                    {
                                        mutexsend_buf_group.lock();
                                        QString msg;
                                        float f_a=1.2;  //加速度
                                        float t_min=0;  //函数返回前的最短时间
                                        msg="speedl("+QString::number(f_speed,'f',ROBOT_SPEED_DECIMAL_PLACE)+","+
                                                      QString::number(f_a,'f',ROBOT_SPEED_DECIMAL_PLACE)+","+
                                                      QString::number(t_min,'f',3)+")";
                                        std::string str=msg.toStdString();
                                        _p->send_buf_group.push_back(str);
                                        mutexsend_buf_group.unlock();
                                    }
                                    break;
                                    case ROBOT_MODEL_KUKA://库卡机器人
                                    {
                                        mutexsend_buf_group.lock();
                                        QDomDocument doc;
                                        QDomText s_data;
                                        QDomElement root_elem = doc.createElement("ROBOTCONTROL");
                                        doc.appendChild(root_elem);
                                        QDomElement obj;
                                        obj=doc.createElement("MOVESPEED");
                                        s_data=doc.createTextNode(QString::number(f_speed));
                                        obj.appendChild(s_data);
                                        root_elem.appendChild(obj);
                                        QString msg=doc.toString();
                                        std::string str=msg.toStdString();
                                        _p->send_buf_group.push_back(str);
                                        mutexsend_buf_group.unlock();
                                    }
                                    break;
                                    case ROBOT_MODEL_KAWASAKI://川崎机器人
                                    {

                                    }
                                    break;
                                }
                            }

                            if(_p->mb_mapping->tab_registers[ROB_IO_OUTPUT1_REG_ADD]!=65535)//机器人IO输出变化
                            {
                                std::vector<int> io_output(ROBOTOUTPUTNUM);
                                for(int n=0;n<ROBOTOUTPUTNUM;n++)
                                {
                                    io_output[n]=_p->mb_mapping->tab_registers[ROB_IO_OUTPUT1_REG_ADD+n];
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
                                        mutexsend_buf_group.lock();
                                        QString msg;    //机器人IO口设置0
                                        QString msg2;
                                        for(int n=0;n<ROBOTOUTPUTNUM;n++)
                                        {
                                            msg2=msg2+QString::number(n+1)+","+QString::number(io_output[n]);
                                            if(n<ROBOTOUTPUTNUM-1)
                                            {
                                               msg2=msg2+",";
                                            }
                                        }
                                        msg="DOGroup("+msg2+")";
                                        std::string str=msg.toStdString();
                                        _p->send_buf_group.push_back(str);
                                        mutexsend_buf_group.unlock();
                                    }
                                    break;
                                    case ROBOT_MODEL_UR://优傲机器人
                                    {

                                    }
                                    break;
                                    case ROBOT_MODEL_KUKA://库卡机器人
                                    {
                                        mutexsend_buf_group.lock();
                                        QDomDocument doc;
                                        QDomText s_data;
                                        QDomElement root_elem = doc.createElement("ROBOTCONTROL");
                                        doc.appendChild(root_elem);
                                        QDomElement obj;
                                        obj=doc.createElement("IO");
                                        for(int n=0;n<io_output.size();n++)
                                        {
                                            QDomElement objmin;
                                            objmin=doc.createElement("OUT"+QString::number(n+1));
                                            s_data=doc.createTextNode(QString::number(io_output[n]));
                                            objmin.appendChild(s_data);
                                            obj.appendChild(objmin);
                                        }
                                        root_elem.appendChild(obj);
                                        QString msg=doc.toString();
                                        std::string str=msg.toStdString();
                                        _p->send_buf_group.push_back(str);
                                        mutexsend_buf_group.unlock();
                                    }
                                    break;
                                    case ROBOT_MODEL_KAWASAKI://川崎机器人
                                    {

                                    }
                                    break;
                                }
                            }
                            if(_p->mb_mapping->tab_registers[ROB_A_OUTPUT1_FH_REG_ADD]!=65535
                             &&_p->mb_mapping->tab_registers[ROB_A_OUTPUT1_FL_REG_ADD]!=65535)//机器人模拟量有变化
                            {
                                std::vector<float> A_output(ROBOTAOUTPUTNUM);
                                for(int n=0;n<ROBOTAOUTPUTNUM;n++)
                                {
                                    A_output[n]=*(float*)&_p->mb_mapping->tab_registers[ROB_A_OUTPUT1_FH_REG_ADD+n*2];
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
                                        mutexsend_buf_group.lock();
                                        QString msg="Sync()";
                                        std::string str=msg.toStdString();
                                        _p->send_buf_group.push_back(str);
                                        msg="AO(1,"+QString::number(A_output[0],'f',3)+")";
                                        str=msg.toStdString();
                                        _p->send_buf_group.push_back(str);

                                        msg="Sync()";
                                        str=msg.toStdString();
                                        _p->send_buf_group.push_back(str);
                                        msg="AO(2,"+QString::number(A_output[1],'f',3)+")";
                                        str=msg.toStdString();
                                        _p->send_buf_group.push_back(str);
                                        mutexsend_buf_group.unlock();
                                    }
                                    break;
                                    case ROBOT_MODEL_UR://优傲机器人
                                    {

                                    }
                                    break;
                                    case ROBOT_MODEL_KUKA://库卡机器人
                                    {
                                        mutexsend_buf_group.lock();
                                        QDomDocument doc;
                                        QDomText s_data;
                                        QDomElement root_elem = doc.createElement("ROBOTCONTROL");
                                        doc.appendChild(root_elem);
                                        QDomElement obj;
                                        obj=doc.createElement("AO");
                                        for(int n=0;n<A_output.size();n++)
                                        {
                                            QDomElement objmin;
                                            objmin=doc.createElement("OUT"+QString::number(n+1));
                                            s_data=doc.createTextNode(QString::number(A_output[n],'f',3));
                                            objmin.appendChild(s_data);
                                            obj.appendChild(objmin);
                                        }
                                        root_elem.appendChild(obj);
                                        QString msg=doc.toString();
                                        std::string str=msg.toStdString();
                                        _p->send_buf_group.push_back(str);
                                        mutexsend_buf_group.unlock();
                                    }
                                    break;
                                    case ROBOT_MODEL_KAWASAKI://川崎机器人
                                    {

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
                                                mutextotalcontrol_buf_group.lock();
                                                QString msg="stop";
                                                std::string str=msg.toStdString();
                                                _p->totalcontrol_buf_group.push_back(str);
                                                mutextotalcontrol_buf_group.unlock();
                                            }
                                            break;
                                            case ROBOT_MODEL_DOBOT://越彊机器人
                                            {
                                                mutextotalcontrol_buf_group.lock();
                                                QString msg="StopScript()";
                                                std::string str=msg.toStdString();
                                                _p->totalcontrol_buf_group.push_back(str);
                                                msg="ResetRobot()";
                                                str=msg.toStdString();
                                                _p->totalcontrol_buf_group.push_back(str);
                                                mutextotalcontrol_buf_group.unlock();
                                            }
                                            break;
                                            case ROBOT_MODEL_UR://优傲机器人
                                            {
                                                mutextotalcontrol_buf_group.lock();
                                                QString msg="stop\r\n";
                                                std::string str=msg.toStdString();
                                                _p->totalcontrol_buf_group.push_back(str);
                                                mutextotalcontrol_buf_group.unlock();
                                            }
                                            break;
                                            case ROBOT_MODEL_KUKA://库卡机器人
                                            {
                                                mutexsend_buf_group.lock();
                                                QDomDocument doc;
                                                QDomText s_data;
                                                QDomElement root_elem = doc.createElement("ROBOTCONTROL");
                                                doc.appendChild(root_elem);
                                                QDomElement obj;
                                                obj=doc.createElement("RUNSTATE");
                                                s_data=doc.createTextNode(QString::number(stopreg));
                                                obj.appendChild(s_data);
                                                root_elem.appendChild(obj);
                                                QString msg=doc.toString();
                                                std::string str=msg.toStdString();
                                                _p->send_buf_group.push_back(str);
                                                mutexsend_buf_group.unlock();
                                            }
                                            break;
                                            case ROBOT_MODEL_KAWASAKI://川崎机器人
                                            {
                                                mutexsend_buf_group.lock();
                                                QString msg="do halt\r\n";
                                                std::string str=msg.toStdString();
                                                _p->send_buf_group.push_back(str);
                                                mutexsend_buf_group.unlock();
                                            }
                                            break;
                                        }
                                    }
                                    break;
                                    case 1://机器人暂停缓存脚本
                                    {
                                        if(_p->m_mcs->tosendbuffer->paused_key==CMD_TRACE_KEY)
                                        {
                                            mutexmovepoint_buffer_group.lock();
                                            _p->pause_movepoint_buffer=_p->movepoint_buffer;
                                            if(_p->pause_movepoint_buffer.size()>0)
                                            {
                                                RobPos robpos=_p->m_mcs->rob->TCPpos;//当前机器人的位置
                                                Eigen::Vector3d P(robpos.X,robpos.Y,robpos.Z);
                                                double mindis=DBL_MAX;
                                                for(int n=0;n<_p->pause_movepoint_buffer.size();n++)
                                                {
                                                    Eigen::Vector3d p(_p->pause_movepoint_buffer[n].robpos.X,_p->pause_movepoint_buffer[n].robpos.Y,_p->pause_movepoint_buffer[n].robpos.Z);
                                                    double dis=(p-P).norm();
                                                    if(dis<mindis)
                                                    {
                                                        mindis=dis;
                                                        _p->pause_movepointN=n;
                                                    }
                                                }
                                                if(mindis>ROBOTCONTROL_PAUSE_DIS)//说明还在往第0号点运动
                                                {
                                                    _p->pause_movepointN=0;
                                                }
                                            }
                                            mutexmovepoint_buffer_group.unlock();
                                        }
                                        switch(_p->rob_mod)
                                        {
                                            case ROBOT_MODEL_NULL://无机器人
                                            {

                                            }
                                            break;
                                            case ROBOT_MODEL_EMERGEN://智昌机器人
                                            {
                                                mutextotalcontrol_buf_group.lock();
                                                QString msg="pause";
                                                std::string str=msg.toStdString();
                                                _p->totalcontrol_buf_group.push_back(str);
                                                mutextotalcontrol_buf_group.unlock();
                                            }
                                            break;
                                            case ROBOT_MODEL_DOBOT://越彊机器人
                                            {
                                                mutextotalcontrol_buf_group.lock();
                                             // QString msg="PauseScript()";
                                                QString msg="StopScript()";
                                                std::string str=msg.toStdString();
                                                _p->totalcontrol_buf_group.push_back(str);
                                                /*
                                                msg="ResetRobot()";
                                                str=msg.toStdString();
                                                _p->totalcontrol_buf_group.push_back(str);*/
                                                mutextotalcontrol_buf_group.unlock();
                                            }
                                            break;
                                            case ROBOT_MODEL_UR://优傲机器人
                                            {
                                                mutextotalcontrol_buf_group.lock();
                                                QString msg="pause\r\n";
                                                std::string str=msg.toStdString();
                                                _p->totalcontrol_buf_group.push_back(str);
                                                mutextotalcontrol_buf_group.unlock();
                                            }
                                            break;
                                            case ROBOT_MODEL_KUKA://库卡机器人
                                            {
                                                mutexsend_buf_group.lock();
                                                QDomDocument doc;
                                                QDomText s_data;
                                                QDomElement root_elem = doc.createElement("ROBOTCONTROL");
                                                doc.appendChild(root_elem);
                                                QDomElement obj;
                                                obj=doc.createElement("RUNSTATE");
                                                s_data=doc.createTextNode(QString::number(stopreg));
                                                obj.appendChild(s_data);
                                                root_elem.appendChild(obj);
                                                QString msg=doc.toString();
                                                std::string str=msg.toStdString();
                                                _p->send_buf_group.push_back(str);
                                                mutexsend_buf_group.unlock();
                                            }
                                            break;
                                            case ROBOT_MODEL_KAWASAKI://川崎机器人
                                            {
                                                mutexsend_buf_group.lock();
                                                QString msg="do hold\r\n";
                                                std::string str=msg.toStdString();
                                                _p->send_buf_group.push_back(str);
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
                                                mutextotalcontrol_buf_group.lock();
                                                QString msg="play";
                                                std::string str=msg.toStdString();
                                                _p->totalcontrol_buf_group.push_back(str);
                                                mutextotalcontrol_buf_group.unlock();
                                            }
                                            break;
                                            case ROBOT_MODEL_DOBOT://越彊机器人
                                            {
                                                mutextotalcontrol_buf_group.lock();
                                            //  QString msg="ContinueScript()";
                                                QString msg="StopScript()";
                                                std::string str=msg.toStdString();
                                                _p->totalcontrol_buf_group.push_back(str);
                                                /*
                                                msg="ResetRobot()";
                                                str=msg.toStdString();
                                                _p->totalcontrol_buf_group.push_back(str);
                                                */
                                                mutextotalcontrol_buf_group.unlock();
                                            }
                                            break;
                                            case ROBOT_MODEL_UR://优傲机器人
                                            {
                                                mutextotalcontrol_buf_group.lock();
                                                QString msg="play\r\n";
                                                std::string str=msg.toStdString();
                                                _p->totalcontrol_buf_group.push_back(str);
                                                mutextotalcontrol_buf_group.unlock();
                                            }
                                            break;
                                            case ROBOT_MODEL_KUKA://库卡机器人
                                            {
                                                mutexsend_buf_group.lock();
                                                QDomDocument doc;
                                                QDomText s_data;
                                                QDomElement root_elem = doc.createElement("ROBOTCONTROL");
                                                doc.appendChild(root_elem);
                                                QDomElement obj;
                                                obj=doc.createElement("RUNSTATE");
                                                s_data=doc.createTextNode(QString::number(stopreg));
                                                obj.appendChild(s_data);
                                                root_elem.appendChild(obj);
                                                QString msg=doc.toString();
                                                std::string str=msg.toStdString();
                                                _p->send_buf_group.push_back(str);
                                                mutexsend_buf_group.unlock();
                                            }
                                            break;
                                            case ROBOT_MODEL_KAWASAKI://川崎机器人
                                            {
                                                mutexsend_buf_group.lock();
                                                QString msg="do continue\r\n";
                                                std::string str=msg.toStdString();
                                                _p->send_buf_group.push_back(str);
                                                mutexsend_buf_group.unlock();
                                            }
                                            break;
                                        }
                                        if(_p->m_mcs->tosendbuffer->paused_key==CMD_TRACE_KEY)
                                        {
                                            /***************************/
                                            //恢复暂停后的移动
                                            for(int n=_p->pause_movepointN;n<_p->pause_movepoint_buffer.size();n++)//焊接运动到一半暂停了
                                            {
                                                uint16_t tcp=_p->pause_movepoint_buffer[n].tcp;
                                                Robmovemodel_ID movemod=_p->pause_movepoint_buffer[n].movemod;
                                                float f_speed=_p->pause_movepoint_buffer[n].f_speed;

                                                float f_movX=_p->pause_movepoint_buffer[n].robpos.X;
                                                float f_movY=_p->pause_movepoint_buffer[n].robpos.Y;
                                                float f_movZ=_p->pause_movepoint_buffer[n].robpos.Z;
                                                float f_movRX=_p->pause_movepoint_buffer[n].robpos.RX;
                                                float f_movRY=_p->pause_movepoint_buffer[n].robpos.RY;
                                                float f_movRZ=_p->pause_movepoint_buffer[n].robpos.RZ;
                                                float f_movX1=_p->pause_movepoint_buffer[n].robpos.X1;
                                                float f_movY1=_p->pause_movepoint_buffer[n].robpos.Y1;
                                                float f_movZ1=_p->pause_movepoint_buffer[n].robpos.Z1;
                                                float f_movRX1=_p->pause_movepoint_buffer[n].robpos.RX1;
                                                float f_movRY1=_p->pause_movepoint_buffer[n].robpos.RY1;
                                                float f_movRZ1=_p->pause_movepoint_buffer[n].robpos.RZ1;
                                                int i_out1=_p->pause_movepoint_buffer[n].robpos.out_1;
                                                int i_out2=_p->pause_movepoint_buffer[n].robpos.out_2;
                                                int i_out3=_p->pause_movepoint_buffer[n].robpos.out_3;
                                                int i_out1_1=_p->pause_movepoint_buffer[n].robpos.out1_1;
                                                int i_out1_2=_p->pause_movepoint_buffer[n].robpos.out1_2;
                                                int i_out1_3=_p->pause_movepoint_buffer[n].robpos.out1_3;

                                                RobotMove(f_movX,f_movY,f_movZ,f_movRX,f_movRY,f_movRZ,
                                                          f_movX1,f_movY1,f_movZ1,f_movRX1,f_movRY1,f_movRZ1,
                                                          i_out1,i_out2,i_out3,i_out1_1,i_out1_2,i_out1_3,
                                                          movemod,tcp,f_speed);
                                            }
                                            /********************************/
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
                else if (ret > 0)//读寄存器
                {
                    modbus_reply(_p->ctx_robotcontrol, query, ret, _p->mb_mapping);
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
        int nextlinktime=5;//失败后5m秒再重联
        usleep(100000);//每隔100ms秒查看一次机器人
        static QString old_rodb_ip="0.0.0.0";   //老机器人IP
        u_int16_t ip[4];
        ip[0]=_p->mb_mapping->tab_registers[ROB_IPADDR_1_REG_ADD];
        ip[1]=_p->mb_mapping->tab_registers[ROB_IPADDR_2_REG_ADD];
        ip[2]=_p->mb_mapping->tab_registers[ROB_IPADDR_3_REG_ADD];
        ip[3]=_p->mb_mapping->tab_registers[ROB_IPADDR_4_REG_ADD];
        QString rodb_ip=QString::number(ip[0])+"."+                 //机器人IP
                        QString::number(ip[1])+"."+
                        QString::number(ip[2])+"."+
                        QString::number(ip[3]);
        int tcp=_p->mb_mapping->tab_registers[ROB_TCP_NUM_REG_ADD];

        static u_int16_t old_robot_totalcontrolnEn=0;
        u_int16_t robot_totalcontrolnEn=_p->mb_mapping->tab_registers[ROB_TOLTAL_CONTROL_REG_ADD];//控制端口连接

        static ROBOT_MODEL old_rob_mod=ROBOT_MODEL_NULL;    //老机器人型号
        _p->rob_mod=(ROBOT_MODEL)_p->mb_mapping->tab_registers[ROB_MODEL_REG_ADD];
        if(old_rodb_ip!=rodb_ip||old_rob_mod!=_p->rob_mod||_p->b_relink==true||old_robot_totalcontrolnEn!=robot_totalcontrolnEn)
        {
            _p->mb_mapping->tab_registers[ROB_CONNECT_STATE_REG_ADD]=0;//远程连接是否完成
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
                    old_rodb_ip=rodb_ip;
                    old_robot_totalcontrolnEn=robot_totalcontrolnEn;
                    old_rob_mod=_p->rob_mod;
                }
                break;
                case ROBOT_MODEL_EMERGEN://智昌机器人
                {
                    _p->m_client.CreateSocket();
                    if(false==_p->m_client.Connect(rodb_ip.toStdString().c_str(),ROBOT_EMERGEN_INFO_PORT))
                    {
                        main_record.lock();
                        QString return_msg=QString::fromLocal8Bit("与远端机器人数据端口连接失败");
                        _p->m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        sleep(nextlinktime);
                        continue;
                    }
                    _p->m_client.SetBlock(0);
                    if(0!=_p->m_client.SetRcvBufferlong(ROBOT_EMERGEN_INFO_RECVBUFFER_MAX*2))
                    {
                        main_record.lock();
                        QString return_msg=QString::fromLocal8Bit("接收远端机器人数据缓存申请失败");
                        _p->m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        sleep(nextlinktime);
                        continue;
                    }
                    _p->b_client=true;

                    _p->rcv_buf=new uint8_t[ROBOT_EMERGEN_INFO_RECVBUFFER_MAX*2+1];
                    _p->b_rcv_thread=true;
                    _p->rcv_thread->start();

                    if(robot_totalcontrolnEn!=0)
                    {
                        _p->m_sendent.CreateSocket();
                        if(false==_p->m_sendent.Connect(rodb_ip.toStdString().c_str(),ROBOT_EMERGEN_SEND_PORT))
                        {
                            main_record.lock();
                            QString return_msg=QString::fromLocal8Bit("与远端机器人命令端口连接失败");
                            _p->m_mcs->main_record.push_back(return_msg);
                            main_record.unlock();
                            sleep(nextlinktime);
                            continue;
                        }
                        _p->m_sendent.SetBlock(0);
                    #ifdef OPEN_SHOW_ROBOTSOCKDATA
                        _p->sendrcv_buf=new uint8_t[ROBOT_EMERGEN_INFO_SENDRECVBUFFER_MAX*2+1];
                        if(0!=_p->m_sendent.SetRcvBufferlong(ROBOT_EMERGEN_INFO_SENDRECVBUFFER_MAX*2))
                        {
                            main_record.lock();
                            QString return_msg=QString::fromLocal8Bit("接收远端机器人命令缓存申请失败");
                            _p->m_mcs->main_record.push_back(return_msg);
                            main_record.unlock();
                            sleep(nextlinktime);
                            continue;
                        }
                    #endif
                        mutexsend_buf_group.lock();
                        _p->send_buf_group.clear();
                        mutexsend_buf_group.unlock();
                        _p->b_sendent=true;
                        _p->b_send_thread=true;
                        _p->send_Thread->start();
                    #ifdef OPEN_SHOW_ROBOTSOCKDATA
                        _p->b_sendrcv_thread=true;
                        _p->sendrcv_Thread->start();
                    #endif

                        _p->m_totalcontrolent.CreateSocket();
                        if(false==_p->m_totalcontrolent.Connect(rodb_ip.toStdString().c_str(),ROBOT_EMERGEN_TOTALCONTROL_RORT))
                        {
                            main_record.lock();
                            QString return_msg=QString::fromLocal8Bit("与远端机器人总控端口连接失败");
                            _p->m_mcs->main_record.push_back(return_msg);
                            main_record.unlock();
                            sleep(nextlinktime);
                            continue;
                        }
                        _p->m_totalcontrolent.SetBlock(0);
                    #ifdef OPEN_SHOW_ROBOTSOCKDATA
                        _p->totalcontrolrcv_buf=new uint8_t[ROBOT_EMERGEN_INFO_TOTALCONTROLCVBUFFER_MAX*2+1];
                        if(0!=_p->m_totalcontrolent.SetRcvBufferlong(ROBOT_EMERGEN_INFO_TOTALCONTROLCVBUFFER_MAX*2))
                        {
                            main_record.lock();
                            QString return_msg=QString::fromLocal8Bit("接收远端机器人总控缓存申请失败");
                            _p->m_mcs->main_record.push_back(return_msg);
                            main_record.unlock();
                            sleep(nextlinktime);
                            continue;
                        }
                    #endif
                        mutextotalcontrol_buf_group.lock();
                        _p->totalcontrol_buf_group.clear();
                        mutextotalcontrol_buf_group.unlock();
                        _p->b_totalcontrolent=true;
                        _p->b_totalcontrol_Thread=true;
                        _p->totalcontrol_Thread->start();
                    #ifdef OPEN_SHOW_ROBOTSOCKDATA
                        _p->b_totalcontrolrcv_Thread=true;
                        _p->totalcontrolrcv_Thread->start();
                    #endif
                    }

                    old_rodb_ip=rodb_ip;
                    old_robot_totalcontrolnEn=robot_totalcontrolnEn;
                    old_rob_mod=_p->rob_mod;
                    _p->b_relink=false;
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
                        sleep(nextlinktime);
                        continue;
                    }
                    _p->m_client.SetBlock(0);
                    if(0!=_p->m_client.SetRcvBufferlong(ROBOT_DOBOT_INFO_RECVBUFFER_MAX*2))
                    {
                        main_record.lock();
                        QString return_msg=QString::fromLocal8Bit("接收远端机器人数据缓存申请失败");
                        _p->m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        sleep(nextlinktime);
                        continue;
                    }
                    _p->b_client=true;

                    _p->rcv_buf=new uint8_t[ROBOT_DOBOT_INFO_RECVBUFFER_MAX*2+1];
                    _p->b_rcv_thread=true;
                    _p->rcv_thread->start();

                    if(robot_totalcontrolnEn!=0)
                    {
                        _p->m_sendent.CreateSocket();
                        if(false==_p->m_sendent.Connect(rodb_ip.toStdString().c_str(),ROBOT_DOBOT_SEND_PORT))
                        {
                            main_record.lock();
                            QString return_msg=QString::fromLocal8Bit("与远端机器人命令端口连接失败");
                            _p->m_mcs->main_record.push_back(return_msg);
                            main_record.unlock();
                            sleep(nextlinktime);
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
                            sleep(nextlinktime);
                            continue;
                        }
                    #endif
                        mutexsend_buf_group.lock();
                        _p->send_buf_group.clear();
                        mutexsend_buf_group.unlock();
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
                            sleep(nextlinktime);
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
                            sleep(nextlinktime);
                            continue;
                        }
                    #endif
                        mutextotalcontrol_buf_group.lock();
                        _p->totalcontrol_buf_group.clear();
                        mutextotalcontrol_buf_group.unlock();
                        _p->b_totalcontrolent=true;
                        _p->b_totalcontrol_Thread=true;
                        _p->totalcontrol_Thread->start();
                    #ifdef OPEN_SHOW_ROBOTSOCKDATA
                        _p->b_totalcontrolrcv_Thread=true;
                        _p->totalcontrolrcv_Thread->start();
                    #endif
                    }

                    old_rodb_ip=rodb_ip;
                    old_robot_totalcontrolnEn=robot_totalcontrolnEn;
                    old_rob_mod=_p->rob_mod;
                    _p->b_relink=false;
                }
                break;
                case ROBOT_MODEL_UR://优傲机器人
                {
                    _p->m_client.CreateSocket();
                    if(false==_p->m_client.Connect(rodb_ip.toStdString().c_str(),ROBOT_UR_INFO_PORT))
                    {
                        main_record.lock();
                        QString return_msg=QString::fromLocal8Bit("与远端机器人数据端口连接失败");
                        _p->m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        sleep(nextlinktime);
                        continue;
                    }
                    _p->m_client.SetBlock(0);
                    if(0!=_p->m_client.SetRcvBufferlong(ROBOT_UR_INFO_RECVBUFFER_MAX*2))
                    {
                        main_record.lock();
                        QString return_msg=QString::fromLocal8Bit("接收远端机器人数据缓存申请失败");
                        _p->m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        sleep(nextlinktime);
                        continue;
                    }
                    _p->b_client=true;

                    _p->rcv_buf=new uint8_t[ROBOT_UR_INFO_RECVBUFFER_MAX*2+1];
                    _p->b_rcv_thread=true;
                    _p->rcv_thread->start();

                    if(robot_totalcontrolnEn!=0)
                    {
                        _p->m_sendent.CreateSocket();
                        if(false==_p->m_sendent.Connect(rodb_ip.toStdString().c_str(),ROBOT_UR_SEND_PORT))
                        {
                            main_record.lock();
                            QString return_msg=QString::fromLocal8Bit("与远端机器人命令端口连接失败");
                            _p->m_mcs->main_record.push_back(return_msg);
                            main_record.unlock();
                            sleep(nextlinktime);
                            continue;
                        }
                        _p->m_sendent.SetBlock(0);
                    #ifdef OPEN_SHOW_ROBOTSOCKDATA
                        _p->sendrcv_buf=new uint8_t[ROBOT_UR_INFO_SENDRECVBUFFER_MAX*2+1];
                        if(0!=_p->m_sendent.SetRcvBufferlong(ROBOT_UR_INFO_SENDRECVBUFFER_MAX*2))
                        {
                            main_record.lock();
                            QString return_msg=QString::fromLocal8Bit("接收远端机器人命令缓存申请失败");
                            _p->m_mcs->main_record.push_back(return_msg);
                            main_record.unlock();
                            sleep(nextlinktime);
                            continue;
                        }
                    #endif
                        mutexsend_buf_group.lock();
                        _p->send_buf_group.clear();
                        mutexsend_buf_group.unlock();
                        _p->b_sendent=true;
                        _p->b_send_thread=true;
                        _p->send_Thread->start();
                    #ifdef OPEN_SHOW_ROBOTSOCKDATA
                        _p->b_sendrcv_thread=true;
                        _p->sendrcv_Thread->start();
                    #endif

                        _p->m_totalcontrolent.CreateSocket();
                        if(false==_p->m_totalcontrolent.Connect(rodb_ip.toStdString().c_str(),ROBOT_UR_TOTALCONTROL_RORT))
                        {
                            main_record.lock();
                            QString return_msg=QString::fromLocal8Bit("与远端机器人总控端口连接失败");
                            _p->m_mcs->main_record.push_back(return_msg);
                            main_record.unlock();
                            sleep(nextlinktime);
                            continue;
                        }
                        _p->m_totalcontrolent.SetBlock(0);
                    #ifdef OPEN_SHOW_ROBOTSOCKDATA
                        _p->totalcontrolrcv_buf=new uint8_t[ROBOT_UR_INFO_TOTALCONTROLCVBUFFER_MAX*2+1];
                        if(0!=_p->m_totalcontrolent.SetRcvBufferlong(ROBOT_UR_INFO_TOTALCONTROLCVBUFFER_MAX*2))
                        {
                            main_record.lock();
                            QString return_msg=QString::fromLocal8Bit("接收远端机器人总控缓存申请失败");
                            _p->m_mcs->main_record.push_back(return_msg);
                            main_record.unlock();
                            sleep(nextlinktime);
                            continue;
                        }
                    #endif
                        mutextotalcontrol_buf_group.lock();
                        _p->totalcontrol_buf_group.clear();
                        mutextotalcontrol_buf_group.unlock();
                        _p->b_totalcontrolent=true;
                        _p->b_totalcontrol_Thread=true;
                        _p->totalcontrol_Thread->start();
                    #ifdef OPEN_SHOW_ROBOTSOCKDATA
                        _p->b_totalcontrolrcv_Thread=true;
                        _p->totalcontrolrcv_Thread->start();
                    #endif
                    }

                    old_rodb_ip=rodb_ip;
                    old_robot_totalcontrolnEn=robot_totalcontrolnEn;
                    old_rob_mod=_p->rob_mod;
                    _p->b_relink=false;
                }
                break;
                case ROBOT_MODEL_KUKA://库卡机器人
                {
                    _p->m_client.CreateSocket();
                    if(false==_p->m_client.Connect(rodb_ip.toStdString().c_str(),ROBOT_KUKA_INFO_PORT))
                    {
                        main_record.lock();
                        QString return_msg=QString::fromLocal8Bit("与远端机器人数据端口连接失败");
                        _p->m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        sleep(nextlinktime);
                        continue;
                    }
                    _p->m_client.SetBlock(0);
                    if(0!=_p->m_client.SetRcvBufferlong(ROBOT_KUKA_INFO_RECVBUFFER_MAX*2))
                    {
                        main_record.lock();
                        QString return_msg=QString::fromLocal8Bit("接收远端机器人数据缓存申请失败");
                        _p->m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        sleep(nextlinktime);
                        continue;
                    }
                    _p->b_client=true;

                    _p->rcv_buf=new uint8_t[ROBOT_KUKA_INFO_RECVBUFFER_MAX*2+1];
                    _p->b_rcv_thread=true;
                    _p->rcv_thread->start();

                    if(robot_totalcontrolnEn!=0)
                    {
                        _p->m_sendent.CreateSocket();
                        if(false==_p->m_sendent.Connect(rodb_ip.toStdString().c_str(),ROBOT_KUKA_SEND_PORT))
                        {
                            main_record.lock();
                            QString return_msg=QString::fromLocal8Bit("与远端机器人命令端口连接失败");
                            _p->m_mcs->main_record.push_back(return_msg);
                            main_record.unlock();
                            sleep(nextlinktime);
                            continue;
                        }
                        _p->m_sendent.SetBlock(0);
                    #ifdef OPEN_SHOW_ROBOTSOCKDATA
                        _p->sendrcv_buf=new uint8_t[ROBOT_UR_INFO_SENDRECVBUFFER_MAX*2+1];
                        if(0!=_p->m_sendent.SetRcvBufferlong(ROBOT_UR_INFO_SENDRECVBUFFER_MAX*2))
                        {
                            main_record.lock();
                            QString return_msg=QString::fromLocal8Bit("接收远端机器人命令缓存申请失败");
                            _p->m_mcs->main_record.push_back(return_msg);
                            main_record.unlock();
                            sleep(nextlinktime);
                            continue;
                        }
                    #endif
                        mutexsend_buf_group.lock();
                        _p->send_buf_group.clear();
                        mutexsend_buf_group.unlock();
                        _p->b_sendent=true;
                        _p->b_send_thread=true;
                        _p->send_Thread->start();
                    #ifdef OPEN_SHOW_ROBOTSOCKDATA
                        _p->b_sendrcv_thread=true;
                        _p->sendrcv_Thread->start();
                    #endif
                    }
                    old_rodb_ip=rodb_ip;
                    old_robot_totalcontrolnEn=robot_totalcontrolnEn;
                    old_rob_mod=_p->rob_mod;
                    _p->b_relink=false;
                }
                break;
                case ROBOT_MODEL_KAWASAKI://川崎机器人
                {
                    _p->m_client.CreateSocket();
                    if(false==_p->m_client.Connect(rodb_ip.toStdString().c_str(),ROBOT_KAWASAKI_INFO_PORT))
                    {
                        main_record.lock();
                        QString return_msg=QString::fromLocal8Bit("与远端机器人数据端口连接失败");
                        _p->m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        sleep(nextlinktime);
                        continue;
                    }
                    _p->m_client.SetBlock(0);
                    if(0!=_p->m_client.SetRcvBufferlong(ROBOT_KAWASAKI_INFO_RECVBUFFER_MAX*2))
                    {
                        main_record.lock();
                        QString return_msg=QString::fromLocal8Bit("接收远端机器人数据缓存申请失败");
                        _p->m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        sleep(nextlinktime);
                        continue;
                    }
                    _p->b_client=true;

                    _p->rcv_buf=new uint8_t[ROBOT_KAWASAKI_INFO_RECVBUFFER_MAX*2+1];
                    _p->b_rcv_thread=true;
                    _p->rcv_thread->start();

                    if(robot_totalcontrolnEn!=0)
                    {
                        _p->m_sendent.CreateSocket();
                        if(false==_p->m_sendent.Connect(rodb_ip.toStdString().c_str(),ROBOT_KAWASAKI_SEND_PORT))
                        {
                            main_record.lock();
                            QString return_msg=QString::fromLocal8Bit("与远端机器人命令端口连接失败");
                            _p->m_mcs->main_record.push_back(return_msg);
                            main_record.unlock();
                            sleep(nextlinktime);
                            continue;
                        }
                        _p->m_sendent.SetBlock(0);
                    #ifdef OPEN_SHOW_ROBOTSOCKDATA
                        _p->sendrcv_buf=new uint8_t[ROBOT_KAWASAKI_INFO_SENDRECVBUFFER_MAX*2+1];
                        if(0!=_p->m_sendent.SetRcvBufferlong(ROBOT_KAWASAKI_INFO_SENDRECVBUFFER_MAX*2))
                        {
                            main_record.lock();
                            QString return_msg=QString::fromLocal8Bit("接收远端机器人命令缓存申请失败");
                            _p->m_mcs->main_record.push_back(return_msg);
                            main_record.unlock();
                            sleep(nextlinktime);
                            continue;
                        }
                    #endif
                        mutexsend_buf_group.lock();
                        _p->send_buf_group.clear();
                        mutexsend_buf_group.unlock();
                        _p->b_sendent=true;
                        _p->b_send_thread=true;
                        _p->send_Thread->start();
                    #ifdef OPEN_SHOW_ROBOTSOCKDATA
                        _p->b_sendrcv_thread=true;
                        _p->sendrcv_Thread->start();
                    #endif
                    }
                    old_rodb_ip=rodb_ip;
                    old_robot_totalcontrolnEn=robot_totalcontrolnEn;
                    old_rob_mod=_p->rob_mod;
                    _p->b_relink=false;
                }
                break;
                case ROBOT_MODEL_YASKAWA://安川机器人
                {

                }
                break;
            }
            _p->RobotInit(tcp);
            main_record.lock();
            QString return_msg=QString::fromLocal8Bit("机器人状态获取成功");
            if(robot_totalcontrolnEn!=0)
            {
                //总控有效的情况下机器人才能完全启动
                _p->mb_mapping->tab_registers[ROB_CONNECT_STATE_REG_ADD]=1;
            }
            _p->m_mcs->main_record.push_back(return_msg);
            main_record.unlock();
        }

        static QString old_weld_ip="0.0.0.0";   //老焊机IP
        ip[0]=_p->mb_mapping->tab_registers[ROB_WELD_IPADDR_1_REG_ADD];
        ip[1]=_p->mb_mapping->tab_registers[ROB_WELD_IPADDR_2_REG_ADD];
        ip[2]=_p->mb_mapping->tab_registers[ROB_WELD_IPADDR_3_REG_ADD];
        ip[3]=_p->mb_mapping->tab_registers[ROB_WELD_IPADDR_4_REG_ADD];
        QString weld_ip=QString::number(ip[0])+"."+                 //焊机IP
                        QString::number(ip[1])+"."+
                        QString::number(ip[2])+"."+
                        QString::number(ip[3]);
        static WELD_MODEL old_weld_mod=WELD_MODEL_NULL;     //老焊机型号
        _p->weld_mod=(WELD_MODEL)_p->mb_mapping->tab_registers[ROB_WELD_MODEL_REG_ADD];
        if(old_weld_ip!=weld_ip||old_weld_mod!=_p->weld_mod)
        {
            if(_p->b_weldsendent==true)
            {
                _p->weldsend_Thread->Stop();
                _p->weldsend_Thread->quit();
                _p->weldsend_Thread->wait();
            #ifdef OPEN_SHOW_WELDSOCKDATA
                _p->weldsendrcv_Thread->Stop();
                _p->weldsendrcv_Thread->quit();
                _p->weldsendrcv_Thread->wait();
            #endif
                _p->m_weldsendent.Close();
            #ifdef OPEN_SHOW_WELDSOCKDATA
                if(_p->weldsendrcv_buf!=NULL)
                {
                    delete []_p->weldsendrcv_buf;
                    _p->weldsendrcv_buf=NULL;
                }
            #endif
                _p->b_weldsendent=false;
            }
            switch(_p->weld_mod)
            {
                case WELD_MODEL_NULL://无焊机
                {
                    old_weld_ip=weld_ip;
                    old_weld_mod=_p->weld_mod;
                }
                break;
                case WELD_MODEL_ROBOT_LINK://机器人直连焊机
                {
                    old_weld_ip=weld_ip;
                    old_weld_mod=_p->weld_mod;
                }
                break;
                /*
                case SOMEONE:       //(非直连焊机类似入下通信线程)
                {
                    _p->m_weldsendent.CreateSocket();
                    if(false==_p->m_weldsendent.Connect(weld_ip.toStdString().c_str(),SOMEONEPORT))
                    {
                        main_record.lock();
                        QString return_msg=QString::fromLocal8Bit("与远端焊机命令端口连接失败");
                        _p->m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        sleep(nextlinktime);
                        continue;
                    }
                    _p->m_weldsendent.SetBlock(0);
                #ifdef OPEN_SHOW_WELDSOCKDATA
                    _p->weldsendrcv_buf=new uint8_t[WELD_SOMEONE_INFO_SENDRECVBUFFER_MAX*2+1];
                    if(0!=_p->m_weldsendent.SetRcvBufferlong(WELD_SOMEONE_INFO_SENDRECVBUFFER_MAX*2))
                    {
                        main_record.lock();
                        QString return_msg=QString::fromLocal8Bit("接收远端焊机命令缓存申请失败");
                        _p->m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                        sleep(nextlinktime);
                        continue;
                    }
                #endif
                    mutexweldsend_buf_group.lock();
                    _p->weldsend_buf_group.clear();
                    mutexweldsend_buf_group.unlock();
                    _p->b_weldsendent=true;
                    _p->b_weldsend_thread=true;
                    _p->weldsend_Thread->start();
                #ifdef OPEN_SHOW_WELDSOCKDATA
                    _p->b_weldsendrcv_thread=true;
                    _p->weldsendrcv_Thread->start();
                #endif
                }
                break;
                */
            }
            _p->WeldInit();
            main_record.lock();
            QString return_msg=QString::fromLocal8Bit("焊机状态获取成功");
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
                    int rcvnum=_p->m_client.Recv((char*)_p->rcv_buf,ROBOT_EMERGEN_INFO_RECVBUFFER_MAX*2);
                    if(rcvnum>0)
                    {
                        bool b_succeed=true;
                        QString rcvmsg=QString::fromLocal8Bit((char*)_p->rcv_buf);
                        float x,y,z,q1,q2,q3,q4;//四元素
                        QStringList strList = rcvmsg.split(",");
                        for(int i=0;i<strList.size();i++)
                        {
                            QString p_data=strList[i];
                            if(p_data.size()>0)
                            {
                                QStringList strPo = p_data.split(":");
                                if(strPo.size()==2)
                                {
                                    QString name=strPo[0];
                                    QString s_data=strPo[1];
                                    bool temp;
                                    float f_data=s_data.toFloat(&temp);
                                    if(temp==false)
                                    {
                                        b_succeed=false;
                                        break;
                                    }
                                    if(name=="x")
                                    {
                                        x=f_data;
                                    }
                                    else if(name=="y")
                                    {
                                        y=f_data;
                                    }
                                    else if(name=="z")
                                    {
                                        z=f_data;
                                    }
                                    else if(name=="q1")
                                    {
                                        q1=f_data;
                                    }
                                    else if(name=="q2")
                                    {
                                        q2=f_data;
                                    }
                                    else if(name=="q3")
                                    {
                                        q3=f_data;
                                    }
                                    else if(name=="q4")
                                    {
                                        q4=f_data;
                                    }
                                    else
                                    {
                                        b_succeed=false;
                                        break;
                                    }
                                }
                                else
                                {
                                    b_succeed=false;
                                    break;
                                }
                            }
                        }
                        if(b_succeed==true)
                        {
                            float f_robRX,f_robRY,f_robRZ;
                            Eigen::Quaterniond tempaddvalue;
                            Eigen::Matrix3d tempres;
                            tempaddvalue.w()=q1;
                            tempaddvalue.x()=q2;
                            tempaddvalue.y()=q3;
                            tempaddvalue.z()=q4;
                            tempres=tempaddvalue;
                            std::array<double, 3> s=_p->Yaskawa_RotMatrixXYZ2Euler(tempres);
                            f_robRX=s[0]*CAL_ANGLE;
                            f_robRY=s[1]*CAL_ANGLE;
                            f_robRZ=s[2]*CAL_ANGLE;
                            //四元素转化成正旋
                            _p->mb_mapping->tab_registers[ROB_X_POS_FH_REG_ADD]=((uint16_t*)(&x))[0];
                            _p->mb_mapping->tab_registers[ROB_X_POS_FL_REG_ADD]=((uint16_t*)(&x))[1];
                            _p->mb_mapping->tab_registers[ROB_Y_POS_FH_REG_ADD]=((uint16_t*)(&y))[0];
                            _p->mb_mapping->tab_registers[ROB_Y_POS_FL_REG_ADD]=((uint16_t*)(&y))[1];
                            _p->mb_mapping->tab_registers[ROB_Z_POS_FH_REG_ADD]=((uint16_t*)(&z))[0];
                            _p->mb_mapping->tab_registers[ROB_Z_POS_FL_REG_ADD]=((uint16_t*)(&z))[1];
                            _p->mb_mapping->tab_registers[ROB_RX_POS_FH_REG_ADD]=((uint16_t*)(&f_robRX))[0];
                            _p->mb_mapping->tab_registers[ROB_RX_POS_FL_REG_ADD]=((uint16_t*)(&f_robRX))[1];
                            _p->mb_mapping->tab_registers[ROB_RY_POS_FH_REG_ADD]=((uint16_t*)(&f_robRY))[0];
                            _p->mb_mapping->tab_registers[ROB_RY_POS_FL_REG_ADD]=((uint16_t*)(&f_robRY))[1];
                            _p->mb_mapping->tab_registers[ROB_RZ_POS_FH_REG_ADD]=((uint16_t*)(&f_robRZ))[0];
                            _p->mb_mapping->tab_registers[ROB_RZ_POS_FL_REG_ADD]=((uint16_t*)(&f_robRZ))[1];

                            uint8_t state=0;
                            if(state==0)
                            {
                                mutexmovepoint_buffer_group.lock();
                                int num=_p->movepoint_buffer.size();
                                if(num!=0)
                                {
                                    Pause_PointInfo moveinfo=_p->movepoint_buffer[num-1];
                                    switch(moveinfo.movemod)
                                    {
                                        case MOVEL:
                                        case MOVEJ:
                                        {
                                            if(fabs(x-moveinfo.robpos.X)>0.1
                                             ||fabs(y-moveinfo.robpos.Y)>0.1
                                             ||fabs(z-moveinfo.robpos.Z)>0.1
                                             ||fabs(f_robRX-moveinfo.robpos.RX)>0.1
                                             ||fabs(f_robRY-moveinfo.robpos.RY)>0.1
                                             ||fabs(f_robRZ-moveinfo.robpos.RZ)>0.1)
                                            {
                                                state=1;
                                            }
                                        }
                                        break;
                                        case MOVEC:
                                        {
                                            if(fabs(x-moveinfo.robpos.X1)>0.1
                                             ||fabs(y-moveinfo.robpos.Y1)>0.1
                                             ||fabs(z-moveinfo.robpos.Z1)>0.1
                                             ||fabs(f_robRX-moveinfo.robpos.RX1)>0.1
                                             ||fabs(f_robRY-moveinfo.robpos.RY1)>0.1
                                             ||fabs(f_robRZ-moveinfo.robpos.RZ1)>0.1)
                                            {
                                                state=1;
                                            }
                                        }
                                        break;
                                    }
                                }
                                mutexmovepoint_buffer_group.unlock();
                                _p->mb_mapping->tab_registers[ROB_STATE_REG_ADD]=state;
                            }
                        }
                    }
                }
                break;
                case ROBOT_MODEL_DOBOT://越彊机器人
                {
                    int rcvnum=_p->m_client.Recv((char*)_p->rcv_buf,ROBOT_DOBOT_INFO_RECVBUFFER_MAX*2);
                    if(rcvnum==ROBOT_DOBOT_INFO_RECVBUFFER_MAX)
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

                        /*
                        static int times=0;
                        if(state==0)
                        {
                            if(times<10)
                            {
                                times++;
                                state=1;
                            }
                            else
                            {
                                state=0;
                            }
                        }
                        else
                        {
                            times=0;
                        }
                        */

                        if(state==0)
                        {
                            mutexmovepoint_buffer_group.lock();
                            int num=_p->movepoint_buffer.size();
                            if(num!=0)
                            {
                                Pause_PointInfo moveinfo=_p->movepoint_buffer[num-1];
                                switch(moveinfo.movemod)
                                {
                                    case MOVEL:
                                    case MOVEJ:
                                    {
                                        if(fabs(d_robX-moveinfo.robpos.X)>1.5
                                         ||fabs(d_robY-moveinfo.robpos.Y)>1.5
                                         ||fabs(d_robZ-moveinfo.robpos.Z)>1.5
                                         ||fabs(d_robRX-moveinfo.robpos.RX)>0.5
                                         ||fabs(d_robRY-moveinfo.robpos.RY)>0.5
                                         ||fabs(d_robRZ-moveinfo.robpos.RZ)>0.5)
                                        {
                                            state=1;
                                        }
                                    }
                                    break;
                                    case MOVEC:
                                    {
                                        if(fabs(d_robX-moveinfo.robpos.X1)>1.5
                                         ||fabs(d_robY-moveinfo.robpos.Y1)>1.5
                                         ||fabs(d_robZ-moveinfo.robpos.Z1)>1.5
                                         ||fabs(d_robRX-moveinfo.robpos.RX1)>0.5
                                         ||fabs(d_robRY-moveinfo.robpos.RY1)>0.5
                                         ||fabs(d_robRZ-moveinfo.robpos.RZ1)>0.5)
                                        {
                                            state=1;
                                        }
                                    }
                                    break;
                                }
                            }
                            mutexmovepoint_buffer_group.unlock();
                        }

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
                case ROBOT_MODEL_UR://优傲机器人
                {
                    int rcvnum=_p->m_client.Recv((char*)_p->rcv_buf,ROBOT_UR_INFO_RECVBUFFER_MAX*2);
                    if(rcvnum>=ROBOT_UR_INFO_RECVBUFFER_MAX)
                    {
                        uint64_t d_robX=ntohll(*(uint64_t*)(&_p->rcv_buf[444]));
                        uint64_t d_robY=ntohll(*(uint64_t*)(&_p->rcv_buf[452]));
                        uint64_t d_robZ=ntohll(*(uint64_t*)(&_p->rcv_buf[460]));
                        uint64_t d_robRX=ntohll(*(uint64_t*)(&_p->rcv_buf[468]));
                        uint64_t d_robRY=ntohll(*(uint64_t*)(&_p->rcv_buf[476]));
                        uint64_t d_robRZ=ntohll(*(uint64_t*)(&_p->rcv_buf[484]));
                        uint64_t d_speed=ntohll(*(uint64_t*)(&_p->rcv_buf[940]));
                        uint8_t state=*(double*)&d_speed;

                        //旋转向量转换成旋转矩阵
                        Eigen::Vector3d rvec(*(double*)&d_robRX,*(double*)&d_robRY,*(double*)&d_robRZ);
                        double n_norm=rvec.norm();
                        Eigen::AngleAxisd rotation_vector(n_norm,rvec/n_norm);
                        Eigen::Matrix3d rotm;
                        rotm=rotation_vector.toRotationMatrix();
                        //旋转矩阵转换为内旋RXRYRZ姿态角
                        std::array<double, 3> s=_p->Yaskawa_RotMatrixXYZ2Euler(rotm);

                        float f_robX=(*(double*)&d_robX)*1000;
                        float f_robY=(*(double*)&d_robY)*1000;
                        float f_robZ=(*(double*)&d_robZ)*1000;
                        float f_robRX=s[0]*CAL_ANGLE;
                        float f_robRY=s[1]*CAL_ANGLE;
                        float f_robRZ=s[2]*CAL_ANGLE;
                        float f_speed=*(double*)&d_speed;

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
                case ROBOT_MODEL_KUKA://库卡机器人
                {
                    int rcvnum=_p->m_client.Recv((char*)_p->rcv_buf,ROBOT_KUKA_INFO_RECVBUFFER_MAX*2);
                    if(rcvnum>0)
                    {
                        QString rcvmsg=QString::fromLocal8Bit((char*)_p->rcv_buf,rcvnum);
                        uint8_t state;
                        float f_robX;
                        float f_robY;
                        float f_robZ;
                        float f_robRX;
                        float f_robRY;
                        float f_robRZ;
                        int f_robout1;
                        int f_robout2;
                        int f_robout3;
                        float f_speed;
                        int i_ioinput;

                        QDomDocument doc;
                        if(true==doc.setContent(rcvmsg))
                        {
                            QDomElement root = doc.documentElement();
                            if(root.tagName()=="ROBOTINFO")
                            {
                                auto child = root.childNodes();
                                for(int i=0;i<child.size();i++)
                                {
                                    auto childobj=child.at(i).toElement();
                                    if(childobj.tagName()=="POS")
                                    {
                                        auto child = childobj.childNodes();
                                        for(int i=0;i<child.size();i++)
                                        {
                                            auto childobj=child.at(i).toElement();
                                            if(childobj.tagName()=="X")
                                            {
                                                f_robX=childobj.text().toFloat();
                                                _p->mb_mapping->tab_registers[ROB_X_POS_FH_REG_ADD]=((uint16_t*)(&f_robX))[0];
                                                _p->mb_mapping->tab_registers[ROB_X_POS_FL_REG_ADD]=((uint16_t*)(&f_robX))[1];
                                            }
                                            else if(childobj.tagName()=="Y")
                                            {
                                                f_robY=childobj.text().toFloat();
                                                _p->mb_mapping->tab_registers[ROB_Y_POS_FH_REG_ADD]=((uint16_t*)(&f_robY))[0];
                                                _p->mb_mapping->tab_registers[ROB_Y_POS_FL_REG_ADD]=((uint16_t*)(&f_robY))[1];
                                            }
                                            else if(childobj.tagName()=="Z")
                                            {
                                                f_robZ=childobj.text().toFloat();
                                                _p->mb_mapping->tab_registers[ROB_Z_POS_FH_REG_ADD]=((uint16_t*)(&f_robZ))[0];
                                                _p->mb_mapping->tab_registers[ROB_Z_POS_FL_REG_ADD]=((uint16_t*)(&f_robZ))[1];
                                            }
                                            else if(childobj.tagName()=="RX")
                                            {
                                                f_robRX=childobj.text().toFloat();
                                                _p->mb_mapping->tab_registers[ROB_RX_POS_FH_REG_ADD]=((uint16_t*)(&f_robRX))[0];
                                                _p->mb_mapping->tab_registers[ROB_RX_POS_FL_REG_ADD]=((uint16_t*)(&f_robRX))[1];
                                            }
                                            else if(childobj.tagName()=="RY")
                                            {
                                                f_robRY=childobj.text().toFloat();
                                                _p->mb_mapping->tab_registers[ROB_RY_POS_FH_REG_ADD]=((uint16_t*)(&f_robRY))[0];
                                                _p->mb_mapping->tab_registers[ROB_RY_POS_FL_REG_ADD]=((uint16_t*)(&f_robRY))[1];
                                            }
                                            else if(childobj.tagName()=="RZ")
                                            {
                                                f_robRZ=childobj.text().toFloat();
                                                _p->mb_mapping->tab_registers[ROB_RZ_POS_FH_REG_ADD]=((uint16_t*)(&f_robRZ))[0];
                                                _p->mb_mapping->tab_registers[ROB_RZ_POS_FL_REG_ADD]=((uint16_t*)(&f_robRZ))[1];
                                            }
                                            else if(childobj.tagName()=="OUT1")
                                            {
                                                f_robout1=childobj.text().toInt();
                                                _p->mb_mapping->tab_registers[ROB_OUT1_POS_FH_REG_ADD]=((uint16_t*)(&f_robout1))[0];
                                                _p->mb_mapping->tab_registers[ROB_OUT1_POS_FL_REG_ADD]=((uint16_t*)(&f_robout1))[1];
                                            }
                                            else if(childobj.tagName()=="OUT2")
                                            {
                                                f_robout2=childobj.text().toInt();
                                                _p->mb_mapping->tab_registers[ROB_OUT2_POS_FH_REG_ADD]=((uint16_t*)(&f_robout2))[0];
                                                _p->mb_mapping->tab_registers[ROB_OUT2_POS_FL_REG_ADD]=((uint16_t*)(&f_robout2))[1];
                                            }
                                            else if(childobj.tagName()=="OUT3")
                                            {
                                                f_robout3=childobj.text().toInt();
                                                _p->mb_mapping->tab_registers[ROB_OUT3_POS_FH_REG_ADD]=((uint16_t*)(&f_robout3))[0];
                                                _p->mb_mapping->tab_registers[ROB_OUT3_POS_FL_REG_ADD]=((uint16_t*)(&f_robout3))[1];
                                            }
                                        }
                                    }
                                    else if(childobj.tagName()=="SPEED")
                                    {
                                        f_speed=childobj.text().toFloat();
                                        _p->mb_mapping->tab_registers[ROB_SPEED_FH_REG_ADD]=((uint16_t*)(&f_speed))[0];
                                        _p->mb_mapping->tab_registers[ROB_SPEED_FL_REG_ADD]=((uint16_t*)(&f_speed))[1];
                                    }
                                    else if(childobj.tagName()=="STATE")
                                    {
                                        state=childobj.text().toInt();
                                    //  _p->mb_mapping->tab_registers[ROB_STATE_REG_ADD]=state;
                                    }
                                    else if(childobj.tagName()=="IO")
                                    {
                                        auto child = childobj.childNodes();
                                        for(int i=0;i<child.size();i++)
                                        {
                                            auto childobj=child.at(i).toElement();
                                            if(childobj.tagName()=="IN1")
                                            {
                                                i_ioinput=childobj.text().toInt();
                                                _p->mb_mapping->tab_registers[ROB_IO_OUTPUT1_REG_ADD]=i_ioinput;
                                            }
                                            else if(childobj.tagName()=="IN2")
                                            {
                                                i_ioinput=childobj.text().toInt();
                                                _p->mb_mapping->tab_registers[ROB_IO_OUTPUT2_REG_ADD]=i_ioinput;
                                            }
                                            else if(childobj.tagName()=="IN3")
                                            {
                                                i_ioinput=childobj.text().toInt();
                                                _p->mb_mapping->tab_registers[ROB_IO_OUTPUT3_REG_ADD]=i_ioinput;
                                            }
                                            else if(childobj.tagName()=="IN4")
                                            {
                                                i_ioinput=childobj.text().toInt();
                                                _p->mb_mapping->tab_registers[ROB_IO_OUTPUT4_REG_ADD]=i_ioinput;
                                            }
                                            else if(childobj.tagName()=="IN5")
                                            {
                                                i_ioinput=childobj.text().toInt();
                                                _p->mb_mapping->tab_registers[ROB_IO_OUTPUT5_REG_ADD]=i_ioinput;
                                            }
                                            else if(childobj.tagName()=="IN6")
                                            {
                                                i_ioinput=childobj.text().toInt();
                                                _p->mb_mapping->tab_registers[ROB_IO_OUTPUT6_REG_ADD]=i_ioinput;
                                            }
                                            else if(childobj.tagName()=="IN7")
                                            {
                                                i_ioinput=childobj.text().toInt();
                                                _p->mb_mapping->tab_registers[ROB_IO_OUTPUT7_REG_ADD]=i_ioinput;
                                            }
                                            else if(childobj.tagName()=="IN8")
                                            {
                                                i_ioinput=childobj.text().toInt();
                                                _p->mb_mapping->tab_registers[ROB_IO_OUTPUT8_REG_ADD]=i_ioinput;
                                            }

                                            /*
                                            QString msg="IN"+QString::number(i+1);
                                            if(childobj.hasAttribute(msg))
                                            {
                                                i_ioinput=childobj.attributeNode(msg).value().toInt();
                                                _p->mb_mapping->tab_registers[ROB_IO_OUTPUT1_REG_ADD+i]=i_ioinput;
                                            }
                                            */
                                        }
                                    }
                                }
                            }
                            if(state==0)
                            {
                                mutexmovepoint_buffer_group.lock();
                                int num=_p->movepoint_buffer.size();
                                if(num!=0)
                                {
                                    Pause_PointInfo moveinfo=_p->movepoint_buffer[num-1];
                                    switch(moveinfo.movemod)
                                    {
                                        case MOVEL:
                                        case MOVEJ:
                                        {
                                            if(fabs(f_robX-moveinfo.robpos.X)>0.1
                                             ||fabs(f_robY-moveinfo.robpos.Y)>0.1
                                             ||fabs(f_robZ-moveinfo.robpos.Z)>0.1
                                             ||fabs(f_robRX-moveinfo.robpos.RX)>0.1
                                             ||fabs(f_robRY-moveinfo.robpos.RY)>0.1
                                             ||fabs(f_robRZ-moveinfo.robpos.RZ)>0.1
                                             ||abs(f_robout1-moveinfo.robpos.out_1)>1
                                             ||abs(f_robout2-moveinfo.robpos.out_2)>1
                                             ||abs(f_robout3-moveinfo.robpos.out_3)>1)
                                            {
                                                state=1;
                                            }
                                        }
                                        break;
                                        case MOVEC:
                                        {
                                            if(fabs(f_robX-moveinfo.robpos.X1)>0.1
                                             ||fabs(f_robY-moveinfo.robpos.Y1)>0.1
                                             ||fabs(f_robZ-moveinfo.robpos.Z1)>0.1
                                             ||fabs(f_robRX-moveinfo.robpos.RX1)>0.1
                                             ||fabs(f_robRY-moveinfo.robpos.RY1)>0.1
                                             ||fabs(f_robRZ-moveinfo.robpos.RZ1)>0.1
                                             ||abs(f_robout1-moveinfo.robpos.out1_1)>1
                                             ||abs(f_robout2-moveinfo.robpos.out1_2)>1
                                             ||abs(f_robout3-moveinfo.robpos.out1_3)>1)
                                            {
                                                state=1;
                                            }
                                        }
                                        break;
                                    }
                                }
                                mutexmovepoint_buffer_group.unlock();
                                _p->mb_mapping->tab_registers[ROB_STATE_REG_ADD]=state;
                            }
                        }
                    }
                }
                break;
                case ROBOT_MODEL_KAWASAKI://川崎机器人
                {
                    static int total_rcvnum=0;
                    static QString s_rcvmsg;
                    int rcvnum=_p->m_client.Recv((char*)_p->rcv_buf,ROBOT_KAWASAKI_INFO_RECVBUFFER_MAX*2);
                    if(rcvnum>0)
                    {
                        total_rcvnum=total_rcvnum+rcvnum;
                        QString rcvmsg=QString::fromLocal8Bit((char*)_p->rcv_buf,rcvnum);
                        s_rcvmsg=s_rcvmsg+rcvmsg;
                        if(rcvmsg[rcvmsg.size()-1]=='\n')//结尾
                        {
                            if(s_rcvmsg.size()>=2&&s_rcvmsg[s_rcvmsg.size()-2]=='\r')
                            {
                                bool b_succeed=true;
                                std::vector<float> pos;
                                QString msg=s_rcvmsg.left(s_rcvmsg.size()-2);
                                QStringList strList = msg.split(" ");
                                for(int i=0;i<strList.size();i++)
                                {
                                    QString s_data=strList[i];
                                    if(s_data.size()>0)
                                    {
                                        bool temp;
                                        float f_data=s_data.toFloat(&temp);
                                        if(temp==false)
                                        {
                                            b_succeed=false;
                                            break;
                                        }
                                        pos.push_back(f_data);
                                    }
                                }
                                if(pos.size()>=6&&b_succeed==true)//解码成功
                                {
                                    float f_data;
                                    f_data=pos[0];
                                    _p->mb_mapping->tab_registers[ROB_X_POS_FH_REG_ADD]=((uint16_t*)(&f_data))[0];
                                    _p->mb_mapping->tab_registers[ROB_X_POS_FL_REG_ADD]=((uint16_t*)(&f_data))[1];
                                    f_data=pos[1];
                                    _p->mb_mapping->tab_registers[ROB_Y_POS_FH_REG_ADD]=((uint16_t*)(&f_data))[0];
                                    _p->mb_mapping->tab_registers[ROB_Y_POS_FL_REG_ADD]=((uint16_t*)(&f_data))[1];
                                    f_data=pos[2];
                                    _p->mb_mapping->tab_registers[ROB_Z_POS_FH_REG_ADD]=((uint16_t*)(&f_data))[0];
                                    _p->mb_mapping->tab_registers[ROB_Z_POS_FL_REG_ADD]=((uint16_t*)(&f_data))[1];
                                    f_data=pos[3];
                                    _p->mb_mapping->tab_registers[ROB_RX_POS_FH_REG_ADD]=((uint16_t*)(&f_data))[0];
                                    _p->mb_mapping->tab_registers[ROB_RX_POS_FL_REG_ADD]=((uint16_t*)(&f_data))[1];
                                    f_data=pos[4];
                                    _p->mb_mapping->tab_registers[ROB_RY_POS_FH_REG_ADD]=((uint16_t*)(&f_data))[0];
                                    _p->mb_mapping->tab_registers[ROB_RY_POS_FL_REG_ADD]=((uint16_t*)(&f_data))[1];
                                    f_data=pos[5];
                                    _p->mb_mapping->tab_registers[ROB_RZ_POS_FH_REG_ADD]=((uint16_t*)(&f_data))[0];
                                    _p->mb_mapping->tab_registers[ROB_RZ_POS_FL_REG_ADD]=((uint16_t*)(&f_data))[1];
                                }
                                //有外部轴
                                if(pos.size()>6&&pos.size()<=9)
                                {
                                    int st=0;
                                    for(int i=6;i<pos.size();i++)
                                    {
                                        int i_data;
                                        i_data=pos[i]*1000;//川崎第六轴放大1000倍
                                        _p->mb_mapping->tab_registers[ROB_OUT1_POS_FH_REG_ADD+st]=((uint16_t*)(&i_data))[0];
                                        _p->mb_mapping->tab_registers[ROB_OUT1_POS_FL_REG_ADD+st+1]=((uint16_t*)(&i_data))[1];
                                        st=st+2;
                                    }
                                }
                            }
                            s_rcvmsg.clear();
                            total_rcvnum=0;
                        }
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
                    QString return_msg=QString::fromLocal8Bit("远端机器人发送命令失败,系统尝试重连机器人");
                    _p->mb_mapping->tab_registers[ROB_CONNECT_STATE_REG_ADD]=0;
                    _p->m_mcs->main_record.push_back(return_msg);
                    _p->b_relink=true;
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
            int rcvnum=0;
            switch(_p->rob_mod)
            {
                case ROBOT_MODEL_NULL://无机器人
                {

                }
                break;
                case ROBOT_MODEL_EMERGEN://智昌机器人
                {
                    rcvnum=_p->m_sendent.Recv((char*)_p->sendrcv_buf,ROBOT_EMERGEN_INFO_SENDRECVBUFFER_MAX*2);
                }
                break;
                case ROBOT_MODEL_DOBOT://越彊机器人
                {
                    rcvnum=_p->m_sendent.Recv((char*)_p->sendrcv_buf,ROBOT_DOBOT_INFO_SENDRECVBUFFER_MAX*2);
                }
                break;
                case ROBOT_MODEL_UR://优傲机器人
                {
                    rcvnum=_p->m_sendent.Recv((char*)_p->sendrcv_buf,ROBOT_UR_INFO_SENDRECVBUFFER_MAX*2);
                }
                break;
                case ROBOT_MODEL_KUKA://库卡机器人
                {
                    rcvnum=_p->m_sendent.Recv((char*)_p->sendrcv_buf,ROBOT_KUKA_INFO_SENDRECVBUFFER_MAX*2);
                }
                break;
                case ROBOT_MODEL_KAWASAKI://川崎机器人
                {
                    rcvnum=_p->m_sendent.Recv((char*)_p->sendrcv_buf,ROBOT_KAWASAKI_INFO_SENDRECVBUFFER_MAX*2);
                }
                break;
                case ROBOT_MODEL_YASKAWA://安川机器人
                {

                }
                break;
            }
            if(rcvnum>0)
            {
                main_record.lock();
                _p->sendrcv_buf[rcvnum]='\0';
                QString return_msg=QString::fromLocal8Bit((char*)_p->sendrcv_buf);
                _p->m_mcs->main_record.push_back(return_msg);
                main_record.unlock();
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
                    QString return_msg=QString::fromLocal8Bit("远端焊机发送命令失败,系统尝试重连机器人");
                    _p->mb_mapping->tab_registers[ROB_CONNECT_STATE_REG_ADD]=0;
                    _p->m_mcs->main_record.push_back(return_msg);
                    _p->b_relink=true;
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
            int rcvnum=0;
            switch(_p->rob_mod)
            {
                case ROBOT_MODEL_NULL://无机器人
                {

                }
                break;
                case ROBOT_MODEL_EMERGEN://智昌机器人
                {
                    rcvnum=_p->m_totalcontrolent.Recv((char*)_p->totalcontrolrcv_buf,ROBOT_EMERGEN_INFO_TOTALCONTROLCVBUFFER_MAX*2);
                }
                break;
                case ROBOT_MODEL_DOBOT://越彊机器人
                {
                    rcvnum=_p->m_totalcontrolent.Recv((char*)_p->totalcontrolrcv_buf,ROBOT_DOBOT_INFO_TOTALCONTROLCVBUFFER_MAX*2);
                }
                break;
                case ROBOT_MODEL_UR://优傲机器人
                {
                    rcvnum=_p->m_totalcontrolent.Recv((char*)_p->totalcontrolrcv_buf,ROBOT_UR_INFO_TOTALCONTROLCVBUFFER_MAX*2);
                }
                break;
                case ROBOT_MODEL_KUKA://库卡机器人
                {

                }
                break;
                case ROBOT_MODEL_KAWASAKI://川崎机器人
                {

                }
                break;
                case ROBOT_MODEL_YASKAWA://安川机器人
                {

                }
                break;
            }
            if(rcvnum>0)
            {
                main_record.lock();
                _p->totalcontrolrcv_buf[rcvnum]='\0';
                QString return_msg=(char*)_p->totalcontrolrcv_buf;
                _p->m_mcs->main_record.push_back(return_msg);
                main_record.unlock();
            }
        }
        else
        {
            _p->b_stop_totalcontrol_Thread=true;
            break;
        }
        sleep(0);
    }
}

void RobottotalcontrolrcvThread::Stop()
{
  if(_p->b_totalcontrolrcv_Thread==true)
  {
    _p->b_stop_totalcontrol_Thread=false;
    _p->b_totalcontrolrcv_Thread=false;
    while (_p->b_stop_totalcontrol_Thread==false)
    {
      sleep(0);
    }
  }
}


WeldsendThread::WeldsendThread(Robotcontrol *statci_p)//发送给焊机命令(非机器人直连时有效)
{
    _p=statci_p;
}

void WeldsendThread::run()
{
    while (1)
    {
        if(_p->b_weldsend_thread==true)
        {
            mutexweldsend_buf_group.lock();
            if(_p->weldsend_buf_group.size()>0)
            {
                std::string send_buf=_p->weldsend_buf_group[0];
                _p->weldsend_buf_group.erase(_p->weldsend_buf_group.begin());
                if(send_buf.size()!=_p->m_weldsendent.Send(send_buf.c_str(),send_buf.size()))
                {
                    main_record.lock();
                    QString return_msg=QString::fromLocal8Bit("远端焊机发送命令失败,系统尝试重连机器人");
                    _p->mb_mapping->tab_registers[ROB_CONNECT_STATE_REG_ADD]=0;
                    _p->m_mcs->main_record.push_back(return_msg);
                    _p->b_relink=true;
                    main_record.unlock();
                }
            #ifdef OPEN_SHOW_WELDSOCKDATA
                main_record.lock();
                QString return_msg=send_buf.c_str();
                _p->m_mcs->main_record.push_back("SendtoWeld:"+return_msg);
                main_record.unlock();
            #endif
            }
            mutexweldsend_buf_group.unlock();
        }
        else
        {
            _p->b_stop_weldsend_thread=true;
            break;
        }
        sleep(0);
    }
}

void WeldsendThread::Stop()
{
  if(_p->b_weldsend_thread==true)
  {
    _p->b_stop_weldsend_thread=false;
    _p->b_weldsend_thread=false;
    while (_p->b_stop_weldsend_thread==false)
    {
      sleep(0);
    }
  }
}

WeldsendrcvThread::WeldsendrcvThread(Robotcontrol *statci_p)
{
    _p=statci_p;
}

void WeldsendrcvThread::run()//获取焊机指令回复数据
{
    while (1)
    {
        if(_p->b_weldsendrcv_thread==true)
        {
            switch(_p->weld_mod)
            {
                case WELD_MODEL_NULL:                      //无焊机
                {
                //不做处理,无效
                }
                break;
                case WELD_MODEL_ROBOT_LINK:                //机器人直连
                {
                //不做处理，无效
                }
                break;
                /*
                case SOMEWELD       //非直连焊机类似如下接收线程
                {
                    int rcvnum=_p->m_weldsendent.Recv((char*)_p->weldsendrcv_buf,WELD_SOMEONE_INFO_SENDRECVBUFFER_MAX*2);
                    if(rcvnum>0)
                    {
                        main_record.lock();
                        _p->weldsendrcv_buf[rcvnum]='\0';
                        QString return_msg=(char*)_p->weldsendrcv_buf;
                        _p->m_mcs->main_record.push_back(return_msg);
                        main_record.unlock();
                    }
                }
                break;
                */
            }
        }
        else
        {
            _p->b_stop_weldsendrcv_thread=true;
            break;
        }
        sleep(0);
    }
}

void WeldsendrcvThread::Stop()
{
  if(_p->b_weldsendrcv_thread==true)
  {
    _p->b_stop_weldsendrcv_thread=false;
    _p->b_weldsendrcv_thread=false;
    while (_p->b_stop_weldsendrcv_thread==false)
    {
      sleep(0);
    }
  }
}

