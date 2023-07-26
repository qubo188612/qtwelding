#include "arrvepointtestdlg.h"
#include "ui_arrvepointtestdlg.h"

arrvepointtestDlg::arrvepointtestDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::arrvepointtestDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸
}

arrvepointtestDlg::~arrvepointtestDlg()
{
    delete ui;
}

void arrvepointtestDlg::init_dlg_show()
{
    ui->record->clear();
}

void arrvepointtestDlg::close_dlg_show()
{

}

//长按到点按下
void arrvepointtestDlg::on_arriveBtn_pressed()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("与机器人的连接异常"));
        return;
    }
    if(ui->lineEdit_X->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填入X数据"));
        return;
    }
    if(ui->lineEdit_Y->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填入Y数据"));
        return;
    }
    if(ui->lineEdit_Z->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填入Z数据"));
        return;
    }
    if(ui->lineEdit_RX->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填入RX数据"));
        return;
    }
    if(ui->lineEdit_RY->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填入RY数据"));
        return;
    }
    if(ui->lineEdit_RZ->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填入RZ数据"));
        return;
    }
    RobPos pos;
    bool rc;
    pos.X=ui->lineEdit_X->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("X数据格式异常"));
        return;
    }
    pos.Y=ui->lineEdit_Y->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("Y数据格式异常"));
        return;
    }
    pos.Z=ui->lineEdit_Z->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("Z数据格式异常"));
        return;
    }
    pos.RX=ui->lineEdit_RX->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("RX数据格式异常"));
        return;
    }
    pos.RY=ui->lineEdit_RY->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("RY数据格式异常"));
        return;
    }
    pos.RZ=ui->lineEdit_RZ->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("RZ数据格式异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    float speed=m_mcs->e2proomdata.maindlg_movespeed;
    my_cmd cmd;
    Robmovemodel movemod=MOVEJ;//获取到的移动模式
    int tcp=m_mcs->e2proomdata.maindlg_movetcp;
    movemod=MOVEJ;//用关节移动方式到位
    m_mcs->tosendbuffer->cmd_move(pos,movemod,speed,tcp);//移动
    ui->record->append(QString::fromLocal8Bit("开始到位中..."));
}

//长按到点抬起
void arrvepointtestDlg::on_arriveBtn_released()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("与机器人的连接异常"));
        return;
    }
    m_mcs->tosendbuffer->cmd_lock(0);
    ui->record->append(QString::fromLocal8Bit("停止到位"));
}


//更新为当前激光
void arrvepointtestDlg::on_updataleaser_clicked()
{
    int num=0;
    m_mcs->cam->sop_cam[0].b_ros_lineEn=false;
    m_mcs->rob->TCPpos.nEn=false;
    while (m_mcs->cam->sop_cam[0].b_ros_lineEn==false||
           m_mcs->rob->TCPpos.nEn==false)
    {
        if(num>10)
        {
            break;
        }
        usleep(ROB_WORK_DELAY_STEP);
        num++;
    }
    if(m_mcs->cam->sop_cam[0].b_ros_lineEn==false)
    {
        ui->record->append(QString::fromLocal8Bit("获取激光头坐标失败"));
        return;
    }
    else if(m_mcs->rob->TCPpos.nEn==false)
    {
        ui->record->append(QString::fromLocal8Bit("获取机器人坐标失败"));
        return;
    }
    else
    {
        std::vector<Scan_trace_line> scan_trace(1);
        std::vector<RobPos> weld_trace;
        scan_trace[0].robottime=m_mcs->rob->robtime;
        scan_trace[0].ros_line=*(m_mcs->cam->sop_cam[0].ros_line);
    #if OPEN_TIMESTAMP==1
        scan_trace[0].robotpos.X=scan_trace[0].ros_line.robpos.posx;
        scan_trace[0].robotpos.Y=scan_trace[0].ros_line.robpos.posy;
        scan_trace[0].robotpos.Z=scan_trace[0].ros_line.robpos.posz;
        scan_trace[0].robotpos.RX=scan_trace[0].ros_line.robpos.posrx;
        scan_trace[0].robotpos.RY=scan_trace[0].ros_line.robpos.posry;
        scan_trace[0].robotpos.RZ=scan_trace[0].ros_line.robpos.posrz;
        scan_trace[0].robotpos.out_1=scan_trace[0].ros_line.robpos.posout1;
        scan_trace[0].robotpos.out_2=scan_trace[0].ros_line.robpos.posout2;
        scan_trace[0].robotpos.out_3=scan_trace[0].ros_line.robpos.posout3;
        scan_trace[0].robotpos.nEn=1;
    #else
        scan_trace[0].robotpos=m_mcs->rob->TCPpos;
    #endif
        if(false==m_mcs->synchronous->Scantrace_to_Weldtrace(scan_trace,weld_trace))
        {
            ui->record->append(QString::fromLocal8Bit("扫描计算结果出错"));
            return;
        }
        ui->lineEdit_X->setText(QString::number(weld_trace[0].X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_Y->setText(QString::number(weld_trace[0].Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_Z->setText(QString::number(weld_trace[0].Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_RX->setText(QString::number(scan_trace[0].robotpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_RY->setText(QString::number(scan_trace[0].robotpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_RZ->setText(QString::number(scan_trace[0].robotpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->record->append(QString::fromLocal8Bit("更新机器人坐标完成"));
    }
}

//更新为当前姿态
void arrvepointtestDlg::on_updatapose_clicked()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("与机器人的连接异常"));
        return;
    }
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
        ui->record->append(QString::fromLocal8Bit("获取机器人坐标失败"));
    }
    else
    {
        ui->lineEdit_RX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_RY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_RZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->record->append(QString::fromLocal8Bit("更新当前机器人坐标完成"));
    }
}

//更新为当前坐标
void arrvepointtestDlg::on_updatapos_clicked()
{
    if(m_mcs->rob->b_link_ctx_posget==false)
    {
        ui->record->append(QString::fromLocal8Bit("与机器人的连接异常"));
        return;
    }
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
        ui->record->append(QString::fromLocal8Bit("获取机器人坐标失败"));
    }
    else
    {
        ui->lineEdit_X->setText(QString::number(m_mcs->rob->TCPpos.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_Y->setText(QString::number(m_mcs->rob->TCPpos.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_Z->setText(QString::number(m_mcs->rob->TCPpos.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_RX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_RY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_RZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->record->append(QString::fromLocal8Bit("更新当前机器人坐标完成"));
    }
}

