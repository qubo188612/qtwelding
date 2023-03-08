#ifndef KEYCAMDLG_H
#define KEYCAMDLG_H

#include <QDialog>
#include "my_parameters.h"
#include "my_cmd.h"

namespace Ui {
class keycamDlg;
}

class keycamDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keycamDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keycamDlg();

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

private slots:
    void on_leaseropenBtn_clicked();

    void on_leasercloseBtn_clicked();

private:
    Ui::keycamDlg *ui;
};

#endif // KEYCAMDLG_H
