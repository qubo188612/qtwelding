#include "keysearchenddlg.h"
#include "ui_keysearchenddlg.h"

keysearchendDlg::keysearchendDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keysearchendDlg)
{
    ui->setupUi(this);

    m_mcs=mcs;

    setmovec=new setmovecDlg(m_mcs);

    for(int n=0;n<ROBOTTCPNUM;n++)
    {
        QString msg="TCP: "+QString::number(n);
        ui->searchendtcpcombo->addItem(msg);
    }

    ui->searchendchangecheckBox->setCheckState(Qt::Unchecked);
    ui->searchendchangecombo->setDisabled(true);

    adoubleValidator_speed = new QDoubleValidator(ROBOT_SPEED_DECIMAL_BOTTOM,ROBOT_SPEED_DECIMAL_TOP,ROBOT_SPEED_DECIMAL_PLACE,this);//限制3位小数
    ui->searchendspeed->setValidator(adoubleValidator_speed);
}

keysearchendDlg::~keysearchendDlg()
{
    delete setmovec;
    delete adoubleValidator_speed;
    delete ui;
}

void keysearchendDlg::init_dlg_show()
{
    ui->searchendchangecombo->clear();
    for(int n=0;n<m_mcs->project->projecr_coord_matrix4d.size();n++)
    {
        ui->searchendchangecombo->addItem(m_mcs->project->projecr_coord_matrix4d[n].name);
    }
    ui->arrive_pos->clear();
    ui->groupBox_2->setDisabled(true);
    cmd_list_in.clear();
    ui->record->clear();
}

void keysearchendDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    cmd_list_in=cmdlist;
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_SEARCHEND_KEY)//寻位指令
        {
            float speed=cmd.cmd_searchend_speed;//获取到的寻位速度
            int tcp=cmd.cmd_searchend_tcp;//获取到寻位TCP
            Robmovemodel movemod=cmd.cmd_searchend_movemod;//获取到的寻位模式
            QString name=cmd.cmd_searchend_name;//获取到的寻位名字
            QString change=cmd.cmd_searchend_change;//获取到的变换矩阵
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
                if(change_trace_num>=0&&change_trace_num<ui->searchendchangecombo->count())
                {
                    ui->searchendchangecombo->setCurrentIndex(change_trace_num);
                }
                ui->searchendchangecheckBox->setCheckState(Qt::Checked);
                ui->searchendchangecombo->setDisabled(true);
            }
            else
            {
                ui->searchendchangecheckBox->setCheckState(Qt::Unchecked);
                ui->searchendchangecombo->setDisabled(false);
            }
            if(movemod==MOVEJ||movemod==MOVEL)
            {
                RobPos pos=cmd.cmd_searchend_pos;//获取到移动坐标
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
                ui->searchendtcpcombo->setCurrentIndex(tcp);
            }
            ui->searchendspeed->setText(QString::number(speed,'f',ROBOT_SPEED_DECIMAL_PLACE));
            if(movemod>=0&&movemod<=ui->searchendmovemodecombo->count())
            {
                ui->searchendmovemodecombo->setCurrentIndex(movemod);
            }
            ui->searchendname->setText(name);
        }
    }
    ui->record->clear();
}

void keysearchendDlg::close_dlg_show()
{

}

void keysearchendDlg::setbutton(int name)
{
    if(name==0)
    {
        b_inster=false;
        ui->searchendaddBtn->setText(QString::fromLocal8Bit("插入寻位指令"));
    }
    else
    {
        b_inster=true;
        ui->searchendaddBtn->setText(QString::fromLocal8Bit("替换寻位指令"));
    }
}

//寻位指令
void keysearchendDlg::on_searchendaddBtn_clicked()
{
    int tcp=ui->searchendtcpcombo->currentIndex();
    if(tcp<0||tcp>ui->searchendtcpcombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择一个tcp"));
        return;
    }
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
        float speed;
        RobPos robpos=m_mcs->rob->TCPpos;
        Robmovemodel movemodel=(Robmovemodel)ui->searchendmovemodecombo->currentIndex();
        std::vector<float> sidemove(3);
        float sidespeed;
        int side;
        QString change;

        my_cmd cmd;
        QString name=ui->searchendname->text();
        if(ui->searchendspeed->text().isEmpty())
        {
            ui->record->append(QString::fromLocal8Bit("请填写寻位速度"));
            return;
        }
        speed=ui->searchendspeed->text().toFloat(&rc);
        if(rc==false)
        {
            ui->record->append(QString::fromLocal8Bit("寻位速度格式出错"));
            return;
        }
        if(ui->searchendname->text().isEmpty())
        {
            ui->record->append(QString::fromLocal8Bit("请填写寻位点名称"));
            return;
        }
        if(ui->searchendchangecheckBox->isChecked()==true)
        {
            change=ui->searchendchangecombo->currentText();
        }

        QString msg;
        switch(movemodel)
        {
            case MOVEL:
            case MOVEJ:
            {
                msg=cmd.cmd_searchend(robpos,movemodel,speed,tcp,name,change);
            }
            break;
            case MOVEC:
            {
                setmovec->init_dlg_show();
                setmovec->setWindowTitle(QString::fromLocal8Bit("圆弧寻位设置"));
                setmovec->set_arrive_param(speed,tcp);
                int rc=setmovec->exec();
                setmovec->close_dlg_show();
                if(rc!=0)//确定
                {
                    msg=cmd.cmd_searchendC(setmovec->pos_st,setmovec->pos_center,setmovec->pos_ed,movemodel,speed,tcp,name,change);
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消圆弧寻位设置"));
                    return;
                }
            }
            break;
        }
        if(b_inster==false)//插入
        {
            std::vector<QString> err_msg;
            m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg);
            for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
            {
                if(name==m_mcs->project->projecr_robpos_trace[n].name)
                {
                    ui->record->append(QString::fromLocal8Bit("寻位点与已有的点重名"));
                    return;
                }
            }
        }
        ui->record->append(QString::fromLocal8Bit("插入寻位指令成功"));
        cmd_msg=msg;
        done(1);
    }
    else
    {
        ui->record->append(QString::fromLocal8Bit("与机器人的连接异常"));
    }
}

//长按到点按下
void keysearchendDlg::on_arriveBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    bool rc;
    float speed=ui->searchendspeed->text().toFloat(&rc);
    my_cmd cmd;
    QString msg;
    if(ui->searchendspeed->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写寻位速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("寻位速度格式出错"));
        return;
    }
    QString key;
    rc=cmd.decodecmd(cmd_list_in,msg,key);
    Robmovemodel movemod=cmd.cmd_searchend_movemod;//获取到的寻位模式
    if(movemod==MOVEJ||movemod==MOVEL)
    {
        int tcp=ui->searchendtcpcombo->currentIndex();
        RobPos pos=cmd.cmd_searchend_pos;//获取到移动坐标
        movemod=MOVEJ;//用关节移动方式到位
        m_mcs->robotcontrol->RobotOPEN_ELE();
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
void keysearchendDlg::on_arriveBtn_released()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    m_mcs->robotcontrol->RobotCLOSE_ELE();
    ui->record->append(QString::fromLocal8Bit("停止到位"));
}

//是否使用变换矩阵
void keysearchendDlg::on_searchendchangecheckBox_stateChanged(int arg1)
{
    if(arg1==0)
    {
        ui->searchendchangecombo->setDisabled(true);
    }
    else
    {
        ui->searchendchangecombo->setDisabled(false);
    }
}



