#include "ipaddress.h"

IPaddress *IPaddress::Get()
{
    static IPaddress fun;
    return &fun;
}

IPaddress::IPaddress()
{
#if _MSC_VER
    QString filePath =  "./SAVE";
    QDir dir(filePath);
    if(!dir.exists())
        QDir().mkdir(filePath);
#else
    std::string dir = "./SAVE";
    if (access(dir.c_str(), 0) == -1)
    {
      mkdir("./SAVE",S_IRWXU);
    }
#endif
    if(0!=LoadIP(IPADDRESS_PATH_MOTO))
    {
        camer_ip[0].ip="192.168.1.2";
        robot_ip[0].ip="192.168.1.3";
        robot_ip[0].port=1600;
    }
}

IPaddress::~IPaddress()
{
    SaveIP(IPADDRESS_PATH_MOTO);
}

int IPaddress::SaveIP(char* filename)
{
    QVariantHash data=encoed_json();

    QJsonObject rootObj = QJsonObject::fromVariantHash(data);
    QJsonDocument document;
    document.setObject(rootObj);

    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    QString json_str(byte_array);
    //根据实际填写路径
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "file error!";
        return 1;
    }
    QTextStream in(&file);
    in << json_str;

    file.close();   // 关闭file

    return 0;
}

int IPaddress::LoadIP(char* filename)
{
    QFile loadFile(filename);

    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Unable to load JSON file";
        return 1;
    }

    QByteArray allData = loadFile.readAll();
    loadFile.close();

    if(0!=decoed_json(allData))
        return 1;

    return 0;
}

QVariantHash IPaddress::encoed_json()
{
    QVariantHash data;
    QVariantHash subData1;
    QString p_id;

    data.insert("camer_ip", camer_ip[0].ip);
    subData1.insert("robot_ip", robot_ip[0].ip);
    subData1.insert("robot_port", robot_ip[0].port);
    data.insert("robot", subData1);

    return data;
}

int IPaddress::decoed_json(QByteArray allData)
{
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

    if (json_error.error != QJsonParseError::NoError)
    {
        qDebug() << "JSON error!";
        return 1;
    }

    QJsonObject rootObj = jsonDoc.object();
    QJsonObject::Iterator it;
    for(it=rootObj.begin();it!=rootObj.end();it++)//遍历Key
    {
        QString keyString=it.key();
        if(keyString=="robot")//机器人
        {
            QJsonObject obj = it->toObject();
            QJsonObject::Iterator it_obj;
            for(it_obj=obj.begin();it_obj!=obj.end();it_obj++)//遍历Key
            {
                QString keyString=it_obj.key();
                if(keyString=="robot_ip")//机器人ip
                {
                    robot_ip[0].ip=it_obj.value().toString();
                }
                else if(keyString=="robot_port")//机器人端口
                {
                    robot_ip[0].port=it_obj.value().toInt();
                }
            }
        }
        else if(keyString=="camer_ip")//相机
        {
            camer_ip[0].ip=it.value().toString();
        }
    }

    return 0;
}

