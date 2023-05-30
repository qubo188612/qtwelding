#ifndef KEYWAVEDLG_H
#define KEYWAVEDLG_H

#include <QDialog>
#include "my_parameters.h"
#include "my_cmd.h"

namespace Ui {
class keywaveDlg;
}

class keywaveDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keywaveDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keywaveDlg();

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);


private slots:
    void on_pushButton_clicked();

private:
    Ui::keywaveDlg *ui;
};

#endif // KEYWAVEDLG_H
