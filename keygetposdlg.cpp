#include "keygetposdlg.h"
#include "ui_keygetposdlg.h"

keygetposDlg::keygetposDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keygetposDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

//美化界面
#ifdef OPEN_BEAUTIFY_INTERFACE
    ui->addXEdit->setStyleSheet(FONT_EDIT_INFO);
    ui->addYEdit->setStyleSheet(FONT_EDIT_INFO);
    ui->addZEdit->setStyleSheet(FONT_EDIT_INFO);
    ui->getposBtn->setStyleSheet(FONT_BUTTON_INFO);
    ui->getposname->setStyleSheet(FONT_EDIT_INFO);
    ui->getpostime->setStyleSheet(FONT_EDIT_INFO);
    ui->record->setStyleSheet(FONT_TEXTBROWERS_INFO);
#endif
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
            std::vector<float> add=cmd.cmd_getpos_add;
            ui->getposname->setText(name);
            ui->getpostime->setText(QString::number(time));
            ui->addXEdit->setText(QString::number(add[0],'f',ROBOT_POSE_DECIMAL_PLACE));
            ui->addYEdit->setText(QString::number(add[1],'f',ROBOT_POSE_DECIMAL_PLACE));
            ui->addZEdit->setText(QString::number(add[2],'f',ROBOT_POSE_DECIMAL_PLACE));
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
        b_inster=false;
        ui->getposBtn->setText(QStringLiteral("插入获取坐标点指令"));
    }
    else
    {
        b_inster=true;
        ui->getposBtn->setText(QStringLiteral("替换获取坐标点指令"));
    }
}

//获取坐标点指令
void keygetposDlg::on_getposBtn_clicked()
{
    my_cmd cmd;
    QString msg;
    QString name=ui->getposname->text();
    std::vector<float> add(3);
    int time;
    bool rc;
    if(ui->getpostime->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写获取时间"));
        return;
    }
    time=ui->getpostime->text().toInt(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("获取时间格式出错"));
        return;
    }
    if(ui->getposname->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写坐标点名称"));
        return;
    }
    if(ui->addXEdit->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写补偿X"));
        return;
    }
    add[0]=ui->addXEdit->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("补偿X格式出错"));
        return;
    }
    if(ui->addYEdit->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写补偿Y"));
        return;
    }
    add[1]=ui->addYEdit->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("补偿Y格式出错"));
        return;
    }
    if(ui->addZEdit->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写补偿Z"));
        return;
    }
    add[2]=ui->addZEdit->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("补偿Z格式出错"));
        return;
    }
    if(b_inster==false)
    {
        for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
        {
            if(name==m_mcs->project->project_robpos_trace[n].name)
            {
                ui->record->append(QStringLiteral("生成的点坐标与已有的点坐标重名"));
                return;
            }
        }
    }
    msg=cmd.cmd_getpos(time,name,add);
    ui->record->append(QStringLiteral("插入获取坐标点指令成功"));
    cmd_msg=msg;
    done(1);
}

