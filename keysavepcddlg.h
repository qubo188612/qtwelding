#ifndef KEYSAVEPCDDLG_H
#define KEYSAVEPCDDLG_H

#include <QDialog>
#include "myfont.h"
#include "my_parameters.h"
#include "my_cmd.h"

namespace Ui {
class keysavepcdDlg;
}

class keysavepcdDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keysavepcdDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keysavepcdDlg();

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);

private slots:
    void on_savepcdBtn_clicked();

    void on_pointsAddBtn_clicked();

    void on_pointsDelBtn_clicked();

    void on_scanAddBtn_clicked();

    void on_scanDelBtn_clicked();

    void on_creatsAddBtn_clicked();

    void on_creatsDelBtn_clicked();

    void on_tracesAddBtn_clicked();

    void on_tracesDelBtn_clicked();

private:
    Ui::keysavepcdDlg *ui;
};

#endif // KEYSAVEPCDDLG_H
