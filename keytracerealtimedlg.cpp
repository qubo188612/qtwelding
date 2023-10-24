#include "keytracerealtimedlg.h"
#include "ui_keytracerealtimedlg.h"

extern QMutex send_group_robot;

keytracerealtimeDlg::keytracerealtimeDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keytracerealtimeDlg)
{
    ui->setupUi(this);

    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

    setmovec=new setmovecDlg(m_mcs);

//美化界面
#ifdef OPEN_BEAUTIFY_INTERFACE
    ui->groupBox->setObjectName(FONT_QUALIFY_MOUDLES_INFO);
    ui->groupBox->setStyleSheet(FONT_GROUPBOX_INFO);
    ui->groupBox_2->setObjectName(FONT_QUALIFY_MOUDLES_INFO);
    ui->groupBox_2->setStyleSheet(FONT_GROUPBOX_INFO);
    ui->groupBox_3->setObjectName(FONT_QUALIFY_MOUDLES_INFO);
    ui->groupBox_3->setStyleSheet(FONT_GROUPBOX_INFO);
    ui->arriveBtn->setStyleSheet(FONT_BUTTON_INFO);
    ui->arriveBtn_2->setStyleSheet(FONT_BUTTON_INFO);
    ui->tracerealtimeaddBtn->setStyleSheet(FONT_BUTTON_INFO);
    ui->tracerealtimechangecombo->setStyleSheet(FONT_COMBOBOX_INFO);
    ui->tracerealtimeerrdis->setStyleSheet(FONT_EDIT_INFO);
    ui->tracerealtimefilepath->setStyleSheet(FONT_EDIT_INFO);
    ui->tracerealtimefilepathBtn->setStyleSheet(FONT_BUTTON_INFO);
    ui->tracerealtimemodecombo->setStyleSheet(FONT_COMBOBOX_INFO);
    ui->tracerealtimemovedownspeed->setStyleSheet(FONT_EDIT_INFO);
    ui->tracerealtimemovemodecombo->setStyleSheet(FONT_COMBOBOX_INFO);
    ui->tracerealtimemovespeed->setStyleSheet(FONT_EDIT_INFO);
    ui->tracerealtimesamplespeed->setStyleSheet(FONT_EDIT_INFO);
    ui->tracerealtimesampletime->setStyleSheet(FONT_EDIT_INFO);
    ui->tracerealtimetcpcombo->setStyleSheet(FONT_COMBOBOX_INFO);
    ui->updata_posBtn->setStyleSheet(FONT_BUTTON_INFO);
    ui->updata_posBtn_2->setStyleSheet(FONT_BUTTON_INFO);
    ui->weldercurrent->setStyleSheet(FONT_EDIT_INFO);
    ui->weldercurrent_v->setStyleSheet(FONT_EDIT_INFO);
    ui->weldermodelcombo->setStyleSheet(FONT_COMBOBOX_INFO);
    ui->record->setStyleSheet(FONT_TEXTBROWERS_INFO);
#endif

    for(int n=0;n<ROBOTTCPNUM;n++)
    {
        QString msg="TCP: "+QString::number(n);
        ui->tracerealtimetcpcombo->addItem(msg);
    }

    for(int n=0;n<TRACEREALTIME_EDIT_ID_TOTAL_NUM;n++)
    {
        QString msg=Tracerealtime_edit_mode_toQString((Tracerealtime_edit_mode)n);
        ui->tracerealtimemodecombo->addItem(msg);
    }

    ui->tracerealtimechangecheckBox->setCheckState(Qt::Unchecked);
    ui->tracerealtimechangecombo->setDisabled(true);

    ui->tracerealtimeweldcheckBox->setCheckState(Qt::Unchecked);
    ui->weldermodelcombo->setDisabled(true);
    ui->weldercurrent->setDisabled(true);
    ui->weldercurrent_v->setDisabled(true);

    adoubleValidator_speed = new QDoubleValidator(ROBOT_SPEED_DECIMAL_BOTTOM,ROBOT_SPEED_DECIMAL_TOP,ROBOT_SPEED_DECIMAL_PLACE,this);//限制3位小数
    ui->tracerealtimemovespeed->setValidator(adoubleValidator_speed);
}

keytracerealtimeDlg::~keytracerealtimeDlg()
{
    delete setmovec;
    delete adoubleValidator_speed;
    delete ui;
}

