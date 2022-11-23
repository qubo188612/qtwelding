#ifndef ROBOTCONTROL_H
#define ROBOTCONTROL_H
#include <modbus/modbus.h>
#include "global.h"
#include "my_parameters.h"
#include "XTcp.h"
#include <QMutex>


class my_parameters;

class RobotlinkThread;
class RobotcontrolThread1;
class RobotrcvThread;
class RobotsendThread;
class RobotsendrcvThread;
class RobottotalcontrolThread;
class RobottotalcontrolrcvThread;


class Robotcontrol
{
public:
    static Robotcontrol *Get(my_parameters *mcs);

    my_parameters *m_mcs;

    ROBOT_MODEL rob_mod;     //机器人型号
    WELD_MODEL weld_mod;     //焊机型号

    void Creat_control_modbus();        //启动服务器
    void Close_control_modbus();        //关闭服务器

    RobotcontrolThread1 *thread1;
    bool server_state;
    bool b_stop_server_state;
    modbus_t *ctx_robotcontrol;
    int sock;
    modbus_mapping_t * mb_mapping;

    RobotlinkThread *linkthread;
    bool link_state;
    bool b_stop_link_state;

    XTcp m_client;      //机器人信息数据sock
    bool b_client;

    XTcp m_sendent;     //机器人运动数据sock
    bool b_sendent;

    XTcp m_totalcontrolent;     //机器人总控数据sock
    bool b_totalcontrolent;

    RobotrcvThread *rcv_thread;
    bool b_rcv_thread;
    bool b_stop_rcv_thread;
    uint8_t *rcv_buf;   //机器人接受数据

    RobotsendThread *send_Thread;
    bool b_send_thread;
    bool b_stop_send_thread;
    std::vector<std::string> send_buf_group;    //发送机器人运动数据队列

    RobotsendrcvThread *sendrcv_Thread;
    bool b_sendrcv_thread;
    bool b_stop_sendrcv_thread;
    uint8_t *sendrcv_buf;   //机器人接受运动发送数据回复信息

    RobottotalcontrolThread *totalcontrol_Thread; //发送机器人总控指令（包括暂停等功能）
    bool b_totalcontrol_Thread;
    bool b_stop_totalcontrol_Thread;
    std::vector<std::string> totalcontrol_buf_group;    //发送机器人总控数据队列

    RobottotalcontrolrcvThread *totalcontrolrcv_Thread; //发送机器人总控数据回复信息
    bool b_totalcontrolrcv_Thread;
    bool b_stop_totalcontrolrcv_Thread;

    uint8_t *totalcontrolrcv_buf;   //机器人接受发送总控数据回复信息

    void RobotInit();   //机器人初始化

    void RobotOPEN_ELE(); //机器人上电
    void RobotCLOSE_ELE(); //机器人断电


protected:

    Robotcontrol();
    ~Robotcontrol();
};

class RobotcontrolThread1 : public QThread      //查看上位机控制信息线程
{
    Q_OBJECT

public:
    RobotcontrolThread1(Robotcontrol *statci_p);
protected:
    void run();
private:
    Robotcontrol *_p;

};

class RobotlinkThread : public QThread      //查看并连接机器人线程
{
    Q_OBJECT

public:
    RobotlinkThread(Robotcontrol *statci_p);
protected:
    void run();
private:
    Robotcontrol *_p;

};

class RobotrcvThread : public QThread       //接收机器人数据线程
{
    Q_OBJECT

public:
    RobotrcvThread(Robotcontrol *statci_p);
    void Stop();
protected:
    void run();
private:
    Robotcontrol *_p;

};

class RobotsendThread : public QThread       //发送机器人数据线程
{
    Q_OBJECT

public:
    RobotsendThread(Robotcontrol *statci_p);
    void Stop();
protected:
    void run();
private:
    Robotcontrol *_p;

};

class RobotsendrcvThread : public QThread       //发送机器人数据后回复命令线程
{
    Q_OBJECT

public:
    RobotsendrcvThread(Robotcontrol *statci_p);
    void Stop();
protected:
    void run();
private:
    Robotcontrol *_p;

};

class RobottotalcontrolThread : public QThread       //发送机器人数据总控命令线程
{
    Q_OBJECT

public:
    RobottotalcontrolThread(Robotcontrol *statci_p);
    void Stop();
protected:
    void run();
private:
    Robotcontrol *_p;

};

class RobottotalcontrolrcvThread : public QThread       //发送机器人数据总控回复命令线程
{
    Q_OBJECT

public:
    RobottotalcontrolrcvThread(Robotcontrol *statci_p);
    void Stop();
protected:
    void run();
private:
    Robotcontrol *_p;

};



#endif // ROBOTCONTROL_H
