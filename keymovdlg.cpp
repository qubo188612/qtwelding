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
    ui->record->clear();
}

void keymovDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_MOV_KEY)//移动指令
        {
            int tcp=cmd.cmd_move_tcp;//获取到移动TCP
            float speed=cmd.cmd_move_speed;//获取到速度值
            Robmovemodel movemod=cmd.cmd_move_movemod;//获取到的移动模式
            if(tcp>=0&&tcp<ROBOTTCPNUM)
            {
                ui->movetcpcombo->setCurrentIndex(tcp);
            }
            ui->movespeed->setText(QString::number(speed,'f',ROBOT_SPEED_DECIMAL_PLACE));
            ui->movemodecombo->setCurrentIndex(movemod);
        }
    }
    ui->record->clear();
}

void keymovDlg::close_dlg_show()
{

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
        switch(movemodel)
        {
            case MOVEL:
            case MOVEJ:
            {
                msg=cmd.cmd_move(robpos,movemodel,speed,tcp);
            }
            break;
            case MOVEC:
            {
                setmovec->init_dlg_show();
                setmovec->setWindowTitle(QString::fromLocal8Bit("圆弧移动设置"));
                int rc=setmovec->exec();
                setmovec->close_dlg_show();
                if(rc!=0)//确定
                {
                    msg=cmd.cmd_moveC(setmovec->pos_st,setmovec->pos_center,setmovec->pos_ed,movemodel,speed,tcp);
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

