#include "robotdata.h"

RobPos::RobPos()
{
    X=0;
    Y=0;
    Z=0;
    RX=0;
    RY=0;
    RZ=0;
    nEn=0;
}

RobotData *RobotData::Get()
{
    static RobotData fun;
    return &fun;
}

RobotData::RobotData()
{

}

RobotData::~RobotData()
{

}
