#include "robotdata.h"

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
