#ifndef MY_CMD_H
#define MY_CMD_H

#include <QString>
#include <QStringList>
#include <global.h>
#include <craft.h>


/************************/
//项目0:命令集合
//注释符号，举例 #测试
//移动指令，举例 MOV: SPEED[25] MOVL[1.3,32.7,45,66,7,89,3,0,0,0] TCP[1] CHANGE[矩阵1]
//延时指令，举例 DELAY: TIME[1000]
//激光指令，举例 CAM: TASK[102] WORK[1]
//激光指令，举例 CAM: WORK[0]
//焊机指令，举例 WELD: WORK[1] ELED[1.23] ELEV[1.3] ELEM[0]
//焊机指令，举例 WELD: WORK[0]
//采集指令，举例 SCAN: MOVL[1.3,32.7,45,66,7,89,3,0,0,0] SPEED[25] TCP[0] NAME[扫描第一条line]
//创建工艺指令，举例 CRAFTS: MODE[1] POINTADDS[点位1，点位2，点位3，点位4] DATA32FS[1.00,2] NAME[工艺1]
//跟踪指令，举例 TRACE: CREAT[跟踪第一条line] SPEED[25] CRAFT[/home/qubo/caf.json] CHANGE[矩阵1] NAME[焊接轨迹1]
//跟踪指令，举例 TRACE2: CREAT[跟踪第一条line] SPEED[25] CRAFTS[工艺1] CHANGE[矩阵1] NAME[焊接轨迹1]
//生成轨迹指令，举例 CREAT: MODE[1] SCAN[扫描第一条line,第二条,第三] NAME[跟踪第一条line]
//IO口输出指令，举例 IO: OUT[1,0,0,1,0,1,0,1]
//IO口等待输入指令，举例IO: WAITIN[1,0,1,0,1,1,1,1
//写PLC指令，举例 PLC: WRITEPLC[20] DATA16[2]
//等待PLC指令，举例 PLC: WAITPLC[20] DATA16[2]
//寻位指令，举例 SEARCH: MOVL[1.3,32.7,45,66,7,89,3,0,0,0] SPEED[25] TCP[0] SIDE[0] SIDEMOVE[1,1,1] SIDESPEED[25] NAME[寻位的点point1]
//工件坐标系定位，举例 COORD: POINTX[寻位的点point1] POINTO[寻位的点point3] CPOINTX[基准点point1],CPOINTO[基准点point2],NAME[零位矩阵]
//扫描的焊缝坐标值，举例 GETPOS: TIME[1000] POINTNAME[寻位的点point1] ADD[1,2,3]
//点位移动指令，举例 SMOV: SPEED[25] SMOVL[寻位的点point1] TCP[1] CHANGE[矩阵1]
//点位采集指令，举例 SSCAN: SMOVL[寻位的点point1] SPEED[25] TCP[0] NAME[扫描第一条line]
//寻位末尾指令，举例 SEARCHEND: MOVL[1.3,32.7,45,66,7,89,3,0,0,0] SPEED[25] TCP[0] NAME[寻位的点point1]
//轨迹点采样指令，举例 SAMPLE: CREAT[跟踪第一条line] SPEED[25] TIME[16] NAME[跟踪第一条line采样结果]
//跟踪焊接轨迹工艺指令，举例 TRACING: TCP[0] NAME[焊接轨迹1] TIME[100]
//跟踪轨迹相加指令，举例 TRACEADD: TRACEADD[第一条，第二条] NAME[焊接轨迹1] SPEED[25] TIME[16]
//前往起弧点指令，举例 GOWELD: TCP[0] SPEED[25] NAME[焊接轨迹1]
//摆焊指令，举例 WAVE: TRACE[第一条] WAVE[1,2,3,4,5,6,7,8,9,10,11,12,0,0,0,0,0,0] NAME[摆焊轨迹]
//模拟量输出指令，举例 AOUT: AOUT[0.1,0.2,0.3,0.4]
//多点位生成焊接轨迹指令, 举例 CREATP: POINTS[点位1，点位2，点位3，点位4] NAME[跟踪第一条line]
//重设点位的姿态指令, 举例 SETPOSE: POINT[点位1] POSE[1.2,1.3,1.6] ADD[1,2,3] NAME[点位2]
//将当前TCP获取为点坐标指令, 举例 GETTCPPOS: NAME[寻位的点point1] ADD[1,2,3]
//生成一个TCP数值的点坐标指令，举例 GETTCPPOS2: POS[1.3,32.7,45,66,7,89,3,0,0,0] NAME[寻位的点point1]
//载入文件焊接轨迹指令, 举例 CREATF: FILE[文件路径] NAME[焊接轨迹1]
//用线计算点的指令, 举例 PLOTPOS: MODE[0] CREATS[第一条,第二条,第三条] NAME[点位3]
//用五点计算点的指令, 举例 PLOTPOS: MODE[1] POINTS[点位1，点位2，点位3，点位4，点位5] NAME[交点]
//三点生成圆弧焊接轨迹指令，举例 CREATC: POINTS[点位1，点位2，点位3] SAMPLESPEED[25] SPEED[25] TIME[16] NAME[跟踪第一条line]
//生成点位附近继续焊接的轨迹指令，举例 TRACECONTINUE: TRACE[第一条] POS[1.3,32.7,45,66,7,89,3,0,0,0] NAME[焊接轨迹]
//滤波指令，举例 FILTER: CREAT[跟踪第一条line] MODE[1] NAME[滤波后轨迹1] FILTERS[1,2,3,4,5,6,7,8]
//程序跳转指令，举例 GOTO: LINE[13]
//程序结束指令，举例 STOP:
//跟踪轨迹相加指令，举例 CREATADD: CREATS[轨迹1,轨迹2,轨迹3] NAME[跟踪轨迹]
//点加跟踪轨迹指令, 举例 CREATADDP: MODE[0] POINT[点位1] CREAT[轨迹1] NAME[跟踪轨迹]
//保存轨迹指令，举例 SAVEPCD: SCAN[扫描第一条line,第二条,第三] POINTS[点位1，点位2，点位3，点位4，点位5] CREATS[第一条,第二条,第三条] TRACES[第一条,第二条,第三条] MODE[0] NAME[保存]


