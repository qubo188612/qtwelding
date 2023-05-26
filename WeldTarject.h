#ifndef WELDTARJECT_H
#define WELDTARJECT_H

#include <Eigen/Core>
#include <Eigen/Dense>
#include <vector>
#include <craft.h>

//摆焊参数
class wWAVEParam
{
public:
    double period;          //周期(s)
    double leftAmp;         //左摆副mm
    double rightAmp;        //右摆副mm
    double leftAmp_z;       //左摆副上下mm
    double rightAmp_z;      //右摆副上下mm
    double leftStopTime;    //左摆停留时间
    double rightStopTime;   //右摆停留时间
    double anglex;          //摆幅与焊缝角度
    double angley;          //摆幅与焊缝角度
    unsigned int startPos;  //
    unsigned int  order;
    Pendulum_mode type;     //摆动类型
    wWAVEParam();
};

#endif // WELDTARJECT_H
