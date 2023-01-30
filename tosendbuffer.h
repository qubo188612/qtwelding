#ifndef TOSENDBUFFER_H
#define TOSENDBUFFER_H

#include <QFile>
#include <QTextCodec>
#include "global.h"
#include "modbus/modbus.h"
#include "my_parameters.h"
#include "TimeFunction.h"
#include "fitlineside.h"
#include "cweldtarject.h"

#define SAVELOGFILE_SCANNAME_HEAD          "SCAN_"               //保存的扫描轨迹类型的文件前缀名称
#define SAVELOGFILE_CREATNAME_HEAD         "CREAT_"              //保存的焊接轨迹类型(未添加姿态前)的文件前缀名称
#define SAVELOGFILE_TRACENAME_HEAD         "TRACE_"              //保存的焊接轨迹类型的文件前缀名称

class my_parameters;

class toSendbuffer
{
public:
    static toSendbuffer *Get(my_parameters *mcs);

    my_parameters *m_mcs;

    void cmd_lock(int lock);   //机器人是否锁住命令:0允许运行(不继续之前的缓存)，1暂停，2继续运行(继续之前的缓存)
    QString paused_key;     //暂停前的命令执行key

    void cmd_move(RobPos pos,Robmovemodel movemodel,float speed,int tcp);//移动命令
    void cmd_delay(int time);//延时命令
    void cmd_cam(int task,int work);//相机启动停止命令
    void cmd_elec(float eled,Alternatingcurrent elem,Weldworkmodel work);//焊机电流和模式设置以及启停命令
    void cmd_elec(Weldworkmodel work);//焊机启停命令
    void cmd_ioout(std::vector<int> io);//IO输出命令

    int cmdlist_check();//检查指令
    int cmdlist_build(volatile int &line);//把机器人文件命令编译,line为当前开始的编译步骤
    void cmdlist_stopbuild();//把机器人文件停止编译
    void cmdlist_pausedbuild();//把机器人文件暂停编译

    int cmdlist_creat_tracename_mem(int beforeline,std::vector<QString> &errmsg);//创建beforeline之前的扫描\跟踪轨迹存放空间,beforeline值为1到m_mcs->project->project_cmdlist.size();


    void cmd_clear_elec_work();//清除焊接寄存器当前状态

protected:
    toSendbuffer();
    ~toSendbuffer();

    volatile bool b_cmdlist_build; //是否在运行cmdlist_build

    u_int16_t u16data_elec_work;     //焊机启停寄存器值

    int savelog_scan(QString filename,std::vector<Scan_trace_line> trace);    //保存扫描轨迹
    int savelog_creat(QString filename,std::vector<RobPos> trace); //保存跟踪(未添加姿态前)轨迹
    int savelog_trace(QString filename,std::vector<RobPos> trace); //保存跟踪轨迹
};

#endif // TOSENDBUFFER_H
