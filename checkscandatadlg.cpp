#include "checkscandatadlg.h"
#include "ui_checkscandatadlg.h"

checkscandataDlg::checkscandataDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::checkscandataDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;

    ui->tabWidget->setTabText(0,QString::fromLocal8Bit("点位"));
    ui->tabWidget->setTabText(1,QString::fromLocal8Bit("扫描轨迹"));
    ui->tabWidget->setTabText(2,QString::fromLocal8Bit("跟踪轨迹"));
    ui->tabWidget->setTabText(3,QString::fromLocal8Bit("工艺轨迹"));
    ui->tabWidget->setTabText(4,QString::fromLocal8Bit("变换矩阵"));
}

checkscandataDlg::~checkscandataDlg()
{
    delete ui;
}

void checkscandataDlg::init_dlg_show()
{
    ui->tab1pointstate->clear();
    ui->tab1listWidget->clear();
    ui->tab1realposX->clear();
    ui->tab1realposY->clear();
    ui->tab1realposZ->clear();
    ui->tab1realposRX->clear();
    ui->tab1realposRY->clear();
    ui->tab1realposRZ->clear();
    ui->tab1realout1->clear();
    ui->tab1realout2->clear();
    ui->tab1realout3->clear();

    ui->tab5state->clear();
    ui->tab5a11_label->clear();
    ui->tab5a12_label->clear();
    ui->tab5a13_label->clear();
    ui->tab5a21_label->clear();
    ui->tab5a22_label->clear();
    ui->tab5a23_label->clear();
    ui->tab5a31_label->clear();
    ui->tab5a32_label->clear();
    ui->tab5a33_label->clear();
    ui->tab5t0_label->clear();
    ui->tab5t1_label->clear();
    ui->tab5t2_label->clear();

    for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
    {
        QString name=m_mcs->project->projecr_robpos_trace[n].name;
        ui->tab1listWidget->addItem(name);
    }

    ui->tab2listWidget->clear();
    for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
    {
        QString name=m_mcs->project->project_scan_trace[n].name;
        ui->tab2listWidget->addItem(name);
    }

    ui->tab3listWidget->clear();
    for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
    {
        QString name=m_mcs->project->project_weld_trace[n].name;
        ui->tab3listWidget->addItem(name);
    }

    ui->tab4listWidget->clear();
    for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
    {
        QString name=m_mcs->project->project_interweld_trace[n].name;
        ui->tab4listWidget->addItem(name);
    }

    ui->tab5listWidget->clear();
    for(int n=0;n<m_mcs->project->projecr_coord_matrix4d.size();n++)
    {
        QString name=m_mcs->project->projecr_coord_matrix4d[n].name;
        ui->tab5listWidget->addItem(name);
    }

    tab2viewer.reset(new pcl::visualization::PCLVisualizer("viewer", false));
    ui->tab2widget->SetRenderWindow(tab2viewer->getRenderWindow());
    tab2viewer->setupInteractor(ui->tab2widget->GetInteractor(), ui->tab2widget->GetRenderWindow());
    tab2viewer->addCoordinateSystem();  //添加坐标系

    tab3viewer.reset(new pcl::visualization::PCLVisualizer("viewer", false));
    ui->tab3widget->SetRenderWindow(tab3viewer->getRenderWindow());
    tab3viewer->setupInteractor(ui->tab3widget->GetInteractor(), ui->tab3widget->GetRenderWindow());
    tab3viewer->addCoordinateSystem();  //添加坐标系

    tab4viewer.reset(new pcl::visualization::PCLVisualizer("viewer", false));
    ui->tab4widget->SetRenderWindow(tab4viewer->getRenderWindow());
    tab4viewer->setupInteractor(ui->tab4widget->GetInteractor(), ui->tab4widget->GetRenderWindow());
    tab4viewer->addCoordinateSystem();  //添加坐标系

}

void checkscandataDlg::close_dlg_show()
{

}

