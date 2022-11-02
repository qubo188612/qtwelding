#ifndef PROCESS1_SCANBEFORETRACE_H
#define PROCESS1_SCANBEFORETRACE_H
#include "my_parameters.h"
#include "my_cmd.h"

class my_parameters;

class Process1Thread;

class Process1_scanbeforetrace      //工程1先扫描后跟踪
{
public:
    static Process1_scanbeforetrace *Get(my_parameters *mcs);

    my_parameters *m_mcs;

    void init_start_process();      //启动流程
    void stop_process();            //停止流程
    void paused_process();          //暂停流程
    void continue_process();        //继续流程

    Process1Thread *thread;            //运行线程
    bool b_thread;
    bool b_stop_thread;

    volatile int buildline;//当前执行的cmd行数

protected:

    Process1_scanbeforetrace();
    ~Process1_scanbeforetrace();
};

class Process1Thread : public QThread
{
public:
    Process1Thread(Process1_scanbeforetrace *statci_p);
    void Stop();
protected:
    void run();
private:
    Process1_scanbeforetrace *_p;
};

#endif // PROCESS1_SCANBEFORETRACE_H
