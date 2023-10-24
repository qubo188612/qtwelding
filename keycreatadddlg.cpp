#include "keycreatadddlg.h"
#include "ui_keycreatadddlg.h"

keycreataddDlg::keycreataddDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keycreataddDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

//美化界面
#ifdef OPEN_BEAUTIFY_INTERFACE
    ui->creataddAddBtn->setStyleSheet(FONT_BUTTON_INFO);
    ui->creataddBtn->setStyleSheet(FONT_BUTTON_INFO);
    ui->creataddDelBtn->setStyleSheet(FONT_BUTTON_INFO);
    ui->creataddcomboBox->setStyleSheet(FONT_COMBOBOX_INFO);
    ui->creataddlist->setStyleSheet(FONT_LISTWIDGET_INFO);
    ui->creataddnamelineEdit->setStyleSheet(FONT_EDIT_INFO);
    ui->record->setStyleSheet(FONT_TEXTBROWERS_INFO);
#endif
}

keycreataddDlg::~keycreataddDlg()
{
    delete ui;
}

void keycreataddDlg::init_dlg_show()
{
    ui->record->clear();
    now_creataddline=-1;
    ui->creataddlist->clear();
    ui->creataddcomboBox->clear();
    for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
    {
        ui->creataddcomboBox->addItem(m_mcs->project->project_weld_trace[n].name);
    }
    now_weldname.clear();
    if(ui->creataddcomboBox->count()>0)
    {
        now_weldname=ui->creataddcomboBox->currentText();
    }
    ui->record->clear();
}

void keycreataddDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_CREATADD_KEY)//轨迹相加指令
        {
            QString name=cmd.cmd_creatadd_nameout;
            weldsname=cmd.cmd_creatadd_names;
            ui->creataddnamelineEdit->setText(name);
            now_creataddline=weldsname.size()-1;
            updatalistUi();
            ui->creataddcomboBox->clear();
            for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
            {
                ui->creataddcomboBox->addItem(m_mcs->project->project_weld_trace[n].name);
            }
            now_weldname.clear();
            if(ui->creataddcomboBox->count()>0)
            {
                now_weldname=ui->creataddcomboBox->currentText();
            }
        }
    }
    ui->record->clear();
}

void keycreataddDlg::close_dlg_show()
{

}

void keycreataddDlg::setbutton(int name)
{
    if(name==0)
    {
        b_inster=false;
        ui->creataddBtn->setText(QStringLiteral("插入相加生成轨迹指令"));
    }
    else
    {
        b_inster=true;
        ui->creataddBtn->setText(QStringLiteral("替换相加生成轨迹指令"));
    }
}

void keycreataddDlg::updatalistUi()
{
    ui->creataddlist->clear();
    for(int n=0;n<weldsname.size();n++)
    {
        QString msg=weldsname[n];
        ui->creataddlist->addItem(msg);
    }
    ui->creataddlist->setCurrentRow(now_creataddline);
}


void keycreataddDlg::on_creataddlist_itemClicked(QListWidgetItem *item)
{
    now_creataddline=ui->creataddlist->currentRow();
    ui->creataddlist->setCurrentRow(now_creataddline);
}

//插入轨迹
void keycreataddDlg::on_creataddAddBtn_clicked()
{
    if(ui->creataddcomboBox->count()>0)
    {
        now_weldname=ui->creataddcomboBox->currentText();
    }
    if(now_weldname.isEmpty())
    {
        ui->record->append(QStringLiteral("请选择要插入的轨迹名称"));
        return;
    }
    if(now_creataddline==weldsname.size()-1)
    {
        weldsname.push_back(now_weldname);
    }
    else
    {
        weldsname.insert(weldsname.begin()+now_creataddline+1,now_weldname);
    }
    now_creataddline++;
    updatalistUi();
    ui->record->append(QStringLiteral("插入轨迹完成"));
}

//删除轨迹
void keycreataddDlg::on_creataddDelBtn_clicked()
{
    if(now_creataddline>=0&&weldsname.size()>now_creataddline)
    {
        weldsname.erase(weldsname.begin()+now_creataddline);
        ui->record->append(QStringLiteral("删除轨迹坐标完成"));
        if(now_creataddline>=weldsname.size())
            now_creataddline=now_creataddline-1;
        updatalistUi();
    }
    else
    {
        ui->record->append(QStringLiteral("请先选中要删除的轨迹"));
    }
}

//相加生成轨迹
void keycreataddDlg::on_creataddBtn_clicked()
{
    QString name=ui->creataddnamelineEdit->text();
    if(name.isEmpty())
    {
        ui->record->append(QStringLiteral("请输入要生成的轨迹名字"));
        return;
    }
    if(weldsname.size()<2)
    {
        ui->record->append(QStringLiteral("生成轨迹至少需要1个以上的轨迹"));
        return;
    }
    if(b_inster==false)
    {
        for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
        {
            if(name==m_mcs->project->project_weld_trace[n].name)
            {
                ui->record->append(QStringLiteral("生成的轨迹与已有的轨迹重名"));
                return;
            }
        }
        for(int n=0;n<weldsname.size();n++)
        {
            QString weldname=weldsname[n];
            bool b_find=false;
            for(int t=0;t<m_mcs->project->project_weld_trace.size();t++)
            {
                if(m_mcs->project->project_weld_trace[t].name==weldname)
                {
                    b_find=1;
                    break;
                }
            }
            if(b_find==false)
            {
                ui->record->append(QStringLiteral("前面没有名为")+weldname+QStringLiteral("的轨迹"));
                return;
            }
        }
    }
    my_cmd cmd;
    QString msg=cmd.cmd_creatadd(weldsname,name);
    ui->record->append(QStringLiteral("插入相加生成轨迹指令成功"));
    cmd_msg=msg;
    done(1);
}

