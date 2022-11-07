#ifndef EDITCRATFDLG_H
#define EDITCRATFDLG_H

#include <QDialog>
#include "my_parameters.h"

#define EDITCRATFDLG_BTN1       1
#define EDITCRATFDLG_BTN2       2

namespace Ui {
class editcratfDlg;
}

class editcratfDlg : public QDialog
{
    Q_OBJECT

public:
    explicit editcratfDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~editcratfDlg();

    my_parameters *m_mcs;

    void init_dlg_show();
    void close_dlg_show();
private slots:
    void on_BTN1_clicked();

    void on_BTN2_clicked();

private:
    Ui::editcratfDlg *ui;
};

#endif // EDITCRATFDLG_H
