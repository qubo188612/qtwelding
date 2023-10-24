#ifndef KEYTRACEADDDLG_H
#define KEYTRACEADDDLG_H

#include <QDialog>
#include "myfont.h"
#include "my_parameters.h"
#include "my_cmd.h"

namespace Ui {
class keytraceaddDlg;
}

class keytraceaddDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keytraceaddDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keytraceaddDlg();

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);

    void Updata();

private slots:
    void on_pushButton_clicked();

    void on_samplelinkcheckBox_stateChanged(int arg1);

private:
    Ui::keytraceaddDlg *ui;
    bool b_sample_link;
};

#endif // KEYTRACEADDDLG_H
