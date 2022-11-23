#ifndef SETCRAFT0DLG_H
#define SETCRAFT0DLG_H

#include <QDialog>
#include <QDoubleValidator>
#include <my_parameters.h>

namespace Ui {
class setcraft0Dlg;
}

class setcraft0Dlg : public QDialog
{
    Q_OBJECT

public:
    explicit setcraft0Dlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~setcraft0Dlg();

    my_parameters *m_mcs;

    void init_dlg_show();
    void close_dlg_show();
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_comboBox_pendulum_mode_currentIndexChanged(int index);

private:
    Ui::setcraft0Dlg *ui;

    QDoubleValidator *adoubleValidator_pose;    //机器人坐标小数
    QDoubleValidator *adoubleValidator_posture; //机器人姿态小数
    QDoubleValidator *adoubleValidator_3;//3位小数

    void UpdataUi();
};

#endif // SETCRAFT0DLG_H
