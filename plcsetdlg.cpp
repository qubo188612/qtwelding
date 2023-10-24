#include "plcsetdlg.h"
#include "ui_plcsetdlg.h"

plcsetDlg::plcsetDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::plcsetDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

//美化界面
#ifdef OPEN_BEAUTIFY_INTERFACE
    ui->plc_ip->setStyleSheet(FONT_EDIT_INFO);
    ui->plc_model->setStyleSheet(FONT_COMBOBOX_INFO);
    ui->plc_port->setStyleSheet(FONT_EDIT_INFO);
    ui->pushButton->setStyleSheet(FONT_BUTTON_INFO);
#endif

    for(int n=0;n<PLC_MODEL_NUM;n++)
    {
        QString msg=m_mcs->rob->plc_model_toQString((PLC_MODEL)n)+" PLC";
        ui->plc_model->addItem(msg);
    }
    ui->plc_model->setCurrentIndex(m_mcs->rob->plc_model);
    ui->plc_ip->setText(m_mcs->ip->robot_ip[0].plc_ip.ip);
    ui->plc_port->setText(QString::number(m_mcs->ip->robot_ip[0].plc_ip.port));
}

plcsetDlg::~plcsetDlg()
{
    delete ui;
}

void plcsetDlg::init_dlg_show()
{
    if(m_mcs->rob->plc_model==PLC_MODEL_NULL)
    {
        ui->label_3->setEnabled(false);
        ui->plc_ip->setEnabled(false);
        ui->plc_port->setEnabled(false);
    }
    else if(m_mcs->rob->plc_model==PLC_MODEL_MODBUSTCP)
    {
        ui->label_3->setEnabled(true);
        ui->plc_ip->setEnabled(true);
        ui->plc_port->setEnabled(true);
    }
}

void plcsetDlg::close_dlg_show()
{

}

void plcsetDlg::on_plc_model_currentIndexChanged(int index)
{
    if(index==PLC_MODEL_NULL)
    {
        ui->label_3->setEnabled(false);
        ui->plc_ip->setEnabled(false);
        ui->plc_port->setEnabled(false);
    }
    else if(index==PLC_MODEL_MODBUSTCP)
    {
        ui->label_3->setEnabled(true);
        ui->plc_ip->setEnabled(true);
        ui->plc_port->setEnabled(true);
    }
}

void plcsetDlg::on_pushButton_clicked()
{
    m_mcs->rob->plc_model=(PLC_MODEL)ui->plc_model->currentIndex();
    m_mcs->rob->SaveRob((char*)ROBOTDATA_PATH_MOTO);
    m_mcs->ip->robot_ip[0].plc_ip.ip=ui->plc_ip->text();
    m_mcs->ip->robot_ip[0].plc_ip.port=ui->plc_port->text().toInt();
    m_mcs->ip->SaveIP((char*)IPADDRESS_PATH_MOTO);
}

