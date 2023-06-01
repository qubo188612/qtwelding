#ifndef KEYGETTCPPOSDLG_H
#define KEYGETTCPPOSDLG_H

#include <QDialog>
#include "my_parameters.h"
#include "my_cmd.h"

namespace Ui {
class keygettcpposDlg;
}

class keygettcpposDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keygettcpposDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keygettcpposDlg();

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);

private slots:
    void on_gettcpposBtn_clicked();

private:
    Ui::keygettcpposDlg *ui;

    bool b_inster;
};

#endif // KEYGETTCPPOSDLG_H
