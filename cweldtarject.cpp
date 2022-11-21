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

    //
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
