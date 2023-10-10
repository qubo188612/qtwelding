#include "keytracerealtime2dlg.h"
#include "ui_keytracerealtime2dlg.h"

extern QMutex send_group_robot;

keytracerealtime2Dlg::keytracerealtime2Dlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keytracerealtime2Dlg)
{
    ui->setupUi(this);

    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

    setmovec=new setmovecDlg(m_mcs);

    for(int n=0;n<ROBOTTCPNUM;n++)
    {
        QString msg="TCP: "+QString::number(n);
        ui->tracerealtime2tcpcombo->addItem(msg);
    }

    for(int n=0;n<TRACEREALTIME_EDIT_ID_TOTAL_NUM;n++)
    {
        QString msg=Tracerealtime_edit_mode_toQString((Tracerealtime_edit_mode)n);
        ui->tracerealtime2modecombo->addItem(msg);
    }

    ui->tracerealtime2changecheckBox->setCheckState(Qt::Unchecked);
    ui->tracerealtime2changecombo->setDisabled(true);

    ui->tracerealtime2weldcheckBox->setCheckState(Qt::Unchecked);
    ui->weldermodelcombo->setDisabled(true);
    ui->weldercurrent->setDisabled(true);
    ui->weldercurrent_v->setDisabled(true);

    adoubleValidator_speed = new QDoubleValidator(ROBOT_SPEED_DECIMAL_BOTTOM,ROBOT_SPEED_DECIMAL_TOP,ROBOT_SPEED_DECIMAL_PLACE,this);//限制3位小数
    ui->tracerealtime2movespeed->setValidator(adoubleValidator_speed);
}

keytracerealtime2Dlg::~keytracerealtime2Dlg()
{
    delete setmovec;
    delete adoubleValidator_speed;
    delete ui;
}

void keytracerealtime2Dlg::init_dlg_show()
{
    ui->tracerealtime2changecombo->clear();
    for(int n=0;n<m_mcs->project->project_coord_matrix4d.size();n++)
    {
        ui->tracerealtime2changecombo->addItem(m_mcs->project->project_coord_matrix4d[n].name);
    }
    ui->tracerealtime2filepathcombo->clear();
    for(int n=0;n<m_mcs->project->project_crafts_trace.size();n++)
    {
        ui->tracerealtime2filepathcombo->addItem(m_mcs->project->project_crafts_trace[n].name);
    }
    ui->arrive_pos->clear();
    ui->groupBox_2->setDisabled(true);
    cmd_list_in.clear();
    ui->record->clear();
}

