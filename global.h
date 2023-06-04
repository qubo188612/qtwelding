#ifndef GLOBAL_H
#define GLOBAL_H

#include "tutorial_interfaces/msg/if_algorhmitcloud.hpp"
#include "QString"
#include <Eigen/Core>
#include <Eigen/Dense>
#include <set>


//#define WINDOWS_TCP 1   //linux仿windowstcp测试
#define USE_MYROBOT_CONTROL         1//使用本地的机器人控制协议
//#define USE_MYMOVEC_CONTROL       1//使用本地moveC插值运动(适用于无曲线moveC移动的机器人)
#define OPEN_SHOW_ROBOTSOCKDATA   1//显示与机器人通信内容
//#define OPEN_SHOW_WELDSOCKDATA    1//显示与焊机通信内容(在非机器人直连时)


#if _MSC_VER
#include "tistdtypes.h"
#include <WinSock2.h>

typedef uint8_t u_int8_t; /* u_int8_t is defined in <machine/types.h> */
typedef uint16_t u_int16_t; /* u_int16_t is defined in <machine/types.h> */
typedef uint32_t u_int32_t; /* u_int32_t is defined in <machine/types.h> */
typedef uint64_t u_int64_t; /* u_int64_t is defined in <machine/types.h> */
#else
#include <arpa/inet.h>
#endif

#define ROBOT_SPEED_DECIMAL_PLACE           3       //机器人速度保留小数后几位
#define ROBOT_POSE_DECIMAL_PLACE            3       //机器人坐标保留小数后几位
#define ROBOT_POSTURE_DECIMAL_PLACE         4       //机器人姿态保留小数点后几位

#define ROBOT_SPEED_DECIMAL_BOTTOM          0         //机器人速度最小值
#define ROBOT_SPEED_DECIMAL_TOP             9999      //机器人速度最大值
#define ROBOT_POSE_DECIMAL_BOTTOM          -9999      //机器人坐标最小值
#define ROBOT_POSE_DECIMAL_TOP              9999      //机器人坐标最大值
#define ROBOT_POSTURE_DECIMAL_BOTTOM       -999       //机器人姿态最小值
#define ROBOT_POSTURE_DECIMAL_TOP           999       //机器人姿态最大值


#define ROBOT_POSE_MOVEC_STEP               0.2     //机器人圆弧运动采样间隔

#define DEBUG_MYINTERFACES          //显示算法结果或者算法原图开关
#define DEBUS_SSH                   //ssh升级方式开关

#define CAMBUILD_IMAGE_WIDTH  1024
#define CAMBUILD_IMAGE_HEIGHT 1536

#define CAMTOTALNUM                 1   //最多相机路数
#define ROBOTTALNUM                 1   //最多机器人路数

#define ROBOTTCPNUM                 10  //每个机器人最多TCP上限

#define CLOULD_POINT_NOTDATE        FLT_MAX

#define ROBOTOUTPUTNUM              8   //机器人IO输出口总数
#define ROBOTINPUTNUM               8   //机器人IO输入口总数
#define ROBOTAOUTPUTNUM             4   //机器人模拟量输出口总数

#define ROBOTTCPPOSOUTNUM           3   //机器人外部轴总数

/*************************/

#define ROWS_PROPORTION              1.0
#define COLS_PROPORTION              1.0

#define DEEPIMG_CALLBACKNUM_DNUM     5

#define DO_NOTHING         0
#define DO_WRITE_TASK      1

#define ROB_WORK_DELAY          100000       //机器人通讯反映时间(微秒)
#define ROB_WORK_DELAY_STEP     10000        //机器人每步循环等待时间(微秒)

/*****************************/
//自带机器人控制接口
#define PORT_ROB_MYSELF_CONTROL_IP            "127.0.0.1"
#define PORT_ROB_MYSELF_CONTROL_PORT          1496
/******************************/

//机器人信息和指令传输端口                        (1477-1496)
#define ROB_CONTROL_REG_TOTALNUM              0x500         //机器人控制器寄存器总个数


#define ROB_X_POS_FH_REG_ADD                  0x0000       //机器人当前坐标
#define ROB_X_POS_FL_REG_ADD                  0x0001
#define ROB_Y_POS_FH_REG_ADD                  0x0002
#define ROB_Y_POS_FL_REG_ADD                  0x0003
#define ROB_Z_POS_FH_REG_ADD                  0x0004
#define ROB_Z_POS_FL_REG_ADD                  0x0005
#define ROB_RX_POS_FH_REG_ADD                 0x0006
#define ROB_RX_POS_FL_REG_ADD                 0x0007
#define ROB_RY_POS_FH_REG_ADD                 0x0008
#define ROB_RY_POS_FL_REG_ADD                 0x0009
#define ROB_RZ_POS_FH_REG_ADD                 0x000a
#define ROB_RZ_POS_FL_REG_ADD                 0x000b
#define ROB_SPEED_FH_REG_ADD                  0x000c        //机器人当前速度
#define ROB_SPEED_FL_REG_ADD                  0x000d
#define ROB_STATE_REG_ADD                     0x000e        //机器人当前状态
#define ROB_TIME_HOURS_REG_ADD                0x000f        //当前时间时
#define ROB_TIME_MINUTES_REG_ADD              0x0010        //当前时间分
#define ROB_TIME_SECONDS_REG_ADD              0x0011        //当前时间秒
#define ROB_TIME_MILLISECONDS_REG_ADD         0x0012        //当前时间毫秒
#define ROB_IO_INPUT1_REG_ADD                 0x0013        //机器人IO输入口1
#define ROB_IO_INPUT2_REG_ADD                 0x0014        //机器人IO输入口2
#define ROB_IO_INPUT3_REG_ADD                 0x0015        //机器人IO输入口3
#define ROB_IO_INPUT4_REG_ADD                 0x0016        //机器人IO输入口4
#define ROB_IO_INPUT5_REG_ADD                 0x0017        //机器人IO输入口5
#define ROB_IO_INPUT6_REG_ADD                 0x0018        //机器人IO输入口6
#define ROB_IO_INPUT7_REG_ADD                 0x0019        //机器人IO输入口7
#define ROB_IO_INPUT8_REG_ADD                 0x001a        //机器人IO输入口8
#define ROB_OUT1_POS_FH_REG_ADD               0x001b        //机器人外部轴1
#define ROB_OUT1_POS_FL_REG_ADD               0x001c        //机器人外部轴1
#define ROB_OUT2_POS_FH_REG_ADD               0x001d        //机器人外部轴2
#define ROB_OUT2_POS_FL_REG_ADD               0x001e        //机器人外部轴2
#define ROB_OUT3_POS_FH_REG_ADD               0x001f        //机器人外部轴3
#define ROB_OUT3_POS_FL_REG_ADD               0x0020        //机器人外部轴3
#define ROB_OUTNUM_REG_ADD                    0x0021        //机器人外部轴数量

