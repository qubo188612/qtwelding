#include "keysetposedlg.h"
#include "ui_keysetposedlg.h"

keysetposeDlg::keysetposeDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keysetposeDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

//美化界面
#ifdef OPEN_BEAUTIFY_INTERFACE
    ui->getposeBtn->setStyleSheet(FONT_BUTTON_INFO);
    ui->setposeaddXEdit->setStyleSheet(FONT_EDIT_INFO);
    ui->setposeaddYEdit->setStyleSheet(FONT_EDIT_INFO);
    ui->setposeaddZEdit->setStyleSheet(FONT_EDIT_INFO);
    ui->setposeBtn->setStyleSheet(FONT_BUTTON_INFO);
    ui->setposecomboBox->setStyleSheet(FONT_COMBOBOX_INFO);
    ui->setposenameEdit->setStyleSheet(FONT_EDIT_INFO);
    ui->setposeRXEdit->setStyleSheet(FONT_EDIT_INFO);
    ui->setposeRYEdit->setStyleSheet(FONT_EDIT_INFO);
    ui->setposeRZEdit->setStyleSheet(FONT_EDIT_INFO);
    ui->record->setStyleSheet(FONT_TEXTBROWERS_INFO);
#endif
}

keysetposeDlg::~keysetposeDlg()
{
    delete ui;
}

void keysetposeDlg::init_dlg_show()
{
    ui->setposecomboBox->clear();
    for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
    {
        ui->setposecomboBox->addItem(m_mcs->project->project_robpos_trace[n].name);
    }
    ui->record->clear();
}

void keysetposeDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    ui->setposecomboBox->clear();
    for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
    {
        ui->setposecomboBox->addItem(m_mcs->project->project_robpos_trace[n].name);
    }
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_SETPOSE_KEY)//坐标点姿态指令
        {
            QString namein=cmd.cmd_setpose_namein;
            QString nameout=cmd.cmd_setpose_nameout;
            std::vector<float> pose=cmd.cmd_setpose_pose;
            std::vector<float> add=cmd.cmd_setpose_add;
            int point_trace_num;
            for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
            {
                if(namein==m_mcs->project->project_robpos_trace[n].name)
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
            ui->setposeaddXEdit->setText(QString::number(add[0],'f',ROBOT_POSE_DECIMAL_PLACE));
            ui->setposeaddYEdit->setText(QString::number(add[1],'f',ROBOT_POSE_DECIMAL_PLACE));
            ui->setposeaddZEdit->setText(QString::number(add[2],'f',ROBOT_POSE_DECIMAL_PLACE));
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
        ui->setposeBtn->setText(QStringLiteral("插入点坐标姿态指令"));
    }
    else
    {
        b_inster=true;
        ui->setposeBtn->setText(QStringLiteral("替换点坐标姿态指令"));
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
        ui->record->append(QStringLiteral("获取机器人坐标失败"));
    }
    else
    {
        ui->setposeRXEdit->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->setposeRYEdit->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->setposeRZEdit->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->record->append(QStringLiteral("获取当前机器人坐标完成"));
    }
}

void keysetposeDlg::on_setposeBtn_clicked()
{
    my_cmd cmd;
    QString msg;
    QString nameout=ui->setposenameEdit->text();
    QString namein=ui->setposecomboBox->currentText();
    std::vector<float> pose(3);
    std::vector<float> add(3);
    bool rc;
    if(namein.isEmpty())
    {
        ui->record->append(QStringLiteral("请选择输入点坐标"));
        return;
    }
    if(nameout.isEmpty())
    {
        ui->record->append(QStringLiteral("请填写生成点坐标名称"));
        return;
    }
    if(ui->setposeRXEdit->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写设置姿态RX"));
        return;
    }
    pose[0]=ui->setposeRXEdit->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("设置姿态RX格式错误"));
        return;
    }
    if(ui->setposeRYEdit->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写设置姿态RY"));
        return;
    }
    pose[1]=ui->setposeRYEdit->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("设置姿态RY格式错误"));
        return;
    }
    if(ui->setposeRZEdit->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写设置姿态RZ"));
        return;
    }
    pose[2]=ui->setposeRZEdit->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("设置姿态RZ格式错误"));
        return;
    }

    if(ui->setposeaddXEdit->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写设置补偿X"));
        return;
    }
    add[0]=ui->setposeaddXEdit->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("设置补偿X格式错误"));
        return;
    }
    if(ui->setposeaddYEdit->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写设置补偿Y"));
        return;
    }
    add[1]=ui->setposeaddYEdit->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("设置补偿Y格式错误"));
        return;
    }
    if(ui->setposeaddZEdit->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写设置补偿Z"));
        return;
    }
    add[2]=ui->setposeaddZEdit->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("设置补偿Z格式错误"));
        return;
    }
    if(b_inster==false)
    {
        for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
        {
            if(nameout==m_mcs->project->project_robpos_trace[n].name)
            {
                ui->record->append(QStringLiteral("生成的点坐标与已有的点坐标重名"));
                return;
            }
        }
    }
    msg=cmd.cmd_setpose(namein,pose,add,nameout);
    ui->record->append(QStringLiteral("插入点坐标姿态指令成功"));
    cmd_msg=msg;
    done(1);
}




