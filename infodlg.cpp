#include "infodlg.h"
#include "ui_infodlg.h"

infoDlg::infoDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::infoDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

    sndata=new sndataDlg(m_mcs);
}

infoDlg::~infoDlg()
{
    delete sndata;
    delete ui;
}

void infoDlg::init_dlg_show()
{
    QString msg=(char*)m_mcs->sn_data.SerialNo;
    ui->maclabel->setText(msg);

    if(m_mcs->sn_data.nLeftHours!=SN_TIME_SN)
    {
        int day=m_mcs->sn_data.nLeftHours/(60*24);
        int hours=(m_mcs->sn_data.nLeftHours-day*24*60)/60;
        int minute=m_mcs->sn_data.nLeftHours-day*24*60-hours*60;
        msg=QString::number(day)+QStringLiteral("天")+
            QString::number(hours)+QStringLiteral("时")+
            QString::number(minute)+QStringLiteral("分");
    }
    else
    {
        msg=QStringLiteral("无限制");
    }
    ui->nLeftHourslabel->setText(msg);

}

void infoDlg::close_dlg_show()
{

}

void infoDlg::on_SN_DataBtn_clicked()//修改有效期
{
    QString msg;
    int day=m_mcs->sn_data.nLeftHours/(60*24);
    int hours=(m_mcs->sn_data.nLeftHours-day*24*60)/60;
    int minute=m_mcs->sn_data.nLeftHours-day*24*60-hours*60;
    msg=QStringLiteral("剩余使用时长")+
            QString::number(day)+QStringLiteral("天")+
            QString::number(hours)+QStringLiteral("时")+
            QString::number(minute)+QStringLiteral("分")+
            QStringLiteral(",请输入序列号:");

    sndata->init_dlg_show(msg);
    sndata->setWindowTitle(QStringLiteral("软件激活"));
    int rc=sndata->exec();
    sndata->close_dlg_show();

    if(rc!=0)//确定
    {
        if(m_mcs->sn_data.nLeftHours!=SN_TIME_SN)
        {
            day=m_mcs->sn_data.nLeftHours/(60*24);
            hours=(m_mcs->sn_data.nLeftHours-day*24*60)/60;
            minute=m_mcs->sn_data.nLeftHours-day*24*60-hours*60;
            msg=QString::number(day)+QStringLiteral("天")+
                QString::number(hours)+QStringLiteral("时")+
                QString::number(minute)+QStringLiteral("分");
        }
        else
        {
            msg=QStringLiteral("无限制");
        }
        ui->nLeftHourslabel->setText(msg);
    }
}

