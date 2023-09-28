#ifndef TCPROSINTERFACE_H
#define TCPROSINTERFACE_H

#include <string>
#include <vector>

class Stamp
{
public:
    int32_t sec;
    uint32_t nanosec;
};

class Header
{
public:
    Stamp stamp;
    std::string frame_id;
};

class Lasertrackoutcloud
{
public:
    float x;
    float y;
    int32_t u;
    int32_t v;
};

class Targetpointoutcloud
{
public:
    float x;
    float y;
    int32_t u;
    int32_t v;
    std::string name;
};

class Soptorobpos
{
public:
    Header header;
    float posx;
    float posy;
    float posz;
    float posrx;
    float posry;
    float posrz;
    float posout1;
    float posout2;
    float posout3;
    int toolid;
    int tcpid;
    int usertcpid;
};

class IFAlgorhmitcloud  //轮廓信息
{
public:
    Header header;
    std::vector<Lasertrackoutcloud> lasertrackoutcloud;
    std::vector<Targetpointoutcloud> targetpointoutcloud;
    Soptorobpos robpos;
    bool solderjoints;
};

#endif // TCPROSINTERFACE_H
