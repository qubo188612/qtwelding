#include "project.h"

Project *Project::Get()
{
    static Project fun;
    return &fun;
}

Project::Project()
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

int Project::SaveProject(char* filename)
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

    project_path=QString::fromLocal8Bit(filename);

    SaveProjectPath(PROJECT_PATH_MOTO);

    return 0;
}

int Project::LoadProject(char* filename)
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

    project_path=QString::fromLocal8Bit(filename);

    SaveProjectPath(PROJECT_PATH_MOTO);

    return 0;
}

int Project::SaveProjectPath(char* filename)
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

int Project::LoadProjectPath(char* filename)
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
    QString p_id;
    switch(project_Id)
    {
        case PROGECT_ID_TEACH_SCAN:
            p_id="PROGECT_ID_TEACH_SCAN";
        break;
    }
    subData1.insert("project_Id", p_id);
    subData1.insert("project_name", project_name);
    data.insert("Project", subData1);

    return data;
}

int Project::decoed_json(QByteArray allData)
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
            msg=QString::fromLocal8Bit("示教扫描类型");
        break;
    }
    return msg;
}
