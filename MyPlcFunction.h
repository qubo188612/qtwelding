#ifndef MYPLCFUNCTION_H
#define MYPLCFUNCTION_H

#include "global.h"
#include <Eigen/Dense>

#if USE_PLC_FILTER==1
#include <pcl/common/common_headers.h>
#include <pcl/surface/mls.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/search/kdtree.h>
#endif

class Mypcl       //点云计算
{
public:
    Mypcl();
    ~Mypcl();

#if USE_PLC_FILTER==1
    //MLS滤波(擅长平滑和拟合)
    static int Moving_Least_Squares(std::vector<RobPos> robpos_in,    //输入点云
                                    std::vector<RobPos> &robpos_out,  //输出点云
                                    int msl_poly=1);                  //多项式最高阶,0为平滑，1为一项线性曲线拟合，2为二项线性曲线拟合

    //SOR滤波(擅长去除离散点)
    static int Statistical_Outlier_Removal(std::vector<RobPos> robpos_in,           //输入点云
                                           std::vector<RobPos> &robpos_out,         //输出点云
                                           int sor_nearpoint_num=50,                //每个点参考的邻域点数量
                                           float sor_standard_deviation=1.0);       //标准差
#endif

    //SVD滤波(擅长去除离散点)
    static int Singular_Value_Decomposition(std::vector<RobPos> robpos_in,    //输入点云
                                            std::vector<RobPos> &robpos_out,  //输出点云
                                            int Degree=1,                     //主轴曲线拟合的阶数,0为直线,1为一项线性曲线拟合，2为二项线性曲线拟合
                                            int WindowSize=5,                 //曲线拟合的统计点数
                                            float SingularThreshold=0.1);     //距离主轴距离阈值

    //Gaussian滤波(擅长平滑和拟合)
    static int Gaussian(std::vector<RobPos> robpos_in,    //输入点云
                        std::vector<RobPos> &robpos_out,  //输出点云
                        float SmoothingRadius = 2.0,      //平滑半径
                        float SmoothingSigma = 0.5);      //标准差



protected:
    static Eigen::VectorXd fitQuadraticCurve(const Eigen::MatrixXd& points,int Degree);
};

#endif // MYPLCFUNCTION_H
