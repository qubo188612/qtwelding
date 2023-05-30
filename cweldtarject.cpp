#include "cweldtarject.h"

CWeldTarject::CWeldTarject()
{

}

CWeldTarject::~CWeldTarject()
{

}

bool CWeldTarject::pos_interpolation(std::vector<RobPos> posw,std::vector<RobPos> &interpolatPos,int itp_time,float speed)
{
    //清空插值数据内存
    interpolatPos.clear();

    if(posw.size()==0)
        return false;
    if(itp_time==0)
    {
        interpolatPos=posw;
        return true;
    }
    float last_point[3] = {posw[0].X,posw[0].Y,posw[0].Z};
    float temp_pos[3] = {posw[0].X,posw[0].Y,posw[0].Z};
    float pose[3] = {posw[0].RX,posw[0].RY,posw[0].RZ};

    RobPos temp;
    temp.X = last_point[0];
    temp.Y = last_point[1];
    temp.Z = last_point[2];
    temp.RX = pose[0];
    temp.RY = pose[1];
    temp.RZ = pose[2];
    interpolatPos.push_back(temp);

    for(size_t i = 1; i < posw.size(); i++)
    {
        double detax = posw[i].X-last_point[0];
        double detay = posw[i].Y-last_point[1];
        double detaz = posw[i].Z-last_point[2];
        double dist = std::sqrt(detax * detax + detay * detay + detaz * detaz);
        double pos_time = dist*1000000/speed;	//us
        float index = pos_time/(itp_time*1000);
        temp_pos[0] = posw[i].X;
        temp_pos[1] = posw[i].Y;
        temp_pos[2] = posw[i].Z;
        if(index >= 1)
        {
            float interval_x = detax/index;
            float interval_y = detay/index;
            float interval_z = detaz/index;
            for (size_t j = 0; j < index; j++)
            {
                temp_pos[0] = last_point[0] + interval_x * (j+1);
                temp_pos[1] = last_point[1] + interval_y * (j+1);
                temp_pos[2] = last_point[2] + interval_z * (j+1);
                RobPos temp;
                temp.X = temp_pos[0];
                temp.Y = temp_pos[1];
                temp.Z = temp_pos[2];
                temp.RX = pose[0];
                temp.RY = pose[1];
                temp.RZ = pose[2];
                interpolatPos.push_back(temp);
            }
            last_point[0] = temp_pos[0];
            last_point[1] = temp_pos[1];
            last_point[2] = temp_pos[2];
        }
        else
        {

        }
    }
    return true;
}

bool CWeldTarject::getCenterOfCircle(std::vector<Eigen::Vector3d> points,Eigen::Vector3d &circlecenter)
{
    if (points.size() != 3)
    {
        return false;
    }

    double x1 = points[0].x(),
           x2 = points[1].x(),
           x3 = points[2].x();
    double y1 = points[0].y(),
           y2 = points[1].y(),
           y3 = points[2].y();
    double z1 = points[0].z(),
           z2 = points[1].z(),
           z3 = points[2].z();

    double a1 = (y1*z2 - y2*z1 - y1*z3 + y3*z1 + y2*z3 - y3*z2),
           b1 = -(x1*z2 - x2*z1 - x1*z3 + x3*z1 + x2*z3 - x3*z2),
           c1 = (x1*y2 - x2*y1 - x1*y3 + x3*y1 + x2*y3 - x3*y2),
           d1 = -(x1*y2*z3 - x1*y3*z2 - x2*y1*z3 + x2*y3*z1 + x3*y1*z2 - x3*y2*z1);

    double a2 = 2 * (x2 - x1),
           b2 = 2 * (y2 - y1),
           c2 = 2 * (z2 - z1),
           d2 = x1*x1 + y1*y1 + z1*z1 - x2*x2 - y2*y2 - z2*z2;

    double a3 = 2 * (x3 - x1),
           b3 = 2 * (y3 - y1),
           c3 = 2 * (z3 - z1),
           d3 = x1*x1 + y1*y1 + z1*z1 - x3*x3 - y3*y3 - z3*z3;

    double f = (a1*b2*c3 - a1*b3*c2 - a2*b1*c3 + a2*b3*c1 + a3*b1*c2 - a3*b2*c1);

    if(f==0)
    {
        return false;
    }

    double cx = -(b1*c2*d3 - b1*c3*d2 - b2*c1*d3 + b2*c3*d1 + b3*c1*d2 - b3*c2*d1)/f;
    double cy =  (a1*c2*d3 - a1*c3*d2 - a2*c1*d3 + a2*c3*d1 + a3*c1*d2 - a3*c2*d1)/f;
    double cz = -(a1*b2*d3 - a1*b3*d2 - a2*b1*d3 + a2*b3*d1 + a3*b1*d2 - a3*b2*d1)/f;

    circlecenter.x()=cx;
    circlecenter.y()=cy;
    circlecenter.z()=cz;

    return true;
}

