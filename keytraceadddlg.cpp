#include "keytraceadddlg.h"
#include "ui_keytraceadddlg.h"

keytraceaddDlg::keytraceaddDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keytraceaddDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

//美化界面
#ifdef OPEN_BEAUTIFY_INTERFACE
    ui->groupBox->setObjectName(FONT_QUALIFY_MOUDLES_INFO);
    ui->groupBox->setStyleSheet(FONT_GROUPBOX_INFO);
    ui->pushButton->setStyleSheet(FONT_BUTTON_INFO);
    ui->samplespeedlineEdit->setStyleSheet(FONT_EDIT_INFO);
    ui->sampletimelineEdit->setStyleSheet(FONT_EDIT_INFO);
    ui->speedlineEdit->setStyleSheet(FONT_EDIT_INFO);
    ui->tracefilename->setStyleSheet(FONT_EDIT_INFO);
    ui->tracingfilenamecombo_1->setStyleSheet(FONT_COMBOBOX_INFO);
    ui->tracingfilenamecombo_2->setStyleSheet(FONT_COMBOBOX_INFO);
    ui->record->setStyleSheet(FONT_TEXTBROWERS_INFO);
#endif

    b_sample_link=false;
    Updata();
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
            float speed=cmd.cmd_traceadd_speed;
            float samplespeed=cmd.cmd_traceadd_samplespeed;
            int time=cmd.cmd_traceadd_time;

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

            b_sample_link=cmd.cmd_traceadd_samplelink;
            if(b_sample_link==true)
            {
                ui->samplelinkcheckBox->setCheckState(Qt::Checked);
                ui->speedlineEdit->setText(QString::number(speed,'f',ROBOT_SPEED_DECIMAL_PLACE));
                ui->sampletimelineEdit->setText(QString::number(time));
                ui->samplespeedlineEdit->setText(QString::number(samplespeed,'f',ROBOT_SPEED_DECIMAL_PLACE));
            }
            else
            {
                ui->samplelinkcheckBox->setCheckState(Qt::Unchecked);
            }
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
        ui->pushButton->setText(QStringLiteral("插入跟踪轨迹相加指令"));
    }
    else
    {
        ui->pushButton->setText(QStringLiteral("替换跟踪轨迹相加指令"));
    }
}

void keytraceaddDlg::Updata()
{
    if(b_sample_link==false)
    {
        ui->groupBox->setDisabled(true);
    }
    else
    {
        ui->groupBox->setDisabled(false);
    }
}

void keytraceaddDlg::on_pushButton_clicked()
{
    bool rc;
    QString name1=ui->tracingfilenamecombo_1->currentText();
    int route1=ui->tracingfilenamecombo_1->currentIndex();
    QString name2=ui->tracingfilenamecombo_2->currentText();
    int route2=ui->tracingfilenamecombo_2->currentIndex();
    QString nameout=ui->tracefilename->text();
    float samplespeed=0;
    float speed=0;
    int time=0;
    if(route1<0||route1>ui->tracingfilenamecombo_1->count()-1)
    {
        ui->record->append(QStringLiteral("请选择一个跟踪轨迹1工艺"));
        return;
    }
    if(route2<0||route2>ui->tracingfilenamecombo_2->count()-1)
    {
        ui->record->append(QStringLiteral("请选择一个跟踪轨迹2工艺"));
        return;
    }
    if(nameout.isEmpty())
    {
        ui->record->append(QStringLiteral("请填写生成的跟踪轨迹工艺名字"));
        return;
    }
    if(b_sample_link==true)
    {
        samplespeed=ui->samplespeedlineEdit->text().toFloat(&rc);
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
        time=ui->sampletimelineEdit->text().toInt(&rc);
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
        speed=ui->speedlineEdit->text().toFloat(&rc);
        if(ui->speedlineEdit->text().isEmpty())
        {
            ui->record->append(QStringLiteral("请填写焊接速度"));
            return;
        }
        if(rc==false)
        {
            ui->record->append(QStringLiteral("焊接速度格式出错"));
            return;
        }
    }
    my_cmd cmd;
    QString msg;
    msg=cmd.cmd_traceadd(name1,name2,nameout,b_sample_link,speed,samplespeed,time);
    cmd_msg=msg;
    done(1);
}


void keytraceaddDlg::on_samplelinkcheckBox_stateChanged(int arg1)
{
    b_sample_link=arg1;
    Updata();
}

