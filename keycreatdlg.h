#ifndef KEYCREATDLG_H
#define KEYCREATDLG_H

#include <QDialog>
#include "my_parameters.h"
#include "my_cmd.h"

namespace Ui {
class keycreatDlg;
}

class keycreatDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keycreatDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keycreatDlg();

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

private:
    Ui::keycreatDlg *ui;
};

#endif // KEYCREATDLG_H
