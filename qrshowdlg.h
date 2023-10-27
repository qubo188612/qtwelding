#ifndef QRSHOWDLG_H
#define QRSHOWDLG_H

#include <QDialog>
#include "myfont.h"
#include "my_parameters.h"
#include "MyQrFunction.h"

namespace Ui {
class qrshowDlg;
}

class qrshowDlg : public QDialog
{
    Q_OBJECT

public:
    explicit qrshowDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~qrshowDlg();

    my_parameters *m_mcs;

    void init_dlg_show();
    void close_dlg_show();

private slots:
    void on_pushButton_clicked();

private:
    Ui::qrshowDlg *ui;
};

#endif // QRSHOWDLG_H
