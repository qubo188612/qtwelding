#include "edittextdlg.h"
#include "ui_edittextdlg.h"

edittextDlg::edittextDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::edittextDlg)
{
    ui->setupUi(this);
    setFixedSize(this->width(), this->height());//禁止拉伸

//美化界面
#ifdef OPEN_BEAUTIFY_INTERFACE
    ui->lineEdit->setStyleSheet(FONT_EDIT_INFO);
    ui->pushButton->setStyleSheet(FONT_BUTTON_INFO);
#endif
}

edittextDlg::~edittextDlg()
{
    delete ui;
}

void edittextDlg::init_dlg_show(QString topic)
{
    ui->label->setText(topic);
    ui->lineEdit->clear();
}

void edittextDlg::close_dlg_show()
{

}

void edittextDlg::on_pushButton_clicked()
{
    msg_edit=ui->lineEdit->text();
    done(1);
}

