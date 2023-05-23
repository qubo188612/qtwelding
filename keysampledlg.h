#ifndef KEYSAMPLEDLG_H
#define KEYSAMPLEDLG_H

#include <QDialog>
#include <QDoubleValidator>
#include "my_parameters.h"
#include "my_cmd.h"

namespace Ui {
class keysampleDlg;
}

class keysampleDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keysampleDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keysampleDlg();

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);

private slots:
    void on_sampleBtn_clicked();

private:
    Ui::keysampleDlg *ui;

    QDoubleValidator *adoubleValidator_speed;    //机器人速度小数
    bool b_inster;//是否替换
};

#endif // KEYSAMPLEDLG_H
