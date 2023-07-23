#ifndef CCOORDCHANGE_H
#define CCOORDCHANGE_H

#include <Eigen/Core>
#include <Eigen/Dense>
#include <global.h>

class CCoordChange  //两坐标矩阵运算
{
public:
    CCoordChange();
    ~CCoordChange();

    static bool coord2RT(Eigen::Vector3d org,Eigen::Vector3d x,Eigen::Vector3d y,Eigen::Matrix3d *R_out,Eigen::Vector3d *T_out);//输入坐标点 原点，X方向上的点，XOY面上的点
    static bool _1coord2RT(Eigen::Vector3d org,Eigen::Vector3d x,Eigen::Vector3d y,Eigen::Matrix3d *R_out,Eigen::Vector3d *T_out);//输入坐标点 原点，X方向上的点，XOY面上的点
    static Eigen::Vector3d point2point(Eigen::Vector3d p,Eigen::Matrix3d R,Eigen::Vector3d T);//将原来的点输出为调整坐标系后的点
    static Eigen::Vector3d _1point2point(Eigen::Vector3d p,Eigen::Matrix3d R,Eigen::Vector3d T);//将原来的点输出为调整坐标系后的点
private:
    static Eigen::Matrix3d ComputeDCM3(std::array<Eigen::Vector3d, 3> &coord_1,std::array<Eigen::Vector3d, 3> &coord_2);
};

#endif // CCOORDCHANGE_H
