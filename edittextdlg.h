#ifndef EDITTEXTDLG_H
#define EDITTEXTDLG_H

#include <QDialog>
#include "global.h"
#include "myfont.h"

namespace Ui {
class edittextDlg;
}

class edittextDlg : public QDialog
{
    Q_OBJECT

public:
    explicit edittextDlg(QWidget *parent = nullptr);
    ~edittextDlg();

    void init_dlg_show(QString topic);
    void close_dlg_show();
    QString msg_edit;       //编辑框文字

private slots:
    void on_pushButton_clicked();

private:
    Ui::edittextDlg *ui;
};

#endif // EDITTEXTDLG_H
