#include "MytracerealtimeFunction.h"

Mytracerealtime::Mytracerealtime()
{

}

Mytracerealtime::~Mytracerealtime()
{

}

//返回值-1:结束
//返回值0:正常
//返回值1:点检测失败
int Mytracerealtime::Principal_Component_Analysis(std::vector<RobPos> &robpos, RobPos &realtimepos,int windowsize,float Threshold)
{
    if(robpos.size()<3||robpos.size()<windowsize)    //点数太少
    {
        return -1;
    }

    Eigen::MatrixXd pts(windowsize, 3);

    for (int i = 0; i < windowsize; i++) {
        pts(i, 0) = robpos[i].X;
        pts(i, 1) = robpos[i].Y;
        pts(i, 2) = robpos[i].Z;
    }

    // 使用PCA算法计算点云的主方向
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(pts, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::Vector3d principalAxis = svd.matrixV().col(2);

    // 将点云投影到主方向上的直线
    Eigen::VectorXd projections(windowsize);
    for (int i = 0; i < windowsize; i++)
    {
        projections(i) = pts.row(i) * principalAxis;
    }

    // 对投影结果进行阈值滤波
    if (fabs(projections(1) - projections(0)) < Threshold && fabs(projections(1) - projections(2)) < Threshold)
    {
        realtimepos=robpos[0];
    }
    else
    {
        return 1;
    }

    //去掉首部
    if(robpos.size()>3)
    {
        robpos.erase(robpos.begin());
    }

    return 0;
}
