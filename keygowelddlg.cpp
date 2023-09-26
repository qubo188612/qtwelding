#include "keygowelddlg.h"
#include "ui_keygowelddlg.h"

keygoweldDlg::keygoweldDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keygoweldDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

    for(int n=0;n<ROBOTTCPNUM;n++)
    {
        QString msg="TCP: "+QString::number(n);
        ui->goweldtcpcombo->addItem(msg);
    }

    adoubleValidator_speed = new QDoubleValidator(ROBOT_SPEED_DECIMAL_BOTTOM,ROBOT_SPEED_DECIMAL_TOP,ROBOT_SPEED_DECIMAL_PLACE,this);//限制3位小数
    ui->goweldspeedlineEdit->setValidator(adoubleValidator_speed);
}

keygoweldDlg::~keygoweldDlg()
{
    delete ui;
}

void keygoweldDlg::init_dlg_show()
{
    ui->goweldfilenamecombo->clear();
    for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
    {
        ui->goweldfilenamecombo->addItem(m_mcs->project->project_interweld_trace[n].name);
    }
    ui->record->clear();
}

void keygoweldDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    ui->goweldfilenamecombo->clear();
    for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
    {
        ui->goweldfilenamecombo->addItem(m_mcs->project->project_interweld_trace[n].name);
    }
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_GOWELD_KEY)//前往起弧点指令
        {
            int tcp=cmd.cmd_goweld_tcp;//获取到前往起弧点TCP
            QString name=cmd.cmd_goweld_name;
            float speed=cmd.cmd_goweld_speed;
            int weld_goweld_num;
            if(tcp>=0&&tcp<ROBOTTCPNUM)
            {
                ui->goweldtcpcombo->setCurrentIndex(tcp);
            }
            for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
            {
                if(name==m_mcs->project->project_interweld_trace[n].name)
                {
                    weld_goweld_num=n;//找到要储存的焊接轨道下标
                    break;
                }
            }
            if(weld_goweld_num>=0&&weld_goweld_num<ui->goweldfilenamecombo->count())
            {
                ui->goweldfilenamecombo->setCurrentIndex(weld_goweld_num);
            }
            ui->goweldspeedlineEdit->setText(QString::number(speed,'f',ROBOT_SPEED_DECIMAL_PLACE));
        }
    }
    ui->record->clear();
}

void keygoweldDlg::close_dlg_show()
{

}

void keygoweldDlg::setbutton(int name)
{
    if(name==0)
    {
        ui->pushButton->setText(QStringLiteral("插入前往起弧点指令"));
    }
    else
    {
        ui->pushButton->setText(QStringLiteral("替换前往起弧点指令"));
    }
}

void keygoweldDlg::on_pushButton_clicked()
{
    int tcp=ui->goweldtcpcombo->currentIndex();
    QString name=ui->goweldfilenamecombo->currentText();
    int route=ui->goweldfilenamecombo->currentIndex();
    float speed=ui->goweldspeedlineEdit->text().toFloat();
    if(tcp<0||tcp>ui->goweldtcpcombo->count()-1)
    {
        ui->record->append(QStringLiteral("请选择一个坐标系"));
        return;
    }
    if(route<0||route>ui->goweldfilenamecombo->count()-1)
    {
        ui->record->append(QStringLiteral("请选择一个跟踪轨迹工艺"));
        return;
    }
    if(ui->goweldspeedlineEdit->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写速度"));
        return;
    }
    my_cmd cmd;
    QString msg;
    msg=cmd.cmd_goweld(tcp,speed,name);
    cmd_msg=msg;
    done(1);
}

