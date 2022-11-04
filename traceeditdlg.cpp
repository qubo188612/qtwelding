#include "traceeditdlg.h"
#include "ui_traceeditdlg.h"

traceeditDlg::traceeditDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::traceeditDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
}

traceeditDlg::~traceeditDlg()
{
    delete ui;
}

void traceeditDlg::init_dlg_show()
{

}

void traceeditDlg::close_dlg_show()
{

}
