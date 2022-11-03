#ifndef MY_CMD_H
#define MY_CMD_H

#include <QString>
#include <QStringList>
#include <global.h>

/************************/
//项目0:命令集合
//注释符号，举例 #测试
//移动指令，举例 MOV: SPEED[25] MOVL[1.3,32.7,45,66,7,89,3] TCP[1]
//延时指令，举例 DELAY: TIME[1000]
//激光指令，举例 CAM: TASK[102] WORK[1]
//激光指令，举例 CAM: WORK[0]
//焊机指令，举例 WELD: WORK[1] ELED[1.23] ELEM[0]
//焊机指令，举例 WELD: WORK[0]
//采集指令，举例 SCAN: MOVL[1.3,32.7,45,66,7,89,3] SPEED[25] TCP[0] NAME[第一条line]
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
#define CMD_NAME                            "NAME"      //命名


/************************/
class my_cmd
{
public:
    my_cmd();

    QString cmd_move(RobPos pos,Robmovemodel movemodel,float speed,int tcp);//移动命令
    QString cmd_delay(int time);//延时命令
    QString cmd_cam(int task,int work);//相机启停命令不
    QString cmd_cam_work(int work);//相机启停命令
    QString cmd_elec(float eled,Alternatingcurrent elem,int work);//焊机启停命令不
    QString cmd_elec_work(int work);//焊机启停命令
    QString cmd_scan(RobPos pos,float speed,int tcp,QString name);//采集命令
    QString cmd_trace(int route,float speed,int tcp);//跟踪命令


    int decodecmd(QString msg,QString &return_msg,QString &return_key);//解码：返回值0:正常
                                                                       //     返回值-1:注释行
                                                                       //     返回值>0:异常
    int cmd_move_tcp;//获取到移动TCP
    RobPos cmd_move_pos;//获取到移动终点坐标
    float cmd_move_speed;//获取到速度值
    Robmovemodel cmd_move_movemod;//获取到的移动模式

    int cmd_delay_time;//获取到延时时间

    int cmd_cam_task;//获取到相机任务号
    int cmd_cam_work_d;//获取到相机启停

    int cmd_elec_work_d;//获取到焊机启停
    Alternatingcurrent cmd_elec_elem;  //获取到焊机交变电流模式
    float cmd_elec_eled; //获取到焊机电流

    RobPos cmd_scan_pos;//获取到扫描终点坐标
    float cmd_scan_speed;//获取到的扫描速度
    int cmd_scan_tcp;//获取到扫描TCP
    Robmovemodel cmd_scan_movemod;//获取到的扫描模式
    QString cmd_scan_name;//获取到的扫描轨迹名字

    int cmd_trace_route;//获取到跟踪轨迹序号
    float cmd_trace_speed;//获取到的跟踪速度
    int cmd_trace_tcp;//获取到跟踪TCP


protected:
    QString rc_tcp(int tcp);
    QString rc_speed(float speed);
    QString rc_move(RobPos pos,Robmovemodel movemodel);
    QString rc_time(int time);
    QString rc_task(int task);
    QString rc_work(int work);
    QString rc_eled(float eled);
    QString rc_elem(Alternatingcurrent elem);
    QString rc_route(int route);
    QString rc_name(QString name);

    int de_param(int param_n,QString msg,QString &paramname,int &data_fpos,int &data_bpos,QString &return_msg);
    int de_float(QString parakey,QString msg,int data_fpos,int data_bpos,float &floatdata,QString &return_msg);
    int de_int(QString parakey,QString msg,int data_fpos,int data_bpos,int &intdata,QString &return_msg);
    int de_robpos(QString parakey,QString msg,int data_fpos,int data_bpos,RobPos &pos,QString &return_msg);
    int de_QString(QString parakey,QString msg,int data_fpos,int data_bpos,QString &QStringdata,QString &return_msg);
};

#endif // MY_CMD_H
