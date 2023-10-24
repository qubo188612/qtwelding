#ifndef KEYSSCANDLG_H
#define KEYSSCANDLG_H

#include <QDialog>
#include <QDoubleValidator>
#include "myfont.h"
#include "my_parameters.h"
#include "my_cmd.h"

namespace Ui {
class keysscanDlg;
}

class keysscanDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keysscanDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keysscanDlg();

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);


private slots:
    void on_sscanmovemodecombo_currentIndexChanged(int index);

    void on_sscanchangecheckBox_stateChanged(int arg1);

    void on_sscanaddBtn_clicked();

private:
    Ui::keysscanDlg *ui;

    QDoubleValidator *adoubleValidator_speed;    //机器人速度小数
    bool b_inster;//是否替换
};

#endif // KEYSSCANDLG_H
