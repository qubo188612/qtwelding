#include "keytracecontinuedlg.h"
#include "ui_keytracecontinuedlg.h"

extern QMutex send_group_robot;

keytracecontinueDlg::keytracecontinueDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keytracecontinueDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

//美化界面
#ifdef OPEN_BEAUTIFY_INTERFACE
    ui->groupBox_2->setObjectName(FONT_QUALIFY_MOUDLES_INFO);
    ui->groupBox_2->setStyleSheet(FONT_GROUPBOX_INFO);
    ui->arriveBtn->setStyleSheet(FONT_BUTTON_INFO);
    ui->tracecontinuename->setStyleSheet(FONT_EDIT_INFO);
    ui->tracecontinuenamecombo->setStyleSheet(FONT_COMBOBOX_INFO);
    ui->updata_posBtn->setStyleSheet(FONT_BUTTON_INFO);
    ui->pushButton->setStyleSheet(FONT_BUTTON_INFO);
    ui->record->setStyleSheet(FONT_TEXTBROWERS_INFO);
#endif
}

keytracecontinueDlg::~keytracecontinueDlg()
{
    delete ui;
}

void keytracecontinueDlg::init_dlg_show()
{
    cmd_list_in.clear();
    ui->record->clear();
    ui->arrive_pos->clear();
    ui->groupBox_2->setDisabled(true);
    ui->tracecontinuenamecombo->clear();
    for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
    {
        ui->tracecontinuenamecombo->addItem(m_mcs->project->project_interweld_trace[n].name);
    }
}

void keytracecontinueDlg::init_dlg_show(QString cmdlist)
{
    ui->tracecontinuenamecombo->clear();
    for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
    {
        ui->tracecontinuenamecombo->addItem(m_mcs->project->project_interweld_trace[n].name);
    }
    QString msg,key;
    my_cmd cmd;
    cmd_list_in=cmdlist;
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_TRACECONTINUE_KEY)//获得点坐标指令
        {
            QString name=cmd.cmd_tracecontinue_nameout;
            QString namein=cmd.cmd_tracecontinue_namein;
            RobPos pos=cmd.cmd_tracecontinue_pos;//获取到移动坐标
            inster_pos=pos;
            int weld_trace_num;
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
            ui->tracecontinuename->setText(name);

            for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
            {
                if(namein==m_mcs->project->project_interweld_trace[n].name)
                {
                    weld_trace_num=n;//找到要储存的焊接轨道下标
                    break;
                }
            }
            if(weld_trace_num>=0&&weld_trace_num<ui->tracecontinuenamecombo->count())
            {
                ui->tracecontinuenamecombo->setCurrentIndex(weld_trace_num);
            }
        }
    }
    ui->record->clear();
}

void keytracecontinueDlg::close_dlg_show()
{

}

void keytracecontinueDlg::setbutton(int name)
{
    if(name==0)
    {
        b_inster=false;
        ui->pushButton->setText(QStringLiteral("插入继续跟踪轨迹指令"));
        ui->updata_posBtn->hide();
    }
    else
    {
        b_inster=true;
        ui->pushButton->setText(QStringLiteral("替换继续跟踪轨迹指令"));
        ui->updata_posBtn->show();
    }
}

//长安到点按下
void keytracecontinueDlg::on_arriveBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    float speed=m_mcs->e2proomdata.maindlg_movespeed;
    my_cmd cmd;
    QString key,msg;
    cmd.decodecmd(cmd_list_in,msg,key);
    Robmovemodel movemod=MOVEJ;//获取到的移动模式
    int tcp=m_mcs->e2proomdata.maindlg_movetcp;
 // RobPos pos=cmd.cmd_tracecontinue_pos;//获取到移动坐标
    RobPos pos=inster_pos;
    movemod=MOVEJ;//用关节移动方式到位
    m_mcs->tosendbuffer->cmd_move(pos,movemod,speed,tcp);//移动
    ui->record->append(QStringLiteral("开始到位中..."));
}



//长按抬起
void keytracecontinueDlg::on_arriveBtn_released()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    ui->record->append(QStringLiteral("停止到位"));
}


void keytracecontinueDlg::on_pushButton_clicked()
{
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
            sendrob.data[0]=m_mcs->e2proomdata.maindlg_movetcp;
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
        my_cmd cmd;
        QString msg;
        QString name=ui->tracecontinuename->text();
        if(name.isEmpty())
        {
            ui->record->append(QStringLiteral("请填写继续跟踪轨迹工艺名"));
            return;
        }
        if(b_inster==false)
        {
            for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
            {
                if(name==m_mcs->project->project_interweld_trace[n].name)
                {
                    ui->record->append(QStringLiteral("生成的跟踪轨迹工艺与已有的跟踪轨迹工艺重名"));
                    return;
                }
            }
        }
        int route=ui->tracecontinuenamecombo->currentIndex();
        QString namein=ui->tracecontinuenamecombo->currentText();
        if(route<0||route>ui->tracecontinuenamecombo->count()-1)
        {
            ui->record->append(QStringLiteral("请选择一个原焊接轨迹工艺名"));
            return;
        }
        ui->record->append(QStringLiteral("插入继续跟踪轨迹指令成功"));
        msg=cmd.cmd_tracecontinue(namein,robpos,name);
        cmd_msg=msg;
        done(1);
    }
    else
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
    }
}


void keytracecontinueDlg::on_updata_posBtn_clicked()
{
    if(m_mcs->rob->b_link_ctx_posget==true)
    {
        send_group_robot.lock();
        sent_info_robot sendrob;
        sendrob.addr=ROB_TCP_NUM_REG_ADD;
        sendrob.ctx=m_mcs->rob->ctx_posget;
        sendrob.data.resize(1);
        sendrob.data[0]=m_mcs->e2proomdata.maindlg_movetcp;
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
        ui->record->append(QStringLiteral("与机器人的连接异常"));
    }
}

