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
#include "craft.h"
#include "synchronous.h"
#include "robotcontrol.h"
#include "SN_Data.h"


class Robotcontrol;
class Process;
class toSendbuffer;
class Synchronous;

class my_parameters
{
public:
    static my_parameters *Get();

    Project *project;   //工程项目

    Craft *craft;   //焊接工艺包

    Cam_Sen *cam;    //相机

    IPaddress *ip;  //设备ip信息

    RobotData *rob; //机器人

    ResultData resultdata;		//激光头实时结果数据

    E2proomData e2proomdata;    //保存文件

    Process *process;   //流程

    toSendbuffer *tosendbuffer; //转以太网协议

    std::vector<QString> main_record;//主页显示列表信息

    MainDlg_robmovestate mainDlg_robmovestate;  //主页机器人移动状态

    bool mainDlg_robotEnable;//主页机器人使能

    Synchronous *synchronous;    //手眼转换计算

    Robotcontrol *robotcontrol;     //自带机器人协议

    SN_Data sn_data;    //序列号信息

protected:
    my_parameters();
    ~my_parameters();
};

#endif // MY_PARAMETERS_H
