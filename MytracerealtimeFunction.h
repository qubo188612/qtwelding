#ifndef MYTRACEREALTIMEFUNCTION_H
#define MYTRACEREALTIMEFUNCTION_H

#include "global.h"

//实时跟踪滤波函数
class Mytracerealtime{
public:
    Mytracerealtime();
    ~Mytracerealtime();

    //返回值-1:结束
    //返回值0:正常
    //返回值1:点检测失败
                                                //PCA滤波(擅长主方向滤波)
    static int Principal_Component_Analysis(std::vector<RobPos> &robpos,    //输入/输出点云
                                            RobPos &realtimepos,            //输出下一个到达的点
                                            int windowsize=5,               //滤波窗函数尺寸
                                            float Threshold=2.0);           //奇异点去除最小距离
};

#endif // MYTRACEREALTIMEFUNCTION_H
