#ifndef SYNCHRONOUS_H
#define SYNCHRONOUS_H

#include "my_parameters.h"

class my_parameters;

class Synchronous
{
public:
    static Synchronous *Get(my_parameters *mcs);

    bool Scantrace_to_Weldtrace(std::vector<Scan_trace_line> scan_trace,std::vector<Eigen::Vector3d> &weld_trace);

protected:
    my_parameters *m_mcs;

    Synchronous();
    ~Synchronous();
};

#endif // SYNCHRONOUS_H
