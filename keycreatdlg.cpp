#include "keycreatdlg.h"
#include "ui_keycreatdlg.h"

keycreatDlg::keycreatDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keycreatDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
}

keycreatDlg::~keycreatDlg()
{
    delete ui;
}

void keycreatDlg::init_dlg_show()
{
    ui->record->clear();
}

void keycreatDlg::init_dlg_show(QString cmdlist)
{
    ui->record->clear();
}

void keycreatDlg::close_dlg_show()
{

}
