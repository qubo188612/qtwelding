#ifndef CAMBUILDDLG_H
#define CAMBUILDDLG_H

#include "global.h"
#if _MSC_VER||WINDOWS_TCP
#else
#include <QDialog>
#include <QDoubleValidator>
#include <my_parameters.h>
#include <QKeyEvent>


namespace Ui {
class cambuilddlg;
}

class cambuildThread;

class cambuilddlg : public QDialog
{
    Q_OBJECT

public:
    explicit cambuilddlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~cambuilddlg();

    my_parameters *m_mcs;

    void init_dlg_show();
    void close_dlg_show();

    void drow_point(cv::Mat cvimg);

    cambuildThread *thread1;
    bool b_thread1;
    bool b_stop_thread1;
    bool b_init_show_cambuild_inlab_finish;

    bool b_key_1_push;      //键盘1按下
    bool b_key_2_push;      //键盘2按下
    bool b_key_3_push;      //键盘3按下
    bool b_key_4_push;      //键盘4按下
    bool b_key_left_pull;   //键盘左抬起
    bool b_key_right_pull;  //键盘右抬起
    bool b_key_up_pull;     //键盘上抬起
    bool b_key_down_pull;   //键盘下抬起

private:
    Ui::cambuilddlg *ui;

    QDoubleValidator *adoubleValidator_3;//3位小数

protected:
    virtual void keyPressEvent(QKeyEvent *ev);
    virtual void keyReleaseEvent(QKeyEvent *ev);

private slots:
    void init_show_cambuild_inlab(cv::Mat);
};

class cambuildThread : public QThread
{
    Q_OBJECT

public:
    cambuildThread(cambuilddlg *statci_p);
    void Stop();
protected:
    void run();
private:
    cambuilddlg *_p;

signals:
    // 自定义信号
    void Send_show_cambuild_inlab(cv::Mat);
};

#endif

#endif // CAMBUILDDLG_H
