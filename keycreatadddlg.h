#ifndef KEYCREATADDDLG_H
#define KEYCREATADDDLG_H

#include <QDialog>
#include <QListWidgetItem>
#include "myfont.h"
#include "my_parameters.h"
#include "my_cmd.h"

namespace Ui {
class keycreataddDlg;
}

class keycreataddDlg : public QDialog
{
    Q_OBJECT

public:
    explicit keycreataddDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~keycreataddDlg();  

    my_parameters *m_mcs;
    QString cmd_msg;        //生成的指令

    void init_dlg_show();
    void init_dlg_show(QString cmdlist);
    void close_dlg_show();

    void setbutton(int name=0);

private slots:

    void on_creataddlist_itemClicked(QListWidgetItem *item);

    void on_creataddAddBtn_clicked();

    void on_creataddDelBtn_clicked();

    void on_creataddBtn_clicked();

private:
    Ui::keycreataddDlg *ui;

    bool b_inster;

    int now_creataddline;         //当前指向轨迹行数
    QString now_weldname;      //当前指向的轨迹名称

    std::vector<QString> weldsname;    //当前保存下的轨迹坐标名字

    void updatalistUi();
};

#endif // KEYCREATADDDLG_H
