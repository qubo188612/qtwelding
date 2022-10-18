#ifndef ROBOTDATA_H
#define ROBOTDATA_H

class RobPos
{
public:
    RobPos();

    float X;
    float Y;
    float Z;
    float RX;
    float RY;
    float RZ;
    bool nEn;
};

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
