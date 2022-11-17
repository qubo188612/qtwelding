#include "setcraft1dlg.h"
#include "ui_setcraft1dlg.h"

setcraft1Dlg::setcraft1Dlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setcraft1Dlg)
{
    ui->setupUi(this);

    adoubleValidator_3 = new QDoubleValidator(0,0,3,this);//限制3位小数
    adoubleValidator_pose = new QDoubleValidator(0,0,ROBOT_POSE_DECIMAL_PLACE,this);//限制3位小数
    adoubleValidator_posture = new QDoubleValidator(0,0,ROBOT_POSTURE_DECIMAL_PLACE,this);//限制4位小数
    ui->lineEdit_stX->setValidator(adoubleValidator_pose);
    ui->lineEdit_stY->setValidator(adoubleValidator_pose);
    ui->lineEdit_stZ->setValidator(adoubleValidator_pose);
    ui->lineEdit_stRX->setValidator(adoubleValidator_posture);
    ui->lineEdit_stRY->setValidator(adoubleValidator_posture);
    ui->lineEdit_stRZ->setValidator(adoubleValidator_posture);
    ui->lineEdit_edX->setValidator(adoubleValidator_pose);
    ui->lineEdit_edY->setValidator(adoubleValidator_pose);
    ui->lineEdit_edZ->setValidator(adoubleValidator_pose);
    ui->lineEdit_edRX->setValidator(adoubleValidator_posture);
    ui->lineEdit_edRY->setValidator(adoubleValidator_posture);
    ui->lineEdit_edRZ->setValidator(adoubleValidator_posture);
    ui->lineEdit_centerX->setValidator(adoubleValidator_pose);
    ui->lineEdit_centerY->setValidator(adoubleValidator_pose);
    ui->lineEdit_centerZ->setValidator(adoubleValidator_pose);
    ui->lineEdit_centerRX->setValidator(adoubleValidator_posture);
    ui->lineEdit_centerRY->setValidator(adoubleValidator_posture);
    ui->lineEdit_centerRZ->setValidator(adoubleValidator_posture);
    ui->lineEdit_X->setValidator(adoubleValidator_pose);
    ui->lineEdit_Y->setValidator(adoubleValidator_pose);
    ui->lineEdit_Z->setValidator(adoubleValidator_pose);
    ui->lineEdit_pendulum_swing->setValidator(adoubleValidator_3);
    ui->lineEdit_pendulum_phaseangle->setValidator(adoubleValidator_3);

    m_mcs=mcs;

    for(int n=0;n<PENDULUM_ID_TOTAL_NUM;n++)
    {
        QString msg=m_mcs->craft->Pendulum_mode_toQString((Pendulum_mode)n);
        ui->comboBox_pendulum_mode->addItem(msg);
    }
}

setcraft1Dlg::~setcraft1Dlg()
{
    delete adoubleValidator_3;
    delete adoubleValidator_pose;
    delete adoubleValidator_posture;
    delete ui;
}