#define ROB_IO_OUTPUT1_REG_ADD                0x0050        //机器人IO输出口1
#define ROB_IO_OUTPUT2_REG_ADD                0x0051        //机器人IO输出口2
#define ROB_IO_OUTPUT3_REG_ADD                0x0052        //机器人IO输出口3
#define ROB_IO_OUTPUT4_REG_ADD                0x0053        //机器人IO输出口4
#define ROB_IO_OUTPUT5_REG_ADD                0x0054        //机器人IO输出口5
#define ROB_IO_OUTPUT6_REG_ADD                0x0055        //机器人IO输出口6
#define ROB_IO_OUTPUT7_REG_ADD                0x0056        //机器人IO输出口7
#define ROB_IO_OUTPUT8_REG_ADD                0x0057        //机器人IO输出口8
#define ROB_A_OUTPUT1_FH_REG_ADD              0x0058        //机器人模拟量输出口1(A1FH)
#define ROB_A_OUTPUT1_FL_REG_ADD              0x0059        //机器人模拟量输出口1(A1FL)
#define ROB_A_OUTPUT2_FH_REG_ADD              0x005a        //机器人模拟量输出口2(A2FH)
#define ROB_A_OUTPUT2_FL_REG_ADD              0x005b        //机器人模拟量输出口2(A2FL)
#define ROB_A_OUTPUT3_FH_REG_ADD              0x005c        //机器人模拟量输出口3(A3FH)
#define ROB_A_OUTPUT3_FL_REG_ADD              0x005d        //机器人模拟量输出口3(A3FL)
#define ROB_A_OUTPUT4_FH_REG_ADD              0x005e        //机器人模拟量输出口4(A4FH)
#define ROB_A_OUTPUT4_FL_REG_ADD              0x005f        //机器人模拟量输出口4(A4FL)

#define ROB_MODEL_REG_ADD                     0x0100        //机器人型号
#define ROB_TCP_NUM_REG_ADD                   0x0101        //TCP坐标系
#define ROB_MOVEMOD_REG_ADD                   0x0102        //移动模式
#define ROB_MOVEFIER_REG_ADD                  0x0103        //是否起弧移动
#define ROB_MOVESPEED_FH_REG_ADD              0x0104        //移动速度
#define ROB_MOVESPEED_FL_REG_ADD              0x0105
#define ROB_MOVE_X_POS_FH_REG_ADD             0x0106        //移动终点
#define ROB_MOVE_X_POS_FL_REG_ADD             0x0107
#define ROB_MOVE_Y_POS_FH_REG_ADD             0x0108
#define ROB_MOVE_Y_POS_FL_REG_ADD             0x0109
#define ROB_MOVE_Z_POS_FH_REG_ADD             0x010a
#define ROB_MOVE_Z_POS_FL_REG_ADD             0x010b
#define ROB_MOVE_RX_POS_FH_REG_ADD            0x010c
#define ROB_MOVE_RX_POS_FL_REG_ADD            0x010d
#define ROB_MOVE_RY_POS_FH_REG_ADD            0x010e
#define ROB_MOVE_RY_POS_FL_REG_ADD            0x010f
#define ROB_MOVE_RZ_POS_FH_REG_ADD            0x0110
#define ROB_MOVE_RZ_POS_FL_REG_ADD            0x0111
#define ROB_MOVE_X1_POS_FH_REG_ADD            0x0112        //移动终点1,MOVEC时有效
#define ROB_MOVE_X1_POS_FL_REG_ADD            0x0113
#define ROB_MOVE_Y1_POS_FH_REG_ADD            0x0114
#define ROB_MOVE_Y1_POS_FL_REG_ADD            0x0115
#define ROB_MOVE_Z1_POS_FH_REG_ADD            0x0116
#define ROB_MOVE_Z1_POS_FL_REG_ADD            0x0117
#define ROB_MOVE_RX1_POS_FH_REG_ADD           0x0118
#define ROB_MOVE_RX1_POS_FL_REG_ADD           0x0119
#define ROB_MOVE_RY1_POS_FH_REG_ADD           0x011a
#define ROB_MOVE_RY1_POS_FL_REG_ADD           0x011b
#define ROB_MOVE_RZ1_POS_FH_REG_ADD           0x011c
#define ROB_MOVE_RZ1_POS_FL_REG_ADD           0x011d
#define ROB_MOVE_OUT_1_POS_FH_REG_ADD         0x011e        //机器人外部轴1
#define ROB_MOVE_OUT_1_POS_FL_REG_ADD         0x011f        //机器人外部轴1
#define ROB_MOVE_OUT_2_POS_FH_REG_ADD         0x0120        //机器人外部轴2
#define ROB_MOVE_OUT_2_POS_FL_REG_ADD         0x0121        //机器人外部轴2
#define ROB_MOVE_OUT_3_POS_FH_REG_ADD         0x0122        //机器人外部轴3
#define ROB_MOVE_OUT_3_POS_FL_REG_ADD         0x0123        //机器人外部轴3
#define ROB_MOVE_OUT1_1_POS_FH_REG_ADD        0x0124        //机器人外部轴1-1   //移动终点1,MOVEC时有效
#define ROB_MOVE_OUT1_1_POS_FL_REG_ADD        0x0125        //机器人外部轴1-1
#define ROB_MOVE_OUT1_2_POS_FH_REG_ADD        0x0126        //机器人外部轴1-2
#define ROB_MOVE_OUT1_2_POS_FL_REG_ADD        0x0127        //机器人外部轴1-2
#define ROB_MOVE_OUT1_3_POS_FH_REG_ADD        0x0128        //机器人外部轴1-3
#define ROB_MOVE_OUT1_3_POS_FL_REG_ADD        0x0129        //机器人外部轴1-3

#define ROB_WELD_CURRENT_FH_REG_ADD           0x0200        //焊接电流
#define ROB_WELD_CURRENT_FL_REG_ADD           0x0201
#define ROB_WELD_CURRENTMOD_REG_ADD           0x0202        //交变电流
#define ROB_WELD_VOLTAGE_FH_REG_ADD           0x0203        //焊接电压
#define ROB_WELD_VOLTAGE_FL_REG_ADD           0x0204
#define ROB_WELD_SWINGWIDTH_FH_REG_ADD        0x0205        //摆焊摆幅
#define ROB_WELD_SWINGWIDTH_FL_REG_ADD        0x0206
#define ROB_WELD_SWINGFREQUENCY_FH_REG_ADD    0x0207        //摆焊频率
#define ROB_WELD_SWINGFREQUENCY_FL_REG_ADD    0x0208
#define ROB_WELD_SWINGMOD_REG_ADD             0x0209        //摆焊模式

