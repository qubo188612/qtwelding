#ifndef QTMYSUNNYDLG_H
#define QTMYSUNNYDLG_H

#include "global.h"
#include <QDialog>
#include "showtasknumdlg.h"
#if _MSC_VER||WINDOWS_TCP
#else
#include "cambuilddlg.h"
#endif
#include <my_parameters.h>
#if _MSC_VER
#include <QDir>
#include <synchapi.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#endif
#include "TimeFunction.h"
#include "PictureBox.h"

#define DO_NOTHING         0
#define DO_WRITE_TASK      1

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

    void img_windowshow(bool b_show,PictureBox *lab_show);

    void open_camer_modbus();       //打开相机采集
    void close_camer_modbus();      //关闭相机采集

    void showupdata_tabWidget(int index);

    getposThread *thread1;
    bool b_thread1;
    bool b_stop_thread1;

    unsigned short pos_data[15];
    unsigned short pos_data2[4];    //附加点
    unsigned short pos_data3[1];    //焊接点

    volatile bool b_init_show_cvimage_inlab_finish;
    volatile bool b_init_show_pos_failed_finish;
    volatile bool b_init_show_pos_list_finish;
    volatile bool b_init_set_task;

    int ctx_result_dosomeing;

//  void showEvent(QShowEvent *e);//重写函数避免界面不刷新

    void init_dlg_show();
    void close_dlg_show();

private:
    Ui::qtmysunnyDlg *ui;

    showtasknumdlg *showtasknum;
#if _MSC_VER||WINDOWS_TCP
#else
    cambuilddlg *cambuild;
#endif

private slots:
    void init_show_pos_list();
    void init_show_pos_failed();
    void init_show_cvimage_inlab(cv::Mat);
    void init_set_task();
    void on_tabWidget_tabBarClicked(int index);
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
