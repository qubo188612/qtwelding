#ifndef IPADDRESS_H
#define IPADDRESS_H

#include "global.h"
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

#define IPADDRESS_PATH_MOTO       "./SAVE/ip.bsd"

class RobotIP
{
public:
    IP_port robot_ip;       //机器人服务器IP
    IP remote_ip;           //机器人远程IP
    IP weld_ip;             //焊机远程IP
    IP_port plc_ip;         //PLC远程IP
};

class IPaddress
{
public:
    static IPaddress *Get();

    IP camer_ip[CAMTOTALNUM];           //相机ip
    RobotIP robot_ip[ROBOTTALNUM];      //机器人ip和端口

    int robotmyselfcontrol_port[ROBOTTALNUM]; //自带的机器人控制端口

    int SaveIP(char* filename);    //保存项目

    int LoadIP(char* filename);    //读取项目

protected:
    IPaddress();
    ~IPaddress();

    QVariantHash encoed_json(); //把项目进行json编码

    int decoed_json(QByteArray allData);//把项目进行json解码
};

#endif // IPADDRESS_H
