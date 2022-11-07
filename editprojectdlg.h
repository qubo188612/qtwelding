#ifndef EDITPROJECTDLG_H
#define EDITPROJECTDLG_H

#include <QDialog>
#include "my_parameters.h"

#define EDITPROJECTDLG_BTN1         1
#define EDITPROJECTDLG_BTN2         2

namespace Ui {
class editprojectDlg;
}

class editprojectDlg : public QDialog
{
    Q_OBJECT

public:
    explicit editprojectDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~editprojectDlg();

    my_parameters *m_mcs;

    void init_dlg_show();
    void close_dlg_show();

private slots:
    void on_BTN1_clicked();

    void on_BTN2_clicked();

private:
    Ui::editprojectDlg *ui;
};

#endif // EDITPROJECTDLG_H
