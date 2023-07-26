#include "keywelddlg.h"
#include "ui_keywelddlg.h"

keyweldDlg::keyweldDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keyweldDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸
}

keyweldDlg::~keyweldDlg()
{
    delete ui;
}

void keyweldDlg::init_dlg_show()
{
    ui->record->clear();
}

void keyweldDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_WELD_KEY)//起光弧指令
        {
            Alternatingcurrent elem=cmd.cmd_elec_elem;  //获取到焊机交变电流模式
            float eled=cmd.cmd_elec_eled; //获取到焊机电流
            float elev=cmd.cmd_elec_elev; //获取到焊机电压
            if(elem>=0&&elem<=ui->weldermodelcombo->count())
            {
                ui->weldermodelcombo->setCurrentIndex(elem);
            }
            ui->weldercurrent->setText(QString::number(eled));
            ui->weldercurrent_v->setText(QString::number(elev));
        }
    }
    ui->record->clear();
}

void keyweldDlg::close_dlg_show()
{

}

void keyweldDlg::setbutton(int name)
{
    if(name==0)
    {
        ui->welderarcingBtn->setText(QString::fromLocal8Bit("插入起弧指令"));
        ui->welderarcoutBtn->setText(QString::fromLocal8Bit("插入熄弧指令"));
    }
    else
    {
        ui->welderarcingBtn->setText(QString::fromLocal8Bit("替换起弧指令"));
        ui->welderarcoutBtn->setText(QString::fromLocal8Bit("替换熄弧指令"));
    }
}

//起弧指令
void keyweldDlg::on_welderarcingBtn_clicked()
{
    bool rc;
    float eled=ui->weldercurrent->text().toFloat(&rc);
    float elev=ui->weldercurrent_v->text().toFloat(&rc);
    Alternatingcurrent elem=(Alternatingcurrent)ui->weldermodelcombo->currentIndex();
    my_cmd cmd;
    QString msg=cmd.cmd_elec(eled,elev,elem,1);
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
    if(ui->weldercurrent_v->text().isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写电压值"));
        return;
    }
    if(rc==false)
    {
        ui->record->append(QString::fromLocal8Bit("电压值格式出错"));
        return;
    }
    ui->record->append(QString::fromLocal8Bit("插入起弧指令成功"));
    cmd_msg=msg;
    done(1);
}

//熄弧指令
void keyweldDlg::on_welderarcoutBtn_clicked()
{
    my_cmd cmd;
    QString msg=cmd.cmd_elec_work(0);
    ui->record->append(QString::fromLocal8Bit("插入息弧指令成功"));
    cmd_msg=msg;
    done(1);
}

