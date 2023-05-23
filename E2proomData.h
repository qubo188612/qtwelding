#ifndef E2PROOMDATA_H
#define E2PROOMDATA_H

#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>
#include <QByteArray>
#include "global.h"
#include <string>
#if _MSC_VER
#include <QDir>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif
#include "tistdtypes.h"
#include "FileOut.h"
#include <Eigen/Core>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>

#define E2POOM_SSHDLG_SYSPATH_MOTO          "./SAVE/E2P_SSHDLG.bsd"

#define E2POOM_CAMDLG_SAVEBUFF              64
#define E2POOM_CAMDLG_SYSPATH_MOTO			"./SAVE/E2P_CAMDLG.bsd"

#define E2POOM_CAMDLG_MODPOSX1_MIN          0
#define E2POOM_CAMDLG_MODPOSX1_MAX          100000
#define E2POOM_CAMDLG_MODPOSX1_USE          0
#define E2POOM_CAMDLG_MODPOSY1_MIN          0
#define E2POOM_CAMDLG_MODPOSY1_MAX          100000
#define E2POOM_CAMDLG_MODPOSY1_USE          8000
#define E2POOM_CAMDLG_MODPOSX2_MIN          0
#define E2POOM_CAMDLG_MODPOSX2_MAX          100000
#define E2POOM_CAMDLG_MODPOSX2_USE          2750
#define E2POOM_CAMDLG_MODPOSY2_MIN          0
#define E2POOM_CAMDLG_MODPOSY2_MAX          100000
#define E2POOM_CAMDLG_MODPOSY2_USE          0
#define E2POOM_CAMDLG_MODPOSX3_MIN          0
#define E2POOM_CAMDLG_MODPOSX3_MAX          100000
#define E2POOM_CAMDLG_MODPOSX3_USE          5750
#define E2POOM_CAMDLG_MODPOSY3_MIN          0
#define E2POOM_CAMDLG_MODPOSY3_MAX          100000
#define E2POOM_CAMDLG_MODPOSY3_USE          0
#define E2POOM_CAMDLG_MODPOSX4_MIN          0
#define E2POOM_CAMDLG_MODPOSX4_MAX          100000
#define E2POOM_CAMDLG_MODPOSX4_USE          8500
#define E2POOM_CAMDLG_MODPOSY4_MIN          0
#define E2POOM_CAMDLG_MODPOSY4_MAX          100000
#define E2POOM_CAMDLG_MODPOSY4_USE          8000

#define E2POOM_CAMDLG_CVIMG_POSX1_MIN       0
#define E2POOM_CAMDLG_CVIMG_POSX1_MAX       CAMBUILD_IMAGE_WIDTH-1
#define E2POOM_CAMDLG_CVIMG_POSX1_USE       0
#define E2POOM_CAMDLG_CVIMG_POSY1_MIN       0
#define E2POOM_CAMDLG_CVIMG_POSY1_MAX       CAMBUILD_IMAGE_HEIGHT-1
#define E2POOM_CAMDLG_CVIMG_POSY1_USE       0
#define E2POOM_CAMDLG_CVIMG_POSX2_MIN       0
#define E2POOM_CAMDLG_CVIMG_POSX2_MAX       CAMBUILD_IMAGE_WIDTH-1
#define E2POOM_CAMDLG_CVIMG_POSX2_USE       0
#define E2POOM_CAMDLG_CVIMG_POSY2_MIN       0
#define E2POOM_CAMDLG_CVIMG_POSY2_MAX       CAMBUILD_IMAGE_HEIGHT-1
#define E2POOM_CAMDLG_CVIMG_POSY2_USE       0
#define E2POOM_CAMDLG_CVIMG_POSX3_MIN       0
#define E2POOM_CAMDLG_CVIMG_POSX3_MAX       CAMBUILD_IMAGE_WIDTH-1
#define E2POOM_CAMDLG_CVIMG_POSX3_USE       0
#define E2POOM_CAMDLG_CVIMG_POSY3_MIN       0
#define E2POOM_CAMDLG_CVIMG_POSY3_MAX       CAMBUILD_IMAGE_HEIGHT-1
#define E2POOM_CAMDLG_CVIMG_POSY3_USE       0
#define E2POOM_CAMDLG_CVIMG_POSX4_MIN       0
#define E2POOM_CAMDLG_CVIMG_POSX4_MAX       CAMBUILD_IMAGE_WIDTH-1
#define E2POOM_CAMDLG_CVIMG_POSX4_USE       0
#define E2POOM_CAMDLG_CVIMG_POSY4_MIN       0
#define E2POOM_CAMDLG_CVIMG_POSY4_MAX       CAMBUILD_IMAGE_HEIGHT-1
#define E2POOM_CAMDLG_CVIMG_POSY4_USE       0

