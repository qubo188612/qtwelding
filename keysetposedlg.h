#ifndef KEYSETPOSEDLG_H
#define KEYSETPOSEDLG_H

#include <QDialog>
#include "myfont.h"
#include "my_parameters.h"
#include "my_cmd.h"
#if _MSC_VER
#include "TimeFunction.h"
#endif

namespace Ui {
class keysetposeDlg;
}

class keysetposeDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keysetposeDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keysetposeDlg();

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);

private slots:
    void on_setposeBtn_clicked();

    void on_getposeBtn_clicked();

private:
    Ui::keysetposeDlg *ui;

    bool b_inster;
};

#endif // KEYSETPOSEDLG_H
