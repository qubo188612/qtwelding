#include "cam_sen.h"

Cam_Sen *Cam_Sen::Get()
{
    static Cam_Sen fun;
    return &fun;
}

Cam_Sen::Cam_Sen()
{

}

Cam_Sen::~Cam_Sen()
{

}
