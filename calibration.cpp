#include "calibration.h"

Calibration::Calibration()
{

}

Calibration::~Calibration()
{

}

bool Calibration::hand_out_yes_point2RT(std::vector<Eigen::Vector3d> &p1,   //输入眼坐标
                                       std::vector<Eigen::Vector3d> &p2,    //输入手坐标
                                       Eigen::Matrix3d &R,                  //输出旋转矩阵
                                       Eigen::Vector3d &T,                  //输出平移举证
                                       double &err,                         //输出总体精度误差
                                       std::vector<double> &errgroup)       //输出单个精度误差
{
    if(p1.size() != p2.size())
        return false;
    int num = p1.size();

    //求质心
    Eigen::Vector3d temp1(0,0,0),temp2(0,0,0);
    for(int i = 0;i < num;i ++)
    {
        temp1 += p1.at(i);
        temp2 += p2.at(i);
    }
    temp1 /= num;
    temp2 /= num;

    //求缩放因子Z
    double Z = 0;
    double Z1 = 0;
    double Z2 = 0;
    for(int i = 0;i < num;i ++)
    {
        double x1 = p1.at(i)(0) - temp1(0);
        double y1 = p1.at(i)(1) - temp1(1);
        double z1 = p1.at(i)(2) - temp1(2);
        double x2 = p2.at(i)(0) - temp2(0);
        double y2 = p2.at(i)(1) - temp2(1);
        double z2 = p2.at(i)(2) - temp2(2);

        Z1 += x1*x1+y1*y1+z1*z1;
        Z2 += x2*x2+y2*y2+z2*z2;
    }
    Z = Z1/Z2;

    //求 H
    Eigen::Matrix3d H;
    H <<	0, 0, 0,
            0, 0, 0,
            0, 0, 0;
    for(int i = 0;i < num;i ++)
    {
        H += (p1.at(i) - temp1)*(p2.at(i) - temp2).transpose();
    }
    // 进行svd分解
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(H,
                                                 Eigen::ComputeThinU |
                                                 Eigen::ComputeThinV);
    // 构建SVD分解结果
    Eigen::MatrixXd U = svd.matrixU();
    Eigen::MatrixXd V = svd.matrixV();
    Eigen::MatrixXd D = svd.singularValues();


    R = V*U.transpose();
    //行列值
    double hlz = R(0,0)*R(1,1)*R(2,2) +
                 R(0,1)*R(1,2)*R(2,0) +
                 R(0,2)*R(1,0)*R(2,1) -
                 R(0,2)*R(1,1)*R(2,0) -
                 R(0,1)*R(1,0)*R(2,2) -
                 R(0,0)*R(1,2)*R(2,1);
    if(hlz < 0)
    {
        V(0,2) = -V(0,2);
        V(1,2) = -V(1,2);
        V(2,2) = -V(2,2);
        R = V*U.transpose();
    }


    T = -R/(pow(Z,0.5))*temp1 + temp2;
    R = R/((pow(Z,0.5)));


    //计算误差
    std::vector<Eigen::Vector3d> p3;
    double errtemp = 0;
    double et = 0;
    errgroup.resize(num);
    for(int i = 0;i < num;i ++)
    {
        Eigen::Vector3d p3temp = R * p1.at(i) + T;
        et = (p3temp - p2.at(i)).norm();
        errgroup[i]= et;
        errtemp += et;
    }
    err = errtemp / num;
    return true;
}

bool Calibration::hand_on_yes_point2RT(CAL_POSTURE robot,                     //输入姿态标准
                                      RobPos robpos,                         //输入标定点手坐标
                                      std::vector<TCP_Leaserpos> data_group, //输入标定点眼坐标
                                      cv::Mat &matrix_camera2plane,          //输出眼到中间矩阵
                                      cv::Mat &matrix_plane2robot,           //输出中间矩阵到机器人矩阵
                                      double &err,                           //输出总体精度误差
                                      std::vector<double> &errgroup)          //输出单个精度误差

