#include "setcraft2dlg.h"
#include "ui_setcraft2dlg.h"

setcraft2Dlg::setcraft2Dlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setcraft2Dlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

    adoubleValidator_pose = new QDoubleValidator(ROBOT_POSE_DECIMAL_BOTTOM,ROBOT_POSE_DECIMAL_TOP,ROBOT_POSE_DECIMAL_PLACE,this);//限制3位小数

    ui->lineEdit_X->setValidator(adoubleValidator_pose);
    ui->lineEdit_Y->setValidator(adoubleValidator_pose);
    ui->lineEdit_Z->setValidator(adoubleValidator_pose);
}

setcraft2Dlg::~setcraft2Dlg()
{
    delete adoubleValidator_pose;
    delete ui;
}

void setcraft2Dlg::init_dlg_show()
{
    if(m_mcs->craft->posturelist.size()==1)
    {
        ui->lineEdit_X->setText(QString::number(m_mcs->craft->posturelist[0].Variable.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_Y->setText(QString::number(m_mcs->craft->posturelist[0].Variable.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_Z->setText(QString::number(m_mcs->craft->posturelist[0].Variable.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
    }
    else
    {
        ui->lineEdit_X->setText(QString::number(0,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_Y->setText(QString::number(0,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_Z->setText(QString::number(0,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    }
    ui->record->clear();
}

void setcraft2Dlg::close_dlg_show()
{

}

void setcraft2Dlg::on_pushButton_clicked()
{
    m_mcs->craft->posturelist.resize(1);
    m_mcs->craft->posturelist[0].Variable.X=ui->lineEdit_X->text().toFloat();
    m_mcs->craft->posturelist[0].Variable.Y=ui->lineEdit_Y->text().toFloat();
    m_mcs->craft->posturelist[0].Variable.Z=ui->lineEdit_Z->text().toFloat();
    m_mcs->craft->SaveProject((char*)m_mcs->craft->craft_path.toStdString().c_str());
    done(1);
}

