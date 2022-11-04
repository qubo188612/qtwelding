#ifndef DEMARCATEDLG_H
#define DEMARCATEDLG_H

#include <QDialog>
#include <QListWidgetItem>
#include "my_parameters.h"
#include "calibration.h"

namespace Ui {
class demarcateDlg;
}

class demarcateDlg : public QDialog
{
    Q_OBJECT

public:
    explicit demarcateDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~demarcateDlg();

    my_parameters *m_mcs;

    void init_dlg_show();
    void close_dlg_show();

private slots:
    void on_radio1_clicked();

    void on_radio2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_7_clicked();

    void on_robposlist_itemClicked(QListWidgetItem *item);

    void on_leaserposlist_itemClicked(QListWidgetItem *item);

private:
    Ui::demarcateDlg *ui;

    int now_robpos;         //当前指向TCP修改位置
    int now_leaserpos;      //当前指向激光头修改位置
    void updataRoblistUi();
    void updataLeaserlistUi();
    void updataDemarcateResult();
    void updataUi();

    std::vector<double> errgroup;
};

#endif // DEMARCATEDLG_H
