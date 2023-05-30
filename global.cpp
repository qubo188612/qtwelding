#include "global.h"

RobPos::RobPos()
{
    X=0;
    Y=0;
    Z=0;
    RX=0;
    RY=0;
    RZ=0;
    out_1=0;
    out_2=0;
    out_3=0;
    nEn=0;
}

leaser_pos::leaser_pos()
{
    Y=0;
    Z=0;
    nEn=0;
}

Pos3f::Pos3f()
{
    X=0;
    Y=0;
    Z=0;
}

bool d_Mysortcompare(d_Mysort &a,d_Mysort &b)
{
    return a.data<b.data;
}

uint64_t htonll(uint64_t val)
{
    return (((uint64_t) htonl(val)) << 32) + htonl(val >> 32);
}

uint64_t ntohll(uint64_t val)
{
    return (((uint64_t) ntohl(val)) << 32) + ntohl(val >> 32);
}

Point_robpos_result::Point_robpos_result()
{
    nEn=false;
}

Coord_Matrix4d_result::Coord_Matrix4d_result()
{
    nEn=false;
}

wWAVEParam::wWAVEParam()
{
    period=0;
    leftAmp=0;
    rightAmp=0;
    leftAmp_z=0;
    rightAmp_z=0;
    leftStopTime=0;
    rightStopTime=0;
    anglex=0;
    angley=0;
    startPos=0;
    order=0;
    pendulum_mode=PENDULUM_ID_SIMPLE;
    timeGap=0;
    trend_mode=TREND_MODE_X_Z;
}

QString Pendulum_mode_toQString(Pendulum_mode pendulum_mode)
{
    QString msg;
    switch(pendulum_mode)
    {
        case PENDULUM_ID_SIMPLE:
            msg=QString::fromLocal8Bit("单摆");
        break;
        case PENDULUM_ID_TRIANGLE:
            msg=QString::fromLocal8Bit("三角摆");
        break;
        case PENDULUM_ID_L:
            msg=QString::fromLocal8Bit("L摆");
        break;
        case PENDULUM_ID_SINE:
            msg=QString::fromLocal8Bit("正弦摆");
        break;
        case PENDULUM_ID_CIRCULAR:
            msg=QString::fromLocal8Bit("椭圆摆");
        break;
    }
    return msg;
}

QString Trend_mode_toQString(Trend_mode trend_mode)
{
    QString msg;
    switch(trend_mode)
    {
        case TREND_MODE_X_Z:
            msg=QString::fromLocal8Bit("X轴走向,开口朝Z轴正方向");
        break;
        case TREND_MODE_Y_Z:
            msg=QString::fromLocal8Bit("Y轴走向,开口朝Z轴正方向");
        break;
        case TREND_MODE_Z_Y:
            msg=QString::fromLocal8Bit("Z轴走向,开口朝Y轴负方向");
        break;
    }
    return msg;
}
