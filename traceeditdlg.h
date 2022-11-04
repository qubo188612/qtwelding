#ifndef TRACEEDITDLG_H
#define TRACEEDITDLG_H

#include <QDialog>
#include <QMessageBox>
#include "my_parameters.h"

namespace Ui {
class traceeditDlg;
}

class traceeditDlg : public QDialog
{
    Q_OBJECT

public:
    explicit traceeditDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~traceeditDlg();

    my_parameters *m_mcs;

    QString name;

    void init_dlg_show();
    void close_dlg_show();

private slots:
    void on_pushButton_clicked();

private:
    Ui::traceeditDlg *ui;
};

#endif // TRACEEDITDLG_H
