#ifndef MYCHECKIPINFO_H
#define MYCHECKIPINFO_H

#include <QNetworkInterface>
#include <QList>

class MyCheckIPinfo
{
public:
    QString getAllAdapterInfo();//获取网卡信息

    int split_mac(QString qmac,std::vector<uchar> *cmac_out);//提取网卡信息

    MyCheckIPinfo();
    ~MyCheckIPinfo();
};


#endif // MYCHECKIPINFO_H
