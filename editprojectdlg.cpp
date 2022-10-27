#include "editprojectdlg.h"
#include "ui_editprojectdlg.h"

editprojectDlg::editprojectDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editprojectDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
}

editprojectDlg::~editprojectDlg()
{
    delete ui;
}

void editprojectDlg::init_dlg_show()
{

}

void editprojectDlg::close_dlg_show()
{

}


void editprojectDlg::on_BTN1_clicked()
{
    done(EDITPROJECTDLG_BTN1);//返回值0
}


void editprojectDlg::on_BTN2_clicked()
{
    done(EDITPROJECTDLG_BTN2);//返回值1
}

