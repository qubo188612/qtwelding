#include "keysearchdlg.h"
#include "ui_keysearchdlg.h"

keysearchDlg::keysearchDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keysearchDlg)
{
    ui->setupUi(this);

    m_mcs=mcs;

    setmovec=new setmovecDlg(m_mcs);

    for(int n=0;n<ROBOTTCPNUM;n++)
    {
        QString msg="TCP: "+QString::number(n);
        ui->searchtcpcombo->addItem(msg);
    }

    ui->searchchangecheckBox->setCheckState(Qt::Unchecked);
    ui->searchchangecombo->setDisabled(true);

    adoubleValidator_speed = new QDoubleValidator(0,0,ROBOT_SPEED_DECIMAL_PLACE,this);//限制3位小数
    ui->searchspeed->setValidator(adoubleValidator_speed);
}

keysearchDlg::~keysearchDlg()
{
    delete setmovec;
    delete ui;
}

void keysearchDlg::init_dlg_show()
{
    ui->searchchangecombo->clear();
    for(int n=0;n<m_mcs->project->projecr_coord_matrix4d.size();n++)
    {
        ui->searchchangecombo->addItem(m_mcs->project->projecr_coord_matrix4d[n].name);
    }
    ui->arrive_pos->clear();
    ui->groupBox_2->setDisabled(true);
    cmd_list_in.clear();
    ui->record->clear();
}

void keysearchDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    cmd_list_in=cmdlist;
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_SEARCH_KEY)//寻位指令
        {
            float speed=cmd.cmd_search_speed;//获取到的寻位速度
            int tcp=cmd.cmd_search_tcp;//获取到寻位TCP
            Robmovemodel movemod=cmd.cmd_search_movemod;//获取到的寻位模式
            QString name=cmd.cmd_search_name;//获取到的寻位名字
            std::vector<float> sizemove=cmd.cmd_search_sidemove;
            float sidespeed=cmd.cmd_search_sidespeed;
            int side=cmd.cmd_search_side;
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
                if(change_trace_num>=0&&change_trace_num<ui->searchchangecombo->count())
                {
                    ui->searchchangecombo->setCurrentIndex(change_trace_num);
                }
                ui->searchchangecheckBox->setCheckState(Qt::Checked);
                ui->searchchangecombo->setDisabled(true);
            }
            else
            {
                ui->searchchangecheckBox->setCheckState(Qt::Unchecked);
                ui->searchchangecombo->setDisabled(false);
            }
            if(movemod==MOVEJ||movemod==MOVEL)
            {
                RobPos pos=cmd.cmd_search_pos;//获取到移动坐标
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
                ui->searchtcpcombo->setCurrentIndex(tcp);
            }
            ui->searchspeed->setText(QString::number(speed,'f',ROBOT_SPEED_DECIMAL_PLACE));
            if(movemod>=0&&movemod<=ui->searchmovemodecombo->count())
            {
                ui->searchmovemodecombo->setCurrentIndex(movemod);
            }
            ui->searchname->setText(name);
            ui->searchside->setText(QString::number(side));
            ui->searchsidespeed->setText(QString::number(sidespeed,'f',3));
            ui->searchsidemovex->setText(QString::number(sizemove[0],'f',ROBOT_POSE_DECIMAL_PLACE));
            ui->searchsidemovey->setText(QString::number(sizemove[1],'f',ROBOT_POSE_DECIMAL_PLACE));
            ui->searchsidemovez->setText(QString::number(sizemove[2],'f',ROBOT_POSE_DECIMAL_PLACE));
        }
    }
    ui->record->clear();
}

void keysearchDlg::close_dlg_show()
{

}

void keysearchDlg::setbutton(int name)
{
    if(name==0)
    {
        b_inster=false;
        ui->searchaddBtn->setText(QString::fromLocal8Bit("插入寻位指令"));
    }
    else
    {
        b_inster=true;
        ui->searchaddBtn->setText(QString::fromLocal8Bit("替换寻位指令"));
    }
}

