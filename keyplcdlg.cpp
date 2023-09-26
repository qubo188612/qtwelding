#include "keyplcdlg.h"
#include "ui_keyplcdlg.h"

keyplcDlg::keyplcDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keyplcDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸
}

keyplcDlg::~keyplcDlg()
{
    delete ui;
}

void keyplcDlg::init_dlg_show()
{
    ui->record->clear();
}

void keyplcDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_PLC_KEY)//PLC指令
        {
            int add=cmd.cmd_plc_register_add;
            int data=cmd.cmd_plc_register_data;
            ui->plc_add->setText(QString::number(add));
            ui->plc_data->setText(QString::number(data));
        }
    }
    ui->record->clear();
}

void keyplcDlg::close_dlg_show()
{

}

void keyplcDlg::setbutton(int name)
{
    if(name==0)
    {
        ui->PLCoutputBtn->setText(QStringLiteral("插入PLC输出指令"));
        ui->PLCinputBtn->setText(QStringLiteral("插入等待PLC输入指令"));
    }
    else
    {
        ui->PLCoutputBtn->setText(QStringLiteral("替换PLC输出指令"));
        ui->PLCinputBtn->setText(QStringLiteral("替换等待PLC输入指令"));
    }
}

//PLC输出
void keyplcDlg::on_PLCoutputBtn_clicked()
{
    my_cmd cmd;
    QString msg;
    bool b_add,b_data;
    int add=ui->plc_add->text().toInt(&b_add);
    int data=ui->plc_data->text().toInt(&b_data);
    if(ui->plc_add->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写寄存器地址"));
        return;
    }
    else
    {
        if(b_add==false)
        {
            ui->record->append(QStringLiteral("寄存器地址格式错误"));
            return;
        }
    }
    if(ui->plc_data->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写寄存器内容"));
        return;
    }
    else
    {
        if(b_data==false)
        {
            ui->record->append(QStringLiteral("寄存器内容格式错误"));
            return;
        }
        else
        {
            if(data<-32768||data>32767)
            {
                ui->record->append(QStringLiteral("寄存器内容取值是-32768到32767之间"));
                return;
            }
        }
    }
    msg=cmd.cmd_plcwrite(add,data);
    ui->record->append(QStringLiteral("插入PLC输出指令成功"));
    cmd_msg=msg;
    done(1);
}

//等待PLC输入
void keyplcDlg::on_PLCinputBtn_clicked()
{
    my_cmd cmd;
    QString msg;
    bool b_add,b_data;
    int add=ui->plc_add->text().toInt(&b_add);
    int data=ui->plc_data->text().toInt(&b_data);
    if(ui->plc_add->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写寄存器地址"));
        return;
    }
    else
    {
        if(b_add==false)
        {
            ui->record->append(QStringLiteral("寄存器地址格式错误"));
            return;
        }
    }
    if(ui->plc_data->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写寄存器内容"));
        return;
    }
    else
    {
        if(b_data==false)
        {
            ui->record->append(QStringLiteral("寄存器内容格式错误"));
            return;
        }
        else
        {
            if(data<-32768||data>32767)
            {
                ui->record->append(QStringLiteral("寄存器内容取值是-32768到32767之间"));
                return;
            }
        }
    }
    msg=cmd.cmd_plcwait(add,data);
    ui->record->append(QStringLiteral("插入等待PLC输入指令成功"));
    cmd_msg=msg;
    done(1);
}

