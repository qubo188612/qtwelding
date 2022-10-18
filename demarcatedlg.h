#ifndef DEMARCATEDLG_H
#define DEMARCATEDLG_H

#include <QDialog>
#include "my_parameters.h"

namespace Ui {
class demarcateDlg;
}

class demarcateDlg : public QDialog
{
    Q_OBJECT

public:
    explicit demarcateDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~demarcateDlg();

    my_parameters *m_mcs;

    void init_dlg_show();
    void close_dlg_show();

private:
    Ui::demarcateDlg *ui;

};

#endif // DEMARCATEDLG_H
