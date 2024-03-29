#ifndef KEYSMOVDLG_H
#define KEYSMOVDLG_H

#include <QDialog>
#include <QDoubleValidator>
#include "myfont.h"
#include "my_parameters.h"
#include "my_cmd.h"

namespace Ui {
class keysmovDlg;
}

class keysmovDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keysmovDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keysmovDlg();

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);

private slots:
    void on_smovemodecombo_currentIndexChanged(int index);

    void on_smovechangecheckBox_stateChanged(int arg1);

    void on_smoveaddBtn_clicked();

private:
    Ui::keysmovDlg *ui;

    QDoubleValidator *adoubleValidator_speed;    //机器人速度小数
};

#endif // KEYSMOVDLG_H
