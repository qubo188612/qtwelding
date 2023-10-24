#ifndef KEYWELDDLG_H
#define KEYWELDDLG_H

#include <QDialog>
#include "myfont.h"
#include "my_parameters.h"
#include "my_cmd.h"

namespace Ui {
class keyweldDlg;
}

class keyweldDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keyweldDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keyweldDlg();

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);

private slots:
    void on_welderarcingBtn_clicked();

    void on_welderarcoutBtn_clicked();

private:
    Ui::keyweldDlg *ui;
};

#endif // KEYWELDDLG_H
