#ifndef ROBOTCONTROL_H
#define ROBOTCONTROL_H
#include <modbus/modbus.h>
#include "global.h"
#include "my_parameters.h"
#include "XTcp.h"
#include <QMutex>
#include <QtXml>
#include <QDomDocument>
#include <QTcpServer>
#include <QTcpSocket>

#define ROBOTCONTROL_PAUSE_DIS     2000 //暂停恢复时判断点离发送点最近的距离大于这个值时，得重新往起点移动，否则继续往下移动

class my_parameters;

class RobotlinkThread;
class RobotcontrolThread1;
class RobotrcvThread;
class RobotsendThread;
class RobotsendrcvThread;
class RobottotalcontrolThread;
class RobottotalcontrolrcvThread;
class WeldsendThread;
class WeldsendrcvThread;

class MovRobPos
{
public:
    MovRobPos();
    float X;
    float Y;
    float Z;
    float RX;
    float RY;
    float RZ;
    int out_1;  //外部轴1
    int out_2;  //外部轴2
    int out_3;  //外部轴3

    float X1;
    float Y1;
    float Z1;
    float RX1;
    float RY1;
    float RZ1;
    int out1_1;
    int out1_2;
    int out1_3;

    bool nEn;
};

//移动点信息
class Pause_PointInfo
{
public:
    MovRobPos robpos;
    uint16_t tcp;
    Robmovemodel_ID movemod;
    float f_speed;
};

class Robotcontrol
{
public:
    static Robotcontrol *Get(my_parameters *mcs);

    my_parameters *m_mcs;

    ROBOT_MODEL rob_mod;     //机器人型号

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

    volatile bool b_startweld_init;   //刚起弧初始化变量标记
    volatile bool b_welding;        //焊接中

/******************************************/
//以下焊机接口
    WELD_MODEL weld_mod;     //焊机型号

    XTcp m_weldsendent;     //焊机命令数据sock(非机器人直连时有效)
    bool b_weldsendent;

    WeldsendThread *weldsend_Thread;
    bool b_weldsend_thread;
    bool b_stop_weldsend_thread;
    std::vector<std::string> weldsend_buf_group;    //发送焊机指令数据队列(非机器人直连时有效)

    WeldsendrcvThread *weldsendrcv_Thread;
    bool b_weldsendrcv_thread;
    bool b_stop_weldsendrcv_thread;
    uint8_t *weldsendrcv_buf;               //焊机接收发送数据回复信息(非机器人直连时有效)

    void WeldInit();    //焊机初始化(非机器人直连时有效)

    std::vector<Pause_PointInfo> movepoint_buffer;     //这个时间段内接收到的全部移动点
    std::vector<Pause_PointInfo> pause_movepoint_buffer;     //暂停期间的全部移动点
    int pause_movepointN;                    //当前暂停的位置下标
    void clear_movepoint_buffer();          //清空这个时间段内接收到的全部移动点
/*******************************************/



    std::array<double, 3> Yaskawa_RotMatrixXYZ2Euler(Eigen::Matrix3d rot_matrix);   //四元素转换为内旋 RX-RY-RZ
protected:

    Robotcontrol();
    ~Robotcontrol();
};

class RobotcontrolThread1 : public QThread      //查看上位机控制信息线程
{
    Q_OBJECT

public:
    RobotcontrolThread1(Robotcontrol *statci_p);

    void RobotMove(float f_movX,float f_movY,float f_movZ,float f_movRX, float f_movRY,float f_movRZ,
                   float f_movX1,float f_movY1,float f_movZ1,float f_movRX1, float f_movRY1,float f_movRZ1,
                   int i_out1,int i_out2,int i_out3,int i_out1_1,int i_out1_2,int i_out1_3,
                   Robmovemodel_ID movemod,uint16_t tcp,float f_speed);
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

class WeldsendThread : public QThread       //发送焊机数据线程（非机器人直连）
{
    Q_OBJECT

public:
    WeldsendThread(Robotcontrol *statci_p);
    void Stop();
protected:
    void run();
private:
    Robotcontrol *_p;

};

class WeldsendrcvThread : public QThread       //发送焊机数据后回复命令线程
{
    Q_OBJECT

public:
    WeldsendrcvThread(Robotcontrol *statci_p);
    void Stop();
protected:
    void run();
private:
    Robotcontrol *_p;

};




#endif // ROBOTCONTROL_H
