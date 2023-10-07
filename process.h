#ifndef PROCESS_H
#define PROCESS_H
#include <QString>
#include <QTextBrowser>

class my_parameters;

class Process1_scanbeforetrace;

class Process           //流程函数
{
public:
    static Process *Get(my_parameters *mcs);

    bool b_processrun;          //工程运行中
    bool b_processpaused;       //工程暂停

    void init_start_process();      //启动流程
    void init_skip_start_process(int stline);  //从第stline行启动程序
    void check_data_process();      //扫描数据查看
    void stop_process();            //停止流程
    void paused_process();          //暂停流程
    void continue_process();        //继续流程

    Process1_scanbeforetrace *process1_scanbeforetrace; //工程类型1先扫描后跟踪

protected:
    Process();
    ~Process();

    my_parameters *m_mcs;
    QTextBrowser *m_record;
};

#endif // PROCESS_H
