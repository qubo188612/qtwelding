#ifndef KEYGETPOSDLG_H
#define KEYGETPOSDLG_H

#include <QDialog>
#include "myfont.h"
#include "my_parameters.h"
#include "my_cmd.h"

namespace Ui {
class keygetposDlg;
}

class keygetposDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keygetposDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keygetposDlg();

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);


private slots:
    void on_getposBtn_clicked();

private:
    Ui::keygetposDlg *ui;

    bool b_inster;
};

#endif // KEYGETPOSDLG_H
