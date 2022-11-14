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

    traceedit=new traceeditDlg(mcs);
    traceedit3=new traceedit3Dlg(mcs);
}

setprojectDlg::~setprojectDlg()
{
    delete traceedit;
    delete traceedit3;
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
        QString msg=cmd.cmd_move(robpos,movemodel,speed,tcp);
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
    my_cmd cmd;
    QString msg=cmd.cmd_delay(time);
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
    my_cmd cmd;
    QString msg=cmd.cmd_cam(task,1);
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
    my_cmd cmd;
    QString msg=cmd.cmd_cam_work(0);
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
    Alternatingcurrent elem=(Alternatingcurrent)ui->weldermodelcombo->currentIndex();
    my_cmd cmd;
    QString msg=cmd.cmd_elec(eled,elem,1);
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
    my_cmd cmd;
    QString msg=cmd.cmd_elec_work(0);
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
        float speed=ui->scanspeed->text().toFloat(&rc);
        RobPos robpos=m_mcs->rob->TCPpos;
        my_cmd cmd;
        QString name=ui->scanname->text();
        QString msg=cmd.cmd_scan(robpos,speed,tcp,name);
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
        if(ui->scanname->text().isEmpty())
        {
            ui->record->append(QString::fromLocal8Bit("请填写轨迹名称"));
            return;
        }
        std::vector<QString> err_msg;
        m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg);
        for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
        {
            if(name==m_mcs->project->project_scan_trace[n].name)
            {
                ui->record->append(QString::fromLocal8Bit("扫描轨迹与已有的轨迹重名"));
                return;
            }
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
    QString name=ui->tracetrackcombo->currentText();
    float speed=ui->tracespeed->text().toFloat(&rc);
    int tcp=ui->tracetcpcombo->currentIndex();
    if(route<0||route>ui->tracetrackcombo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择要插入的轨迹名字"));
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
    my_cmd cmd;
    QString filepath=ui->tracefilepath->text();//工艺包路径
    //请选择工艺包路径
    if(filepath.size()==0)
    {
        ui->record->append(QString::fromLocal8Bit("工艺包路径为空"));
        return;
    }
    else
    {
        //这里判断工艺包是否格式正确
    #if _MSC_VER
        QTextCodec *code = QTextCodec::codecForName("GBK");
    #else
        QTextCodec *code = QTextCodec::codecForName("UTF-8");
    #endif
        std::string fname = code->fromUnicode(filepath).data();
        int rc=m_mcs->craft->LoadCraft((char*)fname.c_str());
        if(rc==1)
        {
            ui->record->append(QString::fromLocal8Bit("该路径找不到工艺包文件"));
            return;
        }
        else if(rc==2)
        {
            ui->record->append(QString::fromLocal8Bit("工艺包文件格式错误"));
            return;
        }
    }
    QString msg=cmd.cmd_trace(speed,tcp,filepath,name);
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


void setprojectDlg::on_traceeditBtn_clicked()//编辑生成跟踪轨迹
{
    std::vector<QString> err_msg;
    if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
    {
        std::vector<QString> err_msg;
        QString name=ui->tracename->text();
        if(name.size()==0)
        {
            ui->record->append(QString::fromLocal8Bit("请输入要插入的轨迹名字"));
            return;
        }
        for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
        {
            if(name==m_mcs->project->project_weld_trace[n].name)
            {
                ui->record->append(QString::fromLocal8Bit("跟踪轨迹与已有的轨迹重名"));
                return;
            }
        }
        m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(now_cmdline+1,err_msg);
        if(m_mcs->project->project_scan_trace.size()<=0)
        {
            ui->record->append(QString::fromLocal8Bit("当前指令位置没有可用的扫描轨迹"));
            return;
        }
        Trace_edit_mode trace_edit_mode=(Trace_edit_mode)ui->traceeditcombo->currentIndex();
        switch(trace_edit_mode)
        {
            case TRACE_EDIT_MODE_ONE_TO_ONE://单扫对单轨道模式
            {
                traceedit->init_dlg_show();
                traceedit->setWindowTitle(QString::fromLocal8Bit("生成跟踪轨迹(单扫对单轨模式)"));
                int rc=traceedit->exec();
                traceedit->close_dlg_show();
                if(rc!=0)//确定保存生成轨迹
                {
                    my_cmd cmd;
                    std::vector<QString> scanname(1);

                    scanname[0]=traceedit->name;
                    QString msg=cmd.cmd_creat(trace_edit_mode,scanname,name);
                    if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                    {
                        m_mcs->project->project_cmdlist.push_back(msg);
                    }
                    else
                    {
                        m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                    }
                    ui->record->append(QString::fromLocal8Bit("插入生成跟踪轨迹指令成功"));
                    now_cmdline++;
                    updatacmdlistUi();
                }
            }
            break;
            case TRACE_EDIT_MODE_THREE_TO_ONE:  //三扫对单轨道模式
            {
                traceedit3->init_dlg_show();
                traceedit3->setWindowTitle(QString::fromLocal8Bit("生成跟踪轨迹(三扫对单轨模式)"));
                int rc=traceedit3->exec();
                traceedit3->close_dlg_show();
                if(rc!=0)//确定保存生成轨迹
                {
                    my_cmd cmd;
                    std::vector<QString> scanname(3);

                    scanname[0]=traceedit3->name0;
                    scanname[1]=traceedit3->name1;
                    scanname[2]=traceedit3->name2;
                    QString msg=cmd.cmd_creat(trace_edit_mode,scanname,name);
                    if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
                    {
                        m_mcs->project->project_cmdlist.push_back(msg);
                    }
                    else
                    {
                        m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,msg);
                    }
                    ui->record->append(QString::fromLocal8Bit("插入生成跟踪轨迹指令成功"));
                    now_cmdline++;
                    updatacmdlistUi();
                }
            }
            break;
        }
    }
    else
    {
        ui->record->append(QString::fromLocal8Bit("有同名的扫描轨迹，请先排除"));
    }
}


void setprojectDlg::on_customaddBtn_clicked()//插入自定义指令
{
    std::vector<QString> err_msg;
    QString msg;
    QString key;
    my_cmd cmd;
    if(0>=cmd.decodecmd(ui->customcmd->text(),msg,key))
    {   
        m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg);
        if(key==CMD_SCAN_KEY)
        {
            QString name=cmd.cmd_scan_name;
            for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
            {
                if(name==m_mcs->project->project_scan_trace[n].name)
                {
                    ui->record->append(QString::fromLocal8Bit("扫描轨迹与已有的轨迹重名"));
                    return;
                }
            }
        }
        else if(key==CMD_CREAT_KEY)
        {
            QString name=cmd.cmd_creat_name;
            for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
            {
                if(name==m_mcs->project->project_weld_trace[n].name)
                {
                    ui->record->append(QString::fromLocal8Bit("从跟踪轨迹与已有的轨迹重名"));
                    return;
                }
            }
        }
        else if(key==CMD_TRACE_KEY)
        {
            QString filepath=cmd.cmd_trace_craftfilepath;
        #if _MSC_VER
            QTextCodec *code = QTextCodec::codecForName("GBK");
        #else
            QTextCodec *code = QTextCodec::codecForName("UTF-8");
        #endif
            std::string fname = code->fromUnicode(filepath).data();
            int rc=m_mcs->craft->LoadCraft((char*)fname.c_str());
            if(rc==1)
            {
                ui->record->append(QString::fromLocal8Bit("该路径找不到工艺包文件"));
                return;
            }
            else if(rc==2)
            {
                ui->record->append(QString::fromLocal8Bit("工艺包文件格式错误"));
                return;
            }
        }
        //解码成功
        if(now_cmdline==m_mcs->project->project_cmdlist.size()-1)
        {
            m_mcs->project->project_cmdlist.push_back(ui->customcmd->text());
        }
        else
        {
            m_mcs->project->project_cmdlist.insert(m_mcs->project->project_cmdlist.begin()+now_cmdline+1,ui->customcmd->text());
        }
        ui->record->append(QString::fromLocal8Bit("插入自定义指令成功"));
        now_cmdline++;
        updatacmdlistUi();
    }
    else
    {
        ui->record->append(msg);
    }
}

