#ifndef PLOTEDIT0DLG_H
#define PLOTEDIT0DLG_H

#include <QDialog>
#include <QMessageBox>
#include "myfont.h"
#include "my_parameters.h"

namespace Ui {
class plotedit0Dlg;
}

class plotedit0Dlg : public QDialog
{
    Q_OBJECT

public:
    explicit plotedit0Dlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~plotedit0Dlg();

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
    Ui::plotedit0Dlg *ui;
};

#endif // PLOTEDIT0DLG_H
