#include "keytracedlg.h"
#include "ui_keytracedlg.h"

keytraceDlg::keytraceDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keytraceDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
}

keytraceDlg::~keytraceDlg()
{
    delete ui;
}

void keytraceDlg::init_dlg_show()
{
    ui->record->clear();
}

void keytraceDlg::init_dlg_show(QString cmdlist)
{
    ui->record->clear();
}

void keytraceDlg::close_dlg_show()
{

}
