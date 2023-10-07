#ifndef ARRVEPOINTTESTDLG_H
#define ARRVEPOINTTESTDLG_H

#include <QDialog>
#include "my_parameters.h"
#if _MSC_VER
#include "TimeFunction.h"
#endif

namespace Ui {
class arrvepointtestDlg;
}

class arrvepointtestDlg : public QDialog
{
    Q_OBJECT

public:
    explicit arrvepointtestDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~arrvepointtestDlg();

    my_parameters *m_mcs;

    void init_dlg_show();
    void close_dlg_show();

private slots:
    void on_arriveBtn_pressed();

    void on_arriveBtn_released();

    void on_updataleaser_clicked();

    void on_updatapose_clicked();

    void on_updatapos_clicked();

private:
    Ui::arrvepointtestDlg *ui;
};

#endif // ARRVEPOINTTESTDLG_H
