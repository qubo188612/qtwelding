#include "traceedit2dlg.h"
#include "ui_traceedit2dlg.h"

traceedit2Dlg::traceedit2Dlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::traceedit2Dlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
}

traceedit2Dlg::~traceedit2Dlg()
{
    delete ui;
}

void traceedit2Dlg::init_dlg_show()
{
    ui->comboBox_0->clear();
    for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
    {
        ui->comboBox_0->addItem(m_mcs->project->project_scan_trace[n].name);
    }
    ui->comboBox_1->clear();
    for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
    {
        ui->comboBox_1->addItem(m_mcs->project->project_scan_trace[n].name);
    }
    ui->comboBox_2->clear();
    for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
    {
        ui->comboBox_2->addItem(m_mcs->project->project_scan_trace[n].name);
    }
    ui->comboBox_3->clear();
    for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
    {
        ui->comboBox_3->addItem(m_mcs->project->project_scan_trace[n].name);
    }
    ui->comboBox_4->clear();
    for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
    {
        ui->comboBox_4->addItem(m_mcs->project->project_scan_trace[n].name);
    }
}

void traceedit2Dlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    int rc;
    ui->comboBox_0->clear();
    for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
    {
        ui->comboBox_0->addItem(m_mcs->project->project_scan_trace[n].name);
    }
    ui->comboBox_1->clear();
    for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
    {
        ui->comboBox_1->addItem(m_mcs->project->project_scan_trace[n].name);
    }
    ui->comboBox_2->clear();
    for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
    {
        ui->comboBox_2->addItem(m_mcs->project->project_scan_trace[n].name);
    }
    ui->comboBox_3->clear();
    for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
    {
        ui->comboBox_3->addItem(m_mcs->project->project_scan_trace[n].name);
    }
    ui->comboBox_4->clear();
    for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
    {
        ui->comboBox_4->addItem(m_mcs->project->project_scan_trace[n].name);
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
                case TRACE_EDIT_MODE_TOWPOINT_THREE_TO_ONE: //两端点三直线交点模式
                {
                    for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
                    {
                        if(scanname[0]==ui->comboBox_0->itemText(n))
                        {
                            ui->comboBox_0->setCurrentIndex(n);
                            break;
                        }
                    }
                    for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
                    {
                        if(scanname[1]==ui->comboBox_1->itemText(n))
                        {
                            ui->comboBox_1->setCurrentIndex(n);
                            break;
                        }
                    }
                    for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
                    {
                        if(scanname[2]==ui->comboBox_2->itemText(n))
                        {
                            ui->comboBox_2->setCurrentIndex(n);
                            break;
                        }
                    }
                    for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
                    {
                        if(scanname[3]==ui->comboBox_3->itemText(n))
                        {
                            ui->comboBox_3->setCurrentIndex(n);
                            break;
                        }
                    }
                    for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
                    {
                        if(scanname[4]==ui->comboBox_4->itemText(n))
                        {
                            ui->comboBox_4->setCurrentIndex(n);
                            break;
                        }
                    }
                }
                break;
            }
        }
    }
}


void traceedit2Dlg::close_dlg_show()
{

}

void traceedit2Dlg::on_pushButton_clicked()
{
    if(ui->comboBox_0->currentIndex()>=0)
    {
        name0=ui->comboBox_0->currentText();
    }
    else
    {
        QMessageBox:: StandardButton result= QMessageBox::information(this, QString::fromLocal8Bit("提示信息"),
                                                                      QString::fromLocal8Bit("请选择一个主要扫描轨迹"),
                                                                      QMessageBox::Yes,
                                                                      QMessageBox::Yes
                                                                      );
        return;
    }
    if(ui->comboBox_1->currentIndex()>=0)
    {
        name1=ui->comboBox_1->currentText();
    }
    else
    {
        QMessageBox:: StandardButton result= QMessageBox::information(this, QString::fromLocal8Bit("提示信息"),
                                                                      QString::fromLocal8Bit("请选择一个端点1的扫描轨迹1"),
                                                                      QMessageBox::Yes,
                                                                      QMessageBox::Yes
                                                                      );
        return;
    }
    if(ui->comboBox_2->currentIndex()>=0)
    {
        name2=ui->comboBox_2->currentText();
    }
    else
    {
        QMessageBox:: StandardButton result= QMessageBox::information(this, QString::fromLocal8Bit("提示信息"),
                                                                      QString::fromLocal8Bit("请选择一个端点1的扫描轨迹2"),
                                                                      QMessageBox::Yes,
                                                                      QMessageBox::Yes
                                                                      );
        return;
    }
    if(ui->comboBox_3->currentIndex()>=0)
    {
        name3=ui->comboBox_3->currentText();
    }
    else
    {
        QMessageBox:: StandardButton result= QMessageBox::information(this, QString::fromLocal8Bit("提示信息"),
                                                                      QString::fromLocal8Bit("请选择一个端点2的扫描轨迹1"),
                                                                      QMessageBox::Yes,
                                                                      QMessageBox::Yes
                                                                      );
        return;
    }
    if(ui->comboBox_4->currentIndex()>=0)
    {
        name4=ui->comboBox_4->currentText();
    }
    else
    {
        QMessageBox:: StandardButton result= QMessageBox::information(this, QString::fromLocal8Bit("提示信息"),
                                                                      QString::fromLocal8Bit("请选择一个端点2的扫描轨迹2"),
                                                                      QMessageBox::Yes,
                                                                      QMessageBox::Yes
                                                                      );
        return;
    }

    if(name0==name1||name0==name2||name1==name2)
    {
        QMessageBox:: StandardButton result= QMessageBox::information(this, QString::fromLocal8Bit("提示信息"),
                                                                      QString::fromLocal8Bit("选择的端点1三个轨迹中存在同名轨迹"),
                                                                      QMessageBox::Yes,
                                                                      QMessageBox::Yes
                                                                      );
        return;
    }
    else if(name0==name3||name0==name4||name3==name4)
    {
        QMessageBox:: StandardButton result= QMessageBox::information(this, QString::fromLocal8Bit("提示信息"),
                                                                      QString::fromLocal8Bit("选择的端点2三个轨迹中存在同名轨迹"),
                                                                      QMessageBox::Yes,
                                                                      QMessageBox::Yes
                                                                      );
        return;
    }
    else if((name3==name1&&name4==name2)||(name3==name2&&name4==name1))
    {
        QMessageBox:: StandardButton result= QMessageBox::information(this, QString::fromLocal8Bit("提示信息"),
                                                                      QString::fromLocal8Bit("端点1与端点2属于同一点"),
                                                                      QMessageBox::Yes,
                                                                      QMessageBox::Yes
                                                                      );
        return;
    }
    else
    {
        done(1);//返回值1
    }
}

