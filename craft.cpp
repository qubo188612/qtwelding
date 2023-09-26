#include "craft.h"

Craft *Craft::Get()
{
    static Craft fun;
    return &fun;
}

Craft::Craft()
{
    craft_id=CRAFT_ID_FIXED_POSTURE;
    posture_distance=POSTURE_DISTANCE_USE;
}

Craft::~Craft()
{

}

int Craft::tidyup_posturelist(std::vector<ChangeRobPosVariable> posturelistIn,std::vector<ChangeRobPosVariable> &posturelistOut,QString &returnmsg)
{
    int pointnum=posturelistIn.size();
    if(pointnum<2)
    {
        returnmsg=QStringLiteral("姿态个数至少需要2个");
        return 1;
    }
    Eigen::Vector3d pointst(posturelistIn[0].posture.X,posturelistIn[0].posture.Y,posturelistIn[0].posture.Z),pointed(posturelistIn[pointnum-1].posture.X,posturelistIn[pointnum-1].posture.Y,posturelistIn[pointnum-1].posture.Z);
    Eigen::Vector3d pointvector=pointed-pointst;//直线向量
    double dis=pointvector.norm();//直线距离
    if(dis==0)
    {
        returnmsg=QStringLiteral("姿态起点与终点位置距离不能等于0");
        return 1;
    }
    std::vector<ChangeRobPosVariable> tempposture(posturelistIn.size());
    tempposture[0]=posturelistIn[0];
    tempposture[pointnum-1]=posturelistIn[pointnum-1];//把头尾姿态放入

    std::vector<d_Mysort> gropu;//向量大小集合
    for(int n=1;n<pointnum-1;n++)
    {
        Eigen::Vector3d pointcenter(posturelistIn[n].posture.X,posturelistIn[n].posture.Y,posturelistIn[n].posture.Z);
        Eigen::Vector3d pointcentervector=pointcenter-pointst;
        double result=pointvector.dot(pointcentervector);
        if(result<0)
        {
            returnmsg=QStringLiteral("中间姿态")+QString::number(n)+QStringLiteral("坐标没有位于起点和终点之间");
            return 1;
        }
        d_Mysort s_gropu;
        s_gropu.data=result;
        s_gropu.subscript=n;
        gropu.push_back(s_gropu);
    }
//这里排序
    sort(gropu.begin(),gropu.end(),d_Mysortcompare);
    for(int n=1;n<pointnum-1;n++)
    {
        tempposture[n]=posturelistIn[gropu[n-1].subscript];
    }
    posturelistOut=tempposture;
    return 0;
}

QString Craft::craft_Id_toQString(Craft_ID craft_id)
{
    QString msg;
    switch(craft_id)
    {
        case CRAFT_ID_FIXED_POSTURE:
            msg=QStringLiteral("固定焊接姿态");
        break;
        case CRAFT_ID_STARTENDCHANGE_POSTURE:
            msg=QStringLiteral("单一方向起终点变姿态");
        break;
        case CRAFT_ID_LASERNORMAL_POSTURE:
            msg=QStringLiteral("激光器测量法线姿态");
        break;
        case CRAFT_ID_CORRUGATED_POSTURE:
            msg=QStringLiteral("波纹板变姿态");
        break;
    }
    return msg;
}

int Craft::LoadCraft(QString filename)    //读取工艺
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

