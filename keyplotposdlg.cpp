#include "keyplotposdlg.h"
#include "ui_keyplotposdlg.h"

keyplotposDlg::keyplotposDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keyplotposDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

    plotedit0=new plotedit0Dlg(mcs);
    plotedit1=new plotedit1Dlg(mcs);

    for(int n=0;n<PLOTPOS_EDIT_ID_TOTAL_NUM;n++)
    {
        QString msg=Plotpos_edit_mode_toQString((Plotpos_edit_mode)n);
        ui->plotposmodecombo->addItem(msg);
    }
}

keyplotposDlg::~keyplotposDlg()
{
    delete plotedit0;
    delete plotedit1;
    delete ui;
}

void keyplotposDlg::init_dlg_show()
{
    cmd_list_in.clear();
    ui->record->clear();
}

void keyplotposDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    cmd_list_in=cmdlist;
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_PLOTPOS_KEY)//计算点坐标指令
        {
            //这里添加点坐标
            QString name=cmd.cmd_plotpos_name;//获取到的点坐标名字
            Plotpos_edit_mode mode=cmd.cmd_plotpos_mode;//获取到的点坐标生成模式
            ui->plotposname->setText(name);
            if(mode>=0&&mode<=ui->plotposmodecombo->count())
            {
                ui->plotposmodecombo->setCurrentIndex(mode);
            }
        }
    }
    ui->record->clear();
}

void keyplotposDlg::close_dlg_show()
{

}

void keyplotposDlg::setbutton(int name)
{
    if(name==0)
    {
        b_inster=false;
        ui->plotposBtn->setText(QString::fromLocal8Bit("插入计算点坐标指令"));
    }
    else
    {
        b_inster=true;
        ui->plotposBtn->setText(QString::fromLocal8Bit("替换计算点坐标指令"));
    }
}

void keyplotposDlg::set_now_cmdline(int num)
{
    now_cmdline=num;
}

void keyplotposDlg::on_plotposBtn_clicked()
{
    int rc;
    std::vector<QString> err_msg;
    rc=m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg);
    if(0==rc||b_inster==true)
    {
        std::vector<QString> err_msg;
        QString name=ui->plotposname->text();
        if(name.size()==0)
        {
            ui->record->append(QString::fromLocal8Bit("请输入要插入的点坐标名字"));
            return;
        }
        if(b_inster==false)
        {
            for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
            {
                if(name==m_mcs->project->projecr_robpos_trace[n].name)
                {
                    ui->record->append(QString::fromLocal8Bit("要生成的点坐标与已有的点坐标重名"));
                    return;
                }
            }
        }
        m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(now_cmdline+1,err_msg);

        Plotpos_edit_mode plotpos_edit_mode=(Plotpos_edit_mode)ui->plotposmodecombo->currentIndex();
        switch(plotpos_edit_mode)
        {
            case PLOTPOS_EDIT_MODE_THREE_TO_ONE:  //三直线交点模式
            {
                if(m_mcs->project->project_weld_trace.size()<=0)
                {
                    ui->record->append(QString::fromLocal8Bit("当前指令位置没有可用的跟踪轨迹"));
                    return;
                }
                if(b_inster==false)
                {
                    plotedit0->init_dlg_show();
                }
                else
                {
                    plotedit0->init_dlg_show(cmd_list_in);
                }
                plotedit0->setWindowTitle(QString::fromLocal8Bit("计算点坐标(三直线交点模式)"));
                int rc=plotedit0->exec();
                plotedit0->close_dlg_show();
                if(rc!=0)//确定保存生成轨迹
                {
                    my_cmd cmd;
                    std::vector<QString> scanname(3);

                    scanname[0]=plotedit0->name0;
                    scanname[1]=plotedit0->name1;
                    scanname[2]=plotedit0->name2;
                    QString msg=cmd.cmd_plotpos(plotpos_edit_mode,scanname,name);
                    ui->record->append(QString::fromLocal8Bit("插入计算点坐标指令成功"));
                    cmd_msg=msg;
                    done(1);
                }
            }
            break;
            case PLOTPOS_EDIT_MODE_FIVEPOINTS_TO_ONE:   //两点直线与三点交点模式
            {
                if(m_mcs->project->projecr_robpos_trace.size()<=0)
                {
                    ui->record->append(QString::fromLocal8Bit("当前指令位置没有可用的跟踪轨迹"));
                    return;
                }
                if(b_inster==false)
                {
                    plotedit1->init_dlg_show();
                }
                else
                {
                    plotedit1->init_dlg_show(cmd_list_in);
                }
                plotedit1->setWindowTitle(QString::fromLocal8Bit("计算点坐标(两点直线与三点交点模式)"));
                int rc=plotedit1->exec();
                plotedit1->close_dlg_show();
                if(rc!=0)//确定保存生成轨迹
                {
                    my_cmd cmd;
                    std::vector<QString> pointsname(5);

                    pointsname[0]=plotedit1->name0;
                    pointsname[1]=plotedit1->name1;
                    pointsname[2]=plotedit1->name2;
                    pointsname[3]=plotedit1->name3;
                    pointsname[4]=plotedit1->name4;
                    QString msg=cmd.cmd_plotpos(plotpos_edit_mode,pointsname,name);
                    ui->record->append(QString::fromLocal8Bit("插入计算点坐标指令成功"));
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

