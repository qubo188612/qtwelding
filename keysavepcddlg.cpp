#include "keysavepcddlg.h"
#include "ui_keysavepcddlg.h"

keysavepcdDlg::keysavepcdDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keysavepcdDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;

    for(int n=0;n<SAVEPCD_EDIT_ID_TOTAL_NUM;n++)
    {
        QString msg=Savepcd_edit_mode_toQString((Savepcd_edit_mode)n);
        ui->savepcdmodecombo->addItem(msg);
    }
}

keysavepcdDlg::~keysavepcdDlg()
{
    delete ui;
}

void keysavepcdDlg::init_dlg_show()
{
    ui->pointslist->clear();
    ui->scanlist->clear();
    ui->creatslist->clear();
    ui->traceslist->clear();

    ui->pointscomboBox->clear();
    ui->scancomboBox->clear();
    ui->creatscomboBox->clear();
    ui->tracescomboBox->clear();
    for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
    {
        ui->pointscomboBox->addItem(m_mcs->project->project_robpos_trace[n].name);
    }
    for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
    {
        ui->scancomboBox->addItem(m_mcs->project->project_scan_trace[n].name);
    }
    for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
    {
        ui->creatscomboBox->addItem(m_mcs->project->project_weld_trace[n].name);
    }
    for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
    {
        ui->tracescomboBox->addItem(m_mcs->project->project_interweld_trace[n].name);
    }

    ui->record->clear();
}

void keysavepcdDlg::init_dlg_show(QString cmdlist)
{
    ui->pointslist->clear();
    ui->scanlist->clear();
    ui->creatslist->clear();
    ui->traceslist->clear();

    ui->pointscomboBox->clear();
    ui->scancomboBox->clear();
    ui->creatscomboBox->clear();
    ui->tracescomboBox->clear();
    for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
    {
        ui->pointscomboBox->addItem(m_mcs->project->project_robpos_trace[n].name);
    }
    for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
    {
        ui->scancomboBox->addItem(m_mcs->project->project_scan_trace[n].name);
    }
    for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
    {
        ui->creatscomboBox->addItem(m_mcs->project->project_weld_trace[n].name);
    }
    for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
    {
        ui->tracescomboBox->addItem(m_mcs->project->project_interweld_trace[n].name);
    }
    QString msg,key;
    my_cmd cmd;
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_SAVEPCD_KEY)//计算点坐标指令
        {
            //这里添加点坐标
            QString name=cmd.cmd_savepcd_name;//获取到的保存点云名字
            Savepcd_edit_mode mode=cmd.cmd_savepcd_mode;//获取到的保存点云模式
            ui->savepcdname->setText(name);
            if(mode>=0&&mode<ui->savepcdmodecombo->count())
            {
                ui->savepcdmodecombo->setCurrentIndex(mode);
            }
            std::vector<QString> scanname=cmd.cmd_savepcd_scanname;//保存点云扫描轨迹
            std::vector<QString> pointsname=cmd.cmd_savepcd_pointsname;//保存点云点轨迹
            std::vector<QString> creatsname=cmd.cmd_savepcd_creatsname;//保存点云焊缝轨迹
            std::vector<QString> tracesname=cmd.cmd_savepcd_tracesname;//保存点云工艺轨迹
            for(int n=0;n<scanname.size();n++)
            {
                ui->scanlist->addItem(scanname[n]);
            }
            for(int n=0;n<pointsname.size();n++)
            {
                ui->pointslist->addItem(pointsname[n]);
            }
            for(int n=0;n<creatsname.size();n++)
            {
                ui->creatslist->addItem(creatsname[n]);
            }
            for(int n=0;n<tracesname.size();n++)
            {
                ui->traceslist->addItem(tracesname[n]);
            }
        }
    }
    ui->record->clear();
}

void keysavepcdDlg::close_dlg_show()
{

}

void keysavepcdDlg::setbutton(int name)
{
    if(name==0)
    {
        ui->savepcdBtn->setText(QString::fromLocal8Bit("插入保存点云指令"));
    }
    else
    {
        ui->savepcdBtn->setText(QString::fromLocal8Bit("替换保存点云指令"));
    }
}

