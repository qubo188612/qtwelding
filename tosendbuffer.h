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

    void cmd_lock(bool lock);   //机器人是否锁住

    void cmd_move(RobPos pos,Robmovemodel movemodel,float speed,int tcp);//移动命令

protected:
    toSendbuffer();
    ~toSendbuffer();

};

#endif // TOSENDBUFFER_H