void setprojectDlg::on_customreplaceBtn_clicked()//替换自定义指令
{
    std::vector<QString> err_msg;
    QString msg;
    QString key;
    my_cmd cmd;
    if(0>=cmd.decodecmd(ui->customcmd->text(),msg,key))
    {
        //解码成功
        if(key==CMD_TRACE_KEY)
        {
            QString filepath=cmd.cmd_trace_craftfilepath;
        #if _MSC_VER
            QTextCodec *code = QTextCodec::codecForName("GBK");
        #else
            QTextCodec *code = QTextCodec::codecForName("UTF-8");
        #endif
            std::string fname = code->fromUnicode(filepath).data();
            int rc=m_mcs->craft->LoadCraft((char*)fname.c_str());
            if(rc==1)
            {
                ui->record->append(QString::fromLocal8Bit("该路径找不到工艺包文件"));
                return;
            }
            else if(rc==2)
            {
                ui->record->append(QString::fromLocal8Bit("工艺包文件格式错误"));
                return;
            }
        }
        if(now_cmdline>=0&&m_mcs->project->project_cmdlist.size()>now_cmdline)
        {
            m_mcs->project->project_cmdlist[now_cmdline]=ui->customcmd->text();
            if(0==m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg))
            {
                ui->record->append(QString::fromLocal8Bit("替换自定义指令成功"));
            }
            else
            {
                for(int n=0;n<err_msg.size();n++)
                {
                    ui->record->append(err_msg[n]);
                }
            }
            updatacmdlistUi();
        }
        else
        {
            ui->record->append(QString::fromLocal8Bit("请先选中要替换的指令"));
        }
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
    updatacmdlistUi();
    ui->customcmd->setText(m_mcs->project->project_cmdlist[now_cmdline]);
}