#define ROB_WELD_MODEL_REG_ADD                0x0250        //焊机型号
#define ROB_WELD_IPADDR_1_REG_ADD             0x0251        //焊机远程IP
#define ROB_WELD_IPADDR_2_REG_ADD             0x0252        //焊机远程IP
#define ROB_WELD_IPADDR_3_REG_ADD             0x0253        //焊机远程IP
#define ROB_WELD_IPADDR_4_REG_ADD             0x0254        //焊机远程IP

#define ROB_IPADDR_1_REG_ADD                  0x0300        //远程IP
#define ROB_IPADDR_2_REG_ADD                  0x0301        //远程IP
#define ROB_IPADDR_3_REG_ADD                  0x0302        //远程IP
#define ROB_IPADDR_4_REG_ADD                  0x0303        //远程IP

#define ROB_STOP_REG_ADD                      0x0400        //急停

/*****************************/
//相机任务号文件查询TCP传输端口
#define PORT_ALSTCP_FTP                       1476
//相机原图TCP传输端口
#define PORT_ALSTCP_CAMIMAGE                  1497
//相机处理结果图TCP传输端口
#define PORT_ALSTCP_CAMIMAGE_RESULT           1498
//点云轨迹处理结果TCP传输端口
#define PORT_ALSTCP_POINTCLOUDS_RESULT        1499

/*****************************/
//激光头算法参数寄存器
#define PORT_ALS_PARAMETER                 1500  //端口号1500: 激光头参数端口号

#define TASKNUM_START   100         //激光头框架2起始任务号


#define ALS100_EXPOSURE_TIME_REG_ADD           0x0000
#define ALS100_PINGJUN_REG_ADD                 0x0001
#define ALS100_B_YANMOFUZHU_REG_ADD            0x0002
#define ALS100_B_GUDINGQUYU_REG_ADD            0x0003
#define ALS100_WIDTHLIANTONGDIS_REG_ADD        0x0004
#define ALS100_HIGHLIANTONGDIS_REG_ADD         0x0005
#define ALS100_GUJIAERZHI_REG_ADD              0x0006
#define ALS100_JIGUANGHIGHT_REG_ADD            0x0007
#define ALS100_JIGUANGLONG_REG_ADD             0x0008
#define ALS100_JIGUANGKUANDU_REG_ADD           0x0009
#define ALS100_UPDIF_REG_ADD                   0x000a
#define ALS100_UPDIFMIN_REG_ADD                0x000b
#define ALS100_UPLONG_REG_ADD                  0x000c
#define ALS100_DOWNDIF_REG_ADD                 0x000d
#define ALS100_DOWNDIFMIN_REG_ADD              0x000e
#define ALS100_DOWNDLONG_REG_ADD               0x000f
#define ALS100_DUANXIANERZHI_REG_ADD           0x0010
#define ALS100_ERZHISIZE_REG_ADD               0x0011
#define ALS100_ERZHISIZE2_REG_ADD              0x0012
#define ALS100_SEARCHDECTANCEMAX_REG_ADD       0x0013
#define ALS100_SEARCHDECTANCEMIN_REG_ADD       0x0014
#define ALS100_DIS_CENTER_ST_REG_ADD           0x0015
#define ALS100_DIS_CENTER_ED_REG_ADD           0x0016
#define ALS100_ANSWERPOINT_REG_ADD             0x0017
#define ALS100_B_KALMANFILTER_REG_ADD          0x0018
#define ALS100_KALMANQF_REG_ADD                0x0019
#define ALS100_KALMANRF_REG_ADD                0x001a


#define ALS100_REG_TOTALNUM                    0x001b

#define ALS100_INIT_REG_ADD                    0x001f



#define ALS101_EXPOSURE_TIME_REG_ADD           0x0020
#define ALS101_PINGJUN_REG_ADD                 0x0021
#define ALS101_B_YANMOFUZHU_REG_ADD            0x0022
#define ALS101_B_GUDINGQUYU_REG_ADD            0x0023
#define ALS101_WIDTHLIANTONGDIS_REG_ADD        0x0024
#define ALS101_HIGHLIANTONGDIS_REG_ADD         0x0025
#define ALS101_GUJIAERZHI_REG_ADD              0x0026
#define ALS101_JIGUANGHIGHT_REG_ADD            0x0027
#define ALS101_JIGUANGLONG_REG_ADD             0x0028
#define ALS101_JIGUANGKUANDU_REG_ADD           0x0029
#define ALS101_UPDIF_REG_ADD                   0x002a
#define ALS101_UPDIFMIN_REG_ADD                0x002b
#define ALS101_UPLONG_REG_ADD                  0x002c
#define ALS101_DOWNDIF_REG_ADD                 0x002d
#define ALS101_DOWNDIFMIN_REG_ADD              0x002e
#define ALS101_DOWNDLONG_REG_ADD               0x002f
#define ALS101_DUANXIANERZHI_REG_ADD           0x0030
#define ALS101_ERZHISIZE_REG_ADD               0x0031
#define ALS101_ERZHISIZE2_REG_ADD              0x0032
#define ALS101_SEARCHDECTANCEMAX_REG_ADD       0x0033
#define ALS101_SEARCHDECTANCEMIN_REG_ADD       0x0034
#define ALS101_DIS_CENTER_ST_REG_ADD           0x0035
#define ALS101_DIS_CENTER_ED_REG_ADD           0x0036
#define ALS101_ANSWERPOINT_REG_ADD             0x0037
#define ALS101_B_KALMANFILTER_REG_ADD          0x0038
#define ALS101_KALMANQF_REG_ADD                0x0039
#define ALS101_KALMANRF_REG_ADD                0x003a


#define ALS101_REG_TOTALNUM                    0x001b

#define ALS101_INIT_REG_ADD                    0x003f


