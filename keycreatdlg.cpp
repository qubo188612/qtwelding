#include "keycreatdlg.h"
#include "ui_keycreatdlg.h"

keycreatDlg::keycreatDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keycreatDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;

    traceedit0=new traceedit0Dlg(mcs);
    traceedit1=new traceedit1Dlg(mcs);
    traceedit2=new traceedit2Dlg(mcs);
}

keycreatDlg::~keycreatDlg()
{
    delete traceedit0;
    delete traceedit1;
    delete traceedit2;
    delete ui;
}

void keycreatDlg::init_dlg_show()
{
    cmd_list_in.clear();
    ui->record->clear();
}

void keycreatDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    cmd_list_in=cmdlist;
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_CREAT_KEY)//生成轨迹命令
        {
            //这里添加轨迹生成
            QString name=cmd.cmd_creat_name;//获取到的生成的轨迹名字
            Trace_edit_mode mode=cmd.cmd_creat_mode;//获取到的轨迹生成模式
            ui->tracename->setText(name);
            if(mode>=0&&mode<=ui->traceeditcombo->count())
            {
                ui->traceeditcombo->setCurrentIndex(mode);
            }
        }
    }
    ui->record->clear();
}

void keycreatDlg::close_dlg_show()
{

}

void keycreatDlg::setbutton(int name)
{
    if(name==0)
    {
        b_inster=false;
        ui->traceeditBtn->setText(QString::fromLocal8Bit("插入生成跟踪轨迹指令"));
    }
    else
    {
        b_inster=true;
        ui->traceeditBtn->setText(QString::fromLocal8Bit("替换生成跟踪轨迹指令"));
    }
}

void keycreatDlg::set_now_cmdline(int num)
{
    now_cmdline=num;
}

//生成轨迹指令
void keycreatDlg::on_traceeditBtn_clicked()
{
    int rc;
    std::vector<QString> err_msg;
    rc=m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg);
    if(0==rc||b_inster==true)
    {
        std::vector<QString> err_msg;
        QString name=ui->tracename->text();
        if(name.size()==0)
        {
            ui->record->append(QString::fromLocal8Bit("请输入要插入的轨迹名字"));
            return;
        }
        if(b_inster==false)
        {
            for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
            {
                if(name==m_mcs->project->project_weld_trace[n].name)
                {
                    ui->record->append(QString::fromLocal8Bit("跟踪轨迹与已有的轨迹重名"));
                    return;
                }
            }
        }
        m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(now_cmdline+1,err_msg);
        if(m_mcs->project->project_scan_trace.size()<=0)
        {
            ui->record->append(QString::fromLocal8Bit("当前指令位置没有可用的扫描轨迹"));
            return;
        }
        Trace_edit_mode trace_edit_mode=(Trace_edit_mode)ui->traceeditcombo->currentIndex();
        switch(trace_edit_mode)
        {
            case TRACE_EDIT_MODE_ONE_TO_ONE://单扫对单轨道模式
            {
                if(b_inster==false)
                {
                    traceedit0->init_dlg_show();
                }
                else
                {
                    traceedit0->init_dlg_show(cmd_list_in);
                }
                traceedit0->setWindowTitle(QString::fromLocal8Bit("生成跟踪轨迹(单扫对单轨模式)"));
                int rc=traceedit0->exec();
                traceedit0->close_dlg_show();
                if(rc!=0)//确定保存生成轨迹
                {
                    my_cmd cmd;
                    std::vector<QString> scanname(1);

                    scanname[0]=traceedit0->name;
                    QString msg=cmd.cmd_creat(trace_edit_mode,scanname,name);
                    ui->record->append(QString::fromLocal8Bit("插入生成跟踪轨迹指令成功"));
                    cmd_msg=msg;
                    done(1);
                }
            }
            break;
            case TRACE_EDIT_MODE_THREE_TO_ONE:  //三直线交点模式
            {
                if(b_inster==false)
                {
                    traceedit1->init_dlg_show();
                }
                else
                {
                    traceedit1->init_dlg_show(cmd_list_in);
                }
                traceedit1->setWindowTitle(QString::fromLocal8Bit("生成跟踪轨迹(三直线交点模式)"));
                int rc=traceedit1->exec();
                traceedit1->close_dlg_show();
                if(rc!=0)//确定保存生成轨迹
                {
                    my_cmd cmd;
                    std::vector<QString> scanname(3);

                    scanname[0]=traceedit1->name0;
                    scanname[1]=traceedit1->name1;
                    scanname[2]=traceedit1->name2;
                    QString msg=cmd.cmd_creat(trace_edit_mode,scanname,name);
                    ui->record->append(QString::fromLocal8Bit("插入生成跟踪轨迹指令成功"));
                    cmd_msg=msg;
                    done(1);
                }
            }
            break;
            case TRACE_EDIT_MODE_TOWPOINT_THREE_TO_ONE: //两端点三直线交点模式
            {
                if(b_inster==false)
                {
                    traceedit2->init_dlg_show();
                }
                else
                {
                    traceedit2->init_dlg_show(cmd_list_in);
                }
                traceedit2->setWindowTitle(QString::fromLocal8Bit("生成跟踪轨迹(两端点三直线交点模式)"));
                int rc=traceedit2->exec();
                traceedit2->close_dlg_show();
                if(rc!=0)//确定保存生成轨迹
                {
                    my_cmd cmd;
                    std::vector<QString> scanname(5);

                    scanname[0]=traceedit2->name0;
                    scanname[1]=traceedit2->name1;
                    scanname[2]=traceedit2->name2;
                    scanname[3]=traceedit2->name3;
                    scanname[4]=traceedit2->name4;

                    QString msg=cmd.cmd_creat(trace_edit_mode,scanname,name);
                    ui->record->append(QString::fromLocal8Bit("插入生成跟踪轨迹指令成功"));
                    cmd_msg=msg;
                    done(1);
                }
            }
            break;
        }
    }
    else
    {
        QString msg=QString::fromLocal8Bit("程序有错误，请先排除:");
        ui->record->append(msg);
        for(int i=0;i<err_msg.size();i++)
        {
            ui->record->append(err_msg[i]);
        }
    }
}

