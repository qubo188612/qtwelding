#ifndef MY_CMD_H
#define MY_CMD_H

#include <QString>
#include <QStringList>
#include <global.h>

/************************/
//项目0:命令集合
//注释符号，举例 #测试
//移动指令，举例 MOV: SPEED[25] MOVL[1.3,32.7,45,66,7,89,3,0,0,0] TCP[1] CHANGE[矩阵1]
//延时指令，举例 DELAY: TIME[1000]
//激光指令，举例 CAM: TASK[102] WORK[1]
//激光指令，举例 CAM: WORK[0]
//焊机指令，举例 WELD: WORK[1] ELED[1.23] ELEM[0]
//焊机指令，举例 WELD: WORK[0]
//采集指令，举例 SCAN: MOVL[1.3,32.7,45,66,7,89,3,0,0,0] SPEED[25] TCP[0] NAME[扫描第一条line]
//跟踪指令，举例 TRACE: NAME[跟踪第一条line] SPEED[25] TCP[0] CRAFT[/home/qubo/caf.json]
//生成轨迹指令，举例 CREAT: MODE[1] SCAN[扫描第一条line,第二条,第三] NAME[跟踪第一条line]
//IO口输出指令，举例 IO: OUT[1,0,0,1,0,1,0,1]
//IO口等待输入指令，举例IO: WAITIN[1,0,1,0,1,1,1,1]
//写PLC指令，举例 PLC: WRITEPLC[20] DATA16[2]
//等待PLC指令，举例 PLC: WAITPLC[20] DATA16[2]
//寻位指令，举例 SEARCH: MOVL[1.3,32.7,45,66,7,89,3,0,0,0] SPEED[25] TCP[0] SIDE[0] SIDEMOVE[1,1,1] SIDESPEED[25] POINTNAME[寻位的点point1]
//工件坐标系定位，举例 COORD: POINTX[寻位的点point1] POINTO[寻位的点point3] NAME[零位矩阵]
//扫描的焊缝坐标值，举例 GETPOS: TIME[1000] POINTNAME[寻位的点point1]
//点位移动指令，举例 SMOV: SPEED[25] SMOVL[寻位的点point1] TCP[1] CHANGE[矩阵1]
//点位采集指令，举例 SSCAN: SMOVL[寻位的点point1] SPEED[25] TCP[0] NAME[扫描第一条line]
//寻位末尾指令，举例 SEARCHEND: MOVL[1.3,32.7,45,66,7,89,3,0,0,0] SPEED[25] TCP[0] POINTNAME[寻位的点point1]


//key项
#define CMD_MOV_KEY                     "MOV:"          //移动命令集合KEY
#define CMD_SMOV_KEY                    "SMOV:"         //点位移动命令集合KEY
#define CMD_DELAY_KEY                   "DELAY:"        //延时命令集合KEY
#define CMD_CAM_KEY                     "CAM:"          //激光命令集合KEY
#define CMD_WELD_KEY                    "WELD:"         //焊机命令集合KEY
#define CMD_SCAN_KEY                    "SCAN:"         //采集命令集合KEY
#define CMD_SSCAN_KEY                   "SSCAN:"        //点位采集命令集合KEY
#define CMD_TRACE_KEY                   "TRACE:"        //跟踪命令集合KEY
#define CMD_CREAT_KEY                   "CREAT:"        //生成轨迹命令KEY
#define CMD_IO_KEY                      "IO:"           //IO命令集合KEY
#define CMD_PLC_KEY                     "PLC:"          //PLC命令集合KEY
#define CMD_SEARCH_KEY                  "SEARCH:"       //寻位命令集合KEY
#define CMD_SEARCHEND_KEY               "SEARCHEND:"    //寻位末尾命令集合KEY
#define CMD_COORD_KEY                   "COORD:"        //工件坐标系定位命令集合KEY
#define CMD_GETPOS_KEY                  "GETPOS:"       //获取扫描的焊缝坐标值命令集合KEY

