#ifndef RESULTDATA_H
#define RESULTDATA_H
#include "opencv2/opencv.hpp"
#include <modbus/modbus.h>
#include <QThread>

#define MODBUS_ROBOT_REGISTERS_NUM      10
#define MODBUS_RESULT_MAXNUM            400
#define MODBUS_PARAM_MAXNUM             400

class leaser_pos //激光器pos点坐标
{
public:
    leaser_pos();

    float Y;
    float Z;
    bool nEn;
};

class modbustcpThread;

class ResultData
{
public:
    ResultData();
    ~ResultData();

    leaser_pos pos1;    //激光器pos1点坐标
    leaser_pos pos2;    //激光器pos2点坐标
    leaser_pos pos3;    //激光器pos3点坐标

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

protected:

};

#endif // RESULTDATA_H
