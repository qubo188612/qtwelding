#ifndef EDITCRAFTDLG_H
#define EDITCRAFTDLG_H

#include <QDialog>
#include "my_parameters.h"
#include "myfont.h"

#define EDITCRAFTDLG_BTN1       1
#define EDITCRAFTDLG_BTN2       2

namespace Ui {
class editcraftDlg;
}

class editcraftDlg : public QDialog
{
    Q_OBJECT

public:
    explicit editcraftDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~editcraftDlg();

    my_parameters *m_mcs;

    void init_dlg_show();
    void close_dlg_show();
private slots:
    void on_BTN1_clicked();

    void on_BTN2_clicked();

private:
    Ui::editcraftDlg *ui;
};

#endif // EDITCRAFTDLG_H
