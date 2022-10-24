#include "robotdata.h"

RobotData *RobotData::Get()
{
    static RobotData fun;
    return &fun;
}

RobotData::RobotData()
{
    b_connect=false;
    b_link_ctx_posget=false;
    robot_speed=0;
    robot_state=ROBOT_STATE_UNLINK;
    robot_model=ROBOT_MODEL_NULL;
}

RobotData::~RobotData()
{

}

QString RobotData::robot_model_toQString()
{
    QString msg;
    switch(robot_model)
    {
    case ROBOT_MODEL_NULL:
        msg="无";
        break;
    case ROBOT_MODEL_EMERGEN:
        msg="智昌";
        break;
    }
    return msg;
}

QString RobotData::robot_state_toQString()
{
    QString msg;
    switch(robot_state)
    {
    case ROBOT_STATE_UNLINK:
        msg="未链接";
        break;
    case ROBOT_STATE_IDLE:
        msg="空闲";
        break;
    case ROBOT_STATE_MOVE:
        msg="移动";
        break;
    case ROBOT_STATE_SCAN:
        msg="扫描";
        break;
    case ROBOT_STATE_TRACE:
        msg="跟踪";
        break;
    }
    return msg;
}

int RobotData::ConnectRobot(QString ip,int port)         //连接机器人
{
    if(b_link_ctx_posget==false)
    {
        ctx_posget = modbus_new_tcp(ip.toUtf8(), port);
        if (modbus_connect(ctx_posget) == -1)
        {
            modbus_free(ctx_posget);
            return 1;
        }
        b_link_ctx_posget=true;
    }
    robot_state=ROBOT_STATE_IDLE;
    b_connect=true;
    return 0;
}

void RobotData::DisconnectRobot()
{
    if(b_connect==true)
    {
         if(b_link_ctx_posget==true)
         {
            modbus_close(ctx_posget);
            b_link_ctx_posget=false;
         }
         b_connect=false;
         robot_state=ROBOT_STATE_UNLINK;
    }
}