{
    if(false==computeMatrix(robot,robpos,data_group,matrix_camera2plane,matrix_plane2robot))
        return false;
    std::vector<Eigen::Vector3d > org_points;
    std::vector<Eigen::Vector3d > points;
    errgroup.resize(data_group.size());

    for(int n=0;n<data_group.size();n++)
    {
        Eigen::Vector3d point;
        cv::Point2d sen;
        point(0)=data_group[n].robotpos.X;
        point(1)=data_group[n].robotpos.Y;
        point(2)=data_group[n].robotpos.Z;
        org_points.push_back(point);
    }
    for (int i = 0;i < data_group.size();i++) {
        Eigen::Vector3d temp = org_points[i];
        points.push_back(temp);
    }
    for(int n=0;n<data_group.size();n++)
    {
        cv::Point3f testrob;
        /**********************error:计算结果需要小盒子坐标与机器人坐标***************************/
        testrob=ComputePosition(robot,data_group[n],matrix_camera2plane,matrix_plane2robot);
        Eigen::Vector3d testpoint(testrob.x,testrob.y,testrob.z);
        Eigen::Vector3d points_0(robpos.X,robpos.Y,robpos.Z);
        Eigen::Vector3d dis=(points_0-testpoint);
        errgroup[n]=dis.norm();
    }
    err=0;
    for(int n=1;n<data_group.size();n++)
    {
        err=err+errgroup[n];
    }
    err=err/data_group.size();
    return true;
}

bool Calibration::hand_out_yes_eyetohand(Eigen::Vector3d p_eye,     //输入手坐标
                                         Eigen::Matrix3d R,         //输入旋转矩阵
                                         Eigen::Vector3d T,         //输入平移举证
                                         Eigen::Vector3d &p_hand    //输出手坐标
                                         )
{
    p_hand = R * p_eye + T;
    return true;
}

bool Calibration::hand_on_yes_eyetohand (CAL_POSTURE robot,                     //输入姿态标准
                                         TCP_Leaserpos data_group,              //输入标定点眼坐标
                                         cv::Mat &matrix_camera2plane,          //输出眼到中间矩阵
                                         cv::Mat &matrix_plane2robot,           //输出中间矩阵到机器人矩阵
                                         Eigen::Vector3d &p_hand                //输出手坐标
                                        )
{
    cv::Point3f testrob;
    /**********************error:计算结果需要小盒子坐标与机器人坐标***************************/
    testrob=ComputePosition(robot,data_group,matrix_camera2plane,matrix_plane2robot);
    p_hand.x()=testrob.x;
    p_hand.y()=testrob.y,
    p_hand.z()=testrob.z;
    return true;
}

