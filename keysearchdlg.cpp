#include "keysearchdlg.h"
#include "ui_keysearchdlg.h"

extern QMutex send_group_robot;

keysearchDlg::keysearchDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keysearchDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

//美化界面
#ifdef OPEN_BEAUTIFY_INTERFACE
    ui->arriveBtn->setStyleSheet(FONT_BUTTON_INFO);
    ui->arriveBtn_2->setStyleSheet(FONT_BUTTON_INFO);
    ui->groupBox->setObjectName(FONT_QUALIFY_MOUDLES_INFO);
    ui->groupBox->setStyleSheet(FONT_GROUPBOX_INFO);
    ui->groupBox_2->setObjectName(FONT_QUALIFY_MOUDLES_INFO);
    ui->groupBox_2->setStyleSheet(FONT_GROUPBOX_INFO);
    ui->groupBox_3->setObjectName(FONT_QUALIFY_MOUDLES_INFO);
    ui->groupBox_3->setStyleSheet(FONT_GROUPBOX_INFO);
    ui->searchaddBtn->setStyleSheet(FONT_BUTTON_INFO);
    ui->searchchangecombo->setStyleSheet(FONT_COMBOBOX_INFO);
    ui->searchmovemodecombo->setStyleSheet(FONT_COMBOBOX_INFO);
    ui->searchname->setStyleSheet(FONT_EDIT_INFO);
    ui->searchside->setStyleSheet(FONT_EDIT_INFO);
    ui->searchsidemovex->setStyleSheet(FONT_EDIT_INFO);
    ui->searchsidemovey->setStyleSheet(FONT_EDIT_INFO);
    ui->searchsidemovez->setStyleSheet(FONT_EDIT_INFO);
    ui->searchsidespeed->setStyleSheet(FONT_EDIT_INFO);
    ui->searchspeed->setStyleSheet(FONT_EDIT_INFO);
    ui->searchtcpcombo->setStyleSheet(FONT_COMBOBOX_INFO);
    ui->updata_posBtn->setStyleSheet(FONT_BUTTON_INFO);
    ui->updata_posBtn_2->setStyleSheet(FONT_BUTTON_INFO);
    ui->record->setStyleSheet(FONT_TEXTBROWERS_INFO);
#endif

    setmovec=new setmovecDlg(m_mcs);

    for(int n=0;n<ROBOTTCPNUM;n++)
    {
        QString msg="TCP: "+QString::number(n);
        ui->searchtcpcombo->addItem(msg);
    }

    ui->searchchangecheckBox->setCheckState(Qt::Unchecked);
    ui->searchchangecombo->setDisabled(true);

    adoubleValidator_speed = new QDoubleValidator(ROBOT_SPEED_DECIMAL_BOTTOM,ROBOT_SPEED_DECIMAL_TOP,ROBOT_SPEED_DECIMAL_PLACE,this);//限制3位小数
    ui->searchspeed->setValidator(adoubleValidator_speed);
    ui->searchsidespeed->setValidator(adoubleValidator_speed);
}

keysearchDlg::~keysearchDlg()
{
    delete setmovec;
    delete adoubleValidator_speed;
    delete ui;
}