#define ALS102_EXPOSURE_TIME_REG_ADD           0x0040
#define ALS102_PINGJUN_REG_ADD                 0x0041
#define ALS102_B_YANMOFUZHU_REG_ADD            0x0042
#define ALS102_B_GUDINGQUYU_REG_ADD            0x0043
#define ALS102_WIDTHLIANTONGDIS_REG_ADD        0x0044
#define ALS102_HIGHLIANTONGDIS_REG_ADD         0x0045
#define ALS102_GUJIAERZHI_REG_ADD              0x0046
#define ALS102_JIGUANGHIGHT_REG_ADD            0x0047
#define ALS102_JIGUANGLONG_REG_ADD             0x0048
#define ALS102_JIGUANGKUANDU_REG_ADD           0x0049
#define ALS102_UPDIF_REG_ADD                   0x004a
#define ALS102_UPDIFMIN_REG_ADD                0x004b
#define ALS102_UPLONG_REG_ADD                  0x004c
#define ALS102_DOWNDIF_REG_ADD                 0x004d
#define ALS102_DOWNDIFMIN_REG_ADD              0x004e
#define ALS102_DOWNDLONG_REG_ADD               0x004f
#define ALS102_ST_DOWN_REG_ADD                 0x0050
#define ALS102_ED_DOWN_REG_ADD                 0x0051
#define ALS102_ST_UP_REG_ADD                   0x0052
#define ALS102_ED_UP_REG_ADD                   0x0053
#define ALS102_UPDIF2_REG_ADD                  0x0054
#define ALS102_UPDIFMIN2_REG_ADD               0x0055
#define ALS102_DIS_CENTER_ST_REG_ADD           0x0056
#define ALS102_DIS_CENTER_ED_REG_ADD           0x0057
#define ALS102_B_OPENGUDINGDIMIAN_REG_ADD      0x0058
#define ALS102_DIMIANPANGDINGJULI_REG_ADD      0x0059
#define ALS102_DIMIANPINGJUNSHUNUM_REG_ADD     0x005a
#define ALS102_DIS_CENTER_ST2_REG_ADD          0x005b
#define ALS102_DIS_CENTER_ED2_REG_ADD          0x005c
#define ALS102_DIS_CENTER_ST3_REG_ADD          0x005d
#define ALS102_DIS_CENTER_ED3_REG_ADD          0x005e
#define ALS102_XUEXIJULI_REG_ADD               0x005f
#define ALS102_B_PINGPOWENGDING_REG_ADD        0x0060
#define ALS102_PINGPOWENGDING_DIS_REG_ADD      0x0061
#define ALS102_B_XIELVOPEN_REG_ADD             0x0062
#define ALS102_XIELVFANWEI_REG_ADD             0x0063
#define ALS102_UPLONG2_REG_ADD                 0x0064
#define ALS102_CEBANKONGDONGDIS_REG_ADD        0x0065
#define ALS102_QIATOUQUWEI_REG_ADD             0x0066
#define ALS102_ANSWERPOINT_REG_ADD             0x0067
#define ALS102_B_KALMANFILTER_REG_ADD          0x0068
#define ALS102_KALMANQF_REG_ADD                0x0069
#define ALS102_KALMANRF_REG_ADD                0x006a


#define ALS102_REG_TOTALNUM                    0x002b

#define ALS102_INIT_REG_ADD                    0x006f


#define ALS103_EXPOSURE_TIME_REG_ADD           0x0070
#define ALS103_PINGJUN_REG_ADD                 0x0071
#define ALS103_GUJIAERZHI_REG_ADD              0x0072
#define ALS103_WIDTHLIANTONGDIS_REG_ADD        0x0073
#define ALS103_HIGHLIANTONGDIS_REG_ADD         0x0074
#define ALS103_JIGUANGLONG_REG_ADD             0x0075
#define ALS103_JIGUANGKUANDU_REG_ADD           0x0076
#define ALS103_JIGUANGDUIBIDU_REG_ADD          0x0077
#define ALS103_LVBOMOD_REG_ADD                 0x0078


#define ALS103_REG_TOTALNUM                    0x0009

#define ALS103_INIT_REG_ADD                    0x007f


#define ALS104_EXPOSURE_TIME_REG_ADD           0x0080
#define ALS104_PINGJUN_REG_ADD                 0x0081
#define ALS104_B_YANMOFUZHU_REG_ADD            0x0082
#define ALS104_B_GUDINGQUYU_REG_ADD            0x0083
#define ALS104_WIDTHLIANTONGDIS_REG_ADD        0x0084
#define ALS104_HIGHLIANTONGDIS_REG_ADD         0x0085
#define ALS104_GUJIAERZHI_REG_ADD              0x0086
#define ALS104_JIGUANGHIGHT_REG_ADD            0x0087
#define ALS104_JIGUANGLONG_REG_ADD             0x0088
#define ALS104_JIGUANGKUANDU_REG_ADD           0x0089
#define ALS104_UPDIF_REG_ADD                   0x008a
#define ALS104_UPDIFMIN_REG_ADD                0x008b
#define ALS104_UPLONG_REG_ADD                  0x008c
#define ALS104_DOWNDIF_REG_ADD                 0x008d
#define ALS104_DOWNDIFMIN_REG_ADD              0x008e
#define ALS104_DOWNDLONG_REG_ADD               0x008f
#define ALS104_DIS_CENTER_ST_REG_ADD           0x0090
#define ALS104_DIS_CENTER_ED_REG_ADD           0x0091
#define ALS104_B_KALMANFILTER_REG_ADD          0x0092
#define ALS104_KALMANQF_REG_ADD                0x0093
#define ALS104_KALMANRF_REG_ADD                0x0094


#define ALS104_REG_TOTALNUM                    0x0015

#define ALS104_INIT_REG_ADD                    0x009f


#define ALS105_EXPOSURE_TIME_REG_ADD           0x00b0
#define ALS105_PINGJUN_REG_ADD                 0x00b1
#define ALS105_B_YANMOFUZHU_REG_ADD            0x00b2
#define ALS105_WIDTHLIANTONGDIS_REG_ADD        0x00b3
#define ALS105_HIGHLIANTONGDIS_REG_ADD         0x00b4
#define ALS105_GUJIAERZHI_REG_ADD              0x00b5
#define ALS105_JIGUANGHIGHT_REG_ADD            0x00b6
#define ALS105_JIGUANGLONG_REG_ADD             0x00b7
#define ALS105_JIGUANGKUANDU_REG_ADD           0x00b8
#define ALS105_UPDIF_REG_ADD                   0x00b9
#define ALS105_UPDIFMIN_REG_ADD                0x00ba
#define ALS105_UPLONG_REG_ADD                  0x00bb
#define ALS105_DOWNDIF_REG_ADD                 0x00bc
#define ALS105_DOWNDIFMIN_REG_ADD              0x00bd
#define ALS105_DOWNDLONG_REG_ADD               0x00be
#define ALS105_DIS_CENTER_ST_REG_ADD           0x00bf
#define ALS105_DIS_CENTER_ED_REG_ADD           0x00c0
#define ALS105_B_CUT_REG_ADD                   0x00c1
#define ALS105_CUTLEFT_REG_ADD                 0x00c2
#define ALS105_CUTRIGHT_REG_ADD                0x00c3
#define ALS105_CUTTOP_REG_ADD                  0x00c4
#define ALS105_CUTDEEP_REG_ADD                 0x00c5
#define ALS105_GUAIDIANYUZHI_REG_ADD           0x00c6
#define ALS105_DUANDIANJULI_REG_ADD            0x00c7
#define ALS105_B_DIBUFAXIANGLIANG_REG_ADD      0x00c8
#define ALS105_ANSWERPOINT_REG_ADD             0x00c9
#define ALS105_B_USEDOWNLIANTONG_REG_ADD       0x00ca
#define ALS105_B_KALMANFILTER_REG_ADD          0x00cb
#define ALS105_KALMANQF_REG_ADD                0x00cc
#define ALS105_KALMANRF_REG_ADD                0x00cd
#define ALS105_CUTSIDE_UP_REG_ADD              0x00ce
#define ALS105_CUTSIDE_DOWN_REG_ADD            0x00cf
#define ALS105_B_ERJIEDAO_REG_ADD              0x00d0
#define ALS105_B_QUXIAN_REG_ADD                0x00d1

