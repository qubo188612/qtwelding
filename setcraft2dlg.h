#ifndef SETCRAFT2DLG_H
#define SETCRAFT2DLG_H

#include <QDialog>
#include <QDoubleValidator>
#include <my_parameters.h>

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
    void on_pushButton_2_clicked();

    void on_comboBox_pendulum_mode_currentIndexChanged(int index);

private:
    Ui::setcraft2Dlg *ui;

    QDoubleValidator *adoubleValidator_3;//3位小数
    void UpdataUi();
};

#endif // SETCRAFT2DLG_H
