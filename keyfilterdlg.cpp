#include "keyfilterdlg.h"
#include "ui_keyfilterdlg.h"

keyfilterDlg::keyfilterDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keyfilterDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;

    for(int n=0;n<FILTER_ID_TOTAL_NUM;n++)
    {
        QString msg=Filter_mode_toQString((Filter_mode)n);
        ui->tabWidget->setTabText(n,msg);
    }

    filterParam filters;
    ui->msl_poly->setText(QString::number(filters.msl_poly));
    ui->sor_nearpoint_num->setText(QString::number(filters.sor_nearpoint_num));
    ui->sor_standard_deviation->setText(QString::number(filters.sor_standard_deviation,'f',3));
}

keyfilterDlg::~keyfilterDlg()
{
    delete ui;
}

void keyfilterDlg::init_dlg_show()
{
    ui->filternamecombo->clear();
    for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
    {
        ui->filternamecombo->addItem(m_mcs->project->project_weld_trace[n].name);
    }
    ui->record->clear();
}

void keyfilterDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    ui->filternamecombo->clear();
    for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
    {
        ui->filternamecombo->addItem(m_mcs->project->project_weld_trace[n].name);
    }
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_FILTER_KEY)
        {
            QString namein=cmd.cmd_filter_namein;//获取到的要滤波的轨迹名称
            QString nameout=cmd.cmd_filter_nameout;//获取到的滤波结果名称
            Filter_mode mode=cmd.cmd_filter_mode;
            filterParam filters=cmd.cmd_filters;//滤波参数
            int weld_trace_creatnum;

            //这里添加移动命令
            for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
            {
                if(namein==m_mcs->project->project_weld_trace[n].name)
                {
                    weld_trace_creatnum=n;//找到要储存的焊接轨道下标
                    break;
                }
            }
            if(weld_trace_creatnum>=0&&weld_trace_creatnum<ui->filternamecombo->count())
            {
                ui->filternamecombo->setCurrentIndex(weld_trace_creatnum);
            }
            if(mode>=0&&mode<ui->tabWidget->count())
            {
                ui->tabWidget->setCurrentIndex(mode);
            }
            ui->msl_poly->setText(QString::number(filters.msl_poly));
            ui->sor_nearpoint_num->setText(QString::number(filters.sor_nearpoint_num));
            ui->sor_standard_deviation->setText(QString::number(filters.sor_standard_deviation,'f',3));

            ui->filternamelineEdit->setText(nameout);

        }
    }
    ui->record->clear();
}

void keyfilterDlg::close_dlg_show()
{

}

void keyfilterDlg::setbutton(int name)
{
    if(name==0)
    {
        b_inster=false;
        ui->pushButton->setText(QString::fromLocal8Bit("插入轨迹滤波指令"));
    }
    else
    {
        b_inster=true;
        ui->pushButton->setText(QString::fromLocal8Bit("替换轨迹滤波指令"));
    }
}

void keyfilterDlg::on_pushButton_clicked()
{
    bool rc;
    int route=ui->filternamecombo->currentIndex();
    QString creatname=ui->filternamecombo->currentText();
    int mode=ui->tabWidget->currentIndex();
    QString name=ui->filternamelineEdit->text();
    filterParam filters;
    float f_data;
    int i_data;

    if(route<0||route>ui->filternamecombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择要滤波的轨迹名字"));
        return;
    }
    if(mode<0||mode>ui->tabWidget->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择滤波模式"));
        return;
    }
    if(name.isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写生成轨迹"));
        return;
    }
    switch(mode)
    {
        case FILTER_MLS:
        {
            if(ui->msl_poly->text().isEmpty())
            {
                ui->record->append(QString::fromLocal8Bit("请填写拟合阶次"));
                return;
            }
            i_data=ui->msl_poly->text().toInt(&rc);
            if(rc==false)
            {
                ui->record->append(QString::fromLocal8Bit("拟合阶次格式错误"));
                return;
            }
            filters.msl_poly=i_data;
        }
        break;
        case FILTER_SOR:
        {
            if(ui->sor_nearpoint_num->text().isEmpty())
            {
                ui->record->append(QString::fromLocal8Bit("请填写邻域点数"));
                return;
            }
            i_data=ui->sor_nearpoint_num->text().toInt(&rc);
            if(rc==false)
            {
                ui->record->append(QString::fromLocal8Bit("邻域点数格式错误"));
                return;
            }
            filters.sor_nearpoint_num=i_data;

            if(ui->sor_standard_deviation->text().isEmpty())
            {
                ui->record->append(QString::fromLocal8Bit("请填写标准差"));
                return;
            }
            f_data=ui->sor_standard_deviation->text().toFloat(&rc);
            if(rc==false)
            {
                ui->record->append(QString::fromLocal8Bit("标准差格式错误"));
                return;
            }
            filters.sor_standard_deviation=f_data;

        }
        break;
    }
    my_cmd cmd;
    QString msg=cmd.cmd_filter(creatname,(Filter_mode)mode,filters,name);
    if(b_inster==false)//插入
    {
        std::vector<QString> err_msg;
        m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg);
        for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
        {
            if(name==m_mcs->project->project_weld_trace[n].name)
            {
                ui->record->append(QString::fromLocal8Bit("生成轨迹与已有的轨迹重名"));
                return;
            }
        }
    }
    ui->record->append(QString::fromLocal8Bit("插入轨迹滤波指令成功"));
    cmd_msg=msg;
    done(1);
}




