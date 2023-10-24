#ifndef KEYTRACECONTINUEDLG_H
#define KEYTRACECONTINUEDLG_H

#include <QDialog>
#include "myfont.h"
#include "my_parameters.h"
#include "my_cmd.h"
#if _MSC_VER
#include "TimeFunction.h"
#endif

namespace Ui {
class keytracecontinueDlg;
}

class keytracecontinueDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keytracecontinueDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keytracecontinueDlg();

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令
    QString cmd_list_in;

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);

private slots:
    void on_arriveBtn_pressed();

    void on_arriveBtn_released();

    void on_pushButton_clicked();

    void on_updata_posBtn_clicked();

private:
    Ui::keytracecontinueDlg *ui;

    bool b_inster;
    RobPos inster_pos;//当前到位
};

#endif // KEYTRACECONTINUEDLG_H
