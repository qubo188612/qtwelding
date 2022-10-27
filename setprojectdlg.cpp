#include "setprojectdlg.h"
#include "ui_setprojectdlg.h"

setprojectDlg::setprojectDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setprojectDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
}

setprojectDlg::~setprojectDlg()
{
    delete ui;
}

void setprojectDlg::init_dlg_show()
{

}

void setprojectDlg::close_dlg_show()
{

}
