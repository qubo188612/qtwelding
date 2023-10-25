#ifndef TRACEEDIT1DLG_H
#define TRACEEDIT1DLG_H

#include <QDialog>
#include <QMessageBox>
#include "myfont.h"
#include "my_parameters.h"


namespace Ui {
class traceedit1Dlg;
}

class traceedit1Dlg : public QDialog
{
    Q_OBJECT

public:
    explicit traceedit1Dlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~traceedit1Dlg();

    my_parameters *m_mcs;

    QString name0;
    QString name1;
    QString name2;

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

private slots:
    void on_pushButton_clicked();

private:
    Ui::traceedit1Dlg *ui;
};

#endif // TRACEEDIT1DLG_H
