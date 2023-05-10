#ifndef ROBOTDATA_H
#define ROBOTDATA_H
#include "global.h"
#include <modbus/modbus.h>
#include "calibration.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>
#include <QByteArray>
#if _MSC_VER
#include <QDir>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

#define ROBOTDATA_PATH_MOTO       "./SAVE/robdata.bsd"
/****************************************/
#define WELD_MODEL_NUM      2  //焊机型号支持的总数
typedef enum WELD_MODEL_ID            //焊机型号
{
    WELD_MODEL_NULL=0,                      //无焊机
    WELD_MODEL_ROBOT_LINK=1,                //机器人直连
}WELD_MODEL;
/**************************************/
#define ROBOT_MODEL_NUM     7  //机器人型号支持的总数
typedef enum ROBOT_MODEL_ID            //机器人型号
{
    ROBOT_MODEL_NULL=0,                //无机器人
    ROBOT_MODEL_EMERGEN=1,             //智昌机器人
    ROBOT_MODEL_DOBOT=2,               //越彊机器人
    ROBOT_MODEL_UR=3,                  //优傲机器人
    ROBOT_MODEL_KUKA=4,                //库卡机器人
    ROBOT_MODEL_KAWASAKI=5,            //川崎机器人
    ROBOT_MODEL_YASKAWA=6,             //安川机器人
}ROBOT_MODEL;

#define PLC_MODEL_NUM     2  //PLC型号（协议）型号支持的总数
typedef enum PLC_MODEL_ID            //PLC型号（协议）
{
    PLC_MODEL_NULL=0,              //无PLC
    PLC_MODEL_MODBUSTCP=1,         //Modbustcp协议
}PLC_MODEL;

#define ROBOT_DOBOT_TOTALCONTROL_RORT             29999    //越彊机器人总控指令数据信息发送端口
#define ROBOT_DOBOT_INFO_TOTALCONTROLCVBUFFER_MAX 1440     //越彊机器人运动指令发送数据回复信息最大个数
#define ROBOT_DOBOT_SEND_PORT                     30003    //越彊机器人运动指令数据信息发送端口
#define ROBOT_DOBOT_INFO_SENDRECVBUFFER_MAX       1440     //越彊机器人运动指令发送数据回复信息最大个数
#define ROBOT_DOBOT_INFO_PORT                     30004    //越彊机器人数据信息获取端口
#define ROBOT_DOBOT_INFO_RECVBUFFER_MAX           1440     //越彊机器人数据信息最大个数

#define ROBOT_UR_TOTALCONTROL_RORT                29999    //优傲机器人总控指令数据信息发送端口
#define ROBOT_UR_INFO_TOTALCONTROLCVBUFFER_MAX    1108     //优傲机器人运动指令发送数据回复信息最大个数
#define ROBOT_UR_SEND_PORT                        30001    //优傲机器人运动指令数据信息发送端口
#define ROBOT_UR_INFO_SENDRECVBUFFER_MAX          1108     //优傲机器人运动指令发送数据回复信息最大个数
#define ROBOT_UR_INFO_PORT                        30003    //优傲机器人数据信息获取端口
#define ROBOT_UR_INFO_RECVBUFFER_MAX              1108     //优傲机器人数据信息最大个数

#define ROBOT_KUKA_SEND_PORT                      30001    //库卡机器人运动指令数据信息发送端口
#define ROBOT_KUKA_INFO_SENDRECVBUFFER_MAX        1440     //库卡机器人运动指令发送数据回复信息最大个数
#define ROBOT_KUKA_INFO_PORT                      30003    //库卡机器人信息端口
#define ROBOT_KUKA_INFO_RECVBUFFER_MAX            1440     //库卡机器人数据信息最大个数

#define ROBOT_KAWASAKI_SEND_PORT                  23       //川崎机器人运动指令数据信息发送端口
#define ROBOT_KAWASAKI_INFO_SENDRECVBUFFER_MAX    1440     //川崎机器人运动指令发送数据回复信息最大个数
#define ROBOT_KAWASAKI_INFO_PORT                  23       //川崎机器人信息端口
#define ROBOT_KAWASAKI_INFO_RECVBUFFER_MAX        1440     //川崎机器人数据信息最大个数

