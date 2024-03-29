#include "setcraft1dlg.h"
#include "ui_setcraft1dlg.h"

setcraft1Dlg::setcraft1Dlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setcraft1Dlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

//美化界面
#ifdef OPEN_BEAUTIFY_INTERFACE
    ui->lineEdit_centerRX->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_centerRY->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_centerRZ->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_X->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_Y->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_Z->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_centerX->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_centerY->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_centerZ->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_edRX->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_edRY->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_edRZ->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_edX->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_edY->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_edZ->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_stRX->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_stRY->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_stRZ->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_stX->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_stY->setStyleSheet(FONT_EDIT_INFO);
    ui->lineEdit_stZ->setStyleSheet(FONT_EDIT_INFO);
    ui->pushButtonOK->setStyleSheet(FONT_BUTTON_INFO);
    ui->pushButton_del->setStyleSheet(FONT_BUTTON_INFO);
    ui->pushButton_get_centerpos->setStyleSheet(FONT_BUTTON_INFO);
    ui->pushButton_get_edpos->setStyleSheet(FONT_BUTTON_INFO);
    ui->pushButton_get_stpos->setStyleSheet(FONT_BUTTON_INFO);
    ui->pushButton_updata_centerpos->setStyleSheet(FONT_BUTTON_INFO);
    ui->pushButton_updata_edpos->setStyleSheet(FONT_BUTTON_INFO);
    ui->pushButton_updata_stpos->setStyleSheet(FONT_BUTTON_INFO);
    ui->robposlist->setStyleSheet(FONT_LISTWIDGET_INFO);
    ui->record->setStyleSheet(FONT_TEXTBROWERS_INFO);
#endif

    adoubleValidator_3 = new QDoubleValidator(0,9999,3,this);//限制3位小数
    adoubleValidator_pose = new QDoubleValidator(ROBOT_POSE_DECIMAL_BOTTOM,ROBOT_POSE_DECIMAL_TOP,ROBOT_POSE_DECIMAL_PLACE,this);//限制3位小数
    adoubleValidator_posture = new QDoubleValidator(ROBOT_POSTURE_DECIMAL_BOTTOM,ROBOT_POSTURE_DECIMAL_TOP,ROBOT_POSTURE_DECIMAL_PLACE,this);//限制4位小数

    ui->lineEdit_stX->setValidator(adoubleValidator_pose);
    ui->lineEdit_stY->setValidator(adoubleValidator_pose);
    ui->lineEdit_stZ->setValidator(adoubleValidator_pose);
    ui->lineEdit_stRX->setValidator(adoubleValidator_posture);
    ui->lineEdit_stRY->setValidator(adoubleValidator_posture);
    ui->lineEdit_stRZ->setValidator(adoubleValidator_posture);
    ui->lineEdit_edX->setValidator(adoubleValidator_pose);
    ui->lineEdit_edY->setValidator(adoubleValidator_pose);
    ui->lineEdit_edZ->setValidator(adoubleValidator_pose);
    ui->lineEdit_edRX->setValidator(adoubleValidator_posture);
    ui->lineEdit_edRY->setValidator(adoubleValidator_posture);
    ui->lineEdit_edRZ->setValidator(adoubleValidator_posture);
    ui->lineEdit_centerX->setValidator(adoubleValidator_pose);
    ui->lineEdit_centerY->setValidator(adoubleValidator_pose);
    ui->lineEdit_centerZ->setValidator(adoubleValidator_pose);
    ui->lineEdit_centerRX->setValidator(adoubleValidator_posture);
    ui->lineEdit_centerRY->setValidator(adoubleValidator_posture);
    ui->lineEdit_centerRZ->setValidator(adoubleValidator_posture);
    ui->lineEdit_X->setValidator(adoubleValidator_pose);
    ui->lineEdit_Y->setValidator(adoubleValidator_pose);
    ui->lineEdit_Z->setValidator(adoubleValidator_pose);
}

setcraft1Dlg::~setcraft1Dlg()
{
    delete adoubleValidator_3;
    delete adoubleValidator_pose;
    delete adoubleValidator_posture;
    delete ui;
}

