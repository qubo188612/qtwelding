#ifndef SETCRAFT2DLG_H
#define SETCRAFT2DLG_H

#include <QDialog>
#include <QDoubleValidator>
#include "my_parameters.h"

namespace Ui {
class setcraft2Dlg;
}

class setcraft2Dlg : public QDialog
{
    Q_OBJECT

public:
    explicit setcraft2Dlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~setcraft2Dlg();

    my_parameters *m_mcs;

    void init_dlg_show();
    void close_dlg_show();

private slots:
    void on_pushButton_clicked();

private:
    Ui::setcraft2Dlg *ui;

    QDoubleValidator *adoubleValidator_pose;    //机器人坐标小数
};

#endif // SETCRAFT2DLG_H
