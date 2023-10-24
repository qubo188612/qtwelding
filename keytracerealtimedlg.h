#ifndef KEYTRACEREALTIMEDLG_H
#define KEYTRACEREALTIMEDLG_H

#include <QDialog>
#include <QDoubleValidator>
#include <QFileDialog>
#include "myfont.h"
#include "my_parameters.h"
#include "setmovecdlg.h"
#include "my_cmd.h"
#if _MSC_VER
#include "TimeFunction.h"
#endif


namespace Ui {
class keytracerealtimeDlg;
}

class keytracerealtimeDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keytracerealtimeDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keytracerealtimeDlg();

    setmovecDlg *setmovec;

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令
    QString cmd_list_in;    //输入的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);


private slots:
    void on_tracerealtimeaddBtn_clicked();

    void on_tracerealtimeweldcheckBox_stateChanged(int arg1);

    void on_tracerealtimechangecheckBox_stateChanged(int arg1);

    void on_tracerealtimefilepathBtn_clicked();

    void on_updata_posBtn_clicked();

    void on_updata_posBtn_2_clicked();

    void on_arriveBtn_pressed();

    void on_arriveBtn_released();

    void on_arriveBtn_2_pressed();

    void on_arriveBtn_2_released();

private:
    Ui::keytracerealtimeDlg *ui;

    bool b_inster;//是否替换
    RobPos inster_pos;//当前到位

    QDoubleValidator *adoubleValidator_speed;    //机器人速度小数
};

#endif // KEYTRACEREALTIMEDLG_H
