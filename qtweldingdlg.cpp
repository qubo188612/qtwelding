#include "qtweldingdlg.h"
#include "ui_qtweldingdlg.h"
#include<QGridLayout>

qtweldingDlg::qtweldingDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::qtweldingDlg)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint        //显示关闭
                  |Qt::WindowMinMaxButtonsHint);    //显示最大最小化
}

qtweldingDlg::~qtweldingDlg()
{
    delete ui;
}
