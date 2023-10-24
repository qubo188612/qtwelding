#include "newcraftdlg.h"
#include "ui_newcraftdlg.h"

newcraftDlg::newcraftDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newcraftDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

//美化界面
#ifdef OPEN_BEAUTIFY_INTERFACE
    ui->craft_Id->setStyleSheet(FONT_LISTWIDGET_INFO);
    ui->pushButton->setStyleSheet(FONT_BUTTON_INFO);
    ui->record->setStyleSheet(FONT_TEXTBROWERS_INFO);
#endif

    QString msg;
    for(int n=0;n<CRAFT_ID_TOTAL_NUM;n++)
    {
        QString msg=QStringLiteral("工艺")+QString::number(n)+": "+m_mcs->craft->craft_Id_toQString((Craft_ID)n);
        ui->craft_Id->addItem(msg);
    }

    edittext=new edittextDlg();
}

newcraftDlg::~newcraftDlg()
{
    delete edittext;
    delete ui;
}

void newcraftDlg::init_dlg_show(bool b_file)
{
    this->b_file=b_file;
    now_craft_Id=ui->craft_Id->count();
    ui->craft_Id->setCurrentRow(now_craft_Id);//设置当前选项无效，也不选中其他
}

void newcraftDlg::close_dlg_show()
{

}

void newcraftDlg::on_pushButton_clicked()
{
    if(now_craft_Id>=ui->craft_Id->count())
    {
        ui->record->append(QStringLiteral("请先选中要新建的工艺类型"));
    }
    else
    {
        if(this->b_file==true)
        {
            QString fileName = QFileDialog::getSaveFileName(this, QStringLiteral("请选择要保存的新工艺路径"), "./CRAFT/.craft", "CRAFT(*.craft)");
            if(fileName.size()>0)
            {
                m_mcs->craft->craft_id=(Craft_ID)now_craft_Id;
                QString msg=fileName;
                if(fileName.size()>=6)
                {
                    QString tem1=".craft";
                    QString tem2=".CRAFT";
                    QString tem=fileName.mid(fileName.size()-6,6);
                    if(tem!=tem1&&tem!=tem2)//文件名末尾不是".craft"或".CRAFT"
                    {
                        msg=msg+".craft";
                    }
                }
                m_mcs->craft->craft_path=msg;
                done(1);
            }
            else
            {
                ui->record->append(QStringLiteral("保存操作未完成，请重新选择路径"));
            }
        }
        else
        {
            QString craftName;
            edittext->init_dlg_show(QStringLiteral("工艺名称:"));
            edittext->setWindowTitle(QStringLiteral("工艺名称"));
            int rc=edittext->exec();
            edittext->close_dlg_show();
            if(rc!=0)//确定
            {
                craftName=edittext->msg_edit;
                m_mcs->craft->craft_name=craftName;
                m_mcs->craft->craft_id=(Craft_ID)now_craft_Id;
                done(1);
            }
            else
            {
                ui->record->append(QStringLiteral("保存操作未完成，请重新命名"));
            }
        }
    }
}


void newcraftDlg::on_craft_Id_itemClicked(QListWidgetItem *item)
{
    now_craft_Id=ui->craft_Id->currentRow();
}

