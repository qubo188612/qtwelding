#include "weldsetdlg.h"
#include "ui_weldsetdlg.h"

weldsetDlg::weldsetDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::weldsetDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

//美化界面
#ifdef OPEN_BEAUTIFY_INTERFACE
    ui->weld_ip->setStyleSheet(FONT_EDIT_INFO);
    ui->weld_model->setStyleSheet(FONT_COMBOBOX_INFO);
    ui->pushButton->setStyleSheet(FONT_BUTTON_INFO);
#endif

    for(int n=0;n<WELD_MODEL_NUM;n++)
    {
        QString msg=m_mcs->rob->weld_model_toQString((WELD_MODEL)n)+QStringLiteral("焊机");
        ui->weld_model->addItem(msg);
    }
    ui->weld_model->setCurrentIndex(m_mcs->rob->weld_model);
    ui->weld_ip->setText(m_mcs->ip->robot_ip[0].weld_ip.ip);
}

weldsetDlg::~weldsetDlg()
{
    delete ui;
}

void weldsetDlg::init_dlg_show()
{
    if(m_mcs->rob->weld_model==WELD_MODEL_NULL||m_mcs->rob->weld_model==WELD_MODEL_ROBOT_LINK)
    {
        ui->label_3->setEnabled(false);
        ui->weld_ip->setEnabled(false);
    }
    else
    {
        ui->label_3->setEnabled(true);
        ui->weld_ip->setEnabled(true);
    }
}

void weldsetDlg::close_dlg_show()
{

}

void weldsetDlg::on_weld_model_currentIndexChanged(int index)
{
    if(index==WELD_MODEL_NULL||index==WELD_MODEL_ROBOT_LINK)
    {
        ui->label_3->setEnabled(false);
        ui->weld_ip->setEnabled(false);
    }
    else
    {
        ui->label_3->setEnabled(true);
        ui->weld_ip->setEnabled(true);
    }
}

void weldsetDlg::on_pushButton_clicked()
{
    m_mcs->rob->weld_model=(WELD_MODEL)ui->weld_model->currentIndex();
    m_mcs->rob->SaveRob((char*)ROBOTDATA_PATH_MOTO);
    m_mcs->ip->robot_ip[0].weld_ip.ip=ui->weld_ip->text();
    m_mcs->ip->SaveIP((char*)IPADDRESS_PATH_MOTO);
}



