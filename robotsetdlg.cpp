#include "robotsetdlg.h"
#include "ui_robotsetdlg.h"

robotsetDlg::robotsetDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::robotsetDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;

    for(int n=0;n<ROBOTTALNUM;n++)
    {
        QString msg="机器人 "+QString::number(n);
        ui->comboBox->addItem(msg);
    }
    ui->robot_model->setCurrentIndex(m_mcs->rob->robot_model);
    ui->robot_ip->setText(m_mcs->ip->robot_ip[0].robot_ip.ip);
    ui->robot_port->setText(QString::number(m_mcs->ip->robot_ip[0].robot_ip.port));
    ui->robot_remoteip->setText(m_mcs->ip->robot_ip[0].remote_ip.ip);
    ui->robot_posure_model->setCurrentIndex(m_mcs->rob->cal_posture_model);
}

robotsetDlg::~robotsetDlg()
{
    delete ui;
}

void robotsetDlg::init_dlg_show()
{

}

void robotsetDlg::close_dlg_show()
{

}

void robotsetDlg::on_pushButton_clicked()
{
    m_mcs->rob->robot_model=(ROBOT_MODEL)ui->robot_model->currentIndex();
    m_mcs->rob->cal_posture_model=(CAL_POSTURE)ui->robot_posure_model->currentIndex();
    m_mcs->rob->SaveRob(ROBOTDATA_PATH_MOTO);
    m_mcs->ip->robot_ip[0].robot_ip.ip=ui->robot_ip->text();
    m_mcs->ip->robot_ip[0].robot_ip.port=ui->robot_port->text().toInt();
    m_mcs->ip->robot_ip[0].remote_ip.ip=ui->robot_remoteip->text();
    m_mcs->ip->SaveIP(IPADDRESS_PATH_MOTO);
}

