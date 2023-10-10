#ifndef KEYTRACEREALTIME2DLG_H
#define KEYTRACEREALTIME2DLG_H

#include <QDialog>
#include <QDoubleValidator>
#include <QFileDialog>
#include "my_parameters.h"
#include "setmovecdlg.h"
#include "my_cmd.h"
#if _MSC_VER
#include "TimeFunction.h"
#endif


namespace Ui {
class keytracerealtime2Dlg;
}

class keytracerealtime2Dlg : public QDialog
{
    Q_OBJECT

public:
    explicit keytracerealtime2Dlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keytracerealtime2Dlg();

    setmovecDlg *setmovec;

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令
    QString cmd_list_in;    //输入的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);

private slots:
    void on_tracerealtime2addBtn_clicked();

    void on_tracerealtime2weldcheckBox_stateChanged(int arg1);

    void on_tracerealtime2changecheckBox_stateChanged(int arg1);

    void on_updata_posBtn_clicked();

    void on_updata_posBtn_2_clicked();

    void on_arriveBtn_pressed();

    void on_arriveBtn_released();

    void on_arriveBtn_2_pressed();

    void on_arriveBtn_2_released();

private:
    Ui::keytracerealtime2Dlg *ui;

    bool b_inster;//是否替换
    RobPos inster_pos;//当前到位

    QDoubleValidator *adoubleValidator_speed;    //机器人速度小数
};

#endif // KEYTRACEREALTIME2DLG_H
