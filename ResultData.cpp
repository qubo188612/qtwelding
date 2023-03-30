#include "ResultData.h"

rob_pinfo::rob_pinfo()
{
  x=0;
  y=0;
  z=0;
  rx=0;
  ry=0;
  rz=0;
  out1=0;
  out2=0;
  out3=0;
  tool=0;
  tcp=0;
  usertcp=0;
  uy=0;
  vz=0;
}

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
