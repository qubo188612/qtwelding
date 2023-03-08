#ifndef KEYTRACEDLG_H
#define KEYTRACEDLG_H

#include <QDialog>
#include "my_parameters.h"
#include "my_cmd.h"

namespace Ui {
class keytraceDlg;
}

class keytraceDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keytraceDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keytraceDlg();

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

private:
    Ui::keytraceDlg *ui;
};

#endif // KEYTRACEDLG_H
