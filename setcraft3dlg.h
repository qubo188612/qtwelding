#ifndef SETCRAFT3DLG_H
#define SETCRAFT3DLG_H

#include <QDialog>
#include <QDoubleValidator>
#include "my_parameters.h"
#include "my_cmd.h"
#if _MSC_VER
#include "TimeFunction.h"
#endif

namespace Ui {
class setcraft3Dlg;
}

class setcraft3Dlg : public QDialog
{
    Q_OBJECT

public:
    explicit setcraft3Dlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~setcraft3Dlg();

    my_parameters *m_mcs;
    QString cmd_msg;

    void init_dlg_show(bool b_file=true);
    void close_dlg_show();

private slots:
    void on_pushButton_get_stpos_clicked();

    void on_pushButton_get_edpos_clicked();

    void on_pushButton_get_centerpos_clicked();

    void on_pushButtonOK_clicked();

    void on_radioButton_x_clicked();

    void on_radioButton_y_clicked();

private:
    Ui::setcraft3Dlg *ui;

    bool b_file;

    QDoubleValidator *adoubleValidator_pose;    //机器人坐标小数
    QDoubleValidator *adoubleValidator_posture; //机器人姿态小数
    QDoubleValidator *adoubleValidator_3;//3位小数

    void UpdataUi();
};

#endif // SETCRAFT3DLG_H
