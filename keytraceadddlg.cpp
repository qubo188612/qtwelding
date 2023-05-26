#include "keytraceadddlg.h"
#include "ui_keytraceadddlg.h"

keytraceaddDlg::keytraceaddDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keytraceaddDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
}

keytraceaddDlg::~keytraceaddDlg()
{
    delete ui;
}

void keytraceaddDlg::init_dlg_show()
{
    ui->tracingfilenamecombo_1->clear();
    for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
    {
        ui->tracingfilenamecombo_1->addItem(m_mcs->project->project_interweld_trace[n].name);
    }
    ui->tracingfilenamecombo_2->clear();
    for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
    {
        ui->tracingfilenamecombo_2->addItem(m_mcs->project->project_interweld_trace[n].name);
    }
    ui->record->clear();
}

void keytraceaddDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    ui->tracingfilenamecombo_1->clear();
    for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
    {
        ui->tracingfilenamecombo_1->addItem(m_mcs->project->project_interweld_trace[n].name);
    }
    ui->tracingfilenamecombo_2->clear();
    for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
    {
        ui->tracingfilenamecombo_2->addItem(m_mcs->project->project_interweld_trace[n].name);
    }
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_TRACEADD_KEY)//跟踪轨迹相加指令
        {
            QString name1=cmd.cmd_traceadd_name1;
            QString name2=cmd.cmd_traceadd_name2;
            QString nameout=cmd.cmd_traceadd_nameout;
            int weld_traceadd_num1;
            int weld_traceadd_num2;
            for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
            {
                if(name1==m_mcs->project->project_interweld_trace[n].name)
                {
                    weld_traceadd_num1=n;//找到要储存的焊接轨道下标
                    break;
                }
            }
            for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
            {
                if(name2==m_mcs->project->project_interweld_trace[n].name)
                {
                    weld_traceadd_num2=n;//找到要储存的焊接轨道下标
                    break;
                }
            }
            if(weld_traceadd_num1>=0&&weld_traceadd_num1<ui->tracingfilenamecombo_1->count())
            {
                ui->tracingfilenamecombo_1->setCurrentIndex(weld_traceadd_num1);
            }
            if(weld_traceadd_num2>=0&&weld_traceadd_num2<ui->tracingfilenamecombo_2->count())
            {
                ui->tracingfilenamecombo_2->setCurrentIndex(weld_traceadd_num2);
            }
            ui->tracefilename->setText(nameout);
        }
    }
    ui->record->clear();
}

void keytraceaddDlg::close_dlg_show()
{

}

void keytraceaddDlg::setbutton(int name)
{
    if(name==0)
    {
        ui->pushButton->setText(QString::fromLocal8Bit("插入跟踪轨迹相加指令"));
    }
    else
    {
        ui->pushButton->setText(QString::fromLocal8Bit("替换跟踪轨迹相加指令"));
    }
}

void keytraceaddDlg::on_pushButton_clicked()
{
    QString name1=ui->tracingfilenamecombo_1->currentText();
    int route1=ui->tracingfilenamecombo_1->currentIndex();
    QString name2=ui->tracingfilenamecombo_2->currentText();
    int route2=ui->tracingfilenamecombo_2->currentIndex();
    QString nameout=ui->tracefilename->text();
    if(route1<0||route1>ui->tracingfilenamecombo_1->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择一个跟踪轨迹1工艺"));
        return;
    }
    if(route2<0||route2>ui->tracingfilenamecombo_2->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择一个跟踪轨迹2工艺"));
        return;
    }
    if(nameout.isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写生成的跟踪轨迹工艺名字"));
        return;
    }
    my_cmd cmd;
    QString msg;
    msg=cmd.cmd_traceadd(name1,name2,nameout);
    cmd_msg=msg;
    done(1);
}

