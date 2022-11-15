#ifndef FITLINESIDE_H
#define FITLINESIDE_H

#include <Eigen/Core>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <global.h>
#include "tistdtypes.h"

class FitlineSide
{   
public:
    FitlineSide();
    ~FitlineSide();

    Eigen::Vector3d computePointOfLineAndSurface(std::vector<Eigen::VectorXd> linePoints,std::vector<Eigen::VectorXd> surfacePoints);	//求一直线与一个面的交点，linePoints输入直线点集，surfacePoints输入面点集

protected:
    Eigen::Vector3d OLS_Plane(std::vector<Eigen::VectorXd> point);
    std::array<Eigen::VectorXd,2>  OLS_Line(std::vector<Eigen::VectorXd> point);
};

#endif // FITLINESIDE_H