//参数项
#define CMD_MOVL                            "MOVL"                //直线移动
#define CMD_MOVJ                            "MOVJ"                //关节移动
#define CMD_MOVC                            "MOVC"                //圆形移动
#define CMD_SMOVL                           "SMOVL"               //直线移动
#define CMD_SMOVJ                           "SMOVJ"               //关节移动
#define CMD_SMOVC                           "SMOVC"               //圆形移动
#define CMD_TCP                             "TCP"                 //TCP
#define CMD_SPEED                           "SPEED"               //速度
#define CMD_CHANGE                          "CHANGE"              //坐标系变换
#define CMD_TIME                            "TIME"                //时间         单位ms
#define CMD_TASK                            "TASK"                //任务号
#define CMD_WORK                            "WORK"                //是否启动      0:停止 1:启动
#define CMD_ELED                            "ELED"                //电流         单位A
#define CMD_ELEM                            "ELEM"                //交变电        0:直流 1:交流
#define CMD_NAME                            "NAME"                //命名
#define CMD_SCAN                            "SCAN"                //扫描轨迹参数
#define CMD_MODE                            "MODE"                //模式参数
#define CMD_CRAFT                           "CRAFT"               //工艺文件路径
#define CMD_OUT                             "OUTIO"               //IO输出
#define CMD_WAITIN                          "WAITINIO"            //等待IO输入
#define CMD_WRITEPLC                        "WRITEPLC"            //写PLC寄存器
#define CMD_WAITPLC                         "WAITPLC"             //等待PLC寄存器输入
#define CMD_DATA16                          "DATA16"              //16位整形数字
#define CMD_SIDE                            "SIDE"                //寻位寻找两侧的范围
#define CMD_SIDEMOVE                        "SIDEMOVE"            //寻位寻找两侧的单位向量
#define CMD_SIDESPEED                       "SIDESPEED"           //寻位寻找两侧的空闲移动速度
#define CMD_POINTX                          "POINTX"              //实际零位矩阵零点的X方向基准点
#define CMD_POINTO                          "POINTO"              //实际零位矩阵零点基准点


/************************/
class my_cmd
{
public:
    my_cmd();

    QString cmd_move(RobPos pos,Robmovemodel movemodel,float speed,int tcp,QString change="");//移动命令
    QString cmd_moveC(RobPos pos1,RobPos pos2,RobPos pos3,Robmovemodel movemodel,float speed,int tcp,QString change="");//移动圆命令
    QString cmd_smove(QString s_pos,Robmovemodel movemodel,float speed,int tcp,QString change="");//点位移动命令
    QString cmd_smoveC(QString s_pos1,QString s_pos2,QString s_pos3,Robmovemodel movemodel,float speed,int tcp,QString change="");//点位移动命令
    QString cmd_delay(int time);//延时命令
    QString cmd_cam(int task,int work);//相机启停命令不
    QString cmd_cam_work(int work);//相机启停命令
    QString cmd_elec(float eled,Alternatingcurrent elem,int work);//焊机启停命令不
    QString cmd_elec_work(int work);//焊机启停命令
    QString cmd_scan(RobPos pos,Robmovemodel movemodel,float speed,int tcp,QString name,QString change="");//采集命令
    QString cmd_scanC(RobPos pos1,RobPos pos2,RobPos pos3,Robmovemodel movemodel,float speed,int tcp,QString name,QString change="");//圆采集命令
    QString cmd_sscan(QString s_pos,Robmovemodel movemodel,float speed,int tcp,QString name,QString change="");//点位采集命令
    QString cmd_sscanC(QString s_pos1,QString s_pos2,QString s_pos3,Robmovemodel movemodel,float speed,int tcp,QString name,QString change="");//点位圆采集命令
    QString cmd_trace(float speed,int tcp,QString craftfilepath,QString name);//跟踪命令
    QString cmd_creat(Trace_edit_mode mode,std::vector<QString> scanname,QString name);//生成跟踪轨迹
    QString cmd_ioout(std::vector<int> io);//输出IO信号
    QString cmd_iowaitin(std::vector<int> io);//等待输入IO信号
    QString cmd_plcwait(int register_add,int16_t register_data);
    QString cmd_plcwrite(int register_add,int16_t register_data);
    QString cmd_search(RobPos pos,Robmovemodel movemodel,float speed,int tcp,int side,std::vector<float> sidemove,float sidespeed,QString name,QString change="");//寻位命令
    QString cmd_searchC(RobPos pos1,RobPos pos2,RobPos pos3,Robmovemodel movemodel,float speed,int tcp,int side,std::vector<float> sidemove,float sidespeed,QString name,QString change="");//圆寻位命令
    QString cmd_searchend(RobPos pos,Robmovemodel movemodel,float speed,int tcp,QString name,QString change="");//寻位末尾命令
    QString cmd_searchendC(RobPos pos1,RobPos pos2,RobPos pos3,Robmovemodel movemodel,float speed,int tcp,QString name,QString change="");//寻位末尾命令
    QString cmd_coord(QString s_pointX,QString s_pointO,QString name);//生成定位变化矩阵
    QString cmd_getpos(int time,QString name);//获取扫描的焊缝坐标值命令

