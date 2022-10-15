#ifndef MY_PARAMETERS_H
#define MY_PARAMETERS_H
#include "project.h"

class my_parameters
{
public:
    static my_parameters *Get();

    Project *project;   //工程项目

protected:
    my_parameters();
    ~my_parameters();
};

#endif // MY_PARAMETERS_H
