#include "keygetposdlg.h"
#include "ui_keygetposdlg.h"

keygetposDlg::keygetposDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keygetposDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
}

keygetposDlg::~keygetposDlg()
{
    delete ui;
}

void keygetposDlg::init_dlg_show()
{
    ui->record->clear();
}

void keygetposDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_GETPOS_KEY)//获取坐标点指令
        {
            QString name=cmd.cmd_getpos_name;
            int time=cmd.cmd_getpos_time;
            ui->getposname->setText(name);
            ui->getpostime->setText(QString::number(time));
        }
    }
    ui->record->clear();
}

void keygetposDlg::close_dlg_show()
{

}

void keygetposDlg::setbutton(int name)
{
    if(name==0)
    {
        ui->getposBtn->setText(QString::fromLocal8Bit("插入获取坐标点指令"));
    }
    else
    {
        ui->getposBtn->setText(QString::fromLocal8Bit("替换获取坐标点指令"));
    }
}

//获取坐标点指令
void keygetposDlg::on_getposBtn_clicked()
{
    my_cmd cmd;
    QString msg;
    QString name=ui->getposname->text();
    int time;
    bool rc;
    if(ui->getpostime->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写获取时间"));
        return;
    }
    time=ui->getpostime->text().toInt(&rc);
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("获取时间格式出错"));
        return;
    }
    if(ui->getposname->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写扫描点名称"));
        return;
    }
    msg=cmd.cmd_getpos(time,name);
    ui->record->append(QString::fromLocal8Bit("插入获取坐标点指令成功"));
    cmd_msg=msg;
    done(1);
}

