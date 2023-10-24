#ifndef KEYIODLG_H
#define KEYIODLG_H

#include <QDialog>
#include "myfont.h"
#include "my_parameters.h"
#include "my_cmd.h"

namespace Ui {
class keyioDlg;
}

class keyioDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keyioDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keyioDlg();

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);

private slots:
    void on_IOoutputBtn_clicked();

    void on_IOinputBtn_clicked();

private:
    Ui::keyioDlg *ui;
};

#endif // KEYIODLG_H
