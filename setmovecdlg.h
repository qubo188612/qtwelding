#ifndef SETMOVECDLG_H
#define SETMOVECDLG_H

#include <QDialog>
#include <QDoubleValidator>
#include "my_parameters.h"

namespace Ui {
class setmovecDlg;
}

class setmovecDlg : public QDialog
{
    Q_OBJECT

public:
    explicit setmovecDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~setmovecDlg();

    my_parameters *m_mcs;

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void set_arrive_param(float speed,int tcp);

    RobPos pos_st;      //圆弧移动起点
    RobPos pos_center;  //圆弧移动途径点
    RobPos pos_ed;      //圆弧移动终点

private slots:
    void on_pushButton_get_stpos_clicked();

    void on_pushButton_get_centerpos_clicked();

    void on_pushButton_get_edpos_clicked();

    void on_pushButton_OK_clicked();

    void on_arriveBtn_st_pressed();

    void on_arriveBtn_st_released();

    void on_arriveBtn_center_pressed();

    void on_arriveBtn_center_released();

    void on_arriveBtn_ed_pressed();

    void on_arriveBtn_ed_released();

private:
    Ui::setmovecDlg *ui;

    QDoubleValidator *adoubleValidator_pose;    //机器人坐标小数
    QDoubleValidator *adoubleValidator_posture; //机器人姿态小数

    float arrive_speed;
    int arrive_tcp;
};

#endif // SETMOVECDLG_H
