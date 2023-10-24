#include "keyaoutdlg.h"
#include "ui_keyaoutdlg.h"

keyaoutDlg::keyaoutDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keyaoutDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

//美化界面
#ifdef OPEN_BEAUTIFY_INTERFACE
    ui->A1lineEdit->setStyleSheet(FONT_EDIT_INFO);
    ui->A2lineEdit->setStyleSheet(FONT_EDIT_INFO);
    ui->A3lineEdit->setStyleSheet(FONT_EDIT_INFO);
    ui->A4lineEdit->setStyleSheet(FONT_EDIT_INFO);
    ui->AoutputBtn->setStyleSheet(FONT_BUTTON_INFO);
    ui->record->setStyleSheet(FONT_TEXTBROWERS_INFO);
#endif
}

keyaoutDlg::~keyaoutDlg()
{
    delete ui;
}

void keyaoutDlg::init_dlg_show()
{
    ui->record->clear();
}

void keyaoutDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_AOUT_KEY)//IO指令
        {
            std::vector<float> aout=cmd.cmd_aout_output;
            ui->A1lineEdit->setText(QString::number(aout[0],'f',3));
            ui->A2lineEdit->setText(QString::number(aout[1],'f',3));
            ui->A3lineEdit->setText(QString::number(aout[2],'f',3));
            ui->A4lineEdit->setText(QString::number(aout[3],'f',3));
        }
    }
    ui->record->clear();
}

void keyaoutDlg::close_dlg_show()
{

}

void keyaoutDlg::setbutton(int name)
{
    if(name==0)
    {
        ui->AoutputBtn->setText(QStringLiteral("插入模拟量输出指令"));
    }
    else
    {
        ui->AoutputBtn->setText(QStringLiteral("替换模拟量输出指令"));
    }
}

void keyaoutDlg::on_AoutputBtn_clicked()
{
    my_cmd cmd;
    std::vector<float> aout(ROBOTAOUTPUTNUM);
    QString msg;
    bool rc;
    QString s_A1=ui->A1lineEdit->text();
    QString s_A2=ui->A2lineEdit->text();
    QString s_A3=ui->A3lineEdit->text();
    QString s_A4=ui->A4lineEdit->text();
    if(s_A1.isEmpty())
    {
        ui->record->append(QStringLiteral("请填写A1模拟量"));
        return;
    }
    aout[0]=s_A1.toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("A1模拟量内容格式错误"));
        return;
    }
    aout[1]=s_A2.toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("A2模拟量内容格式错误"));
        return;
    }
    aout[2]=s_A3.toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("A3模拟量内容格式错误"));
        return;
    }
    aout[3]=s_A4.toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("A4模拟量内容格式错误"));
        return;
    }
    msg=cmd.cmd_aout(aout);
    ui->record->append(QStringLiteral("插入模拟量输出指令成功"));
    cmd_msg=msg;
    done(1);
}