bool CWeldTarject::coord2RT(std::vector<Eigen::Vector3d> coor_new,std::vector<Eigen::Vector3d> coor_old,Eigen::Matrix3d &R)
{
    if(coor_new.size()!=3||coor_old.size()!=3)
        return false;

    Eigen::Matrix3d rotation = Eigen::Matrix3d::Zero();

    Eigen::Vector3d i = coor_new[0], j = coor_new[1], k = coor_new[2];
    Eigen::Vector3d x = coor_old[0], y = coor_old[1], z = coor_old[2];

    rotation(0, 0) = i.dot(x) / (i.norm()*x.norm());
    rotation(0, 1) = i.dot(y) / (i.norm()*y.norm());
    rotation(0, 2) = i.dot(z) / (i.norm()*z.norm());
    rotation(1, 0) = j.dot(x) / (j.norm()*x.norm());
    rotation(1, 1) = j.dot(y) / (j.norm()*y.norm());
    rotation(1, 2) = j.dot(z) / (j.norm()*z.norm());
    rotation(2, 0) = k.dot(x) / (k.norm()*x.norm());
    rotation(2, 1) = k.dot(y) / (k.norm()*y.norm());
    rotation(2, 2) = k.dot(z) / (k.norm()*z.norm());


    for (int i = 0 ; i < 3; i++)
    {
        for (int j = 0 ; j < 3; j++)
        {
            R(i,j) = rotation(i,j);
        }
    }
    return true;
}

