#ifndef KEYTRACEDLG_H
#define KEYTRACEDLG_H

#include <QDialog>
#include <QDoubleValidator>
#include <QFileDialog>
#include "myfont.h"
#include "my_parameters.h"
#include "my_cmd.h"

namespace Ui {
class keytraceDlg;
}

class keytraceDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keytraceDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keytraceDlg();

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令
    QString cmd_list_in;    //输入的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);

private slots:
    void on_tracefilepathBtn_clicked();

    void on_tracecmdaddBtn_clicked();

    void on_tracechangecheckBox_stateChanged(int arg1);

private:
    Ui::keytraceDlg *ui;

    QDoubleValidator *adoubleValidator_speed;    //机器人速度小数
};

#endif // KEYTRACEDLG_H
