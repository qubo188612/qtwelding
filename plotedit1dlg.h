#ifndef PLOTEDIT1DLG_H
#define PLOTEDIT1DLG_H

#include <QDialog>
#include <QMessageBox>
#include "my_parameters.h"

namespace Ui {
class plotedit1Dlg;
}

class plotedit1Dlg : public QDialog
{
    Q_OBJECT

public:
    explicit plotedit1Dlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~plotedit1Dlg();

    my_parameters *m_mcs;

    QString name0;
    QString name1;
    QString name2;
    QString name3;
    QString name4;

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

private slots:
    void on_pushButton_clicked();

private:
    Ui::plotedit1Dlg *ui;
};

#endif // PLOTEDIT1DLG_H