#define ALS105_REG_TOTALNUM                    0x0022

#define ALS105_INIT_REG_ADD                    0x00df

#define ALS106_EXPOSURE_TIME_REG_ADD           0x00e0
#define ALS106_PINGJUN_REG_ADD                 0x00e1
#define ALS106_B_YANMOFUZHU_REG_ADD            0x00e2
#define ALS106_B_GUDINGQUYU_REG_ADD            0x00e3
#define ALS106_WIDTHLIANTONGDIS_REG_ADD        0x00e4
#define ALS106_HIGHLIANTONGDIS_REG_ADD         0x00e5
#define ALS106_GUJIAERZHI_REG_ADD              0x00e6
#define ALS106_JIGUANGHIGHT_REG_ADD            0x00e7
#define ALS106_JIGUANGLONG_REG_ADD             0x00e8
#define ALS106_JIGUANGKUANDU_REG_ADD           0x00e9
#define ALS106_B_CUT_REG_ADD                   0x00ea
#define ALS106_CUTLEFT_REG_ADD                 0x00eb
#define ALS106_CUTRIGHT_REG_ADD                0x00ec
#define ALS106_CUTTOP_REG_ADD                  0x00ed
#define ALS106_CUTDEEP_REG_ADD                 0x00ee
#define ALS106_DIFMIN_REG_ADD                  0x00ef
#define ALS106_SIDELONG_REG_ADD                0x00f0
#define ALS106_UPBACK_ST_REG_ADD               0x00f1
#define ALS106_UPBACK_ED_REG_ADD               0x00f2
#define ALS106_DOWNBACK_ST_REG_ADD             0x00f3
#define ALS106_DOWNBACK_ED_REG_ADD             0x00f4
#define ALS106_UPMINDIS_REG_ADD                0x00f5
#define ALS106_DOWNMINDIS_REG_ADD              0x00f6
#define ALS106_UPLONG_REG_ADD                  0x00f7
#define ALS106_DOWNDLONG_REG_ADD               0x00f8
#define ALS106_DISCENTER_ED_REG_ADD            0x00f9
#define ALS106_DUANDIANJULI_REG_ADD            0x00fa
#define ALS106_POKOUMOD_REG_ADD                0x00fb
#define ALS106_POKOUUPDIF_REG_ADD              0x00fc
#define ALS106_POKOUUPDIFMIN_REG_ADD           0x00fd
#define ALS106_POKOUUPBACK_ST_REG_ADD          0x00fe
#define ALS106_POKOUUPBACK_ED_REG_ADD          0x00ff
#define ALS106_POKOUDOWNDIF_REG_ADD            0x0100
#define ALS106_POKOUDOWNDIFMIN_REG_ADD         0x0101
#define ALS106_POKOUDOWNBACK_ST_REG_ADD        0x0102
#define ALS106_POKOUDOWNBACK_ED_REG_ADD        0x0103
#define ALS106_POKOUUPLONG_REG_ADD             0x0104
#define ALS106_POKOUDOWNLONG_REG_ADD           0x0105
#define ALS106_B_POKOUYAOBIAN_REG_ADD          0x0106
#define ALS106_POKOUYAOBIANHSIZE_REG_ADD       0x0107
#define ALS106_POKOUDUANXIANERZHI_REG_ADD      0x0108
#define ALS106_POKOUSEARCHDECTANCEMAX_REG_ADD  0x0109
#define ALS106_POKOUSEARCHDECTANCEMIN_REG_ADD  0x010a
#define ALS106_ANSWERPOINT_REG_ADD             0x010b
#define ALS106_B_KALMANFILTER_REG_ADD          0x010c
#define ALS106_KALMANQF_REG_ADD                0x010d
#define ALS106_KALMANRF_REG_ADD                0x010e

#define ALS106_REG_TOTALNUM                    0x002f

#define ALS106_INIT_REG_ADD                    0x010f

#define ALS107_EXPOSURE_TIME_REG_ADD           0x0110
#define ALS107_PINGJUN_REG_ADD                 0x0111
#define ALS107_B_YANMOFUZHU_REG_ADD            0x0112
#define ALS107_B_GUDINGQUYU_REG_ADD            0x0113
#define ALS107_WIDTHLIANTONGDIS_REG_ADD        0x0114
#define ALS107_HIGHLIANTONGDIS_REG_ADD         0x0115
#define ALS107_GUJIAERZHI_REG_ADD              0x0116
#define ALS107_JIGUANGHIGHT_REG_ADD            0x0117
#define ALS107_JIGUANGLONG_REG_ADD             0x0118
#define ALS107_JIGUANGKUANDU_REG_ADD           0x0119
#define ALS107_UPDIF_REG_ADD                   0x011a
#define ALS107_UPDIFMIN_REG_ADD                0x011b
#define ALS107_UPLONG_REG_ADD                  0x011c
#define ALS107_DOWNDIF_REG_ADD                 0x011d
#define ALS107_DOWNDIFMIN_REG_ADD              0x011e
#define ALS107_DOWNDLONG_REG_ADD               0x011f
#define ALS107_ST_DOWN_REG_ADD                 0x0120
#define ALS107_ED_DOWN_REG_ADD                 0x0121
#define ALS107_ST_UP_REG_ADD                   0x0122
#define ALS107_ED_UP_REG_ADD                   0x0123
#define ALS107_DIS_CENTER_ST_REG_ADD           0x0124
#define ALS107_DIS_CENTER_ST2_REG_ADD          0x0125
#define ALS107_DIS_CENTER_ED2_REG_ADD          0x0126
#define ALS107_B_KALMANFILTER_REG_ADD          0x0127
#define ALS107_KALMANQF_REG_ADD                0x0128
#define ALS107_KALMANRF_REG_ADD                0x0129

