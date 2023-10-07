#ifndef WINSLEEP_H
#define WINSLEEP_H

#include <QCoreApplication>

#if _MSC_VER
#include <windows.h>
#define sleep(sec)   Sleep(sec * 1000)
static void usleep(unsigned long usec)
{
    HANDLE timer;
    LARGE_INTEGER interval;
    interval.QuadPart = -(10 * usec);

    timer = CreateWaitableTimer(NULL, TRUE, NULL);
    SetWaitableTimer(timer, &interval, 0, NULL, NULL, 0);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}
#endif

#endif // WINSLEEP_H
