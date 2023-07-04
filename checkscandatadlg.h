#ifndef CHECKSCANDATADLG_H
#define CHECKSCANDATADLG_H

#include <QDialog>
#include <QListWidgetItem>
#include "my_parameters.h"
#include <pcl/common/common_headers.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/visualization/cloud_viewer.h>

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


    pcl::visualization::PCLVisualizer::Ptr tab2viewer;
    pcl::visualization::PCLVisualizer::Ptr tab3viewer;
    pcl::visualization::PCLVisualizer::Ptr tab4viewer;

private slots:
    void on_tab1listWidget_itemClicked(QListWidgetItem *item);

    void on_tab5listWidget_itemClicked(QListWidgetItem *item);

    void on_tab2listWidget_itemClicked(QListWidgetItem *item);

    void on_tab3listWidget_itemClicked(QListWidgetItem *item);

    void on_tab4listWidget_itemClicked(QListWidgetItem *item);


private:
    Ui::checkscandataDlg *ui;
};

#endif // CHECKSCANDATADLG_H
