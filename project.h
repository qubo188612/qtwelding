#ifndef PROJECT_H
#define PROJECT_H

#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>
#include <QByteArray>


typedef enum PROJECT_ID
{
    PROGECT_ID_TEACH_SCAN=0,       //示教扫描类型
}Project_ID;

class Project
{
public:
    static Project *Get();

    Project_ID project_Id;      //项目总类型id

    QString project_name;       //项目名

    int SaveProject(char* filename);    //保存项目

    int LoadProject(char* filename);    //读取项目

protected:
    Project();
    ~Project();

    QVariantHash encoed_json(); //把项目进行json编码

    int decoed_json(QByteArray allData);//把项目进行json解码

    QString JsonToQstring(QJsonObject jsonObject);

    QJsonObject QstringToJson(QString jsonString);
};

#endif // PROJECT_H
