#include "setprojectdlg.h"
#include "ui_setprojectdlg.h"

setprojectDlg::setprojectDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setprojectDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;

    for(int n=0;n<ROBOTTCPNUM;n++)
    {
        QString msg="TCP: "+QString::number(n);
        ui->movetcpcombo->addItem(msg);
        ui->tracetcpcombo->addItem(msg);
        ui->scantcpcombo->addItem(msg);
    }
}

setprojectDlg::~setprojectDlg()
{
    delete ui;
}

void setprojectDlg::init_dlg_show()
{
    now_cmdline=m_mcs->project->project_cmdlist.size()-1;
    updatacmdlistUi();
}

void setprojectDlg::close_dlg_show()
{

}

void setprojectDlg::on_moveaddBtn_clicked()//插入移动指令
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
        usleep(50000);
        int num=0;
        while(m_mcs->rob->b_send_group_robot==false)
        {
            if(num>10)
            {
                break;
            }
            usleep(10000);
            num++;
        }
        if(m_mcs->rob->b_send_group_robot==false)
        {
            ui->record->append(QString::fromLocal8Bit("机器人TCP设置异常"));
            return;
        }
        usleep(50000);//等待服务器获取到机器人坐标
        num=0;
        m_mcs->rob->TCPpos.nEn=false;
        while (m_mcs->rob->TCPpos.nEn==false)
        {
            if(num>10)
            {
                break;
            }
            usleep(10000);
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
        QString msg=my_cmd::cmd_move(robpos,movemodel,speed,tcp);
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
        if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
        {
            m_mcs->project->project_cmdlist.push_back(msg);
        }
        else
        {
            m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
        }
        ui->record->append(QString::fromLocal8Bit("插入移动指令成功"));
        now_cmdline++;
        updatacmdlistUi();
    }
    else
    {
        ui->record->append(QString::fromLocal8Bit("与机器人的连接异常"));
    }
}


void setprojectDlg::on_delaytimeBtn_clicked()//插入延时指令
{
    bool rc;
    int time=ui->delaytime->text().toInt(&rc);
    QString msg=my_cmd::cmd_delay(time);
    if(ui->delaytime->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写延时时间"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("延时时间格式出错"));
        return;
    }
    if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
    {
        m_mcs->project->project_cmdlist.push_back(msg);
    }
    else
    {
        m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
    }
    ui->record->append(QString::fromLocal8Bit("插入延时指令成功"));
    now_cmdline++;
    updatacmdlistUi();
}


void setprojectDlg::on_leaseropenBtn_clicked()//插入开激光指令
{
    bool rc;
    int task=ui->leasertasknum->text().toInt(&rc);
    QString msg=my_cmd::cmd_cam(task,1);
    if(ui->leasertasknum->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写任务号"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("任务号格式出错"));
        return;
    }
    if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
    {
        m_mcs->project->project_cmdlist.push_back(msg);
    }
    else
    {
        m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
    }
    ui->record->append(QString::fromLocal8Bit("插入开激光指令成功"));
    now_cmdline++;
    updatacmdlistUi();
}


void setprojectDlg::on_leasercloseBtn_clicked()//插入关激光指令
{
    QString msg=my_cmd::cmd_cam_work(0);
    if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
    {
        m_mcs->project->project_cmdlist.push_back(msg);
    }
    else
    {
        m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
    }
    ui->record->append(QString::fromLocal8Bit("插入关激光指令成功"));
    now_cmdline++;
    updatacmdlistUi();
}


void setprojectDlg::on_welderarcingBtn_clicked()//插入起弧指令
{
    bool rc;
    float eled=ui->weldercurrent->text().toFloat(&rc);
    int elem=ui->weldermodelcombo->currentIndex();
    QString msg=my_cmd::cmd_elec(eled,elem,1);
    if(ui->weldercurrent->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写电流值"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("电流值格式出错"));
        return;
    }
    if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
    {
        m_mcs->project->project_cmdlist.push_back(msg);
    }
    else
    {
        m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
    }
    ui->record->append(QString::fromLocal8Bit("插入起弧指令成功"));
    now_cmdline++;
    updatacmdlistUi();
}


void setprojectDlg::on_welderarcoutBtn_clicked()//插入息弧指令
{
    QString msg=my_cmd::cmd_elec_work(0);
    if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
    {
        m_mcs->project->project_cmdlist.push_back(msg);
    }
    else
    {
        m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
    }
    ui->record->append(QString::fromLocal8Bit("插入息弧指令成功"));
    now_cmdline++;
    updatacmdlistUi();
}