void keysearchDlg::init_dlg_show()
{
    ui->searchchangecombo->clear();
    for(int n=0;n<m_mcs->project->project_coord_matrix4d.size();n++)
    {
        ui->searchchangecombo->addItem(m_mcs->project->project_coord_matrix4d[n].name);
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
            QString change=cmd.cmd_search_change;//获取到的变换矩阵
            int change_trace_num;//找到要变换矩阵下标
            if(!change.isEmpty())
            {
                for(int n=0;n<m_mcs->project->project_coord_matrix4d.size();n++)
                {
                    if(change==m_mcs->project->project_coord_matrix4d[n].name)
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
                ui->searchchangecombo->setDisabled(false);
            }
            else
            {
                ui->searchchangecheckBox->setCheckState(Qt::Unchecked);
                ui->searchchangecombo->setDisabled(true);
            }
            if(movemod==MOVEJ||movemod==MOVEL)
            {
                RobPos pos=cmd.cmd_search_pos;//获取到移动坐标
                inster_pos=pos;
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
        ui->searchaddBtn->setText(QStringLiteral("插入寻位指令"));
        ui->updata_posBtn->hide();
        ui->updata_posBtn_2->hide();
    }
    else
    {
        b_inster=true;
        ui->searchaddBtn->setText(QStringLiteral("替换寻位指令"));
        ui->updata_posBtn->show();
        ui->updata_posBtn_2->show();
    }
}

//寻位指令
void keysearchDlg::on_searchaddBtn_clicked()
{
    int tcp=ui->searchtcpcombo->currentIndex();
    if(tcp<0||tcp>ui->searchtcpcombo->count()-1)
    {
        ui->record->append(QStringLiteral("请选择一个tcp"));
        return;
    }
    if(m_mcs->rob->b_link_ctx_posget==true)
    {
        RobPos robpos;
        if(b_inster==false)
        {
            send_group_robot.lock();
            sent_info_robot sendrob;
            sendrob.addr=ROB_TCP_NUM_REG_ADD;
            sendrob.ctx=m_mcs->rob->ctx_posget;
            sendrob.data.resize(1);
            sendrob.data[0]=tcp;
            m_mcs->rob->b_send_group_robot=false;
            m_mcs->rob->send_group_robot.push_back(sendrob);
            m_mcs->rob->ctx_robot_dosomeing=DO_WRITE_TASK;
            send_group_robot.unlock();
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
                ui->record->append(QStringLiteral("机器人TCP设置异常"));
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
                ui->record->append(QStringLiteral("获取机器人坐标失败"));
                return;
            }
            robpos=m_mcs->rob->TCPpos;
        }
        else
        {
            robpos=inster_pos;
        }
        bool rc;
        float speed;
        Robmovemodel movemodel=(Robmovemodel)ui->searchmovemodecombo->currentIndex();
        std::vector<float> sidemove(3);
        float sidespeed;
        int side;
        QString change;

        my_cmd cmd;
        QString name=ui->searchname->text();
        if(ui->searchspeed->text().isEmpty())
        {
            ui->record->append(QStringLiteral("请填写寻位速度"));
            return;
        }
        speed=ui->searchspeed->text().toFloat(&rc);
        if(rc==false)
        {
            ui->record->append(QStringLiteral("寻位速度格式出错"));
            return;
        }
        if(ui->searchname->text().isEmpty())
        {
            ui->record->append(QStringLiteral("请填写寻位点名称"));
            return;
        }
        if(ui->searchsidespeed->text().isEmpty())
        {
            ui->record->append(QStringLiteral("请填写移动速度"));
            return;
        }
        sidespeed=ui->searchsidespeed->text().toFloat(&rc);
        if(rc==false)
        {
            ui->record->append(QStringLiteral("移动速度格式出错"));
            return;
        }
        if(ui->searchside->text().isEmpty())
        {
            ui->record->append(QStringLiteral("请填写寻位范围"));
            return;
        }
        side=ui->searchside->text().toInt(&rc);
        if(rc==false)
        {
            ui->record->append(QStringLiteral("寻位范围格式出错"));
            return;
        }
        if(ui->searchsidemovex->text().isEmpty())
        {
            ui->record->append(QStringLiteral("请填写寻位范围X"));
            return;
        }
        if(ui->searchsidemovey->text().isEmpty())
        {
            ui->record->append(QStringLiteral("请填写寻位范围Y"));
            return;
        }
        if(ui->searchsidemovez->text().isEmpty())
        {
            ui->record->append(QStringLiteral("请填写寻位范围Z"));
            return;
        }
        sidemove[0]=ui->searchsidemovex->text().toFloat(&rc);
        if(rc==false)
        {
            ui->record->append(QStringLiteral("寻位范围x格式出错"));
            return;
        }
        sidemove[1]=ui->searchsidemovey->text().toFloat(&rc);
        if(rc==false)
        {
            ui->record->append(QStringLiteral("寻位范围Y格式出错"));
            return;
        }
        sidemove[2]=ui->searchsidemovez->text().toFloat(&rc);
        if(rc==false)
        {
            ui->record->append(QStringLiteral("寻位范围Z格式出错"));
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
                setmovec->init_dlg_show(cmd_list_in,ui->searchchangecheckBox->isChecked(),change);
                setmovec->setWindowTitle(QStringLiteral("圆弧寻位设置"));
                setmovec->set_arrive_param(speed,tcp);
                int rc=setmovec->exec();
                setmovec->close_dlg_show();
                if(rc!=0)//确定
                {
                    msg=cmd.cmd_searchC(setmovec->pos_st,setmovec->pos_center,setmovec->pos_ed,movemodel,speed,tcp,side,sidemove,sidespeed,name,change);
                }
                else
                {
                    ui->record->append(QStringLiteral("取消圆弧寻位设置"));
                    return;
                }
            }
            break;
        }
        if(b_inster==false)//插入
        {
            std::vector<QString> err_msg;
            m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg);
            for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
            {
                if(name==m_mcs->project->project_robpos_trace[n].name)
                {
                    ui->record->append(QStringLiteral("寻位点与已有的点重名"));
                    return;
                }
            }
        }
        ui->record->append(QStringLiteral("插入寻位指令成功"));
        cmd_msg=msg;
        done(1);
    }
    else
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
    }
}

