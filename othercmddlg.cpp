#include "othercmddlg.h"
#include "ui_othercmddlg.h"

othercmdDlg::othercmdDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::othercmdDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
}

othercmdDlg::~othercmdDlg()
{
    delete ui;
}

void othercmdDlg::init_dlg_show()
{
    cmdline=-1;
    ui->listWidget->setCurrentRow(cmdline);
}

void othercmdDlg::close_dlg_show()
{

}

//确定
void othercmdDlg::on_OK_Btn_clicked()
{
    if(cmdline>=0)
    {
        QString msg=ui->listWidget->currentItem()->text();
        QString returnmsg,key;
        my_cmd cmd;
        cmd.getkey(msg,returnmsg,key);
        cmdkey=key;
        cmdname=msg;
        done(1);
    }
    else
    {
        QMessageBox:: StandardButton result= QMessageBox::information(this, QString::fromLocal8Bit("提示信息"),
                                                                      QString::fromLocal8Bit("请选择一个指令"),
                                                                      QMessageBox::Yes,
                                                                      QMessageBox::Yes
                                                                      );
    }
}

void othercmdDlg::on_listWidget_itemClicked(QListWidgetItem *item)
{
    cmdline=ui->listWidget->currentRow();
}

