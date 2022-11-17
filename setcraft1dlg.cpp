#include "setcraft1dlg.h"
#include "ui_setcraft1dlg.h"

setcraft1Dlg::setcraft1Dlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setcraft1Dlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
}

setcraft1Dlg::~setcraft1Dlg()
{
    delete ui;
}

void setcraft1Dlg::init_dlg_show()
{

}

void setcraft1Dlg::close_dlg_show()
{

}

void setcraft1Dlg::on_pushButton_get_stpos_clicked()//获取起点坐标姿态
{

}


void setcraft1Dlg::on_pushButton_updata_stpos_clicked()//更新起点坐标姿态
{

}


void setcraft1Dlg::on_pushButton_get_edpos_clicked()//获取终点坐标姿态
{

}


void setcraft1Dlg::on_pushButton_updata_edpos_clicked()//更新终点坐标姿态
{

}


void setcraft1Dlg::on_pushButton_get_centerpos_clicked()//获取中间点坐标姿态
{

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

