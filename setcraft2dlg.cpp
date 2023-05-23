#include "setcraft2dlg.h"
#include "ui_setcraft2dlg.h"

setcraft2Dlg::setcraft2Dlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setcraft2Dlg)
{
    ui->setupUi(this);

    adoubleValidator_3 = new QDoubleValidator(0,9999,3,this);//限制3位小数

    ui->lineEdit_pendulum_swing->setValidator(adoubleValidator_3);
    ui->lineEdit_pendulum_phaseangle->setValidator(adoubleValidator_3);

    m_mcs=mcs;

    for(int n=0;n<PENDULUM_ID_TOTAL_NUM;n++)
    {
        QString msg=m_mcs->craft->Pendulum_mode_toQString((Pendulum_mode)n);
        ui->comboBox_pendulum_mode->addItem(msg);
    }
}

setcraft2Dlg::~setcraft2Dlg()
{
    delete adoubleValidator_3;
    delete ui;
}

void setcraft2Dlg::init_dlg_show()
{
    ui->comboBox_pendulum_mode->setModelColumn(m_mcs->craft->pendulum_mode);
    ui->lineEdit_pendulum_swing->setText(QString::number(m_mcs->craft->pendulum_swing,'f',ROBOT_POSE_DECIMAL_PLACE));
    ui->lineEdit_pendulum_phaseangle->setText(QString::number(m_mcs->craft->pendulum_phaseangle,'f',3));
}

void setcraft2Dlg::close_dlg_show()
{

}

void setcraft2Dlg::on_pushButton_2_clicked()
{
    m_mcs->craft->posturelist.clear();
    m_mcs->craft->pendulum_phaseangle=ui->lineEdit_pendulum_phaseangle->text().toFloat();
    m_mcs->craft->pendulum_swing=ui->lineEdit_pendulum_swing->text().toFloat();
    m_mcs->craft->SaveProject((char*)m_mcs->craft->craft_path.toStdString().c_str());
    done(1);
}


void setcraft2Dlg::on_comboBox_pendulum_mode_currentIndexChanged(int index)
{
    m_mcs->craft->pendulum_mode=(Pendulum_mode)index;
    UpdataUi();
}

void setcraft2Dlg::UpdataUi()
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


