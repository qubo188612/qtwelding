#include "setcraft3dlg.h"
#include "ui_setcraft3dlg.h"

setcraft3Dlg::setcraft3Dlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setcraft3Dlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

//美化界面
#ifdef OPEN_BEAUTIFY_INTERFACE
    ui->lineEdit_downRX->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_downRY->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_downRZ->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_offsetsdownX->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_offsetsdownY->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_offsetsdownZ->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_offsetsupX->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_offsetsupY->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_offsetsupZ->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_offsetsX->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_offsetsY->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_offsetsZ->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_posture_distance->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_RX->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_RY->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_RZ->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_upRX->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_upRY->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_upRZ->setStyleSheet(FONT_EDIT_INFO);
    ui->pushButtonOK->setStyleSheet(FONT_BUTTON_INFO);
    ui->pushButton_get_centerpos->setStyleSheet(FONT_BUTTON_INFO);
    ui->pushButton_get_edpos->setStyleSheet(FONT_BUTTON_INFO);
    ui->pushButton_get_stpos->setStyleSheet(FONT_BUTTON_INFO);
    ui->record->setStyleSheet(FONT_TEXTBROWERS_INFO);
#endif

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
    ui->lineEdit_posture_distance->setValidator(adoubleValidator_pose);
}

setcraft3Dlg::~setcraft3Dlg()
{
    delete adoubleValidator_3;
    delete adoubleValidator_pose;
    delete adoubleValidator_posture;
    delete ui;
}

void setcraft3Dlg::init_dlg_show(bool b_file)
{
    this->b_file=b_file;
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
        ui->record->append(QStringLiteral("获取机器人坐标失败"));
    }
    else
    {
        ui->lineEdit_RX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_RY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_RZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->record->append(QStringLiteral("获取当前机器人平坡姿态完成"));
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
        ui->record->append(QStringLiteral("获取机器人坐标失败"));
    }
    else
    {
        ui->lineEdit_upRX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_upRY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_upRZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->record->append(QStringLiteral("获取当前机器人上坡姿态完成"));
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
        ui->record->append(QStringLiteral("获取机器人坐标失败"));
    }
    else
    {
        ui->lineEdit_downRX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_downRY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_downRZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->record->append(QStringLiteral("获取当前机器人下坡姿态完成"));
    }
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
    m_mcs->craft->posture_distance=ui->lineEdit_posture_distance->text().toFloat();
    if(b_file==true)
        m_mcs->craft->SaveProject(m_mcs->craft->craft_path);
    else
    {
        my_cmd cmd;
        std::vector<float> param(2);
        param[0]=m_mcs->craft->posture_distance;
        param[1]=(int)m_mcs->craft->weld_direction;
        cmd_msg=cmd.cmd_crafts(CRAFT_ID_CORRUGATED_POSTURE,m_mcs->craft->posturelist,param,m_mcs->craft->craft_name);
    }
    done(1);
}



