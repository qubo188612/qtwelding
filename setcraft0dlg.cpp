#include "setcraft0dlg.h"
#include "ui_setcraft0dlg.h"

setcraft0Dlg::setcraft0Dlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setcraft0Dlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

    adoubleValidator_3 = new QDoubleValidator(0,9999,3,this);//限制3位小数
    adoubleValidator_pose = new QDoubleValidator(ROBOT_POSE_DECIMAL_BOTTOM,ROBOT_POSE_DECIMAL_TOP,ROBOT_POSE_DECIMAL_PLACE,this);//限制3位小数
    adoubleValidator_posture = new QDoubleValidator(ROBOT_POSTURE_DECIMAL_BOTTOM,ROBOT_POSTURE_DECIMAL_TOP,ROBOT_POSTURE_DECIMAL_PLACE,this);//限制4位小数
    ui->lineEdit_X->setValidator(adoubleValidator_pose);
    ui->lineEdit_Y->setValidator(adoubleValidator_pose);
    ui->lineEdit_Z->setValidator(adoubleValidator_pose);
    ui->lineEdit_RX->setValidator(adoubleValidator_posture);
    ui->lineEdit_RY->setValidator(adoubleValidator_posture);
    ui->lineEdit_RZ->setValidator(adoubleValidator_posture);
}

setcraft0Dlg::~setcraft0Dlg()
{
    delete adoubleValidator_3;
    delete adoubleValidator_pose;
    delete adoubleValidator_posture;
    delete ui;
}

void setcraft0Dlg::init_dlg_show()
{
    if(m_mcs->craft->posturelist.size()==1)
    {
        ui->lineEdit_X->setText(QString::number(m_mcs->craft->posturelist[0].Variable.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_Y->setText(QString::number(m_mcs->craft->posturelist[0].Variable.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_Z->setText(QString::number(m_mcs->craft->posturelist[0].Variable.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_RX->setText(QString::number(m_mcs->craft->posturelist[0].posture.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_RY->setText(QString::number(m_mcs->craft->posturelist[0].posture.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_RZ->setText(QString::number(m_mcs->craft->posturelist[0].posture.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    }
    else
    {
        ui->lineEdit_X->setText(QString::number(0,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_Y->setText(QString::number(0,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_Z->setText(QString::number(0,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_RX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_RY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_RZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    }
    ui->record->clear();
    UpdataUi();
}

void setcraft0Dlg::close_dlg_show()
{

}

void setcraft0Dlg::on_pushButton_clicked()//获取当前焊接姿态
{
    int num=0;
    m_mcs->rob->TCPpos.nEn=false;
    while (m_mcs->rob->TCPpos.nEn==false)
    {
        if(num>10)
        {
            break;
        }
        usleep(ROB_WORK_DELAY_STEP);
        num++;
    }
    if(m_mcs->rob->TCPpos.nEn==false)
    {
        ui->record->append(QString::fromLocal8Bit("获取机器人坐标失败"));
    }
    else
    {
        ui->lineEdit_RX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_RY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_RZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->record->append(QString::fromLocal8Bit("获取当前机器人坐标完成"));
    }
}

void setcraft0Dlg::on_pushButton_2_clicked()//确定并保存
{
    m_mcs->craft->posturelist.resize(1);
    m_mcs->craft->posturelist[0].Variable.X=ui->lineEdit_X->text().toFloat();
    m_mcs->craft->posturelist[0].Variable.Y=ui->lineEdit_Y->text().toFloat();
    m_mcs->craft->posturelist[0].Variable.Z=ui->lineEdit_Z->text().toFloat();
    m_mcs->craft->posturelist[0].posture.RX=ui->lineEdit_RX->text().toFloat();
    m_mcs->craft->posturelist[0].posture.RY=ui->lineEdit_RY->text().toFloat();
    m_mcs->craft->posturelist[0].posture.RZ=ui->lineEdit_RZ->text().toFloat(); 
    m_mcs->craft->SaveProject((char*)m_mcs->craft->craft_path.toStdString().c_str());
    done(1);
}

void setcraft0Dlg::UpdataUi()
{

}


