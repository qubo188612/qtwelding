#include "qrshowdlg.h"
#include "ui_qrshowdlg.h"

qrshowDlg::qrshowDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::qrshowDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

    ui->state->setWordWrap(true);   //自动换行

//美化界面
#ifdef OPEN_BEAUTIFY_INTERFACE
    ui->pushButton->setStyleSheet(FONT_BUTTON_INFO);
#endif
}

qrshowDlg::~qrshowDlg()
{
    delete ui;
}

void qrshowDlg::init_dlg_show()
{
    ui->state->clear();
    Myqr::QrInfo_to_Qr(m_mcs->project->project_path,"QRCode",fileImage);
    fileImage = fileImage.scaled(ui->widget->width(),ui->widget->height(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->widget->setPixmap(QPixmap::fromImage(fileImage));
}

void qrshowDlg::close_dlg_show()
{

}

//保存二维码
void qrshowDlg::on_pushButton_clicked()
{
    QString fileName=m_mcs->project->project_path;
    QString msg=fileName;

    if(fileName.size()>=5)
    {
        QString tem1=".json";
        QString tem2=".JSON";
        QString tem=fileName.mid(fileName.size()-5,5);
        if(tem!=tem1&&tem!=tem2)//文件名末尾不是".json"或".JSON"
        {
            msg=msg+".png";
        }
        else
        {
            QString other=fileName.mid(0,fileName.size()-5);
            msg=other+".png";
        }
    }
    fileImage.save(msg);
    QString savemsg=QStringLiteral("保存路径:")+msg;
    ui->state->setText(savemsg);
}

