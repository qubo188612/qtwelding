#ifndef CRAFT_H
#define CRAFT_H

#include <Eigen/Core>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>
#include <QByteArray>
#include "global.h"

#define CRAFT_ID_TOTAL_NUM       2      //工艺类型总数
typedef enum CRAFT_ID
{
    CRAFT_ID_FIXED_POSTURE=0,               //固定焊接姿态
    CRAFT_ID_STARTENDCHANGE_POSTURE=1,      //起终点变姿态
}Craft_ID;

#define PENDULUM_ID_TOTAL_NUM     1     //摆焊接模式总数
typedef enum PENDULUM_MODE_ID   //摆焊模式
{
    PENDULUM_ID_FLAT=0,     //平焊
}Pendulum_mode;

class Craft     //焊接工艺包
{
public:
    static Craft *Get();

    Craft_ID craft_id;      //工艺类型

    QString craft_path;       //最后一次工艺路径

    Pendulum_mode pendulum_mode;    //固定焊接姿态的摆焊接模式

    float pendulum_swing;   //固定焊接姿态的摆焊接幅度

    float pendulum_phaseangle;  //固定焊接姿态的摆焊相角

    QString craft_Id_toQString(Craft_ID craft_id);

    QString Pendulum_mode_toQString(Pendulum_mode pendulum_mode);

    int LoadCraft(char* filename);    //读取工艺,返回值0:正确，返回值1:个路径问题，返回值2:工艺包格式问题

    int SaveProject(char* filename);    //保存工艺

    std::vector<ChangeRobPosVariable> posturelist;        //焊接姿态
                                            //craft_id=CRAFT_ID_FIXED_POSTURE时，posturelist.size()=1,且posturelist[0]为这个轨迹的焊接姿态

    //整理posturelist姿态,把姿态坐标点沿着起点到终点的方向排序
    int tidyup_posturelist(std::vector<ChangeRobPosVariable> posturelistIn,           //输入姿态值
                           std::vector<ChangeRobPosVariable> &posturelistOut,         //输出整理后的姿态值
                           QString &returnmsg);                         //输出报警信息


protected:

    QVariantHash encoed_json(); //把项目进行json编码

    int decoed_json(QByteArray allData);//把项目进行json解码

    Craft();
    ~Craft();
};

#endif // CRAFT_H
