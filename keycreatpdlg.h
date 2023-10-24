#ifndef KEYCREATPDLG_H
#define KEYCREATPDLG_H

#include <QDialog>
#include <QListWidgetItem>
#include "myfont.h"
#include "my_parameters.h"
#include "my_cmd.h"

namespace Ui {
class keycreatpDlg;
}

class keycreatpDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keycreatpDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keycreatpDlg();

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);

private slots:
    void on_creatpAddBtn_clicked();

    void on_creatplist_itemClicked(QListWidgetItem *item);

    void on_creatpDelBtn_clicked();

    void on_creatpBtn_clicked();

private:
    Ui::keycreatpDlg *ui;

    bool b_inster;

    int now_creatpline;         //当前指向点行数
    QString now_pointname;      //当前指向的点名称

    std::vector<QString> pointsname;    //当前保存下的点坐标名字

    void updatalistUi();
};

#endif // KEYCREATPDLG_H
