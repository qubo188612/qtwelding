#include "keysscandlg.h"
#include "ui_keysscandlg.h"

keysscanDlg::keysscanDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keysscanDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

    for(int n=0;n<ROBOTTCPNUM;n++)
    {
        QString msg="TCP: "+QString::number(n);
        ui->sscantcpcombo->addItem(msg);
    }

    ui->sscanchangecheckBox->setCheckState(Qt::Unchecked);
    ui->sscanchangecombo->setDisabled(true);

    ui->sscanmovemodecombo->setCurrentIndex(MOVEL);
    ui->label_17->setText(QString::fromLocal8Bit("移动点位"));
    ui->label_18->hide();
    ui->label_19->hide();
    ui->pos2comboBox->hide();
    ui->pos3comboBox->hide();

    adoubleValidator_speed = new QDoubleValidator(ROBOT_SPEED_DECIMAL_BOTTOM,ROBOT_SPEED_DECIMAL_TOP,ROBOT_SPEED_DECIMAL_PLACE,this);//限制3位小数
    ui->sscanspeed->setValidator(adoubleValidator_speed);
}

keysscanDlg::~keysscanDlg()
{
    delete adoubleValidator_speed;
    delete ui;
}

void keysscanDlg::init_dlg_show()
{
    ui->pos1comboBox->clear();
    ui->pos2comboBox->clear();
    ui->pos3comboBox->clear();
    for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
    {
        ui->pos1comboBox->addItem(m_mcs->project->projecr_robpos_trace[n].name);
        ui->pos2comboBox->addItem(m_mcs->project->projecr_robpos_trace[n].name);
        ui->pos3comboBox->addItem(m_mcs->project->projecr_robpos_trace[n].name);
    }
    ui->sscanchangecombo->clear();
    for(int n=0;n<m_mcs->project->projecr_coord_matrix4d.size();n++)
    {
        ui->sscanchangecombo->addItem(m_mcs->project->projecr_coord_matrix4d[n].name);
    }
    ui->record->clear();
}

void keysscanDlg::init_dlg_show(QString cmdlist)
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
    ui->sscanchangecombo->clear();
    for(int n=0;n<m_mcs->project->projecr_coord_matrix4d.size();n++)
    {
        ui->sscanchangecombo->addItem(m_mcs->project->projecr_coord_matrix4d[n].name);
    }
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_SCAN_KEY)//采集指令
        {
            float speed=cmd.cmd_sscan_speed;//获取到的扫描速度
            int tcp=cmd.cmd_sscan_tcp;//获取到扫描TCP
            Robmovemodel movemod=cmd.cmd_sscan_movemod;//获取到的扫描模式
            QString name=cmd.cmd_sscan_name;//获取到的扫描名字
            QString change=cmd.cmd_sscan_change;//获取到的变换矩阵
            int change_trace_num;//找到要变换矩阵下标
            QString s_pos=cmd.cmd_sscan_pos;//找到的扫描点位名字
            QString s_pos1=cmd.cmd_sscan_pos1;//找到的扫描点位名字
            QString s_pos2=cmd.cmd_sscan_pos2;//找到的扫描点位名字
            QString s_pos3=cmd.cmd_sscan_pos3;//找到的扫描点位名字
            int point_trace_num;//找到扫描点位下标
            int point1_trace_num;//找到扫描点位下标
            int point2_trace_num;//找到扫描点位下标
            int point3_trace_num;//找到扫描点位下标
            if(!change.isEmpty())
            {
                for(int n=0;n<m_mcs->project->projecr_coord_matrix4d.size();n++)
                {
                    if(change==m_mcs->project->projecr_coord_matrix4d[n].name)
                    {
                        change_trace_num=n;//找到要储存的焊接轨道下标
                        break;
                    }
                }
                if(change_trace_num>=0&&change_trace_num<ui->sscanchangecombo->count())
                {
                    ui->sscanchangecombo->setCurrentIndex(change_trace_num);
                }
                ui->sscanchangecheckBox->setCheckState(Qt::Checked);
                ui->sscanchangecombo->setDisabled(true);
            }
            else
            {
                ui->sscanchangecheckBox->setCheckState(Qt::Unchecked);
                ui->sscanchangecombo->setDisabled(false);
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
            if(tcp>=0&&tcp<ROBOTTCPNUM)
            {
                ui->sscantcpcombo->setCurrentIndex(tcp);
            }
            ui->sscanspeed->setText(QString::number(speed,'f',ROBOT_SPEED_DECIMAL_PLACE));
            if(movemod>=0&&movemod<=ui->sscanmovemodecombo->count())
            {
                ui->sscanmovemodecombo->setCurrentIndex(movemod);
            }
            ui->sscanname->setText(name);
        }
    }
    ui->record->clear();
}

void keysscanDlg::close_dlg_show()
{

}

void keysscanDlg::setbutton(int name)
{
    if(name==0)
    {
        ui->sscanaddBtn->setText(QString::fromLocal8Bit("插入采集数据指令"));
    }
    else
    {
        ui->sscanaddBtn->setText(QString::fromLocal8Bit("替换采集数据指令"));
    }
}

//移动模式改变
void keysscanDlg::on_sscanmovemodecombo_currentIndexChanged(int index)
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
void keysscanDlg::on_sscanchangecheckBox_stateChanged(int arg1)
{
    if(arg1==0)
    {
        b_inster=false;
        ui->sscanchangecombo->setDisabled(true);
    }
    else
    {
        b_inster=true;
        ui->sscanchangecombo->setDisabled(false);
    }
}

//采集数据指令
void keysscanDlg::on_sscanaddBtn_clicked()
{
    bool rc;
    float speed=ui->sscanspeed->text().toFloat(&rc);
    RobPos robpos=m_mcs->rob->TCPpos;
    QString change;
    int tcp=ui->sscantcpcombo->currentIndex();
    QString s_pos=ui->pos1comboBox->currentText();
    QString s_pos1=ui->pos1comboBox->currentText();
    QString s_pos2=ui->pos2comboBox->currentText();
    QString s_pos3=ui->pos3comboBox->currentText();
    Robmovemodel movemodel=(Robmovemodel)ui->sscanmovemodecombo->currentIndex();
    my_cmd cmd;
    QString name=ui->sscanname->text();
    QString msg;
    if(tcp<0||tcp>ui->sscantcpcombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择一个tcp"));
        return;
    }
    if(ui->sscanspeed->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写采集速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("采集速度格式出错"));
        return;
    }
    if(ui->sscanname->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写轨迹名称"));
        return;
    }
    if(ui->sscanchangecheckBox->isChecked()==true)
    {
        change=ui->sscanchangecombo->currentText();
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
            msg=cmd.cmd_sscan(s_pos,movemodel,speed,tcp,name,change);
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
            msg=cmd.cmd_sscanC(s_pos1,s_pos2,s_pos3,movemodel,speed,tcp,name,change);
        }
        break;
    }
    if(b_inster==false)//插入
    {
        std::vector<QString> err_msg;
        m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg);
        for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
        {
            if(name==m_mcs->project->project_scan_trace[n].name)
            {
                ui->record->append(QString::fromLocal8Bit("扫描轨迹与已有的轨迹重名"));
                return;
            }
        }
    }
    ui->record->append(QString::fromLocal8Bit("插入采集数据指令成功"));
    cmd_msg=msg;
    done(1);
}

