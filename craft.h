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

#define STARTENDCHANGE_POSTURE_ALLOWERROR       100     //变姿态工艺与实际轨迹长度允许的误差(mm)

#define CRAFT_ID_TOTAL_NUM       4      //工艺类型总数
typedef enum CRAFT_ID
{
    CRAFT_ID_FIXED_POSTURE=0,               //固定焊接姿态
    CRAFT_ID_STARTENDCHANGE_POSTURE=1,      //单一方向起终点变姿态
    CRAFT_ID_LASERNORMAL_POSTURE=2,         //激光器测量法线姿态
    CRAFT_ID_CORRUGATED_POSTURE=3,          //波纹板变姿态
}Craft_ID;

#define PENDULUM_ID_TOTAL_NUM     6     //摆焊接模式总数
typedef enum PENDULUM_MODE_ID   //摆焊模式
{
    PENDULUM_ID_FLAT=0,     //平焊
    PENDULUM_ID_SIMPLE=1,   //单摆
    PENDULUM_ID_TRIANGLE=2, //三角摆
    PENDULUM_ID_L=3,        //L摆
    PENDULUM_ID_SINE=4,     //正弦摆
    PENDULUM_ID_CIRCULAR=5, //椭圆摆
}Pendulum_mode;

//摆焊参数
class wWAVEParam
{
public:
    double period;                  //周期(s)
    double leftAmp;                 //左摆副mm
    double rightAmp;                //右摆副mm
    double leftAmp_z;               //左摆副上下mm
    double rightAmp_z;              //右摆副上下mm
    double leftStopTime;            //左摆停留时间
    double rightStopTime;           //右摆停留时间
    double anglex;                  //摆幅与焊缝角度
    double angley;                  //摆幅与焊缝角度
    unsigned int startPos;          //起摆位置
    unsigned int  order;            //先往左摆还是右摆
    Pendulum_mode pendulum_mode;    //摆焊接模式
    wWAVEParam();
};

typedef enum WELD_DIRECTION_ID  //焊缝走向
{
    WELD_DIRECTION_X=0,     //大致X走向
    WELD_DIRECTION_Y=1,     //大致Y走向
}Weld_direction;

#define POSTURE_DISTANCE_USE    10  //默认变姿态距离

class Craft     //焊接工艺包
{
public:
    static Craft *Get();

    Craft_ID craft_id;      //工艺类型

    QString craft_path;       //最后一次工艺路径

    /**********************/
    //普遍工艺参数
    Pendulum_mode pendulum_mode;    //摆焊接模式

    float pendulum_swing;   //摆焊接幅度

    float pendulum_phaseangle;  //摆焊相角

    /**************************/
    //其它工艺参数
    float posture_distance; //变姿态距离

    Weld_direction weld_direction;//焊缝大致走向
    /*************************/

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
