#ifndef KEYCREATCDLG_H
#define KEYCREATCDLG_H

#include <QDialog>
#include <QDoubleValidator>
#include "myfont.h"
#include "my_parameters.h"
#include "my_cmd.h"

namespace Ui {
class keycreatcDlg;
}

class keycreatcDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keycreatcDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keycreatcDlg();

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);

private slots:
    void on_creatcBtn_clicked();

private:
    Ui::keycreatcDlg *ui;

    bool b_inster;

    QDoubleValidator *adoubleValidator_speed;    //机器人速度小数
};

#endif // KEYCREATCDLG_H
