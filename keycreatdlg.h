#ifndef KEYCREATDLG_H
#define KEYCREATDLG_H

#include <QDialog>
#include "my_parameters.h"
#include "my_cmd.h"
#include "traceedit0dlg.h"
#include "traceedit1dlg.h"
#include "traceedit2dlg.h"

namespace Ui {
class keycreatDlg;
}

class keycreatDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keycreatDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keycreatDlg();

    traceedit0Dlg *traceedit0;
    traceedit1Dlg *traceedit1;
    traceedit2Dlg *traceedit2;

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令
    QString cmd_list_in;    //输入的指令


    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);
    void set_now_cmdline(int num);

private slots:
    void on_traceeditBtn_clicked();

private:
    Ui::keycreatDlg *ui;
    int now_cmdline;
    bool b_inster;//是否替换
};

#endif // KEYCREATDLG_H
