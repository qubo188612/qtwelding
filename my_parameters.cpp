#include "my_parameters.h"

my_parameters *my_parameters::Get()
{
    static my_parameters fun;
    return &fun;
}

my_parameters::my_parameters()
{
    project=project->Get();
    craft=craft->Get();
    cam=cam->Get();
    ip=ip->Get();
    rob=rob->Get();
    tosendbuffer=tosendbuffer->Get(this);
    process=process->Get(this);
    process->process1_scanbeforetrace=process->process1_scanbeforetrace->Get(this);
    for(int n=0;n<CAMTOTALNUM;n++)
    {
        cam->sop_cam[n].ipaddress=ip->camer_ip[n].ip;
    }
    for(int n=0;n<CAMTOTALNUM;n++)
    {
        cam->sop_cam[n].ipaddress=ip->camer_ip[n].ip;
    }
}

my_parameters::~my_parameters()
{

}
