#ifndef SNDATADLG_H
#define SNDATADLG_H

#include <QDialog>
#include <QMessageBox>
#include "myfont.h"
#include "my_parameters.h"
#if _MSC_VER
#include "TimeFunction.h"
#endif

namespace Ui {
class sndataDlg;
}

class sndataDlg : public QDialog
{
    Q_OBJECT

public:
    explicit sndataDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~sndataDlg();

    my_parameters *m_mcs;

    void init_dlg_show(QString topic);
    void close_dlg_show();

private slots:
    void on_pushButton_clicked();

private:
    Ui::sndataDlg *ui;
};

#endif // SNDATADLG_H
