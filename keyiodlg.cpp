#include "keyiodlg.h"
#include "ui_keyiodlg.h"

keyioDlg::keyioDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keyioDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
}

keyioDlg::~keyioDlg()
{
    delete ui;
}

void keyioDlg::init_dlg_show()
{
    ui->record->clear();
}

void keyioDlg::init_dlg_show(QString cmdlist)
{
    ui->record->clear();
}

void keyioDlg::close_dlg_show()
{

}