void setcraft1Dlg::init_dlg_show()
{
    ui->comboBox_pendulum_mode->setModelColumn(m_mcs->craft->pendulum_mode);
    if(m_mcs->craft->posturelist.size()>=2)
    {
        ui->lineEdit_stX->setText(QString::number(m_mcs->craft->posturelist[0].X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_stY->setText(QString::number(m_mcs->craft->posturelist[0].Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_stZ->setText(QString::number(m_mcs->craft->posturelist[0].Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_stRX->setText(QString::number(m_mcs->craft->posturelist[0].RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_stRY->setText(QString::number(m_mcs->craft->posturelist[0].RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_stRZ->setText(QString::number(m_mcs->craft->posturelist[0].RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));

        ui->lineEdit_centerX->setText(QString::number(m_mcs->rob->TCPpos.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_centerY->setText(QString::number(m_mcs->rob->TCPpos.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_centerZ->setText(QString::number(m_mcs->rob->TCPpos.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_centerRX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_centerRY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_centerRZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));

        ui->lineEdit_edX->setText(QString::number(m_mcs->craft->posturelist[m_mcs->craft->posturelist.size()-1].X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_edY->setText(QString::number(m_mcs->craft->posturelist[m_mcs->craft->posturelist.size()-1].Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_edZ->setText(QString::number(m_mcs->craft->posturelist[m_mcs->craft->posturelist.size()-1].Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_edRX->setText(QString::number(m_mcs->craft->posturelist[m_mcs->craft->posturelist.size()-1].RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_edRY->setText(QString::number(m_mcs->craft->posturelist[m_mcs->craft->posturelist.size()-1].RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_edRZ->setText(QString::number(m_mcs->craft->posturelist[m_mcs->craft->posturelist.size()-1].RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    }
    else
    {
        ui->lineEdit_stX->setText(QString::number(m_mcs->rob->TCPpos.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_stY->setText(QString::number(m_mcs->rob->TCPpos.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_stZ->setText(QString::number(m_mcs->rob->TCPpos.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_stRX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_stRY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_stRZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));

        ui->lineEdit_centerX->setText(QString::number(m_mcs->rob->TCPpos.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_centerY->setText(QString::number(m_mcs->rob->TCPpos.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_centerZ->setText(QString::number(m_mcs->rob->TCPpos.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_centerRX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_centerRY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_centerRZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));

        ui->lineEdit_edX->setText(QString::number(m_mcs->rob->TCPpos.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_edY->setText(QString::number(m_mcs->rob->TCPpos.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_edZ->setText(QString::number(m_mcs->rob->TCPpos.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_edRX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_edRY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_edRZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    }
}

void setcraft1Dlg::close_dlg_show()
{

}

void setcraft1Dlg::on_pushButton_get_stpos_clicked()//获取起点坐标姿态
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
        ui->record->append(QString::fromLocal8Bit("获取起点坐标失败"));
    }
    else
    {
        ui->lineEdit_stX->setText(QString::number(m_mcs->rob->TCPpos.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_stY->setText(QString::number(m_mcs->rob->TCPpos.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_stZ->setText(QString::number(m_mcs->rob->TCPpos.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_stRX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_stRY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_stRZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->record->append(QString::fromLocal8Bit("获取起点坐标成功"));
    }
}


void setcraft1Dlg::on_pushButton_updata_stpos_clicked()//更新起点坐标姿态
{

}


void setcraft1Dlg::on_pushButton_get_edpos_clicked()//获取终点坐标姿态
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
        ui->record->append(QString::fromLocal8Bit("获取终点坐标失败"));
    }
    else
    {
        ui->lineEdit_edX->setText(QString::number(m_mcs->rob->TCPpos.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_edY->setText(QString::number(m_mcs->rob->TCPpos.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_edZ->setText(QString::number(m_mcs->rob->TCPpos.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_edRX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_edRY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_edRZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->record->append(QString::fromLocal8Bit("获取终点坐标成功"));
    }
}


void setcraft1Dlg::on_pushButton_updata_edpos_clicked()//更新终点坐标姿态
{

}


void setcraft1Dlg::on_pushButton_get_centerpos_clicked()//获取中间点坐标姿态
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
        ui->record->append(QString::fromLocal8Bit("获取中间点坐标失败"));
    }
    else
    {
        ui->lineEdit_centerX->setText(QString::number(m_mcs->rob->TCPpos.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_centerY->setText(QString::number(m_mcs->rob->TCPpos.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_centerZ->setText(QString::number(m_mcs->rob->TCPpos.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_centerRX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_centerRY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_centerRZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->record->append(QString::fromLocal8Bit("获取中间点坐标成功"));
    }
}


void setcraft1Dlg::on_pushButton_updata_centerpos_clicked()//插入中间点坐标姿态
{

}


void setcraft1Dlg::on_pushButton_del_clicked()//删除选中姿态
{

}


void setcraft1Dlg::on_pushButtonOK_clicked()//确定并保存
{

}