#define E2POOM_DEMDLG_SYSPATH_MOTO			"./SAVE/E2P_DEMDLG.bsd"
#define E2POOM_DEMDLG_RADIO_MOD_MIN         0
#define E2POOM_DEMDLG_RADIO_MOD_MAX         1
#define E2POOM_DEMDLG_RADIO_MOD_USE         0

#define E2POOM_MAINDLG_SAVEBUFF             24
#define E2POOM_MAINDLG_SYSPATH_MOTO         "./SAVE/E2P_MAINDLG.bsd"
#define E2POOM_MAINDLG_SAVEDATA_MIN         0
#define E2POOM_MAINDLG_SAVEDATA_MAX         1
#define E2POOM_MAINDLG_SAVEDATA_USE         0
#define E2POOM_MAINDLG_WELDELED_MIN         0
#define E2POOM_MAINDLG_WELDELED_USE         0
#define E2POOM_MAINDLG_WELDELEM_MIN         0
#define E2POOM_MAINDLG_WELDELEM_MAX         1
#define E2POOM_MAINDLG_WELDELEM_USE         0
#define E2POOM_MAINDLG_MOVESPEED_MIN        0
#define E2POOM_MAINDLG_MOVESPEED_USE        10
#define E2POOM_MAINDLG_MOVESPEED_MAX        200
#define E2POOM_MAINDLG_MOVETCP_MIN          0
#define E2POOM_MAINDLG_MOVETCP_USE          0
#define E2POOM_MAINDLG_MOVETCP_MAX          ROBOTTCPNUM
#define E2POOM_MAINDLG_CIRCLERUN_MIN        0
#define E2POOM_MAINDLG_CIRCLERUN_MAX        1
#define E2POOM_MAINDLG_CIRCLERUN_USE        0

#define E2POOM_SETPROJECTDLG_SAVEBUFF                8
#define E2POOM_SETPROJECTDLG_SYSPATH_MOTO            "./SAVE/E2P_SETPROJECTDLG.bsd"
#define E2POOM_SETPROJECTDLG_CTLMOVESPEED_MIN        0
#define E2POOM_SETPROJECTDLG_CTLMOVESPEED_USE        10
#define E2POOM_SETPROJECTDLG_CTLMOVESPEED_MAX        200
#define E2POOM_SETPROJECTDLG_CTLMOVETCP_MIN          0
#define E2POOM_SETPROJECTDLG_CTLMOVETCP_USE          0
#define E2POOM_SETPROJECTDLG_CTLMOVETCP_MAX          ROBOTTCPNUM


class E2proomData
{
public:
    E2proomData();
    ~E2proomData();
/***************************/
    //相机升级界面
    QString sshdlg_usename;
    QString sshdlg_password;
    void write_sshdlg_para();
    void read_sshdlg_para();
    void init_sshdlg_para();
    QVariantHash sshdlg_enjson();
    int sshdlg_dejson(QByteArray allData);

    //相机内参标定界面
    Int32 camdlg_modposX1;
    Int32 camdlg_modposY1;
    Int32 camdlg_modposX2;
    Int32 camdlg_modposY2;
    Int32 camdlg_modposX3;
    Int32 camdlg_modposY3;
    Int32 camdlg_modposX4;
    Int32 camdlg_modposY4;

    Int32 camdlg_cvimg_posX1;
    Int32 camdlg_cvimg_posY1;
    Int32 camdlg_cvimg_posX2;
    Int32 camdlg_cvimg_posY2;
    Int32 camdlg_cvimg_posX3;
    Int32 camdlg_cvimg_posY3;
    Int32 camdlg_cvimg_posX4;
    Int32 camdlg_cvimg_posY4;

    void read_camdlg_para();
    void write_camdlg_para();
    void init_camdlg_para();

    Int32 camdlg_modposX1_min;
    Int32 camdlg_modposX1_max;
    Int32 camdlg_modposX1_use;
    Int32 camdlg_modposY1_min;
    Int32 camdlg_modposY1_max;
    Int32 camdlg_modposY1_use;
    Int32 camdlg_modposX2_min;
    Int32 camdlg_modposX2_max;
    Int32 camdlg_modposX2_use;
    Int32 camdlg_modposY2_min;
    Int32 camdlg_modposY2_max;
    Int32 camdlg_modposY2_use;
    Int32 camdlg_modposX3_min;
    Int32 camdlg_modposX3_max;
    Int32 camdlg_modposX3_use;
    Int32 camdlg_modposY3_min;
    Int32 camdlg_modposY3_max;
    Int32 camdlg_modposY3_use;
    Int32 camdlg_modposX4_min;
    Int32 camdlg_modposX4_max;
    Int32 camdlg_modposX4_use;
    Int32 camdlg_modposY4_min;
    Int32 camdlg_modposY4_max;
    Int32 camdlg_modposY4_use;

