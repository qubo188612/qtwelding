#ifndef CHECKSCANDATADLG_H
#define CHECKSCANDATADLG_H

#include <QDialog>
#include <QListWidgetItem>
#include "my_parameters.h"

namespace Ui {
class checkscandataDlg;
}

class checkscandataDlg : public QDialog
{
    Q_OBJECT

public:
    explicit checkscandataDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~checkscandataDlg();

    my_parameters *m_mcs;

    void init_dlg_show();
    void close_dlg_show();

    int tab2nowline;
    int tab3nowline;
    int tab4nowline;

private slots:
    void on_tab1listWidget_itemClicked(QListWidgetItem *item);

    void on_tab5listWidget_itemClicked(QListWidgetItem *item);

    void on_tab2listWidget_itemClicked(QListWidgetItem *item);

    void on_tab3listWidget_itemClicked(QListWidgetItem *item);

    void on_tab4listWidget_itemClicked(QListWidgetItem *item);

    void on_tab2viewcloudBtn_clicked();

    void on_tab3viewcloudBtn_clicked();

    void on_tab4viewcloudBtn_clicked();

private:
    Ui::checkscandataDlg *ui;
};

#endif // CHECKSCANDATADLG_H
