#include "setcraft1dlg.h"
#include "ui_setcraft1dlg.h"

setcraft1Dlg::setcraft1Dlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setcraft1Dlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
}

setcraft1Dlg::~setcraft1Dlg()
{
    delete ui;
}

void setcraft1Dlg::init_dlg_show()
{

}

void setcraft1Dlg::close_dlg_show()
{

}
