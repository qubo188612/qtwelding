#include "process1_scanbeforetrace.h"

Process1_scanbeforetrace *Process1_scanbeforetrace::Get(my_parameters *mcs)
{
    static Process1_scanbeforetrace fun;
    fun.m_mcs=mcs;
    return &fun;
}

Process1_scanbeforetrace::Process1_scanbeforetrace()
{

}

Process1_scanbeforetrace::~Process1_scanbeforetrace()
{

}

void Process1_scanbeforetrace::init_start_process()
{

}

void Process1_scanbeforetrace::stop_process()
{

}

void Process1_scanbeforetrace::paused_process()
{

}

void Process1_scanbeforetrace::continue_process()
{

}
