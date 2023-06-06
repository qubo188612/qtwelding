#ifndef KEYGETTCPPOS2DLG_H
#define KEYGETTCPPOS2DLG_H

#include <QDialog>
#include "my_parameters.h"
#include "my_cmd.h"

namespace Ui {
class keygettcppos2Dlg;
}

class keygettcppos2Dlg : public QDialog
{
    Q_OBJECT

public:
    explicit keygettcppos2Dlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keygettcppos2Dlg();

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令
    QString cmd_list_in;    //输入的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);

private slots:
    void on_arriveBtn_pressed();

    void on_arriveBtn_released();

    void on_gettcppos2Btn_clicked();

private:
    Ui::keygettcppos2Dlg *ui;

    bool b_inster;
};

#endif // KEYGETTCPPOS2DLG_H
