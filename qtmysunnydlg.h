#ifndef QTMYSUNNYDLG_H
#define QTMYSUNNYDLG_H

#include "global.h"
#include <QDialog>
#include "myfont.h"
#include "showtasknumdlg.h"
#include "taskcleardlg.h"
#include "pshowdlg.h"
#ifdef DEBUS_SSH
#include "sshpassworddlg.h"
#endif
#include "cambuilddlg.h"
#include "my_parameters.h"
#if _MSC_VER
#include <QDir>
#include <synchapi.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#endif
#include "TimeFunction.h"
#include "PictureBox.h"
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define TABWIDGET_TASK100       1
#define TABWIDGET_TASK101       2
#define TABWIDGET_TASK102       3
#define TABWIDGET_TASK103       4
#define TABWIDGET_TASK104       5
#define TABWIDGET_TASK105       6
#define TABWIDGET_TASK106       7
#define TABWIDGET_TASK107       8
#define TABWIDGET_TASK108       9

namespace Ui {
class qtmysunnyDlg;
}

class getposThread;

class qtmysunnyDlg : public QDialog
{
    Q_OBJECT

public:
    explicit qtmysunnyDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~qtmysunnyDlg();

    my_parameters *m_mcs;

    void UpdataUi();
    void UpdataRobot(u_int16_t robotmod);

    bool ipAddrIsOK(const QString & ip,std::vector<u_int8_t> &address);

    void img_windowshow(bool b_show,PictureBox *lab_show);

    void open_camer_modbus();       //打开相机采集
    void close_camer_modbus();      //关闭相机采集

    void showupdata_tabWidget(int index);

    QString JsonToQstring(QJsonObject jsonObject);

    QJsonObject QstringToJson(QString jsonString);

    getposThread *thread1;          //激光器线程
    bool b_thread1;
    bool b_stop_thread1;

    unsigned short pos_data[15];
    unsigned short pos_data2[4];    //附加点
    unsigned short pos_data3[1];    //焊接点

    volatile bool b_init_show_cvimage_inlab_finish;
    volatile bool b_init_show_pos_failed_finish;
    volatile bool b_init_show_pos_list_finish;
    volatile bool b_init_set_task;

    volatile int ctx_result_dosomeing;

    int32_t tabWidget_task; //当前选择的tab任务
    int32_t zoom_left;
    int32_t zoom_right;
    int32_t zoom_top;
    int32_t zoom_deep;
    int32_t zoom_center_x;
    int32_t zoom_center_y;
    uint16_t u16_zoom_left;
    uint16_t u16_zoom_right;
    uint16_t u16_zoom_top;
    uint16_t u16_zoom_deep;
    uint16_t u16_zoom_center_x;
    uint16_t u16_zoom_center_y;
    int32_t drow_height;
    int32_t drow_width;
    void drow_image(int32_t oldheight,int32_t oldwidth,QImage *img);
    int32_t drowstep;
    int32_t drowstep_temp=0;

    bool b_robot_serves;//机器人是否服务端

//  void showEvent(QShowEvent *e);//重写函数避免界面不刷新

    void init_dlg_show();
    void close_dlg_show();

private:
    Ui::qtmysunnyDlg *ui;

    showtasknumdlg *showtasknum;
    taskcleardlg *taskclear;
    pshowdlg *pshow;
#ifdef DEBUS_SSH
    sshpasswordDlg *sshpassword;
#endif
    cambuilddlg *cambuild;

    QIntValidator *aIntValidator;   //限制输入整数

private slots:
    void init_show_pos_list();
    void init_show_pos_failed();
    void init_show_cvimage_inlab(cv::Mat);
    void init_set_task();
    void on_tabWidget_currentChanged(int index);
};

class getposThread : public QThread
{
    Q_OBJECT

public:
    getposThread(qtmysunnyDlg *statci_p);
    void Stop();
protected:
    void run();
private:
    qtmysunnyDlg *_p;

signals:
    // 自定义信号
    void Send_show_pos_list();
    void Send_show_pos_failed();
    void Send_show_cvimage_inlab(cv::Mat);
    void Send_set_task();
};

#endif // QTMYSUNNYDLG_H