//生成保存点云指令
void keysavepcdDlg::on_savepcdBtn_clicked()
{
    QString name=ui->savepcdname->text();
    if(name.isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请输入保存名称"));
        return;
    }
    Savepcd_edit_mode mode=(Savepcd_edit_mode)ui->savepcdmodecombo->currentIndex();
    if(mode<0||mode>=ui->savepcdmodecombo->count())
    {
        ui->record->append(QString::fromLocal8Bit("请选择保存模式"));
        return;
    }
    std::vector<QString> scanname;
    std::vector<QString> pointsname;
    std::vector<QString> creatsname;
    std::vector<QString> tracesname;

    for(int n=0;n<ui->pointslist->count();n++)
    {
        pointsname.push_back(ui->pointslist->item(n)->text());
    }
    if(false==b_nosame_vector_QString(pointsname))
    {
        ui->record->append(QString::fromLocal8Bit("保存的点名称里有重复"));
        return;
    }
    for(int n=0;n<ui->scanlist->count();n++)
    {
        scanname.push_back(ui->scanlist->item(n)->text());
    }
    if(false==b_nosame_vector_QString(scanname))
    {
        ui->record->append(QString::fromLocal8Bit("保存的扫描轨迹名称里有重复"));
        return;
    }
    for(int n=0;n<ui->creatslist->count();n++)
    {
        creatsname.push_back(ui->creatslist->item(n)->text());
    }
    if(false==b_nosame_vector_QString(creatsname))
    {
        ui->record->append(QString::fromLocal8Bit("保存的跟踪轨迹名称里有重复"));
        return;
    }
    for(int n=0;n<ui->traceslist->count();n++)
    {
        tracesname.push_back(ui->traceslist->item(n)->text());
    }
    if(false==b_nosame_vector_QString(tracesname))
    {
        ui->record->append(QString::fromLocal8Bit("保存的工艺轨迹名称里有重复"));
        return;
    }

    my_cmd cmd;
    QString msg=cmd.cmd_savepcd(scanname,pointsname,creatsname,tracesname,mode,name);
    ui->record->append(QString::fromLocal8Bit("插入保存点云指令成功"));
    cmd_msg=msg;
    done(1);
}

//插入点位
void keysavepcdDlg::on_pointsAddBtn_clicked()
{
    QString name;
    if(ui->pointscomboBox->count()>0)
    {
        name=ui->pointscomboBox->currentText();
    }
    if(name.isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请选择要保存的点名称"));
        return;
    }
    ui->pointslist->addItem(name);
    ui->record->append(QString::fromLocal8Bit("保存点添加完成"));
}

//删除点位
void keysavepcdDlg::on_pointsDelBtn_clicked()
{
    int line=ui->pointslist->currentIndex().row();
    if(line<0||line>=ui->pointslist->count())
    {
        ui->record->append(QString::fromLocal8Bit("请选择要删除的点名称"));
        return;
    }
    ui->pointslist->takeItem(line);
    ui->record->append(QString::fromLocal8Bit("删除点完成"));
}

//插入扫描轨迹
void keysavepcdDlg::on_scanAddBtn_clicked()
{
    QString name;
    if(ui->scancomboBox->count()>0)
    {
        name=ui->scancomboBox->currentText();
    }
    if(name.isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请选择要保存的扫描轨迹名称"));
        return;
    }
    ui->scanlist->addItem(name);
    ui->record->append(QString::fromLocal8Bit("保存扫描轨迹添加完成"));
}

//删除扫描轨迹
void keysavepcdDlg::on_scanDelBtn_clicked()
{
    int line=ui->scanlist->currentIndex().row();
    if(line<0||line>=ui->scanlist->count())
    {
        ui->record->append(QString::fromLocal8Bit("请选择要删除的扫描轨迹名称"));
        return;
    }
    ui->scanlist->takeItem(line);
    ui->record->append(QString::fromLocal8Bit("删除扫描轨迹完成"));
}

//插入跟踪轨迹
void keysavepcdDlg::on_creatsAddBtn_clicked()
{
    QString name;
    if(ui->creatscomboBox->count()>0)
    {
        name=ui->creatscomboBox->currentText();
    }
    if(name.isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请选择要保存的跟踪轨迹名称"));
        return;
    }
    ui->creatslist->addItem(name);
    ui->record->append(QString::fromLocal8Bit("保存跟踪轨迹添加完成"));
}

//删除跟踪轨迹
void keysavepcdDlg::on_creatsDelBtn_clicked()
{
    int line=ui->creatslist->currentIndex().row();
    if(line<0||line>=ui->creatslist->count())
    {
        ui->record->append(QString::fromLocal8Bit("请选择要删除的跟踪轨迹名称"));
        return;
    }
    ui->creatslist->takeItem(line);
    ui->record->append(QString::fromLocal8Bit("删除跟踪轨迹完成"));
}

//插入工艺轨迹
void keysavepcdDlg::on_tracesAddBtn_clicked()
{
    QString name;
    if(ui->tracescomboBox->count()>0)
    {
        name=ui->tracescomboBox->currentText();
    }
    if(name.isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请选择要保存的工艺轨迹名称"));
        return;
    }
    ui->traceslist->addItem(name);
    ui->record->append(QString::fromLocal8Bit("保存工艺轨迹添加完成"));
}

//删除工艺轨迹
void keysavepcdDlg::on_tracesDelBtn_clicked()
{
    int line=ui->traceslist->currentIndex().row();
    if(line<0||line>=ui->traceslist->count())
    {
        ui->record->append(QString::fromLocal8Bit("请选择要删除的工艺轨迹名称"));
        return;
    }
    ui->traceslist->takeItem(line);
    ui->record->append(QString::fromLocal8Bit("删除工艺轨迹完成"));
}

