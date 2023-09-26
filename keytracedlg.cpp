#include "keytracedlg.h"
#include "ui_keytracedlg.h"

keytraceDlg::keytraceDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keytraceDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

    ui->tracechangecheckBox->setCheckState(Qt::Unchecked);
    ui->tracechangecombo->setDisabled(true);

    adoubleValidator_speed = new QDoubleValidator(ROBOT_SPEED_DECIMAL_BOTTOM,ROBOT_SPEED_DECIMAL_TOP,ROBOT_SPEED_DECIMAL_PLACE,this);//限制3位小数
    ui->tracespeed->setValidator(adoubleValidator_speed);

}

keytraceDlg::~keytraceDlg()
{
    delete adoubleValidator_speed;
    delete ui;
}

void keytraceDlg::init_dlg_show()
{
    ui->tracetrackcombo->clear();
    for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
    {
        ui->tracetrackcombo->addItem(m_mcs->project->project_weld_trace[n].name);
    }
    ui->tracechangecombo->clear();
    for(int n=0;n<m_mcs->project->project_coord_matrix4d.size();n++)
    {
        ui->tracechangecombo->addItem(m_mcs->project->project_coord_matrix4d[n].name);
    }
    cmd_list_in.clear();
    ui->record->clear();
}

void keytraceDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    cmd_list_in=cmdlist;
    ui->tracetrackcombo->clear();
    for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
    {
        ui->tracetrackcombo->addItem(m_mcs->project->project_weld_trace[n].name);
    }
    ui->tracechangecombo->clear();
    for(int n=0;n<m_mcs->project->project_coord_matrix4d.size();n++)
    {
        ui->tracechangecombo->addItem(m_mcs->project->project_coord_matrix4d[n].name);
    }
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_TRACE_KEY)//移动指令
        {
            QString name_in=cmd.cmd_trace_name_in;//获取到跟踪轨迹序号
            QString name_out=cmd.cmd_trace_name_out;//获取到跟踪轨迹工艺名字
            float speed=cmd.cmd_trace_speed;//获取到的跟踪速度
            QString change=cmd.cmd_trace_change;//获取到的变换矩阵
            QString craftfilepath=cmd.cmd_trace_craftfilepath;//获取到工艺包的文件路径
            ui->tracefilepath->setText(craftfilepath);
            ui->tracespeed->setText(QString::number(speed,'f',ROBOT_SPEED_DECIMAL_PLACE));
            int weld_trace_num;//搜索到的焊接轨道序号
            //这里添加移动命令
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
                if(change_trace_num>=0&&change_trace_num<ui->tracechangecombo->count())
                {
                    ui->tracechangecombo->setCurrentIndex(change_trace_num);
                }
                ui->tracechangecheckBox->setCheckState(Qt::Checked);
                ui->tracechangecombo->setDisabled(false);
            }
            else
            {
                ui->tracechangecheckBox->setCheckState(Qt::Unchecked);
                ui->tracechangecombo->setDisabled(true);
            }
            for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
            {
                if(name_in==m_mcs->project->project_weld_trace[n].name)
                {
                    weld_trace_num=n;//找到要储存的焊接轨道下标
                    break;
                }
            }
            if(weld_trace_num>=0&&weld_trace_num<ui->tracetrackcombo->count())
            {
                ui->tracetrackcombo->setCurrentIndex(weld_trace_num);
            }
            ui->tracefilename->setText(name_out);
        }
    }
    ui->record->clear();
}

void keytraceDlg::close_dlg_show()
{

}

void keytraceDlg::setbutton(int name)
{
    if(name==0)
    {
        ui->tracecmdaddBtn->setText(QStringLiteral("插入跟踪轨迹指令"));
    }
    else
    {
        ui->tracecmdaddBtn->setText(QStringLiteral("替换跟踪轨迹指令"));
    }
}

void keytraceDlg::on_tracefilepathBtn_clicked()
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
        ui->tracefilepath->setText(fileName);
    }
}

void keytraceDlg::on_tracecmdaddBtn_clicked()
{
    bool rc;
    int route=ui->tracetrackcombo->currentIndex();
    QString name_in=ui->tracetrackcombo->currentText();
    QString name_out=ui->tracefilename->text();
    float speed=ui->tracespeed->text().toFloat(&rc);
    if(route<0||route>ui->tracetrackcombo->count()-1)
    {
        ui->record->append(QStringLiteral("请选择要插入的轨迹名字"));
        return;
    }
    else if(ui->tracespeed->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写跟踪速度"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QStringLiteral("跟踪速度格式出错"));
        return;
    }
    if(name_out.isEmpty())
    {
        ui->record->append(QStringLiteral("请填写生成的跟踪轨迹工艺名字"));
        return;
    }
    my_cmd cmd;
    QString filepath=ui->tracefilepath->text();//工艺包路径
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
    QString change;
    if(ui->tracechangecheckBox->isChecked()==true)
    {
        change=ui->tracechangecombo->currentText();
    }
    QString msg=cmd.cmd_trace(name_in,speed,filepath,name_out,change);
    ui->record->append(QStringLiteral("插入跟踪轨迹指令成功"));
    cmd_msg=msg;
    done(1);
}

//变换矩阵有效
void keytraceDlg::on_tracechangecheckBox_stateChanged(int arg1)
{
    if(arg1==0)
    {
        ui->tracechangecombo->setDisabled(true);
    }
    else
    {
        ui->tracechangecombo->setDisabled(false);
    }
}