//点位选择
void checkscandataDlg::on_tab1listWidget_itemClicked(QListWidgetItem *item)
{
    QString name=ui->tab1listWidget->currentItem()->text();
    int nowline=ui->tab1listWidget->currentRow();
    ui->tab1listWidget->setCurrentRow(nowline);
    if(name.isEmpty())
    {
        ui->tab1pointstate->clear();
        ui->tab1realposX->clear();
        ui->tab1realposY->clear();
        ui->tab1realposZ->clear();
        ui->tab1realposRX->clear();
        ui->tab1realposRY->clear();
        ui->tab1realposRZ->clear();
        ui->tab1realout1->clear();
        ui->tab1realout2->clear();
        ui->tab1realout3->clear();
    }
    else
    {
        int pointnum;
        QString msg;
        for(int n=0;n<m_mcs->project->projecr_robpos_trace.size();n++)
        {
            if(name==m_mcs->project->projecr_robpos_trace[n].name)
            {
                pointnum=n;
                break;
            }
        }
        if(m_mcs->project->projecr_robpos_trace[pointnum].nEn==false)
        {
            msg=QString::fromLocal8Bit("点位数据无效");
            ui->tab1pointstate->setText(msg);
            ui->tab1realposX->clear();
            ui->tab1realposY->clear();
            ui->tab1realposZ->clear();
            ui->tab1realposRX->clear();
            ui->tab1realposRY->clear();
            ui->tab1realposRZ->clear();
            ui->tab1realout1->clear();
            ui->tab1realout2->clear();
            ui->tab1realout3->clear();
        }
        else
        {
            RobPos robotpos=m_mcs->project->projecr_robpos_trace[pointnum].robotpos;
            ui->tab1pointstate->clear();
            ui->tab1realposX->setText(QString::number(robotpos.X,'f',ROBOT_POSE_DECIMAL_PLACE));
            ui->tab1realposY->setText(QString::number(robotpos.Y,'f',ROBOT_POSE_DECIMAL_PLACE));
            ui->tab1realposZ->setText(QString::number(robotpos.Z,'f',ROBOT_POSE_DECIMAL_PLACE));
            ui->tab1realposRX->setText(QString::number(robotpos.RX,'f',ROBOT_POSTURE_DECIMAL_PLACE));
            ui->tab1realposRY->setText(QString::number(robotpos.RY,'f',ROBOT_POSTURE_DECIMAL_PLACE));
            ui->tab1realposRZ->setText(QString::number(robotpos.RZ,'f',ROBOT_POSTURE_DECIMAL_PLACE));
            ui->tab1realout1->setText(QString::number(robotpos.out_1));
            ui->tab1realout2->setText(QString::number(robotpos.out_2));
            ui->tab1realout3->setText(QString::number(robotpos.out_3));
        }
    }
}

//扫描轨迹选择
void checkscandataDlg::on_tab2listWidget_itemClicked(QListWidgetItem *item)
{
    int tab2nowline=ui->tab2listWidget->currentRow();
    ui->tab2listWidget->setCurrentRow(tab2nowline);

    if(tab2nowline>=0)
    {
        int index;
        QString name=ui->tab2listWidget->currentItem()->text();
        for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
        {
            if(name==m_mcs->project->project_scan_trace[n].name)
            {
                index=n;
                break;
            }
        }
        std::vector<Scan_trace_line> lines;
        lines=m_mcs->project->project_scan_trace[index].point;
        std::vector<RobPos>cv_cloud;
        m_mcs->synchronous->Scantraceline_to_Weldtrace(lines,cv_cloud);

        pcl::PointCloud<pcl::PointXYZRGB>::Ptr rgbclould;
        rgbclould.reset(new pcl::PointCloud<pcl::PointXYZRGB>);
        rgbclould->clear();

        uint32_t R=255,G=0,B=0;
        uint32_t rgb = (static_cast<uint32_t>(R) << 16 | static_cast<uint32_t>(G) << 8 | static_cast<uint32_t>(B));
        if(cv_cloud.size()>0)
        {
            for(int n=0;n<cv_cloud.size();n++)
            {
                pcl::PointXYZRGB point;
                point.rgb = *reinterpret_cast<float*>(&rgb);
                point.x=cv_cloud[n].X-cv_cloud[0].X;
                point.y=cv_cloud[n].Y-cv_cloud[0].Y;
                point.z=cv_cloud[n].Z-cv_cloud[0].Z;
                rgbclould->points.push_back(point);
            }
        }
        rgbclould->width=rgbclould->points.size();
        rgbclould->height=1;

        tab2viewer->removeAllPointClouds();
        tab2viewer->removeAllShapes();
        tab2viewer->addPointCloud(rgbclould);
        ui->tab2widget->update();
    }
}

