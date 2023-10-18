#include "MyPlcFunction.h"

Mypcl::Mypcl()
{

}

Mypcl::~Mypcl()
{

}

#if USE_PLC_FILTER==1
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

#endif

// 定义二次多项式拟合函数
Eigen::VectorXd Mypcl::fitQuadraticCurve(const Eigen::MatrixXd& points,int Degree)
{
    int numPoints = points.rows();

    Eigen::MatrixXd A(numPoints, Degree + 1);
    Eigen::VectorXd b(numPoints);

    // 构建系数矩阵A和目标向量b
    for (int i = 0; i < numPoints; ++i) {
        double x = points(i, 0);
        double y = points(i, 1);
        double z = points(i, 2);

        for (int j = 0; j <= Degree; ++j) {
            A(i, j) = pow(x, j);
        }

        b(i) = z;
    }
// 使用最小二乘法求解系数向量
   Eigen::VectorXd coefficients = A.colPivHouseholderQr().solve(b);

   return coefficients;
}

int Mypcl::Singular_Value_Decomposition(std::vector<RobPos> robpos_in,    //输入点云
                                        std::vector<RobPos> &robpos_out,  //输出点云
                                        int Degree,                       //主轴曲线拟合的阶数,0为直线,1为一项线性曲线拟合，2为二项线性曲线拟合
                                        int WindowSize,                   //曲线拟合的统计点数
                                        float SingularThreshold)          //距离主轴距离阈值
{
    if(robpos_in.size()<=3)
    {
        robpos_out=robpos_in;
        return 0;
    }
    Eigen::MatrixXd trajectory(robpos_in.size(),3);
    for(int i=0;i<robpos_in.size();i++)
    {
        trajectory(i,0)=robpos_in[i].X;
        trajectory(i,1)=robpos_in[i].Y;
        trajectory(i,2)=robpos_in[i].Z;
    }

    int numPoints = trajectory.rows();

    robpos_out.reserve(numPoints);

    // 遍历轨迹中的每个点
    for (int i = 0; i < numPoints; ++i) {
        // 计算局部窗口的起始和结束索引
        int startIdx = std::max(0, i - WindowSize);
        int endIdx = std::min(numPoints - 1, i + WindowSize);

        // 提取局部窗口内的点
        Eigen::MatrixXd localPoints = trajectory.block(startIdx, 0, endIdx - startIdx + 1, 3);

        // 执行曲线拟合
        Eigen::VectorXd coefficients = fitQuadraticCurve(localPoints,Degree);

        // 计算当前点与拟合曲线之间的距离
        double x = trajectory(i, 0);
        double y = trajectory(i, 1);
        double z = trajectory(i, 2);
        double sun=0;
        double distance;

        for(int n=0;n<=Degree;n++)
        {
            sun=coefficients(n)*std::pow(x,n)+sun;
        }
        distance=std::abs(z-sun);

        // 根据奇异点判定阈值进行奇异点删除
        if (distance <= SingularThreshold) {
            RobPos rob=robpos_in[i];
            robpos_out.push_back(rob);
        }
    }

    return 0;
}

int Mypcl::Gaussian(std::vector<RobPos> robpos_in,    //输入点云
                    std::vector<RobPos> &robpos_out,  //输出点云
                    float SmoothingRadius,            //平滑半径
                    float SmoothingSigma)             //标准差
{
    if(robpos_in.size()<=3)
    {
        robpos_out=robpos_in;
        return 0;
    }
    Eigen::MatrixXd filteredTrajectory(robpos_in.size(),3);
    for(int i=0;i<robpos_in.size();i++)
    {
        filteredTrajectory(i,0)=robpos_in[i].X;
        filteredTrajectory(i,1)=robpos_in[i].Y;
        filteredTrajectory(i,2)=robpos_in[i].Z;
    }

    for (int i = 0; i < filteredTrajectory.rows(); ++i)
    {
       Eigen::Vector3d point = filteredTrajectory.row(i);

       double weightSum = 0.0;
       Eigen::Vector3d smoothedPoint = Eigen::Vector3d::Zero();

       for (int j = 0; j < filteredTrajectory.rows(); ++j) {
           Eigen::Vector3d neighbor = filteredTrajectory.row(j);
           double distance = (point - neighbor).norm();

           if (distance < SmoothingRadius) {
               double weight = std::exp(-distance * distance / (2.0 * SmoothingSigma * SmoothingSigma));
               smoothedPoint += weight * neighbor;
               weightSum += weight;
           }
       }

       if (weightSum > 0.0) {
           smoothedPoint /= weightSum;
       }
       RobPos rob=robpos_in[i];
       rob.X=smoothedPoint.x();
       rob.Y=smoothedPoint.y();
       rob.Z=smoothedPoint.z();
       robpos_out.push_back(rob);
    }

    return 0;
}

//PCA滤波(擅长主方向滤波)
int Mypcl::Principal_Component_Analysis(std::vector<RobPos> robpos_in, std::vector<RobPos> &robpos_out, float Threshold)
{
    if(robpos_in.size()<3)    //点数太少
    {
       return -1;
    }

    Eigen::MatrixXd pts(robpos_in.size(), 3);

    for (int i = 0; i < robpos_in.size(); i++) {
       pts(i, 0) = robpos_in[i].X;
       pts(i, 1) = robpos_in[i].Y;
       pts(i, 2) = robpos_in[i].Z;
    }

    // 使用PCA算法计算点云的主方向
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(pts, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::Vector3d principalAxis = svd.matrixV().col(2);

    // 将点云投影到主方向上的直线
    Eigen::VectorXd projections(robpos_in.size());
    for (int i = 0; i < robpos_in.size(); i++)
    {
       projections(i) = pts.row(i) * principalAxis;
    }

    // 对投影结果进行阈值滤波
    robpos_out.reserve(robpos_in.size());

    for (int i = 1; i < projections.size() - 1; i++)
    {
       if (fabs(projections(i) - projections(i-1)) < Threshold && fabs(projections(i) - projections(i+1)) < Threshold)
       {
           RobPos rob=robpos_in[i];
           robpos_out.push_back(rob);
       }
    }

    return 0;
}
