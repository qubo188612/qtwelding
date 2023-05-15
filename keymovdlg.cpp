#include "keymovdlg.h"
#include "ui_keymovdlg.h"

keymovDlg::keymovDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keymovDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;

    setmovec=new setmovecDlg(m_mcs);

    for(int n=0;n<ROBOTTCPNUM;n++)
    {
        QString msg="TCP: "+QString::number(n);
        ui->movetcpcombo->addItem(msg);
    }

    ui->movechangecheckBox->setCheckState(Qt::Unchecked);
    ui->movechangecombo->setDisabled(true);

    adoubleValidator_speed = new QDoubleValidator(0,0,ROBOT_SPEED_DECIMAL_PLACE,this);//限制3位小数
    ui->movespeed->setValidator(adoubleValidator_speed);
}

keymovDlg::~keymovDlg()
{
    delete setmovec;
    delete ui;
}

void keymovDlg::init_dlg_show()
{
    ui->movechangecombo->clear();
    for(int n=0;n<m_mcs->project->projecr_coord_matrix4d.size();n++)
    {
        ui->movechangecombo->addItem(m_mcs->project->projecr_coord_matrix4d[n].name);
    }
    ui->arrive_pos->clear();
    ui->groupBox_2->setDisabled(true);
    cmd_list_in.clear();
    ui->record->clear();
}

void keymovDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    cmd_list_in=cmdlist;
    ui->movechangecombo->clear();
    for(int n=0;n<m_mcs->project->projecr_coord_matrix4d.size();n++)
    {
        ui->movechangecombo->addItem(m_mcs->project->projecr_coord_matrix4d[n].name);
    }
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_MOV_KEY)//移动指令
        {
            int tcp=cmd.cmd_move_tcp;//获取到移动TCP
            float speed=cmd.cmd_move_speed;//获取到速度值
            Robmovemodel movemod=cmd.cmd_move_movemod;//获取到的移动模式
            QString change=cmd.cmd_move_change;//获取到的变换矩阵
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
                if(change_trace_num>=0&&change_trace_num<ui->movechangecombo->count())
                {
                    ui->movechangecombo->setCurrentIndex(change_trace_num);
                }
                ui->movechangecheckBox->setCheckState(Qt::Checked);
                ui->movechangecombo->setDisabled(true);
            }
            else
            {
                ui->movechangecheckBox->setCheckState(Qt::Unchecked);
                ui->movechangecombo->setDisabled(false);
            }
            if(movemod==MOVEJ||movemod==MOVEL)
            {
                RobPos pos=cmd.cmd_move_pos;//获取到移动坐标
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
                ui->movetcpcombo->setCurrentIndex(tcp);
            }
            ui->movespeed->setText(QString::number(speed,'f',ROBOT_SPEED_DECIMAL_PLACE));
            if(movemod>=0&&movemod<ui->movemodecombo->count())
            {
                ui->movemodecombo->setCurrentIndex(movemod);
            }
        }
    }
    ui->record->clear();
}

void keymovDlg::close_dlg_show()
{

}

void keymovDlg::setbutton(int name)
{
    if(name==0)
    {
        ui->moveaddBtn->setText(QString::fromLocal8Bit("插入移动指令"));
    }
    else
    {
        ui->moveaddBtn->setText(QString::fromLocal8Bit("替换移动指令"));
    }
}

//插入移动指令
void keymovDlg::on_moveaddBtn_clicked()
{
    int tcp=ui->movetcpcombo->currentIndex();
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
        Robmovemodel movemodel=(Robmovemodel)ui->movemodecombo->currentIndex();
        bool rc;
        float speed=ui->movespeed->text().toFloat(&rc);
        QString change;
        RobPos robpos=m_mcs->rob->TCPpos;
        my_cmd cmd;
        QString msg;
        if(ui->movespeed->text().isEmpty())
        {
            ui->record->append(QString::fromLocal8Bit("请填写移动速度"));
            return;
        }
        if(rc==false)
        {
            ui->record->append(QString::fromLocal8Bit("移动速度格式出错"));
            return;
        }   
        if(ui->movechangecheckBox->isChecked()==true)
        {
            change=ui->movechangecombo->currentText();
        }
        switch(movemodel)
        {
            case MOVEL:
            case MOVEJ:
            {
                msg=cmd.cmd_move(robpos,movemodel,speed,tcp,change);
            }
            break;
            case MOVEC:
            {
                setmovec->init_dlg_show(cmd_list_in);
                setmovec->setWindowTitle(QString::fromLocal8Bit("圆弧移动设置"));
                setmovec->set_arrive_param(speed,tcp);
                int rc=setmovec->exec();
                setmovec->close_dlg_show();
                if(rc!=0)//确定
                {
                    msg=cmd.cmd_moveC(setmovec->pos_st,setmovec->pos_center,setmovec->pos_ed,movemodel,speed,tcp,change);
                }
                else
                {
                    ui->record->append(QString::fromLocal8Bit("取消圆弧移动设置"));
                    return;
                }
            }
            break;
        }
        ui->record->append(QString::fromLocal8Bit("插入移动指令成功"));
        cmd_msg=msg;
        done(1);
    }
    else
    {
        ui->record->append(QString::fromLocal8Bit("与机器人的连接异常"));
    }
}


//到点按下
void keymovDlg::on_arriveBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    bool rc;
    float speed=ui->movespeed->text().toFloat(&rc);
    my_cmd cmd;
    QString msg;
    if(ui->movespeed->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("移动速度格式出错"));
        return;
    }
    QString key;
    rc=cmd.decodecmd(cmd_list_in,msg,key);
    Robmovemodel movemod=cmd.cmd_move_movemod;//获取到的移动模式
    if(movemod==MOVEJ||movemod==MOVEL)
    {
        int tcp=ui->movetcpcombo->currentIndex();
        RobPos pos=cmd.cmd_move_pos;//获取到移动坐标
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

//到点抬起
void keymovDlg::on_arriveBtn_released()
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
void keymovDlg::on_movechangecheckBox_stateChanged(int arg1)
{
    if(arg1==0)
    {
        ui->movechangecombo->setDisabled(true);
    }
    else
    {
        ui->movechangecombo->setDisabled(false);
    }
}

