#include "weldsetdlg.h"
#include "ui_weldsetdlg.h"

weldsetDlg::weldsetDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::weldsetDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

    for(int n=0;n<WELD_MODEL_NUM;n++)
    {
        QString msg=m_mcs->rob->weld_model_toQString((WELD_MODEL)n)+"焊机";
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
    if(index==WELD_MODEL_NULL||m_mcs->rob->weld_model==WELD_MODEL_ROBOT_LINK)
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
    m_mcs->rob->SaveRob(ROBOTDATA_PATH_MOTO);
    m_mcs->ip->robot_ip[0].weld_ip.ip=ui->weld_ip->text();
    m_mcs->ip->SaveIP(IPADDRESS_PATH_MOTO);
}



