#ifndef KEYTRACE2DLG_H
#define KEYTRACE2DLG_H

#include <QDialog>
#include <QDoubleValidator>
#include <QFileDialog>
#include "myfont.h"
#include "my_parameters.h"
#include "my_cmd.h"

namespace Ui {
class keytrace2Dlg;
}

class keytrace2Dlg : public QDialog
{
    Q_OBJECT

public:
    explicit keytrace2Dlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keytrace2Dlg();

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令
    QString cmd_list_in;    //输入的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);

private slots:
    void on_trace2cmdaddBtn_clicked();

    void on_trace2changecheckBox_stateChanged(int arg1);

private:
    Ui::keytrace2Dlg *ui;

    QDoubleValidator *adoubleValidator_speed;    //机器人速度小数
};

#endif // KEYTRACE2DLG_H
