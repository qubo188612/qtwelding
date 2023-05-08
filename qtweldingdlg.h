#ifndef QTWELDINGDLG_H
#define QTWELDINGDLG_H

#include <QDialog>
#include <QDoubleValidator>
#include <QFileDialog>
#include <QTextCodec>
#include <QMutex>
#include "my_parameters.h"
#include "qthread.h"
#if _MSC_VER
#include <QDir>
#include <synchapi.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#endif
#include <vector>
#include "qtmysunnydlg.h"
#include "demarcatedlg.h"
#include "robotsetdlg.h"
#include "weldsetdlg.h"
#include "plcsetdlg.h"
#include "editprojectdlg.h"
#include "newprojectdlg.h"
#include "setprojectdlg.h"
#include "editcraftdlg.h"
#include "newcraftdlg.h"
#include "setcraft0dlg.h"
#include "setcraft1dlg.h"
#include "setcraft2dlg.h"


namespace Ui {
class qtweldingDlg;
}

class qtweldingThread;

class qtgetrobThread;

class qtrecordThread;

//class qtplcThread;

#define ROBOT_MAINDLG_MOVEPOS_STEP             100  //主页机器人移动目标点距离
#define ROBOT_MAINDLG_MOVEPOS_STEPMIN          50   //主页机器人当前点距离移动目标点小于该值时更新新目标点
#define ROBOT_MAINDLG_MOVEPOSTURE_STEP         30   //主页机器人移动目标点姿态距离(度)
#define ROBOT_MAINDLG_MOVEPOSTURE_STEPMIN      15   //主页机器人当前点距离移动目标点姿态距离小于该值时更新新目标点姿态距离(度)
#define ROBOT_MAINDLG_MOVEOUT_STEP             1000 //主页机器人移动第三轴目标点距离
#define ROBOT_MAINDLG_MOVEOUT_STEPMIN          500  //主页机器人第三轴当前点距离移动目标点小于该值时更新新目标点

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

class qtweldingDlg : public QDialog
{
    Q_OBJECT

public:
    explicit qtweldingDlg(QWidget *parent = nullptr);
    ~qtweldingDlg();

    my_parameters *m_mcs;

    MainDlg_robmovestate mainDlg_robmovestate;  //主页机器人移动状态

    qtweldingThread *thread1;            //相机线程
    bool b_thread1;
    bool b_stop_thread1;
    volatile bool b_init_show_ui_list;
    volatile bool b_init_sent_leaser;    //相机往外发送

    qtgetrobThread *thread2;          //机器人线程
    bool b_thread2;
    bool b_stop_thread2;
    volatile bool b_init_show_robpos_list;
    volatile bool b_init_set_robtask;

    qtrecordThread *thread3;        //主页列表显示
    bool b_thread3;
    bool b_stop_thread3;
    volatile bool b_init_show_record_list;
/*
    qtplcThread *thread4;       //PLC线程
    bool b_thread4;
    bool b_stop_thread4;
    volatile bool b_init_set_plctask;
*/
    qtmysunnyDlg *qtmysunny;
    demarcateDlg *demarcate;
    robotsetDlg *robotset;
    weldsetDlg *weldset;
    plcsetDlg *plcset;
    editprojectDlg *editproject;
    newprojectDlg *newproject;
    setprojectDlg *setproject;
    editcraftDlg *editcraft;
    newcraftDlg *newcraft;
    setcraft0Dlg *setcraft0;
    setcraft1Dlg *setcraft1;
    setcraft2Dlg *setcraft2;


    unsigned short leaser_rcv_data[15];
    unsigned short leaser_rcv_data2[4];
    unsigned short leaser_rcv_data3[1];

    unsigned short robotpos_rcv_data[19+ROBOTINPUTNUM+ROBOTTCPPOSOUTNUM*2];
//  unsigned short plc_rcv_data[PLC_RED_NUM];


private slots:
    void init_show_ui_list();

    void init_sent_leaser();

    void init_show_robpos_list();

    void init_set_robtask();

//  void init_set_plctask();

    void init_show_record_list(QString msg);

    void on_importprojectBtn_clicked();

    void on_runprojectBtn_clicked();

    void on_editprojectBtn_clicked();

