#include "qtweldingdlg.h"
#include "ui_qtweldingdlg.h"
#include<QGridLayout>

qtweldingDlg::qtweldingDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::qtweldingDlg)
{
#if _MSC_VER
    QString filePath =  "./DATA";
    QDir dir(filePath);
    if(!dir.exists())
        QDir().mkdir(filePath);
#else
    std::string dir = "./DATA";
    if (access(dir.c_str(), 0) == -1)
    {
      mkdir("./DATA",S_IRWXU);
    }
#endif
    m_mcs=m_mcs->Get();

    b_init_show_ui_list=true;

    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint        //显示关闭
                  |Qt::WindowMinMaxButtonsHint);    //显示最大最小化

    thread = new qtweldingThread(this);
    connect(thread, SIGNAL(Send_show_ui_list()), this, SLOT(init_show_ui_list()));

    b_thread=true;
    thread->start();

    ui->record->append(QString::fromLocal8Bit("系统启动成功"));
}

qtweldingDlg::~qtweldingDlg()
{
    thread->Stop();
    thread->quit();
    thread->wait();

    delete ui;
}

void qtweldingDlg::on_importprojectBtn_clicked()//导入工程
{
    QString fileName = QFileDialog::getOpenFileName(this, "open project", "", "Project File(*.json)");
#if _MSC_VER
    QTextCodec *code = QTextCodec::codecForName("GBK");
#else
    QTextCodec *code = QTextCodec::codecForName("UTF-8");
#endif
    std::string name = code->fromUnicode(fileName).data();
    if(name.size()>0)
    {
        if(0!=m_mcs->project->LoadProject((char*)name.c_str()))
        {
            ui->record->append(QString::fromLocal8Bit("工程文件读取失败"));
        }
        else
        {
            QString path=name.c_str();
            QString msg="已经成功加载工程文件:"+path;
            ui->record->append(msg);
            msg="工程名称:"+m_mcs->project->project_name;
            ui->record->append(msg);
        }
    }
}

void qtweldingDlg::on_runprojectBtn_clicked()//运行工程
{

}

void qtweldingDlg::on_editprojectBtn_clicked()//工程编辑
{

}


void qtweldingDlg::on_editweldprocessBtn_clicked()//焊接工艺编辑
{

}


void qtweldingDlg::on_setlaserheadBtn_clicked()//激光头设置
{

}


void qtweldingDlg::on_setrobotBtn_clicked()//机器人设置
{

}


void qtweldingDlg::on_setwelderBtn_clicked()//焊机设置
{

}

void qtweldingDlg::init_show_ui_list()//界面刷新
{
    ui->project_name->setText(m_mcs->project->project_name);
    switch(m_mcs->project->project_Id)
    {
        case PROGECT_ID_TEACH_SCAN:
            ui->project_Id->setText(QString::fromLocal8Bit("示教扫描类型"));
        break;
    }
    b_init_show_ui_list=true;
}

qtweldingThread::qtweldingThread(qtweldingDlg *statci_p)
{
    _p=statci_p;
}

void qtweldingThread::run()
{
    while (1)
    {
        if(_p->b_thread==true)
        {
            if(_p->b_init_show_ui_list==true)
            {
                _p->b_init_show_ui_list=false;
                emit Send_show_ui_list();
            }
            sleep(0);
        }
        else
        {
            _p->b_stop_thread=true;
            break;
        }
    }
}

void qtweldingThread::Stop()
{
  if(_p->b_thread==true)
  {
    _p->b_stop_thread=false;
    _p->b_thread=false;
    while (_p->b_stop_thread==false)
    {
      sleep(0);
    }
  }
}

