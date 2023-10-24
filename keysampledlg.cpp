#include "keysampledlg.h"
#include "ui_keysampledlg.h"

keysampleDlg::keysampleDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keysampleDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

//美化界面
#ifdef OPEN_BEAUTIFY_INTERFACE
    ui->sampleBtn->setStyleSheet(FONT_BUTTON_INFO);
    ui->samplecreatnamecombo->setStyleSheet(FONT_COMBOBOX_INFO);
    ui->samplenamelineEdit->setStyleSheet(FONT_EDIT_INFO);
    ui->samplespeedlineEdit->setStyleSheet(FONT_EDIT_INFO);
    ui->sampletimelineEdit->setStyleSheet(FONT_EDIT_INFO);
    ui->record->setStyleSheet(FONT_TEXTBROWERS_INFO);
#endif

    adoubleValidator_speed = new QDoubleValidator(ROBOT_SPEED_DECIMAL_BOTTOM,ROBOT_SPEED_DECIMAL_TOP,ROBOT_SPEED_DECIMAL_PLACE,this);//限制3位小数
    ui->samplespeedlineEdit->setValidator(adoubleValidator_speed);
}

keysampleDlg::~keysampleDlg()
{
    delete ui;
}

void keysampleDlg::init_dlg_show()
{
    ui->samplecreatnamecombo->clear();
    for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
    {
        ui->samplecreatnamecombo->addItem(m_mcs->project->project_weld_trace[n].name);
    }
    ui->record->clear();
}

void keysampleDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    ui->samplecreatnamecombo->clear();
    for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
    {
        ui->samplecreatnamecombo->addItem(m_mcs->project->project_weld_trace[n].name);
    }
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_SAMPLE_KEY)//采样指令
        {
            QString creatname=cmd.cmd_sample_creatname;//获取到的要采样的轨迹名称
            QString name=cmd.cmd_sample_name;//获取到的采样结果点名称
            float speed=cmd.cmd_sample_speed;//获取到的采样时轨迹的速度
            int time=cmd.cmd_sample_time;//获取到的采样点之间的时间单位ms
            int weld_trace_creatnum;//搜索到的采样的轨迹名称
            //这里添加移动命令
            for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
            {
                if(creatname==m_mcs->project->project_weld_trace[n].name)
                {
                    weld_trace_creatnum=n;//找到要储存的焊接轨道下标
                    break;
                }
            }
            if(weld_trace_creatnum>=0&&weld_trace_creatnum<ui->samplecreatnamecombo->count())
            {
                ui->samplecreatnamecombo->setCurrentIndex(weld_trace_creatnum);
            }
            ui->samplenamelineEdit->setText(name);
            ui->samplespeedlineEdit->setText(QString::number(speed,'f',ROBOT_SPEED_DECIMAL_PLACE));
            ui->sampletimelineEdit->setText(QString::number(time));
        }
    }
    ui->record->clear();
}

void keysampleDlg::close_dlg_show()
{

}

void keysampleDlg::setbutton(int name)
{
    if(name==0)
    {
        b_inster=false;
        ui->sampleBtn->setText(QStringLiteral("插入采样指令"));
    }
    else
    {
        b_inster=true;
        ui->sampleBtn->setText(QStringLiteral("替换采样指令"));
    }
}

//采样指令
void keysampleDlg::on_sampleBtn_clicked()
{
    bool rc;
    int route=ui->samplecreatnamecombo->currentIndex();
    QString creatname=ui->samplecreatnamecombo->currentText();
    float speed=ui->samplespeedlineEdit->text().toFloat(&rc);
    QString name=ui->samplenamelineEdit->text();
    if(route<0||route>ui->samplecreatnamecombo->count()-1)
    {
        ui->record->append(QStringLiteral("请选择要采样的轨迹名字"));
        return;
    }
    if(ui->samplespeedlineEdit->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写采样速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QStringLiteral("采样速度格式出错"));
        return;
    }
    int time=ui->sampletimelineEdit->text().toInt(&rc);
    if(ui->sampletimelineEdit->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写采样间隔"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QStringLiteral("采样间隔格式出错"));
        return;
    }
    if(ui->samplenamelineEdit->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写生成轨迹"));
        return;
    }
    my_cmd cmd;
    QString msg=cmd.cmd_sample(creatname,speed,time,name);
    if(b_inster==false)//插入
    {
        std::vector<QString> err_msg;
        m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg);
        for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
        {
            if(name==m_mcs->project->project_weld_trace[n].name)
            {
                ui->record->append(QStringLiteral("生成轨迹与已有的轨迹重名"));
                return;
            }
        }
    }
    ui->record->append(QStringLiteral("插入采样指令成功"));
    cmd_msg=msg;
    done(1);
}

