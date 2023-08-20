#include "plotedit2dlg.h"
#include "ui_plotedit2dlg.h"

plotedit2Dlg::plotedit2Dlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::plotedit2Dlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸
}

plotedit2Dlg::~plotedit2Dlg()
{
    delete ui;
}

void plotedit2Dlg::init_dlg_show()
{
    ui->comboBox_0->clear();
    for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
    {
        ui->comboBox_0->addItem(m_mcs->project->project_weld_trace[n].name);
    }
    ui->comboBox_1->clear();
    for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
    {
        ui->comboBox_1->addItem(m_mcs->project->project_robpos_trace[n].name);
    }
    ui->comboBox_2->clear();
    for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
    {
        ui->comboBox_2->addItem(m_mcs->project->project_robpos_trace[n].name);
    }
    ui->comboBox_3->clear();
    for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
    {
        ui->comboBox_3->addItem(m_mcs->project->project_robpos_trace[n].name);
    }
}

void plotedit2Dlg::init_dlg_show(QString cmdlist)
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
    for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
    {
        ui->comboBox_1->addItem(m_mcs->project->project_robpos_trace[n].name);
    }
    ui->comboBox_2->clear();
    for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
    {
        ui->comboBox_2->addItem(m_mcs->project->project_robpos_trace[n].name);
    }
    ui->comboBox_3->clear();
    for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
    {
        ui->comboBox_3->addItem(m_mcs->project->project_robpos_trace[n].name);
    }
    rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_PLOTPOS_KEY)//生成计算点轨迹命令
        {
            std::vector<QString> scanname=cmd.cmd_plotpos_creatname;//获取到生成轨迹所需要的轨迹名字
            std::vector<QString> pointsname=cmd.cmd_plotpos_pointsname;//获取到生成轨迹所需要的轨迹名字
            Plotpos_edit_mode mode=cmd.cmd_plotpos_mode;//获取到的轨迹生成模式
            switch(mode)
            {
                case PLOTPOS_EDIT_MODE_LINE_THREEPOINTS_TO_ONE:  //直线与三点交点模式
                {
                    for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
                    {
                        if(scanname[0]==ui->comboBox_0->itemText(n))
                        {
                            ui->comboBox_0->setCurrentIndex(n);
                            break;
                        }
                    }
                    for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
                    {
                        if(pointsname[0]==ui->comboBox_0->itemText(n))
                        {
                            ui->comboBox_1->setCurrentIndex(n);
                            break;
                        }
                    }
                    for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
                    {
                        if(pointsname[1]==ui->comboBox_1->itemText(n))
                        {
                            ui->comboBox_2->setCurrentIndex(n);
                            break;
                        }
                    }
                    for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
                    {
                        if(pointsname[2]==ui->comboBox_2->itemText(n))
                        {
                            ui->comboBox_3->setCurrentIndex(n);
                            break;
                        }
                    }
                }
                break;
            }
        }
    }
}

void plotedit2Dlg::close_dlg_show()
{

}

void plotedit2Dlg::on_pushButton_clicked()
{
    if(ui->comboBox_0->currentIndex()>=0)
    {
        name0=ui->comboBox_0->currentText();
    }
    else
    {
        QMessageBox:: StandardButton result= QMessageBox::information(this, QString::fromLocal8Bit("提示信息"),
                                                                      QString::fromLocal8Bit("请选择一个主要跟踪轨迹"),
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
                                                                      QString::fromLocal8Bit("请选择一个拟合平面点1"),
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
                                                                      QString::fromLocal8Bit("请选择一个拟合平面点2"),
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
                                                                      QString::fromLocal8Bit("请选择一个拟合平面点3"),
                                                                      QMessageBox::Yes,
                                                                      QMessageBox::Yes
                                                                      );
        return;
    }
    if(name1==name2||name1==name3||name2==name3)
    {
        QMessageBox:: StandardButton result= QMessageBox::information(this, QString::fromLocal8Bit("提示信息"),
                                                                      QString::fromLocal8Bit("选择的三个点中存在同名点"),
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

