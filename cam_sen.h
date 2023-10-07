#ifndef CAM_SEN_H
#define CAM_SEN_H

#include "global.h"
#if _MSC_VER||WINDOWS_TCP
#include "soptocameratcpip.h"
#else
#include "soptopcamera.h"
#endif

class Cam_Sen
{
public:
    static Cam_Sen *Get();

/****************************/
//soptop相机
#if _MSC_VER||WINDOWS_TCP
    Soptocameratcpip sop_cam[CAMTOTALNUM];
#else
    SoptopCamera sop_cam[CAMTOTALNUM];
#endif

/****************************/
//其他相机

/****************************/
protected:
    Cam_Sen();
    ~Cam_Sen();
};

#endif // CAM_SEN_H
