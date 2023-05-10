#include "ccoordchange.h"

CCoordChange::CCoordChange()
{

}

CCoordChange::~CCoordChange()
{

}

Eigen::Matrix3d CCoordChange::ComputeDCM3(std::array<Eigen::Vector3d, 3> &coord_1,std::array<Eigen::Vector3d, 3> &coord_2)
{

    using Eigen::Matrix3d;
    using Eigen::Vector3d;
    Matrix3d rotation = Matrix3d::Zero();

    Vector3d i = coord_1[0], j = coord_1[1], k = coord_1[2];
    Vector3d x = coord_2[0], y = coord_2[1], z = coord_2[2];

    rotation(0, 0) = i.dot(x) / (i.norm()*x.norm());
    rotation(0, 1) = i.dot(y) / (i.norm()*y.norm());
    rotation(0, 2) = i.dot(z) / (i.norm()*z.norm());
    rotation(1, 0) = j.dot(x) / (j.norm()*x.norm());
    rotation(1, 1) = j.dot(y) / (j.norm()*y.norm());
    rotation(1, 2) = j.dot(z) / (j.norm()*z.norm());
    rotation(2, 0) = k.dot(x) / (k.norm()*x.norm());
    rotation(2, 1) = k.dot(y) / (k.norm()*y.norm());
    rotation(2, 2) = k.dot(z) / (k.norm()*z.norm());

    return rotation;
}

bool CCoordChange::coord2RT(Eigen::Vector3d org,Eigen::Vector3d x,Eigen::Vector3d y,Eigen::Matrix3d *R_out,Eigen::Vector3d *T_out)//输入坐标点 原点，X方向上的点，XOY面上的点
{
    Eigen::Matrix3d R;		//旋转缩放矩阵
    Eigen::Vector3d T;		//平移变量
    Eigen::Vector3d x1,y1,z1;
    Eigen::Vector3d x2(1,0,0);
    Eigen::Vector3d y2(0,1,0);
    Eigen::Vector3d z2(0,0,1);
    Eigen::Vector3d lo = org;
    Eigen::Vector3d la = x;
    Eigen::Vector3d lb = y;
    x1 = la - lo;
    z1 = x1.cross(lb-lo);
    y1 = z1.cross(x1);
    std::array<Eigen::Vector3d,3> coord1 = {x1,y1,z1};
    std::array<Eigen::Vector3d,3> coord2 = {x2,y2,z2};
    Eigen::Matrix3d roate = ComputeDCM3(coord2,coord1);
    for (int i = 0 ; i < 3; i++)
    {
        for (int j = 0 ; j < 3; j++)
        {
            R(i,j) = roate(i,j);
        }
    }
    T(0) = org.x();
    T(1) = org.y();
    T(2) = org.z();

    *R_out=R;
    *T_out=T;

    return true;
}

Eigen::Vector3d CCoordChange::point2point(Eigen::Vector3d p,Eigen::Matrix3d R,Eigen::Vector3d T)
{
    Eigen::Vector3d temp;
    temp = R*p+T;
    return temp;
}