std::vector<Eigen::Vector3d> Calibration::Attitudedifference(CAL_POSTURE robot,Eigen::Vector3d PosR_st,Eigen::Vector3d PosR_ed,int differenceNum)
{
    std::vector<Eigen::Vector3d> tempResult;
    std::vector<Eigen::Vector3d> Result;
    Eigen::Matrix3d _3x3_PosR_st,_3x3_PosR_ed,_3x3_PosR_ed_T,_3x3_PosR_st_T;
    Eigen::Matrix3d addvalue;
    double d_add=1.0/(differenceNum+1);
    //把姿态转化为旋转矩阵
    std::array<double,3> pstst,psted;
    Eigen::Vector3d tm_PosR_st,tm_PosR_ed;
    char b_fan[3]={0,0,0};

    tm_PosR_st=PosR_st;
    tm_PosR_ed=PosR_ed;


    pstst[0]=tm_PosR_st[0];
    pstst[1]=tm_PosR_st[1];
    pstst[2]=tm_PosR_st[2];
    psted[0]=tm_PosR_ed[0];
    psted[1]=tm_PosR_ed[1];
    psted[2]=tm_PosR_ed[2];

    _3x3_PosR_st=Euler2RotMatrixXYZ(robot,pstst);
    _3x3_PosR_ed=Euler2RotMatrixXYZ(robot,psted);

    //旋转矩阵变四元数
    Eigen::Quaterniond quaternion_st,quaternion_ed;
    quaternion_st = _3x3_PosR_st;
    quaternion_ed = _3x3_PosR_ed;

    double pointfilt;
    pointfilt=quaternion_st.x()*quaternion_ed.x()+quaternion_st.y()*quaternion_ed.y()+quaternion_st.z()*quaternion_ed.z()+quaternion_st.w()*quaternion_ed.w();

    if(pointfilt<0)
    {
        quaternion_ed.x()=-quaternion_ed.x();
        quaternion_ed.y()=-quaternion_ed.y();
        quaternion_ed.z()=-quaternion_ed.z();
        quaternion_ed.w()=-quaternion_ed.w();
    }

    for(int i=0;i<differenceNum;i++)
    {
        Eigen::Vector3d sing;
        Eigen::Quaterniond tempaddvalue;
        Eigen::Matrix3d tempres;
        std::array<double, 3> s;
        double dis;
        tempaddvalue=quaternion_st;
        tempaddvalue.x()=quaternion_st.x()*(1-(i+1)*d_add)+(i+1)*d_add*quaternion_ed.x();
        tempaddvalue.y()=quaternion_st.y()*(1-(i+1)*d_add)+(i+1)*d_add*quaternion_ed.y();
        tempaddvalue.z()=quaternion_st.z()*(1-(i+1)*d_add)+(i+1)*d_add*quaternion_ed.z();
        tempaddvalue.w()=quaternion_st.w()*(1-(i+1)*d_add)+(i+1)*d_add*quaternion_ed.w();
        dis=sqrt(tempaddvalue.x()*tempaddvalue.x()+tempaddvalue.y()*tempaddvalue.y()+tempaddvalue.z()*tempaddvalue.z()+tempaddvalue.w()*tempaddvalue.w());
        tempaddvalue.x()=tempaddvalue.x()/dis;
        tempaddvalue.y()=tempaddvalue.y()/dis;
        tempaddvalue.z()=tempaddvalue.z()/dis;
        tempaddvalue.w()=tempaddvalue.w()/dis;
        tempres=tempaddvalue;
        s=RotMatrixXYZ2Euler(robot,tempres);
        sing[0]=s[0]*CAL_ANGLE;
        sing[1]=s[1]*CAL_ANGLE;
        sing[2]=s[2]*CAL_ANGLE;
        tempResult.push_back(sing);
    }
    return tempResult;
}

cv::Point3f Calibration::ComputePosition(CAL_POSTURE robot,TCP_Leaserpos Pic,cv::Mat &matrix_camera2plane,cv::Mat &matrix_plane2robot)
{
    std::vector<cv::Point2d > org_uv;
    org_uv.push_back(cv::Point2d(Pic.leaserpos.Y,Pic.leaserpos.Z));
    std::vector<cv::Point2d > textrobot(1);

    cv::perspectiveTransform(org_uv,textrobot,matrix_camera2plane);
    cv::Mat mps_2_1(2,1,CV_64F);		//uv-light
    cv::Mat mps_3_1(3,1,CV_64F);		//robot-xyz
    //Eigen::Matrix3d posture_matrix;	//robot-rxyz-matrix
    std::array<double,3> posture={Pic.robotpos.RX,Pic.robotpos.RY,Pic.robotpos.RZ};
    mps_2_1.at<double>(0,0) = textrobot[0].x;
    mps_2_1.at<double>(1,0) = textrobot[0].y;

    mps_3_1.at<double>(0,0) = Pic.robotpos.X;
    mps_3_1.at<double>(1,0) = Pic.robotpos.Y;
    mps_3_1.at<double>(2,0) = Pic.robotpos.Z;
    //cv::Mat mat_robot = matrix_plane2robot*mps_3_1;
    Eigen::Matrix3d posture_matrix=Euler2RotMatrixXYZ(robot,posture);
    cv::Mat mat_robot = ComputePos(mps_3_1,mps_2_1,posture_matrix,matrix_plane2robot);
    cv::Point3f robotpos;
    robotpos.x = mat_robot.at<double>(0,0);
    robotpos.y = mat_robot.at<double>(1,0);
    robotpos.z = mat_robot.at<double>(2,0);
    return robotpos;
}

