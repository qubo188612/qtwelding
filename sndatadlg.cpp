#include "sndatadlg.h"
#include "ui_sndatadlg.h"

sndataDlg::sndataDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sndataDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸
}

sndataDlg::~sndataDlg()
{
    delete ui;
}

void sndataDlg::init_dlg_show(QString topic)
{
    ui->addmac->setText(QString((char*)m_mcs->sn_data.SerialNo));
    ui->label->setText(topic);
    ui->state->clear();
    ui->lineEdit->clear();
}

void sndataDlg::close_dlg_show()
{

}

void sndataDlg::on_pushButton_clicked()
{
    QString err_msg;
    QString msg=ui->lineEdit->text();
    if(0!=m_mcs->sn_data.JudgeSN((Uint8*)msg.toStdString().c_str(),msg.toStdString().size(),&err_msg))
    {
        ui->state->setText(err_msg);
    }
    else
    {
        QString msg;
        if(m_mcs->sn_data.nLeftHours==SN_TIME_SN)
        {
            msg=QStringLiteral("序列号验证正确,有效期永久");
        }
        else
        {
            int day=m_mcs->sn_data.nLeftHours/(60*24);
            int hours=(m_mcs->sn_data.nLeftHours-day*24*60)/60;
            int minute=m_mcs->sn_data.nLeftHours-day*24*60-hours*60;
            msg=QStringLiteral("序列号验证正确,剩余使用时长")+
                    QString::number(day)+QStringLiteral("天")+
                    QString::number(hours)+QStringLiteral("时")+
                    QString::number(minute)+QStringLiteral("分");
        }
        QMessageBox:: StandardButton result= QMessageBox::information(this, QStringLiteral("提示信息"),
                                                                      msg,
                                                                      QMessageBox::Yes,
                                                                      QMessageBox::Yes
                                                                      );
        done(1);
    }
}