bool CWeldTarject::pos_circle(CAL_POSTURE robot,RobPos pos_st,RobPos pos_center,RobPos pos_ed,std::vector<RobPos> &interpolatPos,float sampling_interval,int itp_time,float speed)
{
    double radius;//半径
    double rad;//每个扇形弧度
    Eigen::Vector3d circlecenter,circlecenter2;//圆心
    Eigen::Vector3d radius_st;//圆心到起点的向量
    Eigen::Vector3d radius_center;//圆心到途径点的向量
    Eigen::Vector3d radius_ed;//圆心到终点的向量
    std::vector<Eigen::Vector3d> points(3);
    points[0].x()=pos_st.X;
    points[0].y()=pos_st.Y;
    points[0].z()=pos_st.Z;
    points[1].x()=pos_center.X;
    points[1].y()=pos_center.Y;
    points[1].z()=pos_center.Z;
    points[2].x()=pos_ed.X;
    points[2].y()=pos_ed.Y;
    points[2].z()=pos_ed.Z;
    if(false==getCenterOfCircle(points,circlecenter))
    {
        return false;
    }
    radius_st=points[0]-circlecenter;
    radius_center=points[1]-circlecenter;
    radius_ed=points[2]-circlecenter;

    radius=radius_st.norm();
    rad=sampling_interval/radius;

    Eigen::Vector3d cross_center;   //圆法线方向
    cross_center=(points[0]-points[1]).cross((points[2]-points[1]));

    //建立圆坐标系
    Eigen::Vector3d rz=cross_center/cross_center.norm();
    Eigen::Vector3d rx=radius_st/radius_st.norm();
    Eigen::Vector3d ry=rz.cross(rx)/rz.cross(rx).norm();

    //建立基坐标系
    Eigen::Vector3d Z(0,0,1);
    Eigen::Vector3d Y(0,1,0);
    Eigen::Vector3d X(1,0,0);

    Eigen::Matrix3d R,RT;		//旋转缩放矩阵
    Eigen::Vector3d T,TT;		//平移变量
    //计算圆坐标系和基坐标系的关系
    T(0) = -circlecenter.x();
    T(1) = -circlecenter.y();
    T(2) = -circlecenter.z();

    TT(0) = circlecenter.x();
    TT(1) = circlecenter.y();
    TT(2) = circlecenter.z();

    std::vector<Eigen::Vector3d> coord_new(3);
    std::vector<Eigen::Vector3d> coord_old(3);
    coord_new[0]=rx;
    coord_new[1]=ry;
    coord_new[2]=rz;
    coord_old[0]=X;
    coord_old[1]=Y;
    coord_old[2]=Z;

    if(false==coord2RT(coord_old,coord_new,RT))
    {
        return false;
    }

    if(false==coord2RT(coord_new,coord_old,R))
    {
        return false;
    }

    std::vector<Eigen::Vector3d> r_circlepoints(3);//r坐标系下的圆的三个点

    for(int n=0;n<3;n++)
    {
        r_circlepoints[n] = R*(points[n]+T);
    }

    /***********/
    //3维转2维平面前半段
    double _2d_rand_1;//起点到途径点的夹角
    double _2d_rand_2;//起点到终点的夹角
    double cross_temp1,cross_temp2;
    _2d_rand_1=acos((r_circlepoints[0]).dot(r_circlepoints[1])/((r_circlepoints[0]).norm()*(r_circlepoints[1]).norm()));
    _2d_rand_2=acos((r_circlepoints[0]).dot(r_circlepoints[2])/((r_circlepoints[0]).norm()*(r_circlepoints[2]).norm()));
    cross_temp1=(r_circlepoints[0]).cross(r_circlepoints[2]).dot(Z);
    if(cross_temp1<0)
    {      
        _2d_rand_1=2*CV_PI-_2d_rand_1;
    }
    cross_temp2=(r_circlepoints[0]).cross(r_circlepoints[1]).dot(Z);
    if(cross_temp2<0)
    {
        _2d_rand_2=2*CV_PI-_2d_rand_2;
    }
    else if(cross_temp2==0)
    {
        return false;
    }

    int addnum_1=_2d_rand_1/rad;
    int addnum=_2d_rand_2/rad;
    if(addnum<4||addnum_1<4)
        return false;
    std::vector<Eigen::Vector3d> r_circlepoints_out;//r坐标系下的圆的插值点
    r_circlepoints_out.resize(addnum_1);

    if(cross_temp2<0)
    {
        rad=-rad;
    }
    for(int n=0;n<addnum_1;n++)
    {
        double radstep=rad*n;
        Eigen::Vector3d point;
        point.x()=radius*cos(radstep);
        point.y()=radius*sin(radstep);
        point.z()=0;
        r_circlepoints_out[n]=point;
    }
    std::vector<RobPos> head_pos(addnum_1);
    for(int n=0;n<addnum_1;n++)
    {
        r_circlepoints_out[n]=RT*r_circlepoints_out[n]+TT;
        head_pos[n].X=r_circlepoints_out[n].x();
        head_pos[n].Y=r_circlepoints_out[n].y();
        head_pos[n].Z=r_circlepoints_out[n].z();
    }
    std::vector<RobPos> head_interpolatPos;

    if(false==pos_interpolation(head_pos,head_interpolatPos,itp_time,speed))
        return false;

    if(head_interpolatPos.size()<4)
        return false;

    Eigen::Vector3d stR1(pos_st.RX,pos_st.RY,pos_st.RZ);
    Eigen::Vector3d edR1(pos_center.RX,pos_center.RY,pos_center.RZ);
    std::vector<Eigen::Vector3d> posR1=Calibration::Attitudedifference(robot,stR1,edR1,head_interpolatPos.size());
    for(int n=0;n<head_interpolatPos.size();n++)
    {
        head_interpolatPos[n].RX=posR1[n][0];
        head_interpolatPos[n].RY=posR1[n][1];
        head_interpolatPos[n].RZ=posR1[n][2];
    }

    r_circlepoints_out.resize(addnum+2-addnum_1);
    for(int n=addnum_1;n<addnum+1;n++)
    {
        double radstep=rad*n;
        Eigen::Vector3d point;
        point.x()=radius*cos(radstep);
        point.y()=radius*sin(radstep);
        point.z()=0;
        r_circlepoints_out[n-addnum_1]=RT*point+TT;
    }
    r_circlepoints_out[addnum+2-addnum_1-1]=RT*r_circlepoints[2]+TT;
    std::vector<RobPos> tail_pos(addnum+2-addnum_1);
    for(int n=0;n<addnum+2-addnum_1;n++)
    {
        tail_pos[n].X=r_circlepoints_out[n].x();
        tail_pos[n].Y=r_circlepoints_out[n].y();
        tail_pos[n].Z=r_circlepoints_out[n].z();
    }
    std::vector<RobPos> tail_interpolatPos;

    if(false==pos_interpolation(tail_pos,tail_interpolatPos,itp_time,speed))
        return false;

    if(tail_interpolatPos.size()<4)
        return false;

    Eigen::Vector3d stR2(pos_center.RX,pos_center.RY,pos_center.RZ);
    Eigen::Vector3d edR2(pos_ed.RX,pos_ed.RY,pos_ed.RZ);
    std::vector<Eigen::Vector3d> posR2=Calibration::Attitudedifference(robot,stR2,edR2,tail_interpolatPos.size()+1);
    for(int n=0;n<tail_interpolatPos.size();n++)
    {
        tail_interpolatPos[n].RX=posR2[n+1][0];
        tail_interpolatPos[n].RY=posR2[n+1][1];
        tail_interpolatPos[n].RZ=posR2[n+1][2];
    }

    head_interpolatPos.insert(head_interpolatPos.end(),tail_interpolatPos.begin(),tail_interpolatPos.end());
    interpolatPos=head_interpolatPos;

    return true;
}

