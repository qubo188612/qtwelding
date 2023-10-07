#include "project.h"

Project *Project::Get()
{
    static Project fun;
    return &fun;
}

Project::Project()
{
#if _MSC_VER||WINDOWS_TCP
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
    if(0!=LoadProjectPath(PROJECT_PATH_MOTO))
    {
        project_path="NULL";
        project_Id=PROGECT_ID_TEACH_SCAN;
        project_name="Noname";
    }
    else
    {
        if(0!=LoadProject((char*)(project_path.toStdString().c_str())))
        {
            project_Id=PROGECT_ID_TEACH_SCAN;
            project_name="Noname";
        }
    }
}

Project::~Project()
{

}

int Project::SaveProject(QString filename)
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

    project_path=filename;

    SaveProjectPath(PROJECT_PATH_MOTO);

    return 0;
}

int Project::LoadProject(QString filename)
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

    project_path=filename;

    SaveProjectPath(PROJECT_PATH_MOTO);

    return 0;
}

int Project::SaveProjectPath(QString filename)
{
    QVariantHash data=encoedpath_json();

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

int Project::LoadProjectPath(QString filename)
{
    QFile loadFile(filename);

    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Unable to load JSON file";
        return 1;
    }

    QByteArray allData = loadFile.readAll();
    loadFile.close();

    if(0!=decoedpath_json(allData))
        return 1;

    return 0;
}

QVariantHash Project::encoed_json()
{
    QVariantHash data;
    QVariantHash subData1;
    QVariantHash subData2;
    QString p_id;
    switch(project_Id)
    {
        case PROGECT_ID_TEACH_SCAN:
            p_id="PROGECT_ID_TEACH_SCAN";
        break;
    }
    subData1.insert("project_Id", p_id);
    subData1.insert("project_name", project_name);
    for(int n=0;n<project_cmdlist.size();n++)
    {
        QString line="line"+QString::number(n);
        subData2.insert(line,project_cmdlist[n]);
    }
    subData1.insert("project_cmdlist", subData2);
    data.insert("Project", subData1);

    return data;
}

int Project::decoed_json(QByteArray allData)
{
    project_cmdlist.clear();
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
        if(keyString=="Project")//项目
        {
            QJsonObject obj = it->toObject();
            QJsonObject::Iterator it_obj;
            for(it_obj=obj.begin();it_obj!=obj.end();it_obj++)//遍历Key
            {
                QString keyString=it_obj.key();
                if(keyString=="project_Id")//项目类型
                {
                    QString msg=it_obj.value().toString();
                    if(msg=="PROGECT_ID_TEACH_SCAN")
                    {
                        project_Id=PROGECT_ID_TEACH_SCAN;
                    }
                    else
                    {

                    }
                }
                else if(keyString=="project_name")//项目名称
                {
                    project_name=it_obj.value().toString();
                }
                else if(keyString=="project_cmdlist")//项目指令集
                {
                    QJsonObject obj = it_obj->toObject();
                    project_cmdlist.reserve(obj.size());
                    for(int t=0;t<obj.size();t++)
                    {
                        QString s_key="line"+QString::number(t);
                        if(obj.contains(s_key))
                        {
                            QJsonValue value = obj.value(s_key);
                            QString msg=value.toString();
                            project_cmdlist.push_back(msg);
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
            }
        }
    }

    return 0;
}

QVariantHash Project::encoedpath_json()
{
    QVariantHash data;
    data.insert("project_path", project_path);
    return data;
}

int Project::decoedpath_json(QByteArray allData)
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
        if(keyString=="project_path")//项目路径
        {
            project_path=it.value().toString();
        }
    }

    return 0;
}


QJsonObject Project::QstringToJson(QString jsonString)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toLocal8Bit().data());
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}

QString Project::JsonToQstring(QJsonObject jsonObject)
{
    return QString(QJsonDocument(jsonObject).toJson());
}

QString Project::project_Id_toQString(Project_ID project_Id)
{
    QString msg;
    switch(project_Id)
    {
        case PROGECT_ID_TEACH_SCAN:
            msg=QStringLiteral("示教扫描类型");
        break;
    }
    return msg;
}