cv::Mat Calibration::ComputePos(cv::Mat Robot_Position,cv::Mat Pic_Position,Eigen::Matrix3d RPY_Rotation,cv::Mat &matrix_plane2robot)
{
    cv::Mat mps_4_4(4,4,CV_64F);
    cv::Mat mps_4_3(4,3,CV_64F);
    cv::Mat mps_3_1(3,1,CV_64F);

    for (int i = 0;i < 3;i++)
    {
        for (int j = 0;j < 3;j++)
        {
            mps_4_4.at<double>(i,j) = RPY_Rotation(i,j);
        }
    }
    mps_4_4.at<double>(0,3) = Robot_Position.at<double>(0,0);
    mps_4_4.at<double>(1,3) = Robot_Position.at<double>(1,0);
    mps_4_4.at<double>(2,3) = Robot_Position.at<double>(2,0);
    mps_4_4.at<double>(3,0) = 0;
    mps_4_4.at<double>(3,1) = 0;
    mps_4_4.at<double>(3,2) = 0;
    mps_4_4.at<double>(3,3) = 1;

    for (int i = 0;i < 3;i++)
    {
        for (int j = 0;j < 3;j++)
        {
            mps_4_3.at<double>(i,j) = matrix_plane2robot.at<double>(i,j);
        }
    }
    mps_4_3.at<double>(3,0) = 0;
    mps_4_3.at<double>(3,1) = 0;
    mps_4_3.at<double>(3,2) = 1;

    mps_3_1.at<double>(0,0) = Pic_Position.at<double>(0,0);
    mps_3_1.at<double>(1,0) = Pic_Position.at<double>(1,0);
    mps_3_1.at<double>(2,0) = 1;

    return cv::Mat(mps_4_4*mps_4_3*mps_3_1);

}


cv::Mat Calibration::OLS_Plane_V(std::vector<Eigen::Vector3d> points)
{
    //Ax = 0的形式，将A, b 写成矩阵的形式
    cv::Mat A(static_cast<int>(points.size()), 3, CV_64F);
    cv::Mat b(static_cast<int>(points.size()), 1, CV_64F);

    //  cout <<"原始点为:"<< point << endl;

    //初始化矩阵A
    for (size_t i = 0; i< points.size(); i++)
    {
        A.at<double>(static_cast<int>(i), 0) = (points[i][0]);
        A.at<double>(static_cast<int>(i), 1) = (points[i][1]);
        A.at<double>(static_cast<int>(i), 2) = 1;
    }

    //  cout << "矩阵A:" << endl << A << endl;

    //初始化矩阵b
    for (size_t i = 0; i< points.size(); i++)
        b.at<double>(static_cast<int>(i), 0) = -(points[i][2]);
    //根据线性代数知识，A'* A * x = A' * b 求得的矩阵 x 即为最优解
    //解 x = (A' * A)^-1 * A' * b
    cv::Mat x = -((A.t()*A).inv())*(A.t())*b;
    //cout << x << endl;
    return x;
}

Eigen::Matrix3d Calibration::ComputeDCM3(std::array<Eigen::Vector3d, 3> &coord_1,std::array<Eigen::Vector3d, 3> &coord_2)
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

