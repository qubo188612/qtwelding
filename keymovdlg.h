#ifndef KEYMOVDLG_H
#define KEYMOVDLG_H

#include <QDialog>
#include <QDoubleValidator>
#include "my_parameters.h"
#include "setmovecdlg.h"
#include "my_cmd.h"

namespace Ui {
class keymovDlg;
}

class keymovDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keymovDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keymovDlg();

    setmovecDlg *setmovec;

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令
    QString cmd_list_in;    //输入的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);

private slots:
    void on_moveaddBtn_clicked();

    void on_arriveBtn_pressed();

    void on_arriveBtn_released();

    void on_movechangecheckBox_stateChanged(int arg1);

    void on_updata_posBtn_clicked();

private:
    Ui::keymovDlg *ui;

    bool b_inster;//是否替换
    RobPos inster_pos;//当前到位

    QDoubleValidator *adoubleValidator_speed;    //机器人速度小数
};

#endif // KEYMOVDLG_H
