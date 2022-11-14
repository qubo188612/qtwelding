#ifndef TRACEEDIT3DLG_H
#define TRACEEDIT3DLG_H

#include <QDialog>
#include <QMessageBox>
#include "my_parameters.h"


namespace Ui {
class traceedit3Dlg;
}

class traceedit3Dlg : public QDialog
{
    Q_OBJECT

public:
    explicit traceedit3Dlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~traceedit3Dlg();

    my_parameters *m_mcs;

    QString name0;
    QString name1;
    QString name2;

    void init_dlg_show();
    void close_dlg_show();

private slots:
    void on_pushButton_clicked();

private:
    Ui::traceedit3Dlg *ui;
};

#endif // TRACEEDIT3DLG_H