//key项
#define CMD_MOV_KEY                     "MOV:"              //移动命令集合KEY
#define CMD_SMOV_KEY                    "SMOV:"             //点位移动命令集合KEY
#define CMD_DELAY_KEY                   "DELAY:"            //延时命令集合KEY
#define CMD_CAM_KEY                     "CAM:"              //激光命令集合KEY
#define CMD_WELD_KEY                    "WELD:"             //焊机命令集合KEY
#define CMD_SCAN_KEY                    "SCAN:"             //采集命令集合KEY
#define CMD_SSCAN_KEY                   "SSCAN:"            //点位采集命令集合KEY
#define CMD_CRAFTS_KEY                  "CRAFTS:"           //创建工艺命令集合KEY
#define CMD_TRACE_KEY                   "TRACE:"            //生成跟踪轨迹工艺命令集合KEY
#define CMD_TRACE2_KEY                  "TRACE2:"           //生成跟踪轨迹工艺命令集合KEY
#define CMD_CREAT_KEY                   "CREAT:"            //生成轨迹命令KEY
#define CMD_IO_KEY                      "IO:"               //IO命令集合KEY
#define CMD_PLC_KEY                     "PLC:"              //PLC命令集合KEY
#define CMD_SEARCH_KEY                  "SEARCH:"           //寻位命令集合KEY
#define CMD_SEARCHEND_KEY               "SEARCHEND:"        //寻位末尾命令集合KEY
#define CMD_COORD_KEY                   "COORD:"            //工件坐标系定位命令集合KEY
#define CMD_GETPOS_KEY                  "GETPOS:"           //获取扫描的焊缝坐标值命令集合KEY
#define CMD_SAMPLE_KEY                  "SAMPLE:"           //轨迹点采样命令集合KEY
#define CMD_TRACING_KEY                 "TRACING:"          //跟踪轨迹轨迹工艺命令集合KEY
#define CMD_TRACEADD_KEY                "TRACEADD:"         //生成跟踪轨迹工艺相加命令集合KEY
#define CMD_GOWELD_KEY                  "GOWELD:"           //前往起弧点命令集合KEY
#define CMD_WAVE_KEY                    "WAVE:"             //摆焊命令集合KEY
#define CMD_AOUT_KEY                    "AOUT:"             //模拟量输出命令集合KEY
#define CMD_CREATP_KEY                  "CREATP:"           //利用点位生成轨迹
#define CMD_SETPOSE_KEY                 "SETPOSE:"          //点坐标设姿态
#define CMD_GETTCPPOS_KEY               "GETTCPPOS:"        //获取TCP的点坐标命令集合KEY
#define CMD_CREATF_KEY                  "CREATF:"           //载入文件焊接轨迹命令集合KEY
#define CMD_PLOTPOS_KEY                 "PLOTPOS:"          //计算点命令集合KEY
#define CMD_CREATC_KEY                  "CREATC:"           //三点生成圆弧焊接轨迹命令集合KEY
#define CMD_GETTCPPOS2_KEY              "GETTCPPOS2:"       //生成一个TCP数值的点坐标命令集合KEY
#define CMD_TRACECONTINUE_KEY           "TRACECONTINUE:"    //生成点位附近继续焊接的轨迹命令集合KEY
#define CMD_FILTER_KEY                  "FILTER:"           //扫描轨迹滤波命令集合KEY
#define CMD_GOTO_KEY                    "GOTO:"             //程序跳转命令集合KEY
#define CMD_STOP_KEY                    "STOP:"             //程序运行结束命令集合KEY
#define CMD_CREATADD_KEY                "CREATADD:"         //跟踪轨迹相加命令集合KEY
#define CMD_CREATADDP_KEY               "CREATADDP:"        //点位与跟踪轨迹相加命令集合KEY
#define CMD_SAVEPCD_KEY                 "SAVEPCD:"          //保存轨迹命令集合KEY


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
#define CMD_ELEV                            "ELEV"                //电压         单位V
#define CMD_NAME                            "NAME"                //命名
#define CMD_SCAN                            "SCAN"                //扫描轨迹参数
#define CMD_MODE                            "MODE"                //模式参数
#define CMD_CRAFT                           "CRAFT"               //工艺文件路径
#define CMD_CRAFTS                          "CRAFTS"              //工艺文件名字
#define CMD_OUT                             "OUTIO"               //IO输出
#define CMD_WAITIN                          "WAITINIO"            //等待IO输入
#define CMD_WRITEPLC                        "WRITEPLC"            //写PLC寄存器
#define CMD_WAITPLC                         "WAITPLC"             //等待PLC寄存器输入
#define CMD_DATA16                          "DATA16"              //16位整形数字
#define CMD_DATA32FS                        "DATA32FS"            //32位浮点形数组
#define CMD_SIDE                            "SIDE"                //寻位寻找两侧的范围
#define CMD_SIDEMOVE                        "SIDEMOVE"            //寻位寻找两侧的单位向量
#define CMD_SIDESPEED                       "SIDESPEED"           //寻位寻找两侧的空闲移动速度
#define CMD_POINTX                          "POINTX"              //实际零位矩阵零点的X方向基准点
#define CMD_POINTO                          "POINTO"              //实际零位矩阵零点基准点
#define CMD_CPOINTX                         "CPOINTX"             //基准零位矩阵零点的X方向基准点
#define CMD_CPOINTO                         "CPOINTO"             //基准零位矩阵零点基准点
#define CMD_CREAT                           "CREAT"               //跟踪轨迹参数
#define CMD_TRACEADD                        "TRACEADD"            //跟踪轨迹相加参数
#define CMD_WAVE                            "WAVE"                //摆焊参数
#define CMD_TRACE                           "TRACE"               //跟踪轨迹
#define CMD_TRACES                          "TRACES"              //跟踪轨迹数组
#define CMD_AOUT                            "AOUT"                //模拟量输出
#define CMD_ADD                             "ADD"                 //补偿
#define CMD_POINTS                          "POINTS"              //点位参数
#define CMD_POINTADDS                       "POINTADDS"           //带补偿的点位参数
#define CMD_POINT                           "POINT"               //点坐标名字
#define CMD_POSE                            "POSE"                //姿态名字
#define CMD_FILE                            "FILE"                //轨迹文件名字
#define CMD_CREATS                          "CREATS"              //生成点用的轨迹名字         
#define CMD_POS                             "POS"                 //机器人坐标
#define CMD_FILTERS                         "FILTERS"             //滤波项参数
#define CMD_SAMPLESPEED                     "SAMPLESPEED"         //连接工艺轨迹时的连接处采样速度
#define CMD_LINE                            "LINE"                //行参数



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
    QString cmd_elec(float eled,float elev,Alternatingcurrent elem,int work);//焊机启停命令不
    QString cmd_elec_work(int work);//焊机启停命令
    QString cmd_scan(RobPos pos,Robmovemodel movemodel,float speed,int tcp,QString name,QString change="");//采集命令
    QString cmd_scanC(RobPos pos1,RobPos pos2,RobPos pos3,Robmovemodel movemodel,float speed,int tcp,QString name,QString change="");//圆采集命令
    QString cmd_sscan(QString s_pos,Robmovemodel movemodel,float speed,int tcp,QString name,QString change="");//点位采集命令
    QString cmd_sscanC(QString s_pos1,QString s_pos2,QString s_pos3,Robmovemodel movemodel,float speed,int tcp,QString name,QString change="");//点位圆采集命令
    QString cmd_trace(QString name_in,float speed,QString craftfilepath,QString name_out,QString change="");//生成跟踪工艺轨迹命令
    QString cmd_trace2(QString name_in,float speed,QString crafts,QString name_out,QString change="");//生成跟踪工艺轨迹命令
    QString cmd_creat(Trace_edit_mode mode,std::vector<QString> scanname,QString name);//生成跟踪轨迹
    QString cmd_ioout(std::vector<int> io);//输出IO信号
    QString cmd_iowaitin(std::vector<int> io);//等待输入IO信号
    QString cmd_plcwait(int register_add,int16_t register_data);
    QString cmd_plcwrite(int register_add,int16_t register_data);
    QString cmd_search(RobPos pos,Robmovemodel movemodel,float speed,int tcp,int side,std::vector<float> sidemove,float sidespeed,QString name,QString change="");//寻位命令
    QString cmd_searchC(RobPos pos1,RobPos pos2,RobPos pos3,Robmovemodel movemodel,float speed,int tcp,int side,std::vector<float> sidemove,float sidespeed,QString name,QString change="");//圆寻位命令
    QString cmd_searchend(RobPos pos,Robmovemodel movemodel,float speed,int tcp,QString name,QString change="");//寻位末尾命令
    QString cmd_searchendC(RobPos pos1,RobPos pos2,RobPos pos3,Robmovemodel movemodel,float speed,int tcp,QString name,QString change="");//寻位末尾命令
    QString cmd_coord(QString s_pointX,QString s_pointO,QString s_cpointX,QString s_cpointO,QString name);//生成定位变化矩阵
    QString cmd_getpos(int time,QString name,std::vector<float> add);//获取扫描的焊缝坐标值命令
    QString cmd_sample(QString name_in,float speed,int time,QString name_out);//采样轨迹点命令
    QString cmd_tracing(QString name,int tcp,int time);//跟踪焊接轨迹工艺命令
    QString cmd_traceadd(QString name1,QString name2,QString name_out,bool b_link,float linkspeed=0,float linksamplespeed=0,int linktime=0);//跟踪焊接轨迹相加命令
    QString cmd_goweld(int tcp,float speed,QString name);//前往起弧点命令
    QString cmd_wave(QString name_in,wWAVEParam cmd_wave_info,QString name_out);//摆焊指令
    QString cmd_aout(std::vector<float> a);//输出模拟量
    QString cmd_creatp(std::vector<QString> pointsname,QString name);//利用点位生成轨迹
    QString cmd_setpose(QString name_in,std::vector<float> pose,std::vector<float> add,QString name_out);//重设点位的姿态
    QString cmd_gettcppos(QString name,std::vector<float> add);//获取tcp的点坐标值命令
    QString cmd_creatf(QString filename,QString name);//利用文件生成轨迹
    QString cmd_plotpos(Plotpos_edit_mode mode,std::vector<QString> weldname,std::vector<QString> pointname,QString posname);//生成点的方法命令
    QString cmd_creatc(std::vector<QString> pointsname,float speed,int time,QString name);//利用点位生成圆弧
    QString cmd_gettcppos2(RobPos pos,QString name);//生成tcp数值的点坐标值命令
    QString cmd_tracecontinue(QString name_in,RobPos pos,QString name_out);//生成点位附近继续焊接的轨迹指令
    QString cmd_filter(QString name_in,Filter_mode mode,filterParam filters,QString name_out);//扫描轨迹滤波指令
    QString cmd_goto(int line);//前往第几行执行
    QString cmd_stop();//程序结束指令
    QString cmd_creatadd(std::vector<QString> names,QString name_out);//跟踪轨迹相加
    QString cmd_creataddp(QString weldname,QString pointname,Creataddp_edit_mode mode,QString name_out);//点位和轨迹相加
    QString cmd_crafts(Craft_ID craft_id,std::vector<ChangeRobPosVariable> posturelist,std::vector<float> params,QString name);//创建焊接工艺指令
    QString cmd_savepcd(std::vector<QString> scanname,std::vector<QString> pointsname,std::vector<QString> creatsname,std::vector<QString> tracesname,Savepcd_edit_mode mode,QString name);


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
    float cmd_elec_elev; //获取到焊机电压

    RobPos cmd_scan_pos;//获取到扫描终点坐标
    RobPos cmd_scan_pos1;//获取到圆扫描起点坐标
    RobPos cmd_scan_pos2;//获取到圆扫描途径点坐标
    RobPos cmd_scan_pos3;//获取到圆扫描终点坐标
    float cmd_scan_speed;//获取到的扫描速度
    int cmd_scan_tcp;//获取到扫描TCP
    Robmovemodel cmd_scan_movemod;//获取到的扫描模式
    QString cmd_scan_name;//获取到的扫描轨迹名字
    QString cmd_scan_change;//获取到的扫描变换矩阵名字

    QString cmd_trace_name_in;//获取到跟踪轨迹名字
    QString cmd_trace_name_out;//生成的焊接工艺轨迹名字
    float cmd_trace_speed;//获取到的跟踪速度
    QString cmd_trace_change;//获取到的扫描变换矩阵名字
    QString cmd_trace_craftfilepath;//获取到的焊接工艺包路径

    QString cmd_trace2_name_in;//获取到跟踪轨迹名字
    QString cmd_trace2_name_out;//生成的焊接工艺轨迹名字
    float cmd_trace2_speed;//获取到的跟踪速度
    QString cmd_trace2_change;//获取到的扫描变换矩阵名字
    QString cmd_trace2_crafts;//获取到的焊接工艺包路径

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
    QString cmd_coord_cpointx;//基准零位矩阵零点的X方向基准点
    QString cmd_coord_cpointo;//基准零位矩阵零点的基准点
    QString cmd_coord_name;//实际零位矩阵名字

    int cmd_getpos_time=0;//获取坐标时的等待时间
    QString cmd_getpos_name;//获取坐标的名字
    std::vector<float> cmd_getpos_add;//获取补偿

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

    QString cmd_sample_creatname;//获取到的采样原轨迹名字
    QString cmd_sample_name;//获取到的采样结果名字
    float cmd_sample_speed;//获取到的采样点移动速度mm/s
    int cmd_sample_time;//获取到的采样点之间的时间间隔ms

    QString cmd_tracing_name;//获取到的跟踪工艺轨迹名字
    int cmd_tracing_tcp;//获取到的跟踪工艺轨迹tcp
    int cmd_tracing_time;//密集采样点发送间隔

    QString cmd_traceadd_name1;//获取到的跟踪工艺轨迹1名字
    QString cmd_traceadd_name2;//获取到的跟踪工艺轨迹2名字
    QString cmd_traceadd_nameout;//获取到的跟踪工艺轨迹1+2的名字
    bool cmd_traceadd_samplelink;//是否要在连接处采样
    float cmd_traceadd_speed;//获取到的连接处的焊接移动速度mm/s
    float cmd_traceadd_samplespeed;//获取到的连接处的采样点移动速度mm/s
    int cmd_traceadd_time;//获取到的连接处的采样点之间的时间间隔ms

    int cmd_goweld_tcp; //获得到起弧点tcp
    float cmd_goweld_speed;//获得到起弧点速度
    QString cmd_goweld_name;//获得到起弧点跟踪工艺轨迹名字

    QString cmd_wave_namein;//获取到的跟踪原轨迹
    wWAVEParam cmd_wave_info;//获得到的摆焊信息
    QString cmd_wave_nameout;//获取到的摆焊轨迹

    std::vector<float> cmd_aout_output;//获取到的A模拟量输出口信息

    std::vector<QString> cmd_creatp_pointsname;//获取到的点位名字
    QString cmd_creatp_name;//获取到的轨迹名字

    QString cmd_setpose_namein;//获得重设点位姿态名字
    QString cmd_setpose_nameout;//获得重设点位姿态名字结果
    std::vector<float> cmd_setpose_pose;//获得重设点位姿态
    std::vector<float> cmd_setpose_add;//获得重设点位姿态补偿


    QString cmd_gettcppos_name;//获取tcp坐标的名字
    std::vector<float> cmd_gettcppos_add;//获取tcp坐标补偿

    QString cmd_creatf_filename;//获取到的轨迹文件路径
    QString cmd_creatf_name;//获取到的轨迹名字

    Plotpos_edit_mode cmd_plotpos_mode;//获取到的生成点的模式
    QString cmd_plotpos_name;//获取到的生成的点名字
    std::vector<QString> cmd_plotpos_creatname;//获取到生成轨迹所需要的轨迹名字
    std::vector<QString> cmd_plotpos_pointsname;//获取到生成点所需要的点名字

    std::vector<QString> cmd_creatc_pointsname;//获取到的圆点位名字
    QString cmd_creatc_name;//获取到的圆弧轨迹名字
    float cmd_creatc_speed;//获取到的采样点移动速度mm/s
    int cmd_creatc_time;//获取到的采样点之间的时间间隔ms

    QString cmd_gettcppos2_name;//获取tcp坐标的名字
    RobPos cmd_gettcppos2_pos;//获取tcp坐标

    QString cmd_tracecontinue_namein;//获取继续焊接的原轨迹
    RobPos cmd_tracecontinue_pos;//获取继续焊接起点
    QString cmd_tracecontinue_nameout;//获取继续轨迹

    QString cmd_filter_namein;//获取到的扫描轨迹名字
    Filter_mode cmd_filter_mode;//获取到的扫描滤波模式
    filterParam cmd_filters;//获取到的滤波参数
    QString cmd_filter_nameout;//生成的滤波轨迹名字

    std::vector<QString> cmd_creatadd_names;//获取到的跟踪轨迹名字
    QString cmd_creatadd_nameout;//生成的跟踪轨迹名字

    QString cmd_creataddp_pointname;//点位名字
    QString cmd_creataddp_weldname;//跟踪轨迹名字
    Creataddp_edit_mode cmd_creataddp_mode;//相加模式
    QString cmd_creataddp_nameout;//生成的跟踪轨迹名字

    Craft_ID cmd_crafts_craft_id;      //工艺类型
    QString cmd_crafts_name;           //工艺名字
    std::vector<ChangeRobPosVariable> cmd_crafts_posturelist;//工艺姿态
    std::vector<float> cmd_crafts_params;   //工艺参数

    std::vector<QString> cmd_savepcd_scanname;//保存点云扫描轨迹
    std::vector<QString> cmd_savepcd_pointsname;//保存点云点轨迹
    std::vector<QString> cmd_savepcd_creatsname;//保存点云焊缝轨迹
    std::vector<QString> cmd_savepcd_tracesname;//保存点云工艺轨迹
    Savepcd_edit_mode cmd_savepcd_mode;//保存文件名模式
    QString cmd_savepcd_name;//保存文件名称

    int cmd_goto_line;//获取到的跳转行数

    int cmd_run_work;//停止程序


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
    QString rc_elev(float elev);
    QString rc_elem(Alternatingcurrent elem);
    QString rc_name(QString name);
    QString rc_mode(int mode);
    QString rc_scan(std::vector<QString> names);
    QString rc_craft(QString craftfilepath);
    QString rc_crafts(QString crafts);
    QString rc_ioout(std::vector<int> io);
    QString rc_iowaitin(std::vector<int> io);
    QString rc_plcwait(int register_add);
    QString rc_plcwrite(int register_add);
    QString rc_16data(int16_t i16_data);
    QString rc_f32datas(std::vector<float> f32_datas);
    QString rc_side(int side);
    QString rc_sidemove(std::vector<float> sidemove);
    QString rc_sidespeed(float speed);
    QString rc_pointX(QString s_pointX);
    QString rc_pointO(QString s_pointO);
    QString rc_cpointX(QString s_cpointX);
    QString rc_cpointO(QString s_cpointO);
    QString rc_creat(QString names);
    QString rc_traceadd(QString name1,QString name2);
    QString rc_wave(wWAVEParam cmd_wave_info);
    QString rc_trace(QString name);
    QString rc_traces(std::vector<QString> names);
    QString rc_aout(std::vector<float> a);
    QString rc_add(std::vector<float> add);
    QString rc_points(std::vector<QString> pointsname);
    QString rc_point(QString name);
    QString rc_pose(std::vector<float> pose);
    QString rc_file(QString filename);
    QString rc_creats(std::vector<QString> names);
    QString rc_plot(Plotpos_edit_mode mode,std::vector<QString> weldnames,std::vector<QString> pointnames);
    QString rc_pos(RobPos pos);
    QString rc_filters(filterParam filters,Filter_mode mode);
    QString rc_samplespeed(float speed);
    QString rc_line(int line);
    QString rc_pointadds(std::vector<ChangeRobPosVariable> posturelist);


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
