#ifndef KEYPLOTPOSDLG_H
#define KEYPLOTPOSDLG_H

#include <QDialog>
#include "my_parameters.h"
#include "my_cmd.h"
#include "plotedit0dlg.h"
#include "plotedit1dlg.h"
#include "plotedit2dlg.h"

namespace Ui {
class keyplotposDlg;
}

class keyplotposDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keyplotposDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keyplotposDlg();

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令
    QString cmd_list_in;    //输入的指令

    plotedit0Dlg *plotedit0;
    plotedit1Dlg *plotedit1;
    plotedit2Dlg *plotedit2;

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);
    void set_now_cmdline(int num);

private slots:
    void on_plotposBtn_clicked();

private:
    Ui::keyplotposDlg *ui;

    int now_cmdline;
    bool b_inster;//是否替换
};

#endif // KEYPLOTPOSDLG_H
