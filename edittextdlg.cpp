#include "edittextdlg.h"
#include "ui_edittextdlg.h"

edittextDlg::edittextDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::edittextDlg)
{
    ui->setupUi(this);
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

