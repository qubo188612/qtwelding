#ifndef TASKCLEARDLG_H
#define TASKCLEARDLG_H

#include <QDialog>
#include "my_parameters.h"
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>

namespace Ui {
class taskcleardlg;
}

class taskcleardlg : public QDialog
{
    Q_OBJECT

public:
    explicit taskcleardlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~taskcleardlg();

    my_parameters *m_mcs;

    void set_task_num();
    void delete_task_num();

    void init_dlg_show();
    void close_dlg_show();

private:
    Ui::taskcleardlg *ui;

    int nowtask_Id;

};

#endif // TASKCLEARDLG_H
