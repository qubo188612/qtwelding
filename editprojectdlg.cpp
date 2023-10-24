#include "editprojectdlg.h"
#include "ui_editprojectdlg.h"

editprojectDlg::editprojectDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editprojectDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

//美化界面
#ifdef OPEN_BEAUTIFY_INTERFACE
    ui->BTN1->setStyleSheet(FONT_EDITPROGECT_BUTTON_BIG_INFO);
    ui->BTN2->setStyleSheet(FONT_EDITPROGECT_BUTTON_BIG_INFO);
#endif
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
    done(EDITPROJECTDLG_BTN1);//返回值1
}


void editprojectDlg::on_BTN2_clicked()
{
    done(EDITPROJECTDLG_BTN2);//返回值2
}