int CWeldTarject::creat_wave(std::vector<RobPos> pTarject,wWAVEParam waveparam,std::vector<RobPos> *wave_out)
{
    unsigned int num = pTarject.size();
    double bhx = 0,bhy = 0,bhz = 0;

    unsigned int timeGap=waveparam.timeGap;
    unsigned int wvTime;
    unsigned int leftStopTime;
    unsigned int rightStopTime;
    double wvLeftAmp,wvRightAmp;
    double wvLeftAmp_z,wvRightAmp_z;
    double wvAngle1,wvAngle2;
    unsigned int start;
    bool order;
    Pendulum_mode wvType;
    bool bCycleComplate = false;
    Trend_mode trend_mode;

    (*wave_out).clear();

    if(num<=1)
        return -1;

    leftStopTime = static_cast<unsigned int>(waveparam.leftStopTime * 1000);
    rightStopTime = static_cast<unsigned int>(waveparam.rightStopTime * 1000);
    wvTime =  static_cast<unsigned int>(waveparam.period * 1000);
    wvLeftAmp = waveparam.leftAmp;
    wvRightAmp = waveparam.rightAmp;
    wvLeftAmp_z = waveparam.leftAmp_z;
    wvRightAmp_z = waveparam.rightAmp_z;
    wvAngle1 = 2*M_PI*waveparam.anglex/360;
    wvAngle2 = 2*M_PI*waveparam.angley/360;
    wvType = waveparam.pendulum_mode;
    start = waveparam.startPos;
    trend_mode = waveparam.trend_mode;
    if(wvTime <= 0)
        return -1;
    if(waveparam.order == 0)
    {
        order = true;
    }
    else
    {
        order = false;
    }
    Eigen::Vector3d vectorBefore(0,1,0);
    Eigen::Vector3d vectorAfter(pTarject.at(pTarject.size()-1).X - pTarject.at(0).X, pTarject.at(pTarject.size()-1).Y - pTarject.at(0).Y, pTarject.at(pTarject.size()-1).Z - pTarject.at(0).Z);
    std::vector<RobPos> pTarject_out;
    for(unsigned int i = 0;i < num;i ++)
    {
        double t = 0.0;
        if(order)
            t = ((i * timeGap) + (start*wvTime/4)) % wvTime;
        else {
            t = wvTime - ((i * timeGap) + (start*wvTime/4)) % wvTime;
        }
        switch (wvType) {
            case PENDULUM_ID_SIMPLE:
                if(t >=0 && t < wvTime/4)
                {
                    bhx = t * (wvLeftAmp / (wvTime/4));
                    bhy = 0;
                    bhz = t * (wvLeftAmp_z / (wvTime/4));
                }
                else if(t >= wvTime/4 && t < wvTime/2)
                {
                    bhx = wvLeftAmp - (t - wvTime/4)  * (wvLeftAmp / (wvTime/4));
                    bhy = 0;
                    bhz = wvLeftAmp_z - (t - wvTime/4)  * (wvLeftAmp_z / (wvTime/4));
                }
                else if(t >= wvTime/2 && t < wvTime*3/4)
                {
                    bhx = - (t - wvTime/2)* (wvRightAmp / (wvTime/4));
                    bhy = 0;
                    bhz = (t - wvTime/2)* (wvRightAmp_z / (wvTime/4));
                }
                else if(t >= wvTime*3/4 && t < wvTime)
                {
                    bhx = - wvRightAmp + (t - wvTime*3/4) * (wvRightAmp / (wvTime/4));
                    bhy = 0;
                    bhz = wvRightAmp_z - (t - wvTime*3/4) * (wvRightAmp_z / (wvTime/4));
                }
                break;
            case PENDULUM_ID_TRIANGLE:
                if(t >=0 && t < wvTime/4)
                {
                    bhx = t * (wvLeftAmp / (wvTime/4));
                    bhy = 0;
                    bhz = 0;
                }
                else if(t >= wvTime/4 && t < wvTime/2)
                {
                    bhx = wvLeftAmp - (t - wvTime/4)  * (wvLeftAmp / (wvTime/4));
                    bhy = 0;
                    bhz = - (t - wvTime/4)  * (wvLeftAmp / (wvTime/4));
                }
                else if(t >= wvTime/2 && t < wvTime*3/4)
                {
                    bhx = - (t - wvTime/2)* (wvLeftAmp / (wvTime/4));
                    bhy = 0;
                    bhz = - wvLeftAmp + (t - wvTime/2)  * (wvLeftAmp / (wvTime/4));
                }
                else if(t >= wvTime*3/4 && t < wvTime)
                {
                    bhx = - wvLeftAmp + (t - wvTime*3/4) * (wvLeftAmp / (wvTime/4));
                    bhy = 0;
                    bhz = 0;
                }
                break;
            case PENDULUM_ID_L:
                if(t >=0 && t < wvTime/4)
                {
                    bhx = t * (wvLeftAmp / (wvTime/4)) * sin((wvAngle1/2)*M_PI/180);
                    bhy = 0;
                    bhz = t * (wvLeftAmp / (wvTime/4)) * cos((wvAngle1/2)*M_PI/180);
                }
                else if(t >= wvTime/4 && t < wvTime/2)
                {
                    bhx = (wvLeftAmp - (t - wvTime/4)  * (wvLeftAmp / (wvTime/4))) * sin((wvAngle1/2)*M_PI/180);
                    bhy = 0;
                    bhz = (wvLeftAmp - (t - wvTime/4)  * (wvLeftAmp / (wvTime/4))) * cos((wvAngle1/2)*M_PI/180);
                }
                else if(t >= wvTime/2 && t < wvTime*3/4)
                {
                    bhx = - (t - wvTime/2) * (wvRightAmp / (wvTime/4)) * sin((wvAngle1/2)*M_PI/180);
                    bhy = 0;
                    bhz = (t - wvTime/2) * (wvRightAmp / (wvTime/4)) * cos((wvAngle1/2)*M_PI/180);
                }
                else if(t >= wvTime*3/4 && t < wvTime)
                {
                    bhx = (- wvRightAmp + (t - wvTime*3/4) * (wvRightAmp / (wvTime/4))) * sin((wvAngle1/2)*M_PI/180);
                    bhy = 0;
                    bhz = (wvRightAmp - (t - wvTime*3/4) * (wvRightAmp / (wvTime/4))) * cos((wvAngle1/2)*M_PI/180);
                }
                break;
            case PENDULUM_ID_SINE:
                if(t >=0 && t < wvTime/4)
                {
                    bhx = wvLeftAmp*sin(2*M_PI*t/wvTime);
                    bhy = 0;
                    bhz = 0;
                }
                else if(t >= wvTime/4 && t < wvTime/2)
                {
                    bhx = wvLeftAmp*sin(2*M_PI*t/wvTime);
                    bhy = 0;
                    bhz = 0;
                }
                else if(t >= wvTime/2 && t < wvTime*3/4)
                {
                    bhx = wvRightAmp*sin(2*M_PI*t/wvTime);
                    bhy = 0;
                    bhz = 0;
                }
                else if(t >= wvTime*3/4 && t < wvTime)
                {
                    bhx = wvRightAmp*sin(2*M_PI*t/wvTime);
                    bhy = 0;
                    bhz = 0;
                }
                break;
            case PENDULUM_ID_CIRCULAR:
             //   double width = weldWidth[i];
             //   wvLeftAmp = (width - 1 + 2)/2;
                //width + zhongxin = amp + 1
                if(order)
                {
                    bhx = wvLeftAmp*cos(2*M_PI*t/wvTime);
                }
                else
                {
                    bhx = wvLeftAmp*cos(2*M_PI*t/wvTime);
                }
                bhy = wvRightAmp*sin(2*M_PI*t/wvTime);
                bhz = 0;
                break;
        }
        Eigen::Vector3d baifu_1,baifu_2;
        if(trend_mode==TREND_MODE_Y_Z)//X轴走向,开口朝Z轴正方向
        {
            baifu_1(0)=0;
            baifu_1(1)=-1;
            baifu_1(2)=0;
            baifu_2(0)=0;
            baifu_2(1)=1;
            baifu_2(2)=1;
        }
        else if(trend_mode==TREND_MODE_X_Z)//Y轴走向,开口朝Z轴正方向
        {
            baifu_1(0)=1;
            baifu_1(1)=0;
            baifu_1(2)=0;
            baifu_2(0)=-1;
            baifu_2(1)=0;
            baifu_2(2)=1;
        }
        else if(trend_mode==TREND_MODE_Z_Y)//Z轴走向,开口朝Y轴负方向
        {
            baifu_1(0)=1;
            baifu_1(1)=0;
            baifu_1(2)=0;
            baifu_2(0)=0;
            baifu_2(1)=-1;
            baifu_2(2)=0;
        }

        Eigen::Vector3d point(bhx,bhy,bhz);
        Eigen::Vector3d userPoint(0,0,0);
        //坐标转换：默认摆焊坐标系->用户摆焊坐标系 (旋转，默认摆焊以Y轴为行进方向，用户只可绕Y轴旋转)
        Eigen::AngleAxisd rollAngle(Eigen::AngleAxisd(0,Eigen::Vector3d::UnitX()));
        Eigen::AngleAxisd pitchAngle(Eigen::AngleAxisd(wvAngle2*M_PI/180,Eigen::Vector3d::UnitY()));
        Eigen::AngleAxisd yawAngle(Eigen::AngleAxisd(0,Eigen::Vector3d::UnitZ()));
        Eigen::Matrix3d rotMatrix;
        rotMatrix=yawAngle*pitchAngle*rollAngle;
        userPoint = rotMatrix*point;
        Eigen::Vector3d base_x(1,0,0);
        Eigen::Vector3d base_y(0,1,0);
        Eigen::Vector3d base_z(0,0,1);
        baifu_1.normalize();
        baifu_2.normalize();
        Eigen::Vector3d wave_x = baifu_1;	//左摆幅
        Eigen::Vector3d wave_z = baifu_2;	//右摆幅
        //Eigen::Vector3d wave_z(-1,0,0);	//右摆幅
        //vectorAfter.normalize();
        //Eigen::Vector3d wave_y = vectorAfter;
        //Eigen::Vector3d wave_x(wave_y[1],-wave_y[0],0);
        //Eigen::Vector3d wave_x = vectorAfter.cross(wave_z);	//左摆幅
        Eigen::Vector3d wave_y = wave_z.cross(wave_x);
        wave_z = wave_x.cross(wave_y);
        //wave_x = wave_y.cross(wave_z);
        std::array<Eigen::Vector3d ,3 > base;
        std::array<Eigen::Vector3d ,3 > wave;
        base[0] = base_x;
        base[1] = base_y;
        base[2] = base_z;
        wave[0] = wave_x;
        wave[1] = wave_y;
        wave[2] = wave_z;
        rotMatrix = ComputeDCM3(base,wave);
        //坐标转换：用户摆焊坐标系->基坐标系
        //rotMatrix = Eigen::Quaterniond::FromTwoVectors(vectorBefore, vectorAfter).toRotationMatrix();
        Eigen::Vector3d wavePoint(0,0,0);
        Eigen::Vector3d trans(pTarject[i].X,pTarject[i].Y,pTarject[i].Z);
        wavePoint = rotMatrix*userPoint + trans;
        pTarject[i].X = wavePoint.x();
        pTarject[i].Y = wavePoint.y();
        pTarject[i].Z = wavePoint.z();

        pTarject_out.push_back(pTarject[i]);
        if(t > wvTime/4 - timeGap/2 && t < wvTime/4 + timeGap/2)
        {
            int pushNum = leftStopTime/timeGap;
            for(int push_i = 0;push_i < pushNum;push_i ++)
            {
                pTarject_out.push_back(pTarject[i]);
            }
        }
        else if(t > wvTime*3/4 - timeGap/2 && t < wvTime*3/4 + timeGap/2)
        {
            int pushNum = rightStopTime/timeGap;
            for(int push_i = 0;push_i < pushNum;push_i ++)
            {
                pTarject_out.push_back(pTarject[i]);
            }
        }
    }
    *wave_out=pTarject_out;
    return 0;
}

Eigen::Matrix3d CWeldTarject::ComputeDCM3(std::array<Eigen::Vector3d, 3> &coord_1,std::array<Eigen::Vector3d, 3> &coord_2)
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

