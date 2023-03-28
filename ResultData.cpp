#include "ResultData.h"


ResultData::ResultData()
{
    link_result_state=false;
    link_param_state=false;
    link_robotset_state=false;
    link_ftp_state=false;
    b_luzhi=false;
    b_send_group_leaser=false;
    ctx_result_dosomeing=DO_NOTHING;

    homography_matrix.resize(9);//激光器中的相机内参
    pData_demdlg_R.resize(9);//激光器中的眼在手外标定矩阵R
    pData_demdlg_T.resize(3);//激光器中的眼在手外标定矩阵T
    pData_matrix_camera2plane.resize(9);//激光器中的眼在手上标定矩阵
    pData_matrix_plane2robot.resize(9);//激光器中的眼在手上标定矩阵
}

ResultData::~ResultData()
{

}
