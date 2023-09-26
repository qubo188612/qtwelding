#include "keydelaydlg.h"
#include "ui_keydelaydlg.h"

keydelayDlg::keydelayDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keydelayDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸
}

keydelayDlg::~keydelayDlg()
{
    delete ui;
}

void keydelayDlg::init_dlg_show()
{
    ui->record->clear();
}

void keydelayDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_DELAY_KEY)//移动指令
        {
            int time=cmd.cmd_delay_time;//获取到延时时间转ms
            ui->delaytime->setText(QString::number(time));
        }
    }
    ui->record->clear();
}

void keydelayDlg::close_dlg_show()
{

}

void keydelayDlg::setbutton(int name)
{
    if(name==0)
    {
        ui->delaytimeBtn->setText(QStringLiteral("插入延时指令"));
    }
    else
    {
        ui->delaytimeBtn->setText(QStringLiteral("替换延时指令"));
    }
}

//插入延时指令
void keydelayDlg::on_delaytimeBtn_clicked()
{
    bool rc;
    int time=ui->delaytime->text().toInt(&rc);
    my_cmd cmd;
    QString msg=cmd.cmd_delay(time);
    if(ui->delaytime->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写延时时间"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QStringLiteral("延时时间格式出错"));
        return;
    }
    ui->record->append(QStringLiteral("插入延时指令成功"));
    cmd_msg=msg;
    done(1);
}

