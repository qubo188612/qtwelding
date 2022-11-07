#include "editcraftdlg.h"
#include "ui_editcraftdlg.h"

editcraftDlg::editcraftDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editcraftDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
}

editcraftDlg::~editcraftDlg()
{
    delete ui;
}


void editcraftDlg::init_dlg_show()
{

}

void editcraftDlg::close_dlg_show()
{

}

void editcraftDlg::on_BTN1_clicked()    //新建工艺
{
    done(EDITCRAFTDLG_BTN1);//返回值1
}

void editcraftDlg::on_BTN2_clicked()    //选择现有工艺
{
    done(EDITCRAFTDLG_BTN2);//返回值2
}