#define ROBOT_EMERGEN_SEND_PORT                   30008    //智昌机器人运动指令端口数据信息发送端口
#define ROBOT_EMERGEN_INFO_SENDRECVBUFFER_MAX     1440     //智昌机器人运动指令发送数据回复信息最大个数
#define ROBOT_EMERGEN_INFO_PORT                   30008    //智昌机器人信息端口
#define ROBOT_EMERGEN_INFO_RECVBUFFER_MAX         1440     //智昌机器人数据信息端口

/***************************************/

typedef enum ROBOT_STATE_ID            //机器人状态
{
    ROBOT_STATE_UNLINK=-1,              //未连接
    ROBOT_STATE_IDLE=0,                 //空闲
    ROBOT_STATE_MOVE=1,                 //忙碌
    ROBOT_STATE_STOP=2,                 //急停
}ROBOT_STATE;

class sent_info_robot             //发送机器人数据
{
public:
    modbus_t *ctx;                  //sock
    int addr;                       //发送寄存器
    std::vector<uint16_t> data;     //发送数据
};

class RobotData
{
public:
    static RobotData *Get();

    RobPos TCPpos;      //机器人当前TCP坐标

    systime robtime;    //机器人时间

    std::vector<int> robioinput;    //机器人输入口IO  

    ROBOT_MODEL robot_model;    //机器人型号
    QString robot_model_toQString(ROBOT_MODEL robot_model);  //机器人型号字符串输出

    ROBOT_STATE robot_state;    //机器人状态
    QString robot_state_toQString(ROBOT_STATE robot_state);  //机器人状态字符串输出

    CAL_POSTURE cal_posture_model;  //机器人姿态标准

    int out_num;//机器人外部轴数量

    float robot_speed;         //机器人当前移动速度

    bool b_connect;             //是否连接机器人

    int ConnectRobot(QString ip,int port);         //连接机器人

    void DisconnectRobot();

    modbus_t *ctx_posget;       //机器人信息模块访问
    bool b_link_ctx_posget;     //机器人信息模块访问连接

    std::vector<sent_info_robot> send_group_robot;    //发送机器人数据队列
    volatile bool b_send_group_robot;       //发送机器人数据是否异常
    int ctx_robot_dosomeing;    //机器人端口忙


    WELD_MODEL weld_model;    //焊机型号
    QString weld_model_toQString(WELD_MODEL weld_model);  //焊机型号字符串输出
    float weld_eled;    //焊接电流
    Alternatingcurrent weld_elem;   //焊接工艺
    QString weldwork_elem_toQString(Alternatingcurrent weld_elem);//焊接工艺字符串输出
    Weldworkmodel weld_state;   //焊接状态
    QString weldwork_model_toQString(Weldworkmodel weld_model);//焊接状态字符串输出

    PLC_MODEL plc_model;    //PLC型号(协议)
    QString plc_model_toQString(PLC_MODEL plc_model);   //PLC型号字符串输出
    modbus_t *ctx_plc;      //PLCmodbus链接
    bool b_link_ctx_plc;    //PLCmodbus链接状态
    int ConnectPLC(QString ip,int port);         //连接PLC
    void DisconnectPLC();
    bool b_connect_plc;     //是否连接PLC
    /*
    int ctx_plc_dosomeing;    //plc端口忙
    std::vector<sent_info_robot> send_group_plc;    //发送PLC数据队列
    volatile bool b_send_group_plc;       //发送plc数据是否异常
    */

    int SaveRob(char* filename);    //保存项目

protected:
    RobotData();
    ~RobotData();

    int LoadRob(char* filename);    //读取项目

    QVariantHash encoed_json(); //把项目进行json编码

    int decoed_json(QByteArray allData);//把项目进行json解码
};



#endif // ROBOTDATA_H