void keytracerealtimeDlg::init_dlg_show()
{
    ui->tracerealtimechangecombo->clear();
    for(int n=0;n<m_mcs->project->project_coord_matrix4d.size();n++)
    {
        ui->tracerealtimechangecombo->addItem(m_mcs->project->project_coord_matrix4d[n].name);
    }
    ui->arrive_pos->clear();
    ui->groupBox_2->setDisabled(true);
    cmd_list_in.clear();
    ui->record->clear();
}

void keytracerealtimeDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    cmd_list_in=cmdlist;
    ui->tracerealtimechangecombo->clear();
    for(int n=0;n<m_mcs->project->project_coord_matrix4d.size();n++)
    {
        ui->tracerealtimechangecombo->addItem(m_mcs->project->project_coord_matrix4d[n].name);
    }
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_TRACEREALTIME_KEY)//实时跟踪指令
        {
            int tcp=cmd.cmd_tracerealtime_tcp;//获取到实时跟踪移动TCP
            float speed=cmd.cmd_tracerealtime_speed;//获取到实时跟踪速度值
            float downspeed=cmd.cmd_tracerealtime_downspeed;//获取到实时跟踪下枪速度值
            bool b_weld=cmd.cmd_tracerealtime_weld;//获取到实时跟踪是否焊接起弧下标
            int time=cmd.cmd_tracerealtime_time;
            float samplespeed=cmd.cmd_tracerealtime2_samplespeed;
            float errdis=cmd.cmd_tracerealtime_errdis;
            Tracerealtime_edit_mode mode=cmd.cmd_tracerealtime_mode;//获取到实时跟踪下枪模式
            Robmovemodel movemod=cmd.cmd_tracerealtime_movemod;//获取到的实时跟踪移动模式
            QString change=cmd.cmd_tracerealtime_change;//获取到的实时跟踪变换矩阵
            int change_trace_num;//找到要变换矩阵下标
            QString craftfilepath=cmd.cmd_tracerealtime_craftfilepath;//获取到实时跟踪工艺包的文件路径
            ui->tracerealtimefilepath->setText(craftfilepath);
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
                if(change_trace_num>=0&&change_trace_num<ui->tracerealtimechangecombo->count())
                {
                    ui->tracerealtimechangecombo->setCurrentIndex(change_trace_num);
                }
                ui->tracerealtimechangecheckBox->setCheckState(Qt::Checked);
                ui->tracerealtimechangecombo->setDisabled(false);
            }
            else
            {
                ui->tracerealtimechangecheckBox->setCheckState(Qt::Unchecked);
                ui->tracerealtimechangecombo->setDisabled(true);
            }
            if(movemod==MOVEJ||movemod==MOVEL)
            {
                RobPos pos=cmd.cmd_tracerealtime_pos;//获取到移动坐标
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
                ui->tracerealtimetcpcombo->setCurrentIndex(tcp);
            }
            if(mode>=0&&mode<ui->tracerealtimemodecombo->count())
            {
                ui->tracerealtimemodecombo->setCurrentIndex(mode);
            }
            ui->tracerealtimemovespeed->setText(QString::number(speed,'f',ROBOT_SPEED_DECIMAL_PLACE));
            ui->tracerealtimemovedownspeed->setText(QString::number(downspeed,'f',ROBOT_SPEED_DECIMAL_PLACE));
            if(movemod>=0&&movemod<ui->tracerealtimemovemodecombo->count())
            {
                ui->tracerealtimemovemodecombo->setCurrentIndex(movemod);
            }
            ui->tracerealtimesampletime->setText(QString::number(time));
            ui->tracerealtimesamplespeed->setText(QString::number(samplespeed,'f',ROBOT_SPEED_DECIMAL_PLACE));
            ui->tracerealtimeerrdis->setText(QString::number(errdis,'f',ROBOT_POSE_DECIMAL_PLACE));
            if(b_weld==true)
            {
                Alternatingcurrent elem=cmd.cmd_tracerealtime_elem;  //获取到焊机交变电流模式
                float eled=cmd.cmd_tracerealtime_eled; //获取到焊机电流
                float elev=cmd.cmd_tracerealtime_elev; //获取到焊机电压
                if(elem>=0&&elem<=ui->weldermodelcombo->count())
                {
                    ui->weldermodelcombo->setCurrentIndex(elem);
                }
                ui->weldercurrent->setText(QString::number(eled));
                ui->weldercurrent_v->setText(QString::number(elev));
                ui->tracerealtimeweldcheckBox->setCheckState(Qt::Checked);
                ui->weldermodelcombo->setDisabled(false);
                ui->weldercurrent->setDisabled(false);
                ui->weldercurrent_v->setDisabled(false);
            }
            else
            {
                ui->tracerealtimeweldcheckBox->setCheckState(Qt::Unchecked);
                ui->weldermodelcombo->setDisabled(true);
                ui->weldercurrent->setDisabled(true);
                ui->weldercurrent_v->setDisabled(true);
            }
        }
    }
    ui->record->clear();
}

