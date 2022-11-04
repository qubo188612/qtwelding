#ifndef CRAFT_H
#define CRAFT_H

#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>
#include <QByteArray>

class Craft     //焊接工艺包
{
public:
    static Craft *Get();

    int LoadCraft(char* filename);    //读取工艺,返回值0:正确，返回值1:个路径问题，返回值2:工艺包格式问题

protected:

    int decoed_json(QByteArray allData);//把项目进行json解码

    Craft();
    ~Craft();
};

#endif // CRAFT_H
