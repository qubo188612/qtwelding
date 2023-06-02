#include "keysetposedlg.h"
#include "ui_keysetposedlg.h"

keysetposeDlg::keysetposeDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keysetposeDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
}

keysetposeDlg::~keysetposeDlg()
{
    delete ui;
}

void keysetposeDlg::init_dlg_show()
{
    ui->setposecomboBox->clear();
    for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
    {
        ui->setposecomboBox->addItem(m_mcs->project->projecr_robpos_trace[n].name);
    }
    ui->record->clear();
}

void keysetposeDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    ui->setposecomboBox->clear();
    for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
    {
        ui->setposecomboBox->addItem(m_mcs->project->projecr_robpos_trace[n].name);
    }
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_SETPOSE_KEY)//坐标点姿态指令
        {
            QString namein=cmd.cmd_setpose_namein;
            QString nameout=cmd.cmd_setpose_nameout;
            std::vector<float> pose=cmd.cmd_setpose_pose;
            int point_trace_num;
            for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
            {
                if(namein==m_mcs->project->projecr_robpos_trace[n].name)
                {
                    point_trace_num=n;//找到要储存的点位下标
                    break;
                }
            }
            if(point_trace_num>=0&&point_trace_num<ui->setposecomboBox->count())
            {
                ui->setposecomboBox->setCurrentIndex(point_trace_num);
            }
            ui->setposenameEdit->setText(nameout);
            ui->setposeRXEdit->setText(QString::number(pose[0],'f',ROBOT_POSTURE_DECIMAL_PLACE));
            ui->setposeRYEdit->setText(QString::number(pose[1],'f',ROBOT_POSTURE_DECIMAL_PLACE));
            ui->setposeRZEdit->setText(QString::number(pose[2],'f',ROBOT_POSTURE_DECIMAL_PLACE));
        }
    }
    ui->record->clear();
}

void keysetposeDlg::close_dlg_show()
{

}

void keysetposeDlg::setbutton(int name)
{
    if(name==0)
    {
        b_inster=false;
        ui->setposeBtn->setText(QString::fromLocal8Bit("插入点坐标姿态指令"));
    }
    else
    {
        b_inster=true;
        ui->setposeBtn->setText(QString::fromLocal8Bit("替换点坐标姿态指令"));
    }
}

void keysetposeDlg::on_getposeBtn_clicked()
{
    //获取当前姿态
    int num=0;
    m_mcs->rob->TCPpos.nEn=false;
    while (m_mcs->rob->TCPpos.nEn==false)
    {
        if(num>10)
        {
            break;
        }
        usleep(ROB_WORK_DELAY_STEP);
        num++;
    }
    if(m_mcs->rob->TCPpos.nEn==false)
    {
        ui->record->append(QString::fromLocal8Bit("获取机器人坐标失败"));
    }
    else
    {
        ui->setposeRXEdit->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->setposeRYEdit->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->setposeRZEdit->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->record->append(QString::fromLocal8Bit("获取当前机器人坐标完成"));
    }
}

void keysetposeDlg::on_setposeBtn_clicked()
{
    my_cmd cmd;
    QString msg;
    QString nameout=ui->setposenameEdit->text();
    QString namein=ui->setposecomboBox->currentText();
    std::vector<float> pose(3);
    bool rc;
    if(namein.isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请选择输入点坐标"));
        return;
    }
    if(nameout.isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写生成点坐标名称"));
        return;
    }
    if(ui->setposeRXEdit->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写设置姿态RX"));
        return;
    }
    pose[0]=ui->setposeRXEdit->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("设置姿态RX格式错误"));
        return;
    }
    if(ui->setposeRYEdit->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写设置姿态RY"));
        return;
    }
    pose[1]=ui->setposeRYEdit->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("设置姿态RY格式错误"));
        return;
    }
    if(ui->setposeRZEdit->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写设置姿态RZ"));
        return;
    }
    pose[2]=ui->setposeRZEdit->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("设置姿态RZ格式错误"));
        return;
    }
    if(b_inster==false)
    {
        for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
        {
            if(nameout==m_mcs->project->projecr_robpos_trace[n].name)
            {
                ui->record->append(QString::fromLocal8Bit("生成的点坐标与已有的点坐标重名"));
                return;
            }
        }
    }
    msg=cmd.cmd_setpose(namein,pose,nameout);
    ui->record->append(QString::fromLocal8Bit("插入点坐标姿态指令成功"));
    cmd_msg=msg;
    done(1);
}




