#ifndef CWELDTARJECT_H
#define CWELDTARJECT_H

#include "global.h"
#include "math.h"
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include "calibration.h"

class CWeldTarject
{
public:
    CWeldTarject();
    ~CWeldTarject();

    //机器人控制插值
    bool pos_interpolation(CAL_POSTURE robot,                       //输入姿态标准
                           std::vector<RobPos> posw,                //输入插值前点
                           std::vector<RobPos> &interpolatPos,      //输出插值后点
                           int itp_time=16,                         //两个点之间的插值点的间隔运动时间:16ms为安川控制标准
                           float speed=1);                          //移动速度;
    //机器人控制插值圆弧
    bool pos_circle(CAL_POSTURE robot,                       //输入姿态标准
                    RobPos pos_st,                           //输入插值起点
                    RobPos pos_center,                       //输入途径点
                    RobPos pos_ed,                           //输入终点
                    std::vector<RobPos> &interpolatPos,      //输出插值后点
                    float sampling_interval=0.01,            //采样间隔mm
                    int itp_time=16,                         //两个点之间的插值点的间隔运动时间:16ms为安川控制标准
                    float speed=1);                          //移动速度;

    //创建摆焊轨迹
    int creat_wave(CAL_POSTURE robot,                       //输入姿态标准
                   std::vector<RobPos> pTarject,            //输入原来轨迹
                   wWAVEParam waveparam,                    //输入摆焊信息
                   std::vector<RobPos> *wave_out,           //输出摆焊轨迹
                   double *t_end);                          //输出末尾的摆焊所在周期

    //继续摆焊轨迹
    int creat_wave_continue(CAL_POSTURE robot,                       //输入姿态标准
                            std::vector<RobPos> pTarject,            //输入原来轨迹
                            wWAVEParam waveparam,                    //输入摆焊信息
                            std::vector<RobPos> *wave_out,           //输出摆焊轨迹
                            double t_st,                             //输出上次的摆焊所在周期
                            double *t_end);                          //输出末尾的摆焊所在周期

protected:
    //根据不共线的空间三点计算圆心坐标
    bool getCenterOfCircle(std::vector<Eigen::Vector3d> points,  //输入空间三点
                           Eigen::Vector3d &circlecenter); //输出圆心

    //求输入两坐标系之间的旋转关系（旧坐标系上的点通过R变化后到新坐标系上）
    bool coord2RT(std::vector<Eigen::Vector3d> coor_new,    //新坐标系
                  std::vector<Eigen::Vector3d> coor_old,    //旧坐标系
                  Eigen::Matrix3d &R);                      //旋转关系

    Eigen::Matrix3d ComputeDCM3(std::array<Eigen::Vector3d, 3> &coord_1,std::array<Eigen::Vector3d, 3> &coord_2);
};

#endif // CWELDTARJECT_H
