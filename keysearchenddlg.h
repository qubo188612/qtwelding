#ifndef KEYSEARCHENDDLG_H
#define KEYSEARCHENDDLG_H

#include <QDialog>
#include <QDoubleValidator>
#include "my_parameters.h"
#include "setmovecdlg.h"
#include "my_cmd.h"

namespace Ui {
class keysearchendDlg;
}

class keysearchendDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keysearchendDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keysearchendDlg();

    setmovecDlg *setmovec;

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令
    QString cmd_list_in;    //输入的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);

private slots:
    void on_searchendaddBtn_clicked();

    void on_searchendchangecheckBox_stateChanged(int arg1);

    void on_arriveBtn_pressed();

    void on_arriveBtn_released();

    void on_updata_posBtn_clicked();

private:
    Ui::keysearchendDlg *ui;

    QDoubleValidator *adoubleValidator_speed;    //机器人速度小数
    bool b_inster;//是否替换
    RobPos inster_pos;//当前到位
};

#endif // KEYSEARCHENDDLG_H
