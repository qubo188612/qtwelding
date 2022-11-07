#include "newcraftdlg.h"
#include "ui_newcraftdlg.h"

newcraftDlg::newcraftDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newcraftDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;

    QString msg;
    for(int n=0;n<CRAFT_ID_TOTAL_NUM;n++)
    {
        QString msg=QString::fromLocal8Bit("工艺")+QString::number(n)+": "+m_mcs->craft->craft_Id_toQString((Craft_ID)n);
        ui->craft_Id->addItem(msg);
    }
}

newcraftDlg::~newcraftDlg()
{
    delete ui;
}

void newcraftDlg::init_dlg_show()
{
    now_craft_Id=ui->craft_Id->count();
    ui->craft_Id->setCurrentRow(now_craft_Id);//设置当前选项无效，也不选中其他
    ui->craft_name->clear();
}

void newcraftDlg::close_dlg_show()
{

}

void newcraftDlg::on_pushButton_clicked()
{
    QString craft_name=ui->craft_name->text();
    if(craft_name.size()==0)
    {
        ui->record->append(QString::fromLocal8Bit("请先选填写要新建的工艺名称"));
    }
    else if(now_craft_Id>=ui->craft_Id->count())
    {
        ui->record->append(QString::fromLocal8Bit("请先选中要新建的工艺类型"));
    }
    else
    {
        QString fileName = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("请选择要保存的新工艺路径"), "./CRAFT/.craft", "CRAFT(*.craft)");
        if(fileName.size()>0)
        {
            m_mcs->craft->craft_name=craft_name;
            m_mcs->craft->craft_id=(Craft_ID)now_craft_Id;
            QString msg=fileName;
            if(fileName.size()>=5)
            {
                QString tem1=".craft";
                QString tem2=".CRAFT";
                QString tem=fileName.mid(fileName.size()-5,5);
                if(tem!=tem1&&tem!=tem2)//文件名末尾不是".craft"或".CRAFT"
                {
                    msg=msg+".craft";
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


void newcraftDlg::on_craft_Id_itemClicked(QListWidgetItem *item)
{
    now_craft_Id=ui->craft_Id->currentRow();
}

