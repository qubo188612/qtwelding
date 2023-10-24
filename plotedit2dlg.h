#ifndef PLOTEDIT2DLG_H
#define PLOTEDIT2DLG_H

#include <QDialog>
#include <QMessageBox>
#include "myfont.h"
#include "my_parameters.h"

namespace Ui {
class plotedit2Dlg;
}

class plotedit2Dlg : public QDialog
{
    Q_OBJECT

public:
    explicit plotedit2Dlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~plotedit2Dlg();

    my_parameters *m_mcs;

    QString name0;
    QString name1;
    QString name2;
    QString name3;

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

private slots:
    void on_pushButton_clicked();

private:
    Ui::plotedit2Dlg *ui;
};

#endif // PLOTEDIT2DLG_H