    void on_setlaserheadBtn_clicked();

    void on_setrobotBtn_clicked();

    void on_demarcateBtn_clicked();

    void on_runpausedBtn_clicked();

    void on_editweldprocessBtn_clicked();

    void on_SaveDatacheckBox_stateChanged(int arg1);

    void on_setweldBtn_clicked();

    void on_weld_windBtn_pressed();

    void on_weld_windBtn_released();

    void on_weld_rewindBtn_pressed();

    void on_weld_rewindBtn_released();

    void on_weld_gassBtn_pressed();

    void on_weld_gassBtn_released();

    void on_weld_fireBtn_pressed();

    void on_weld_fireBtn_released();

    void on_weld_elem_currentIndexChanged(int index);

    void on_weld_eled_editingFinished();

    void on_setplcBtn_clicked();

    void on_posXsubBtn_pressed();

    void on_posXsubBtn_released();

    void on_posXaddBtn_pressed();

    void on_posXaddBtn_released();

    void on_posYsubBtn_pressed();

    void on_posYsubBtn_released();

    void on_posYaddBtn_pressed();

    void on_posYaddBtn_released();

    void on_posZsubBtn_pressed();

    void on_posZsubBtn_released();

    void on_posZaddBtn_pressed();

    void on_posZaddBtn_released();

    void on_posRXsubBtn_pressed();

    void on_posRXsubBtn_released();

    void on_posRXaddBtn_pressed();

    void on_posRXaddBtn_released();

    void on_posRYsubBtn_pressed();

    void on_posRYsubBtn_released();

    void on_posRYaddBtn_pressed();

    void on_posRYaddBtn_released();

    void on_posRZsubBtn_pressed();

    void on_posRZsubBtn_released();

    void on_posRZaddBtn_pressed();

    void on_posRZaddBtn_released();

    void on_posOut1subBtn_pressed();

    void on_posOut1subBtn_released();

    void on_posOut1addBtn_pressed();

    void on_posOut1addBtn_released();

    void on_posOut2subBtn_pressed();

    void on_posOut2subBtn_released();

    void on_posOut2addBtn_pressed();

    void on_posOut2addBtn_released();

    void on_posOut3subBtn_pressed();

    void on_posOut3subBtn_released();

    void on_posOut3addBtn_pressed();

    void on_posOut3addBtn_released();

private:
    Ui::qtweldingDlg *ui;

    void ConnectCamer();       //连接相机
    void DisconnectCamer();    //断开相机

    void ConnectRobot();        //连接机器人
    void DisconnectRobot();     //断开机器人

    void ConnectPLC();          //连接PLC
    void DisconnectPLC();       //断开PLC

    bool b_RunAlgCamer;
    void RunAlgCamer();        //运行算法
    void StopAlgCamer();       //停止算法

    void UpdataUi();            //控件使能

    QDoubleValidator *adoubleValidator_speed;    //机器人速度小数
};

class qtweldingThread : public QThread
{
    Q_OBJECT

public:
    qtweldingThread(qtweldingDlg *statci_p);
    void Stop();
protected:
    void run();
private:
    qtweldingDlg *_p;
signals:
    // 自定义信号
    void Send_show_ui_list();
    void Send_sent_leaser();
};

class qtgetrobThread : public QThread
{
    Q_OBJECT

public:
    qtgetrobThread(qtweldingDlg *statci_p);
    void Stop();
protected:
    void run();
private:
    qtweldingDlg *_p;

signals:
    // 自定义信号
    void Send_show_robpos_list();
    void Send_set_robtask();
};

class qtrecordThread : public QThread
{
    Q_OBJECT

public:
    qtrecordThread(qtweldingDlg *statci_p);
    void Stop();
protected:
    void run();
private:
    qtweldingDlg *_p;

signals:
    // 自定义信号
    void Send_show_record_list(QString msg);
};

/*
class qtplcThread : public QThread
{
    Q_OBJECT

public:
    qtplcThread(qtweldingDlg *statci_p);
    void Stop();
protected:
    void run();
private:
    qtweldingDlg *_p;
signals:
    // 自定义信号
    void Send_set_plctask();
};
*/

#endif // QTWELDINGDLG_H
