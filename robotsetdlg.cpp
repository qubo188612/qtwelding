#include "robotsetdlg.h"
#include "ui_robotsetdlg.h"

robotsetDlg::robotsetDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::robotsetDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

//美化界面
#ifdef OPEN_BEAUTIFY_INTERFACE
    ui->comboBox->setStyleSheet(FONT_COMBOBOX_INFO);
    ui->robot_ip->setStyleSheet(FONT_EDIT_INFO);
    ui->robot_model->setStyleSheet(FONT_COMBOBOX_INFO);
    ui->robot_outnum->setStyleSheet(FONT_COMBOBOX_INFO);
    ui->robot_port->setStyleSheet(FONT_EDIT_INFO);
    ui->robot_posure_model->setStyleSheet(FONT_COMBOBOX_INFO);
    ui->robot_remoteip->setStyleSheet(FONT_EDIT_INFO);
    ui->pushButton->setStyleSheet(FONT_BUTTON_INFO);
#endif

    for(int n=0;n<ROBOTTALNUM;n++)  //机器人控制台数
    {
        QString msg=QStringLiteral("机器人 ")+QString::number(n);
        ui->comboBox->addItem(msg);
    }
    for(int n=0;n<ROBOT_MODEL_NUM;n++)
    {
        QString msg=m_mcs->rob->robot_model_toQString((ROBOT_MODEL)n)+QStringLiteral("机器人");
        ui->robot_model->addItem(msg);
    }
    ui->robot_model->setCurrentIndex(m_mcs->rob->robot_model);
    ui->robot_outnum->setCurrentIndex(m_mcs->rob->out_num);
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
    if(m_mcs->rob->robot_model==ROBOT_MODEL_NULL)
    {
        ui->label_3->setEnabled(false);
        ui->robot_remoteip->setEnabled(false);
        ui->label_6->setEnabled(false);
        ui->robot_posure_model->setEnabled(false);
        ui->label_7->setEnabled(false);
        ui->robot_outnum->setEnabled(false);
    }
    else
    {
        ui->label_3->setEnabled(true);
        ui->robot_remoteip->setEnabled(true);
        ui->label_6->setEnabled(true);
        ui->robot_posure_model->setEnabled(true);
        ui->label_7->setEnabled(true);
        ui->robot_outnum->setEnabled(true);
    }
}

void robotsetDlg::close_dlg_show()
{

}

void robotsetDlg::on_robot_model_currentIndexChanged(int index)
{
    if(index==ROBOT_MODEL_NULL)
    {
        ui->label_3->setEnabled(false);
        ui->robot_remoteip->setEnabled(false);
        ui->label_6->setEnabled(false);
        ui->robot_posure_model->setEnabled(false);
        ui->label_7->setEnabled(false);
        ui->robot_outnum->setEnabled(false);
    }
    else
    {
        ui->label_3->setEnabled(true);
        ui->robot_remoteip->setEnabled(true);
        ui->label_6->setEnabled(true);
        ui->robot_posure_model->setEnabled(true);
        ui->label_7->setEnabled(true);
        ui->robot_outnum->setEnabled(true);
    }
}


void robotsetDlg::on_pushButton_clicked()
{
    QString ip=ui->robot_ip->text();
    if(false==IsIp(ip))
    {
        QMessageBox::information(this, QStringLiteral("错误"), QStringLiteral("机器人ip地址需要为本地127回环地址"));
        return;
    }
    m_mcs->rob->out_num=ui->robot_outnum->currentIndex();
    m_mcs->rob->robot_model=(ROBOT_MODEL)ui->robot_model->currentIndex();
    m_mcs->rob->cal_posture_model=(CAL_POSTURE)ui->robot_posure_model->currentIndex();
    m_mcs->rob->SaveRob((char*)ROBOTDATA_PATH_MOTO);
    m_mcs->ip->robot_ip[0].robot_ip.ip=ui->robot_ip->text();
    m_mcs->ip->robot_ip[0].robot_ip.port=ui->robot_port->text().toInt();
    m_mcs->ip->robot_ip[0].remote_ip.ip=ui->robot_remoteip->text();
    m_mcs->ip->SaveIP((char*)IPADDRESS_PATH_MOTO);
    done(1);
}

bool robotsetDlg::IsIp(QString currentIp)
{
    QHostAddress address(currentIp);
    return address.protocol() != QAbstractSocket::UnknownNetworkLayerProtocol && address.isLoopback();
}