//跟踪轨迹选择
void checkscandataDlg::on_tab3listWidget_itemClicked(QListWidgetItem *item)
{
    int tab3nowline=ui->tab3listWidget->currentRow();
    ui->tab3listWidget->setCurrentRow(tab3nowline);

    if(tab3nowline>=0)
    {
        int index;
        QString name=ui->tab3listWidget->currentItem()->text();
        for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
        {
            if(name==m_mcs->project->project_weld_trace[n].name)
            {
                index=n;
                break;
            }
        }
        std::vector<RobPos> cv_cloud;
        cv_cloud=m_mcs->project->project_weld_trace[index].point;

        pcl::PointCloud<pcl::PointXYZRGB>::Ptr rgbclould;
        rgbclould.reset(new pcl::PointCloud<pcl::PointXYZRGB>);
        rgbclould->clear();

        uint32_t R=255,G=0,B=0;
        uint32_t rgb = (static_cast<uint32_t>(R) << 16 | static_cast<uint32_t>(G) << 8 | static_cast<uint32_t>(B));
        if(cv_cloud.size()>0)
        {
            for(int n=0;n<cv_cloud.size();n++)
            {
                pcl::PointXYZRGB point;
                point.rgb = *reinterpret_cast<float*>(&rgb);
                point.x=cv_cloud[n].X-cv_cloud[0].X;
                point.y=cv_cloud[n].Y-cv_cloud[0].Y;
                point.z=cv_cloud[n].Z-cv_cloud[0].Z;
                rgbclould->points.push_back(point);
            }
        }
        rgbclould->width=rgbclould->points.size();
        rgbclould->height=1;

        tab3viewer->removeAllPointClouds();
        tab3viewer->removeAllShapes();
        tab3viewer->addPointCloud(rgbclould);
        ui->tab3widget->update();
    }
}

//跟踪工艺选择
void checkscandataDlg::on_tab4listWidget_itemClicked(QListWidgetItem *item)
{
    int tab4nowline=ui->tab4listWidget->currentRow();
    ui->tab4listWidget->setCurrentRow(tab4nowline);

    if(tab4nowline>=0)
    {
        int index;
        QString name=ui->tab4listWidget->currentItem()->text();
        for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
        {
            if(name==m_mcs->project->project_interweld_trace[n].name)
            {
                index=n;
                break;
            }
        }
        std::vector<Weld_trace_onec> trace;
        trace=m_mcs->project->project_interweld_trace[index].trace;
        std::vector<RobPos> cv_cloud;
        for(int n=0;n<trace.size();n++)
        {
            for(int m=0;m<trace[n].point.size();m++)
            {
                cv_cloud.push_back(trace[n].point[m]);
            }
        }

        pcl::PointCloud<pcl::PointXYZRGB>::Ptr rgbclould;
        rgbclould.reset(new pcl::PointCloud<pcl::PointXYZRGB>);
        rgbclould->clear();

        uint32_t R=255,G=0,B=0;
        uint32_t rgb = (static_cast<uint32_t>(R) << 16 | static_cast<uint32_t>(G) << 8 | static_cast<uint32_t>(B));
        if(cv_cloud.size()>0)
        {
            for(int n=0;n<cv_cloud.size();n++)
            {
                pcl::PointXYZRGB point;
                point.rgb = *reinterpret_cast<float*>(&rgb);
                point.x=cv_cloud[n].X-cv_cloud[0].X;
                point.y=cv_cloud[n].Y-cv_cloud[0].Y;
                point.z=cv_cloud[n].Z-cv_cloud[0].Z;
                rgbclould->points.push_back(point);
            }
        }
        rgbclould->width=rgbclould->points.size();
        rgbclould->height=1;

        tab4viewer->removeAllPointClouds();
        tab4viewer->removeAllShapes();
        tab4viewer->addPointCloud(rgbclould);
        ui->tab4widget->update();
    }
}