cv::Mat Calibration::computePlane2Robot_posture(cv::Mat mp0,cv::Mat mp_r,cv::Mat mp_s,Eigen::Matrix3d pst)
{
    int N = mp_r.cols;

    cv::Mat mps(3,N,CV_64F);
    cv::Mat mps_3_4(3,4,CV_64F);
    cv::Mat mps_4_1(4,1,CV_64F);
    //Eigen::Matrix3d RPY_Rotation;
    Eigen::Matrix3d RPY_Rotation_Inv;
    cv::Mat RPY_Rotation_Inv_Mat(3,3,CV_64F);
    RPY_Rotation_Inv = pst.inverse();

    for (int i = 0;i < 3;i++)
    {
        for (int j = 0;j < 3;j++)
        {
            mps_3_4.at<double>(i,j) = RPY_Rotation_Inv(i,j);
            RPY_Rotation_Inv_Mat.at<double>(i,j) = RPY_Rotation_Inv(i,j);
        }
    }
    mps_4_1.at<double>(0,0) = mp0.at<double>(0,0);
    mps_4_1.at<double>(1,0) = mp0.at<double>(1,0);
    mps_4_1.at<double>(2,0) = mp0.at<double>(2,0);
    mps_4_1.at<double>(3,0) = 1;
    //std::cout << "mps_4_1:"<< mps_4_1 << std::endl;
    for (int i = 0; i < N; i++) {
        mps_3_4.col(3) = -RPY_Rotation_Inv_Mat*mp_r.col(i);
        mps.col(i) = mps_3_4*mps_4_1;
    }
    return computePlane2Robot(mp_s,mps,N);
}

cv::Mat Calibration::computePlane2Robot(cv::Mat plane, cv::Mat robot,int num)
{
    cv::Mat A = cv::Mat::zeros(3*num,9,CV_64F);
    cv::Mat b(3*num,1,CV_64F);
    cv::Mat X(9,1,CV_64F);
    cv::Mat result(3,3,CV_64F);


    //std::cout << "mps:"<< mps << std::endl;
    for (int i = 0; i < num; i++)
    {
      A.at<double>(3*i,0)=plane.at<double>(0, i);
      A.at<double>(3*i,1)=plane.at<double>(1, i);
      A.at<double>(3*i,2)=1;
      A.at<double>(3*i+1,3)=plane.at<double>(0, i);
      A.at<double>(3*i+1,4)=plane.at<double>(1, i);
      A.at<double>(3*i+1,5)=1;
      A.at<double>(3*i+2,6)=plane.at<double>(0, i);
      A.at<double>(3*i+2,7)=plane.at<double>(1, i);
      A.at<double>(3*i+2,8)=1;
      b.at<double>(3*i,0)=robot.at<double>(0, i);
      b.at<double>(3*i+1,0)=robot.at<double>(1, i);
      b.at<double>(3*i+2,0)=robot.at<double>(2, i);
    }

    //cout << A << endl;
    cv::solve(A,b,X,cv::DECOMP_SVD);
    result.at<double>(0,0)=X.at<double>(0, 0);
    result.at<double>(0,1)=X.at<double>(1, 0);
    result.at<double>(0,2)=X.at<double>(2, 0);
    result.at<double>(1,0)=X.at<double>(3, 0);
    result.at<double>(1,1)=X.at<double>(4, 0);
    result.at<double>(1,2)=X.at<double>(5, 0);
    result.at<double>(2,0)=X.at<double>(6, 0);
    result.at<double>(2,1)=X.at<double>(7, 0);
    result.at<double>(2,2)=X.at<double>(8, 0);
    return result;
}

Eigen::Matrix3d Calibration::Euler2RotMatrixX(const double rx)
{
    Eigen::Matrix3d rot;
    double wt = rx *CAL_RADIAN;
    rot(0,0) = 1;
    rot(0,1) = 0;
    rot(0,2) = 0;
    rot(1,0) = 0;
    rot(1,1) = cos(wt);
    rot(1,2) = -sin(wt);
    rot(2,0) = 0;
    rot(2,1) = sin(wt);
    rot(2,2) = cos(wt);
    return rot;

}

