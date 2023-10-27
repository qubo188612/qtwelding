#include "qrshowdlg.h"
#include "ui_qrshowdlg.h"

qrshowDlg::qrshowDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::qrshowDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸

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
    QImage fileImage;
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

}

