#ifndef CHECKSCANDATADLG_H
#define CHECKSCANDATADLG_H

#include <QDialog>
#include <QListWidgetItem>
#include "my_parameters.h"
#include <pcl/common/common_headers.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/visualization/cloud_viewer.h>
#include <vtkCamera.h>
#include <vtkBoundingBox.h>
#include <vtkRendererCollection.h>
#include <vtkMath.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>

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

    pcl::visualization::PCLVisualizer::Ptr tab1viewer;
    pcl::visualization::PCLVisualizer::Ptr tab2viewer;
    pcl::visualization::PCLVisualizer::Ptr tab3viewer;
    pcl::visualization::PCLVisualizer::Ptr tab4viewer;

private slots:
    void on_tab1listWidget_itemClicked(QListWidgetItem *item);

    void on_tab5listWidget_itemClicked(QListWidgetItem *item);

    void on_tab2listWidget_itemClicked(QListWidgetItem *item);

    void on_tab3listWidget_itemClicked(QListWidgetItem *item);

    void on_tab4listWidget_itemClicked(QListWidgetItem *item);


    void on_tab1pushButton_1_clicked();

    void on_tab1pushButton_2_clicked();

    void on_tab1pushButton_3_clicked();

    void on_tab1pushButton_4_clicked();

    void on_tab1pushButton_5_clicked();

    void on_tab1pushButton_6_clicked();

    void on_tab2pushButton_1_clicked();

    void on_tab2pushButton_2_clicked();

    void on_tab2pushButton_3_clicked();

    void on_tab2pushButton_4_clicked();

    void on_tab2pushButton_5_clicked();

    void on_tab2pushButton_6_clicked();

    void on_tab3pushButton_1_clicked();

    void on_tab3pushButton_2_clicked();

    void on_tab3pushButton_3_clicked();

    void on_tab3pushButton_4_clicked();

    void on_tab3pushButton_5_clicked();

    void on_tab3pushButton_6_clicked();

    void on_tab4pushButton_1_clicked();

    void on_tab4pushButton_2_clicked();

    void on_tab4pushButton_3_clicked();

    void on_tab4pushButton_4_clicked();

    void on_tab4pushButton_5_clicked();

    void on_tab4pushButton_6_clicked();

private:
    Ui::checkscandataDlg *ui;

    void tab1updateCameraView(double focalPoint[3], double position[3], double viewUp[3]);
    void tab2updateCameraView(double focalPoint[3], double position[3], double viewUp[3]);
    void tab3updateCameraView(double focalPoint[3], double position[3], double viewUp[3]);
    void tab4updateCameraView(double focalPoint[3], double position[3], double viewUp[3]);

    pcl::PointCloud<pcl::PointXYZRGB>::Ptr print_allpoint(double centerx,double centery,double centerz);
};

#endif // CHECKSCANDATADLG_H