void keytracerealtime2Dlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    cmd_list_in=cmdlist;
    ui->tracerealtime2changecombo->clear();
    for(int n=0;n<m_mcs->project->project_coord_matrix4d.size();n++)
    {
        ui->tracerealtime2changecombo->addItem(m_mcs->project->project_coord_matrix4d[n].name);
    }
    ui->tracerealtime2filepathcombo->clear();
    for(int n=0;n<m_mcs->project->project_crafts_trace.size();n++)
    {
        ui->tracerealtime2filepathcombo->addItem(m_mcs->project->project_crafts_trace[n].name);
    }
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_TRACEREALTIME2_KEY)//实时跟踪指令
        {
            int tcp=cmd.cmd_tracerealtime2_tcp;//获取到实时跟踪移动TCP
            float speed=cmd.cmd_tracerealtime2_speed;//获取到实时跟踪速度值
            float downspeed=cmd.cmd_tracerealtime2_downspeed;//获取到实时跟踪下枪速度值
            bool b_weld=cmd.cmd_tracerealtime2_weld;//获取到实时跟踪是否焊接起弧下标
            Tracerealtime_edit_mode mode=cmd.cmd_tracerealtime2_mode;//获取到实时跟踪下枪模式
            Robmovemodel movemod=cmd.cmd_tracerealtime2_movemod;//获取到的实时跟踪移动模式
            QString change=cmd.cmd_tracerealtime2_change;//获取到的实时跟踪变换矩阵
            int change_trace_num;//找到要变换矩阵下标
            QString craftfilepath=cmd.cmd_tracerealtime2_crafts;//获取到实时跟踪工艺包的文件路径
            int crafts_trace_num;//找到跟踪工艺下标
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
                if(change_trace_num>=0&&change_trace_num<ui->tracerealtime2changecombo->count())
                {
                    ui->tracerealtime2changecombo->setCurrentIndex(change_trace_num);
                }
                ui->tracerealtime2changecheckBox->setCheckState(Qt::Checked);
                ui->tracerealtime2changecombo->setDisabled(false);
            }
            else
            {
                ui->tracerealtime2changecheckBox->setCheckState(Qt::Unchecked);
                ui->tracerealtime2changecombo->setDisabled(true);
            }
            for(int n=0;n<m_mcs->project->project_crafts_trace.size();n++)
            {
                if(craftfilepath==m_mcs->project->project_crafts_trace[n].name)
                {
                    crafts_trace_num=n;//找到要储存的焊接轨道下标
                    break;
                }
            }
            if(crafts_trace_num>=0&&crafts_trace_num<ui->tracerealtime2filepathcombo->count())
            {
                ui->tracerealtime2filepathcombo->setCurrentIndex(crafts_trace_num);
            }
            if(movemod==MOVEJ||movemod==MOVEL)
            {
                RobPos pos=cmd.cmd_tracerealtime2_pos;//获取到移动坐标
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
                ui->tracerealtime2tcpcombo->setCurrentIndex(tcp);
            }
            if(mode>=0&&mode<ui->tracerealtime2modecombo->count())
            {
                ui->tracerealtime2modecombo->setCurrentIndex(mode);
            }
            ui->tracerealtime2movespeed->setText(QString::number(speed,'f',ROBOT_SPEED_DECIMAL_PLACE));
            ui->tracerealtime2movedownspeed->setText(QString::number(downspeed,'f',ROBOT_SPEED_DECIMAL_PLACE));
            if(movemod>=0&&movemod<ui->tracerealtime2movemodecombo->count())
            {
                ui->tracerealtime2movemodecombo->setCurrentIndex(movemod);
            }
            if(b_weld==true)
            {
                Alternatingcurrent elem=cmd.cmd_tracerealtime2_elem;  //获取到焊机交变电流模式
                float eled=cmd.cmd_tracerealtime2_eled; //获取到焊机电流
                float elev=cmd.cmd_tracerealtime2_elev; //获取到焊机电压
                if(elem>=0&&elem<=ui->weldermodelcombo->count())
                {
                    ui->weldermodelcombo->setCurrentIndex(elem);
                }
                ui->weldercurrent->setText(QString::number(eled));
                ui->weldercurrent_v->setText(QString::number(elev));
                ui->tracerealtime2weldcheckBox->setCheckState(Qt::Checked);
                ui->weldermodelcombo->setDisabled(false);
                ui->weldercurrent->setDisabled(false);
                ui->weldercurrent_v->setDisabled(false);
            }
            else
            {
                ui->tracerealtime2weldcheckBox->setCheckState(Qt::Unchecked);
                ui->weldermodelcombo->setDisabled(true);
                ui->weldercurrent->setDisabled(true);
                ui->weldercurrent_v->setDisabled(true);
            }
        }
    }
    ui->record->clear();
}

void keytracerealtime2Dlg::close_dlg_show()
{

}

void keytracerealtime2Dlg::setbutton(int name)
{
    if(name==0)
    {
        b_inster=false;
        ui->tracerealtime2addBtn->setText(QStringLiteral("插入实时跟踪指令"));
        ui->updata_posBtn->hide();
        ui->updata_posBtn_2->hide();
    }
    else
    {
        b_inster=true;
        ui->tracerealtime2addBtn->setText(QStringLiteral("替换实时跟踪指令"));
        ui->updata_posBtn->show();
        ui->updata_posBtn_2->show();
    }
}

