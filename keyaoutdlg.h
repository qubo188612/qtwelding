#ifndef KEYAOUTDLG_H
#define KEYAOUTDLG_H

#include <QDialog>
#include "myfont.h"
#include "my_parameters.h"
#include "my_cmd.h"

namespace Ui {
class keyaoutDlg;
}

class keyaoutDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keyaoutDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keyaoutDlg();

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);

private slots:
    void on_AoutputBtn_clicked();

private:
    Ui::keyaoutDlg *ui;
};

#endif // KEYAOUTDLG_H
