#include "global.h"

RobPos::RobPos()
{
    X=0;
    Y=0;
    Z=0;
    RX=0;
    RY=0;
    RZ=0;
    nEn=0;
}

leaser_pos::leaser_pos()
{
    Y=0;
    Z=0;
    nEn=0;
}

Pos3f::Pos3f()
{
    X=0;
    Y=0;
    Z=0;
}

bool d_Mysortcompare(d_Mysort &a,d_Mysort &b)
{
    return a.data<b.data;
}
