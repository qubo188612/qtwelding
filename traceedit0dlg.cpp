#include "traceedit0dlg.h"
#include "ui_traceedit0dlg.h"

traceedit0Dlg::traceedit0Dlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::traceedit0Dlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸
}

traceedit0Dlg::~traceedit0Dlg()
{
    delete ui;
}

void traceedit0Dlg::init_dlg_show()
{
    ui->comboBox->clear();
    for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
    {
        ui->comboBox->addItem(m_mcs->project->project_scan_trace[n].name);
    }
}

void traceedit0Dlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    int rc;
    ui->comboBox->clear();
    for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
    {
        ui->comboBox->addItem(m_mcs->project->project_scan_trace[n].name);
    }
    rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_CREAT_KEY)//生成轨迹命令
        {
            std::vector<QString> scanname=cmd.cmd_creat_scanname;//获取到生成轨迹所需要的轨迹名字
            Trace_edit_mode mode=cmd.cmd_creat_mode;//获取到的轨迹生成模式
            switch(mode)
            {
                case TRACE_EDIT_MODE_ONE_TO_ONE://单扫单轨
                {
                    for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
                    {
                        if(scanname[0]==ui->comboBox->itemText(n))
                        {
                            ui->comboBox->setCurrentIndex(n);
                            break;
                        }
                    }
                }
                break;
            }
        }
    }
}

void traceedit0Dlg::close_dlg_show()
{

}

void traceedit0Dlg::on_pushButton_clicked()
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

