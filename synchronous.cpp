#include "synchronous.h"


Synchronous *Synchronous::Get(my_parameters *mcs)
{
    static Synchronous fun;
    fun.m_mcs=mcs;
    return &fun;
}

Synchronous::Synchronous()
{

}

Synchronous::~Synchronous()
{

}

bool Synchronous::Scantrace_to_Weldtrace(std::vector<Scan_trace_line> scan_trace,std::vector<RobPos> &weld_trace)
{
    std::vector<RobPos> reweld_trace;
    switch(m_mcs->e2proomdata.demdlg_radio_mod)
    {
        case HAND_IN_EYE:       //眼在手上
        {
            for(int n=0;n<scan_trace.size();n++)
            {
                RobPos robpos=scan_trace[n].robotpos;
                Eigen::Vector3d eye,hand;
                TCP_Leaserpos data_group;
                data_group.leaserpos.Y=scan_trace[n].ros_line.targetpointoutcloud[0].x;
                data_group.leaserpos.Z=scan_trace[n].ros_line.targetpointoutcloud[0].y;
                //时间轴同步在这里做
                data_group.robotpos=scan_trace[n].robotpos;
                if(false==Calibration::hand_on_yes_eyetohand(m_mcs->rob->cal_posture_model,data_group,m_mcs->e2proomdata.matrix_camera2plane,m_mcs->e2proomdata.matrix_plane2robot,hand))
                    return false;
                robpos.X=hand.x();
                robpos.Y=hand.y();
                robpos.Z=hand.z();
                reweld_trace.push_back(robpos);
            }
        }
        break;
        case HAND_OUT_EYE:      //眼在手外
        {
            for(int n=0;n<scan_trace.size();n++)
            {
                RobPos robpos=scan_trace[n].robotpos;
                Eigen::Vector3d eye,hand;
                eye.x()=0;
                eye.y()=scan_trace[n].ros_line.targetpointoutcloud[0].x;
                eye.z()=scan_trace[n].ros_line.targetpointoutcloud[0].y;
                if(false==Calibration::hand_out_yes_eyetohand(eye,m_mcs->e2proomdata.demdlg_R,m_mcs->e2proomdata.demdlg_T,hand))
                    return false;
                robpos.X=hand.x();
                robpos.Y=hand.y();
                robpos.Z=hand.z();
                reweld_trace.push_back(robpos);
            }
        }
        break;
    }
    weld_trace=reweld_trace;
    return true;
}
