#ifndef CWELDTARJECT_H
#define CWELDTARJECT_H

#include "global.h"
#include "math.h"

class CWeldTarject
{
public:
    CWeldTarject();
    ~CWeldTarject();

    //机器人控制插值
    bool pos_interpolation(std::vector<RobPos> posw,                //输入插值前点
                           std::vector<RobPos> &interpolatPos,      //输出插值后点
                           int itp_time=16,                         //两个点之间的插值点的间隔运动时间:16ms为安川控制标准
                           float speed=1);                          //移动速度;

};

#endif // CWELDTARJECT_H
