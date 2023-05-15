#include "keycoorddlg.h"
#include "ui_keycoorddlg.h"

keycoordDlg::keycoordDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keycoordDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
}

keycoordDlg::~keycoordDlg()
{
    delete ui;
}

void keycoordDlg::init_dlg_show()
{
    ui->record->clear();
    ui->comboBox_pointo->clear();
    ui->comboBox_pointx->clear();
    for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
    {
        ui->comboBox_pointo->addItem(m_mcs->project->projecr_robpos_trace[n].name);
        ui->comboBox_pointx->addItem(m_mcs->project->projecr_robpos_trace[n].name);
    }
    cmd_list_in.clear();
}

void keycoordDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    cmd_list_in=cmdlist;
    ui->comboBox_pointo->clear();
    ui->comboBox_pointx->clear();
    for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
    {
        ui->comboBox_pointo->addItem(m_mcs->project->projecr_robpos_trace[n].name);
        ui->comboBox_pointx->addItem(m_mcs->project->projecr_robpos_trace[n].name);
    }
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_COORD_KEY)//定位指令
        {
            QString s_pointx=cmd.cmd_coord_pointx;
            QString s_pointo=cmd.cmd_coord_pointo;
            QString name=cmd.cmd_coord_name;
            int pointx_trace_num;//搜索到的s_pointx序号
            int pointo_trace_num;//搜索到的s_pointo序号
            //这里添加移动命令
            for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
            {
                if(s_pointx==m_mcs->project->projecr_robpos_trace[n].name)
                {
                    pointx_trace_num=n;//找到要储存的焊接轨道下标
                    break;
                }
            }
            if(pointx_trace_num>=0&&pointx_trace_num<ui->comboBox_pointx->count())
            {
                ui->comboBox_pointx->setCurrentIndex(pointx_trace_num);
            }
            for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
            {
                if(s_pointo==m_mcs->project->projecr_robpos_trace[n].name)
                {
                    pointo_trace_num=n;//找到要储存的焊接轨道下标
                    break;
                }
            }
            if(pointo_trace_num>=0&&pointx_trace_num<ui->comboBox_pointo->count())
            {
                ui->comboBox_pointo->setCurrentIndex(pointo_trace_num);
            }
            ui->name->setText(name);
        }
    }
    ui->record->clear();
}

void keycoordDlg::close_dlg_show()
{

}

void keycoordDlg::setbutton(int name)
{
    if(name==0)
    {
        b_inster=false;
        ui->pushButton->setText(QString::fromLocal8Bit("插入定位指令"));
    }
    else
    {
        b_inster=true;
        ui->pushButton->setText(QString::fromLocal8Bit("替换定位指令"));
    }
}


void keycoordDlg::on_pushButton_clicked()
{
    int route_pointo=ui->comboBox_pointo->currentIndex();
    QString name_pointo=ui->comboBox_pointo->currentText();
    int route_pointx=ui->comboBox_pointx->currentIndex();
    QString name_pointx=ui->comboBox_pointx->currentText();
    QString name=ui->name->text();
    my_cmd cmd;
    QString msg;
    if(route_pointo<0||route_pointo>ui->comboBox_pointo->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择一个坐标系零点"));
        return;
    }
    if(route_pointx<0||route_pointx>ui->comboBox_pointx->count()-1)
    {
        ui->record->append(QString::fromLocal8Bit("请选择一个坐标系X方向点"));
        return;
    }
    if(name_pointo==name_pointx)
    {
        ui->record->append(QString::fromLocal8Bit("坐标系零点和坐标系X方向点不能为同一个点"));
        return;
    }
    if(ui->name->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写生成的定位矩阵名称"));
        return;
    }
    if(b_inster==false)//插入
    {
        std::vector<QString> err_msg;
        m_mcs->tosendbuffer->cmdlist_creat_tracename_mem(m_mcs->project->project_cmdlist.size(),err_msg);
        for(int n=0;n<m_mcs->project->projecr_coord_matrix4d.size();n++)
        {
            if(name==m_mcs->project->projecr_coord_matrix4d[n].name)
            {
                ui->record->append(QString::fromLocal8Bit("生成的定位矩阵名称与已有的矩阵重名"));
                return;
            }
        }
    }
    msg=cmd.cmd_coord(name_pointx,name_pointo,name);
    cmd_msg=msg;
    done(1);
}