//变换矩阵选择
void checkscandataDlg::on_tab5listWidget_itemClicked(QListWidgetItem *item)
{
    QString name=ui->tab5listWidget->currentItem()->text();
    int nowline=ui->tab5listWidget->currentRow();
    ui->tab5listWidget->setCurrentRow(nowline);
    if(name.isEmpty())
    {
        ui->tab5state->clear();
        ui->tab5a11_label->clear();
        ui->tab5a12_label->clear();
        ui->tab5a13_label->clear();
        ui->tab5a21_label->clear();
        ui->tab5a22_label->clear();
        ui->tab5a23_label->clear();
        ui->tab5a31_label->clear();
        ui->tab5a32_label->clear();
        ui->tab5a33_label->clear();
        ui->tab5t0_label->clear();
        ui->tab5t1_label->clear();
        ui->tab5t2_label->clear();
    }
    else
    {
        int matrix4dnum;
        QString msg;
        for(int n=0;n<m_mcs->project->projecr_coord_matrix4d.size();n++)
        {
            if(name==m_mcs->project->projecr_coord_matrix4d[n].name)
            {
                matrix4dnum=n;
                break;
            }
        }
        if(m_mcs->project->projecr_coord_matrix4d[matrix4dnum].nEn==false)
        {
            msg=QString::fromLocal8Bit("变化矩阵数据无效");
            ui->tab5state->setText(msg);
            ui->tab5a11_label->clear();
            ui->tab5a12_label->clear();
            ui->tab5a13_label->clear();
            ui->tab5a21_label->clear();
            ui->tab5a22_label->clear();
            ui->tab5a23_label->clear();
            ui->tab5a31_label->clear();
            ui->tab5a32_label->clear();
            ui->tab5a33_label->clear();
            ui->tab5t0_label->clear();
            ui->tab5t1_label->clear();
            ui->tab5t2_label->clear();
        }
        else
        {
            ui->tab5state->clear();
            ui->tab5a11_label->setText(QString::number(m_mcs->project->projecr_coord_matrix4d[matrix4dnum].R(0,0),'f',3));
            ui->tab5a12_label->setText(QString::number(m_mcs->project->projecr_coord_matrix4d[matrix4dnum].R(0,1),'f',3));
            ui->tab5a13_label->setText(QString::number(m_mcs->project->projecr_coord_matrix4d[matrix4dnum].R(0,2),'f',3));
            ui->tab5a21_label->setText(QString::number(m_mcs->project->projecr_coord_matrix4d[matrix4dnum].R(1,0),'f',3));
            ui->tab5a22_label->setText(QString::number(m_mcs->project->projecr_coord_matrix4d[matrix4dnum].R(1,1),'f',3));
            ui->tab5a23_label->setText(QString::number(m_mcs->project->projecr_coord_matrix4d[matrix4dnum].R(1,2),'f',3));
            ui->tab5a31_label->setText(QString::number(m_mcs->project->projecr_coord_matrix4d[matrix4dnum].R(2,0),'f',3));
            ui->tab5a32_label->setText(QString::number(m_mcs->project->projecr_coord_matrix4d[matrix4dnum].R(2,1),'f',3));
            ui->tab5a33_label->setText(QString::number(m_mcs->project->projecr_coord_matrix4d[matrix4dnum].R(2,2),'f',3));
            ui->tab5t0_label->setText(QString::number(m_mcs->project->projecr_coord_matrix4d[matrix4dnum].T(0),'f',3));
            ui->tab5t1_label->setText(QString::number(m_mcs->project->projecr_coord_matrix4d[matrix4dnum].T(1),'f',3));
            ui->tab5t2_label->setText(QString::number(m_mcs->project->projecr_coord_matrix4d[matrix4dnum].T(2),'f',3));
        }
    }
}





