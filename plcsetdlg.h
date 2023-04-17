#ifndef PLCSETDLG_H
#define PLCSETDLG_H

#include <QDialog>
#include "my_parameters.h"

namespace Ui {
class plcsetDlg;
}

class plcsetDlg : public QDialog
{
    Q_OBJECT

public:
    explicit plcsetDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~plcsetDlg();

    my_parameters *m_mcs;
    void init_dlg_show();
    void close_dlg_show();

private slots:
    void on_plc_model_currentIndexChanged(int index);

    void on_pushButton_clicked();

private:
    Ui::plcsetDlg *ui;
};

#endif // PLCSETDLG_H
