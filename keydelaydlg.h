#ifndef KEYDELAYDLG_H
#define KEYDELAYDLG_H

#include <QDialog>
#include "myfont.h"
#include "my_parameters.h"
#include "my_cmd.h"

namespace Ui {
class keydelayDlg;
}

class keydelayDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keydelayDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keydelayDlg();

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);

private slots:
    void on_delaytimeBtn_clicked();

private:
    Ui::keydelayDlg *ui;
};

#endif // KEYDELAYDLG_H
