#include "keycreatcdlg.h"
#include "ui_keycreatcdlg.h"

keycreatcDlg::keycreatcDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keycreatcDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

//美化界面
#ifdef OPEN_BEAUTIFY_INTERFACE
    ui->pos1comboBox->setStyleSheet(FONT_COMBOBOX_INFO);
    ui->pos2comboBox->setStyleSheet(FONT_COMBOBOX_INFO);
    ui->pos3comboBox->setStyleSheet(FONT_COMBOBOX_INFO);
    ui->creatctimelineEdit->setStyleSheet(FONT_EDIT_INFO);
    ui->creatcspeedlineEdit->setStyleSheet(FONT_EDIT_INFO);
    ui->creatcnamelineEdit->setStyleSheet(FONT_EDIT_INFO);
    ui->creatcBtn->setStyleSheet(FONT_BUTTON_INFO);
    ui->record->setStyleSheet(FONT_TEXTBROWERS_INFO);
#endif

    adoubleValidator_speed = new QDoubleValidator(ROBOT_SPEED_DECIMAL_BOTTOM,ROBOT_SPEED_DECIMAL_TOP,ROBOT_SPEED_DECIMAL_PLACE,this);//限制3位小数
    ui->creatcspeedlineEdit->setValidator(adoubleValidator_speed);
}

keycreatcDlg::~keycreatcDlg()
{
    delete adoubleValidator_speed;
    delete ui;
}

void keycreatcDlg::init_dlg_show()
{
    ui->pos1comboBox->clear();
    for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
    {
        ui->pos1comboBox->addItem(m_mcs->project->project_robpos_trace[n].name);
    }
    ui->pos2comboBox->clear();
    for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
    {
        ui->pos2comboBox->addItem(m_mcs->project->project_robpos_trace[n].name);
    }
    ui->pos3comboBox->clear();
    for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
    {
        ui->pos3comboBox->addItem(m_mcs->project->project_robpos_trace[n].name);
    }
    ui->record->clear();
}

void keycreatcDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    ui->pos1comboBox->clear();
    for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
    {
        ui->pos1comboBox->addItem(m_mcs->project->project_robpos_trace[n].name);
    }
    ui->pos2comboBox->clear();
    for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
    {
        ui->pos2comboBox->addItem(m_mcs->project->project_robpos_trace[n].name);
    }
    ui->pos3comboBox->clear();
    for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
    {
        ui->pos3comboBox->addItem(m_mcs->project->project_robpos_trace[n].name);
    }
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_CREATC_KEY)//生成圆弧跟踪轨迹指令
        {
            QString name=cmd.cmd_creatc_name;//获取到的生成的轨迹名字
            std::vector<QString> pointsname=cmd.cmd_creatc_pointsname;//获取到要点的轨道名字
            float speed=cmd.cmd_creatc_speed;
            int time=cmd.cmd_creatc_time;
            std::vector<int> point_trace_num(pointsname.size());
            for(int m=0;m<pointsname.size();m++)
            {
                for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
                {
                    if(pointsname[m]==m_mcs->project->project_robpos_trace[n].name)
                    {
                        point_trace_num[m]=n;//找到要储存的点位下标
                        break;
                    }
                }
            }
            if(point_trace_num[0]>=0&&point_trace_num[0]<ui->pos1comboBox->count())
            {
                ui->pos1comboBox->setCurrentIndex(point_trace_num[0]);
            }
            if(point_trace_num[1]>=0&&point_trace_num[1]<ui->pos2comboBox->count())
            {
                ui->pos2comboBox->setCurrentIndex(point_trace_num[1]);
            }
            if(point_trace_num[2]>=0&&point_trace_num[2]<ui->pos3comboBox->count())
            {
                ui->pos3comboBox->setCurrentIndex(point_trace_num[2]);
            }
            ui->creatcnamelineEdit->setText(name);
            ui->creatcspeedlineEdit->setText(QString::number(speed,'f',ROBOT_SPEED_DECIMAL_PLACE));
            ui->creatctimelineEdit->setText(QString::number(time));
        }
    }
    ui->record->clear();
}

void keycreatcDlg::close_dlg_show()
{

}

void keycreatcDlg::setbutton(int name)
{
    if(name==0)
    {
        b_inster=false;
        ui->creatcBtn->setText(QStringLiteral("插入生成跟踪轨迹指令"));
    }
    else
    {
        b_inster=true;
        ui->creatcBtn->setText(QStringLiteral("替换生成跟踪轨迹指令"));
    }
}

void keycreatcDlg::on_creatcBtn_clicked()
{
    QString name=ui->creatcnamelineEdit->text();
    std::vector<QString> pointsname(3);
    float speed;
    int time;
    int route;
    bool rc;
    route=ui->pos1comboBox->currentIndex();
    if(route<0||route>ui->pos1comboBox->count()-1)
    {
        ui->record->append(QStringLiteral("请选择圆弧起点点位"));
        return;
    }
    pointsname[0]=ui->pos1comboBox->currentText();
    route=ui->pos2comboBox->currentIndex();
    if(route<0||route>ui->pos2comboBox->count()-1)
    {
        ui->record->append(QStringLiteral("请选择圆弧中间点位"));
        return;
    }
    pointsname[1]=ui->pos2comboBox->currentText();
    route=ui->pos3comboBox->currentIndex();
    if(route<0||route>ui->pos3comboBox->count()-1)
    {
        ui->record->append(QStringLiteral("请选择圆弧终点点位"));
        return;
    }
    pointsname[2]=ui->pos3comboBox->currentText();
    if(ui->creatcspeedlineEdit->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写采样速度"));
        return;
    }
    speed=ui->creatcspeedlineEdit->text().toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("采样速度格式出错"));
        return;
    }
    if(ui->creatctimelineEdit->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写采样间隔"));
        return;
    }
    time=ui->creatctimelineEdit->text().toInt(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("采样间隔格式出错"));
        return;
    }
    if(ui->creatcnamelineEdit->text().isEmpty())
    {
        ui->record->append(QStringLiteral("请填写生成轨迹名称"));
        return;
    }
    if(b_inster==false)
    {
        for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
        {
            if(name==m_mcs->project->project_robpos_trace[n].name)
            {
                ui->record->append(QStringLiteral("生成的点坐标与已有的点坐标重名"));
                return;
            }
        }
        if(false==b_nosame_vector_QString(pointsname))
        {
            ui->record->append(QStringLiteral("圆弧起点、终点、中间点不能有相同点"));
            return;
        }
    }
    my_cmd cmd;
    QString msg=cmd.cmd_creatc(pointsname,speed,time,name);
    ui->record->append(QStringLiteral("插入生成轨迹指令成功"));
    cmd_msg=msg;
    done(1);
}

