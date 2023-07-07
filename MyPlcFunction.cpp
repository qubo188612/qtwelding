#include "MyPlcFunction.h"

Mypcl::Mypcl()
{

}

Mypcl::~Mypcl()
{

}
/*
int Mypcl::Moving_Least_Squares(std::vector<RobPos> robpos_in,           //输入点云
                                std::vector<RobPos> &robpos_out,         //输出点云
                                float msl_search_size,                   //设置搜索半径
                                int msl_poly,                            //多项式最高阶
                                float msl_samp_radius,                   //设置上采样半径
                                float msl_samp_step)                     //设置上采样步长
{
    return 0;
}

int Mypcl::Statistical_Outlier_Removal(std::vector<RobPos> robpos_in,           //输入点云
                                       std::vector<RobPos> &robpos_out,         //输出点云
                                       int sor_nearpoint_num,                   //设置邻域点数量
                                       float sor_standard_deviation)            //标准差
{
    return 0;
}
*/

int Mypcl::Moving_Least_Squares(std::vector<RobPos> robpos_in,            //输入点云
                                 std::vector<RobPos> &robpos_out,         //输出点云
                                 int msl_poly)                            //多项式最高阶
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
        point.data[3]=n;
        input_cloud->push_back(point);
    }

    pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>);
    tree->setInputCloud(input_cloud);

    mls.setInputCloud(input_cloud);
    mls.setComputeNormals(false);  //设置在最小二乘计算中是否需要存储计算的法线
    mls.setPolynomialOrder(msl_poly);
    mls.setSearchMethod(tree);
    mls.setSearchRadius(INT_MAX); // 设置搜索半径

    // 设置输出点云的大小与输入点云相同
    pcl::PointCloud<pcl::PointXYZ>::Ptr output_cloud(new pcl::PointCloud<pcl::PointXYZ>);
    output_cloud->resize(input_cloud->size());

    mls.process(*output_cloud);

    robpos_out.reserve(output_cloud->points.size());
    for(int n=0;n<output_cloud->points.size();n++)
    {
        RobPos pos;
        int id=output_cloud->points[n].data[3];
        pos=robpos_in[id];
        pos.X=output_cloud->points[n].x;
        pos.Y=output_cloud->points[n].y;
        pos.Z=output_cloud->points[n].z;
        robpos_out.push_back(pos);
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
        point.data[3]=n;
        input_cloud->push_back(point);
    }

    sor.setInputCloud(input_cloud);
    sor.setMeanK(sor_nearpoint_num); // 设置邻域点数量
    sor.setStddevMulThresh(sor_standard_deviation); // 设置标准差倍数阈值

    // 设置输出点云的大小与输入点云相同
    pcl::PointCloud<pcl::PointXYZ>::Ptr output_cloud(new pcl::PointCloud<pcl::PointXYZ>);
    output_cloud->resize(input_cloud->size());

    sor.filter(*output_cloud);

    robpos_out.reserve(output_cloud->points.size());
    for(int n=0;n<output_cloud->points.size();n++)
    {
        RobPos pos;
        int id=output_cloud->points[n].data[3];
        pos=robpos_in[id];
        pos.X=output_cloud->points[n].x;
        pos.Y=output_cloud->points[n].y;
        pos.Z=output_cloud->points[n].z;
        robpos_out.push_back(pos);
    }
    return 0;
}

