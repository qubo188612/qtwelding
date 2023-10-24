#ifndef INFODLG_H
#define INFODLG_H

#include <QDialog>
#include "my_parameters.h"
#include "myfont.h"
#include "sndatadlg.h"

namespace Ui {
class infoDlg;
}

class infoDlg : public QDialog
{
    Q_OBJECT

public:
    explicit infoDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~infoDlg();

    my_parameters *m_mcs;

    sndataDlg *sndata;

    void init_dlg_show();
    void close_dlg_show();

private slots:
    void on_SN_DataBtn_clicked();

private:
    Ui::infoDlg *ui;
};

#endif // INFODLG_H
