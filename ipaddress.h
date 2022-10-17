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

#define IPADDRESS_PATH_MOTO       "./SAVE/ip.json"

class IP                //ip
{
public:
    QString ip;         //ip地址
};

class IP_port      //ip和port
{
public:
    QString ip;         //ip地址
    int port;           //ip端口
};

class IPaddress
{
public:
    static IPaddress *Get();

    IP camer_ip[CAMTOTALNUM];        //相机ip
    IP_port robot_ip[ROBOTTALNUM];   //机器人ip

    int SaveIP(char* filename);    //保存项目

    int LoadIP(char* filename);    //读取项目

protected:
    IPaddress();
    ~IPaddress();

    QVariantHash encoed_json(); //把项目进行json编码

    int decoed_json(QByteArray allData);//把项目进行json解码
};

#endif // IPADDRESS_H
