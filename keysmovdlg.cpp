#include "keysmovdlg.h"
#include "ui_keysmovdlg.h"

keysmovDlg::keysmovDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keysmovDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;

    for(int n=0;n<ROBOTTCPNUM;n++)
    {
        QString msg="TCP: "+QString::number(n);
        ui->smovetcpcombo->addItem(msg);
    }

    ui->smovechangecheckBox->setCheckState(Qt::Unchecked);
    ui->smovechangecombo->setDisabled(true);

    ui->smovemodecombo->setCurrentIndex(MOVEL);
    ui->label_17->setText(QString::fromLocal8Bit("移动点位"));
    ui->label_18->hide();
    ui->label_19->hide();
    ui->pos2comboBox->hide();
    ui->pos3comboBox->hide();

    adoubleValidator_speed = new QDoubleValidator(0,0,ROBOT_SPEED_DECIMAL_PLACE,this);//限制3位小数
    ui->smovespeed->setValidator(adoubleValidator_speed);
}

keysmovDlg::~keysmovDlg()
{
    delete ui;
}

void keysmovDlg::init_dlg_show()
{
    ui->record->clear();
    ui->pos1comboBox->clear();
    ui->pos2comboBox->clear();
    ui->pos3comboBox->clear();
    for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
    {
        ui->pos1comboBox->addItem(m_mcs->project->projecr_robpos_trace[n].name);
        ui->pos2comboBox->addItem(m_mcs->project->projecr_robpos_trace[n].name);
        ui->pos3comboBox->addItem(m_mcs->project->projecr_robpos_trace[n].name);
    }
    ui->smovechangecombo->clear();
    for(int n=0;n<m_mcs->project->projecr_coord_matrix4d.size();n++)
    {
        ui->smovechangecombo->addItem(m_mcs->project->projecr_coord_matrix4d[n].name);
    }
}

void keysmovDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    ui->pos1comboBox->clear();
    ui->pos2comboBox->clear();
    ui->pos3comboBox->clear();
    for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
    {
        ui->pos1comboBox->addItem(m_mcs->project->projecr_robpos_trace[n].name);
        ui->pos2comboBox->addItem(m_mcs->project->projecr_robpos_trace[n].name);
        ui->pos3comboBox->addItem(m_mcs->project->projecr_robpos_trace[n].name);
    }
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_SMOV_KEY)//定位指令
        {
            int tcp=cmd.cmd_smove_tcp;//获取到移动TCP
            float speed=cmd.cmd_smove_speed;//获取到速度值
            Robmovemodel movemod=cmd.cmd_smove_movemod;//获取到的移动模式
            QString change=cmd.cmd_smove_change;//找到变换矩阵名字
            QString s_pos=cmd.cmd_smove_pos;//找到的点位名字
            QString s_pos1=cmd.cmd_smove_pos1;//找到的点位名字
            QString s_pos2=cmd.cmd_smove_pos2;//找到的点位名字
            QString s_pos3=cmd.cmd_smove_pos3;//找到的点位名字
            int change_trace_num;//找到要变换矩阵下标
            int point_trace_num;//找到点位下标
            int point1_trace_num;//找到点位下标
            int point2_trace_num;//找到点位下标
            int point3_trace_num;//找到点位下标
            //这里添加移动命令
            if(!change.isEmpty())
            {
                for(int n=0;n<m_mcs->project->projecr_coord_matrix4d.size();n++)
                {
                    if(change==m_mcs->project->projecr_coord_matrix4d[n].name)
                    {
                        change_trace_num=n;//找到要储存的变换矩阵下标
                        break;
                    }
                }
                if(change_trace_num>=0&&change_trace_num<ui->smovechangecombo->count())
                {
                    ui->smovechangecombo->setCurrentIndex(change_trace_num);
                }
                ui->smovechangecheckBox->setCheckState(Qt::Checked);
                ui->smovechangecombo->setDisabled(true);
            }
            else
            {
                ui->smovechangecheckBox->setCheckState(Qt::Unchecked);
                ui->smovechangecombo->setDisabled(false);
            }
            if(tcp>=0&&tcp<ROBOTTCPNUM)
            {
                ui->smovetcpcombo->setCurrentIndex(tcp);
            }
            ui->smovespeed->setText(QString::number(speed,'f',ROBOT_SPEED_DECIMAL_PLACE));
            if(movemod>=0&&movemod<ui->smovemodecombo->count())
            {
                ui->smovemodecombo->setCurrentIndex(movemod);
            }
            if(movemod==MOVEJ||movemod==MOVEL)
            {
                for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
                {
                    if(s_pos==m_mcs->project->projecr_robpos_trace[n].name)
                    {
                        point_trace_num=n;//找到要储存的点位下标
                        break;
                    }
                }
                ui->label_17->setText(QString::fromLocal8Bit("移动点位"));
                ui->label_18->hide();
                ui->label_19->hide();
                ui->pos2comboBox->hide();
                ui->pos3comboBox->hide();
                if(point_trace_num>=0&&point_trace_num<ui->pos1comboBox->count())
                {
                    ui->pos1comboBox->setCurrentIndex(point_trace_num);
                }
            }
            else if(movemod==MOVEC)
            {
                for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
                {
                    if(s_pos1==m_mcs->project->projecr_robpos_trace[n].name)
                    {
                        point1_trace_num=n;//找到要储存的点位下标
                        break;
                    }
                }
                for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
                {
                    if(s_pos2==m_mcs->project->projecr_robpos_trace[n].name)
                    {
                        point2_trace_num=n;//找到要储存的点位下标
                        break;
                    }
                }
                for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
                {
                    if(s_pos3==m_mcs->project->projecr_robpos_trace[n].name)
                    {
                        point3_trace_num=n;//找到要储存的点位下标
                        break;
                    }
                }
                ui->label_17->setText(QString::fromLocal8Bit("圆弧起点点位"));
                ui->label_18->show();
                ui->label_19->show();
                ui->pos2comboBox->show();
                ui->pos3comboBox->show();
                if(point1_trace_num>=0&&point1_trace_num<ui->pos1comboBox->count())
                {
                    ui->pos1comboBox->setCurrentIndex(point1_trace_num);
                }
                if(point2_trace_num>=0&&point2_trace_num<ui->pos2comboBox->count())
                {
                    ui->pos2comboBox->setCurrentIndex(point2_trace_num);
                }
                if(point3_trace_num>=0&&point3_trace_num<ui->pos3comboBox->count())
                {
                    ui->pos3comboBox->setCurrentIndex(point3_trace_num);
                }
            }
        }
    }
    ui->record->clear();
}

