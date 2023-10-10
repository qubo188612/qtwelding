#include "global.h"

RobPos::RobPos()
{
    X=0;
    Y=0;
    Z=0;
    RX=0;
    RY=0;
    RZ=0;
    out_1=0;
    out_2=0;
    out_3=0;
    nEn=0;
}

leaser_pos::leaser_pos()
{
    Y=0;
    Z=0;
    nEn=0;
}

Pos3f::Pos3f()
{
    X=0;
    Y=0;
    Z=0;
}

bool d_Mysortcompare(d_Mysort &a,d_Mysort &b)
{
    return a.data<b.data;
}
#if _MSC_VER
#else
uint64_t htonll(uint64_t val)
{
    return (((uint64_t) htonl(val)) << 32) + htonl(val >> 32);
}

uint64_t ntohll(uint64_t val)
{
    return (((uint64_t) ntohl(val)) << 32) + ntohl(val >> 32);
}
#endif

bool b_nosame_vector_QString(std::vector<QString> vec)
{
    if(std::set<QString>(vec.begin(),vec.end()).size()!=vec.size())
        return false;
    else
        return true;
}

Point_robpos_result::Point_robpos_result()
{
    nEn=false;
}

Coord_Matrix4d_result::Coord_Matrix4d_result()
{
    nEn=false;
}

Weld_trace_onec::Weld_trace_onec()
{
    Sample=false;
}

Weld_trace_result::Weld_trace_result()
{
    Sample=false;
}

wWAVEParam::wWAVEParam()
{
    period=0;
    leftAmp=0;
    rightAmp=0;
    leftAmp_z=0;
    rightAmp_z=0;
    leftStopTime=0;
    rightStopTime=0;
    anglex=0;
    angley=0;
    startPos=0;
    order=0;
    pendulum_mode=PENDULUM_ID_SIMPLE;
    timeGap=0;
    trend_mode=TREND_MODE_X_Z;
}

filterParam::filterParam()
{
#if USE_PLC_FILTER==1
    msl_poly=1;                   //多项式最高阶,0为平滑，1为一项线性曲线拟合，2为二项线性曲线拟合

    sor_nearpoint_num=50;                //每个点参考的邻域点数量
    sor_standard_deviation=1.0;       //标准差
#endif

    svd_Degree=1;                 //主轴曲线拟合的阶数,0为直线,1为一项线性曲线拟合，2为二项线性曲线拟合
    svd_WindowSize=5;             //曲线拟合的统计点数
    svd_SingularThreshold=2.0;    //距离主轴距离阈值

    gaussian_SmoothingRadius=5.0;      //平滑半径
    gaussian_SmoothingSigma=0.5;       //标准差
}

QString Pendulum_mode_toQString(Pendulum_mode pendulum_mode)
{
    QString msg;
    switch(pendulum_mode)
    {
        case PENDULUM_ID_SIMPLE:
            msg=QStringLiteral("单摆");
        break;
        case PENDULUM_ID_TRIANGLE:
            msg=QStringLiteral("三角摆");
        break;
        case PENDULUM_ID_L:
            msg=QStringLiteral("L摆");
        break;
        case PENDULUM_ID_SINE:
            msg=QStringLiteral("正弦摆");
        break;
        case PENDULUM_ID_CIRCULAR:
            msg=QStringLiteral("椭圆摆");
        break;
    }
    return msg;
}

QString Trend_mode_toQString(Trend_mode trend_mode)
{
    QString msg;
    switch(trend_mode)
    {
        case TREND_MODE_X_Z:
            msg=QStringLiteral("X轴走向,开口朝Z轴正方向");
        break;
        case TREND_MODE_Y_Z:
            msg=QStringLiteral("Y轴走向,开口朝Z轴正方向");
        break;
        case TREND_MODE_Z_Y:
            msg=QStringLiteral("Z轴走向,开口朝Y轴负方向");
        break;
    }
    return msg;
}

QString Trace_edit_mode_toQString(Trace_edit_mode trace_edit_mode)
{
    QString msg;
    switch(trace_edit_mode)
    {
        case TRACE_EDIT_MODE_ONE_TO_ONE:
            msg=QStringLiteral("单扫对单轨道模式");
        break;
        case TRACE_EDIT_MODE_THREE_TO_ONE:
            msg=QStringLiteral("三直线交点模式");
        break;
        case TRACE_EDIT_MODE_TOWPOINT_THREE_TO_ONE:
            msg=QStringLiteral("两端点三直线交点模式");
        break;
    }
    return msg;
}

QString Plotpos_edit_mode_toQString(Plotpos_edit_mode plotpos_edit_mode)
{
    QString msg;
    switch(plotpos_edit_mode)
    {
        case PLOTPOS_EDIT_MODE_THREE_TO_ONE:
            msg=QStringLiteral("三直线交点模式");
        break;
        case PLOTPOS_EDIT_MODE_FIVEPOINTS_TO_ONE:
            msg=QStringLiteral("两点直线与三点交点模式");
        break;
        case PLOTPOS_EDIT_MODE_LINE_THREEPOINTS_TO_ONE:
            msg=QStringLiteral("直线与三点交点模式");
        break;
    }
    return msg;
}

QString Creataddp_edit_mode_toQString(Creataddp_edit_mode creataddp_edit_mode)
{
    QString msg;
    switch(creataddp_edit_mode)
    {
        case CREATADDP_EDIT_MODE_HEAD:
            msg=QStringLiteral("点位放在轨迹头部");
        break;
        case CREATADDP_EDIT_MODE_TAIL:
            msg=QStringLiteral("点位放在轨迹尾部");
        break;
    }
    return msg;
}

QString Filter_mode_toQString(Filter_mode filter_mode)
{
    QString msg;
    switch(filter_mode)
    {
    #if USE_PLC_FILTER==1
        case FILTER_MLS:
            msg=QStringLiteral("MLS滤波");
        break;
        case FILTER_SOR:
            msg=QStringLiteral("SOR滤波");
        break;
    #endif
        case FILTER_SVD:
            msg=QStringLiteral("SVD滤波");
        break;
        case FILTER_GAUSSIAN:
            msg=QStringLiteral("GAUSSIAN滤波");
        break;
    }
    return msg;
}

QString Savepcd_edit_mode_toQString(Savepcd_edit_mode savepcd_mode)
{
    QString msg;
    switch(savepcd_mode)
    {
    case SAVEPCD_EDIT_MODE_TIME:
        msg=QStringLiteral("文件名用时间后缀");
        break;
    case SAVEPCD_EDIT_MODE_STATIC:
        msg=QStringLiteral("文件名无后缀(会替换调之前保存的文件)");
        break;
    }
    return msg;
}

QString Tracerealtime_edit_mode_toQString(Tracerealtime_edit_mode Tracerealtime_mode)
{
    QString msg;
    switch(Tracerealtime_mode)
    {
    case TRACEREALTIME_EDIT_MODE_STOPDOWN:
        msg=QStringLiteral("在起始位置停止后再下枪");
        break;
    case TRACEREALTIME_EDIT_MODE_MOVINEDOWN:
        msg=QStringLiteral("在起始位置边运动边下枪");
        break;
    }
    return msg;
}
