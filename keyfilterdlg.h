#ifndef KEYFILTERDLG_H
#define KEYFILTERDLG_H

#include <QDialog>
#include "my_parameters.h"
#include "my_cmd.h"

namespace Ui {
class keyfilterDlg;
}

class keyfilterDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keyfilterDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keyfilterDlg();

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);

private slots:
    void on_pushButton_clicked();

    void on_filtermodecombo_currentIndexChanged(int index);

private:
    Ui::keyfilterDlg *ui;
    bool b_inster;//是否替换
};

#endif // KEYFILTERDLG_H
