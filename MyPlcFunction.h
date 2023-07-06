#ifndef MYPLCFUNCTION_H
#define MYPLCFUNCTION_H

#include "global.h"
#include <pcl/common/common_headers.h>
#include <pcl/surface/mls.h>
#include <pcl/filters/statistical_outlier_removal.h>

class Mypcl       //点云计算
{
public:
    Mypcl();
    ~Mypcl();

    //MLS滤波(擅长平滑和拟合)
    static int Moving_Least_Squares(std::vector<RobPos> robpos_in,    //输入点云
                                    std::vector<RobPos> &robpos_out,  //输出点云
                                    float msl_search_size=0.1,        //设置搜索半径
                                    int msl_poly=1,                   //多项式最高阶,0为平滑，1为一项线性曲线拟合，2为二项线性曲线拟合
                                    float msl_samp_radius=0.05,       //设置上采样半径
                                    float msl_samp_step=0.03);        //设置上采样步长

    //SOR滤波(擅长去除离散点)
    static int Statistical_Outlier_Removal(std::vector<RobPos> robpos_in,           //输入点云
                                           std::vector<RobPos> &robpos_out,         //输出点云
                                           int sor_nearpoint_num=50,                //每个点参考的邻域点数量
                                           float sor_standard_deviation=1.0);       //标准差

protected:

};

#endif // MYPLCFUNCTION_H
