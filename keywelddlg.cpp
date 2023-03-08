#include "keywelddlg.h"
#include "ui_keywelddlg.h"

keyweldDlg::keyweldDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keyweldDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
}

keyweldDlg::~keyweldDlg()
{
    delete ui;
}

void keyweldDlg::init_dlg_show()
{
    ui->record->clear();
}

void keyweldDlg::init_dlg_show(QString cmdlist)
{
    ui->record->clear();
}

void keyweldDlg::close_dlg_show()
{

}
