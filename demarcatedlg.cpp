#include "demarcatedlg.h"
#include "ui_demarcatedlg.h"

demarcateDlg::demarcateDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::demarcateDlg)
{
    ui->setupUi(this);

    m_mcs=mcs;  

    switch(m_mcs->e2proomdata.demdlg_radio_mod)
    {
        case 0:
            ui->radio1->setChecked(1);
        break;
        case 1:
            ui->radio2->setChecked(1);
        break;
    }

    ui->a00_label->setText(QString::number(m_mcs->e2proomdata.demdlg_R(0,0),'f',3));
    ui->a01_label->setText(QString::number(m_mcs->e2proomdata.demdlg_R(0,1),'f',3));
    ui->a02_label->setText(QString::number(m_mcs->e2proomdata.demdlg_R(0,2),'f',3));
    ui->a10_label->setText(QString::number(m_mcs->e2proomdata.demdlg_R(1,0),'f',3));
    ui->a11_label->setText(QString::number(m_mcs->e2proomdata.demdlg_R(1,1),'f',3));
    ui->a12_label->setText(QString::number(m_mcs->e2proomdata.demdlg_R(1,2),'f',3));
    ui->a20_label->setText(QString::number(m_mcs->e2proomdata.demdlg_R(2,0),'f',3));
    ui->a21_label->setText(QString::number(m_mcs->e2proomdata.demdlg_R(2,1),'f',3));
    ui->a22_label->setText(QString::number(m_mcs->e2proomdata.demdlg_R(2,2),'f',3));
    ui->t0_label->setText(QString::number(m_mcs->e2proomdata.demdlg_T(0),'f',3));
    ui->t1_label->setText(QString::number(m_mcs->e2proomdata.demdlg_T(1),'f',3));
    ui->t2_label->setText(QString::number(m_mcs->e2proomdata.demdlg_T(2),'f',3));
}

demarcateDlg::~demarcateDlg()
{

    delete ui;
}

void demarcateDlg::init_dlg_show()
{

}

void demarcateDlg::close_dlg_show()
{

}

void demarcateDlg::on_radio1_clicked()      //眼在手上
{
    m_mcs->e2proomdata.demdlg_radio_mod=0;
}


void demarcateDlg::on_radio2_clicked()      //眼在手外
{
    m_mcs->e2proomdata.demdlg_radio_mod=1;
}

void demarcateDlg::on_pushButton_clicked()  //添加TCP点
{

}

void demarcateDlg::on_pushButton_3_clicked() //删除TCP点
{

}


void demarcateDlg::on_pushButton_2_clicked()   //替换TCP点
{

}


void demarcateDlg::on_pushButton_8_clicked()    //清空TCP点
{

}


void demarcateDlg::on_pushButton_4_clicked()    //添加激光头点
{

}


void demarcateDlg::on_pushButton_6_clicked()    //删除激光头点
{

}


void demarcateDlg::on_pushButton_5_clicked()    //替换激光头点
{

}


void demarcateDlg::on_pushButton_9_clicked()     //清空激光头点
{

}


void demarcateDlg::on_pushButton_7_clicked()      //计算标定结果
{

}

