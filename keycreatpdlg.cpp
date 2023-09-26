#include "keycreatpdlg.h"
#include "ui_keycreatpdlg.h"

keycreatpDlg::keycreatpDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keycreatpDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸
}

keycreatpDlg::~keycreatpDlg()
{
    delete ui;
}

void keycreatpDlg::init_dlg_show()
{
    pointsname.clear();
    now_creatpline=-1;
    ui->creatplist->clear();
    ui->creatpcomboBox->clear();
    for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
    {
        ui->creatpcomboBox->addItem(m_mcs->project->project_robpos_trace[n].name);
    }
    now_pointname.clear();
    if(ui->creatpcomboBox->count()>0)
    {
        now_pointname=ui->creatpcomboBox->currentText();
    }
    ui->record->clear();
}

void keycreatpDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_CREATP_KEY)//点位生成轨迹指令
        {
            QString name=cmd.cmd_creatp_name;
            pointsname=cmd.cmd_creatp_pointsname;
            ui->creatpnamelineEdit->setText(name);
            now_creatpline=pointsname.size()-1;
            updatalistUi();
            ui->creatpcomboBox->clear();
            for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
            {
                ui->creatpcomboBox->addItem(m_mcs->project->project_robpos_trace[n].name);
            }
            now_pointname.clear();
            if(ui->creatpcomboBox->count()>0)
            {
                now_pointname=ui->creatpcomboBox->currentText();
            }
        }
    }
    ui->record->clear();
}

void keycreatpDlg::close_dlg_show()
{

}

void keycreatpDlg::setbutton(int name)
{
    if(name==0)
    {
        b_inster=false;
        ui->creatpBtn->setText(QStringLiteral("插入点位生成轨迹指令"));
    }
    else
    {
        b_inster=true;
        ui->creatpBtn->setText(QStringLiteral("替换点位生成轨迹指令"));
    }
}

void keycreatpDlg::updatalistUi()
{
    ui->creatplist->clear();
    for(int n=0;n<pointsname.size();n++)
    {
        QString msg=pointsname[n];
        ui->creatplist->addItem(msg);
    }
    ui->creatplist->setCurrentRow(now_creatpline);
}

void keycreatpDlg::on_creatplist_itemClicked(QListWidgetItem *item)
{
    now_creatpline=ui->creatplist->currentRow();
    ui->creatplist->setCurrentRow(now_creatpline);
}

//插入点位
void keycreatpDlg::on_creatpAddBtn_clicked()
{
    if(ui->creatpcomboBox->count()>0)
    {
        now_pointname=ui->creatpcomboBox->currentText();
    }
    if(now_pointname.isEmpty())
    {
        ui->record->append(QStringLiteral("请选择要插入的点名称"));
        return;
    }
    if(now_creatpline==pointsname.size()-1)
    {
        pointsname.push_back(now_pointname);
    }
    else
    {
        pointsname.insert(pointsname.begin()+now_creatpline+1,now_pointname);
    }
    now_creatpline++;
    updatalistUi();
    ui->record->append(QStringLiteral("插入点坐标完成"));
}

//删除点位
void keycreatpDlg::on_creatpDelBtn_clicked()
{
    if(now_creatpline>=0&&pointsname.size()>now_creatpline)
    {
        pointsname.erase(pointsname.begin()+now_creatpline);
        ui->record->append(QStringLiteral("删除点坐标完成"));
        if(now_creatpline>=pointsname.size())
            now_creatpline=now_creatpline-1;
        updatalistUi();
    }
    else
    {
        ui->record->append(QStringLiteral("请先选中要删除的点坐标"));
    }
}

//点位生成
void keycreatpDlg::on_creatpBtn_clicked()
{
    QString name=ui->creatpnamelineEdit->text();
    if(name.isEmpty())
    {
        ui->record->append(QStringLiteral("请输入要生成的轨迹名字"));
        return;
    }
    if(pointsname.size()<2)
    {
        ui->record->append(QStringLiteral("生成轨迹至少需要1个以上的点坐标"));
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
        for(int n=0;n<pointsname.size();n++)
        {
            QString pointname=pointsname[n];
            bool b_find=false;
            for(int t=0;t<m_mcs->project->project_robpos_trace.size();t++)
            {
                if(m_mcs->project->project_robpos_trace[t].name==pointname)
                {
                    b_find=1;
                    break;
                }
            }
            if(b_find==false)
            {
                ui->record->append(QStringLiteral("前面没有名为")+pointname+QStringLiteral("的坐标点"));
                return;
            }
        }
    }
    my_cmd cmd;
    QString msg=cmd.cmd_creatp(pointsname,name);
    ui->record->append(QStringLiteral("插入生成跟踪轨迹指令成功"));
    cmd_msg=msg;
    done(1);
}









