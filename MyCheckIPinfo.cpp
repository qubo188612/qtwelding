#include "MyCheckIPinfo.h"

MyCheckIPinfo::MyCheckIPinfo()
{

}

MyCheckIPinfo::~MyCheckIPinfo()
{

}

QString MyCheckIPinfo::getAllAdapterInfo()
{
    QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// 获取所有网络接口列表
    int nCnt = nets.count();
    QString strMacAddr = "";
    bool b_find=false;
    for(int i = 0; i < nCnt; i ++)
    {
        // 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址
        /*if(nets[i].flags().testFlag(QNetworkInterface::IsUp)      //网线插入
              && nets[i].flags().testFlag(QNetworkInterface::IsRunning)     //网络是否工作
                && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))    //是否是回环
        */
        if(nets[i].type()==QNetworkInterface::Ethernet&&    //有线网络接口
            !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            // 该mac的ip地址不能是回环地址并且是ipv4地址
            for(int j = 0; j < nets[i].addressEntries().size(); j++)
            {
                if(nets[i].addressEntries().at(j).ip() != QHostAddress::LocalHost &&
                   nets[i].addressEntries().at(j).ip().protocol() == QAbstractSocket::IPv4Protocol)
                {
                    strMacAddr = nets[i].hardwareAddress();
                    b_find=true;
                    break;
                }
            }
        }
        if(b_find==true)
        {
            break;
        }
    }
    return strMacAddr;
}

int MyCheckIPinfo::split_mac(QString qmac,std::vector<uchar> *cmac_out)
{
    std::vector<uchar> mac(6);
    QStringList strList = qmac.split(":");
    if(strList.size()!=6)
    {
        return 1;
    }
    for(int i=0;i<6;i++)
    {
        QByteArray array;
        array.append(strList[i].toUtf8());
        mac[i]=array.toUInt(Q_NULLPTR,16);
    }
    *cmac_out=mac;
    return 0;
}


