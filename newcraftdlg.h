#ifndef NEWCRAFTDLG_H
#define NEWCRAFTDLG_H

#include <QDialog>
#include <QListWidgetItem>
#include <QFileDialog>
#include "my_parameters.h"
#include "edittextdlg.h"

namespace Ui {
class newcraftDlg;
}

class newcraftDlg : public QDialog
{
    Q_OBJECT

public:
    explicit newcraftDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~newcraftDlg();

    my_parameters *m_mcs;

    edittextDlg *edittext;

    void init_dlg_show(bool b_file=true);
    void close_dlg_show();
private slots:
    void on_pushButton_clicked();

    void on_craft_Id_itemClicked(QListWidgetItem *item);

private:
    Ui::newcraftDlg *ui;

    int now_craft_Id; //当前指向的项目类型

    bool b_file;//文件形式保存
};

#endif // NEWCRAFTDLG_H
