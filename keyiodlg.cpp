#include "keyiodlg.h"
#include "ui_keyiodlg.h"

keyioDlg::keyioDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keyioDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸
}

keyioDlg::~keyioDlg()
{
    delete ui;
}

void keyioDlg::init_dlg_show()
{
    ui->record->clear();
}

void keyioDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_IO_KEY)//IO指令
        {
            std::vector<int> io;
            switch(cmd.cmd_io_workmod)
            {
                case OUTIO://输出IO
                {
                    io=cmd.cmd_io_output;
                }
                break;
                case WAITINIO://等待IO输入
                {
                    io=cmd.cmd_io_input;
                }
                break;
            }
            if(io[0]!=0)
                ui->checkIO_1->setChecked(true);
            else
                ui->checkIO_1->setChecked(false);
            if(io[1]!=0)
                ui->checkIO_2->setChecked(true);
            else
                ui->checkIO_2->setChecked(false);
            if(io[2]!=0)
                ui->checkIO_3->setChecked(true);
            else
                ui->checkIO_3->setChecked(false);
            if(io[3]!=0)
                ui->checkIO_4->setChecked(true);
            else
                ui->checkIO_4->setChecked(false);
            if(io[4]!=0)
                ui->checkIO_5->setChecked(true);
            else
                ui->checkIO_5->setChecked(false);
            if(io[5]!=0)
                ui->checkIO_6->setChecked(true);
            else
                ui->checkIO_6->setChecked(false);
            if(io[6]!=0)
                ui->checkIO_7->setChecked(true);
            else
                ui->checkIO_7->setChecked(false);
            if(io[7]!=0)
                ui->checkIO_8->setChecked(true);
            else
                ui->checkIO_8->setChecked(false);
        }
    }
    ui->record->clear();
}

void keyioDlg::close_dlg_show()
{

}

void keyioDlg::setbutton(int name)
{
    if(name==0)
    {
        ui->IOoutputBtn->setText(QStringLiteral("插入IO口输出指令"));
        ui->IOinputBtn->setText(QStringLiteral("插入等待IO口输入指令"));
    }
    else
    {
        ui->IOoutputBtn->setText(QStringLiteral("替换IO口输出指令"));
        ui->IOinputBtn->setText(QStringLiteral("替换等待IO口输入指令"));
    }
}

//IO输出指令
void keyioDlg::on_IOoutputBtn_clicked()
{
    my_cmd cmd;
    std::vector<int> io(ROBOTOUTPUTNUM);
    QString msg;
    if(ui->checkIO_1->isChecked())
    {
        io[0]=1;
    }
    else
    {
        io[0]=0;
    }
    if(ui->checkIO_2->isChecked())
    {
        io[1]=1;
    }
    else
    {
        io[1]=0;
    }
    if(ui->checkIO_3->isChecked())
    {
        io[2]=1;
    }
    else
    {
        io[2]=0;
    }
    if(ui->checkIO_4->isChecked())
    {
        io[3]=1;
    }
    else
    {
        io[3]=0;
    }
    if(ui->checkIO_5->isChecked())
    {
        io[4]=1;
    }
    else
    {
        io[4]=0;
    }
    if(ui->checkIO_6->isChecked())
    {
        io[5]=1;
    }
    else
    {
        io[5]=0;
    }
    if(ui->checkIO_7->isChecked())
    {
        io[6]=1;
    }
    else
    {
        io[6]=0;
    }
    if(ui->checkIO_8->isChecked())
    {
        io[7]=1;
    }
    else
    {
        io[7]=0;
    }
    msg=cmd.cmd_ioout(io);
    ui->record->append(QStringLiteral("插入IO口输出指令成功"));
    cmd_msg=msg;
    done(1);
}

//等待IO输入指令
void keyioDlg::on_IOinputBtn_clicked()
{
    my_cmd cmd;
    std::vector<int> io(ROBOTINPUTNUM);
    QString msg;
    if(ui->checkIO_1->isChecked())
    {
        io[0]=1;
    }
    else
    {
        io[0]=0;
    }
    if(ui->checkIO_2->isChecked())
    {
        io[1]=1;
    }
    else
    {
        io[1]=0;
    }
    if(ui->checkIO_3->isChecked())
    {
        io[2]=1;
    }
    else
    {
        io[2]=0;
    }
    if(ui->checkIO_4->isChecked())
    {
        io[3]=1;
    }
    else
    {
        io[3]=0;
    }
    if(ui->checkIO_5->isChecked())
    {
        io[4]=1;
    }
    else
    {
        io[4]=0;
    }
    if(ui->checkIO_6->isChecked())
    {
        io[5]=1;
    }
    else
    {
        io[5]=0;
    }
    if(ui->checkIO_7->isChecked())
    {
        io[6]=1;
    }
    else
    {
        io[6]=0;
    }
    if(ui->checkIO_8->isChecked())
    {
        io[7]=1;
    }
    else
    {
        io[7]=0;
    }
    msg=cmd.cmd_iowaitin(io);
    ui->record->append(QStringLiteral("插入等待IO口输入指令成功"));
    cmd_msg=msg;
    done(1);
}

