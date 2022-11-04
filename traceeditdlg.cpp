#include "traceeditdlg.h"
#include "ui_traceeditdlg.h"

traceeditDlg::traceeditDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::traceeditDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
}

traceeditDlg::~traceeditDlg()
{
    delete ui;
}

void traceeditDlg::init_dlg_show()
{
    ui->comboBox->clear();
    for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
    {
        ui->comboBox->addItem(m_mcs->project->project_scan_trace[n].name);
    }
}

void traceeditDlg::close_dlg_show()
{

}

void traceeditDlg::on_pushButton_clicked()
{
    if(ui->comboBox->currentIndex()>=0)
    {
        name=ui->comboBox->currentText();
        done(1);//返回值1
    }
    else
    {
        QMessageBox:: StandardButton result= QMessageBox::information(this, QString::fromLocal8Bit("提示信息"),
                                                                      QString::fromLocal8Bit("请选择一个扫描轨迹"),
                                                                      QMessageBox::Yes,
                                                                      QMessageBox::Yes
                                                                      );
    }
}

