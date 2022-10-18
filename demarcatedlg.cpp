#include "demarcatedlg.h"
#include "ui_demarcatedlg.h"

demarcateDlg::demarcateDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::demarcateDlg)
{
    ui->setupUi(this);

    m_mcs=mcs;  

}

demarcateDlg::~demarcateDlg()
{

    delete ui;
}

void demarcateDlg::init_dlg_show()
{

}

void demarcateDlg::close_dlg_show()
{

}
