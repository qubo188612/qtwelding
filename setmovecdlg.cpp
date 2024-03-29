#include "setmovecdlg.h"
#include "ui_setmovecdlg.h"

setmovecDlg::setmovecDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setmovecDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

//美化界面
#ifdef OPEN_BEAUTIFY_INTERFACE
    ui->arriveBtn_center->setStyleSheet(FONT_BUTTON_INFO);
    ui->arriveBtn_center_2->setStyleSheet(FONT_BUTTON_INFO);
    ui->arriveBtn_ed->setStyleSheet(FONT_BUTTON_INFO);
    ui->arriveBtn_ed_2->setStyleSheet(FONT_BUTTON_INFO);
    ui->arriveBtn_st->setStyleSheet(FONT_BUTTON_INFO);
    ui->arriveBtn_st_2->setStyleSheet(FONT_BUTTON_INFO);
    ui->lineEdit_centerRX->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_centerRY->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_centerRZ->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_centerX->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_edRX->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_edRY->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_edRZ->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_edX->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_edY->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_edZ->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_stRX->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_stRY->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_stRZ->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_stX->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_stY->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_stZ->setStyleSheet(FONT_EDIT_INFO);
    ui->pushButton_get_centerpos->setStyleSheet(FONT_BUTTON_INFO);
    ui->pushButton_get_centerpos_2->setStyleSheet(FONT_BUTTON_INFO);
    ui->pushButton_get_edpos->setStyleSheet(FONT_BUTTON_INFO);
    ui->pushButton_get_edpos_2->setStyleSheet(FONT_BUTTON_INFO);
    ui->pushButton_get_stpos->setStyleSheet(FONT_BUTTON_INFO);
    ui->pushButton_get_stpos_2->setStyleSheet(FONT_BUTTON_INFO);
    ui->record->setStyleSheet(FONT_TEXTBROWERS_INFO);
#endif

    adoubleValidator_pose = new QDoubleValidator(ROBOT_POSE_DECIMAL_BOTTOM,ROBOT_POSE_DECIMAL_TOP,ROBOT_POSE_DECIMAL_PLACE,this);//限制3位小数
    adoubleValidator_posture = new QDoubleValidator(ROBOT_POSTURE_DECIMAL_BOTTOM,ROBOT_POSTURE_DECIMAL_TOP,ROBOT_POSTURE_DECIMAL_PLACE,this);//限制4位小数

    ui->lineEdit_stX->setValidator(adoubleValidator_pose);
    ui->lineEdit_stY->setValidator(adoubleValidator_pose);
    ui->lineEdit_stZ->setValidator(adoubleValidator_pose);
    ui->lineEdit_stRX->setValidator(adoubleValidator_posture);
    ui->lineEdit_stRY->setValidator(adoubleValidator_posture);
    ui->lineEdit_stRZ->setValidator(adoubleValidator_posture);
    ui->lineEdit_edX->setValidator(adoubleValidator_pose);
    ui->lineEdit_edY->setValidator(adoubleValidator_pose);
    ui->lineEdit_edZ->setValidator(adoubleValidator_pose);
    ui->lineEdit_edRX->setValidator(adoubleValidator_posture);
    ui->lineEdit_edRY->setValidator(adoubleValidator_posture);
    ui->lineEdit_edRZ->setValidator(adoubleValidator_posture);
    ui->lineEdit_centerX->setValidator(adoubleValidator_pose);
    ui->lineEdit_centerY->setValidator(adoubleValidator_pose);
    ui->lineEdit_centerZ->setValidator(adoubleValidator_pose);
    ui->lineEdit_centerRX->setValidator(adoubleValidator_posture);
    ui->lineEdit_centerRY->setValidator(adoubleValidator_posture);
    ui->lineEdit_centerRZ->setValidator(adoubleValidator_posture);
}

setmovecDlg::~setmovecDlg()
{
    delete adoubleValidator_pose;
    delete adoubleValidator_posture;
    delete ui;
}

