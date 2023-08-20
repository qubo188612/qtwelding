#include "keygettcpposdlg.h"
#include "ui_keygettcpposdlg.h"

keygettcpposDlg::keygettcpposDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keygettcpposDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸
}

keygettcpposDlg::~keygettcpposDlg()
{
    delete ui;
}

void keygettcpposDlg::init_dlg_show()
{
    ui->record->clear();
}

void keygettcpposDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_GETPOS_KEY)//获取坐标点指令
        {
            QString name=cmd.cmd_gettcppos_name;
            std::vector<float> add=cmd.cmd_gettcppos_add;
            ui->gettcpposname->setText(name);
            ui->addXEdit->setText(QString::number(add[0],'f',ROBOT_POSE_DECIMAL_PLACE));
            ui->addYEdit->setText(QString::number(add[1],'f',ROBOT_POSE_DECIMAL_PLACE));
            ui->addZEdit->setText(QString::number(add[2],'f',ROBOT_POSE_DECIMAL_PLACE));
        }
    }
    ui->record->clear();
}

void keygettcpposDlg::close_dlg_show()
{

}

void keygettcpposDlg::setbutton(int name)
{
    if(name==0)
    {
        b_inster=false;
        ui->gettcpposBtn->setText(QString::fromLocal8Bit("插入获取坐标点指令"));
    }
    else
    {
        b_inster=true;
        ui->gettcpposBtn->setText(QString::fromLocal8Bit("替换获取坐标点指令"));
    }
}

void keygettcpposDlg::on_gettcpposBtn_clicked()
{
    my_cmd cmd;
    QString msg;
    QString name=ui->gettcpposname->text();
    std::vector<float> add(3);
    bool rc;
    if(ui->gettcpposname->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写坐标点名称"));
        return;
    }
    if(ui->addXEdit->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写补偿X"));
        return;
    }
    add[0]=ui->addXEdit->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("补偿X格式出错"));
        return;
    }
    if(ui->addYEdit->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写补偿Y"));
        return;
    }
    add[1]=ui->addYEdit->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("补偿Y格式出错"));
        return;
    }
    if(ui->addZEdit->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写补偿Z"));
        return;
    }
    add[2]=ui->addZEdit->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("补偿Z格式出错"));
        return;
    }
    if(b_inster==false)
    {
        for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
        {
            if(name==m_mcs->project->project_robpos_trace[n].name)
            {
                ui->record->append(QString::fromLocal8Bit("生成的点坐标与已有的点坐标重名"));
                return;
            }
        }
    }
    msg=cmd.cmd_gettcppos(name,add);
    ui->record->append(QString::fromLocal8Bit("插入获取坐标点指令成功"));
    cmd_msg=msg;
    done(1);
}

