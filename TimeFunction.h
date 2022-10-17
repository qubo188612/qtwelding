#ifndef TIMEFUNCTION_H
#define TIMEFUNCTION_H

#include <global.h>
#include <stdio.h>
#include <stdlib.h>
#if _MSC_VER
#include<chrono>
#else
#include <sys/time.h>
#endif
#include <time.h>
#include <string>

//#include <QString>

class TimeFunction
{
public:
    TimeFunction();
    ~TimeFunction();

    void get_time_ms(std::string *timeOut);
};
#endif
