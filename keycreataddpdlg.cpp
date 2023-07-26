#include "keycreataddpdlg.h"
#include "ui_keycreataddpdlg.h"

keycreataddpDlg::keycreataddpDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keycreataddpDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;

    for(int n=0;n<CREATADDP_EDIT_ID_TOTAL_NUM;n++)
    {
        QString msg=Creataddp_edit_mode_toQString((Creataddp_edit_mode)n);
        ui->traceaddpmodecombo->addItem(msg);
    }
}

keycreataddpDlg::~keycreataddpDlg()
{
    delete ui;
}

void keycreataddpDlg::init_dlg_show()
{
    ui->traceaddpweldcombo->clear();
    for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
    {
        ui->traceaddpweldcombo->addItem(m_mcs->project->project_weld_trace[n].name);
    }
    ui->traceaddpointcombo->clear();
    for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
    {
        ui->traceaddpointcombo->addItem(m_mcs->project->projecr_robpos_trace[n].name);
    }
    ui->record->clear();
}

void keycreataddpDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_CREATADDP_KEY)//轨迹相加指令
        {
            QString name=cmd.cmd_creataddp_nameout;
            QString weldname=cmd.cmd_creataddp_weldname;
            QString pointname=cmd.cmd_creataddp_pointname;
            int mode=cmd.cmd_creataddp_mode;
            int weld_trace_creatnum;//搜索到的轨迹名称
            int point_trace_creatnum;//搜索到的点名称

            ui->traceaddpweldcombo->clear();
            for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
            {
                ui->traceaddpweldcombo->addItem(m_mcs->project->project_weld_trace[n].name);
            }
            ui->traceaddpointcombo->clear();
            for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
            {
                ui->traceaddpointcombo->addItem(m_mcs->project->projecr_robpos_trace[n].name);
            }

            for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
            {
                if(weldname==m_mcs->project->project_weld_trace[n].name)
                {
                    weld_trace_creatnum=n;//找到要储存的焊接轨道下标
                    break;
                }
            }
            if(weld_trace_creatnum>=0&&weld_trace_creatnum<ui->traceaddpweldcombo->count())
            {
                ui->traceaddpweldcombo->setCurrentIndex(weld_trace_creatnum);
            }
            for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
            {
                if(pointname==m_mcs->project->projecr_robpos_trace[n].name)
                {
                    point_trace_creatnum=n;//找到要储存的焊接轨道下标
                    break;
                }
            }
            if(point_trace_creatnum>=0&&point_trace_creatnum<ui->traceaddpointcombo->count())
            {
                ui->traceaddpointcombo->setCurrentIndex(point_trace_creatnum);
            }
            if(mode>=0&&mode<ui->traceaddpmodecombo->count())
            {
                ui->traceaddpmodecombo->setCurrentIndex(mode);
            }
            ui->traceaddpname->setText(name);
        }
    }
    ui->record->clear();
}

void keycreataddpDlg::close_dlg_show()
{

}

void keycreataddpDlg::setbutton(int name)
{
    if(name==0)
    {
        b_inster=false;
        ui->pushButton->setText(QString::fromLocal8Bit("插入跟踪轨迹相加指令"));
    }
    else
    {
        b_inster=true;
        ui->pushButton->setText(QString::fromLocal8Bit("替换跟踪轨迹相加指令"));
    }
}

//跟踪轨迹相加指令
void keycreataddpDlg::on_pushButton_clicked()
{
    QString weldname=ui->traceaddpweldcombo->currentText();
    int routeweld=ui->traceaddpweldcombo->currentIndex();
    QString pointname=ui->traceaddpointcombo->currentText();
    int routepoint=ui->traceaddpointcombo->currentIndex();
    QString nameout=ui->traceaddpname->text();
    int mode=ui->traceaddpmodecombo->currentIndex();
    if(routeweld<0||routeweld>ui->traceaddpweldcombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择一个跟踪轨迹"));
        return;
    }
    if(routepoint<0||routepoint>ui->traceaddpointcombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择一个点位"));
        return;
    }
    if(mode<0||mode>ui->traceaddpmodecombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择一个相加模式"));
        return;
    }
    if(nameout.isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写生成的跟踪轨迹名字"));
        return;
    }
    my_cmd cmd;
    QString msg;
    msg=cmd.cmd_creataddp(weldname,pointname,(Creataddp_edit_mode)mode,nameout);
    cmd_msg=msg;
    done(1);
}

