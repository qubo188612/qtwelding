#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <Eigen/Core>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <global.h>


#define  CAL_RADIAN			(CV_PI/180.0)
#define  CAL_ANGLE          (180.0/CV_PI)

typedef enum CAL_POSTURE_MODEL_ID          //机器人姿态角含义模式
{
    CAL_ROBOT_YASKAWA=0,        //安川、ABB、川崎、UR、越彊姿态标准 内旋 RX-RY-RZ
    CAL_ROBOT_KUKA=1,           //库卡姿态标准 内旋 RZ-RY-RX
    CAL_ROBOT_PANASONIC=2,      //松下姿态标准 内旋 RX-"RY"-RZ
    CAL_ROBOT_RZRXRZ=3,         //内旋RZ-RX-RZ
    CAL_ROBOT_KAWASAKI=4,       //川崎姿态标准 内旋RZ-RY-RZ
    CAL_ROBOT_RYRZRX=5,         //内旋RY-RZ-RX
    CAL_ROBOT_RZRXRY=6,         //内旋RZ-RX-RY
    CAL_ROBOT_RXRZRY=7,         //内旋RX-RZ-RY
    CAL_ROBOT_RYRXRZ=8,         //内旋RY-RX-RZ
    CAL_ROBOT_RXRYRX=9,         //内旋RX-RY-RX
    CAL_ROBOT_RYRZRY=10,        //内旋RY-RZ-RY
    CAL_ROBOT_RXRZRX=11,        //内旋RX-RZ-RX
    CAL_ROBOT_RYRXRY=12,        //内旋RY-RX-RY
}CAL_POSTURE;

class Calibration       //机器人手眼标定
{
public:
    Calibration();
    ~Calibration();
    //眼在手外的标定,计算旋转和标定矩阵
    static bool hand_out_yes_point2RT(std::vector<Eigen::Vector3d> &p1,    //输入眼坐标
                                      std::vector<Eigen::Vector3d> &p2,    //输入手坐标
                                      Eigen::Matrix3d &R,                  //输出旋转矩阵
                                      Eigen::Vector3d &T,                  //输出平移举证
                                      double &err,                         //输出总体精度误差
                                      std::vector<double> &errgroup);      //输出单个精度误差

    //眼在手上的标定,计算旋转和标定矩阵
    static bool hand_on_yes_point2RT(CAL_POSTURE robot,                     //输入姿态标准
                                     RobPos robpos,                         //输入标定点手坐标
                                     std::vector<TCP_Leaserpos> data_group, //输入标定点眼坐标
                                     cv::Mat &matrix_camera2plane,          //输出眼到中间矩阵
                                     cv::Mat &matrix_plane2robot,           //输出中间矩阵到机器人矩阵
                                     double &err,                           //输出总体精度误差
                                     std::vector<double> &errgroup);        //输出单个精度误差

    //眼在手外的标定,计算眼到手输出点
    static bool hand_out_yes_eyetohand(Eigen::Vector3d p_eys,  //输入眼坐标
                                       Eigen::Matrix3d R,      //输入旋转矩阵
                                       Eigen::Vector3d T,      //输入平移举证
                                       Eigen::Vector3d &p_hand //输出手坐标
                                       );

    //眼在手上的标定,计算眼到手输出点
    static bool hand_on_yes_eyetohand (CAL_POSTURE robot,                     //输入姿态标准
                                       TCP_Leaserpos data_group,              //输入标定点眼坐标
                                       cv::Mat &matrix_camera2plane,          //输出眼到中间矩阵
                                       cv::Mat &matrix_plane2robot,           //输出中间矩阵到机器人矩阵
                                       Eigen::Vector3d &p_hand                //输出手坐标
                                       );
    //姿态差值
    static std::vector<Eigen::Vector3d> Attitudedifference(CAL_POSTURE robot,          //输入姿态标准
                                                           Eigen::Vector3d PosR_st,    //起点姿态
                                                           Eigen::Vector3d PosR_ed,    //终点姿态
                                                           int differenceNum);         //插值个数

    //获取第N个姿态
    static Eigen::Vector3d Attitudedifference_N(CAL_POSTURE robot,          //输入姿态标准
                                                Eigen::Vector3d PosR_st,    //起点姿态
                                                Eigen::Vector3d PosR_ed,    //终点姿态
                                                int differenceNum,          //插值个数
                                                int N);                     //第N个值,N取值为0到differenceNum-1

   //姿态角旋转
   static std::array<double,3> Attitudeangleroation(CAL_POSTURE robot,           //输入姿态标准
                                                    Eigen::Matrix3d R,           //输入旋转矩阵
                                                    std::array<double,3> pose);  //输入姿态
protected:

