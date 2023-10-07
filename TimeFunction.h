#ifndef TIMEFUNCTION_H
#define TIMEFUNCTION_H

#include <global.h>
#include <stdio.h>
#include <stdlib.h>
#if _MSC_VER
#include<chrono>
#include <QCoreApplication>
#include <windows.h>
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

#if _MSC_VER
void usleep(unsigned long usec);
#endif

#endif
