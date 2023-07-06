#include "MyPlcFunction.h"
#include <pcl/kdtree/kdtree_flann.h>

Mypcl::Mypcl()
{

}

Mypcl::~Mypcl()
{

}

int Mypcl::Moving_Least_Squares(std::vector<RobPos> robpos_in,           //输入点云
                                 std::vector<RobPos> &robpos_out,         //输出点云
                                 float msl_search_size,                   //设置搜索半径
                                 int msl_poly,                            //多项式最高阶
                                 float msl_samp_radius,                   //设置上采样半径
                                 float msl_samp_step)                     //设置上采样步长
{ 
    if(robpos_in.size()<=3)
    {
        robpos_out=robpos_in;
        return 0;
    }
    pcl::MovingLeastSquares<pcl::PointXYZ, pcl::PointXYZ> mls;
    pcl::PointCloud<pcl::PointXYZ>::Ptr input_cloud(new pcl::PointCloud<pcl::PointXYZ>);

    for(int n=0;n<robpos_in.size();n++)
    {
        pcl::PointXYZ point;
        point.x=robpos_in[n].X;
        point.y=robpos_in[n].Y;
        point.z=robpos_in[n].Z;
        input_cloud->push_back(point);
    }

    mls.setInputCloud(input_cloud);
    mls.setSearchRadius(msl_search_size); // 设置搜索半径
    mls.setPolynomialOrder(msl_poly);
    mls.setUpsamplingMethod(pcl::MovingLeastSquares<pcl::PointXYZ, pcl::PointXYZ>::SAMPLE_LOCAL_PLANE);
    mls.setUpsamplingRadius(msl_samp_radius); // 设置上采样半径
    mls.setUpsamplingStepSize(msl_samp_step); // 设置上采样步长

    // 设置输出点云的大小与输入点云相同
    pcl::PointCloud<pcl::PointXYZ>::Ptr output_cloud(new pcl::PointCloud<pcl::PointXYZ>);
    output_cloud->resize(input_cloud->size());

    mls.process(*output_cloud);

    if(output_cloud->points.size()!=robpos_in.size())
    {
        return 1;
    }
    robpos_out=robpos_in;
    for(int n=0;n<robpos_out.size();n++)
    {
        robpos_out[n].X=output_cloud->points[n].x;
        robpos_out[n].Y=output_cloud->points[n].y;
        robpos_out[n].Z=output_cloud->points[n].z;
    }
    return 0;
}

int Mypcl::Statistical_Outlier_Removal(std::vector<RobPos> robpos_in,           //输入点云
                                        std::vector<RobPos> &robpos_out,         //输出点云
                                        int sor_nearpoint_num,                   //设置邻域点数量
                                        float sor_standard_deviation)            //标准差
{
    if(robpos_in.size()<=3)
    {
        robpos_out=robpos_in;
        return 0;
    }

    pcl::StatisticalOutlierRemoval<pcl::PointXYZ> sor;
    pcl::PointCloud<pcl::PointXYZ>::Ptr input_cloud(new pcl::PointCloud<pcl::PointXYZ>);

    for(int n=0;n<robpos_in.size();n++)
    {
        pcl::PointXYZ point;
        point.x=robpos_in[n].X;
        point.y=robpos_in[n].Y;
        point.z=robpos_in[n].Z;
        input_cloud->push_back(point);
    }

    sor.setInputCloud(input_cloud);
    sor.setMeanK(sor_nearpoint_num); // 设置邻域点数量
    sor.setStddevMulThresh(sor_standard_deviation); // 设置标准差倍数阈值

    // 设置输出点云的大小与输入点云相同
    pcl::PointCloud<pcl::PointXYZ>::Ptr output_cloud(new pcl::PointCloud<pcl::PointXYZ>);
    output_cloud->resize(input_cloud->size());

    sor.filter(*output_cloud);

    if(output_cloud->points.size()!=robpos_in.size())
    {
        return 1;
    }
    robpos_out=robpos_in;
    for(int n=0;n<robpos_out.size();n++)
    {
        robpos_out[n].X=output_cloud->points[n].x;
        robpos_out[n].Y=output_cloud->points[n].y;
        robpos_out[n].Z=output_cloud->points[n].z;
    }
    return 0;
}

