#ifndef KEYCREATFDLG_H
#define KEYCREATFDLG_H

#include <QDialog>
#include <QFileDialog>
#include "my_parameters.h"
#include "my_cmd.h"

namespace Ui {
class keycreatfDlg;
}

class keycreatfDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keycreatfDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keycreatfDlg();

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);

private slots:
    void on_creatfilepathBtn_clicked();

    void on_creatfBtn_clicked();

private:
    Ui::keycreatfDlg *ui;

    bool b_inster;
};

#endif // KEYCREATFDLG_H
