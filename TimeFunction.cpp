#include "TimeFunction.h"

TimeFunction::TimeFunction()
{

}

TimeFunction::~TimeFunction()
{

}

void TimeFunction::get_time_ms(std::string *timeOut)
{
#if _MSC_VER
    typedef struct
    {
        __int64 tv_sec;
        __int64 tv_usec;
    }timeval;
    timeval tv;
#else
    struct timeval tv;
#endif
    struct tm* ptm;
    char time_string[40];
    char time_string2[40];
    long milliseconds;
#if _MSC_VER
    auto time_now = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    auto duration_in_s = std::chrono::duration_cast<std::chrono::seconds>(time_now.time_since_epoch()).count();
    auto duration_in_us = std::chrono::duration_cast<std::chrono::microseconds>(time_now.time_since_epoch()).count();
    tv.tv_sec = duration_in_s;
    tv.tv_usec = duration_in_us;
#else
    gettimeofday(&tv, NULL);
#endif
    ptm = localtime (&(tv.tv_sec));
    milliseconds = tv.tv_usec / 1000;
    strftime (time_string, sizeof(time_string), "%Y_%m_%d_%H_%M_%S", ptm);
    snprintf (time_string2, sizeof(time_string2), "%s_%03ld", time_string, milliseconds);
    (*timeOut)=time_string2;
}
