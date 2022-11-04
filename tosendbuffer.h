#ifndef TOSENDBUFFER_H
#define TOSENDBUFFER_H

#include "global.h"
#include "modbus/modbus.h"
#include "my_parameters.h"

class my_parameters;

class toSendbuffer
{
public:
    static toSendbuffer *Get(my_parameters *mcs);

    my_parameters *m_mcs;

    void cmd_lock(bool lock);   //机器人是否锁住命令

    void cmd_move(RobPos pos,Robmovemodel movemodel,float speed,int tcp);//移动命令
    void cmd_delay(int time);//延时命令
    void cmd_cam(int task,int work);//相机启动停止命令
    void cmd_elec(float eled,Alternatingcurrent elem,int work);//焊机启停命令

    int cmdlist_check();//检查指令
    int cmdlist_build(volatile int &line);//把机器人文件命令编译,line为当前开始的编译步骤
    void cmdlist_stopbuild();//把机器人文件停止编译

    int cmdlist_creat_tracename_mem(int beforeline,std::vector<QString> &errmsg);//创建beforeline之前的扫描\跟踪轨迹存放空间,beforeline值为1到m_mcs->project->project_cmdlist.size();


    void cmd_clear_elec_work();//清除焊接寄存器当前状态

protected:
    toSendbuffer();
    ~toSendbuffer();

    volatile bool b_cmdlist_build; //是否在运行cmdlist_build

    u_int16_t u16data_elec_work;     //焊机启停寄存器值

};

#endif // TOSENDBUFFER_H