//插入实时跟踪指令
void keytracerealtime2Dlg::on_tracerealtime2addBtn_clicked()
{
    int tcp=ui->tracerealtime2tcpcombo->currentIndex();
    if(tcp<0||tcp>ui->tracerealtime2tcpcombo->count()-1)
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
        Robmovemodel movemodel=(Robmovemodel)ui->tracerealtime2movemodecombo->currentIndex();
        Tracerealtime_edit_mode mode=(Tracerealtime_edit_mode)ui->tracerealtime2modecombo->currentIndex();
        bool rc;
        float speed=ui->tracerealtime2movespeed->text().toFloat(&rc);
        QString change;
        my_cmd cmd;
        QString msg;
        bool weld=ui->tracerealtime2weldcheckBox->isChecked();
        float eled=0;
        float elev=0;
        Alternatingcurrent elem=DIRECT;
        if(ui->tracerealtime2movespeed->text().isEmpty())
        {
            ui->record->append(QStringLiteral("请填写跟踪速度"));
            return;
        }
        if(rc==false)
        {
            ui->record->append(QStringLiteral("跟踪速度格式出错"));
            return;
        }
        float downspeed=ui->tracerealtime2movedownspeed->text().toFloat(&rc);
        if(ui->tracerealtime2movedownspeed->text().isEmpty())
        {
            ui->record->append(QStringLiteral("请填写下枪速度"));
            return;
        }
        if(rc==false)
        {
            ui->record->append(QStringLiteral("下枪速度格式出错"));
            return;
        }
        if(ui->tracerealtime2changecheckBox->isChecked()==true)
        {
            change=ui->tracerealtime2changecombo->currentText();
        }
        int route=ui->tracerealtime2filepathcombo->currentIndex();
        if(route<0||route>ui->tracerealtime2filepathcombo->count()-1)
        {
            ui->record->append(QStringLiteral("请选择要插入的工艺包"));
            return;
        }
        QString filepath=ui->tracerealtime2filepathcombo->currentText();//工艺包路径
        if(weld==true)
        {
            eled=ui->weldercurrent->text().toFloat(&rc);
            if(ui->weldercurrent->text().isEmpty())
            {
                ui->record->append(QStringLiteral("请填写电流值"));
                return;
            }
            if(rc==false)
            {
                ui->record->append(QStringLiteral("电流值格式出错"));
                return;
            }
            elev=ui->weldercurrent_v->text().toFloat(&rc);
            if(ui->weldercurrent_v->text().isEmpty())
            {
                ui->record->append(QStringLiteral("请填写电压值"));
                return;
            }
            if(rc==false)
            {
                ui->record->append(QStringLiteral("电压值格式出错"));
                return;
            }
            elem=(Alternatingcurrent)ui->weldermodelcombo->currentIndex();
        }
        switch(movemodel)
        {
            case MOVEL:
            case MOVEJ:
            {
                msg=cmd.cmd_tracerealtime2(robpos,movemodel,speed,downspeed,mode,tcp,filepath,eled,elev,elem,weld,change);
            }
            break;
            case MOVEC:
            {
                setmovec->init_dlg_show(cmd_list_in,ui->tracerealtime2changecheckBox->isChecked(),change);
                setmovec->setWindowTitle(QStringLiteral("圆弧移动设置"));
                setmovec->set_arrive_param(speed,tcp);
                int rc=setmovec->exec();
                setmovec->close_dlg_show();
                if(rc!=0)//确定
                {
                    msg=cmd.cmd_tracerealtimeC2(setmovec->pos_st,setmovec->pos_center,setmovec->pos_ed,movemodel,speed,downspeed,mode,tcp,filepath,eled,elev,elem,weld,change);
                }
                else
                {
                    ui->record->append(QStringLiteral("取消圆弧移动设置"));
                    return;
                }
            }
            break;
        }
        ui->record->append(QStringLiteral("插入移动指令成功"));
        cmd_msg=msg;
        done(1);
    }
    else
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
    }
}