    Int32 camdlg_cvimg_posX1_min;
    Int32 camdlg_cvimg_posX1_max;
    Int32 camdlg_cvimg_posX1_use;
    Int32 camdlg_cvimg_posY1_min;
    Int32 camdlg_cvimg_posY1_max;
    Int32 camdlg_cvimg_posY1_use;
    Int32 camdlg_cvimg_posX2_min;
    Int32 camdlg_cvimg_posX2_max;
    Int32 camdlg_cvimg_posX2_use;
    Int32 camdlg_cvimg_posY2_min;
    Int32 camdlg_cvimg_posY2_max;
    Int32 camdlg_cvimg_posY2_use;
    Int32 camdlg_cvimg_posX3_min;
    Int32 camdlg_cvimg_posX3_max;
    Int32 camdlg_cvimg_posX3_use;
    Int32 camdlg_cvimg_posY3_min;
    Int32 camdlg_cvimg_posY3_max;
    Int32 camdlg_cvimg_posY3_use;
    Int32 camdlg_cvimg_posX4_min;
    Int32 camdlg_cvimg_posX4_max;
    Int32 camdlg_cvimg_posX4_use;
    Int32 camdlg_cvimg_posY4_min;
    Int32 camdlg_cvimg_posY4_max;
    Int32 camdlg_cvimg_posY4_use;
 /***************************/
    //手眼标定页面
    Eye_Hand_calibrationmode demdlg_radio_mod;     //0:眼在手上，1:眼在手外
    Eigen::Matrix3d demdlg_R;          //眼在手外旋转矩阵
    Eigen::Vector3d demdlg_T;          //眼在手外平移变量
    std::vector<RobPos> demdlg_Robotpos;  //机器人手眼标定点
    std::vector<TCP_Leaserpos> demdlg_Leaserpos; //激光头手眼标定点

    cv::Mat matrix_camera2plane;        //眼在手上旋转矩阵1
    cv::Mat matrix_plane2robot;         //眼在手上旋转矩阵2

    Int32 demdlg_radio_mod_min;
    Int32 demdlg_radio_mod_max;
    Int32 demdlg_radio_mod_use;
    void read_demdlg_para();
    void write_demdlg_para();
    void init_demdlg_para();
 /***************************/
    //主页面
    Int32 maindlg_SaveDatacheckBox;             //0:不保存轨道处理数据，1:保存轨道处理数据
    float maindlg_Weldeled;                     //点焊电流
    Alternatingcurrent_ID maindlg_Weldelem;     //点焊工艺
    float maindlg_movespeed;                    //移动速度
    Int32 maindlg_movetcp;                      //TCP
    Int32 maindlg_circlerun;                    //循环执行

    void read_maindlg_para();
    void write_maindlg_para();
    void init_maindlg_para();

    Int32 maindlg_SaveDatacheckBox_min;
    Int32 maindlg_SaveDatacheckBox_max;
    Int32 maindlg_SaveDatacheckBox_use;
    float maindlg_Weldeled_min;
    float maindlg_Weldeled_use;
    float maindlg_Weldelem_min;
    Int32 maindlg_Weldelem_max;
    Int32 maindlg_Weldelem_use;
    float maindlg_movespeed_min;
    float maindlg_movespeed_max;
    float maindlg_movespeed_use;
    Int32 maindlg_movetcp_min;
    Int32 maindlg_movetcp_max;
    Int32 maindlg_movetcp_use;
    Int32 maindlg_circlerun_min;
    Int32 maindlg_circlerun_max;
    Int32 maindlg_circlerun_use;

 /***************************/
    //工程编辑页面
    float setprojectdlg_ctlmovespeed;                 //移动速度
    Int32 setprojectdlg_ctlmovetcp;                      //TCP

    void read_setprojectdlg_para();
    void write_setprojectdlg_para();
    void init_setprojectdlg_para();

    float setprojectdlg_ctlmovespeed_min;
    float setprojectdlg_ctlmovespeed_max;
    float setprojectdlg_ctlmovespeed_use;
    Int32 setprojectdlg_ctlmovetcp_min;
    Int32 setprojectdlg_ctlmovetcp_max;
    Int32 setprojectdlg_ctlmovetcp_use;

 /***************************/
    void write();
private:
    void read_para();
    void check_para();

    int Savejsonfile(char* filename,QVariantHash data);    //保存json文件
    int Loadjsonfile(char* filename,QJsonDocument &jsonDoc);    //读取json文件
};

#endif // E2PROOMDATA_H
