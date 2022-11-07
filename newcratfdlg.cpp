#include "newcratfdlg.h"
#include "ui_newcratfdlg.h"

newcratfDlg::newcratfDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newcratfDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
}

newcratfDlg::~newcratfDlg()
{
    delete ui;
}

void newcratfDlg::init_dlg_show()
{

}

void newcratfDlg::close_dlg_show()
{

}