//是否跟踪起弧
void keytracerealtime2Dlg::on_tracerealtime2weldcheckBox_stateChanged(int arg1)
{
    if(arg1==0)
    {
        ui->weldermodelcombo->setDisabled(true);
        ui->weldercurrent->setDisabled(true);
        ui->weldercurrent_v->setDisabled(true);
    }
    else
    {
        ui->weldermodelcombo->setDisabled(false);
        ui->weldercurrent->setDisabled(false);
        ui->weldercurrent_v->setDisabled(false);
    }
}

//是否使用变换矩阵
void keytracerealtime2Dlg::on_tracerealtime2changecheckBox_stateChanged(int arg1)
{
    if(arg1==0)
    {
        ui->tracerealtime2changecombo->setDisabled(true);
    }
    else
    {
        ui->tracerealtime2changecombo->setDisabled(false);
    }
}

//更新到当前点位
void keytracerealtime2Dlg::on_updata_posBtn_clicked()
{
    int tcp=ui->tracerealtime2tcpcombo->currentIndex();
    if(tcp<0||tcp>ui->tracerealtime2tcpcombo->count()-1)
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
        Robmovemodel movemodel=(Robmovemodel)ui->tracerealtime2movemodecombo->currentIndex();
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
void keytracerealtime2Dlg::on_updata_posBtn_2_clicked()
{
    int tcp=ui->tracerealtime2tcpcombo->currentIndex();
    if(tcp<0||tcp>ui->tracerealtime2tcpcombo->count()-1)
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
        Robmovemodel movemodel=(Robmovemodel)ui->tracerealtime2movemodecombo->currentIndex();
        if(movemodel==MOVEJ||movemodel==MOVEL)
        {
            RobPos pos=m_mcs->rob->TCPpos;
            if(ui->tracerealtime2changecheckBox->isChecked()==true)
            {
                QString name=ui->tracerealtime2changecombo->currentText();
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

//长按到点按下
void keytracerealtime2Dlg::on_arriveBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    bool rc;
    float speed=ui->tracerealtime2movespeed->text().toFloat(&rc);
    my_cmd cmd;
    QString msg;
    if(ui->tracerealtime2movespeed->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QStringLiteral("移动速度格式出错"));
        return;
    }
    QString key;
    rc=cmd.decodecmd(cmd_list_in,msg,key);
    Robmovemodel movemod=cmd.cmd_tracerealtime2_movemod;//获取到的移动模式
    if(movemod==MOVEJ||movemod==MOVEL)
    {
        int tcp=ui->tracerealtime2tcpcombo->currentIndex();
        //  RobPos pos=cmd.cmd_move_pos;//获取到移动坐标
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

//长按到点抬起
void keytracerealtime2Dlg::on_arriveBtn_released()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    ui->record->append(QStringLiteral("停止到位"));
}

//长按到变换后点按下
void keytracerealtime2Dlg::on_arriveBtn_2_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    bool rc;
    float speed=ui->tracerealtime2movespeed->text().toFloat(&rc);
    my_cmd cmd;
    QString msg;
    if(ui->tracerealtime2movespeed->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写移动速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QStringLiteral("移动速度格式出错"));
        return;
    }
    QString key;
    rc=cmd.decodecmd(cmd_list_in,msg,key);
    Robmovemodel movemod=cmd.cmd_tracerealtime2_movemod;//获取到的移动模式
    if(movemod==MOVEJ||movemod==MOVEL)
    {
        int tcp=ui->tracerealtime2tcpcombo->currentIndex();
        //  RobPos pos=cmd.cmd_move_pos;//获取到移动坐标
        RobPos pos=inster_pos;
        movemod=MOVEJ;//用关节移动方式到位
        if(ui->tracerealtime2changecheckBox->isChecked()==true)
        {
            QString name=ui->tracerealtime2changecombo->currentText();
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

//长按到变换后点抬起
void keytracerealtime2Dlg::on_arriveBtn_2_released()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    ui->record->append(QStringLiteral("停止到位"));
}