//寻位指令
void keysearchDlg::on_searchaddBtn_clicked()
{
    int tcp=ui->searchtcpcombo->currentIndex();
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
        Robmovemodel movemodel=(Robmovemodel)ui->searchmovemodecombo->currentIndex();
        std::vector<float> sidemove(3);
        float sidespeed;
        int side;
        QString change;

        my_cmd cmd;
        QString name=ui->searchname->text();
        if(ui->searchspeed->text().isEmpty())
        {
            ui->record->append(QString::fromLocal8Bit("请填写寻位速度"));
            return;
        }
        speed=ui->searchspeed->text().toFloat(&rc);
        if(rc==false)
        {
            ui->record->append(QString::fromLocal8Bit("寻位速度格式出错"));
            return;
        }
        if(ui->searchname->text().isEmpty())
        {
            ui->record->append(QString::fromLocal8Bit("请填写寻位点名称"));
            return;
        }
        if(ui->searchsidespeed->text().isEmpty())
        {
            ui->record->append(QString::fromLocal8Bit("请填写移动速度"));
            return;
        }
        sidespeed=ui->searchsidespeed->text().toFloat(&rc);
        if(rc==false)
        {
            ui->record->append(QString::fromLocal8Bit("移动速度格式出错"));
            return;
        }
        if(ui->searchside->text().isEmpty())
        {
            ui->record->append(QString::fromLocal8Bit("请填写寻位范围"));
            return;
        }
        side=ui->searchside->text().toInt(&rc);
        if(rc==false)
        {
            ui->record->append(QString::fromLocal8Bit("寻位范围格式出错"));
            return;
        }
        if(ui->searchsidemovex->text().isEmpty())
        {
            ui->record->append(QString::fromLocal8Bit("请填写寻位范围X"));
            return;
        }
        if(ui->searchsidemovey->text().isEmpty())
        {
            ui->record->append(QString::fromLocal8Bit("请填写寻位范围Y"));
            return;
        }
        if(ui->searchsidemovez->text().isEmpty())
        {
            ui->record->append(QString::fromLocal8Bit("请填写寻位范围Z"));
            return;
        }
        sidemove[0]=ui->searchsidemovex->text().toFloat(&rc);
        if(rc==false)
        {
            ui->record->append(QString::fromLocal8Bit("寻位范围x格式出错"));
            return;
        }
        sidemove[1]=ui->searchsidemovey->text().toFloat(&rc);
        if(rc==false)
        {
            ui->record->append(QString::fromLocal8Bit("寻位范围Y格式出错"));
            return;
        }
        sidemove[2]=ui->searchsidemovez->text().toFloat(&rc);
        if(rc==false)
        {
            ui->record->append(QString::fromLocal8Bit("寻位范围Z格式出错"));
            return;
        }
        if(ui->searchchangecheckBox->isChecked()==true)
        {
            change=ui->searchchangecombo->currentText();
        }

        QString msg;
        switch(movemodel)
        {
            case MOVEL:
            case MOVEJ:
            {
                msg=cmd.cmd_search(robpos,movemodel,speed,tcp,side,sidemove,sidespeed,name,change);
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
                    msg=cmd.cmd_searchC(setmovec->pos_st,setmovec->pos_center,setmovec->pos_ed,movemodel,speed,tcp,side,sidemove,sidespeed,name,change);
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
void keysearchDlg::on_arriveBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    bool rc;
    float speed=ui->searchspeed->text().toFloat(&rc);
    my_cmd cmd;
    QString msg;
    if(ui->searchspeed->text().isEmpty())
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
    Robmovemodel movemod=cmd.cmd_search_movemod;//获取到的寻位模式
    if(movemod==MOVEJ||movemod==MOVEL)
    {
        int tcp=ui->searchtcpcombo->currentIndex();
        RobPos pos=cmd.cmd_search_pos;//获取到移动坐标
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
void keysearchDlg::on_arriveBtn_released()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    ui->record->append(QString::fromLocal8Bit("停止到位"));
}

//是否使用变换矩阵
void keysearchDlg::on_searchchangecheckBox_stateChanged(int arg1)
{
    if(arg1==0)
    {
        ui->searchchangecombo->setDisabled(true);
    }
    else
    {
        ui->searchchangecombo->setDisabled(false);
    }
}

