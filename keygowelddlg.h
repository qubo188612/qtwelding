#ifndef KEYGOWELDDLG_H
#define KEYGOWELDDLG_H

#include <QDialog>
#include <QDoubleValidator>
#include "my_parameters.h"
#include "my_cmd.h"

namespace Ui {
class keygoweldDlg;
}

class keygoweldDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keygoweldDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keygoweldDlg();

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);


private slots:
    void on_pushButton_clicked();

private:
    Ui::keygoweldDlg *ui;

    QDoubleValidator *adoubleValidator_speed;    //机器人速度小数
};

#endif // KEYGOWELDDLG_H
