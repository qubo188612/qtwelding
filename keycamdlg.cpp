#include "keycamdlg.h"
#include "ui_keycamdlg.h"

keycamDlg::keycamDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keycamDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

//美化界面
#ifdef OPEN_BEAUTIFY_INTERFACE
    ui->leasercloseBtn->setStyleSheet(FONT_BUTTON_INFO);
    ui->leaseropenBtn->setStyleSheet(FONT_BUTTON_INFO);
    ui->leasertasknum->setStyleSheet(FONT_EDIT_INFO);
    ui->record->setStyleSheet(FONT_TEXTBROWERS_INFO);
#endif
}

keycamDlg::~keycamDlg()
{
    delete ui;
}

void keycamDlg::init_dlg_show()
{
    ui->record->clear();
}

void keycamDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_CAM_KEY)//移动指令
        {
            int task=cmd.cmd_cam_task;
            ui->leasertasknum->setText(QString::number(task));
        }
    }
    ui->record->clear();
}

void keycamDlg::close_dlg_show()
{

}

void keycamDlg::setbutton(int name)
{
    if(name==0)
    {
        ui->leaseropenBtn->setText(QStringLiteral("插入开激光指令"));
        ui->leasercloseBtn->setText(QStringLiteral("插入关激光指令"));
    }
    else
    {
        ui->leaseropenBtn->setText(QStringLiteral("替换开激光指令"));
        ui->leasercloseBtn->setText(QStringLiteral("替换关激光指令"));
    }
}

//插入开激光指令
void keycamDlg::on_leaseropenBtn_clicked()
{
    bool rc;
    int task=ui->leasertasknum->text().toInt(&rc);
    my_cmd cmd;
    QString msg=cmd.cmd_cam(task,1);
    if(ui->leasertasknum->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写任务号"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QStringLiteral("任务号格式出错"));
        return;
    }
    ui->record->append(QStringLiteral("插入开激光指令成功"));
    cmd_msg=msg;
    done(1);
}

//插入关激光指令
void keycamDlg::on_leasercloseBtn_clicked()
{
    my_cmd cmd;
    QString msg=cmd.cmd_cam_work(0);
    ui->record->append(QStringLiteral("插入关激光指令成功"));
    cmd_msg=msg;
    done(1);
}

