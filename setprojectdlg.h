#ifndef SETPROJECTDLG_H
#define SETPROJECTDLG_H

#include <QDialog>
#include "my_parameters.h"

namespace Ui {
class setprojectDlg;
}

class setprojectDlg : public QDialog
{
    Q_OBJECT

public:
    explicit setprojectDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~setprojectDlg();

    my_parameters *m_mcs;

    void init_dlg_show();
    void close_dlg_show();

private:
    Ui::setprojectDlg *ui;
};

#endif // SETPROJECTDLG_H