#define ALS107_REG_TOTALNUM                    0x001a

#define ALS107_INIT_REG_ADD                    0x012f

#define ALS108_EXPOSURE_TIME_REG_ADD           0x0130
#define ALS108_CENTER_X_REG_ADD                0x0131
#define ALS108_CENTER_Y_REG_ADD                0x0132
#define ALS108_SEARCT_W_REG_ADD                0x0133
#define ALS108_SEARCT_H_REG_ADD                0x0134
#define ALS108_STC_ALPHA_REG_ADD               0x0135
#define ALS108_STC_BETA_REG_ADD                0x0136
#define ALS108_STC_RHO_REG_ADD                 0x0137
#define ALS108_STC_SIGMA_REG_ADD               0x0138

#define ALS108_REG_TOTALNUM                    0x0009

#define ALS108_INIT_REG_ADD                    0x013f


#define ALS_SHOW_STEP_REG_ADD                  0x018f


/*****************************/
//激光头机器人型号和相机尺寸寄存器
#define PORT_ALSROBOTCAM_SET               1501  //端口号1501: 激光头机器人型号和相机尺寸端口号

#define ALSROBOTCAM_ROBOTMOD_REG_ADD                        0x0000  //机器人型号
#define ALSROBOTCAM_ROBOTPORT_REG_ADD                       0x0001  //机器人端口号
#define ALSROBOTCAM_COMPENSATION_X                          0x0002  //标定补偿X
#define ALSROBOTCAM_COMPENSATION_Y                          0x0003  //标定补偿Y
#define ALSROBOTCAM_COMPENSATION_Z                          0x0004  //标定补偿Z
#define ALSROBOTCAM_CAMWIDTH_REG_ADD                        0x0005  //相机算法宽度
#define ALSROBOTCAM_CAMHEIGHT_REG_ADD                       0x0006  //相机算法高度
#define ALSROBOTCAM_CAMFPS_REG_ADD                          0x0007  //相机帧率
#define ALSROBOTCAM_VIEW_CAM_WIDTH_REG_ADD                  0x0008  //相机视野宽度
#define ALSROBOTCAM_VIEW_CAM_HEIGHT_REG_ADD                 0x0009  //相机视野高度
#define ALSROBOTCAM_REVERSE_Y_REG_ADD                       0x000a  //相机Y数据反向
#define ALSROBOTCAM_REVERSE_Z_REG_ADD                       0x000b  //相机Z数据反向
#define ALSROBOTCAM_P_DATA_EN_REG_ADD                       0x000c  //P寄存器功能开关
#define ALSROBOTCAM_P_DATA_CAL_POSTURE_REG_ADD              0x000d  //P变量姿态内外旋
#define ALSROBOTCAM_P_DATA_EYE_HAND_CALIBRATIONMODE_REG_ADD 0x000e  //P寄存器激光器安装方式
#define ALSROBOTCAM_ROBOTIPADDRESS_1_REG_ADD                0x000f  //机器人IP1,机器人为服务器时使用
#define ALSROBOTCAM_ROBOTIPADDRESS_2_REG_ADD                0x0010  //机器人IP2,机器人为服务器时使用
#define ALSROBOTCAM_ROBOTIPADDRESS_3_REG_ADD                0x0011  //机器人IP3,机器人为服务器时使用
#define ALSROBOTCAM_ROBOTIPADDRESS_4_REG_ADD                0x0012  //机器人IP4,机器人为服务器时使用

/*****************************/
//激光头计算结果寄存器
#define PORT_ALS_RESULT                    1502  //端口号1502: 激光头计算结果寄存器

#define ALS_VERSION_REG_ADD                     0x0000  //版本号
#define ALS_DELAY_REG_ADD                       0x0001  //延迟
#define ALS_STATE_REG_ADD                       0x0002  //搜索状态
#define ALS_Y_POINT1_REG_ADD                    0x0003  //Y坐标POINT1
#define ALS_Z_POINT1_REG_ADD                    0x0004  //Z坐标POINT1
#define ALS_WELD_WIDTH_REG_ADD                  0x0005  //焊缝宽度
#define ALS_WELD_HIGHT_REG_ADD                  0x0006  //焊缝高度
#define ALS_TIMESTAMP_HOURS_REG_ADD             0x0007  //时间戳时
#define ALS_TIMESTAMP_MINUTES_REG_ADD           0x0008  //时间戳分
#define ALS_TIMESTAMP_SECONDS_REG_ADD           0x0009  //时间戳秒
#define ALS_TIMESTAMP_MILLISECONDS_REG_ADD      0x000a  //时间戳毫秒
#define ALS_RESULT_FPS_REG_ADD                  0x000b  //数据帧率
#define ALS_CAM_FPS_REG_ADD                     0x000c  //相机帧率
#define ALS_TIME_HOURS_REG_ADD                  0x000d  //当前时间时
#define ALS_TIME_MINUTES_REG_ADD                0x000e  //当前时间分
#define ALS_TIME_SECONDS_REG_ADD                0x000f  //当前时间秒
#define ALS_TIME_MILLISECONDS_REG_ADD           0x0010  //当前时间毫秒
#define ALS_STATE2_REG_ADD                      0x0011  //传感器状态,0表示正在处理,1表示成功,-1表示失败
#define ALS_P_REALTIME_POSX_REG_ADD             0x0012  //P变量实时检测结果X，(0x012-0x013)单位微米
#define ALS_P_REALTIME_POSY_REG_ADD             0x0014  //P变量实时检测结果Y，(0x014-0x015)单位微米
#define ALS_P_REALTIME_POSZ_REG_ADD             0x0016  //P变量实时检测结果Z，(0x016-0x017)单位微米
#define ALS_P_REALTIME_POSRX_REG_ADD            0x0018  //P变量实时检测结果RX，(0x018-0x019)单位0.0001deg
#define ALS_P_REALTIME_POSRY_REG_ADD            0x001a  //P变量实时检测结果RY，(0x01a-0x01b)单位0.0001deg
#define ALS_P_REALTIME_POSRZ_REG_ADD            0x001c  //P变量实时检测结果RZ，(0x01c-0x01d)单位0.0001deg
#define ALS_P_REALTIME_POSOUT1_REG_ADD          0x001e  //P变量实时检测结果，机器人外部轴1，(0x01e-0x01f)
#define ALS_P_REALTIME_POSOUT2_REG_ADD          0x0020  //P变量实时检测结果，机器人外部轴2，(0x020-0x021)
#define ALS_P_REALTIME_POSOUT3_REG_ADD          0x0022  //P变量实时检测结果，机器人外部轴3，(0x022-0x023)
#define ALS_P_REALTIME_TOOL_REG_ADD             0x0024  //P变量实时检测结果，机器人工具号
#define ALS_P_REALTIME_TCP_REG_ADD              0x0025  //P变量实时检测结果，机器人坐标系号
#define ALS_P_REALTIME_USERTCP_REG_ADD          0x0026  //P变量实时检测结果，机器人用户坐标系号

