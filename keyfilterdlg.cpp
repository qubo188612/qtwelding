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
        ui->filtermodecombo->addItem(msg);
    }
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
            if(mode>=0&&mode<ui->filtermodecombo->count())
            {
                ui->filtermodecombo->setCurrentIndex(mode);
            }
            ui->filters1lineEdit->setText(QString::number(filters.distance,'f',ROBOT_POSE_DECIMAL_PLACE));
            ui->filters2lineEdit->setText(QString::number(filters.mutation_limit,'f',ROBOT_POSE_DECIMAL_PLACE));
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
        ui->pushButton->setText(QString::fromLocal8Bit("插入轨迹滤波指令"));
    }
    else
    {
        ui->pushButton->setText(QString::fromLocal8Bit("替换轨迹滤波指令"));
    }
}

void keyfilterDlg::on_pushButton_clicked()
{
    bool rc;
    int route=ui->filternamecombo->currentIndex();
    QString creatname=ui->filternamecombo->currentText();
    int mode=ui->filtermodecombo->currentIndex();
    QString name=ui->filternamelineEdit->text();
    filterParam filters;
    float f_data;

    if(route<0||route>ui->filternamecombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择要滤波的轨迹名字"));
        return;
    }
    if(mode<0||mode>ui->filtermodecombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择滤波模式"));
        return;
    }
    if(name.isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写生成轨迹"));
        return;
    }
    if(ui->filters1lineEdit->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写滤波距离"));
        return;
    }
    f_data=ui->filters1lineEdit->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("滤波距离格式错误"));
        return;
    }
    filters.distance=f_data;

    if(ui->filters2lineEdit->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写突变限制"));
        return;
    }
    f_data=ui->filters2lineEdit->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("突变限制格式错误"));
        return;
    }
    filters.mutation_limit=f_data;

    my_cmd cmd;
    QString msg=cmd.cmd_filter(creatname,(Filter_mode)mode,filters,name);
    ui->record->append(QString::fromLocal8Bit("插入轨迹滤波指令成功"));
    cmd_msg=msg;
    done(1);
}

