#ifndef ROBOTSETDLG_H
#define ROBOTSETDLG_H

#include <QDialog>
#include <QHostAddress>
#include <QMessageBox>
#include "myfont.h"
#include "my_parameters.h"

namespace Ui {
class robotsetDlg;
}

class robotsetDlg : public QDialog
{
    Q_OBJECT

public:
    explicit robotsetDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~robotsetDlg();

    my_parameters *m_mcs;

    void init_dlg_show();
    void close_dlg_show();

    bool IsIp(QString currentIp);   //判断IP格式
private slots:
    void on_pushButton_clicked();

    void on_robot_model_currentIndexChanged(int index);

private:
    Ui::robotsetDlg *ui;
};

#endif // ROBOTSETDLG_H