#define ALS_Y_POINT2_REG_ADD                    0x0050  //Y坐标POINT2
#define ALS_Z_POINT2_REG_ADD                    0x0051  //Z坐标POINT2
#define ALS_Y_POINT3_REG_ADD                    0x0052  //Y坐标POINT3
#define ALS_Z_POINT3_REG_ADD                    0x0053  //Z坐标POINT3
#define ALS_Y_POINT4_REG_ADD                    0x0054  //Y坐标POINT4
#define ALS_Z_POINT4_REG_ADD                    0x0055  //Z坐标POINT4

#define ALS_SOLDER_REG_ADD                      0x0060  //焊点

#define ALS_X_POINT1_REG_ADD                    0x0070  //X坐标POINT
#define ALS_WELD_LONG_REG_ADD                   0x0071  //焊缝X法向量

#define ALS_OPEN_REG_ADD                        0x0101  //跟踪开关
#define ALS_TASKNUM_REG_ADD                     0x0102  //任务号

#define ALS_REALTIME_POSX_REG_ADD               0x0111  //写入实时坐标，机器人X，(0x111-0x112)单位微米
#define ALS_REALTIME_POSY_REG_ADD               0x0113  //写入实时坐标，机器人Y，(0x113-0x114)单位微米
#define ALS_REALTIME_POSZ_REG_ADD               0x0115  //写入实时坐标，机器人Z，(0x115-0x116)单位微米
#define ALS_REALTIME_POSRX_REG_ADD              0x0117  //写入实时坐标，机器人RX，(0x117-0x118)单位0.0001deg
#define ALS_REALTIME_POSRY_REG_ADD              0x0119  //写入实时坐标，机器人RY，(0x119-0x11a)单位0.0001deg
#define ALS_REALTIME_POSRZ_REG_ADD              0x011b  //写入实时坐标，机器人RY，(0x11b-0x11c)单位0.0001deg
#define ALS_REALTIME_POSOUT1_REG_ADD            0x011d  //写入实时坐标，机器人外部轴1，(0x11d-0x11e)
#define ALS_REALTIME_POSOUT2_REG_ADD            0x011f  //写入实时坐标，机器人外部轴2，(0x11f-0x120)
#define ALS_REALTIME_POSOUT3_REG_ADD            0x0121  //写入实时坐标，机器人外部轴3，(0x121-0x122)
#define ALS_REALTIME_TOOL_REG_ADD               0x0123  //写入实时坐标，机器人工具号
#define ALS_REALTIME_TCP_REG_ADD                0x0124  //写入实时坐标，机器人坐标系号
#define ALS_REALTIME_USERTCP_REG_ADD            0x0125  //写入实时坐标，机器人用户坐标系号

/*****************************/
//PLC寄存器
/*
#define PLC_ST_REG_ADD                         0x0000   //PLC起始寄存器
#define PLC_RED_NUM                            0x0010   //PLC读取数量
*/
/****************************/


//64位浮点排序类
class d_Mysort
{
public:
    double data;        //值
    int subscript;      //下标
};
//64位浮点排序函数
bool d_Mysortcompare(d_Mysort &a,d_Mysort &b);

//网络字节序64位大小端发送
uint64_t htonll(uint64_t val);

//网络字节序64位大小端接收
uint64_t ntohll(uint64_t val);


class IP                //ip
{
public:
    QString ip;         //ip地址
};

class IP_port      //ip和port
{
public:
    QString ip;         //ip地址
    int port;           //ip端口
};

class Pos3f         //3维坐标
{
public:
   Pos3f();
   float X;
   float Y;
   float Z;
};

class RobPos        //机器人坐标
{
public:
    RobPos();

    float X;
    float Y;
    float Z;
    float RX;
    float RY;
    float RZ;

    int out_1;       //外部轴1
    int out_2;       //外部轴2
    int out_3;       //外部轴3

    bool nEn;
};

class systime   //时间
{
public:
    u_int16_t hour;
    u_int16_t min;
    u_int16_t sec;
    u_int16_t msec;
};

class leaser_pos //激光器pos点坐标
{
public:
    leaser_pos();

    float Y;
    float Z;
    bool nEn;
};

class TCP_Leaserpos //机器人激光头手眼标定点坐标系
{
public:
    leaser_pos leaserpos;  //激光头坐标
    RobPos robotpos;       //此时的机器人坐标
};

class Scan_trace_line  //扫描线结果
{
public:
    tutorial_interfaces::msg::IfAlgorhmitcloud ros_line;//线激光结果
    RobPos robotpos;       //此时的机器人坐标
    systime robottime;     //机器人当前采集的时间
};

class Point_robpos_result //机器人点结果
{
public:
    Point_robpos_result();
    RobPos robotpos;    //点结果坐标
    QString name;       //点名字
    bool nEn;           //点是否有效
};

class Coord_Matrix4d_result     //零点定位矩阵
{
public:
    Coord_Matrix4d_result();
    Eigen::Matrix3d R;          //旋转矩阵
    Eigen::Vector3d T;          //平移矩阵(零点坐标)
    QString name;               //矩阵名字
    bool nEn;                   //矩阵是否有效
};

class Scan_trace_result    //扫描轨迹
{
public:
    std::vector<Scan_trace_line> point;//线激光结果
    QString name; //扫描轨迹名字
};

class Weld_trace_result  //跟踪轨迹
{
public:
    std::vector<RobPos> point;//跟踪轨迹结果
    QString name; //跟踪轨迹名字
};

class Weld_trace_onec//跟踪工艺轨迹的一小段
{
public:
    std::vector<RobPos> point;//跟踪轨迹结果
    float speed;//这一小段的跟踪速度
};

class Weld_tracing_result  //跟踪工艺轨迹
{
public:
    std::vector<Weld_trace_onec> trace;//跟踪轨迹结果
    QString name; //跟踪工艺轨迹名字
};


//姿态类补偿类
class ChangeRobPosVariable
{
public:
    RobPos posture; //姿态
    Pos3f Variable;//补偿
};

typedef enum Robmovemodel_ID            //机器人移动方式
{
    MOVEL=0,        //直线运动
    MOVEJ=1,        //关节运动
    MOVEC=2,        //圆运动
}Robmovemodel;

