#ifndef MY_CMD_H
#define MY_CMD_H

#include <QString>
#include <QStringList>
#include <global.h>

/************************/
//项目0:命令集合
//移动指令，举例 MOV: SPEED[25] MOVL[1.3,32.7,45,66,7,89,3] TCP[1]
//延时指令，举例 DELAY: TIME[1000]
//激光指令，举例 CAM: TASK[102] WORK[1]
//激光指令，举例 CAM: WORK[0]
//焊机指令，举例 WELD: WORK[1] ELED[1.23] ELEM[0]
//焊机指令，举例 WELD: WORK[0]
//采集指令，举例 SCAN: MOVL[1.3,32.7,45,66,7,89,3] SPEED[25] TCP[0]
//跟踪指令，举例 TRACE: ROUTE[0] SPEED[25] TCP[0]
//key项
#define CMD_MOV_KEY                     "MOV:"          //移动命令集合KEY
#define CMD_DELAY_KEY                   "DELAY:"        //延时命令集合KEY
#define CMD_CAM_KEY                     "CAM:"          //激光命令集合KEY
#define CMD_WELD_KEY                    "WELD:"         //焊机命令集合KEY
#define CMD_SCAN_KEY                    "SCAN:"         //采集命令集合KEY
#define CMD_TRACE_KEY                   "TRACE:"        //跟踪命令集合KEY


//参数项
#define CMD_MOVL                            "MOVL"      //直线移动
#define CMD_MOVJ                            "MOVJ"      //关节移动
#define CMD_TCP                             "TCP"       //TCP
#define CMD_SPEED                           "SPEED"     //速度
#define CMD_TIME                            "TIME"      //时间         单位ms
#define CMD_TASK                            "TASK"      //任务号
#define CMD_WORK                            "WORK"      //是否启动      0:停止 1:启动
#define CMD_ELED                            "ELED"      //电流         单位A
#define CMD_ELEM                            "ELEM"      //交变电        0:直流 1:交流
#define CMD_ROUTE                           "ROUTE"     //轨迹序号



/************************/
class my_cmd
{
public:
    my_cmd();

    static QString cmd_move(RobPos pos,Robmovemodel movemodel,float speed,int tcp);//移动命令
    static QString cmd_delay(int time);//延时命令
    static QString cmd_cam(int task,int work);//相机启停命令不
    static QString cmd_cam_work(int work);//相机启停命令
    static QString cmd_elec(float eled,int elem,int work);//焊机启停命令不
    static QString cmd_elec_work(int work);//焊机启停命令
    static QString cmd_scan(RobPos pos,float speed,int tcp);//采集命令
    static QString cmd_trace(int route,float speed,int tcp);//跟踪命令


    static int decodecmd(QString msg,QString &return_msg);//解码
protected:
    static QString rc_tcp(int tcp);
    static QString rc_speed(float speed);
    static QString rc_move(RobPos pos,Robmovemodel movemodel);
    static QString rc_time(int time);
    static QString rc_task(int task);
    static QString rc_work(int work);
    static QString rc_eled(float eled);
    static QString rc_elem(int elem);
    static QString rc_route(int route);

    static int de_param(int param_n,QString msg,QString &paramname,int &data_fpos,int &data_bpos,QString &return_msg);
    static int de_float(QString parakey,QString msg,int data_fpos,int data_bpos,float &floatdata,QString &return_msg);
    static int de_int(QString parakey,QString msg,int data_fpos,int data_bpos,int &intdata,QString &return_msg);
    static int de_robpos(QString parakey,QString msg,int data_fpos,int data_bpos,RobPos &pos,QString &return_msg);
};

#endif // MY_CMD_H
