#include "fitlineside.h"

FitlineSide::FitlineSide()
{

}

FitlineSide::~FitlineSide()
{

}

Eigen::Vector3d FitlineSide::computePointOfLineAndSurface(std::vector<Eigen::VectorXd> linePoints,std::vector<Eigen::VectorXd> surfacePoints)
{
    std::array<Eigen::VectorXd,2> lineP = OLS_Line(linePoints);
    Eigen::Vector3d planeP = OLS_Plane(surfacePoints);
    double x0 = lineP[0][0],y0 = lineP[0][1],z0 = lineP[0][2];
    double a = lineP[1][0],b = lineP[1][1],c = lineP[1][2];
    double m = planeP[0],n = planeP[1],d = planeP[2];

    double t1 = (m*a+n*b)/c-1;
    double t2 = (m*a*z0+n*b*z0)/c-m*x0-n*y0-d;
    double z = t2/t1;
    double y = (z-z0)*b/c+y0;
    double x = (z-z0)*a/c+x0;
    Eigen::Vector3d a1;
    a1 << x,y,z;
    return a1;
}

Eigen::Vector3d FitlineSide::OLS_Plane(std::vector<Eigen::VectorXd> point)
{
    //Ax = 0的形式，将A, b 写成矩阵的形式
    cv::Mat A((int)point.size(), 3, CV_64F);
    cv::Mat b((int)point.size(), 1, CV_64F);
    //Eigen::MatrixXd A(point.size(),3);
    //Eigen::MatrixXd b(point.size(),1);
//  cout <<"原始点为:"<< point << endl;

    //初始化矩阵A
    for (size_t i = 0; i< point.size(); i++)
    {
        A.at<double>((int)i, 0) = (point[i].x());
        A.at<double>((int)i, 1) = (point[i].y());
        A.at<double>((int)i, 2) = 1;
    }

//  cout << "矩阵A:" << endl << A << endl;

    //初始化矩阵b
    for (size_t i = 0; i< point.size(); i++)
        b.at<double>((int)i, 0) = -(point[i].z());
    //根据线性代数知识，A'* A * x = A' * b 求得的矩阵 x 即为最优解
    //解 x = (A' * A)^-1 * A' * b
    cv::Mat x = -((A.t()*A).inv())*(A.t())*b;
    //cout << x << endl;
    Eigen::Vector3d a ;
    a<<x.at<double>(0,0) ,x.at<double>(1,0),x.at<double>(2, 0);
    return a;
}

std::array<Eigen::VectorXd,2>  FitlineSide::OLS_Line(std::vector<Eigen::VectorXd> point)
{
    int Msize = point[0].size();
    Eigen::MatrixXd I=Eigen::MatrixXd::Identity(Msize,Msize);
    Eigen::MatrixXd X = Eigen::MatrixXd::Zero(Msize,Msize);
    Eigen::VectorXd P = Eigen::VectorXd::Zero(Msize,1);
    //S.setZero():
    //Eigen::Matrix3d t = a*I;
    //Eigen::MatrixXd t1 = point[0]*point[0].adjoint();
    for(Uint32 i = 0; i < point.size() ;i++)
    {
        P+=point[i];
    }
    P/=point.size();
    for(Uint32 i = 0; i < point.size() ;i++)
    {
        Eigen::VectorXd Y = point[i]-P;
        double a = Y.adjoint()*Y;
        X+=(a*I-Y*Y.adjoint());
        //cout << X << endl;
    }
    Eigen::EigenSolver<Eigen::MatrixXd> S(X);
    //求特征值
    Eigen::MatrixXd D = S.eigenvalues().real();
    int r,c;
    D.minCoeff(&r,&c);
    //取最小特征值的特征向量
    Eigen::MatrixXd V = S.eigenvectors().real();
//  cout << D << endl;
//  cout << V << endl;
    Eigen::VectorXd s = V.col(r);
    //return (x-x0)/a=(y-y0)/b=(z-z0)/c
    std::array<Eigen::VectorXd,2> a ;
    a[0] = Eigen::VectorXd::Zero(Msize,1);
    a[1] = Eigen::VectorXd::Zero(Msize,1);
    a[0] << P;
    a[1] << s;
    /*
    a[0](1) = P(1);
    a[0](2) = P(2);
    a[1](0) = s(0);
    a[1](1) = s(1);
    a[1](2) = s(2);
    */
    return a;
}
