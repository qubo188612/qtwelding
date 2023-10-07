#include "process.h"
#include "process1_scanbeforetrace.h"
#include "my_parameters.h"

Process *Process::Get(my_parameters *mcs)
{
    static Process fun;
    fun.m_mcs=mcs;
    return &fun;
}

Process::Process()
{
    b_processrun=false;
    b_processpaused=false;
}

Process::~Process()
{

}

void Process::init_start_process()      //启动流程
{
    switch(m_mcs->project->project_Id)
    {
        case PROGECT_ID_TEACH_SCAN:
            m_mcs->process->process1_scanbeforetrace->init_start_process();
        break;
    }
    b_processrun=true;
    b_processpaused=false;
}

void Process::init_skip_start_process(int stline)
{
    switch(m_mcs->project->project_Id)
    {
        case PROGECT_ID_TEACH_SCAN:
            m_mcs->process->process1_scanbeforetrace->init_skip_start_process(stline);
        break;
    }
    b_processrun=true;
    b_processpaused=false;
}

void Process::check_data_process()
{
    switch(m_mcs->project->project_Id)
    {
        case PROGECT_ID_TEACH_SCAN:
            m_mcs->process->process1_scanbeforetrace->check_data_process();
        break;
    }
}

void Process::stop_process()           //停止流程
{
    switch(m_mcs->project->project_Id)
    {
        case PROGECT_ID_TEACH_SCAN:
            m_mcs->process->process1_scanbeforetrace->stop_process();
        break;
    }
    b_processrun=false;
    b_processpaused=false;
}

void Process::paused_process()          //暂停流程
{
    switch(m_mcs->project->project_Id)
    {
        case PROGECT_ID_TEACH_SCAN:
            m_mcs->process->process1_scanbeforetrace->paused_process();
        break;
    }
    b_processpaused=true;
}

void Process::continue_process()        //继续流程
{
    switch(m_mcs->project->project_Id)
    {
        case PROGECT_ID_TEACH_SCAN:
            m_mcs->process->process1_scanbeforetrace->continue_process();
        break;
    }
    b_processpaused=false;
}
