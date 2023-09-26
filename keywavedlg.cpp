#include "keywavedlg.h"
#include "ui_keywavedlg.h"

keywaveDlg::keywaveDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keywaveDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸
    for(int n=0;n<TREND_ID_TOTAL_NUM;n++)
    {
        QString msg=Trend_mode_toQString((Trend_mode)n);
        ui->wavetrend_modecombo->addItem(msg);
    }

    for(int n=0;n<PENDULUM_ID_TOTAL_NUM;n++)
    {
        QString msg=Pendulum_mode_toQString((Pendulum_mode)n);
        ui->wavependulum_modecombo->addItem(msg);
    }
}

keywaveDlg::~keywaveDlg()
{
    delete ui;
}

void keywaveDlg::init_dlg_show()
{
    ui->wavefilenamecombo->clear();
    for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
    {
        ui->wavefilenamecombo->addItem(m_mcs->project->project_interweld_trace[n].name);
    }
    ui->record->clear();
}

void keywaveDlg::init_dlg_show(QString cmdlist)
{
    QString msg,key;
    my_cmd cmd;
    ui->wavefilenamecombo->clear();
    for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
    {
        ui->wavefilenamecombo->addItem(m_mcs->project->project_interweld_trace[n].name);
    }
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_WAVE_KEY)//跟踪轨迹相加指令
        {
            QString name_in=cmd.cmd_wave_namein;
            QString name_out=cmd.cmd_wave_nameout;
            wWAVEParam wave_info=cmd.cmd_wave_info;
            int weld_tracing_num_in;
            for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
            {
                if(name_in==m_mcs->project->project_interweld_trace[n].name)
                {
                    weld_tracing_num_in=n;//找到焊接轨道下标
                    break;
                }
            }
            if(weld_tracing_num_in>=0&&weld_tracing_num_in<ui->wavefilenamecombo->count())
            {
                ui->wavefilenamecombo->setCurrentIndex(weld_tracing_num_in);
            }
            if((int)wave_info.pendulum_mode>=0&&(int)wave_info.pendulum_mode<ui->wavependulum_modecombo->count())
            {
                ui->wavependulum_modecombo->setCurrentIndex(wave_info.pendulum_mode);
            }
            if((int)wave_info.trend_mode>=0&&(int)wave_info.trend_mode<ui->wavetrend_modecombo->count())
            {
                ui->wavetrend_modecombo->setCurrentIndex(wave_info.trend_mode);
            }
            if((int)wave_info.order>=0&&(int)wave_info.order<ui->waveordercombo->count())
            {
                ui->waveordercombo->setCurrentIndex(wave_info.order);
            }
            ui->waveperiod->setText(QString::number(wave_info.period,'f',3));
            ui->waveleftAmp->setText(QString::number(wave_info.leftAmp,'f',ROBOT_POSE_DECIMAL_PLACE));
            ui->waverightAmp->setText(QString::number(wave_info.rightAmp,'f',ROBOT_POSE_DECIMAL_PLACE));
            ui->waveleftAmp_z->setText(QString::number(wave_info.leftAmp_z,'f',ROBOT_POSE_DECIMAL_PLACE));
            ui->waverightAmp_z->setText(QString::number(wave_info.rightAmp_z,'f',ROBOT_POSE_DECIMAL_PLACE));
            ui->waveleftStopTime->setText(QString::number(wave_info.leftStopTime,'f',3));
            ui->waverightStopTime->setText(QString::number(wave_info.rightStopTime,'f',3));
            ui->waveanglex->setText(QString::number(wave_info.anglex,'f',ROBOT_POSTURE_DECIMAL_PLACE));
            ui->waveangley->setText(QString::number(wave_info.angley,'f',ROBOT_POSTURE_DECIMAL_PLACE));
            ui->waveStartPos->setText(QString::number(wave_info.startPos));
            ui->wavetimeGap->setText(QString::number(wave_info.timeGap));
            ui->leftaddRX->setText(QString::number(wave_info.leftAddRX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
            ui->leftaddRY->setText(QString::number(wave_info.leftAddRY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
            ui->leftaddRZ->setText(QString::number(wave_info.leftAddRZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
            ui->rightaddRX->setText(QString::number(wave_info.rightAddRX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
            ui->rightaddRY->setText(QString::number(wave_info.rightAddRY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
            ui->rightaddRZ->setText(QString::number(wave_info.rightAddRZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
            ui->wavefilename->setText(name_out);
        }
    }
    ui->record->clear();
}

void keywaveDlg::close_dlg_show()
{

}

void keywaveDlg::setbutton(int name)
{
    if(name==0)
    {
        ui->pushButton->setText(QStringLiteral("插入生成摆焊轨迹指令"));
    }
    else
    {
        ui->pushButton->setText(QStringLiteral("替换生成摆焊轨迹指令"));
    }
}

void keywaveDlg::on_pushButton_clicked()
{
    QString namein=ui->wavefilenamecombo->currentText();
    int route;
    QString nameout=ui->wavefilename->text();
    QString waveperiod=ui->waveperiod->text();
    QString waveleftAmp=ui->waveleftAmp->text();
    QString waverightAmp=ui->waverightAmp->text();
    QString waveleftAmp_z=ui->waveleftAmp_z->text();
    QString waverightAmp_z=ui->waverightAmp_z->text();
    QString waveleftStopTime=ui->waveleftStopTime->text();
    QString waverightStopTime=ui->waverightStopTime->text();
    QString waveanglex=ui->waveanglex->text();
    QString waveangley=ui->waveangley->text();
    QString waveStartPos=ui->waveStartPos->text();
    QString wavetimeGap=ui->wavetimeGap->text();
    QString leftaddRX=ui->leftaddRX->text();
    QString leftaddRY=ui->leftaddRY->text();
    QString leftaddRZ=ui->leftaddRZ->text();
    QString rightaddRX=ui->rightaddRX->text();
    QString rightaddRY=ui->rightaddRY->text();
    QString rightaddRZ=ui->rightaddRZ->text();
    wWAVEParam cmd_wave_info;
    bool rc;
    route=ui->wavefilenamecombo->currentIndex();
    if(route<0||route>ui->wavefilenamecombo->count()-1)
    {
        ui->record->append(QStringLiteral("请选择一个跟踪轨迹工艺"));
        return;
    }
    if(nameout.isEmpty())
    {
        ui->record->append(QStringLiteral("请填写生成的摆焊轨迹工艺名字"));
        return;
    }
    route=ui->wavependulum_modecombo->currentIndex();
    if(route<0||route>ui->wavependulum_modecombo->count()-1)
    {
        ui->record->append(QStringLiteral("请选择一个摆焊模式"));
        return;
    }
    cmd_wave_info.pendulum_mode=(Pendulum_mode)route;
    route=ui->wavetrend_modecombo->currentIndex();
    if(route<0||route>ui->wavetrend_modecombo->count()-1)
    {
        ui->record->append(QStringLiteral("请选择一个摆焊走向"));
        return;
    }
    cmd_wave_info.trend_mode=(Trend_mode)route;
    route=ui->waveordercombo->currentIndex();
    if(route<0||route>ui->waveordercombo->count()-1)
    {
        ui->record->append(QStringLiteral("请选择一个起摆模式"));
        return;
    }
    cmd_wave_info.order=route;


    if(waveperiod.isEmpty())
    {
        ui->record->append(QStringLiteral("请填写摆焊周期"));
        return;
    }
    cmd_wave_info.period=waveperiod.toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("摆焊周期内容格式错误"));
        return;
    }

    if(waveleftAmp.isEmpty())
    {
        ui->record->append(QStringLiteral("请填写左摆幅"));
        return;
    }
    cmd_wave_info.leftAmp=waveleftAmp.toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("左摆幅内容格式错误"));
        return;
    }

    if(waverightAmp.isEmpty())
    {
        ui->record->append(QStringLiteral("请填写右摆幅"));
        return;
    }
    cmd_wave_info.rightAmp=waverightAmp.toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("右摆幅内容格式错误"));
        return;
    }

    if(waveleftAmp_z.isEmpty())
    {
        ui->record->append(QStringLiteral("请填写左摆幅上扬"));
        return;
    }
    cmd_wave_info.leftAmp_z=waveleftAmp_z.toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("左摆幅上扬内容格式错误"));
        return;
    }

    if(waverightAmp_z.isEmpty())
    {
        ui->record->append(QStringLiteral("请填写右摆幅上扬"));
        return;
    }
    cmd_wave_info.rightAmp_z=waverightAmp_z.toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("右摆幅上扬内容格式错误"));
        return;
    }

    if(waveleftStopTime.isEmpty())
    {
        ui->record->append(QStringLiteral("请填写左摆停留时间"));
        return;
    }
    cmd_wave_info.leftStopTime=waveleftStopTime.toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("左摆停留时间内容格式错误"));
        return;
    }

    if(waverightStopTime.isEmpty())
    {
        ui->record->append(QStringLiteral("请填写右摆停留时间"));
        return;
    }
    cmd_wave_info.rightStopTime=waverightStopTime.toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("右摆停留时间内容格式错误"));
        return;
    }

    if(waveanglex.isEmpty())
    {
        ui->record->append(QStringLiteral("请填写L摆夹角角度"));
        return;
    }
    cmd_wave_info.anglex=waveanglex.toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("L摆夹角角度内容格式错误"));
        return;
    }

    if(waveangley.isEmpty())
    {
        ui->record->append(QStringLiteral("请填写L摆夹角法线角度"));
        return;
    }
    cmd_wave_info.angley=waveangley.toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("L摆夹角法线角度内容格式错误"));
        return;
    }

    if(waveStartPos.isEmpty())
    {
        ui->record->append(QStringLiteral("请填写起摆位置"));
        return;
    }
    cmd_wave_info.startPos=waveStartPos.toInt(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("起摆位置内容格式错误"));
        return;
    }

    if(wavetimeGap.isEmpty())
    {
        ui->record->append(QStringLiteral("请填写采样点时间间隔"));
        return;
    }
    cmd_wave_info.timeGap=wavetimeGap.toInt(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("采样点时间间隔内容格式错误"));
        return;
    }

    if(leftaddRX.isEmpty())
    {
        ui->record->append(QStringLiteral("请填写单摆左摆姿态增量RX"));
        return;
    }
    cmd_wave_info.leftAddRX=leftaddRX.toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("单摆左摆姿态增量RX内容格式错误"));
        return;
    }

    if(leftaddRY.isEmpty())
    {
        ui->record->append(QStringLiteral("请填写单摆左摆姿态增量RY"));
        return;
    }
    cmd_wave_info.leftAddRY=leftaddRY.toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("单摆左摆姿态增量RY内容格式错误"));
        return;
    }

    if(leftaddRZ.isEmpty())
    {
        ui->record->append(QStringLiteral("请填写单摆左摆姿态增量RZ"));
        return;
    }
    cmd_wave_info.leftAddRZ=leftaddRZ.toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("单摆左摆姿态增量RZ内容格式错误"));
        return;
    }

    if(rightaddRX.isEmpty())
    {
        ui->record->append(QStringLiteral("请填写单摆右摆姿态增量RX"));
        return;
    }
    cmd_wave_info.rightAddRX=rightaddRX.toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("单摆右摆姿态增量RX内容格式错误"));
        return;
    }

    if(rightaddRY.isEmpty())
    {
        ui->record->append(QStringLiteral("请填写单摆右摆姿态增量RY"));
        return;
    }
    cmd_wave_info.rightAddRY=rightaddRY.toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("单摆右摆姿态增量RY内容格式错误"));
        return;
    }

    if(rightaddRZ.isEmpty())
    {
        ui->record->append(QStringLiteral("请填写单摆右摆姿态增量RZ"));
        return;
    }
    cmd_wave_info.rightAddRZ=rightaddRZ.toFloat(&rc);
    if(rc==false)
    {
        ui->record->append(QStringLiteral("单摆右摆姿态增量RZ内容格式错误"));
        return;
    }

    my_cmd cmd;
    QString msg;
    msg=cmd.cmd_wave(namein,cmd_wave_info,nameout);
    cmd_msg=msg;
    done(1);
}