Eigen::Matrix3d Calibration::Euler2RotMatrixY(const double ry)
{
    Eigen::Matrix3d rot;
    double wt = ry * CAL_RADIAN;
    rot(0,0) = cos(wt);
    rot(0,1) = 0;
    rot(0,2) = sin(wt);
    rot(1,0) = 0;
    rot(1,1) = 1;
    rot(1,2) = 0;
    rot(2,0) = -sin(wt);
    rot(2,1) = 0;
    rot(2,2) = cos(wt);
    return rot;

}


Eigen::Matrix3d Calibration::Euler2RotMatrixZ(const double rz)
{
    Eigen::Matrix3d rot;
    double wt = rz*CAL_RADIAN;
    rot(0,0) = cos(wt);
    rot(0,1) = -sin(wt);
    rot(0,2) = 0;
    rot(1,0) = sin(wt);
    rot(1,1) = cos(wt);
    rot(1,2) = 0;
    rot(2,0) = 0;
    rot(2,1) = 0;
    rot(2,2) = 1;
    return rot;

}

Eigen::Matrix3d Calibration::Yaskawa_Euler2RotMatrixXYZ(std::array<double,3> pst)
{
    double rx = pst[0],ry = pst[1],rz = pst[2];
    double c1 = cos(rx*CAL_RADIAN);
    double c2 = cos(ry*CAL_RADIAN);
    double c3 = cos(rz*CAL_RADIAN);
    double s1 = sin(rx*CAL_RADIAN);
    double s2 = sin(ry*CAL_RADIAN);
    double s3 = sin(rz*CAL_RADIAN);
    Eigen::Matrix3d rot;
    rot(0,0) = c2*c3;
    rot(0,1) = s1*s2*c3 - c1*s3;
    rot(0,2) = c1*s2*c3 + s1*s3;
    rot(1,0) = c2*s3;
    rot(1,1) = s1*s2*s3 + c1*c3;
    rot(1,2) = c1*s2*s3 - s1*c3;
    rot(2,0) = -s2;
    rot(2,1) = s1*c2;
    rot(2,2) = c1*c2;
    return rot;

}


Eigen::Matrix3d Calibration::Kuka_Euler2RotMatrixXYZ(std::array<double,3> pst)
{
    double rx = pst[0],ry = pst[1],rz = pst[2];
    double c1 = cos(rx*CAL_RADIAN);
    double c2 = cos(ry*CAL_RADIAN);
    double c3 = cos(rz*CAL_RADIAN);
    double s1 = sin(rx*CAL_RADIAN);
    double s2 = sin(ry*CAL_RADIAN);
    double s3 = sin(rz*CAL_RADIAN);
    Eigen::Matrix3d rot;

    rot(0,0) = c1*c2;
    rot(0,1) = c1*s2*s3 - s1*c3;
    rot(0,2) = c1*s2*c3 + s1*s3;
    rot(1,0) = s1*c2;
    rot(1,1) = s1*s2*s3 + c1*c3;
    rot(1,2) = s1*s2*c3 - c1*s3;
    rot(2,0) = -s2;
    rot(2,1) = c2*s3;
    rot(2,2) = c2*c3;
    return rot;

}


Eigen::Matrix3d Calibration::Panasonic_Euler2RotMatrixXYZ(std::array<double,3> pst)
{
    return Euler2RotMatrixZ(pst[0])*Euler2RotMatrixY(90 - pst[1])*Euler2RotMatrixX(pst[2]);

}

