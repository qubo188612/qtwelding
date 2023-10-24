#ifndef KEYTRACINGDLG_H
#define KEYTRACINGDLG_H

#include <QDialog>
#include "myfont.h"
#include "my_parameters.h"
#include "my_cmd.h"

namespace Ui {
class keytracingDlg;
}

class keytracingDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keytracingDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keytracingDlg();

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);

private slots:
    void on_pushButton_clicked();

private:
    Ui::keytracingDlg *ui;
};

#endif // KEYTRACINGDLG_H
