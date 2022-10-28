#ifndef QTWELDINGDLG_H
#define QTWELDINGDLG_H

#include <QDialog>
#include <QFileDialog>
#include <QTextCodec>
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
#include "editprojectdlg.h"
#include "newprojectdlg.h"
#include "setprojectdlg.h"


namespace Ui {
class qtweldingDlg;
}

class qtweldingThread;

class qtgetrobThread;

class qtweldingDlg : public QDialog
{
    Q_OBJECT

public:
    explicit qtweldingDlg(QWidget *parent = nullptr);
    ~qtweldingDlg();

    my_parameters *m_mcs;

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

    qtmysunnyDlg *qtmysunny;
    demarcateDlg *demarcate;
    robotsetDlg *robotset;
    editprojectDlg *editproject;
    newprojectDlg *newproject;
    setprojectDlg *setproject;


    unsigned short leaser_rcv_data[15];
    unsigned short leaser_rcv_data2[4];


    unsigned short robotpos_rcv_data[14];


private slots:
    void on_importprojectBtn_clicked();

    void on_runprojectBtn_clicked();

    void on_editprojectBtn_clicked();

    void on_editweldprocessBtn_clicked();

    void on_setlaserheadBtn_clicked();

    void on_setrobotBtn_clicked();

    void on_setwelderBtn_clicked();

    void init_show_ui_list();

    void init_sent_leaser();

    void on_demarcateBtn_clicked();

    void init_show_robpos_list();

    void init_set_robtask();

private:
    Ui::qtweldingDlg *ui;

    void ConnectCamer();       //连接相机
    void DisconnectCamer();    //断开相机

    void ConnectRobot();        //连接机器人
    void DisconnectRobot();     //断开机器人

    bool b_RunAlgCamer;
    void RunAlgCamer();        //运行算法
    void StopAlgCamer();       //停止算法

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

#endif // QTWELDINGDLG_H