    int getkey(QString msg,QString &return_msg,QString &return_key);   //解key 返回值0:正常，返回值-1:注释行，返回值>0:异常
    int decodecmd(QString msg,QString &return_msg,QString &return_key);//解码：返回值0:正常
                                                                       //     返回值-1:注释行
                                                                       //     返回值>0:异常
    int cmd_move_tcp;//获取到移动TCP
    RobPos cmd_move_pos;//获取到移动终点坐标
    RobPos cmd_move_pos1;//获取到圆移动起点坐标
    RobPos cmd_move_pos2;//获取到圆移动途径点坐标
    RobPos cmd_move_pos3;//获取到圆移动终点坐标
    float cmd_move_speed;//获取到速度值
    Robmovemodel cmd_move_movemod;//获取到的移动模式
    QString cmd_move_change;//获取到的变换矩阵名字


    int cmd_delay_time;//获取到延时时间

    int cmd_cam_task;//获取到相机任务号
    int cmd_cam_work_d;//获取到相机启停

    int cmd_elec_work_d;//获取到焊机启停
    Alternatingcurrent cmd_elec_elem;  //获取到焊机交变电流模式
    float cmd_elec_eled; //获取到焊机电流

    RobPos cmd_scan_pos;//获取到扫描终点坐标
    RobPos cmd_scan_pos1;//获取到圆扫描起点坐标
    RobPos cmd_scan_pos2;//获取到圆扫描途径点坐标
    RobPos cmd_scan_pos3;//获取到圆扫描终点坐标
    float cmd_scan_speed;//获取到的扫描速度
    int cmd_scan_tcp;//获取到扫描TCP
    Robmovemodel cmd_scan_movemod;//获取到的扫描模式
    QString cmd_scan_name;//获取到的扫描轨迹名字
    QString cmd_scan_change;//获取到的扫描变换矩阵名字

    QString cmd_trace_name;//获取到跟踪轨迹名字
    float cmd_trace_speed;//获取到的跟踪速度
    int cmd_trace_tcp;//获取到跟踪TCP
    QString cmd_trace_craftfilepath;//获取到的焊接工艺包路径

    QString cmd_creat_name;//获取到的生成的轨迹名字
    Trace_edit_mode cmd_creat_mode;//获取到的轨迹生成模式
    std::vector<QString> cmd_creat_scanname;//获取到生成轨迹所需要的轨迹名字

    std::vector<int> cmd_io_output;//获取到的IO输出口信息
    std::vector<int> cmd_io_input;//获取到的IO输入口信息
    IOmodel cmd_io_workmod;//IO工作状态

    int cmd_plc_register_add;   //获取到的PLC寄存器信息
    int cmd_plc_register_data;  //获取到的PLC寄存器内容
    Plcmodel cmd_plc_mod;       //PLC读写模式

    RobPos cmd_search_pos;//获取到寻位终点坐标
    RobPos cmd_search_pos1;//获取到圆寻位起点坐标
    RobPos cmd_search_pos2;//获取到圆寻位途径点坐标
    RobPos cmd_search_pos3;//获取到圆寻位终点坐标
    float cmd_search_speed;//获取到的寻位速度
    int cmd_search_tcp;//获取到寻位TCP
    Robmovemodel cmd_search_movemod;//获取到的寻位模式
    QString cmd_search_name;//获取到的寻位点名字
    int cmd_search_side;//寻位寻找两侧的范围
    std::vector<float> cmd_search_sidemove;//寻位寻找两侧的单位向量
    float cmd_search_sidespeed;//寻位寻找两侧的空闲移动速度
    QString cmd_search_change;//获取到的寻位变换矩阵名字

    RobPos cmd_searchend_pos;//获取到寻位终点坐标
    RobPos cmd_searchend_pos1;//获取到圆寻位起点坐标
    RobPos cmd_searchend_pos2;//获取到圆寻位途径点坐标
    RobPos cmd_searchend_pos3;//获取到圆寻位终点坐标
    float cmd_searchend_speed;//获取到的寻位速度
    int cmd_searchend_tcp;//获取到寻位TCP
    Robmovemodel cmd_searchend_movemod;//获取到的寻位模式
    QString cmd_searchend_name;//获取到的寻位点名字
    QString cmd_searchend_change;//获取到的寻位变换矩阵名字