void setmovecDlg::init_dlg_show()
{
    b_change=false;
    changename.clear();
    ui->lineEdit_stX->setText(QString::number(pos_st.X,'f',ROBOT_POSE_DECIMAL_PLACE));
    ui->lineEdit_stY->setText(QString::number(pos_st.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
    ui->lineEdit_stZ->setText(QString::number(pos_st.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
    ui->lineEdit_stRX->setText(QString::number(pos_st.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    ui->lineEdit_stRY->setText(QString::number(pos_st.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    ui->lineEdit_stRZ->setText(QString::number(pos_st.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));

    ui->lineEdit_centerX->setText(QString::number(pos_center.X,'f',ROBOT_POSE_DECIMAL_PLACE));
    ui->lineEdit_centerY->setText(QString::number(pos_center.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
    ui->lineEdit_centerZ->setText(QString::number(pos_center.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
    ui->lineEdit_centerRX->setText(QString::number(pos_center.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    ui->lineEdit_centerRY->setText(QString::number(pos_center.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    ui->lineEdit_centerRZ->setText(QString::number(pos_center.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));

    ui->lineEdit_edX->setText(QString::number(pos_ed.X,'f',ROBOT_POSE_DECIMAL_PLACE));
    ui->lineEdit_edY->setText(QString::number(pos_ed.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
    ui->lineEdit_edZ->setText(QString::number(pos_ed.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
    ui->lineEdit_edRX->setText(QString::number(pos_ed.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    ui->lineEdit_edRY->setText(QString::number(pos_ed.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    ui->lineEdit_edRZ->setText(QString::number(pos_ed.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    ui->record->clear();
}

void setmovecDlg::init_dlg_show(QString cmdlist)
{
    b_change=false;
    changename.clear();
    QString msg,key;
    my_cmd cmd;
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        Robmovemodel movemod;
        RobPos pos1;
        RobPos pos2;
        RobPos pos3;
        if(key==CMD_MOV_KEY)//移动指令
        {
            movemod=cmd.cmd_move_movemod;//获取到的移动模式
            pos1=cmd.cmd_move_pos1;//获取到移动坐标
            pos2=cmd.cmd_move_pos2;//获取到移动坐标
            pos3=cmd.cmd_move_pos3;//获取到移动坐标
        }
        else if(key==CMD_SEARCH_KEY)
        {
            movemod=cmd.cmd_search_movemod;//获取到的移动模式
            pos1=cmd.cmd_search_pos1;//获取到移动坐标
            pos2=cmd.cmd_search_pos2;//获取到移动坐标
            pos3=cmd.cmd_search_pos3;//获取到移动坐标
        }
        else if(key==CMD_SEARCHEND_KEY)
        {
            movemod=cmd.cmd_searchend_movemod;//获取到的移动模式
            pos1=cmd.cmd_searchend_pos1;//获取到移动坐标
            pos2=cmd.cmd_searchend_pos2;//获取到移动坐标
            pos3=cmd.cmd_searchend_pos3;//获取到移动坐标
        }
        else if(key==CMD_SCAN_KEY)
        {
            movemod=cmd.cmd_scan_movemod;//获取到的移动模式
            pos1=cmd.cmd_scan_pos1;//获取到移动坐标
            pos2=cmd.cmd_scan_pos2;//获取到移动坐标
            pos3=cmd.cmd_scan_pos3;//获取到移动坐标
        }
        if(movemod==MOVEC)
        {
            ui->lineEdit_stX->setText(QString::number(pos1.X,'f',ROBOT_POSE_DECIMAL_PLACE));
            ui->lineEdit_stY->setText(QString::number(pos1.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
            ui->lineEdit_stZ->setText(QString::number(pos1.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
            ui->lineEdit_stRX->setText(QString::number(pos1.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
            ui->lineEdit_stRY->setText(QString::number(pos1.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
            ui->lineEdit_stRZ->setText(QString::number(pos1.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));

            ui->lineEdit_centerX->setText(QString::number(pos2.X,'f',ROBOT_POSE_DECIMAL_PLACE));
            ui->lineEdit_centerY->setText(QString::number(pos2.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
            ui->lineEdit_centerZ->setText(QString::number(pos2.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
            ui->lineEdit_centerRX->setText(QString::number(pos2.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
            ui->lineEdit_centerRY->setText(QString::number(pos2.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
            ui->lineEdit_centerRZ->setText(QString::number(pos2.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));

            ui->lineEdit_edX->setText(QString::number(pos3.X,'f',ROBOT_POSE_DECIMAL_PLACE));
            ui->lineEdit_edY->setText(QString::number(pos3.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
            ui->lineEdit_edZ->setText(QString::number(pos3.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
            ui->lineEdit_edRX->setText(QString::number(pos3.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
            ui->lineEdit_edRY->setText(QString::number(pos3.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
            ui->lineEdit_edRZ->setText(QString::number(pos3.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        }
    }
    ui->record->clear();
}

void setmovecDlg::init_dlg_show(QString cmdlist,bool b_change,QString changename)
{
    init_dlg_show(cmdlist);
    this->b_change=b_change;
    this->changename=changename;
}

void setmovecDlg::close_dlg_show()
{

}

void setmovecDlg::on_pushButton_get_stpos_clicked() //获取起点坐标姿态
{
    int num=0;
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
        ui->record->append(QStringLiteral("获取起点坐标失败"));
    }
    else
    {
        ui->lineEdit_stX->setText(QString::number(m_mcs->rob->TCPpos.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_stY->setText(QString::number(m_mcs->rob->TCPpos.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_stZ->setText(QString::number(m_mcs->rob->TCPpos.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_stRX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_stRY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_stRZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->record->append(QStringLiteral("获取起点坐标成功"));
    }
}


void setmovecDlg::on_pushButton_get_centerpos_clicked()//获取中间点坐标姿态
{
    int num=0;
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
        ui->record->append(QStringLiteral("获取途径点坐标失败"));
    }
    else
    {
        ui->lineEdit_centerX->setText(QString::number(m_mcs->rob->TCPpos.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_centerY->setText(QString::number(m_mcs->rob->TCPpos.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_centerZ->setText(QString::number(m_mcs->rob->TCPpos.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_centerRX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_centerRY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_centerRZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->record->append(QStringLiteral("获取途径点坐标成功"));
    }
}

void setmovecDlg::on_pushButton_get_edpos_clicked()//获取终点坐标姿态
{
    int num=0;
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
        ui->record->append(QStringLiteral("获取终点坐标失败"));
    }
    else
    {
        ui->lineEdit_edX->setText(QString::number(m_mcs->rob->TCPpos.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_edY->setText(QString::number(m_mcs->rob->TCPpos.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_edZ->setText(QString::number(m_mcs->rob->TCPpos.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_edRX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_edRY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_edRZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->record->append(QStringLiteral("获取终点坐标成功"));
    }
}


void setmovecDlg::on_pushButton_OK_clicked()
{
    pos_st.X=ui->lineEdit_stX->text().toFloat();
    pos_st.Y=ui->lineEdit_stY->text().toFloat();
    pos_st.Z=ui->lineEdit_stZ->text().toFloat();
    pos_st.RX=ui->lineEdit_stRX->text().toFloat();
    pos_st.RY=ui->lineEdit_stRY->text().toFloat();
    pos_st.RZ=ui->lineEdit_stRZ->text().toFloat();
    pos_center.X=ui->lineEdit_centerX->text().toFloat();
    pos_center.Y=ui->lineEdit_centerY->text().toFloat();
    pos_center.Z=ui->lineEdit_centerZ->text().toFloat();
    pos_center.RX=ui->lineEdit_centerRX->text().toFloat();
    pos_center.RY=ui->lineEdit_centerRY->text().toFloat();
    pos_center.RZ=ui->lineEdit_centerRZ->text().toFloat();
    pos_ed.X=ui->lineEdit_edX->text().toFloat();
    pos_ed.Y=ui->lineEdit_edY->text().toFloat();
    pos_ed.Z=ui->lineEdit_edZ->text().toFloat();
    pos_ed.RX=ui->lineEdit_edRX->text().toFloat();
    pos_ed.RY=ui->lineEdit_edRY->text().toFloat();
    pos_ed.RZ=ui->lineEdit_edRZ->text().toFloat();
    done(1);//返回值1
}

void setmovecDlg::set_arrive_param(float speed,int tcp)
{
    arrive_speed=speed;
    arrive_tcp=tcp;
}

//按下长按到点
void setmovecDlg::on_arriveBtn_st_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    bool rc;
    float speed=arrive_speed;
    int tcp=arrive_tcp;
    Robmovemodel movemod=MOVEJ;//用关节移动方式到位
    RobPos pos;
    if(ui->lineEdit_stX->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写起点X数据"));
        return;
    }
    if(ui->lineEdit_stY->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写起点Y数据"));
        return;
    }
    if(ui->lineEdit_stZ->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写起点Z数据"));
        return;
    }
    if(ui->lineEdit_stRX->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写起点RX数据"));
        return;
    }
    if(ui->lineEdit_stRY->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写起点RY数据"));
        return;
    }
    if(ui->lineEdit_stRZ->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写起点RZ数据"));
        return;
    }
    pos.X=ui->lineEdit_stX->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("起点X数据格式出错"));
        return;
    }
    pos.Y=ui->lineEdit_stY->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("起点Y数据格式出错"));
        return;
    }
    pos.Z=ui->lineEdit_stZ->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("起点Z数据格式出错"));
        return;
    }
    pos.RX=ui->lineEdit_stRX->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("起点RX数据格式出错"));
        return;
    }
    pos.RY=ui->lineEdit_stRY->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("起点RY数据格式出错"));
        return;
    }
    pos.RZ=ui->lineEdit_stRZ->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("起点RZ数据格式出错"));
        return;
    }
    m_mcs->tosendbuffer->cmd_move(pos,movemod,speed,tcp);//移动
    ui->record->append(QStringLiteral("开始到位中..."));
}

//松开长按到点
void setmovecDlg::on_arriveBtn_st_released()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    ui->record->append(QStringLiteral("停止到位"));
}

//按下长按到点
void setmovecDlg::on_arriveBtn_center_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    bool rc;
    float speed=arrive_speed;
    int tcp=arrive_tcp;
    Robmovemodel movemod=MOVEJ;//用关节移动方式到位
    RobPos pos;
    if(ui->lineEdit_centerX->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写途径点X数据"));
        return;
    }
    if(ui->lineEdit_centerY->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写途径点Y数据"));
        return;
    }
    if(ui->lineEdit_centerZ->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写途径点Z数据"));
        return;
    }
    if(ui->lineEdit_centerRX->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写途径点RX数据"));
        return;
    }
    if(ui->lineEdit_centerRY->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写途径点RY数据"));
        return;
    }
    if(ui->lineEdit_centerRZ->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写途径点RZ数据"));
        return;
    }
    pos.X=ui->lineEdit_centerX->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("途径点X数据格式出错"));
        return;
    }
    pos.Y=ui->lineEdit_centerY->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("途径点Y数据格式出错"));
        return;
    }
    pos.Z=ui->lineEdit_centerZ->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("途径点Z数据格式出错"));
        return;
    }
    pos.RX=ui->lineEdit_centerRX->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("途径点RX数据格式出错"));
        return;
    }
    pos.RY=ui->lineEdit_centerRY->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("途径点RY数据格式出错"));
        return;
    }
    pos.RZ=ui->lineEdit_centerRZ->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("途径点RZ数据格式出错"));
        return;
    }
    m_mcs->tosendbuffer->cmd_move(pos,movemod,speed,tcp);//移动
    ui->record->append(QStringLiteral("开始到位中..."));
}

//松开长按到点
void setmovecDlg::on_arriveBtn_center_released()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    ui->record->append(QStringLiteral("停止到位"));
}

//按下长按到点
void setmovecDlg::on_arriveBtn_ed_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    bool rc;
    float speed=arrive_speed;
    int tcp=arrive_tcp;
    Robmovemodel movemod=MOVEJ;//用关节移动方式到位
    RobPos pos;
    if(ui->lineEdit_edX->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写终点X数据"));
        return;
    }
    if(ui->lineEdit_edY->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写终点Y数据"));
        return;
    }
    if(ui->lineEdit_edZ->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写终点Z数据"));
        return;
    }
    if(ui->lineEdit_edRX->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写终点RX数据"));
        return;
    }
    if(ui->lineEdit_edRY->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写终点RY数据"));
        return;
    }
    if(ui->lineEdit_edRZ->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写终点RZ数据"));
        return;
    }
    pos.X=ui->lineEdit_edX->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("终点X数据格式出错"));
        return;
    }
    pos.Y=ui->lineEdit_edY->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("终点Y数据格式出错"));
        return;
    }
    pos.Z=ui->lineEdit_edZ->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("终点Z数据格式出错"));
        return;
    }
    pos.RX=ui->lineEdit_edRX->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("终点RX数据格式出错"));
        return;
    }
    pos.RY=ui->lineEdit_edRY->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("终点RY数据格式出错"));
        return;
    }
    pos.RZ=ui->lineEdit_edRZ->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("终点RZ数据格式出错"));
        return;
    }
    m_mcs->tosendbuffer->cmd_move(pos,movemod,speed,tcp);//移动
    ui->record->append(QStringLiteral("开始到位中..."));
}

//松开长按到点
void setmovecDlg::on_arriveBtn_ed_released()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    ui->record->append(QStringLiteral("停止到位"));
}

//长按到变化后起点
void setmovecDlg::on_arriveBtn_st_2_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    bool rc;
    float speed=arrive_speed;
    int tcp=arrive_tcp;
    Robmovemodel movemod=MOVEJ;//用关节移动方式到位
    RobPos pos;
    if(ui->lineEdit_stX->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写起点X数据"));
        return;
    }
    if(ui->lineEdit_stY->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写起点Y数据"));
        return;
    }
    if(ui->lineEdit_stZ->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写起点Z数据"));
        return;
    }
    if(ui->lineEdit_stRX->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写起点RX数据"));
        return;
    }
    if(ui->lineEdit_stRY->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写起点RY数据"));
        return;
    }
    if(ui->lineEdit_stRZ->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写起点RZ数据"));
        return;
    }
    pos.X=ui->lineEdit_stX->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("起点X数据格式出错"));
        return;
    }
    pos.Y=ui->lineEdit_stY->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("起点Y数据格式出错"));
        return;
    }
    pos.Z=ui->lineEdit_stZ->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("起点Z数据格式出错"));
        return;
    }
    pos.RX=ui->lineEdit_stRX->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("起点RX数据格式出错"));
        return;
    }
    pos.RY=ui->lineEdit_stRY->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("起点RY数据格式出错"));
        return;
    }
    pos.RZ=ui->lineEdit_stRZ->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("起点RZ数据格式出错"));
        return;
    }
    if(b_change==false)
    {
        ui->record->append(QStringLiteral("请先勾选使用变换矩阵"));
        return;
    }
    else if(changename.isEmpty())
    {
        ui->record->append(QStringLiteral("请先选择要使用的变换矩阵"));
        return;
    }
    else
    {
        int matrix4d_trace_num;
        bool b_find=false;
        m_mcs->process->check_data_process();
        for(int n=0;n<m_mcs->project->project_coord_matrix4d.size();n++)
        {
            if(changename==m_mcs->project->project_coord_matrix4d[n].name)
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
    m_mcs->tosendbuffer->cmd_move(pos,movemod,speed,tcp);//移动
    ui->record->append(QStringLiteral("开始到位中..."));
}

//松开长按到点
void setmovecDlg::on_arriveBtn_st_2_released()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    ui->record->append(QStringLiteral("停止到位"));
}

void setmovecDlg::on_arriveBtn_center_2_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    bool rc;
    float speed=arrive_speed;
    int tcp=arrive_tcp;
    Robmovemodel movemod=MOVEJ;//用关节移动方式到位
    RobPos pos;
    if(ui->lineEdit_centerX->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写途径点X数据"));
        return;
    }
    if(ui->lineEdit_centerY->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写途径点Y数据"));
        return;
    }
    if(ui->lineEdit_centerZ->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写途径点Z数据"));
        return;
    }
    if(ui->lineEdit_centerRX->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写途径点RX数据"));
        return;
    }
    if(ui->lineEdit_centerRY->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写途径点RY数据"));
        return;
    }
    if(ui->lineEdit_centerRZ->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写途径点RZ数据"));
        return;
    }
    pos.X=ui->lineEdit_centerX->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("途径点X数据格式出错"));
        return;
    }
    pos.Y=ui->lineEdit_centerY->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("途径点Y数据格式出错"));
        return;
    }
    pos.Z=ui->lineEdit_centerZ->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("途径点Z数据格式出错"));
        return;
    }
    pos.RX=ui->lineEdit_centerRX->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("途径点RX数据格式出错"));
        return;
    }
    pos.RY=ui->lineEdit_centerRY->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("途径点RY数据格式出错"));
        return;
    }
    pos.RZ=ui->lineEdit_centerRZ->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("途径点RZ数据格式出错"));
        return;
    }
    if(b_change==false)
    {
        ui->record->append(QStringLiteral("请先勾选使用变换矩阵"));
        return;
    }
    else if(changename.isEmpty())
    {
        ui->record->append(QStringLiteral("请先选择要使用的变换矩阵"));
        return;
    }
    else
    {
        int matrix4d_trace_num;
        bool b_find=false;
        m_mcs->process->check_data_process();
        for(int n=0;n<m_mcs->project->project_coord_matrix4d.size();n++)
        {
            if(changename==m_mcs->project->project_coord_matrix4d[n].name)
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
    m_mcs->tosendbuffer->cmd_move(pos,movemod,speed,tcp);//移动
    ui->record->append(QStringLiteral("开始到位中..."));
}

//松开长按到点
void setmovecDlg::on_arriveBtn_center_2_released()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    ui->record->append(QStringLiteral("停止到位"));
}