void keysmovDlg::close_dlg_show()
{

}

void keysmovDlg::setbutton(int name)
{
    if(name==0)
    {
        ui->smoveaddBtn->setText(QString::fromLocal8Bit("插入点位移动指令"));
    }
    else
    {
        ui->smoveaddBtn->setText(QString::fromLocal8Bit("替换点位移动指令"));
    }
}

//移动模式改变
void keysmovDlg::on_smovemodecombo_currentIndexChanged(int index)
{
    switch(index)
    {
        case MOVEL:
        case MOVEJ:
        {
            ui->label_17->setText(QString::fromLocal8Bit("移动点位"));
            ui->label_18->hide();
            ui->label_19->hide();
            ui->pos2comboBox->hide();
            ui->pos3comboBox->hide();
        }
        break;
        case MOVEC:
        {
            ui->label_17->setText(QString::fromLocal8Bit("圆弧起点点位"));
            ui->label_18->show();
            ui->label_19->show();
            ui->pos2comboBox->show();
            ui->pos3comboBox->show();
        }
        break;
    }
}

//使用变换矩阵
void keysmovDlg::on_smovechangecheckBox_stateChanged(int arg1)
{
    if(arg1==0)
    {
        ui->smovechangecombo->setDisabled(true);
    }
    else
    {
        ui->smovechangecombo->setDisabled(false);
    }
}

//点位移动指令
void keysmovDlg::on_smoveaddBtn_clicked()
{
    Robmovemodel movemodel=(Robmovemodel)ui->smovemodecombo->currentIndex();
    bool rc;
    float speed=ui->smovespeed->text().toFloat(&rc);
    int tcp=ui->smovetcpcombo->currentIndex();
    QString change;
    QString s_pos=ui->pos1comboBox->currentText();
    QString s_pos1=ui->pos1comboBox->currentText();
    QString s_pos2=ui->pos2comboBox->currentText();
    QString s_pos3=ui->pos3comboBox->currentText();
    my_cmd cmd;
    QString msg;
    if(ui->smovespeed->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("移动速度格式出错"));
        return;
    }
    if(ui->smovechangecheckBox->isChecked()==true)
    {
        change=ui->smovechangecombo->currentText();
    }
    switch(movemodel)
    {
        case MOVEL:
        case MOVEJ:
        {
            if(s_pos.isEmpty())
            {
                ui->record->append(QString::fromLocal8Bit("请选择移动点位"));
                return;
            }
            msg=cmd.cmd_smove(s_pos,movemodel,speed,tcp,change);
        }
        break;
        case MOVEC:
        {
            if(s_pos1.isEmpty())
            {
                ui->record->append(QString::fromLocal8Bit("请选择圆弧起点位"));
                return;
            }
            if(s_pos2.isEmpty())
            {
                ui->record->append(QString::fromLocal8Bit("请选择圆弧中间点位"));
                return;
            }
            if(s_pos3.isEmpty())
            {
                ui->record->append(QString::fromLocal8Bit("请选择圆弧终点位"));
                return;
            }
            msg=cmd.cmd_smoveC(s_pos1,s_pos2,s_pos3,movemodel,speed,tcp,change);
        }
        break;
    }
    ui->record->append(QString::fromLocal8Bit("插入移动指令成功"));
    cmd_msg=msg;
    done(1);
}

