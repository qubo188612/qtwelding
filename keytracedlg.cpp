#include "keytracedlg.h"
#include "ui_keytracedlg.h"

keytraceDlg::keytraceDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keytraceDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

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
    int rc=cmd.decodecmd(cmdlist,msg,key);
    if(rc==0)
    {
        if(key==CMD_TRACE_KEY)//移动指令
        {
            QString name_in=cmd.cmd_trace_name_in;//获取到跟踪轨迹序号
            QString name_out=cmd.cmd_trace_name_out;//获取到跟踪轨迹工艺名字
            float speed=cmd.cmd_trace_speed;//获取到的跟踪速度
            QString craftfilepath=cmd.cmd_trace_craftfilepath;//获取到工艺包的文件路径
            ui->tracefilepath->setText(craftfilepath);
            ui->tracespeed->setText(QString::number(speed,'f',ROBOT_SPEED_DECIMAL_PLACE));
            int weld_trace_num;//搜索到的焊接轨道序号
            //这里添加移动命令
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
        ui->tracecmdaddBtn->setText(QString::fromLocal8Bit("插入跟踪轨迹指令"));
    }
    else
    {
        ui->tracecmdaddBtn->setText(QString::fromLocal8Bit("替换跟踪轨迹指令"));
    }
}

void keytraceDlg::on_tracefilepathBtn_clicked()
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

void keytraceDlg::on_tracecmdaddBtn_clicked()
{
    bool rc;
    int route=ui->tracetrackcombo->currentIndex();
    QString name_in=ui->tracetrackcombo->currentText();
    QString name_out=ui->tracefilename->text();
    float speed=ui->tracespeed->text().toFloat(&rc);
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
    if(name_out.isEmpty())
    {
        ui->record->append(QString::fromLocal8Bit("请填写生成的跟踪轨迹工艺名字"));
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
    QString msg=cmd.cmd_trace(name_in,speed,filepath,name_out);
    ui->record->append(QString::fromLocal8Bit("插入跟踪轨迹指令成功"));
    cmd_msg=msg;
    done(1);
}