//长按到点按下
void keysearchDlg::on_arriveBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    bool rc;
    float speed=ui->searchspeed->text().toFloat(&rc);
    my_cmd cmd;
    QString msg;
    if(ui->searchspeed->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写寻位速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QStringLiteral("寻位速度格式出错"));
        return;
    }
    QString key;
    rc=cmd.decodecmd(cmd_list_in,msg,key);
    Robmovemodel movemod=cmd.cmd_search_movemod;//获取到的寻位模式
    if(movemod==MOVEJ||movemod==MOVEL)
    {
        int tcp=ui->searchtcpcombo->currentIndex();
    //  RobPos pos=cmd.cmd_search_pos;//获取到移动坐标
        RobPos pos=inster_pos;
        movemod=MOVEJ;//用关节移动方式到位
        m_mcs->tosendbuffer->cmd_move(pos,movemod,speed,tcp);//移动
    }
    else
    {
        ui->record->append(QStringLiteral("只有MOVEJ和MOVEL的点可以到位运动"));
        return;
    }
    ui->record->append(QStringLiteral("开始到位中..."));
}

//长按到点松开
void keysearchDlg::on_arriveBtn_released()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    ui->record->append(QStringLiteral("停止到位"));
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

//更新到当前位置点
void keysearchDlg::on_updata_posBtn_clicked()
{
    int tcp=ui->searchtcpcombo->currentIndex();
    if(tcp<0||tcp>ui->searchtcpcombo->count()-1)
    {
        ui->record->append(QStringLiteral("请选择一个tcp"));
        return;
    }
    if(m_mcs->rob->b_link_ctx_posget==true)
    {
        send_group_robot.lock();
        sent_info_robot sendrob;
        sendrob.addr=ROB_TCP_NUM_REG_ADD;
        sendrob.ctx=m_mcs->rob->ctx_posget;
        sendrob.data.resize(1);
        sendrob.data[0]=tcp;
        m_mcs->rob->b_send_group_robot=false;
        m_mcs->rob->send_group_robot.push_back(sendrob);
        m_mcs->rob->ctx_robot_dosomeing=DO_WRITE_TASK;
        send_group_robot.unlock();
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
            ui->record->append(QStringLiteral("机器人TCP设置异常"));
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
            ui->record->append(QStringLiteral("获取机器人坐标失败"));
            return;
        }
        Robmovemodel movemodel=(Robmovemodel)ui->searchmovemodecombo->currentIndex();
        if(movemodel==MOVEJ||movemodel==MOVEL)
        {
            RobPos pos=m_mcs->rob->TCPpos;
            inster_pos=pos;
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
        }
        else
        {
            ui->record->append(QStringLiteral("只有MOVEJ和MOVEL的点可以直接更新到当前点"));
            return;
        }
    }
    else
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
    }
}

