#ifndef SETCRAFT1DLG_H
#define SETCRAFT1DLG_H

#include <QDialog>
#include <QDoubleValidator>
#include <QListWidgetItem>
#include "my_parameters.h"

namespace Ui {
class setcraft1Dlg;
}

class setcraft1Dlg : public QDialog
{
    Q_OBJECT

public:
    explicit setcraft1Dlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~setcraft1Dlg();

    my_parameters *m_mcs;

    void init_dlg_show();
    void close_dlg_show();


private slots:
    void on_pushButton_get_stpos_clicked();

    void on_pushButton_updata_stpos_clicked();

    void on_pushButton_get_edpos_clicked();

    void on_pushButton_updata_edpos_clicked();

    void on_pushButton_get_centerpos_clicked();

    void on_pushButton_updata_centerpos_clicked();

    void on_pushButton_del_clicked();

    void on_pushButtonOK_clicked();

    void on_robposlist_itemClicked(QListWidgetItem *item);


private:
    Ui::setcraft1Dlg *ui;

    QDoubleValidator *adoubleValidator_pose;    //机器人坐标小数
    QDoubleValidator *adoubleValidator_posture; //机器人姿态小数
    QDoubleValidator *adoubleValidator_3;//3位小数

    int now_robpos;         //当前指向TCP修改位置
    void updataRoblistUi();
    void UpdataUi();
};

#endif // SETCRAFT1DLG_H
