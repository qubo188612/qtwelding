#include "keytrace2dlg.h"
#include "ui_keytrace2dlg.h"

keytrace2Dlg::keytrace2Dlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keytrace2Dlg)
{
    ui->setupUi(this);
    m_mcs=mcs;

    setFixedSize(this->width(), this->height());//禁止拉伸

    ui->trace2changecheckBox->setCheckState(Qt::Unchecked);
    ui->trace2changecombo->setDisabled(true);

    adoubleValidator_speed = new QDoubleValidator(ROBOT_SPEED_DECIMAL_BOTTOM,ROBOT_SPEED_DECIMAL_TOP,ROBOT_SPEED_DECIMAL_PLACE,this);//限制3位小数
    ui->trace2speed->setValidator(adoubleValidator_speed);
}

keytrace2Dlg::~keytrace2Dlg()
{
    delete ui;
}

void keytrace2Dlg::init_dlg_show()
{
    ui->trace2filepathcombo->clear();
    for(int n=0;n<m_mcs->project->project_crafts_trace.size();n++)
    {
        ui->trace2filepathcombo->addItem(m_mcs->project->project_crafts_trace[n].name);
    }
    ui->trace2trackcombo->clear();
    for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
    {
        ui->trace2trackcombo->addItem(m_mcs->project->project_weld_trace[n].name);
    }
    ui->trace2changecombo->clear();
    for(int n=0;n<m_mcs->project->project_coord_matrix4d.size();n++)
    {
        ui->trace2changecombo->addItem(m_mcs->project->project_coord_matrix4d[n].name);
    }
    cmd_list_in.clear();
    ui->record->clear();
}

void keytrace2Dlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    cmd_list_in=cmdlist;
    ui->trace2filepathcombo->clear();
    for(int n=0;n<m_mcs->project->project_crafts_trace.size();n++)
    {
        ui->trace2filepathcombo->addItem(m_mcs->project->project_crafts_trace[n].name);
    }
    ui->trace2trackcombo->clear();
    for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
    {
        ui->trace2trackcombo->addItem(m_mcs->project->project_weld_trace[n].name);
    }
    ui->trace2changecombo->clear();
    for(int n=0;n<m_mcs->project->project_coord_matrix4d.size();n++)
    {
        ui->trace2changecombo->addItem(m_mcs->project->project_coord_matrix4d[n].name);
    }
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_TRACE2_KEY)//移动指令
        {
            QString name_in=cmd.cmd_trace2_name_in;//获取到跟踪轨迹序号
            QString name_out=cmd.cmd_trace2_name_out;//获取到跟踪轨迹工艺名字
            float speed=cmd.cmd_trace2_speed;//获取到的跟踪速度
            QString change=cmd.cmd_trace2_change;//获取到的变换矩阵
            QString craftfilepath=cmd.cmd_trace2_crafts;//获取到工艺包的文件路径
            int crafts_trace_num;//搜索到的工艺序号
            ui->trace2speed->setText(QString::number(speed,'f',ROBOT_SPEED_DECIMAL_PLACE));
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
                if(change_trace_num>=0&&change_trace_num<ui->trace2changecombo->count())
                {
                    ui->trace2changecombo->setCurrentIndex(change_trace_num);
                }
                ui->trace2changecheckBox->setCheckState(Qt::Checked);
                ui->trace2changecombo->setDisabled(false);
            }
            else
            {
                ui->trace2changecheckBox->setCheckState(Qt::Unchecked);
                ui->trace2changecombo->setDisabled(true);
            }
            for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
            {
                if(name_in==m_mcs->project->project_weld_trace[n].name)
                {
                    weld_trace_num=n;//找到要储存的焊接轨道下标
                    break;
                }
            }
            if(weld_trace_num>=0&&weld_trace_num<ui->trace2trackcombo->count())
            {
                ui->trace2trackcombo->setCurrentIndex(weld_trace_num);
            }
            for(int n=0;n<m_mcs->project->project_crafts_trace.size();n++)
            {
                if(craftfilepath==m_mcs->project->project_crafts_trace[n].name)
                {
                    crafts_trace_num=n;//找到要储存的焊接轨道下标
                    break;
                }
            }
            if(crafts_trace_num>=0&&crafts_trace_num<ui->trace2filepathcombo->count())
            {
                ui->trace2filepathcombo->setCurrentIndex(crafts_trace_num);
            }
            ui->trace2filename->setText(name_out);
        }
    }
    ui->record->clear();
}

void keytrace2Dlg::close_dlg_show()
{

}

void keytrace2Dlg::setbutton(int name)
{
    if(name==0)
    {
        ui->trace2cmdaddBtn->setText(QStringLiteral("插入跟踪轨迹指令"));
    }
    else
    {
        ui->trace2cmdaddBtn->setText(QStringLiteral("替换跟踪轨迹指令"));
    }
}


void keytrace2Dlg::on_trace2cmdaddBtn_clicked()
{
    bool rc;
    int route=ui->trace2trackcombo->currentIndex();
    QString name_in=ui->trace2trackcombo->currentText();
    QString name_out=ui->trace2filename->text();
    float speed=ui->trace2speed->text().toFloat(&rc);
    if(route<0||route>ui->trace2trackcombo->count()-1)
    {
        ui->record->append(QStringLiteral("请选择要插入的轨迹名字"));
        return;
    }
    else if(ui->trace2speed->text().isEmpty())
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
    route=ui->trace2filepathcombo->currentIndex();
    if(route<0||route>ui->trace2filepathcombo->count()-1)
    {
        ui->record->append(QStringLiteral("请选择要插入的工艺包"));
        return;
    }
    QString filepath=ui->trace2filepathcombo->currentText();//工艺包路径
    //请选择工艺包路径
    QString change;
    if(ui->trace2changecheckBox->isChecked()==true)
    {
        change=ui->trace2changecombo->currentText();
    }
    QString msg=cmd.cmd_trace2(name_in,speed,filepath,name_out,change);
    ui->record->append(QStringLiteral("插入跟踪轨迹指令成功"));
    cmd_msg=msg;
    done(1);
}

//变换矩阵有效
void keytrace2Dlg::on_trace2changecheckBox_stateChanged(int arg1)
{
    if(arg1==0)
    {
        ui->trace2changecombo->setDisabled(true);
    }
    else
    {
        ui->trace2changecombo->setDisabled(false);
    }
}

