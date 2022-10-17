#include "my_parameters.h"

my_parameters *my_parameters::Get()
{
    static my_parameters fun;
    return &fun;
}

my_parameters::my_parameters()
{
    project=project->Get();
    cam=cam->Get();
    ip=ip->Get();
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