void setcraft1Dlg::init_dlg_show(bool b_file)
{
    this->b_file=b_file;
    if(m_mcs->craft->posturelist.size()>=2)
    {
        ui->lineEdit_stX->setText(QString::number(m_mcs->craft->posturelist[0].posture.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_stY->setText(QString::number(m_mcs->craft->posturelist[0].posture.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_stZ->setText(QString::number(m_mcs->craft->posturelist[0].posture.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_stRX->setText(QString::number(m_mcs->craft->posturelist[0].posture.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_stRY->setText(QString::number(m_mcs->craft->posturelist[0].posture.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_stRZ->setText(QString::number(m_mcs->craft->posturelist[0].posture.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));

        ui->lineEdit_centerX->setText(QString::number(m_mcs->rob->TCPpos.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_centerY->setText(QString::number(m_mcs->rob->TCPpos.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_centerZ->setText(QString::number(m_mcs->rob->TCPpos.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_centerRX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_centerRY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_centerRZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));

        ui->lineEdit_edX->setText(QString::number(m_mcs->craft->posturelist[m_mcs->craft->posturelist.size()-1].posture.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_edY->setText(QString::number(m_mcs->craft->posturelist[m_mcs->craft->posturelist.size()-1].posture.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_edZ->setText(QString::number(m_mcs->craft->posturelist[m_mcs->craft->posturelist.size()-1].posture.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_edRX->setText(QString::number(m_mcs->craft->posturelist[m_mcs->craft->posturelist.size()-1].posture.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_edRY->setText(QString::number(m_mcs->craft->posturelist[m_mcs->craft->posturelist.size()-1].posture.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_edRZ->setText(QString::number(m_mcs->craft->posturelist[m_mcs->craft->posturelist.size()-1].posture.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));

    }
    else
    {
        ui->lineEdit_stX->setText(QString::number(m_mcs->rob->TCPpos.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_stY->setText(QString::number(m_mcs->rob->TCPpos.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_stZ->setText(QString::number(m_mcs->rob->TCPpos.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_stRX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_stRY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_stRZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));

        ui->lineEdit_centerX->setText(QString::number(m_mcs->rob->TCPpos.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_centerY->setText(QString::number(m_mcs->rob->TCPpos.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_centerZ->setText(QString::number(m_mcs->rob->TCPpos.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_centerRX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_centerRY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_centerRZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));

        ui->lineEdit_edX->setText(QString::number(m_mcs->rob->TCPpos.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_edY->setText(QString::number(m_mcs->rob->TCPpos.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_edZ->setText(QString::number(m_mcs->rob->TCPpos.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_edRX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_edRY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_edRZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    }
    ui->lineEdit_X->setText(QString::number(0,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    ui->lineEdit_Y->setText(QString::number(0,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    ui->lineEdit_Z->setText(QString::number(0,'f',ROBOT_POSTURE_DECIMAL_PLACE));

    if(m_mcs->craft->posturelist.size()>1)
    {
        now_robpos=0;
    }
    else
    {
        now_robpos=m_mcs->craft->posturelist.size()-1;
    }
    ui->record->clear();
    updataRoblistUi();
}

void setcraft1Dlg::close_dlg_show()
{

}

void setcraft1Dlg::on_pushButton_get_stpos_clicked()//获取起点坐标姿态
{
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
        ui->record->append(QStringLiteral("获取起点坐标失败"));
    }
    else
    {
        ui->lineEdit_stX->setText(QString::number(m_mcs->rob->TCPpos.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_stY->setText(QString::number(m_mcs->rob->TCPpos.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_stZ->setText(QString::number(m_mcs->rob->TCPpos.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_stRX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_stRY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_stRZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->record->append(QStringLiteral("获取起点坐标成功"));
    }
}


void setcraft1Dlg::on_pushButton_updata_stpos_clicked()//更新起点坐标姿态
{
    ChangeRobPosVariable robpos;
    robpos.posture.X=ui->lineEdit_stX->text().toFloat();
    robpos.posture.Y=ui->lineEdit_stY->text().toFloat();
    robpos.posture.Z=ui->lineEdit_stZ->text().toFloat();
    robpos.posture.RX=ui->lineEdit_stRX->text().toFloat();
    robpos.posture.RY=ui->lineEdit_stRY->text().toFloat();
    robpos.posture.RZ=ui->lineEdit_stRZ->text().toFloat();
    robpos.Variable.X=ui->lineEdit_X->text().toFloat();
    robpos.Variable.Y=ui->lineEdit_Y->text().toFloat();
    robpos.Variable.Z=ui->lineEdit_Z->text().toFloat();
    if(m_mcs->craft->posturelist.size()>=1)
    {
        m_mcs->craft->posturelist[0]=robpos;
    }
    else
    {
        m_mcs->craft->posturelist.push_back(robpos);
        now_robpos++;
    }
    updataRoblistUi();
    ui->record->append(QStringLiteral("更新起点坐标成功"));
}


void setcraft1Dlg::on_pushButton_get_edpos_clicked()//获取终点坐标姿态
{
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
        ui->record->append(QStringLiteral("获取终点坐标失败"));
    }
    else
    {
        ui->lineEdit_edX->setText(QString::number(m_mcs->rob->TCPpos.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_edY->setText(QString::number(m_mcs->rob->TCPpos.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_edZ->setText(QString::number(m_mcs->rob->TCPpos.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_edRX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_edRY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_edRZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->record->append(QStringLiteral("获取终点坐标成功"));
    }
}


void setcraft1Dlg::on_pushButton_updata_edpos_clicked()//更新终点坐标姿态
{
    if(m_mcs->craft->posturelist.size()==0)
    {
        ui->record->append(QStringLiteral("请先添加起点坐标"));
        return;
    }
    ChangeRobPosVariable robpos;
    robpos.posture.X=ui->lineEdit_edX->text().toFloat();
    robpos.posture.Y=ui->lineEdit_edY->text().toFloat();
    robpos.posture.Z=ui->lineEdit_edZ->text().toFloat();
    robpos.posture.RX=ui->lineEdit_edRX->text().toFloat();
    robpos.posture.RY=ui->lineEdit_edRY->text().toFloat();
    robpos.posture.RZ=ui->lineEdit_edRZ->text().toFloat();
    robpos.Variable.X=ui->lineEdit_X->text().toFloat();
    robpos.Variable.Y=ui->lineEdit_Y->text().toFloat();
    robpos.Variable.Z=ui->lineEdit_Z->text().toFloat();
    if(m_mcs->craft->posturelist.size()>=2)
    {
        m_mcs->craft->posturelist[m_mcs->craft->posturelist.size()-1]=robpos;
        updataRoblistUi();
        ui->record->append(QStringLiteral("更新终点坐标成功"));
    }
    else if(m_mcs->craft->posturelist.size()==1)
    {
        m_mcs->craft->posturelist.push_back(robpos);
        now_robpos=0;
        updataRoblistUi();
        ui->record->append(QStringLiteral("更新终点坐标成功"));
    }
}


void setcraft1Dlg::on_pushButton_get_centerpos_clicked()//获取中间点坐标姿态
{
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
        ui->record->append(QStringLiteral("获取中间点坐标失败"));
    }
    else
    {
        ui->lineEdit_centerX->setText(QString::number(m_mcs->rob->TCPpos.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_centerY->setText(QString::number(m_mcs->rob->TCPpos.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_centerZ->setText(QString::number(m_mcs->rob->TCPpos.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_centerRX->setText(QString::number(m_mcs->rob->TCPpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_centerRY->setText(QString::number(m_mcs->rob->TCPpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_centerRZ->setText(QString::number(m_mcs->rob->TCPpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->record->append(QStringLiteral("获取中间点坐标成功"));
    }
}


void setcraft1Dlg::on_pushButton_updata_centerpos_clicked()//插入中间点坐标姿态
{
    if(m_mcs->craft->posturelist.size()<1)
    {
        ui->record->append(QStringLiteral("请先添加起点坐标"));
        return;
    }
    else if(m_mcs->craft->posturelist.size()<2)
    {
        ui->record->append(QStringLiteral("请先添加终点坐标"));
        return;
    }
    ChangeRobPosVariable robpos;
    robpos.posture.X=ui->lineEdit_centerX->text().toFloat();
    robpos.posture.Y=ui->lineEdit_centerY->text().toFloat();
    robpos.posture.Z=ui->lineEdit_centerZ->text().toFloat();
    robpos.posture.RX=ui->lineEdit_centerRX->text().toFloat();
    robpos.posture.RY=ui->lineEdit_centerRY->text().toFloat();
    robpos.posture.RZ=ui->lineEdit_centerRZ->text().toFloat();
    robpos.Variable.X=ui->lineEdit_X->text().toFloat();
    robpos.Variable.Y=ui->lineEdit_Y->text().toFloat();
    robpos.Variable.Z=ui->lineEdit_Z->text().toFloat();
    if(now_robpos==m_mcs->craft->posturelist.size()-1)
    {
        ui->record->append(QStringLiteral("不能在终点后方插入中间点坐标姿态"));
        return;
    }
    else
    {
        m_mcs->craft->posturelist.insert(m_mcs->craft->posturelist.begin()+now_robpos+1,robpos);
    }
    ui->record->append(QStringLiteral("插入中间点坐标成功"));
    now_robpos++;
    updataRoblistUi();
}


void setcraft1Dlg::on_pushButton_del_clicked()//删除选中姿态
{
    if(now_robpos>0&&m_mcs->craft->posturelist.size()-1!=now_robpos&&m_mcs->craft->posturelist.size()>now_robpos)
    {
        m_mcs->craft->posturelist.erase(m_mcs->craft->posturelist.begin()+now_robpos);
        ui->record->append(QStringLiteral("删除中间点坐标成功"));
        if(now_robpos>=m_mcs->craft->posturelist.size())
            now_robpos=now_robpos-1;
        updataRoblistUi();
    }
    else if(now_robpos==0)
    {
        ui->record->append(QStringLiteral("起点姿态不能删除"));
    }
    else if(m_mcs->craft->posturelist.size()-1==now_robpos)
    {
        ui->record->append(QStringLiteral("终点姿态不能删除"));
    }
    else
    {
        ui->record->append(QStringLiteral("请先选中要删除的激光头点"));
    }
}


void setcraft1Dlg::on_pushButtonOK_clicked()//确定并保存
{
    std::vector<ChangeRobPosVariable> tempposturelist;
    QString msg;
    if(m_mcs->craft->posturelist.size()<2)
    {
        ui->record->append(QStringLiteral("至少添加起点和终点姿态坐标"));
        return;
    }
    //检查姿态位置是否合理
    if(0!=m_mcs->craft->tidyup_posturelist(m_mcs->craft->posturelist,tempposturelist,msg))
    {
        ui->record->append(msg);
        return;
    }
    if(b_file==true)
        m_mcs->craft->SaveProject(m_mcs->craft->craft_path);
    else
    {
        my_cmd cmd;
        std::vector<float> param;
        cmd_msg=cmd.cmd_crafts(CRAFT_ID_STARTENDCHANGE_POSTURE,m_mcs->craft->posturelist,param,m_mcs->craft->craft_name);
    }
    done(1);
}

void setcraft1Dlg::updataRoblistUi()
{
    ui->robposlist->clear();
    QString msg;
    for(int n=0;n<m_mcs->craft->posturelist.size();n++)
    {
        if(n==0)
        {
            QString msg=QStringLiteral("起点坐标姿态: (")+QString::number(m_mcs->craft->posturelist[0].posture.X,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                QString::number(m_mcs->craft->posturelist[0].posture.Y,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                QString::number(m_mcs->craft->posturelist[0].posture.Z,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                QString::number(m_mcs->craft->posturelist[0].posture.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                                QString::number(m_mcs->craft->posturelist[0].posture.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                                QString::number(m_mcs->craft->posturelist[0].posture.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+") 补偿: ("+
                                                                QString::number(m_mcs->craft->posturelist[0].Variable.X,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                QString::number(m_mcs->craft->posturelist[0].Variable.Y,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                QString::number(m_mcs->craft->posturelist[0].Variable.Z,'f',ROBOT_POSE_DECIMAL_PLACE)+")";
            ui->robposlist->addItem(msg);
        }
        else if(n==m_mcs->craft->posturelist.size()-1)
        {
            QString msg=QStringLiteral("终点坐标姿态: (")+QString::number(m_mcs->craft->posturelist[m_mcs->craft->posturelist.size()-1].posture.X,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                QString::number(m_mcs->craft->posturelist[m_mcs->craft->posturelist.size()-1].posture.Y,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                QString::number(m_mcs->craft->posturelist[m_mcs->craft->posturelist.size()-1].posture.Z,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                QString::number(m_mcs->craft->posturelist[m_mcs->craft->posturelist.size()-1].posture.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                                QString::number(m_mcs->craft->posturelist[m_mcs->craft->posturelist.size()-1].posture.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                                QString::number(m_mcs->craft->posturelist[m_mcs->craft->posturelist.size()-1].posture.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+") 补偿: ("+
                                                                QString::number(m_mcs->craft->posturelist[m_mcs->craft->posturelist.size()-1].Variable.X,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                QString::number(m_mcs->craft->posturelist[m_mcs->craft->posturelist.size()-1].Variable.Y,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                QString::number(m_mcs->craft->posturelist[m_mcs->craft->posturelist.size()-1].Variable.Z,'f',ROBOT_POSE_DECIMAL_PLACE)+")";
            ui->robposlist->addItem(msg);
        }
        else
        {
            QString msg=QStringLiteral("中间姿态")+QString::number(n)+": ("+QString::number(m_mcs->craft->posturelist[n].posture.X,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                                   QString::number(m_mcs->craft->posturelist[n].posture.Y,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                                   QString::number(m_mcs->craft->posturelist[n].posture.Z,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                                   QString::number(m_mcs->craft->posturelist[n].posture.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                                                   QString::number(m_mcs->craft->posturelist[n].posture.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE)+","+
                                                                                   QString::number(m_mcs->craft->posturelist[n].posture.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE)+") 补偿: ("+
                                                                                   QString::number(m_mcs->craft->posturelist[n].Variable.X,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                                   QString::number(m_mcs->craft->posturelist[n].Variable.Y,'f',ROBOT_POSE_DECIMAL_PLACE)+","+
                                                                                   QString::number(m_mcs->craft->posturelist[n].Variable.Z,'f',ROBOT_POSE_DECIMAL_PLACE)+")";
            ui->robposlist->addItem(msg);
        }
    }
    if(m_mcs->craft->posturelist.size()>0)
    {
        ui->robposlist->setCurrentRow(now_robpos);
    }
}

void setcraft1Dlg::on_robposlist_itemClicked(QListWidgetItem *item)
{
    now_robpos=ui->robposlist->currentRow();
    if(m_mcs->craft->posturelist.size()>2&&now_robpos>0&&now_robpos<m_mcs->craft->posturelist.size()-1)
    {
        ui->lineEdit_centerX->setText(QString::number(m_mcs->craft->posturelist[now_robpos].posture.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_centerY->setText(QString::number(m_mcs->craft->posturelist[now_robpos].posture.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_centerZ->setText(QString::number(m_mcs->craft->posturelist[now_robpos].posture.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_centerRX->setText(QString::number(m_mcs->craft->posturelist[now_robpos].posture.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_centerRY->setText(QString::number(m_mcs->craft->posturelist[now_robpos].posture.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_centerRZ->setText(QString::number(m_mcs->craft->posturelist[now_robpos].posture.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    }
    else if(m_mcs->craft->posturelist.size()>=2&&now_robpos==m_mcs->craft->posturelist.size())
    {
        ui->lineEdit_edX->setText(QString::number(m_mcs->craft->posturelist[now_robpos].posture.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_edY->setText(QString::number(m_mcs->craft->posturelist[now_robpos].posture.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_edZ->setText(QString::number(m_mcs->craft->posturelist[now_robpos].posture.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_edRX->setText(QString::number(m_mcs->craft->posturelist[now_robpos].posture.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_edRY->setText(QString::number(m_mcs->craft->posturelist[now_robpos].posture.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_edRZ->setText(QString::number(m_mcs->craft->posturelist[now_robpos].posture.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    }
    else if(m_mcs->craft->posturelist.size()>=1&&now_robpos==0)
    {
        ui->lineEdit_stX->setText(QString::number(m_mcs->craft->posturelist[now_robpos].posture.X,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_stY->setText(QString::number(m_mcs->craft->posturelist[now_robpos].posture.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_stZ->setText(QString::number(m_mcs->craft->posturelist[now_robpos].posture.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
        ui->lineEdit_stRX->setText(QString::number(m_mcs->craft->posturelist[now_robpos].posture.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_stRY->setText(QString::number(m_mcs->craft->posturelist[now_robpos].posture.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_stRZ->setText(QString::number(m_mcs->craft->posturelist[now_robpos].posture.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    }
    if(now_robpos>=0&&now_robpos<m_mcs->craft->posturelist.size())
    {
        ui->lineEdit_X->setText(QString::number(m_mcs->craft->posturelist[now_robpos].Variable.X,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_Y->setText(QString::number(m_mcs->craft->posturelist[now_robpos].Variable.Y,'f',ROBOT_POSTURE_DECIMAL_PLACE));
        ui->lineEdit_Z->setText(QString::number(m_mcs->craft->posturelist[now_robpos].Variable.Z,'f',ROBOT_POSTURE_DECIMAL_PLACE));
    }
}


void setcraft1Dlg::UpdataUi()
{

}

