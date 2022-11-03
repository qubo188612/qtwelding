#ifndef MY_PARAMETERS_H
#define MY_PARAMETERS_H
#include <opencv2/opencv.hpp>
#include "cam_sen.h"
#include "soptopcamera.h"
#include "ResultData.h"
#include "E2proomData.h"
#include "project.h"
#include "ipaddress.h"
#include "robotdata.h"
#include "process.h"
#include "tosendbuffer.h"

class Process;
class toSendbuffer;

class my_parameters
{
public:
    static my_parameters *Get();

    Project *project;   //工程项目

    Cam_Sen *cam;    //相机

    IPaddress *ip;  //设备ip信息

    RobotData *rob; //机器人

    ResultData resultdata;		//激光头实时结果数据

    E2proomData e2proomdata;    //保存文件

    Process *process;   //流程

    toSendbuffer *tosendbuffer; //转以太网协议

    std::vector<QString> main_record;//主页显示列表信息

protected:
    my_parameters();
    ~my_parameters();
};

#endif // MY_PARAMETERS_H
