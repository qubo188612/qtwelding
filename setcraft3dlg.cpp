#include "setcraft3dlg.h"
#include "ui_setcraft3dlg.h"

setcraft3Dlg::setcraft3Dlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setcraft3Dlg)
{
    ui->setupUi(this);

    adoubleValidator_3 = new QDoubleValidator(0,9999,3,this);//限制3位小数
    adoubleValidator_pose = new QDoubleValidator(ROBOT_POSE_DECIMAL_BOTTOM,ROBOT_POSE_DECIMAL_TOP,ROBOT_POSE_DECIMAL_PLACE,this);//限制3位小数
    adoubleValidator_posture = new QDoubleValidator(ROBOT_POSTURE_DECIMAL_BOTTOM,ROBOT_POSTURE_DECIMAL_TOP,ROBOT_POSTURE_DECIMAL_PLACE,this);//限制4位小数

    ui->lineEdit_RX->setValidator(adoubleValidator_posture);
    ui->lineEdit_RY->setValidator(adoubleValidator_posture);
    ui->lineEdit_RZ->setValidator(adoubleValidator_posture);
    ui->lineEdit_offsetsX->setValidator(adoubleValidator_pose);
    ui->lineEdit_offsetsY->setValidator(adoubleValidator_pose);
    ui->lineEdit_offsetsZ->setValidator(adoubleValidator_pose);
    ui->lineEdit_upRX->setValidator(adoubleValidator_posture);
    ui->lineEdit_upRY->setValidator(adoubleValidator_posture);
    ui->lineEdit_upRZ->setValidator(adoubleValidator_posture);
    ui->lineEdit_offsetsupX->setValidator(adoubleValidator_pose);
    ui->lineEdit_offsetsupY->setValidator(adoubleValidator_pose);
    ui->lineEdit_offsetsupZ->setValidator(adoubleValidator_pose);
    ui->lineEdit_downRX->setValidator(adoubleValidator_posture);
    ui->lineEdit_downRY->setValidator(adoubleValidator_posture);
    ui->lineEdit_downRZ->setValidator(adoubleValidator_posture);
    ui->lineEdit_offsetsdownX->setValidator(adoubleValidator_pose);
    ui->lineEdit_offsetsdownY->setValidator(adoubleValidator_pose);
    ui->lineEdit_offsetsdownZ->setValidator(adoubleValidator_pose);
    ui->lineEdit_pendulum_swing->setValidator(adoubleValidator_3);
    ui->lineEdit_pendulum_phaseangle->setValidator(adoubleValidator_3);
    ui->lineEdit_posture_distance->setValidator(adoubleValidator_pose);

    m_mcs=mcs;

    for(int n=0;n<PENDULUM_ID_TOTAL_NUM;n++)
    {
        QString msg=m_mcs->craft->Pendulum_mode_toQString((Pendulum_mode)n);
        ui->comboBox_pendulum_mode->addItem(msg);
    }
}

setcraft3Dlg::~setcraft3Dlg()
{
    delete adoubleValidator_3;
    delete adoubleValidator_pose;
    delete adoubleValidator_posture;
    delete ui;
}

