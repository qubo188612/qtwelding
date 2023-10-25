#ifndef TRACEEDIT0DLG_H
#define TRACEEDIT0DLG_H

#include <QDialog>
#include <QMessageBox>
#include "myfont.h"
#include "my_parameters.h"


namespace Ui {
class traceedit0Dlg;
}

class traceedit0Dlg : public QDialog
{
    Q_OBJECT

public:
    explicit traceedit0Dlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~traceedit0Dlg();

    my_parameters *m_mcs;

    QString name;

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

private slots:
    void on_pushButton_clicked();

private:
    Ui::traceedit0Dlg *ui;
};

#endif // TRACEEDIT0DLG_H
