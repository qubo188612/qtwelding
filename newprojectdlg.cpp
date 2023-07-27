#include "newprojectdlg.h"
#include "ui_newprojectdlg.h"

newprojectDlg::newprojectDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newprojectDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

    QString msg;
    for(int n=0;n<PROJECT_ID_TOTAL_NUM;n++)
    {
        QString msg=QString::fromLocal8Bit("工程")+QString::number(n)+": "+m_mcs->project->project_Id_toQString((Project_ID)n);
        ui->project_Id->addItem(msg);
    }
}

newprojectDlg::~newprojectDlg()
{
    delete ui;
}

void newprojectDlg::init_dlg_show()
{
    now_project_Id=ui->project_Id->count();
    ui->project_Id->setCurrentRow(now_project_Id);//设置当前选项无效，也不选中其他
    ui->project_name->clear();
}

void newprojectDlg::close_dlg_show()
{

}

void newprojectDlg::on_pushButton_clicked()
{
    QString project_name=ui->project_name->text();
    if(project_name.size()==0)
    {
        ui->record->append(QString::fromLocal8Bit("请先选填写要新建的工程名称"));
    }
    else if(now_project_Id>=ui->project_Id->count())
    {
        ui->record->append(QString::fromLocal8Bit("请先选中要新建的工程类型"));
    }
    else
    {
        QString fileName = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("请选择要保存的新工程路径"), "./DATA/.json", "JSON(*.json)");
        if(fileName.size()>0)
        {
            m_mcs->project->project_name=project_name;
            m_mcs->project->project_Id=(Project_ID)now_project_Id;
            QString msg=fileName;
            if(fileName.size()>=5)
            {
                QString tem1=".json";
                QString tem2=".JSON";
                QString tem=fileName.mid(fileName.size()-5,5);
                if(tem!=tem1&&tem!=tem2)//文件名末尾不是".json"或".JSON"
                {
                    msg=msg+".json";
                }
            }
            m_mcs->project->project_path=msg;
            m_mcs->project->SaveProject((char*)msg.toStdString().c_str());
            done(1);
        }
        else
        {
            ui->record->append(QString::fromLocal8Bit("保存操作未完成，请重新选择路径"));
        }
    }
}


void newprojectDlg::on_project_Id_itemClicked(QListWidgetItem *item)
{
    now_project_Id=ui->project_Id->currentRow();
}

