#include "keycreatfdlg.h"
#include "ui_keycreatfdlg.h"

keycreatfDlg::keycreatfDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keycreatfDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸
}

keycreatfDlg::~keycreatfDlg()
{
    delete ui;
}

void keycreatfDlg::init_dlg_show()
{
    ui->record->clear();
}

void keycreatfDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_CREATF_KEY)//导入文件生成轨迹指令
        {
            QString namefile=cmd.cmd_creatf_filename;
            QString nameout=cmd.cmd_creatf_name;
            ui->creatffilename->setText(namefile);
            ui->creatfname->setText(nameout);
        }
    }
    ui->record->clear();
}

void keycreatfDlg::close_dlg_show()
{

}

void keycreatfDlg::setbutton(int name)
{
    if(name==0)
    {
        b_inster=false;
        ui->creatfBtn->setText(QStringLiteral("插入跟踪轨迹指令"));
    }
    else
    {
        b_inster=true;
        ui->creatfBtn->setText(QStringLiteral("替换跟踪轨迹指令"));
    }
}


//修改路径
void keycreatfDlg::on_creatfilepathBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, QStringLiteral("请选择要导入的焊接轨迹文件"), "./log/", "TXT(*.txt)");
    if(fileName.size()>0)
    {
        std::vector<RobPos> weld;
        if(0!=m_mcs->tosendbuffer->loadlog_creat(fileName,weld))
        {
            ui->record->append(QStringLiteral("焊接轨迹文件读取失败"));
            return;
        }
        ui->record->append(QStringLiteral("焊接轨迹文件读取成功"));
        ui->creatfilepathBtn->setText(fileName);
    }
}


void keycreatfDlg::on_creatfBtn_clicked()
{
    QString namefile=ui->creatffilename->text();
    QString nameout=ui->creatfname->text();
    if(nameout.isEmpty())
    {
        ui->record->append(QStringLiteral("请填写生成跟踪轨迹名"));
        return;
    }
    if(namefile.isEmpty())
    {
        ui->record->append(QStringLiteral("请填写焊接轨迹文件"));
        return;
    }
    std::vector<RobPos> weld;
    int rc=m_mcs->tosendbuffer->loadlog_creat(namefile,weld);
    if(rc==1)
    {
        ui->record->append(QStringLiteral("该路径找不到轨迹文件"));
        return;
    }
    else if(rc==2)
    {
        ui->record->append(QStringLiteral("焊接轨迹文件读取失败"));
        return;
    }
    if(b_inster==false)
    {
        for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
        {
            if(nameout==m_mcs->project->project_weld_trace[n].name)
            {
                ui->record->append(QStringLiteral("跟踪轨迹与已有的轨迹重名"));
                return;
            }
        }
    }
    my_cmd cmd;
    QString msg;
    msg=cmd.cmd_creatf(namefile,nameout);
    ui->record->append(QStringLiteral("插入跟踪轨迹指令成功"));
    cmd_msg=msg;
    done(1);
}

