#ifndef NEWPROJECTDLG_H
#define NEWPROJECTDLG_H

#include <QDialog>
#include <QFileDialog>
#include <QListWidgetItem>
#include "myfont.h"
#include "my_parameters.h"

namespace Ui {
class newprojectDlg;
}

class newprojectDlg : public QDialog
{
    Q_OBJECT

public:
    explicit newprojectDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~newprojectDlg();

    my_parameters *m_mcs;

    void init_dlg_show();
    void close_dlg_show();

private slots:
    void on_pushButton_clicked();

    void on_project_Id_itemClicked(QListWidgetItem *item);

private:
    Ui::newprojectDlg *ui;

    int now_project_Id; //当前指向的项目类型
};

#endif // NEWPROJECTDLG_H
