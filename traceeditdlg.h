#ifndef TRACEEDITDLG_H
#define TRACEEDITDLG_H

#include <QDialog>
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

    void init_dlg_show();
    void close_dlg_show();

private:
    Ui::traceeditDlg *ui;
};

#endif // TRACEEDITDLG_H
