#include "keyscandlg.h"
#include "ui_keyscandlg.h"

keyscanDlg::keyscanDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keyscanDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
}

keyscanDlg::~keyscanDlg()
{
    delete ui;
}

void keyscanDlg::init_dlg_show()
{
    ui->record->clear();
}

void keyscanDlg::init_dlg_show(QString cmdlist)
{
    ui->record->clear();
}

void keyscanDlg::close_dlg_show()
{

}
