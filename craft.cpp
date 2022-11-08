#include "craft.h"

Craft *Craft::Get()
{
    static Craft fun;
    return &fun;
}

Craft::Craft()
{
    craft_id=CRAFT_ID_FIXED_POSTURE;
    pendulum_mode=PENDULUM_ID_FLAT;
    pendulum_swing=0;
    pendulum_phaseangle=0;
}

Craft::~Craft()
{

}

QString Craft::craft_Id_toQString(Craft_ID craft_id)
{
    QString msg;
    switch(craft_id)
    {
        case CRAFT_ID_FIXED_POSTURE:
            msg=QString::fromLocal8Bit("固定焊接姿态");
        break;
    }
    return msg;
}

QString Craft::Pendulum_mode_toQString(Pendulum_mode pendulum_mode)
{
    QString msg;
    switch(pendulum_mode)
    {
        case PENDULUM_ID_FLAT:
            msg=QString::fromLocal8Bit("平焊");
        break;
    }
    return msg;
}

int Craft::LoadCraft(char* filename)    //读取工艺
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
        return 2;

    return 0;
}

int Craft::SaveProject(char* filename)
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

QVariantHash Craft::encoed_json()
{
    QVariantHash data;
    QVariantHash subData1;

    QString p_id;
    switch(craft_id)
    {
        case CRAFT_ID_FIXED_POSTURE:    //固定焊接姿态
            p_id="CRAFT_ID_FIXED_POSTURE";
        break;
    }
    data.insert("craft_id", p_id);
    for(int n=0;n<posturelist.size();n++)
    {
        QJsonArray arrData;
        arrData.append(posturelist[n].X);
        arrData.append(posturelist[n].Y);
        arrData.append(posturelist[n].Z);
        arrData.append(posturelist[n].RX);
        arrData.append(posturelist[n].RY);
        arrData.append(posturelist[n].RZ);
        QString line="lineposture"+QString::number(n);
        subData1.insert(line,arrData);
    }
    data.insert("posturelist", subData1);
    switch(pendulum_mode)
    {
        case PENDULUM_ID_FLAT:    //固定焊接姿态
            p_id="PENDULUM_ID_FLAT";
        break;
    }
    data.insert("pendulum_mode", p_id);
    data.insert("pendulum_swing",pendulum_swing);
    data.insert("pendulum_phaseangle",pendulum_phaseangle);

    return data;
}

int Craft::decoed_json(QByteArray allData)
{
    //这里添加解码
    posturelist.clear();
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
        if(keyString=="craft_id")//工艺类型
        {
            QString msg=it.value().toString();
            if(msg=="CRAFT_ID_FIXED_POSTURE")
            {
                craft_id=CRAFT_ID_FIXED_POSTURE;
            }
            else
            {

            }
        }
        else if(keyString=="posturelist")//项目指令集
        {
            QJsonObject obj = it->toObject();
            QJsonObject::Iterator it;
            posturelist.resize(obj.size());
            for(int t=0;t<obj.size();t++)
            {
                QString s_key="lineposture"+QString::number(t);
                for(it=obj.begin();it!=obj.end();it++)//遍历Key
                {
                    QString keyString=it.key();
                    if(keyString==s_key)
                    {
                        QJsonArray arrData=it.value().toArray();
                        if(arrData.size()!=6)
                        {
                            return 1;
                        }
                        posturelist[t].X=arrData[0].toDouble();
                        posturelist[t].Y=arrData[1].toDouble();
                        posturelist[t].Z=arrData[2].toDouble();
                        posturelist[t].RX=arrData[3].toDouble();
                        posturelist[t].RY=arrData[4].toDouble();
                        posturelist[t].RZ=arrData[5].toDouble();
                    }
                }
            }
        }
        else if(keyString=="pendulum_mode")//工艺类型
        {
            QString msg=it.value().toString();
            if(msg=="PENDULUM_ID_FLAT")
            {
                pendulum_mode=PENDULUM_ID_FLAT;
            }
            else
            {

            }
        }
        else if(keyString=="pendulum_swing")
        {
            pendulum_swing=it.value().toDouble();
        }
        else if(keyString=="pendulum_phaseangle")
        {
            pendulum_phaseangle=it.value().toDouble();
        }
    }
    /*********************/
    //判断数据合理性
    switch(craft_id)
    {
        case CRAFT_ID_FIXED_POSTURE:    //固定焊接姿态
        {
            if(posturelist.size()!=1)    //固定姿态要只有一个姿态
            {
                return 1;
            }
        }
        break;
    }

    return 0;
}