//更新到变换前点位
void keysearchDlg::on_updata_posBtn_2_clicked()
{
    int tcp=ui->searchtcpcombo->currentIndex();
    if(tcp<0||tcp>ui->searchtcpcombo->count()-1)
    {
        ui->record->append(QStringLiteral("请选择一个tcp"));
        return;
    }
    if(m_mcs->rob->b_link_ctx_posget==true)
    {
        send_group_robot.lock();
        sent_info_robot sendrob;
        sendrob.addr=ROB_TCP_NUM_REG_ADD;
        sendrob.ctx=m_mcs->rob->ctx_posget;
        sendrob.data.resize(1);
        sendrob.data[0]=tcp;
        m_mcs->rob->b_send_group_robot=false;
        m_mcs->rob->send_group_robot.push_back(sendrob);
        m_mcs->rob->ctx_robot_dosomeing=DO_WRITE_TASK;
        send_group_robot.unlock();
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
            ui->record->append(QStringLiteral("机器人TCP设置异常"));
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
            ui->record->append(QStringLiteral("获取机器人坐标失败"));
            return;
        }
        Robmovemodel movemodel=(Robmovemodel)ui->searchmovemodecombo->currentIndex();
        if(movemodel==MOVEJ||movemodel==MOVEL)
        {
            RobPos pos=m_mcs->rob->TCPpos;
            if(ui->searchchangecheckBox->isChecked()==true)
            {
                QString name=ui->searchchangecombo->currentText();
                if(name.isEmpty()==false)
                {
                    int matrix4d_trace_num;
                    bool b_find=false;
                    m_mcs->process->check_data_process();
                    for(int n=0;n<m_mcs->project->project_coord_matrix4d.size();n++)
                    {
                        if(name==m_mcs->project->project_coord_matrix4d[n].name)
                        {
                            matrix4d_trace_num=n;
                            b_find=true;
                            break;
                        }
                    }
                    if(b_find==false)
                    {
                        ui->record->append(QStringLiteral("代码中没有该旋转矩阵"));
                        return;
                    }
                    else if(m_mcs->project->project_coord_matrix4d[matrix4d_trace_num].nEn==false)
                    {
                        ui->record->append(QStringLiteral("该旋转矩阵还没有有效值"));
                        return;
                    }
                    else
                    {
                        Eigen::Matrix3d R1;          //旋转矩阵
                        Eigen::Vector3d T1;          //平移矩阵(零点坐标)
                        Eigen::Matrix3d R;           //旋转矩阵
                        Eigen::Vector3d T;           //平移矩阵(零点坐标)
                        R1=m_mcs->project->project_coord_matrix4d[matrix4d_trace_num].R1;
                        T1=m_mcs->project->project_coord_matrix4d[matrix4d_trace_num].T1;
                        R=m_mcs->project->project_coord_matrix4d[matrix4d_trace_num].R;
                        T=m_mcs->project->project_coord_matrix4d[matrix4d_trace_num].T;

                        Eigen::Vector3d tempin,tempout;
                        Eigen::Matrix3d nR1=R1.inverse(),nR=R.inverse();
                        tempin.x()=pos.X;
                        tempin.y()=pos.Y;
                        tempin.z()=pos.Z;
                        tempout=nR1*nR*(tempin-T)-T1;
                        pos.X=tempout.x();
                        pos.Y=tempout.y();
                        pos.Z=tempout.z();
                        /************/
                        //姿态自适应变化
                        std::array<double,3> posture={pos.RX,pos.RY,pos.RZ};
                        std::array<double,3> posture_map=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,nR,posture);
                        std::array<double,3> posture_out=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,nR1,posture_map);
                        pos.RX=posture_out[0];
                        pos.RY=posture_out[1];
                        pos.RZ=posture_out[2];
                        /************/
                    }
                }
                else
                {
                    ui->record->append(QStringLiteral("请先选择要使用的变换矩阵"));
                    return;
                }
            }
            else
            {
                ui->record->append(QStringLiteral("请先勾选使用变换矩阵"));
                return;
            }
            inster_pos=pos;
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
        }
        else
        {
            ui->record->append(QStringLiteral("只有MOVEJ和MOVEL的点可以直接更新到当前点"));
            return;
        }
    }
    else
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
    }
}

