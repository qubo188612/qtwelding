#include "keyscandlg.h"
#include "ui_keyscandlg.h"

keyscanDlg::keyscanDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keyscanDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;

    setmovec=new setmovecDlg(m_mcs);

    for(int n=0;n<ROBOTTCPNUM;n++)
    {
        QString msg="TCP: "+QString::number(n);
        ui->scantcpcombo->addItem(msg);
    }

    ui->scanchangecheckBox->setCheckState(Qt::Unchecked);
    ui->scanchangecombo->setDisabled(true);

    adoubleValidator_speed = new QDoubleValidator(0,0,ROBOT_SPEED_DECIMAL_PLACE,this);//限制3位小数
    ui->scanspeed->setValidator(adoubleValidator_speed);
}

keyscanDlg::~keyscanDlg()
{
    delete setmovec;
    delete ui;
}

void keyscanDlg::init_dlg_show()
{
    ui->scanchangecombo->clear();
    for(int n=0;n<m_mcs->project->projecr_coord_matrix4d.size();n++)
    {
        ui->scanchangecombo->addItem(m_mcs->project->projecr_coord_matrix4d[n].name);
    }
    ui->arrive_pos->clear();
    ui->groupBox_2->setDisabled(true);
    cmd_list_in.clear();
    ui->record->clear();
}

void keyscanDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    cmd_list_in=cmdlist;
    ui->scanchangecombo->clear();
    for(int n=0;n<m_mcs->project->projecr_coord_matrix4d.size();n++)
    {
        ui->scanchangecombo->addItem(m_mcs->project->projecr_coord_matrix4d[n].name);
    }
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_SCAN_KEY)//采集指令
        {
            float speed=cmd.cmd_scan_speed;//获取到的扫描速度
            int tcp=cmd.cmd_scan_tcp;//获取到扫描TCP
            Robmovemodel movemod=cmd.cmd_scan_movemod;//获取到的扫描模式
            QString name=cmd.cmd_scan_name;//获取到的扫描名字
            QString change=cmd.cmd_scan_change;//获取到的变换矩阵
            int change_trace_num;//找到要变换矩阵下标
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
                if(change_trace_num>=0&&change_trace_num<ui->scanchangecombo->count())
                {
                    ui->scanchangecombo->setCurrentIndex(change_trace_num);
                }
                ui->scanchangecheckBox->setCheckState(Qt::Checked);
                ui->scanchangecombo->setDisabled(true);
            }
            else
            {
                ui->scanchangecheckBox->setCheckState(Qt::Unchecked);
                ui->scanchangecombo->setDisabled(false);
            }
            if(movemod==MOVEJ||movemod==MOVEL)
            {
                RobPos pos=cmd.cmd_scan_pos;//获取到移动坐标
                QString msg="("+QString::number(pos.X,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                QString::number(pos.Y,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                QString::number(pos.Z,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                QString::number(pos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                QString::number(pos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                QString::number(pos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                QString::number(pos.out_1)+","+
                                QString::number(pos.out_2)+","+
                                QString::number(pos.out_3)+")";
                ui->arrive_pos->setText(msg);
                ui->groupBox_2->setDisabled(false);
            }
            else if(movemod==MOVEC)
            {
                ui->arrive_pos->clear();
                ui->groupBox_2->setDisabled(true);
            }
            if(tcp>=0&&tcp<ROBOTTCPNUM)
            {
                ui->scantcpcombo->setCurrentIndex(tcp);
            }
            ui->scanspeed->setText(QString::number(speed,'f',ROBOT_SPEED_DECIMAL_PLACE));
            if(movemod>=0&&movemod<=ui->scanmovemodecombo->count())
            {
                ui->scanmovemodecombo->setCurrentIndex(movemod);
            }
            ui->scanname->setText(name);
        }
    }
    ui->record->clear();
}

void keyscanDlg::close_dlg_show()
{

}

void keyscanDlg::setbutton(int name)
{
    if(name==0)
    {
        b_inster=false;
        ui->scanaddBtn->setText(QString::fromLocal8Bit("插入采集数据指令"));
    }
    else
    {
        b_inster=true;
        ui->scanaddBtn->setText(QString::fromLocal8Bit("替换采集数据指令"));
    }
}

//采集数据指令
void keyscanDlg::on_scanaddBtn_clicked()
{
    int tcp=ui->scantcpcombo->currentIndex();
    if(m_mcs->rob->b_link_ctx_posget==true)
    {
        sent_info_robot sendrob;
        sendrob.addr=ROB_TCP_NUM_REG_ADD;
        sendrob.ctx=m_mcs->rob->ctx_posget;
        sendrob.data.resize(1);
        sendrob.data[0]=tcp;
        m_mcs->rob->b_send_group_robot=false;
        m_mcs->rob->send_group_robot.push_back(sendrob);
        m_mcs->rob->ctx_robot_dosomeing=DO_WRITE_TASK;
        usleep(ROB_WORK_DELAY);
        int num=0;
        while(m_mcs->rob->b_send_group_robot==false)
        {
            if(num>10)
            {
                break;
            }
            usleep(ROB_WORK_DELAY_STEP);
            num++;
        }
        if(m_mcs->rob->b_send_group_robot==false)
        {
            ui->record->append(QString::fromLocal8Bit("机器人TCP设置异常"));
            return;
        }
        usleep(ROB_WORK_DELAY);//等待服务器获取到机器人坐标
        num=0;
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
            return;
        }
        bool rc;
        float speed=ui->scanspeed->text().toFloat(&rc);
        RobPos robpos=m_mcs->rob->TCPpos;
        QString change;
        Robmovemodel movemodel=(Robmovemodel)ui->scanmovemodecombo->currentIndex();
        my_cmd cmd;
        QString name=ui->scanname->text();
        QString msg;
        if(ui->scanspeed->text().isEmpty())
        {
            ui->record->append(QString::fromLocal8Bit("请填写采集速度"));
            return;
        }
        if(rc==false)
        {
            ui->record->append(QString::fromLocal8Bit("采集速度格式出错"));
            return;
        }
        if(ui->scanname->text().isEmpty())
        {
            ui->record->append(QString::fromLocal8Bit("请填写轨迹名称"));
            return;
        }
        if(ui->scanchangecheckBox->isChecked()==true)
        {
            change=ui->scanchangecombo->currentText();
        }
        switch(movemodel)
        {
            case MOVEL:
            case MOVEJ:
            {
                msg=cmd.cmd_scan(robpos,movemodel,speed,tcp,name,change);
            }
            break;
            case MOVEC:
            {
                setmovec->init_dlg_show();
                setmovec->setWindowTitle(QString::fromLocal8Bit("圆弧采集设置"));
                setmovec->set_arrive_param(speed,tcp);
                int rc=setmovec->exec();
                setmovec->close_dlg_show();
                if(rc!=0)//确定
                {
                    msg=cmd.cmd_scanC(setmovec->pos_st,setmovec->pos_center,setmovec->pos_ed,movemodel,speed,tcp,name,change);
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消圆弧采集设置"));
                    return;
                }
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
    else
    {
        ui->record->append(QString::fromLocal8Bit("与机器人的连接异常"));
    }
}

//长按到点按下
void keyscanDlg::on_arriveBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    bool rc;
    float speed=ui->scanspeed->text().toFloat(&rc);
    my_cmd cmd;
    QString msg;
    if(ui->scanspeed->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写采集速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("采集速度格式出错"));
        return;
    }
    QString key;
    rc=cmd.decodecmd(cmd_list_in,msg,key);
    Robmovemodel movemod=cmd.cmd_scan_movemod;//获取到的移动模式
    if(movemod==MOVEJ||movemod==MOVEL)
    {
        int tcp=ui->scantcpcombo->currentIndex();
        RobPos pos=cmd.cmd_scan_pos;//获取到移动坐标
        movemod=MOVEJ;//用关节移动方式到位
        m_mcs->tosendbuffer->cmd_move(pos,movemod,speed,tcp);//移动
    }
    else
    {
        ui->record->append(QString::fromLocal8Bit("只有MOVEJ和MOVEL的点可以到位运动"));
        return;
    }
    ui->record->append(QString::fromLocal8Bit("开始到位中..."));
}

//长按到点松开
void keyscanDlg::on_arriveBtn_released()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    ui->record->append(QString::fromLocal8Bit("停止到位"));
}

//变换矩阵有效
void keyscanDlg::on_scanchangecheckBox_stateChanged(int arg1)
{
    if(arg1==0)
    {
        ui->scanchangecombo->setDisabled(true);
    }
    else
    {
        ui->scanchangecombo->setDisabled(false);
    }
}