typedef enum IOmodel_ID     //机器人IO口收发方式
{
    OUT=0,        //输出
    WAITIN=1,     //等待输入
}IOmodel;

typedef enum Plcmodel_ID     //PLC读写方式
{
    PLC_WRITE=0,        //写PLC
    PLC_WAIT=1,         //等待输入
}Plcmodel;

typedef enum Weldworkmodel_ID       //焊机工作状态
{
    STATIC=0,       //空闲
    FIRE=1,         //起弧
    WIND=2,         //送丝
    REWIND=3,       //退丝
    GASS=4,         //出气
}Weldworkmodel;

typedef enum Alternatingcurrent_ID    //交变电流
{
    DIRECT=0,       //直流电
    ALTERNATING=1,  //交流电
}Alternatingcurrent;

//轨迹生成模式
typedef enum TRACE_EDIT_MODE_ID
{
    TRACE_EDIT_MODE_ONE_TO_ONE=0,                //单扫对单轨道模式
    TRACE_EDIT_MODE_THREE_TO_ONE=1,              //三直线交点模式
    TRACE_EDIT_MODE_TOWPOINT_THREE_TO_ONE=2,     //两端点三直线交点模式
}Trace_edit_mode;

//点生成模式
typedef enum PLOTPOS_EDIT_MODE_ID
{
    PLOTPOS_EDIT_MODE_THREE_TO_ONE=0,           //三直线交点模式
    PLOTPOS_EDIT_MODE_FIVEPOINTS_TO_ONE=1,      //两点直线与三点交点模式
}Plotpos_edit_mode;

//手眼标定模式
typedef enum EYE_HAND_CALIBRATIONMODE_ID
{
    HAND_IN_EYE=0,      //眼在手上
    HAND_OUT_EYE=1,     //眼在手外
}Eye_Hand_calibrationmode;

typedef enum MainDlg_robmovestate_ID        //主页机器人移动状态
{
    MAINDLG_STATIC,         //机器人空闲
    MAINDLG_NOTMOVE,        //机器人不动
    MAINDLG_XSUB,           //机器人X负移动
    MAINDLG_XSUB_ING,       //机器人X负移动中
    MAINDLG_XADD,           //机器人X正移动
    MAINDLG_XADD_ING,       //机器人X正移动中
    MAINDLG_YSUB,           //机器人Y负移动
    MAINDLG_YSUB_ING,       //机器人Y负移动中
    MAINDLG_YADD,           //机器人Y正移动
    MAINDLG_YADD_ING,       //机器人Y正移动中
    MAINDLG_ZSUB,           //机器人Z负移动
    MAINDLG_ZSUB_ING,       //机器人Z负移动中
    MAINDLG_ZADD,           //机器人Z正移动
    MAINDLG_ZADD_ING,       //机器人Z正移动中
    MAINDLG_RXSUB,          //机器人RX负移动
    MAINDLG_RXSUB_ING,      //机器人RX负移动中
    MAINDLG_RXADD,          //机器人RX正移动
    MAINDLG_RXADD_ING,      //机器人RX正移动中
    MAINDLG_RYSUB,          //机器人RY负移动
    MAINDLG_RYSUB_ING,      //机器人RY负移动中
    MAINDLG_RYADD,          //机器人RY正移动
    MAINDLG_RYADD_ING,      //机器人RY正移动中
    MAINDLG_RZSUB,          //机器人RZ负移动
    MAINDLG_RZSUB_ING,      //机器人RZ负移动中
    MAINDLG_RZADD,          //机器人RZ正移动
    MAINDLG_RZADD_ING,      //机器人RZ正移动中
    MAINDLG_OUT1SUB,        //机器人外部轴1负移动
    MAINDLG_OUT1SUB_ING,    //机器人外部轴1负移动中
    MAINDLG_OUT1ADD,        //机器人外部轴1正移动
    MAINDLG_OUT1ADD_ING,    //机器人外部轴1正移动中
    MAINDLG_OUT2SUB,        //机器人外部轴2负移动
    MAINDLG_OUT2SUB_ING,    //机器人外部轴2负移动中
    MAINDLG_OUT2ADD,        //机器人外部轴2正移动
    MAINDLG_OUT2ADD_ING,    //机器人外部轴2正移动中
    MAINDLG_OUT3SUB,        //机器人外部轴3负移动
    MAINDLG_OUT3SUB_ING,    //机器人外部轴3负移动中
    MAINDLG_OUT3ADD,        //机器人外部轴3正移动
    MAINDLG_OUT3ADD_ING,    //机器人外部轴3正移动中
}MainDlg_robmovestate;

#define PENDULUM_ID_TOTAL_NUM     5     //摆焊接模式总数
typedef enum PENDULUM_MODE_ID   //摆焊模式
{
    PENDULUM_ID_SIMPLE=0,   //单摆
    PENDULUM_ID_TRIANGLE=1, //三角摆
    PENDULUM_ID_L=2,        //L摆
    PENDULUM_ID_SINE=3,     //正弦摆
    PENDULUM_ID_CIRCULAR=4, //椭圆摆
}Pendulum_mode;

#define TREND_ID_TOTAL_NUM     3     //摆焊接模式总数
typedef enum TREND_MODE_ID  //摆幅方向
{
    TREND_MODE_X_Z=0,     //X轴走向,开口朝Z轴正方向
    TREND_MODE_Y_Z=1,     //Y轴走向,开口朝Z轴正方向
    TREND_MODE_Z_Y=2,     //Z轴走向,开口朝Y轴负方向
}Trend_mode;

//摆焊参数
class wWAVEParam
{
public:
    float period;                  //周期(s)
    float leftAmp;                 //左摆副mm
    float rightAmp;                //右摆副mm
    float leftAmp_z;               //左摆副上下mm
    float rightAmp_z;              //右摆副上下mm
    float leftStopTime;            //左摆停留时间(s)
    float rightStopTime;           //右摆停留时间(s)
    float anglex;                  //摆幅与焊缝角度
    float angley;                  //摆幅与焊缝角度
    unsigned int startPos;         //起摆位置
    unsigned int  order;           //先往左摆还是右摆
    Pendulum_mode pendulum_mode;   //摆焊接模式
    unsigned int timeGap;          //两点之间采样点周期(ms)
    Trend_mode trend_mode;         //摆焊走向
    wWAVEParam();
};

QString Pendulum_mode_toQString(Pendulum_mode pendulum_mode);

QString Trend_mode_toQString(Trend_mode trend_mode);

bool b_nosame_vector_QString(std::vector<QString> vec);//判断容器里是否有相同元素,没有则返回true

#endif // GLOBAL_H
