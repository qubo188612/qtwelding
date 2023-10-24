#include "plotedit0dlg.h"
#include "ui_plotedit0dlg.h"

plotedit0Dlg::plotedit0Dlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::plotedit0Dlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

//美化界面
#ifdef OPEN_BEAUTIFY_INTERFACE
    ui->comboBox_0->setStyleSheet(FONT_COMBOBOX_INFO);
    ui->comboBox_1->setStyleSheet(FONT_COMBOBOX_INFO);
    ui->comboBox_2->setStyleSheet(FONT_COMBOBOX_INFO);
    ui->pushButton->setStyleSheet(FONT_BUTTON_INFO);
#endif
}

plotedit0Dlg::~plotedit0Dlg()
{
    delete ui;
}

void plotedit0Dlg::init_dlg_show()
{
    ui->comboBox_0->clear();
    for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
    {
        ui->comboBox_0->addItem(m_mcs->project->project_weld_trace[n].name);
    }
    ui->comboBox_1->clear();
    for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
    {
        ui->comboBox_1->addItem(m_mcs->project->project_weld_trace[n].name);
    }
    ui->comboBox_2->clear();
    for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
    {
        ui->comboBox_2->addItem(m_mcs->project->project_weld_trace[n].name);
    }
}

void plotedit0Dlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    int rc;
    ui->comboBox_0->clear();
    for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
    {
        ui->comboBox_0->addItem(m_mcs->project->project_weld_trace[n].name);
    }
    ui->comboBox_1->clear();
    for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
    {
        ui->comboBox_1->addItem(m_mcs->project->project_weld_trace[n].name);
    }
    ui->comboBox_2->clear();
    for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
    {
        ui->comboBox_2->addItem(m_mcs->project->project_weld_trace[n].name);
    }
    rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_PLOTPOS_KEY)//生成计算点轨迹命令
        {
            std::vector<QString> scanname=cmd.cmd_plotpos_creatname;//获取到生成轨迹所需要的轨迹名字
            Plotpos_edit_mode mode=cmd.cmd_plotpos_mode;//获取到的轨迹生成模式
            switch(mode)
            {
                case PLOTPOS_EDIT_MODE_THREE_TO_ONE:  //三直线交点模式
                {
                    for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
                    {
                        if(scanname[0]==ui->comboBox_0->itemText(n))
                        {
                            ui->comboBox_0->setCurrentIndex(n);
                            break;
                        }
                    }
                    for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
                    {
                        if(scanname[1]==ui->comboBox_1->itemText(n))
                        {
                            ui->comboBox_1->setCurrentIndex(n);
                            break;
                        }
                    }
                    for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
                    {
                        if(scanname[2]==ui->comboBox_2->itemText(n))
                        {
                            ui->comboBox_2->setCurrentIndex(n);
                            break;
                        }
                    }
                }
                break;
            }
        }
    }
}

void plotedit0Dlg::close_dlg_show()
{

}


void plotedit0Dlg::on_pushButton_clicked()
{
    if(ui->comboBox_0->currentIndex()>=0)
    {
        name0=ui->comboBox_0->currentText();
    }
    else
    {
        QMessageBox:: StandardButton result= QMessageBox::information(this, QStringLiteral("提示信息"),
                                                                      QStringLiteral("请选择一个主要跟踪轨迹"),
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
        QMessageBox:: StandardButton result= QMessageBox::information(this, QStringLiteral("提示信息"),
                                                                      QStringLiteral("请选择一个跟踪轨迹1"),
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
        QMessageBox:: StandardButton result= QMessageBox::information(this, QStringLiteral("提示信息"),
                                                                      QStringLiteral("请选择一个跟踪轨迹2"),
                                                                      QMessageBox::Yes,
                                                                      QMessageBox::Yes
                                                                      );
        return;
    }
    if(name0==name1||name0==name2||name1==name2)
    {
        QMessageBox:: StandardButton result= QMessageBox::information(this, QStringLiteral("提示信息"),
                                                                      QStringLiteral("选择的三个轨迹中存在同名轨迹"),
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