   static cv::Mat OLS_Plane_V(std::vector<Eigen::Vector3d> points);
   static Eigen::Matrix3d ComputeDCM3(std::array<Eigen::Vector3d, 3> &coord_1,std::array<Eigen::Vector3d, 3> &coord_2);
   static cv::Mat computePlane2Robot_posture(cv::Mat mp0,cv::Mat mp_r,cv::Mat mp_s,Eigen::Matrix3d pst);
   static cv::Mat computePlane2Robot(cv::Mat plane, cv::Mat robot,int num);
   static Eigen::Matrix3d Euler2RotMatrixX(const double rx);
   static Eigen::Matrix3d Euler2RotMatrixY(const double ry);
   static Eigen::Matrix3d Euler2RotMatrixZ(const double rz);
   static Eigen::Matrix3d Yaskawa_Euler2RotMatrixXYZ(std::array<double,3> pst);
   static Eigen::Matrix3d Kuka_Euler2RotMatrixXYZ(std::array<double,3> pst);
   static Eigen::Matrix3d Panasonic_Euler2RotMatrixXYZ(std::array<double,3> pst);
   static Eigen::Matrix3d RzRxRz_Euler2RotMatrixXYZ(std::array<double,3> pst);
   static Eigen::Matrix3d RyRzRx_Euler2RotMatrixXYZ(std::array<double,3> pst);
   static Eigen::Matrix3d RzRxRy_Euler2RotMatrixXYZ(std::array<double,3> pst);
   static Eigen::Matrix3d RxRzRy_Euler2RotMatrixXYZ(std::array<double,3> pst);
   static Eigen::Matrix3d RyRxRz_Euler2RotMatrixXYZ(std::array<double,3> pst);
   static Eigen::Matrix3d RxRyRx_Euler2RotMatrixXYZ(std::array<double,3> pst);
   static Eigen::Matrix3d RyRzRy_Euler2RotMatrixXYZ(std::array<double,3> pst);
   static Eigen::Matrix3d RxRzRx_Euler2RotMatrixXYZ(std::array<double,3> pst);
   static Eigen::Matrix3d RyRxRy_Euler2RotMatrixXYZ(std::array<double,3> pst);
   static Eigen::Matrix3d Kawasaki_Euler2RotMatrixXYZ(std::array<double,3> pst);
   static Eigen::Matrix3d Euler2RotMatrixXYZ(CAL_POSTURE robot,std::array<double,3> pst);//姿态转化为旋转矩阵
   static std::array<double, 3> Yaskawa_RotMatrixXYZ2Euler(Eigen::Matrix3d rot_matrix);
   static std::array<double, 3> Kuka_RotMatrixXYZ2Euler(Eigen::Matrix3d rot_matrix);
   static std::array<double, 3> Panasonic_RotMatrixXYZ2Euler(Eigen::Matrix3d rot_matrix);
   static std::array<double, 3> RzRxRz_RotMatrixXYZ2Euler(Eigen::Matrix3d rot_matrix);
   static std::array<double, 3> RyRzRx_RotMatrixXYZ2Euler(Eigen::Matrix3d rot_matrix);
   static std::array<double, 3> RzRxRy_RotMatrixXYZ2Euler(Eigen::Matrix3d rot_matrix);
   static std::array<double, 3> RxRzRy_RotMatrixXYZ2Euler(Eigen::Matrix3d rot_matrix);
   static std::array<double, 3> RyRxRz_RotMatrixXYZ2Euler(Eigen::Matrix3d rot_matrix);
   static std::array<double, 3> RxRyRx_RotMatrixXYZ2Euler(Eigen::Matrix3d rot_matrix);
   static std::array<double, 3> RyRzRy_RotMatrixXYZ2Euler(Eigen::Matrix3d rot_matrix);
   static std::array<double, 3> RxRzRx_RotMatrixXYZ2Euler(Eigen::Matrix3d rot_matrix);
   static std::array<double, 3> RyRxRy_RotMatrixXYZ2Euler(Eigen::Matrix3d rot_matrix);
   static std::array<double, 3> Kawasaki_RotMatrixXYZ2Euler(Eigen::Matrix3d rot_matrix);
   static std::array<double, 3> RotMatrixXYZ2Euler(CAL_POSTURE robot,Eigen::Matrix3d rot_matrix); //旋转矩阵转换为姿态
   static bool computeMatrix(CAL_POSTURE robot,RobPos robpos,std::vector<TCP_Leaserpos> data_group,cv::Mat &matrix_camera2plane,cv::Mat &matrix_plane2robot);
   static cv::Point3f ComputePosition(CAL_POSTURE robot,TCP_Leaserpos Pic,cv::Mat &matrix_camera2plane,cv::Mat &matrix_plane2robot);
   static cv::Mat ComputePos(cv::Mat Robot_Position,cv::Mat Pic_Position,Eigen::Matrix3d RPY_Rotation,cv::Mat &matrix_plane2robot);
};

#endif // CALIBRATION_H
