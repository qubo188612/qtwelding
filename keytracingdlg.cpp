#include "keytracingdlg.h"
#include "ui_keytracingdlg.h"

keytracingDlg::keytracingDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keytracingDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

//美化界面
#ifdef OPEN_BEAUTIFY_INTERFACE
    ui->pushButton->setStyleSheet(FONT_BUTTON_INFO);
    ui->timelineEdit->setStyleSheet(FONT_EDIT_INFO);
    ui->tracingfilenamecombo->setStyleSheet(FONT_COMBOBOX_INFO);
    ui->tracingtcpcombo->setStyleSheet(FONT_COMBOBOX_INFO);
    ui->record->setStyleSheet(FONT_TEXTBROWERS_INFO);
#endif


    for(int n=0;n<ROBOTTCPNUM;n++)
    {
        QString msg="TCP: "+QString::number(n);
        ui->tracingtcpcombo->addItem(msg);
    }
}

keytracingDlg::~keytracingDlg()
{
    delete ui;
}

void keytracingDlg::init_dlg_show()
{
    ui->tracingfilenamecombo->clear();
    for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
    {
        ui->tracingfilenamecombo->addItem(m_mcs->project->project_interweld_trace[n].name);
    }
    ui->record->clear();
}

void keytracingDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    ui->tracingfilenamecombo->clear();
    for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
    {
        ui->tracingfilenamecombo->addItem(m_mcs->project->project_interweld_trace[n].name);
    }
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_TRACING_KEY)//跟踪指令
        {
            int tcp=cmd.cmd_tracing_tcp;//获取到跟踪TCP
            QString name=cmd.cmd_tracing_name;
            int time=cmd.cmd_tracing_time;
            int weld_tracing_num;
            if(tcp>=0&&tcp<ROBOTTCPNUM)
            {
                ui->tracingtcpcombo->setCurrentIndex(tcp);
            }
            for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
            {
                if(name==m_mcs->project->project_interweld_trace[n].name)
                {
                    weld_tracing_num=n;//找到要储存的焊接轨道下标
                    break;
                }
            }
            if(weld_tracing_num>=0&&weld_tracing_num<ui->tracingfilenamecombo->count())
            {
                ui->tracingfilenamecombo->setCurrentIndex(weld_tracing_num);
            }
            ui->timelineEdit->setText(QString::number(time));
        }
    }
    ui->record->clear();
}

void keytracingDlg::close_dlg_show()
{

}

void keytracingDlg::setbutton(int name)
{
    if(name==0)
    {
        ui->pushButton->setText(QStringLiteral("插入跟踪指令"));
    }
    else
    {
        ui->pushButton->setText(QStringLiteral("替换跟踪指令"));
    }
}

void keytracingDlg::on_pushButton_clicked()
{
    int tcp=ui->tracingtcpcombo->currentIndex();
    QString name=ui->tracingfilenamecombo->currentText();
    int route=ui->tracingfilenamecombo->currentIndex();
    QString stime=ui->timelineEdit->text();
    if(tcp<0||tcp>ui->tracingtcpcombo->count()-1)
    {
        ui->record->append(QStringLiteral("请选择一个坐标系"));
        return;
    }
    if(route<0||route>ui->tracingfilenamecombo->count()-1)
    {
        ui->record->append(QStringLiteral("请选择一个跟踪轨迹工艺"));
        return;
    }
    if(ui->timelineEdit->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写采样点发送间隔时间"));
        return;
    }
    bool rc;
    int time=stime.toInt(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("采样点时间格式错误"));
        return;
    }
    my_cmd cmd;
    QString msg;
    msg=cmd.cmd_tracing(name,tcp,time);
    cmd_msg=msg;
    done(1);
}

