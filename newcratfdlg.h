#ifndef NEWCRATFDLG_H
#define NEWCRATFDLG_H

#include <QDialog>
#include "my_parameters.h"

namespace Ui {
class newcratfDlg;
}

class newcratfDlg : public QDialog
{
    Q_OBJECT

public:
    explicit newcratfDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~newcratfDlg();

    my_parameters *m_mcs;

    void init_dlg_show();
    void close_dlg_show();
private:
    Ui::newcratfDlg *ui;
};

#endif // NEWCRATFDLG_H