    QString cmd_coord_pointx;//实际零位矩阵零点的X方向基准点
    QString cmd_coord_pointo;//实际零位矩阵零点的基准点
    QString cmd_coord_name;//实际零位矩阵名字

    int cmd_getpos_time=0;//获取坐标时的等待时间
    QString cmd_getpos_name;//获取坐标的名字

    int cmd_smove_tcp;//获取到点位移动TCP
    QString cmd_smove_pos;//获取到点位移动终点坐标
    QString cmd_smove_pos1;//获取到点位圆移动起点坐标
    QString cmd_smove_pos2;//获取到点位圆移动途径点坐标
    QString cmd_smove_pos3;//获取到点位圆移动终点坐标
    float cmd_smove_speed;//获取到点位移动速度值
    Robmovemodel cmd_smove_movemod;//获取到的点位移动模式
    QString cmd_smove_change;//获取到的变换矩阵名字

    QString cmd_sscan_pos;//获取到点位扫描终点坐标
    QString cmd_sscan_pos1;//获取到点位圆扫描起点坐标
    QString cmd_sscan_pos2;//获取到点位圆扫描途径点坐标
    QString cmd_sscan_pos3;//获取到点位圆扫描终点坐标
    float cmd_sscan_speed;//获取到的点位扫描速度
    int cmd_sscan_tcp;//获取到点位扫描TCP
    Robmovemodel cmd_sscan_movemod;//获取到的点位扫描模式
    QString cmd_sscan_name;//获取到的点位扫描轨迹名字
    QString cmd_sscan_change;//获取到的点位扫描变换矩阵名字

protected:
    QString rc_tcp(int tcp);
    QString rc_speed(float speed);
    QString rc_move(RobPos pos,Robmovemodel movemodel);
    QString rc_moveC(RobPos pos1,RobPos pos2,RobPos pos3,Robmovemodel movemodel);
    QString rc_smove(QString s_pos,Robmovemodel movemodel);
    QString rc_smoveC(QString s_pos1,QString s_pos2,QString s_pos3,Robmovemodel movemodel);
    QString rc_change(QString name);
    QString rc_time(int time);
    QString rc_task(int task);
    QString rc_work(int work);
    QString rc_eled(float eled);
    QString rc_elem(Alternatingcurrent elem);
    QString rc_name(QString name);
    QString rc_mode(int mode);
    QString rc_scan(std::vector<QString> names);
    QString rc_craft(QString craftfilepath);
    QString rc_ioout(std::vector<int> io);
    QString rc_iowaitin(std::vector<int> io);
    QString rc_plcwait(int register_add);
    QString rc_plcwrite(int register_add);
    QString rc_16data(int16_t i16_data);
    QString rc_side(int side);
    QString rc_sidemove(std::vector<float> sidemove);
    QString rc_sidespeed(float speed);
    QString rc_pointX(QString s_pointX);
    QString rc_pointO(QString s_pointO);

    int de_param(int param_n,QString msg,QString &paramname,int &data_fpos,int &data_bpos,QString &return_msg);
    int de_float(QString parakey,QString msg,int data_fpos,int data_bpos,float &floatdata,QString &return_msg);
    int de_vector_float(QString parakey,QString msg,int data_fpos,int data_bpos,std::vector<float> &vector_floatdata,QString &return_msg);
    int de_int(QString parakey,QString msg,int data_fpos,int data_bpos,int &intdata,QString &return_msg);
    int de_vector_int(QString parakey,QString msg,int data_fpos,int data_bpos,std::vector<int> &vector_intdata,QString &return_msg);
    int de_robpos(QString parakey,QString msg,int data_fpos,int data_bpos,RobPos &pos,QString &return_msg);
    int de_robposP(QString parakey,QString msg,int data_fpos,int data_bpos,RobPos &pos1,RobPos &pos2,RobPos &pos3,QString &return_msg);
    int de_QString(QString parakey,QString msg,int data_fpos,int data_bpos,QString &QStringdata,QString &return_msg);
    int de_vector_QString(QString parakey,QString msg,int data_fpos,int data_bpos,std::vector<QString> &vector_QStringdata,QString &return_msg);
};

#endif // MY_CMD_H
