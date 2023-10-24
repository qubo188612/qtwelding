#ifndef SETCRAFT0DLG_H
#define SETCRAFT0DLG_H

#include <QDialog>
#include <QDoubleValidator>
#include "myfont.h"
#include "my_parameters.h"
#include "my_cmd.h"

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
    QString cmd_msg;

    void init_dlg_show(bool b_file=true);
    void close_dlg_show();
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();


private:
    Ui::setcraft0Dlg *ui;

    bool b_file;


    QDoubleValidator *adoubleValidator_pose;    //机器人坐标小数
    QDoubleValidator *adoubleValidator_posture; //机器人姿态小数
    QDoubleValidator *adoubleValidator_3;//3位小数

    void UpdataUi();
};

#endif // SETCRAFT0DLG_H