void setprojectDlg::on_OKBtn_clicked()//保存指令
{
    m_mcs->project->SaveProject((char*)m_mcs->project->project_path.toStdString().c_str());
    ui->record->append(QString::fromLocal8Bit("已经保存当前操作"));
}

void setprojectDlg::on_tracefilepathBtn_clicked()//修改路径
{
    QString fileName = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("请选择要导入的焊接工艺文件"), "./CRAFT/", "CRAFT(*.craft)");
#if _MSC_VER
    QTextCodec *code = QTextCodec::codecForName("GBK");
#else
    QTextCodec *code = QTextCodec::codecForName("UTF-8");
#endif
    std::string name = code->fromUnicode(fileName).data();
    if(name.size()>0)
    {
        if(0!=m_mcs->craft->LoadCraft((char*)name.c_str()))
        {
            ui->record->append(QString::fromLocal8Bit("工艺文件读取失败"));
            return;
        }
        ui->record->append(QString::fromLocal8Bit("工艺文件读取成功"));
        ui->tracefilepath->setText(fileName);
    }
}

void setprojectDlg::updatacmdlistUi()
{
    std::vector<QString> err_msg;

    ui->cmdlist->clear();
    for(int n=0;n<m_mcs->project->project_cmdlist.size();n++)
    {
        QString msg;
        msg="Line"+QString::number(n)+"  "+m_mcs->project->project_cmdlist[n];
        ui->cmdlist->addItem(msg);
    }
    if(m_mcs->project->project_cmdlist.size()>0)
    {
        ui->cmdlist->setCurrentRow(now_cmdline);
    }

    m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(now_cmdline+1,err_msg);
    ui->tracetrackcombo->clear();
    for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
    {
        ui->tracetrackcombo->addItem(m_mcs->project->project_weld_trace[n].name);
    }
}




