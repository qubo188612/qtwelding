#include "editcratfdlg.h"
#include "ui_editcratfdlg.h"

editcratfDlg::editcratfDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editcratfDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
}

editcratfDlg::~editcratfDlg()
{
    delete ui;
}


void editcratfDlg::init_dlg_show()
{

}

void editcratfDlg::close_dlg_show()
{

}

void editcratfDlg::on_BTN1_clicked()    //新建工艺
{
    done(EDITCRATFDLG_BTN1);//返回值1
}

void editcratfDlg::on_BTN2_clicked()    //选择现有工艺
{
    done(EDITCRATFDLG_BTN2);//返回值2
}

