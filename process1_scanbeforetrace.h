#ifndef PROCESS1_SCANBEFORETRACE_H
#define PROCESS1_SCANBEFORETRACE_H
#include "my_parameters.h"

class my_parameters;

class Process1_scanbeforetrace      //工程1先扫描后跟踪
{
public:
    static Process1_scanbeforetrace *Get(my_parameters *mcs);

    void init_start_process();      //启动流程
    void stop_process();        //停止流程
    void paused_process();          //暂停流程
    void continue_process();        //继续流程

protected:

    Process1_scanbeforetrace();
    ~Process1_scanbeforetrace();

    my_parameters *m_mcs;
};

#endif // PROCESS1_SCANBEFORETRACE_H
