#ifndef ROBOTCONTROL_H
#define ROBOTCONTROL_H
#include <modbus/modbus.h>
#include "global.h"
#include "my_parameters.h"
#include "XTcp.h"


class my_parameters;

class RobotlinkThread;
class RobotcontrolThread1;
class RobotrcvThread;
class RobotsendThread;

class Robotcontrol
{
public:
    static Robotcontrol *Get(my_parameters *mcs);

    my_parameters *m_mcs;

    ROBOT_MODEL_ID rob_mod;     //机器人型号

    void Creat_control_modbus();        //启动服务器

    RobotcontrolThread1 *thread1;
    bool server_state;
    modbus_t *ctx_robotcontrol;
    int sock;
    modbus_mapping_t * mb_mapping;
    uint16_t mod_registers[ROB_CONTROL_REG_TOTALNUM];

    RobotlinkThread *linkthread;
    bool link_state;

    XTcp m_client;      //机器人接收数据sock
    bool b_client;

    XTcp m_sendent;     //机器人发送数据sock
    bool b_sendent;

    RobotrcvThread *rcv_thread;
    bool b_rcv_thread;
    bool b_stop_rcv_thread;
    uint8_t *rcv_buf;   //机器人接受数据

    RobotsendThread *send_Thread;
    bool b_send_thread;
    bool b_stop_send_thread;
    std::vector<std::string> send_buf_group;    //发送机器人数据队列


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

#endif // ROBOTCONTROL_H
