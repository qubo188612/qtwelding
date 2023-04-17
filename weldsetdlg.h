#ifndef WELDSETDLG_H
#define WELDSETDLG_H

#include <QDialog>
#include "my_parameters.h"
#include "robotdata.h"

namespace Ui {
class weldsetDlg;
}

class weldsetDlg : public QDialog
{
    Q_OBJECT

public:
    explicit weldsetDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~weldsetDlg();

    my_parameters *m_mcs;

    void init_dlg_show();
    void close_dlg_show();
private slots:
    void on_pushButton_clicked();

    void on_weld_model_currentIndexChanged(int index);

private:
    Ui::weldsetDlg *ui;
};

#endif // WELDSETDLG_H
