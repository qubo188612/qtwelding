#ifndef SETCRAFT1DLG_H
#define SETCRAFT1DLG_H

#include <QDialog>
#include <my_parameters.h>

namespace Ui {
class setcraft1Dlg;
}

class setcraft1Dlg : public QDialog
{
    Q_OBJECT

public:
    explicit setcraft1Dlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~setcraft1Dlg();

    my_parameters *m_mcs;

    void init_dlg_show();
    void close_dlg_show();
private:
    Ui::setcraft1Dlg *ui;
};

#endif // SETCRAFT1DLG_H
