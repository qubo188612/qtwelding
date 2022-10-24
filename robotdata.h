#ifndef ROBOTDATA_H
#define ROBOTDATA_H
#include "global.h"

class RobotData
{
public:
    static RobotData *Get();
    RobPos TCPpos;      //机器人TCP针头坐标

protected:
    RobotData();
    ~RobotData();
};

#endif // ROBOTDATA_H