void setmovecDlg::on_arriveBtn_ed_2_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    bool rc;
    float speed=arrive_speed;
    int tcp=arrive_tcp;
    Robmovemodel movemod=MOVEJ;//用关节移动方式到位
    RobPos pos;
    if(ui->lineEdit_edX->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写终点X数据"));
        return;
    }
    if(ui->lineEdit_edY->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写终点Y数据"));
        return;
    }
    if(ui->lineEdit_edZ->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写终点Z数据"));
        return;
    }
    if(ui->lineEdit_edRX->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写终点RX数据"));
        return;
    }
    if(ui->lineEdit_edRY->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写终点RY数据"));
        return;
    }
    if(ui->lineEdit_edRZ->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写终点RZ数据"));
        return;
    }
    pos.X=ui->lineEdit_edX->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("终点X数据格式出错"));
        return;
    }
    pos.Y=ui->lineEdit_edY->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("终点Y数据格式出错"));
        return;
    }
    pos.Z=ui->lineEdit_edZ->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("终点Z数据格式出错"));
        return;
    }
    pos.RX=ui->lineEdit_edRX->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("终点RX数据格式出错"));
        return;
    }
    pos.RY=ui->lineEdit_edRY->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("终点RY数据格式出错"));
        return;
    }
    pos.RZ=ui->lineEdit_edRZ->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("终点RZ数据格式出错"));
        return;
    }
    if(b_change==false)
    {
        ui->record->append(QStringLiteral("请先勾选使用变换矩阵"));
        return;
    }
    else if(changename.isEmpty())
    {
        ui->record->append(QStringLiteral("请先选择要使用的变换矩阵"));
        return;
    }
    else
    {
        int matrix4d_trace_num;
        bool b_find=false;
        m_mcs->process->check_data_process();
        for(int n=0;n<m_mcs->project->project_coord_matrix4d.size();n++)
        {
            if(changename==m_mcs->project->project_coord_matrix4d[n].name)
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
    m_mcs->tosendbuffer->cmd_move(pos,movemod,speed,tcp);//移动
    ui->record->append(QStringLiteral("开始到位中..."));
}

//松开长按到点
void setmovecDlg::on_arriveBtn_ed_2_released()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QStringLiteral("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    ui->record->append(QStringLiteral("停止到位"));
}

//获取变换前起点坐标
void setmovecDlg::on_pushButton_get_stpos_2_clicked()
{
    int num=0;
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
        ui->record->append(QStringLiteral("获取起点坐标失败"));
    }
    else
    {
        RobPos pos=m_mcs->rob->TCPpos;
        if(b_change==true)
        {
            if(changename.isEmpty()==false)
            {
                int matrix4d_trace_num;
                bool b_find=false;
                m_mcs->process->check_data_process();
                for(int n=0;n<m_mcs->project->project_coord_matrix4d.size();n++)
                {
                    if(changename==m_mcs->project->project_coord_matrix4d[n].name)
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
        ui->lineEdit_stX->setText(QString::number(pos.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_stY->setText(QString::number(pos.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_stZ->setText(QString::number(pos.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_stRX->setText(QString::number(pos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_stRY->setText(QString::number(pos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_stRZ->setText(QString::number(pos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->record->append(QStringLiteral("获取起点坐标成功"));
    }
}

//获取变换前途经点坐标
void setmovecDlg::on_pushButton_get_centerpos_2_clicked()
{
    int num=0;
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
        ui->record->append(QStringLiteral("获取途径点坐标失败"));
    }
    else
    {
        RobPos pos=m_mcs->rob->TCPpos;
        if(b_change==true)
        {
            if(changename.isEmpty()==false)
            {
                int matrix4d_trace_num;
                bool b_find=false;
                m_mcs->process->check_data_process();
                for(int n=0;n<m_mcs->project->project_coord_matrix4d.size();n++)
                {
                    if(changename==m_mcs->project->project_coord_matrix4d[n].name)
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
        ui->lineEdit_centerX->setText(QString::number(pos.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_centerY->setText(QString::number(pos.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_centerZ->setText(QString::number(pos.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_centerRX->setText(QString::number(pos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_centerRY->setText(QString::number(pos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_centerRZ->setText(QString::number(pos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->record->append(QStringLiteral("获取途径点坐标成功"));
    }
}

//获取变换前终点坐标
void setmovecDlg::on_pushButton_get_edpos_2_clicked()
{
    int num=0;
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
        ui->record->append(QStringLiteral("获取终点坐标失败"));
    }
    else
    {
        RobPos pos=m_mcs->rob->TCPpos;
        if(b_change==true)
        {
            if(changename.isEmpty()==false)
            {
                int matrix4d_trace_num;
                bool b_find=false;
                m_mcs->process->check_data_process();
                for(int n=0;n<m_mcs->project->project_coord_matrix4d.size();n++)
                {
                    if(changename==m_mcs->project->project_coord_matrix4d[n].name)
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
        ui->lineEdit_edX->setText(QString::number(pos.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_edY->setText(QString::number(pos.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_edZ->setText(QString::number(pos.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_edRX->setText(QString::number(pos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_edRY->setText(QString::number(pos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_edRZ->setText(QString::number(pos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->record->append(QStringLiteral("获取终点坐标成功"));
    }
}