Eigen::Matrix3d Calibration::Euler2RotMatrixXYZ(CAL_POSTURE robot,std::array<double,3> pst)
{
    Eigen::Matrix3d posture_matrix;
    switch(robot)
    {
    case CAL_ROBOT_YASKAWA:
        posture_matrix = Yaskawa_Euler2RotMatrixXYZ(pst);
        break;
    case CAL_ROBOT_KUKA:
        posture_matrix = Kuka_Euler2RotMatrixXYZ(pst);
        break;
    case CAL_ROBOT_PANASONIC:
        posture_matrix = Panasonic_Euler2RotMatrixXYZ(pst);
        break;
    }
    return posture_matrix;
}

std::array<double, 3> Calibration::Yaskawa_RotMatrixXYZ2Euler(Eigen::Matrix3d rot_matrix)
{
    double r11,r21,r31,r32,r33;
    r11 = rot_matrix(0,0);
    r21 = rot_matrix(1,0);
    r31 = rot_matrix(2,0);
    r32 = rot_matrix(2,1);
    r33 = rot_matrix(2,2);
    double rx = atan2(r32,r33);
    double ry = atan2(-r31,sqrt(r11*r11 + r21*r21));
    double rz = atan2(r21,r11);

    std::array<double,3> temp;
    temp[0]= rx;
    temp[1]= ry;
    temp[2]= rz;
    return temp;
}

std::array<double, 3> Calibration::Kuka_RotMatrixXYZ2Euler(Eigen::Matrix3d rot_matrix)
{
    double r11,r21,r31,r32,r33;
    r11 = rot_matrix(0,0);
    r21 = rot_matrix(1,0);
    r31 = rot_matrix(2,0);
    r32 = rot_matrix(2,1);
    r33 = rot_matrix(2,2);
    double rx = atan2(r32,r33);
    //double rz = atan2(r21,r11)
    double ry = atan2(-r31,sqrt(r11*r11 + r21*r21));
    double rz = atan2(r21,r11);

    std::array<double,3> temp;
    temp[0]= rz;
    temp[1]= ry;
    temp[2]= rx;
    //return array<double, 3>{rz, ry, rx};
    return temp;
}

std::array<double, 3> Calibration::Panasonic_RotMatrixXYZ2Euler(Eigen::Matrix3d rot_matrix)
{
    double r11,r12,r13,r21,r23,r31,r32,r33;
    r11 = rot_matrix(0,0);
    r12 = rot_matrix(0,1);
    r13 = rot_matrix(0,2);
    r21 = rot_matrix(1,0);
    r23 = rot_matrix(1,2);
    r31 = rot_matrix(2,0);
    r32 = rot_matrix(2,1);
    r33 = rot_matrix(2,2);
    double ry = atan2(sqrt(r31*r31+r32*r32),r33);
    //double rz = atan2(r21,r11)
    double rx,rz;
    if(ry*CAL_ANGLE == 0)
    {
        rx = 0;
        rz = atan2(-r12,r11);
    }
    else if(ry*CAL_ANGLE == 180)
    {
        rx = 0;
        rz = atan2(r12,-r11);
    }
    else
    {
        rx = atan2(r23/sin(ry),r13/sin(ry));
        rz = atan2(r32/sin(ry),-r31/sin(ry));
    }
    std::array<double,3> temp;
    temp[0]= rz;
    temp[1]= ry;
    temp[2]= rx;
    return temp;
}

std::array<double, 3> Calibration::RotMatrixXYZ2Euler(CAL_POSTURE robot,Eigen::Matrix3d rot_matrix)
{
    std::array<double, 3> temp;
    switch(robot)
    {
    case CAL_ROBOT_YASKAWA:
        temp = Yaskawa_RotMatrixXYZ2Euler(rot_matrix);
        break;
    case CAL_ROBOT_KUKA:
        temp = Kuka_RotMatrixXYZ2Euler(rot_matrix);
        break;
    case CAL_ROBOT_PANASONIC:
        temp = Panasonic_RotMatrixXYZ2Euler(rot_matrix);
        break;
    }
    return temp;
}

