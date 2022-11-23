#ifndef TRACEEDIT2DLG_H
#define TRACEEDIT2DLG_H

#include <QDialog>
#include <QMessageBox>
#include "my_parameters.h"

namespace Ui {
class traceedit2Dlg;
}

class traceedit2Dlg : public QDialog
{
    Q_OBJECT

public:
    explicit traceedit2Dlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~traceedit2Dlg();

    my_parameters *m_mcs;

    QString name0;
    QString name1;
    QString name2;
    QString name3;
    QString name4;

    void init_dlg_show();
    void close_dlg_show();

private slots:
    void on_pushButton_clicked();

private:
    Ui::traceedit2Dlg *ui;
};

#endif // TRACEEDIT2DLG_H