void keytracerealtimeDlg::close_dlg_show()
{

}

void keytracerealtimeDlg::setbutton(int name)
{
    if(name==0)
    {
        b_inster=false;
        ui->tracerealtimeaddBtn->setText(QStringLiteral("插入实时跟踪指令"));
        ui->updata_posBtn->hide();
        ui->updata_posBtn_2->hide();
    }
    else
    {
        b_inster=true;
        ui->tracerealtimeaddBtn->setText(QStringLiteral("替换实时跟踪指令"));
        ui->updata_posBtn->show();
        ui->updata_posBtn_2->show();
    }
}

//插入实时跟踪指令
void keytracerealtimeDlg::on_tracerealtimeaddBtn_clicked()
{
    int tcp=ui->tracerealtimetcpcombo->currentIndex();

    if(tcp<0||tcp>ui->tracerealtimetcpcombo->count()-1)
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
        Robmovemodel movemodel=(Robmovemodel)ui->tracerealtimemovemodecombo->currentIndex();
        Tracerealtime_edit_mode mode=(Tracerealtime_edit_mode)ui->tracerealtimemodecombo->currentIndex();
        bool rc;
        float speed=ui->tracerealtimemovespeed->text().toFloat(&rc);
        QString change;
        my_cmd cmd;
        QString msg;
        bool weld=ui->tracerealtimeweldcheckBox->isChecked();
        float eled=0;
        float elev=0;
        Alternatingcurrent elem=DIRECT;
        if(ui->tracerealtimemovespeed->text().isEmpty())
        {
            ui->record->append(QStringLiteral("请填写跟踪速度"));
            return;
        }
        if(rc==false)
        {
            ui->record->append(QStringLiteral("跟踪速度格式出错"));
            return;
        }
        float downspeed=ui->tracerealtimemovedownspeed->text().toFloat(&rc);
        if(ui->tracerealtimemovedownspeed->text().isEmpty())
        {
            ui->record->append(QStringLiteral("请填写下枪速度"));
            return;
        }
        if(rc==false)
        {
            ui->record->append(QStringLiteral("下枪速度格式出错"));
            return;
        }
        if(ui->tracerealtimechangecheckBox->isChecked()==true)
        {
            change=ui->tracerealtimechangecombo->currentText();
        }
        QString filepath=ui->tracerealtimefilepath->text();//工艺包路径
        //请选择工艺包路径
        if(filepath.size()==0)
        {
            ui->record->append(QStringLiteral("工艺包路径为空"));
            return;
        }
        else
        {
            //这里判断工艺包是否格式正确
            int rc=m_mcs->craft->LoadCraft(filepath);
            if(rc==1)
            {
                ui->record->append(QStringLiteral("该路径找不到工艺包文件"));
                return;
            }
            else if(rc==2)
            {
                ui->record->append(QStringLiteral("工艺包文件格式错误"));
                return;
            }
        }
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
        int time=ui->tracerealtimesampletime->text().toInt(&rc);
        if(ui->tracerealtimesampletime->text().isEmpty())
        {
            ui->record->append(QStringLiteral("请填写采样点发送间隔时间"));
            return;
        }
        if(rc==false)
        {
            ui->record->append(QStringLiteral("采样点发送间隔时间格式出错"));
            return;
        }
        float samplespeed=ui->tracerealtimesamplespeed->text().toFloat(&rc);
        if(ui->tracerealtimesamplespeed->text().isEmpty())
        {
            ui->record->append(QStringLiteral("请填写采样速度"));
            return;
        }
        if(rc==false)
        {
            ui->record->append(QStringLiteral("采样速度格式出错"));
            return;
        }
        float errdis=ui->tracerealtimeerrdis->text().toFloat(&rc);
        if(ui->tracerealtimeerrdis->text().isEmpty())
        {
            ui->record->append(QStringLiteral("请填写容错长度"));
            return;
        }
        if(rc==false)
        {
            ui->record->append(QStringLiteral("容错长度格式出错"));
            return;
        }
        switch(movemodel)
        {
            case MOVEL:
            case MOVEJ:
            {
                msg=cmd.cmd_tracerealtime(robpos,movemodel,speed,downspeed,mode,tcp,time,samplespeed,errdis,filepath,eled,elev,elem,weld,change);
            }
            break;
            case MOVEC:
            {
                setmovec->init_dlg_show(cmd_list_in,ui->tracerealtimechangecheckBox->isChecked(),change);
                setmovec->setWindowTitle(QStringLiteral("圆弧移动设置"));
                setmovec->set_arrive_param(speed,tcp);
                int rc=setmovec->exec();
                setmovec->close_dlg_show();
                if(rc!=0)//确定
                {
                    msg=cmd.cmd_tracerealtimeC(setmovec->pos_st,setmovec->pos_center,setmovec->pos_ed,movemodel,speed,downspeed,mode,tcp,time,samplespeed,errdis,filepath,eled,elev,elem,weld,change);
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
void keytracerealtimeDlg::on_tracerealtimeweldcheckBox_stateChanged(int arg1)
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
void keytracerealtimeDlg::on_tracerealtimechangecheckBox_stateChanged(int arg1)
{
    if(arg1==0)
    {
        ui->tracerealtimechangecombo->setDisabled(true);
    }
    else
    {
        ui->tracerealtimechangecombo->setDisabled(false);
    }
}

//修改路径
void keytracerealtimeDlg::on_tracerealtimefilepathBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, QStringLiteral("请选择要导入的焊接工艺文件"), "./CRAFT/", "CRAFT(*.craft)");
    if(fileName.toStdString().size()>0)
    {
        if(0!=m_mcs->craft->LoadCraft(fileName))
        {
            ui->record->append(QStringLiteral("工艺文件读取失败"));
            return;
        }
        ui->record->append(QStringLiteral("工艺文件读取成功"));
        ui->tracerealtimefilepath->setText(fileName);
    }
}

//更新到当前点位
void keytracerealtimeDlg::on_updata_posBtn_clicked()
{
    int tcp=ui->tracerealtimetcpcombo->currentIndex();
    if(tcp<0||tcp>ui->tracerealtimetcpcombo->count()-1)
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
        Robmovemodel movemodel=(Robmovemodel)ui->tracerealtimemovemodecombo->currentIndex();
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
void keytracerealtimeDlg::on_updata_posBtn_2_clicked()
{
    int tcp=ui->tracerealtimetcpcombo->currentIndex();
    if(tcp<0||tcp>ui->tracerealtimetcpcombo->count()-1)
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
        Robmovemodel movemodel=(Robmovemodel)ui->tracerealtimemovemodecombo->currentIndex();
        if(movemodel==MOVEJ||movemodel==MOVEL)
        {
            RobPos pos=m_mcs->rob->TCPpos;
            if(ui->tracerealtimechangecheckBox->isChecked()==true)
            {
                QString name=ui->tracerealtimechangecombo->currentText();
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
void keytracerealtimeDlg::on_arriveBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    bool rc;
    float speed=ui->tracerealtimemovespeed->text().toFloat(&rc);
    my_cmd cmd;
    QString msg;
    if(ui->tracerealtimemovespeed->text().isEmpty())
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
    Robmovemodel movemod=cmd.cmd_tracerealtime_movemod;//获取到的移动模式
    if(movemod==MOVEJ||movemod==MOVEL)
    {
        int tcp=ui->tracerealtimetcpcombo->currentIndex();
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
void keytracerealtimeDlg::on_arriveBtn_released()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    ui->record->append(QStringLiteral("停止到位"));
}

//长按到变化后点按下
void keytracerealtimeDlg::on_arriveBtn_2_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    bool rc;
    float speed=ui->tracerealtimemovespeed->text().toFloat(&rc);
    my_cmd cmd;
    QString msg;
    if(ui->tracerealtimemovespeed->text().isEmpty())
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
    Robmovemodel movemod=cmd.cmd_tracerealtime_movemod;//获取到的移动模式
    if(movemod==MOVEJ||movemod==MOVEL)
    {
        int tcp=ui->tracerealtimetcpcombo->currentIndex();
        //  RobPos pos=cmd.cmd_move_pos;//获取到移动坐标
        RobPos pos=inster_pos;
        movemod=MOVEJ;//用关节移动方式到位
        if(ui->tracerealtimechangecheckBox->isChecked()==true)
        {
            QString name=ui->tracerealtimechangecombo->currentText();
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

//长按到变化后点抬起
void keytracerealtimeDlg::on_arriveBtn_2_released()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    ui->record->append(QStringLiteral("停止到位"));
}