bool Calibration::computeMatrix(CAL_POSTURE robot,RobPos robpos,std::vector<TCP_Leaserpos> data_group,cv::Mat &matrix_camera2plane,cv::Mat &matrix_plane2robot)
{
    std::vector<Eigen::Vector3d > org_points;
    //vector<Eigen::Vector3d > points;
    std::vector<cv::Point2d> org_sensor;
    std::vector<cv::Point2d> sensor;
    size_t num = data_group.size();

    for(int n=0;n<num;n++)
    {
        Eigen::Vector3d point;
        cv::Point2d sen;
        point(0)=data_group[n].robotpos.X;
        point(1)=data_group[n].robotpos.Y;
        point(2)=data_group[n].robotpos.Z;
        org_points.push_back(point);
        sen.x=data_group[n].leaserpos.Y;
        sen.y=data_group[n].leaserpos.Z;
        org_sensor.push_back(sen);
    }
    if(num<=3)
        return false;

    cv::Mat normal_vector = OLS_Plane_V(org_points);

    Eigen::Vector3d normal (normal_vector.at<double>(0,0)/normal_vector.at<double>(2,0),
    normal_vector.at<double>(1,0)/normal_vector.at<double>(2,0),
    -1.0/normal_vector.at<double>(2,0));
    normal.normalize();

    Eigen::Vector3d a (0,0,1);
    Eigen::Vector3d b (0,1,0);
    Eigen::Vector3d c (1,0,0);
    Eigen::Vector3d rx = a.cross(normal);
    Eigen::Vector3d ry = rx.cross(normal);
    std::array<Eigen::Vector3d,3> tag = {rx,ry,normal};

    std::array<Eigen::Vector3d,3> org = {c,b,a};
    Eigen::Matrix3d mat = ComputeDCM3(tag,org);

    std::vector<cv::Point2f > temppp_uv;
    std::vector<cv::Point2f > point_2;
    for (size_t i = 0; i< org_points.size(); i++)
    {
        Eigen::Vector3d temp;
        temp = mat*org_points[i];

        cv::Point3d temp_p (temp[0],temp[1],0);
        cv::Point2d temp_2 (temp[0],temp[1]);
        cv::Point2d temp_uv (org_sensor[i].x,org_sensor[i].y);
        temppp_uv.push_back(temp_uv);
        point_2.push_back(temp_2);
    }
    matrix_camera2plane = cv::findHomography(temppp_uv,point_2,cv::RANSAC);
    if(matrix_camera2plane.empty())
    {
       matrix_camera2plane=cv::Mat::zeros(3,3,CV_64F);
       return false;
    }

    double first[3] = {robpos.X,robpos.Y,robpos.Z};
    cv::Mat mp0 = cv::Mat(3,1,CV_64F,first);

    int cN = static_cast<int>(point_2.size());
    cv::Mat mp=cv::Mat(3,cN,CV_64F);
    cv::Mat mpi=cv::Mat(2,cN,CV_64F);
    for (int i = 0 ; i < cN ; i++)
    {
        mpi.at<double>(0,i) =  point_2[static_cast<size_t>(i)].x;
        mpi.at<double>(1,i) =  point_2[static_cast<size_t>(i)].y;
        mp.at<double>(0,i) = org_points[static_cast<size_t>(i)][0];
        mp.at<double>(1,i) = org_points[static_cast<size_t>(i)][1];
        mp.at<double>(2,i) = org_points[static_cast<size_t>(i)][2];
    }
    std::array<double,3> posture = {data_group[data_group.size()-1].robotpos.RX,data_group[data_group.size()-1].robotpos.RY,data_group[data_group.size()-1].robotpos.RZ};
    Eigen::Matrix3d posture_matrix=Euler2RotMatrixXYZ(robot,posture);

    matrix_plane2robot=computePlane2Robot_posture(mp0,mp,mpi,posture_matrix);
    return true;
}
