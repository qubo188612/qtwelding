#ifndef SETCRAFT1DLG_H
#define SETCRAFT1DLG_H

#include <QDialog>
#include <my_parameters.h>

namespace Ui {
class setcraft1Dlg;
}

class setcraft1Dlg : public QDialog
{
    Q_OBJECT

public:
    explicit setcraft1Dlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~setcraft1Dlg();

    my_parameters *m_mcs;

    void init_dlg_show();
    void close_dlg_show();
private slots:
    void on_pushButton_get_stpos_clicked();

    void on_pushButton_updata_stpos_clicked();

    void on_pushButton_get_edpos_clicked();

    void on_pushButton_updata_edpos_clicked();

    void on_pushButton_get_centerpos_clicked();

    void on_pushButton_updata_centerpos_clicked();

    void on_pushButton_del_clicked();

    void on_pushButtonOK_clicked();

private:
    Ui::setcraft1Dlg *ui;
};

#endif // SETCRAFT1DLG_H
