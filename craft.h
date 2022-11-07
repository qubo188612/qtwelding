#ifndef CRAFT_H
#define CRAFT_H

#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>
#include <QByteArray>

#define CRAFT_ID_TOTAL_NUM       1      //工艺类型总数
typedef enum CRAFT_ID
{
    CRAFT_ID_FIXED_POSTURE=0,       //固定焊接姿态
}Craft_ID;

class Craft     //焊接工艺包
{
public:
    static Craft *Get();

    Craft_ID craft_id;      //工艺类型

    QString craft_name;       //工艺名

    QString craft_Id_toQString(Craft_ID craft_id);

    int LoadCraft(char* filename);    //读取工艺,返回值0:正确，返回值1:个路径问题，返回值2:工艺包格式问题

protected:

    int decoed_json(QByteArray allData);//把项目进行json解码

    Craft();
    ~Craft();
};

#endif // CRAFT_H
