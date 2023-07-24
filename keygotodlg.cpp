#include "keygotodlg.h"
#include "ui_keygotodlg.h"

keygotoDlg::keygotoDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keygotoDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
}

keygotoDlg::~keygotoDlg()
{
    delete ui;
}

void keygotoDlg::init_dlg_show()
{
    ui->record->clear();
}

void keygotoDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_GOTO_KEY)//跳转指令
        {
            int line=cmd.cmd_goto_line;
            ui->gotoline->setText(QString::number(line));
        }
    }
    ui->record->clear();
}

void keygotoDlg::close_dlg_show()
{

}

void keygotoDlg::setbutton(int name)
{
    if(name==0)
    {
        ui->pushButton->setText(QString::fromLocal8Bit("插入跳转指令"));
    }
    else
    {
        ui->pushButton->setText(QString::fromLocal8Bit("替换跳转指令"));
    }
}

void keygotoDlg::on_pushButton_clicked()
{
    bool rc;
    int line=ui->gotoline->text().toInt(&rc);
    my_cmd cmd;
    QString msg=cmd.cmd_goto(line);
    if(ui->gotoline->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写跳转行数"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("跳转行数格式出错"));
        return;
    }
    ui->record->append(QString::fromLocal8Bit("插入跳转指令成功"));
    cmd_msg=msg;
    done(1);
}

