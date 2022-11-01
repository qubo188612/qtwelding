#include "process1_scanbeforetrace.h"

Process1_scanbeforetrace *Process1_scanbeforetrace::Get(my_parameters *mcs)
{
    static Process1_scanbeforetrace fun;
    fun.m_mcs=mcs;
    return &fun;
}

Process1_scanbeforetrace::Process1_scanbeforetrace()
{
    b_thread=false;
    thread = new Process1Thread(this);
}

Process1_scanbeforetrace::~Process1_scanbeforetrace()
{
    if(b_thread==true)
    {
        thread->Stop();
        thread->quit();
        thread->wait();
    }
    delete thread;
}

void Process1_scanbeforetrace::init_start_process()
{
    b_thread=true;
    thread->start();
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

Process1Thread::Process1Thread(Process1_scanbeforetrace *statci_p)
{
    _p=statci_p;
}

void Process1Thread::run()
{
    if(_p->b_thread==true)
    {

    }

    _p->b_thread=false;
    _p->thread->quit();
    _p->thread->wait();
}

void Process1Thread::Stop()
{
  if(_p->b_thread==true)
  {
    _p->b_stop_thread=false;
    _p->b_thread=false;
    while (_p->b_stop_thread==false)
    {
      sleep(0);
    }
  }
}

