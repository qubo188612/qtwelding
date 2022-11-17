#ifndef SETCRAFTDLG_H
#define SETCRAFTDLG_H

#include <QDialog>
#include <QDoubleValidator>
#include <my_parameters.h>

namespace Ui {
class setcraftDlg;
}

class setcraftDlg : public QDialog
{
    Q_OBJECT

public:
    explicit setcraftDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~setcraftDlg();

    my_parameters *m_mcs;

    void init_dlg_show();
    void close_dlg_show();
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_comboBox_pendulum_mode_currentIndexChanged(int index);

private:
    Ui::setcraftDlg *ui;

    QDoubleValidator *adoubleValidator_pose;    //机器人坐标小数
    QDoubleValidator *adoubleValidator_posture; //机器人姿态小数
    QDoubleValidator *adoubleValidator_3;//3位小数

    void UpdataUi();
};

#endif // SETCRAFTDLG_H
