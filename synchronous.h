#ifndef SYNCHRONOUS_H
#define SYNCHRONOUS_H

#include "global.h"

class my_parameters;

class Synchronous
{
public:
    static Synchronous *Get(my_parameters *mcs);

    bool Scantrace_to_Weldtrace(std::vector<Scan_trace_line> scan_trace,std::vector<RobPos> &weld_trace);

    bool Scantraceline_to_Weldtrace(std::vector<Scan_trace_line> scan_trace,std::vector<RobPos> &point_cloud);

protected:
    my_parameters *m_mcs;

    Synchronous();
    ~Synchronous();
};

#endif // SYNCHRONOUS_H
