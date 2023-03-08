#ifndef KEYSCANDLG_H
#define KEYSCANDLG_H

#include <QDialog>
#include "my_parameters.h"
#include "my_cmd.h"

namespace Ui {
class keyscanDlg;
}

class keyscanDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keyscanDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keyscanDlg();

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

private:
    Ui::keyscanDlg *ui;
};

#endif // KEYSCANDLG_H
