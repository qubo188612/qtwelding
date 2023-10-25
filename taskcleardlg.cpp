#include "taskcleardlg.h"
#include "ui_taskcleardlg.h"

taskcleardlg::taskcleardlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::taskcleardlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

//美化界面
#ifdef OPEN_BEAUTIFY_INTERFACE
    ui->taskclearallBtn->setStyleSheet(FONT_BUTTON_INFO);
    ui->taskclearBtn->setStyleSheet(FONT_BUTTON_INFO);
    ui->tasklist->setStyleSheet(FONT_LISTWIDGET_INFO);
    ui->record->setStyleSheet(FONT_TEXTBROWERS_INFO);
#endif

    connect(ui->tasklist,&QListWidget::itemClicked,[=](){
        nowtask_Id=ui->tasklist->currentRow();
    });

    connect(ui->taskclearBtn,&QPushButton::clicked,[=](){
        if(nowtask_Id>=ui->tasklist->count())
        {
            ui->record->append(QStringLiteral("请先选中要删除的自定义任务号"));
        }
        else
        {
            QListWidgetItem *obj=ui->tasklist->currentItem();
            QString msg=obj->text();
            QStringList list=msg.split(" ");
            if(list.size()!=2)
            {
                ui->record->append(QStringLiteral("查找自定义任务号出错"));
                return;
            }
            QStringList data=list[0].split(":");
            if(data[0]!="自定义任务号")
            {
                ui->record->append(QStringLiteral("只能删除自定义任务号"));
                return;
            }
            int task=data[1].toInt();
            QJsonObject json;
            QJsonObject sing;
            sing.insert("taskname",task);
            json.insert("rm",sing);
            QString send=QString(QJsonDocument(json).toJson());
            m_mcs->resultdata.client->write(send.toUtf8());
        }
     });

    connect(ui->taskclearallBtn,&QPushButton::clicked,[=](){
        QMessageBox::StandardButton result= QMessageBox::information(this, QStringLiteral("提示信息"),
                                                                      QStringLiteral("确认删除全部自定义任务号？"),
                                                                      QMessageBox::Yes|QMessageBox::No,
                                                                      QMessageBox::No
                                                                      );
        switch (result)
        {
            case QMessageBox::Yes:
            {
                QJsonObject json;
                QJsonObject sing;
                sing.insert("taskname","all");
                json.insert("rm",sing);
                QString send=QString(QJsonDocument(json).toJson());
                m_mcs->resultdata.client->write(send.toUtf8());
                ui->record->append(QStringLiteral("正在删除全部自定义任务号"));
            }
            break;
            case QMessageBox::No:
            {
                ui->record->append(QStringLiteral("取消删除全部自定义任务号操作"));
            }
            break;
        }
    });
}

taskcleardlg::~taskcleardlg()
{
    delete ui;
}


void taskcleardlg::init_dlg_show()
{
    ui->record->clear();
}

void taskcleardlg::close_dlg_show()
{

}

void taskcleardlg::set_task_num()
{
    ui->tasklist->clear();
    for(int i=0;i<m_mcs->resultdata.taskfilename.size();i++)
    {
        QString msg;
        if(m_mcs->resultdata.taskfilename[i].taskname>=100&&m_mcs->resultdata.taskfilename[i].taskname<200)
        {
            msg=QStringLiteral("任务号:")+QString::number(m_mcs->resultdata.taskfilename[i].taskname)+
                    QStringLiteral(" 算法号:")+QString::number(m_mcs->resultdata.taskfilename[i].alsnum);
        }
        else
        {
            msg=QStringLiteral("自定义任务号:")+QString::number(m_mcs->resultdata.taskfilename[i].taskname)+
                    QStringLiteral(" 算法号:")+QString::number(m_mcs->resultdata.taskfilename[i].alsnum);
        }
        ui->tasklist->addItem(msg);
    }
    nowtask_Id=ui->tasklist->count();
    ui->tasklist->setCurrentRow(nowtask_Id);//设置当前选项无效，也不选中其他
}

void taskcleardlg::delete_task_num()
{
    ui->record->append(QStringLiteral("删除自定义任务号完成"));
}

void taskcleardlg::get_task_list()
{
    QJsonObject json;
    QJsonArray task;
    task.append("task");
    json.insert("ls",task);
    QString msg=QString(QJsonDocument(json).toJson());
    QByteArray arry=msg.toUtf8();
    arry.push_back('\0');
    m_mcs->resultdata.client->write(arry);
}