int Craft::SaveProject(QString filename)
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
        {
            p_id="CRAFT_ID_FIXED_POSTURE";            
        }
        break;
        case CRAFT_ID_STARTENDCHANGE_POSTURE:   //单一方向起终点变姿态
        {
            p_id="CRAFT_ID_STARTENDCHANGE_POSTURE";
        }
        break;
        case CRAFT_ID_LASERNORMAL_POSTURE: //激光器测量法线姿态
        {
            p_id="CRAFT_ID_LASERNORMAL_POSTURE";
        }
        break;
        case CRAFT_ID_CORRUGATED_POSTURE://波纹板变姿态
        {
            p_id="CRAFT_ID_CORRUGATED_POSTURE";
        }
        break;
    } 
    data.insert("craft_id", p_id);
    for(int n=0;n<posturelist.size();n++)
    {
        QJsonArray arrData;
        arrData.append(posturelist[n].posture.X);
        arrData.append(posturelist[n].posture.Y);
        arrData.append(posturelist[n].posture.Z);
        arrData.append(posturelist[n].posture.RX);
        arrData.append(posturelist[n].posture.RY);
        arrData.append(posturelist[n].posture.RZ);
        arrData.append(posturelist[n].Variable.X);
        arrData.append(posturelist[n].Variable.Y);
        arrData.append(posturelist[n].Variable.Z);
        QString line="lineposture"+QString::number(n);
        subData1.insert(line,arrData);
    }
    data.insert("posturelist", subData1);

    /****************************/
    //这里添加补充参数
    switch(craft_id)
    {
        case CRAFT_ID_FIXED_POSTURE:    //固定焊接姿态
        break;
        case CRAFT_ID_STARTENDCHANGE_POSTURE:   //单一方向起终点变姿态
        break;
        case CRAFT_ID_LASERNORMAL_POSTURE:     //激光器测量法线姿态
        break;
        case CRAFT_ID_CORRUGATED_POSTURE:   //波纹板变姿态
            data.insert("posture_distance",posture_distance);
            data.insert("weld_direction",weld_direction);
        break;
    }
    /****************************/
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
    if(rootObj.contains("craft_id"))//工艺类型
    {
        QJsonValue value = rootObj.value("craft_id");
        QString msg=value.toString();
        if(msg=="CRAFT_ID_FIXED_POSTURE")
        {
            craft_id=CRAFT_ID_FIXED_POSTURE;
        }
        else if(msg=="CRAFT_ID_STARTENDCHANGE_POSTURE")
        {
            craft_id=CRAFT_ID_STARTENDCHANGE_POSTURE;
        }
        else if(msg=="CRAFT_ID_LASERNORMAL_POSTURE")
        {
            craft_id=CRAFT_ID_LASERNORMAL_POSTURE;
        }
        else if(msg=="CRAFT_ID_CORRUGATED_POSTURE")
        {
            craft_id=CRAFT_ID_CORRUGATED_POSTURE;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }
    for(it=rootObj.begin();it!=rootObj.end();it++)//遍历Key
    {
        QString keyString=it.key();
        if(keyString=="posturelist")//姿态指令集
        {
            QJsonObject obj = it->toObject();
            posturelist.resize(obj.size());
            for(int t=0;t<obj.size();t++)
            {
                QString s_key="lineposture"+QString::number(t);
                if(obj.contains(s_key))
                {
                    QJsonValue value = obj.value(s_key);
                    QJsonArray arrData=value.toArray();
                    if(arrData.size()!=9)
                    {
                        return 1;
                    }
                    posturelist[t].posture.X=arrData[0].toDouble();
                    posturelist[t].posture.Y=arrData[1].toDouble();
                    posturelist[t].posture.Z=arrData[2].toDouble();
                    posturelist[t].posture.RX=arrData[3].toDouble();
                    posturelist[t].posture.RY=arrData[4].toDouble();
                    posturelist[t].posture.RZ=arrData[5].toDouble();
                    posturelist[t].Variable.X=arrData[6].toDouble();
                    posturelist[t].Variable.Y=arrData[7].toDouble();
                    posturelist[t].Variable.Z=arrData[8].toDouble();
                }
                else
                {
                    return 1;
                }
            }
        }
        //其他参数
        else if(keyString=="posture_distance")
        {
            posture_distance=it.value().toDouble();
        }
        else if(keyString=="weld_direction")
        {
            weld_direction=(Weld_direction)it.value().toInt();
        }
    }
    /*********************/
    //判断数据合理性,补充其他数据
    if(0!=posturelistcheck(craft_id,posturelist))
    {
        return 1;
    }
    return 0;
}

int Craft::posturelistcheck(Craft_ID craft_id,std::vector<ChangeRobPosVariable> posturelist)
{
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
        case CRAFT_ID_STARTENDCHANGE_POSTURE:   //起终点变姿态
        {
                if(posturelist.size()<2)    //起终点变姿态至少要有起终点
                {
                    return 1;
                }
        }
        break;
        case CRAFT_ID_LASERNORMAL_POSTURE:   //激光器测量法线姿态
        {
        }
        break;
        case CRAFT_ID_CORRUGATED_POSTURE: //波纹板变姿态
        {
                if(posturelist.size()!=3)    //分别是平坡姿态，上坡姿态，下坡姿态
                {
                    return 1;
                }
        }
        break;
    }
    return 0;
}
