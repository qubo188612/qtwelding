#ifndef KEYPLCDLG_H
#define KEYPLCDLG_H

#include <QDialog>
#include "my_parameters.h"
#include "my_cmd.h"

namespace Ui {
class keyplcDlg;
}

class keyplcDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keyplcDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keyplcDlg();

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);

private slots:
    void on_PLCoutputBtn_clicked();

    void on_PLCinputBtn_clicked();

private:
    Ui::keyplcDlg *ui;
};

#endif // KEYPLCDLG_H
