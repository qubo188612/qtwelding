#ifndef RESULTDATA_H
#define RESULTDATA_H
#include "opencv2/opencv.hpp"
#include <modbus/modbus.h>
#include <QThread>
#include "global.h"

#define MODBUS_ROBOT_REGISTERS_NUM      10
#define MODBUS_RESULT_MAXNUM            400
#define MODBUS_PARAM_MAXNUM             400

class modbustcpThread;

class sent_info_leaser              //发送相机数据
{
public:
    modbus_t *ctx;                  //sock
    int addr;                       //发送寄存器
    std::vector<uint16_t> data;     //发送数据
};

class systime
{
public:
    u_int16_t hour;
    u_int16_t min;
    u_int16_t sec;
    u_int16_t msec;
};

class ResultData
{
public:
    ResultData();
    ~ResultData();

    leaser_pos pos1;    //激光器pos1点坐标
    leaser_pos pos2;    //激光器pos2点坐标
    leaser_pos pos3;    //激光器pos3点坐标
    systime leaser_time;//激光器时间
    systime time_stamp; //数据时间戳
    float fps;          //算法帧率
    float camfps;       //相机帧率
    uint16_t state;     //有效位


    uint8_t link_result_state;
    uint8_t link_param_state;
    uint8_t link_robotset_state;
    uint8_t b_luzhi;

    modbus_t *ctx_robotset;
    modbus_t *ctx_param;
    modbus_t *ctx_result;

    unsigned short red_robotset[MODBUS_ROBOT_REGISTERS_NUM];

    /******************/
    uint16_t alg0_99_threshold;
    uint16_t alg100_threshold;
    uint16_t alg101_threshold;
    uint16_t alg102_threshold;
    uint16_t alg103_threshold;

    std::vector<sent_info_leaser> send_group_leaser;    //发送相机数据队列
    volatile bool b_send_group_leaser;  //发送相机数据队列是否异常
    int ctx_result_dosomeing;   //1502端口忙

protected:

};

#endif // RESULTDATA_H