void setcraft3Dlg::init_dlg_show()
{
    ui->comboBox_pendulum_mode->setModelColumn(m_mcs->craft->pendulum_mode);
    if(m_mcs->craft->posturelist.size()==3)
    {
        ui->lineEdit_offsetsX->setText(QString::number(m_mcs->craft->posturelist[0].Variable.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_offsetsY->setText(QString::number(m_mcs->craft->posturelist[0].Variable.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_offsetsZ->setText(QString::number(m_mcs->craft->posturelist[0].Variable.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_RX->setText(QString::number(m_mcs->craft->posturelist[0].posture.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_RY->setText(QString::number(m_mcs->craft->posturelist[0].posture.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_RZ->setText(QString::number(m_mcs->craft->posturelist[0].posture.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_offsetsupX->setText(QString::number(m_mcs->craft->posturelist[1].Variable.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_offsetsupY->setText(QString::number(m_mcs->craft->posturelist[1].Variable.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_offsetsupZ->setText(QString::number(m_mcs->craft->posturelist[1].Variable.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_upRX->setText(QString::number(m_mcs->craft->posturelist[1].posture.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_upRY->setText(QString::number(m_mcs->craft->posturelist[1].posture.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_upRZ->setText(QString::number(m_mcs->craft->posturelist[1].posture.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_offsetsdownX->setText(QString::number(m_mcs->craft->posturelist[2].Variable.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_offsetsdownY->setText(QString::number(m_mcs->craft->posturelist[2].Variable.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_offsetsdownZ->setText(QString::number(m_mcs->craft->posturelist[2].Variable.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_downRX->setText(QString::number(m_mcs->craft->posturelist[2].posture.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_downRY->setText(QString::number(m_mcs->craft->posturelist[2].posture.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_downRZ->setText(QString::number(m_mcs->craft->posturelist[2].posture.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    }
    else
    {
        ui->lineEdit_offsetsX->setText(QString::number(0,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_offsetsY->setText(QString::number(0,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_offsetsZ->setText(QString::number(0,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_RX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_RY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_RZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_offsetsupX->setText(QString::number(0,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_offsetsupY->setText(QString::number(0,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_offsetsupZ->setText(QString::number(0,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_upRX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_upRY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_upRZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_offsetsdownX->setText(QString::number(0,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_offsetsdownY->setText(QString::number(0,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_offsetsdownZ->setText(QString::number(0,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_downRX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_downRY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_downRZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    }
    ui->lineEdit_pendulum_swing->setText(QString::number(m_mcs->craft->pendulum_swing,'f',ROBOT_POSE_DECIMAL_PLACE));
    ui->lineEdit_pendulum_phaseangle->setText(QString::number(m_mcs->craft->pendulum_phaseangle,'f',3));
    ui->lineEdit_posture_distance->setText(QString::number(m_mcs->craft->posture_distance,'f',ROBOT_POSE_DECIMAL_PLACE));
    switch(m_mcs->craft->weld_direction)
    {
        case WELD_DIRECTION_X:
            ui->radioButton_x->setChecked(1);
        break;
        case WELD_DIRECTION_Y:
            ui->radioButton_y->setChecked(1);
        break;
    }
    ui->record->clear();
    UpdataUi();
}

void setcraft3Dlg::close_dlg_show()
{

}

void setcraft3Dlg::UpdataUi()
{
    switch(m_mcs->craft->pendulum_mode)
    {
        case PENDULUM_ID_FLAT:
        {
            ui->lineEdit_pendulum_phaseangle->setDisabled(false);
            ui->lineEdit_pendulum_swing->setDisabled(false);
        }
        break;
        default:
        {
            ui->lineEdit_pendulum_phaseangle->setDisabled(true);
            ui->lineEdit_pendulum_swing->setDisabled(true);
        }
        break;
    }
}

//获取平坡姿态
void setcraft3Dlg::on_pushButton_get_stpos_clicked()
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
        ui->record->append(QString::fromLocal8Bit("获取当前机器人平坡姿态完成"));
    }
}

//获取上坡姿态
void setcraft3Dlg::on_pushButton_get_edpos_clicked()
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
        ui->lineEdit_upRX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_upRY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_upRZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->record->append(QString::fromLocal8Bit("获取当前机器人上坡姿态完成"));
    }
}


//获取下坡姿态
void setcraft3Dlg::on_pushButton_get_centerpos_clicked()
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
        ui->lineEdit_downRX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_downRY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_downRZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->record->append(QString::fromLocal8Bit("获取当前机器人下坡姿态完成"));
    }
}

void setcraft3Dlg::on_comboBox_pendulum_mode_currentIndexChanged(int index)
{
    m_mcs->craft->pendulum_mode=(Pendulum_mode)index;
    UpdataUi();
}

//X走向
void setcraft3Dlg::on_radioButton_x_clicked()
{
    m_mcs->craft->weld_direction=WELD_DIRECTION_X;
    UpdataUi();
}

//Y走向
void setcraft3Dlg::on_radioButton_y_clicked()
{
    m_mcs->craft->weld_direction=WELD_DIRECTION_Y;
    UpdataUi();
}

//确定并保存
void setcraft3Dlg::on_pushButtonOK_clicked()
{
    m_mcs->craft->posturelist.resize(3);
    m_mcs->craft->posturelist[0].Variable.X=ui->lineEdit_offsetsX->text().toFloat();
    m_mcs->craft->posturelist[0].Variable.Y=ui->lineEdit_offsetsY->text().toFloat();
    m_mcs->craft->posturelist[0].Variable.Z=ui->lineEdit_offsetsZ->text().toFloat();
    m_mcs->craft->posturelist[0].posture.RX=ui->lineEdit_RX->text().toFloat();
    m_mcs->craft->posturelist[0].posture.RY=ui->lineEdit_RY->text().toFloat();
    m_mcs->craft->posturelist[0].posture.RZ=ui->lineEdit_RZ->text().toFloat();
    m_mcs->craft->posturelist[1].Variable.X=ui->lineEdit_offsetsupX->text().toFloat();
    m_mcs->craft->posturelist[1].Variable.Y=ui->lineEdit_offsetsupY->text().toFloat();
    m_mcs->craft->posturelist[1].Variable.Z=ui->lineEdit_offsetsupZ->text().toFloat();
    m_mcs->craft->posturelist[1].posture.RX=ui->lineEdit_upRX->text().toFloat();
    m_mcs->craft->posturelist[1].posture.RY=ui->lineEdit_upRY->text().toFloat();
    m_mcs->craft->posturelist[1].posture.RZ=ui->lineEdit_upRZ->text().toFloat();
    m_mcs->craft->posturelist[2].Variable.X=ui->lineEdit_offsetsdownX->text().toFloat();
    m_mcs->craft->posturelist[2].Variable.Y=ui->lineEdit_offsetsdownY->text().toFloat();
    m_mcs->craft->posturelist[2].Variable.Z=ui->lineEdit_offsetsdownZ->text().toFloat();
    m_mcs->craft->posturelist[2].posture.RX=ui->lineEdit_downRX->text().toFloat();
    m_mcs->craft->posturelist[2].posture.RY=ui->lineEdit_downRY->text().toFloat();
    m_mcs->craft->posturelist[2].posture.RZ=ui->lineEdit_downRZ->text().toFloat();
    m_mcs->craft->pendulum_phaseangle=ui->lineEdit_pendulum_phaseangle->text().toFloat();
    m_mcs->craft->pendulum_swing=ui->lineEdit_pendulum_swing->text().toFloat();
    m_mcs->craft->posture_distance=ui->lineEdit_posture_distance->text().toFloat();
    m_mcs->craft->SaveProject((char*)m_mcs->craft->craft_path.toStdString().c_str());
    done(1);
}