void setprojectDlg::on_scanaddBtn_clicked()//插入采集数据指令
{
    int tcp=ui->scantcpcombo->currentIndex();
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
        usleep(50000);
        int num=0;
        while(m_mcs->rob->b_send_group_robot==false)
        {
            if(num>10)
            {
                break;
            }
            usleep(10000);
            num++;
        }
        if(m_mcs->rob->b_send_group_robot==false)
        {
            ui->record->append(QString::fromLocal8Bit("机器人TCP设置异常"));
            return;
        }
        usleep(50000);//等待服务器获取到机器人坐标
        num=0;
        m_mcs->rob->TCPpos.nEn=false;
        while (m_mcs->rob->TCPpos.nEn==false)
        {
            if(num>10)
            {
                break;
            }
            usleep(10000);
            num++;
        }
        if(m_mcs->rob->TCPpos.nEn==false)
        {
            ui->record->append(QString::fromLocal8Bit("获取机器人坐标失败"));
            return;
        }
        bool rc;
        float speed=ui->scanspeed->text().toFloat(&rc);
        RobPos robpos=m_mcs->rob->TCPpos;
        QString msg=my_cmd::cmd_scan(robpos,speed,tcp);
        if(ui->scanspeed->text().isEmpty())
        {
            ui->record->append(QString::fromLocal8Bit("请填写采集速度"));
            return;
        }
        if(rc==false)
        {
            ui->record->append(QString::fromLocal8Bit("采集速度格式出错"));
            return;
        }
        if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
        {
            m_mcs->project->project_cmdlist.push_back(msg);
        }
        else
        {
            m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
        }
        ui->record->append(QString::fromLocal8Bit("插入采集指令成功"));
        now_cmdline++;
        updatacmdlistUi();
    }
    else
    {
        ui->record->append(QString::fromLocal8Bit("与机器人的连接异常"));
    }
}


void setprojectDlg::on_tracecmdaddBtn_clicked()//插入跟踪轨迹指令
{
    bool rc;
    int route=ui->tracetrackcombo->currentIndex();
    float speed=ui->tracespeed->text().toFloat(&rc);
    int tcp=ui->tracetcpcombo->currentIndex();
    if(route<0||route>ui->tracetrackcombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择要插入的轨迹序号"));
        return;
    }
    else if(ui->tracespeed->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写跟踪速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("跟踪速度格式出错"));
        return;
    }
    QString msg=my_cmd::cmd_trace(route,speed,tcp);
    if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
    {
        m_mcs->project->project_cmdlist.push_back(msg);
    }
    else
    {
        m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
    }
    ui->record->append(QString::fromLocal8Bit("插入跟踪轨迹指令成功"));
    now_cmdline++;
    updatacmdlistUi();
}


void setprojectDlg::on_traceeditBtn_clicked()//编辑轨迹
{

}


void setprojectDlg::on_customaddBtn_clicked()//插入自定义指令
{
    QString msg;
    if(0==my_cmd::decodecmd(ui->customcmd->text(),msg))
    {
        //解码成功
        ui->record->append(QString::fromLocal8Bit("插入自定义指令成功"));
    }
    else
    {
        ui->record->append(msg);
    }
}


void setprojectDlg::on_customcheckBtn_clicked()//指令表查看
{

}


void setprojectDlg::on_cmdlistdeleteBtn_clicked()//删除指令
{
    if(now_cmdline>=0&&m_mcs->project->project_cmdlist.size()>now_cmdline)
    {
        m_mcs->project->project_cmdlist.erase(m_mcs->project->project_cmdlist.begin()+now_cmdline);
        ui->record->append(QString::fromLocal8Bit("删除指令"));
        if(now_cmdline>=m_mcs->project->project_cmdlist.size())
            now_cmdline=now_cmdline-1;
        updatacmdlistUi();
    }
    else
    {
        ui->record->append(QString::fromLocal8Bit("请先选中要删除的指令"));
    }
}


void setprojectDlg::on_cmdlistclearBtn_clicked()//清空指令
{
    QMessageBox:: StandardButton result= QMessageBox::information(this, QString::fromLocal8Bit("提示信息"),
                                                                  QString::fromLocal8Bit("确认清空全部指令？"),
                                                                  QMessageBox::Yes|QMessageBox::No,
                                                                  QMessageBox::No
                                                                  );
    switch (result)
    {
        case QMessageBox::Yes:
        {
            m_mcs->project->project_cmdlist.clear();
            now_cmdline=m_mcs->project->project_cmdlist.size()-1;
            updatacmdlistUi();
            ui->record->append(QString::fromLocal8Bit("清空指令完成"));
        }
        break;
        case QMessageBox::No:
        {
            ui->record->append(QString::fromLocal8Bit("取消清空指令操作"));
        }
        break;
    }
}

void setprojectDlg::on_cmdlist_itemClicked(QListWidgetItem *item)//选择值令
{
    now_cmdline=ui->cmdlist->currentRow();
    ui->customcmd->setText(m_mcs->project->project_cmdlist[now_cmdline]);
}

void setprojectDlg::on_OKBtn_clicked()//保存指令
{
    m_mcs->project->SaveProject((char*)m_mcs->project->project_path.toStdString().c_str());
    ui->record->append(QString::fromLocal8Bit("已经保存当前操作"));
}

void setprojectDlg::updatacmdlistUi()
{
    ui->cmdlist->clear();
    for(int n=0;n<m_mcs->project->project_cmdlist.size();n++)
    {
        QString msg;
        msg="Line"+QString::number(n+1)+"  "+m_mcs->project->project_cmdlist[n];
        ui->cmdlist->addItem(msg);
    }
    if(m_mcs->project->project_cmdlist.size()>0)
    {
        ui->cmdlist->setCurrentRow(now_cmdline);
    }
}



