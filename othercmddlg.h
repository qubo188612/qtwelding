#ifndef OTHERCMDDLG_H
#define OTHERCMDDLG_H

#include <QDialog>
#include <QListWidgetItem>
#include <QMessageBox>
#include "my_parameters.h"
#include "my_cmd.h"

namespace Ui {
class othercmdDlg;
}

class othercmdDlg : public QDialog
{
    Q_OBJECT

public:
    explicit othercmdDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~othercmdDlg();

    my_parameters *m_mcs;

    void init_dlg_show();
    void close_dlg_show();

    int cmdline;    //当前选择的指令行数
    QString cmdkey; //当前选择的指令
    QString cmdname;   //当前选择的指令名字

private slots:
    void on_OK_Btn_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::othercmdDlg *ui;
};

#endif // OTHERCMDDLG_H
