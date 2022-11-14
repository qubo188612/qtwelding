#ifndef SETPROJECTDLG_H
#define SETPROJECTDLG_H

#include <QDialog>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextCodec>
#include "my_parameters.h"
#include "my_cmd.h"
#include "traceeditdlg.h"
#include "traceedit3dlg.h"

namespace Ui {
class setprojectDlg;
}

class setprojectDlg : public QDialog
{
    Q_OBJECT

public:
    explicit setprojectDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~setprojectDlg();

    traceeditDlg *traceedit;
    traceedit3Dlg *traceedit3;

    my_parameters *m_mcs;

    void init_dlg_show();
    void close_dlg_show();

private slots:
    void on_moveaddBtn_clicked();

    void on_delaytimeBtn_clicked();

    void on_leaseropenBtn_clicked();

    void on_leasercloseBtn_clicked();

    void on_welderarcingBtn_clicked();

    void on_welderarcoutBtn_clicked();

    void on_scanaddBtn_clicked();

    void on_tracecmdaddBtn_clicked();

    void on_traceeditBtn_clicked();

    void on_customaddBtn_clicked();

    void on_customcheckBtn_clicked();

    void on_cmdlistdeleteBtn_clicked();

    void on_cmdlistclearBtn_clicked();

    void on_OKBtn_clicked();

    void on_cmdlist_itemClicked(QListWidgetItem *item);

    void on_customreplaceBtn_clicked();

    void on_tracefilepathBtn_clicked();

private:
    Ui::setprojectDlg *ui;
    int now_cmdline;         //当前指向CMD行数

    void updatacmdlistUi();
};

#endif // SETPROJECTDLG_H