//长按到变换后点
void keysearchDlg::on_arriveBtn_2_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    bool rc;
    float speed=ui->searchspeed->text().toFloat(&rc);
    my_cmd cmd;
    QString msg;
    if(ui->searchspeed->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写寻位速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QStringLiteral("寻位速度格式出错"));
        return;
    }
    QString key;
    rc=cmd.decodecmd(cmd_list_in,msg,key);
    Robmovemodel movemod=cmd.cmd_search_movemod;//获取到的寻位模式
    if(movemod==MOVEJ||movemod==MOVEL)
    {
        int tcp=ui->searchtcpcombo->currentIndex();
    //  RobPos pos=cmd.cmd_search_pos;//获取到移动坐标
        RobPos pos=inster_pos;
        movemod=MOVEJ;//用关节移动方式到位
        if(ui->searchchangecheckBox->isChecked()==true)
        {
            QString name=ui->searchchangecombo->currentText();
            if(name.isEmpty()==false)
            {
                int matrix4d_trace_num;
                bool b_find=false;
                m_mcs->process->check_data_process();
                for(int n=0;n<m_mcs->project->project_coord_matrix4d.size();n++)
                {
                    if(name==m_mcs->project->project_coord_matrix4d[n].name)
                    {
                        matrix4d_trace_num=n;
                        b_find=true;
                        break;
                    }
                }
                if(b_find==false)
                {
                    ui->record->append(QStringLiteral("代码中没有该旋转矩阵"));
                    return;
                }
                else if(m_mcs->project->project_coord_matrix4d[matrix4d_trace_num].nEn==false)
                {
                    ui->record->append(QStringLiteral("该旋转矩阵还没有有效值"));
                    return;
                }
                else
                {
                    Eigen::Matrix3d R1;          //旋转矩阵
                    Eigen::Vector3d T1;          //平移矩阵(零点坐标)
                    Eigen::Matrix3d R;          //旋转矩阵
                    Eigen::Vector3d T;          //平移矩阵(零点坐标)
                    R1=m_mcs->project->project_coord_matrix4d[matrix4d_trace_num].R1;
                    T1=m_mcs->project->project_coord_matrix4d[matrix4d_trace_num].T1;
                    R=m_mcs->project->project_coord_matrix4d[matrix4d_trace_num].R;
                    T=m_mcs->project->project_coord_matrix4d[matrix4d_trace_num].T;
                    Eigen::Vector3d tempin,tempout;
                    tempin.x()=pos.X;
                    tempin.y()=pos.Y;
                    tempin.z()=pos.Z;
                    tempout=CCoordChange::_1point2point(tempin,R1,T1);
                    tempout=CCoordChange::point2point(tempout,R,T);
                    pos.X=tempout.x();
                    pos.Y=tempout.y();
                    pos.Z=tempout.z();
                    /************/
                    //姿态自适应变化
                    std::array<double,3> posture={pos.RX,pos.RY,pos.RZ};
                    std::array<double,3> posture_map=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R1,posture);
                    std::array<double,3> posture_out=Calibration::Attitudeangleroation(m_mcs->rob->cal_posture_model,R,posture_map);
                    pos.RX=posture_out[0];
                    pos.RY=posture_out[1];
                    pos.RZ=posture_out[2];
                    /************/
                }
            }
            else
            {
                ui->record->append(QStringLiteral("请先勾选使用变换矩阵"));
                return;
            }
        }
        else
        {
            ui->record->append(QStringLiteral("请先勾选使用变换矩阵"));
            return;
        }
        m_mcs->tosendbuffer->cmd_move(pos,movemod,speed,tcp);//移动
    }
    else
    {
        ui->record->append(QStringLiteral("只有MOVEJ和MOVEL的点可以到位运动"));
        return;
    }
    ui->record->append(QStringLiteral("开始到位中..."));
}

//长按抬起
void keysearchDlg::on_arriveBtn_2_released()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    ui->record->append(QStringLiteral("停止到位"));
}

