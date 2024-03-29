#include "checkscandatadlg.h"
#include "ui_checkscandatadlg.h"

checkscandataDlg::checkscandataDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::checkscandataDlg)
{
    ui->setupUi(this);
    setFixedSize(this->width(), this->height());//禁止拉伸

    m_mcs=mcs;

//美化界面
#ifdef OPEN_BEAUTIFY_INTERFACE
    ui->tab1listWidget->setStyleSheet(FONT_LISTWIDGET_INFO);
    ui->tab2listWidget->setStyleSheet(FONT_LISTWIDGET_INFO);
    ui->tab3listWidget->setStyleSheet(FONT_LISTWIDGET_INFO);
    ui->tab4listWidget->setStyleSheet(FONT_LISTWIDGET_INFO);
    ui->tab5listWidget->setStyleSheet(FONT_LISTWIDGET_INFO);
#endif

    ui->tabWidget->setTabText(0,QStringLiteral("点位"));
    ui->tabWidget->setTabText(1,QStringLiteral("扫描轨迹"));
    ui->tabWidget->setTabText(2,QStringLiteral("跟踪轨迹"));
    ui->tabWidget->setTabText(3,QStringLiteral("工艺轨迹"));
    ui->tabWidget->setTabText(4,QStringLiteral("变换矩阵"));

    ui->tab1pushButton_1->setIcon(QIcon(":/new/prefix1/icons/1.png"));
    ui->tab1pushButton_2->setIcon(QIcon(":/new/prefix1/icons/2.png"));
    ui->tab1pushButton_3->setIcon(QIcon(":/new/prefix1/icons/3.png"));
    ui->tab1pushButton_4->setIcon(QIcon(":/new/prefix1/icons/4.png"));
    ui->tab1pushButton_5->setIcon(QIcon(":/new/prefix1/icons/5.png"));
    ui->tab1pushButton_6->setIcon(QIcon(":/new/prefix1/icons/6.png"));
    ui->tab1pushButton_1->setIconSize(ui->tab1pushButton_1->size());
    ui->tab1pushButton_2->setIconSize(ui->tab1pushButton_2->size());
    ui->tab1pushButton_3->setIconSize(ui->tab1pushButton_3->size());
    ui->tab1pushButton_4->setIconSize(ui->tab1pushButton_4->size());
    ui->tab1pushButton_5->setIconSize(ui->tab1pushButton_5->size());
    ui->tab1pushButton_6->setIconSize(ui->tab1pushButton_6->size());

    ui->tab2pushButton_1->setIcon(QIcon(":/new/prefix1/icons/1.png"));
    ui->tab2pushButton_2->setIcon(QIcon(":/new/prefix1/icons/2.png"));
    ui->tab2pushButton_3->setIcon(QIcon(":/new/prefix1/icons/3.png"));
    ui->tab2pushButton_4->setIcon(QIcon(":/new/prefix1/icons/4.png"));
    ui->tab2pushButton_5->setIcon(QIcon(":/new/prefix1/icons/5.png"));
    ui->tab2pushButton_6->setIcon(QIcon(":/new/prefix1/icons/6.png"));
    ui->tab2pushButton_1->setIconSize(ui->tab2pushButton_1->size());
    ui->tab2pushButton_2->setIconSize(ui->tab2pushButton_2->size());
    ui->tab2pushButton_3->setIconSize(ui->tab2pushButton_3->size());
    ui->tab2pushButton_4->setIconSize(ui->tab2pushButton_4->size());
    ui->tab2pushButton_5->setIconSize(ui->tab2pushButton_5->size());
    ui->tab2pushButton_6->setIconSize(ui->tab2pushButton_6->size());

    ui->tab3pushButton_1->setIcon(QIcon(":/new/prefix1/icons/1.png"));
    ui->tab3pushButton_2->setIcon(QIcon(":/new/prefix1/icons/2.png"));
    ui->tab3pushButton_3->setIcon(QIcon(":/new/prefix1/icons/3.png"));
    ui->tab3pushButton_4->setIcon(QIcon(":/new/prefix1/icons/4.png"));
    ui->tab3pushButton_5->setIcon(QIcon(":/new/prefix1/icons/5.png"));
    ui->tab3pushButton_6->setIcon(QIcon(":/new/prefix1/icons/6.png"));
    ui->tab3pushButton_1->setIconSize(ui->tab3pushButton_1->size());
    ui->tab3pushButton_2->setIconSize(ui->tab3pushButton_2->size());
    ui->tab3pushButton_3->setIconSize(ui->tab3pushButton_3->size());
    ui->tab3pushButton_4->setIconSize(ui->tab3pushButton_4->size());
    ui->tab3pushButton_5->setIconSize(ui->tab3pushButton_5->size());
    ui->tab3pushButton_6->setIconSize(ui->tab3pushButton_6->size());

    ui->tab4pushButton_1->setIcon(QIcon(":/new/prefix1/icons/1.png"));
    ui->tab4pushButton_2->setIcon(QIcon(":/new/prefix1/icons/2.png"));
    ui->tab4pushButton_3->setIcon(QIcon(":/new/prefix1/icons/3.png"));
    ui->tab4pushButton_4->setIcon(QIcon(":/new/prefix1/icons/4.png"));
    ui->tab4pushButton_5->setIcon(QIcon(":/new/prefix1/icons/5.png"));
    ui->tab4pushButton_6->setIcon(QIcon(":/new/prefix1/icons/6.png"));
    ui->tab4pushButton_1->setIconSize(ui->tab4pushButton_1->size());
    ui->tab4pushButton_2->setIconSize(ui->tab4pushButton_2->size());
    ui->tab4pushButton_3->setIconSize(ui->tab4pushButton_3->size());
    ui->tab4pushButton_4->setIconSize(ui->tab4pushButton_4->size());
    ui->tab4pushButton_5->setIconSize(ui->tab4pushButton_5->size());
    ui->tab4pushButton_6->setIconSize(ui->tab4pushButton_6->size());
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
    ui->tab5a11_label_2->clear();
    ui->tab5a12_label_2->clear();
    ui->tab5a13_label_2->clear();
    ui->tab5a21_label_2->clear();
    ui->tab5a22_label_2->clear();
    ui->tab5a23_label_2->clear();
    ui->tab5a31_label_2->clear();
    ui->tab5a32_label_2->clear();
    ui->tab5a33_label_2->clear();
    ui->tab5t0_label_2->clear();
    ui->tab5t1_label_2->clear();
    ui->tab5t2_label_2->clear();

    for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
    {
        QString name=m_mcs->project->project_robpos_trace[n].name;
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
    for(int n=0;n<m_mcs->project->project_coord_matrix4d.size();n++)
    {
        QString name=m_mcs->project->project_coord_matrix4d[n].name;
        ui->tab5listWidget->addItem(name);
    }
#if VTK_MAJOR_VERSION > 8
    auto renderer1 = vtkSmartPointer<vtkRenderer>::New();
    auto renderWindow1 = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindow1->AddRenderer(renderer1);
    tab1viewer.reset(new pcl::visualization::PCLVisualizer(renderer1,renderWindow1,"viewer", false));
#else
    tab1viewer.reset(new pcl::visualization::PCLVisualizer("viewer", false));
#endif
 // ui->tab1widget->SetRenderWindow(tab1viewer->getRenderWindow());
    ui->tab1widget->setRenderWindow(tab1viewer->getRenderWindow());
 // tab1viewer->setupInteractor(ui->tab1widget->GetInteractor(), ui->tab1widget->GetRenderWindow());
    tab1viewer->setupInteractor(ui->tab1widget->interactor(), ui->tab1widget->renderWindow());
    tab1viewer->addCoordinateSystem();  //添加坐标系

#if VTK_MAJOR_VERSION > 8
    auto renderer2 = vtkSmartPointer<vtkRenderer>::New();
    auto renderWindow2 = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindow2->AddRenderer(renderer2);
    tab2viewer.reset(new pcl::visualization::PCLVisualizer(renderer2,renderWindow2,"viewer", false));
#else
    tab2viewer.reset(new pcl::visualization::PCLVisualizer("viewer", false));
#endif
 // ui->tab2widget->SetRenderWindow(tab2viewer->getRenderWindow());
    ui->tab2widget->setRenderWindow(tab2viewer->getRenderWindow());
//  tab2viewer->setupInteractor(ui->tab2widget->GetInteractor(), ui->tab2widget->GetRenderWindow());
    tab2viewer->setupInteractor(ui->tab2widget->interactor(), ui->tab2widget->renderWindow());
    tab2viewer->addCoordinateSystem();  //添加坐标系

#if VTK_MAJOR_VERSION > 8
    auto renderer3 = vtkSmartPointer<vtkRenderer>::New();
    auto renderWindow3 = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindow3->AddRenderer(renderer3);
    tab3viewer.reset(new pcl::visualization::PCLVisualizer(renderer3,renderWindow3,"viewer", false));
#else
    tab3viewer.reset(new pcl::visualization::PCLVisualizer("viewer", false));
#endif
//  ui->tab3widget->SetRenderWindow(tab3viewer->getRenderWindow());
    ui->tab3widget->setRenderWindow(tab3viewer->getRenderWindow());
//  tab3viewer->setupInteractor(ui->tab3widget->GetInteractor(), ui->tab3widget->GetRenderWindow());
    tab3viewer->setupInteractor(ui->tab3widget->interactor(), ui->tab3widget->renderWindow());
    tab3viewer->addCoordinateSystem();  //添加坐标系

#if VTK_MAJOR_VERSION > 8
    auto renderer4 = vtkSmartPointer<vtkRenderer>::New();
    auto renderWindow4 = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindow4->AddRenderer(renderer4);
    tab4viewer.reset(new pcl::visualization::PCLVisualizer(renderer4,renderWindow4,"viewer", false));
#else
    tab4viewer.reset(new pcl::visualization::PCLVisualizer("viewer", false));
#endif
//  ui->tab4widget->SetRenderWindow(tab4viewer->getRenderWindow());
    ui->tab4widget->setRenderWindow(tab4viewer->getRenderWindow());
//  tab4viewer->setupInteractor(ui->tab4widget->GetInteractor(), ui->tab4widget->GetRenderWindow());
    tab4viewer->setupInteractor(ui->tab4widget->interactor(), ui->tab4widget->renderWindow());
    tab4viewer->addCoordinateSystem();  //添加坐标系
}

void checkscandataDlg::close_dlg_show()
{

}

pcl::PointCloud<pcl::PointXYZRGB>::Ptr checkscandataDlg::print_allpoint(double centerx,double centery,double centerz)
{
    uint32_t R=0,G=255,B=0;
    uint32_t rgb = (static_cast<uint32_t>(R) << 16 | static_cast<uint32_t>(G) << 8 | static_cast<uint32_t>(B));

    pcl::PointCloud<pcl::PointXYZRGB>::Ptr rgbclould;
    rgbclould.reset(new pcl::PointCloud<pcl::PointXYZRGB>);
    rgbclould->clear();

    pcl::PointXYZRGB point;

    for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
    {
        if(m_mcs->project->project_robpos_trace[n].nEn==true)
        {
            point.rgb = *reinterpret_cast<float*>(&rgb);
            point.x=m_mcs->project->project_robpos_trace[n].robotpos.X-centerx;
            point.y=m_mcs->project->project_robpos_trace[n].robotpos.Y-centery;
            point.z=m_mcs->project->project_robpos_trace[n].robotpos.Z-centerz;
            rgbclould->points.push_back(point);
        }
    }

    for(int n=0;n<m_mcs->project->project_scan_trace.size();n++)
    {
        std::vector<Scan_trace_line> lines;
        lines=m_mcs->project->project_scan_trace[n].point;
        std::vector<RobPos>cv_cloud;
        m_mcs->synchronous->Scantraceline_to_Weldtrace(lines,cv_cloud);

        if(cv_cloud.size()>0)
        {
            for(int n=0;n<cv_cloud.size();n++)
            {
                point.rgb = *reinterpret_cast<float*>(&rgb);
                point.x=cv_cloud[n].X-centerx;
                point.y=cv_cloud[n].Y-centery;
                point.z=cv_cloud[n].Z-centerz;
                rgbclould->points.push_back(point);
            }
        }
    }

    for(int n=0;n<m_mcs->project->project_weld_trace.size();n++)
    {
        std::vector<RobPos> cv_cloud;
        cv_cloud=m_mcs->project->project_weld_trace[n].point;
        if(cv_cloud.size()>0)
        {
            for(int n=0;n<cv_cloud.size();n++)
            {
                point.rgb = *reinterpret_cast<float*>(&rgb);
                point.x=cv_cloud[n].X-centerx;
                point.y=cv_cloud[n].Y-centery;
                point.z=cv_cloud[n].Z-centerz;
                rgbclould->points.push_back(point);
            }
        }
    }

    for(int n=0;n<m_mcs->project->project_interweld_trace.size();n++)
    {
        std::vector<Weld_trace_onec> trace;
        trace=m_mcs->project->project_interweld_trace[n].trace;
        std::vector<RobPos> cv_cloud;
        for(int n=0;n<trace.size();n++)
        {
            for(int m=0;m<trace[n].point.size();m++)
            {
                cv_cloud.push_back(trace[n].point[m]);
            }
        }
        if(cv_cloud.size()>0)
        {
            for(int n=0;n<cv_cloud.size();n++)
            {
                pcl::PointXYZRGB point;
                point.rgb = *reinterpret_cast<float*>(&rgb);
                point.x=cv_cloud[n].X-centerx;
                point.y=cv_cloud[n].Y-centery;
                point.z=cv_cloud[n].Z-centerz;
                rgbclould->points.push_back(point);
            }
        }
    }

    rgbclould->width=rgbclould->points.size();
    rgbclould->height=1;

    return rgbclould;
}

void checkscandataDlg::tab1updateCameraView(double focalPoint[3], double position[3], double viewUp[3])
{
//  vtkCamera *camera = ui->tab1widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActiveCamera();
    vtkCamera *camera = ui->tab1widget->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActiveCamera();
//  camera->SetFocalPoint(focalPoint);
    camera->SetPosition(position);
    camera->SetViewUp(viewUp);
    // 刷新渲染窗口
    tab1viewer->getRenderWindow()->Render();
    ui->tab1widget->update();
//  ui->tab1widget->show();
}

void checkscandataDlg::tab2updateCameraView(double focalPoint[3], double position[3], double viewUp[3])
{
//  vtkCamera *camera = ui->tab2widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActiveCamera();
    vtkCamera *camera = ui->tab2widget->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActiveCamera();
//  camera->SetFocalPoint(focalPoint);
    camera->SetPosition(position);
    camera->SetViewUp(viewUp);
    // 刷新渲染窗口
    tab2viewer->getRenderWindow()->Render();
    ui->tab2widget->update();
//  ui->tab2widget->show();
}

void checkscandataDlg::tab3updateCameraView(double focalPoint[3], double position[3], double viewUp[3])
{
//  vtkCamera *camera = ui->tab3widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActiveCamera();
    vtkCamera *camera = ui->tab3widget->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActiveCamera();
//  camera->SetFocalPoint(focalPoint);
    camera->SetPosition(position);
    camera->SetViewUp(viewUp);
    // 刷新渲染窗口
    tab3viewer->getRenderWindow()->Render();
    ui->tab3widget->update();
//  ui->tab3widget->show();
}

void checkscandataDlg::tab4updateCameraView(double focalPoint[3], double position[3], double viewUp[3])
{
//  vtkCamera *camera = ui->tab4widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActiveCamera();
    vtkCamera *camera = ui->tab4widget->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActiveCamera();
//  camera->SetFocalPoint(focalPoint);
    camera->SetPosition(position);
    camera->SetViewUp(viewUp);
    // 刷新渲染窗口
    tab4viewer->getRenderWindow()->Render();
    ui->tab4widget->update();
//  ui->tab4widget->show();
}

//点位选择
void checkscandataDlg::on_tab1listWidget_itemClicked(QListWidgetItem *item)
{
    QString name=ui->tab1listWidget->currentItem()->text();
    int nowline=ui->tab1listWidget->currentRow();
    ui->tab1listWidget->setCurrentRow(nowline);
    std::vector<RobPos>cv_cloud;
    int cv_num=0;
    bool b_cv_num=false;

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
        int pointnum=0;
        QString msg;
        for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
        {
            if(name==m_mcs->project->project_robpos_trace[n].name)
            {
                pointnum=n;
                break;
            }
        }
        if(m_mcs->project->project_robpos_trace[pointnum].nEn==false)
        {
            msg=QStringLiteral("点位数据无效");
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
            RobPos robotpos=m_mcs->project->project_robpos_trace[pointnum].robotpos;
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

        int tempcv_num=0;
        cv_cloud.reserve(m_mcs->project->project_robpos_trace.size());
        for(int n=0;n<m_mcs->project->project_robpos_trace.size();n++)
        {
            if(m_mcs->project->project_robpos_trace[n].nEn==true)
            {
                if(n==pointnum)
                {
                    cv_num=tempcv_num;
                    b_cv_num=true;
                }
                cv_cloud.push_back(m_mcs->project->project_robpos_trace[n].robotpos);
                tempcv_num++;
            }
        }
    }

    pcl::PointCloud<pcl::PointXYZRGB>::Ptr rgbclould;
    rgbclould.reset(new pcl::PointCloud<pcl::PointXYZRGB>);
    rgbclould->clear();

    if(cv_cloud.size()>0)
    {
        for(int n=0;n<cv_cloud.size();n++)
        {
            if(b_cv_num==true&&n==cv_num)
            {
                uint32_t R=255,G=0,B=0;
                uint32_t rgb = (static_cast<uint32_t>(R) << 16 | static_cast<uint32_t>(G) << 8 | static_cast<uint32_t>(B));
                pcl::PointXYZRGB point;
                point.rgb = *reinterpret_cast<float*>(&rgb);
                point.x=cv_cloud[n].X-cv_cloud[0].X;
                point.y=cv_cloud[n].Y-cv_cloud[0].Y;
                point.z=cv_cloud[n].Z-cv_cloud[0].Z;
                rgbclould->points.push_back(point);
            }
            else
            {
                uint32_t R=0,G=255,B=0;
                uint32_t rgb = (static_cast<uint32_t>(R) << 16 | static_cast<uint32_t>(G) << 8 | static_cast<uint32_t>(B));
                pcl::PointXYZRGB point;
                point.rgb = *reinterpret_cast<float*>(&rgb);
                point.x=cv_cloud[n].X-cv_cloud[0].X;
                point.y=cv_cloud[n].Y-cv_cloud[0].Y;
                point.z=cv_cloud[n].Z-cv_cloud[0].Z;
                rgbclould->points.push_back(point);
            }
        }
    }
    rgbclould->width=rgbclould->points.size();
    rgbclould->height=1;

    tab1viewer->removeAllPointClouds();
    tab1viewer->removeAllShapes();
    if(ui->checkBox->isChecked())
    {
        if(cv_cloud.size()>0)
        {
            pcl::PointCloud<pcl::PointXYZRGB>::Ptr allcloud=print_allpoint(cv_cloud[0].X,cv_cloud[0].Y,cv_cloud[0].Z);
            *rgbclould=*allcloud+*rgbclould;
        }
    }
    tab1viewer->addPointCloud(rgbclould);
    tab1viewer->getRenderWindow()->Render();
    ui->tab1widget->update();
//  ui->tab1widget->show();
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
        if(ui->checkBox->isChecked())
        {
            if(cv_cloud.size()>0)
            {
                pcl::PointCloud<pcl::PointXYZRGB>::Ptr allcloud=print_allpoint(cv_cloud[0].X,cv_cloud[0].Y,cv_cloud[0].Z);
                *rgbclould=*allcloud+*rgbclould;
            }
        }
        tab2viewer->addPointCloud(rgbclould);
        tab2viewer->getRenderWindow()->Render();
        ui->tab2widget->update();
    //  ui->tab2widget->show();
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
        if(ui->checkBox->isChecked())
        {
            if(cv_cloud.size()>0)
            {
                pcl::PointCloud<pcl::PointXYZRGB>::Ptr allcloud=print_allpoint(cv_cloud[0].X,cv_cloud[0].Y,cv_cloud[0].Z);
                *rgbclould=*allcloud+*rgbclould;
            }
        }
        tab3viewer->addPointCloud(rgbclould);
        tab3viewer->getRenderWindow()->Render();
        ui->tab3widget->update();
    //  ui->tab3widget->show();
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
        if(ui->checkBox->isChecked())
        {
            if(cv_cloud.size()>0)
            {
                pcl::PointCloud<pcl::PointXYZRGB>::Ptr allcloud=print_allpoint(cv_cloud[0].X,cv_cloud[0].Y,cv_cloud[0].Z);
                *rgbclould=*allcloud+*rgbclould;
            }
        }
        tab4viewer->addPointCloud(rgbclould);
        tab4viewer->getRenderWindow()->Render();
        ui->tab4widget->update();
    //  ui->tab4widget->show();
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
        ui->tab5a11_label_2->clear();
        ui->tab5a12_label_2->clear();
        ui->tab5a13_label_2->clear();
        ui->tab5a21_label_2->clear();
        ui->tab5a22_label_2->clear();
        ui->tab5a23_label_2->clear();
        ui->tab5a31_label_2->clear();
        ui->tab5a32_label_2->clear();
        ui->tab5a33_label_2->clear();
        ui->tab5t0_label_2->clear();
        ui->tab5t1_label_2->clear();
        ui->tab5t2_label_2->clear();
    }
    else
    {
        int matrix4dnum;
        QString msg;
        for(int n=0;n<m_mcs->project->project_coord_matrix4d.size();n++)
        {
            if(name==m_mcs->project->project_coord_matrix4d[n].name)
            {
                matrix4dnum=n;
                break;
            }
        }
        if(m_mcs->project->project_coord_matrix4d[matrix4dnum].nEn==false)
        {
            msg=QStringLiteral("变化矩阵数据无效");
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
            ui->tab5a11_label_2->clear();
            ui->tab5a12_label_2->clear();
            ui->tab5a13_label_2->clear();
            ui->tab5a21_label_2->clear();
            ui->tab5a22_label_2->clear();
            ui->tab5a23_label_2->clear();
            ui->tab5a31_label_2->clear();
            ui->tab5a32_label_2->clear();
            ui->tab5a33_label_2->clear();
            ui->tab5t0_label_2->clear();
            ui->tab5t1_label_2->clear();
            ui->tab5t2_label_2->clear();
        }
        else
        {
            ui->tab5state->clear();
            ui->tab5a11_label->setText(QString::number(m_mcs->project->project_coord_matrix4d[matrix4dnum].R(0,0),'f',3));
            ui->tab5a12_label->setText(QString::number(m_mcs->project->project_coord_matrix4d[matrix4dnum].R(0,1),'f',3));
            ui->tab5a13_label->setText(QString::number(m_mcs->project->project_coord_matrix4d[matrix4dnum].R(0,2),'f',3));
            ui->tab5a21_label->setText(QString::number(m_mcs->project->project_coord_matrix4d[matrix4dnum].R(1,0),'f',3));
            ui->tab5a22_label->setText(QString::number(m_mcs->project->project_coord_matrix4d[matrix4dnum].R(1,1),'f',3));
            ui->tab5a23_label->setText(QString::number(m_mcs->project->project_coord_matrix4d[matrix4dnum].R(1,2),'f',3));
            ui->tab5a31_label->setText(QString::number(m_mcs->project->project_coord_matrix4d[matrix4dnum].R(2,0),'f',3));
            ui->tab5a32_label->setText(QString::number(m_mcs->project->project_coord_matrix4d[matrix4dnum].R(2,1),'f',3));
            ui->tab5a33_label->setText(QString::number(m_mcs->project->project_coord_matrix4d[matrix4dnum].R(2,2),'f',3));
            ui->tab5t0_label->setText(QString::number(m_mcs->project->project_coord_matrix4d[matrix4dnum].T(0),'f',3));
            ui->tab5t1_label->setText(QString::number(m_mcs->project->project_coord_matrix4d[matrix4dnum].T(1),'f',3));
            ui->tab5t2_label->setText(QString::number(m_mcs->project->project_coord_matrix4d[matrix4dnum].T(2),'f',3));
            ui->tab5a11_label_2->setText(QString::number(m_mcs->project->project_coord_matrix4d[matrix4dnum].R1(0,0),'f',3));
            ui->tab5a12_label_2->setText(QString::number(m_mcs->project->project_coord_matrix4d[matrix4dnum].R1(0,1),'f',3));
            ui->tab5a13_label_2->setText(QString::number(m_mcs->project->project_coord_matrix4d[matrix4dnum].R1(0,2),'f',3));
            ui->tab5a21_label_2->setText(QString::number(m_mcs->project->project_coord_matrix4d[matrix4dnum].R1(1,0),'f',3));
            ui->tab5a22_label_2->setText(QString::number(m_mcs->project->project_coord_matrix4d[matrix4dnum].R1(1,1),'f',3));
            ui->tab5a23_label_2->setText(QString::number(m_mcs->project->project_coord_matrix4d[matrix4dnum].R1(1,2),'f',3));
            ui->tab5a31_label_2->setText(QString::number(m_mcs->project->project_coord_matrix4d[matrix4dnum].R1(2,0),'f',3));
            ui->tab5a32_label_2->setText(QString::number(m_mcs->project->project_coord_matrix4d[matrix4dnum].R1(2,1),'f',3));
            ui->tab5a33_label_2->setText(QString::number(m_mcs->project->project_coord_matrix4d[matrix4dnum].R1(2,2),'f',3));
            ui->tab5t0_label_2->setText(QString::number(m_mcs->project->project_coord_matrix4d[matrix4dnum].T1(0),'f',3));
            ui->tab5t1_label_2->setText(QString::number(m_mcs->project->project_coord_matrix4d[matrix4dnum].T1(1),'f',3));
            ui->tab5t2_label_2->setText(QString::number(m_mcs->project->project_coord_matrix4d[matrix4dnum].T1(2),'f',3));
        }
    }
}


void checkscandataDlg::on_tab1pushButton_1_clicked()//俯视图
{
//  vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab1widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab1widget->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    double center[3];
    polydata->GetCenter(center);
    double bounds[6];
    polydata->GetBounds(bounds);
    double max_range = std::max({bounds[1]-bounds[0], bounds[3]-bounds[2], bounds[5]-bounds[4]});
    double cam_distance = max_range / tan(45.0*vtkMath::Pi()/360.0);
    double cam_height = cam_distance * tan(vtkMath::Pi()/4.0);
    double position[3] = {center[0], center[1], center[2]+cam_height};
    double viewUp[3] = {0.0, 1.0, 0.0};
    tab1updateCameraView(center, position, viewUp);
}

void checkscandataDlg::on_tab1pushButton_2_clicked()//前视图
{
//  vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab1widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab1widget->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    double center[3];
    polydata->GetCenter(center);
    double bounds[6];
    polydata->GetBounds(bounds);
    double max_range = std::max({bounds[1]-bounds[0], bounds[3]-bounds[2], bounds[4]});
    double cam_distance = max_range / tan(45.0*vtkMath::Pi()/360.0);
    double position[3] = {center[0], center[1]-cam_distance, center[2]};
    double viewUp[3] = {0.0, 0.0, 1.0};
    tab1updateCameraView(center, position, viewUp);
}

void checkscandataDlg::on_tab1pushButton_3_clicked()//左视图
{
//  vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab1widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab1widget->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    double center[3];
    polydata->GetCenter(center);
    double bounds[6];
    polydata->GetBounds(bounds);
    double max_range = std::max({bounds[0], bounds[3]-bounds[2], bounds[5]-bounds[4]});
    double cam_distance = max_range / tan(45.0*vtkMath::Pi()/360.0);
    double position[3] = {center[0]-cam_distance, center[1], center[2]};
    double viewUp[3] = {0.0, 0.0, 1.0};
    tab1updateCameraView(center, position, viewUp);
}


void checkscandataDlg::on_tab1pushButton_4_clicked()//后视图
{
//  vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab1widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab1widget->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    double center[3];
    polydata->GetCenter(center);
    double bounds[6];
    polydata->GetBounds(bounds);
    double max_range = std::max({bounds[0]-bounds[1], bounds[3]-bounds[2], bounds[5]-bounds[4]});
    double cam_distance = max_range / tan(45.0*vtkMath::Pi()/360.0);
    double position[3] = {center[0], center[1]+cam_distance, center[2]};
    double viewUp[3] = {0.0, 0.0, 1.0};
    tab1updateCameraView(center, position, viewUp);
}


void checkscandataDlg::on_tab1pushButton_5_clicked()//右视图
{
//  vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab1widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab1widget->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    double center[3];
    polydata->GetCenter(center);
    double bounds[6];
    polydata->GetBounds(bounds);
    double max_range = std::max({bounds[1], bounds[3]-bounds[2], bounds[5]-bounds[4]});
    double cam_distance = max_range / tan(45.0*vtkMath::Pi()/360.0);
    double position[3] = {center[0]+cam_distance, center[1], center[2]};
    double viewUp[3] = {0.0, 0.0, 1.0};
    tab1updateCameraView(center, position, viewUp);
}


void checkscandataDlg::on_tab1pushButton_6_clicked()//仰视图
{
//  vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab1widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab1widget->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    double center[3];
    polydata->GetCenter(center);
    double bounds[6];
    polydata->GetBounds(bounds);
    double max_range = std::max({bounds[1]-bounds[0], bounds[5]-bounds[4], bounds[2]-bounds[3]});
    double cam_distance = max_range / tan(45.0*vtkMath::Pi()/360.0);
    double cam_height = cam_distance * tan(vtkMath::Pi()/4.0);
    double position[3] = {center[0], center[1], center[2]-cam_height};
    double viewUp[3] = {0.0, -1.0, 0.0};
    tab1updateCameraView(center, position, viewUp);
}


void checkscandataDlg::on_tab2pushButton_1_clicked()//俯视图
{
//  vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab2widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab2widget->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    double center[3];
    polydata->GetCenter(center);
    double bounds[6];
    polydata->GetBounds(bounds);
    double max_range = std::max({bounds[1]-bounds[0], bounds[3]-bounds[2], bounds[5]-bounds[4]});
    double cam_distance = max_range / tan(45.0*vtkMath::Pi()/360.0);
    double cam_height = cam_distance * tan(vtkMath::Pi()/4.0);
    double position[3] = {center[0], center[1], center[2]+cam_height};
    double viewUp[3] = {0.0, 1.0, 0.0};
    tab2updateCameraView(center, position, viewUp);
}


void checkscandataDlg::on_tab2pushButton_2_clicked()//前视图
{
//  vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab2widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab2widget->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    double center[3];
    polydata->GetCenter(center);
    double bounds[6];
    polydata->GetBounds(bounds);
    double max_range = std::max({bounds[1]-bounds[0], bounds[3]-bounds[2], bounds[4]});
    double cam_distance = max_range / tan(45.0*vtkMath::Pi()/360.0);
    double position[3] = {center[0], center[1]-cam_distance, center[2]};
    double viewUp[3] = {0.0, 0.0, 1.0};
    tab2updateCameraView(center, position, viewUp);
}

void checkscandataDlg::on_tab2pushButton_3_clicked()//左视图
{
//  vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab2widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab2widget->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    double center[3];
    polydata->GetCenter(center);
    double bounds[6];
    polydata->GetBounds(bounds);
    double max_range = std::max({bounds[0], bounds[3]-bounds[2], bounds[5]-bounds[4]});
    double cam_distance = max_range / tan(45.0*vtkMath::Pi()/360.0);
    double position[3] = {center[0]-cam_distance, center[1], center[2]};
    double viewUp[3] = {0.0, 0.0, 1.0};
    tab2updateCameraView(center, position, viewUp);
}


void checkscandataDlg::on_tab2pushButton_4_clicked()//后视图
{
//  vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab2widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab2widget->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    double center[3];
    polydata->GetCenter(center);
    double bounds[6];
    polydata->GetBounds(bounds);
    double max_range = std::max({bounds[0]-bounds[1], bounds[3]-bounds[2], bounds[5]-bounds[4]});
    double cam_distance = max_range / tan(45.0*vtkMath::Pi()/360.0);
    double position[3] = {center[0], center[1]+cam_distance, center[2]};
    double viewUp[3] = {0.0, 0.0, 1.0};
    tab2updateCameraView(center, position, viewUp);
}

void checkscandataDlg::on_tab2pushButton_5_clicked()//右视图
{
//  vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab2widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab2widget->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    double center[3];
    polydata->GetCenter(center);
    double bounds[6];
    polydata->GetBounds(bounds);
    double max_range = std::max({bounds[1], bounds[3]-bounds[2], bounds[5]-bounds[4]});
    double cam_distance = max_range / tan(45.0*vtkMath::Pi()/360.0);
    double position[3] = {center[0]+cam_distance, center[1], center[2]};
    double viewUp[3] = {0.0, 0.0, 1.0};
    tab2updateCameraView(center, position, viewUp);
}

void checkscandataDlg::on_tab2pushButton_6_clicked()//仰视图
{
//  vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab2widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab2widget->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    double center[3];
    polydata->GetCenter(center);
    double bounds[6];
    polydata->GetBounds(bounds);
    double max_range = std::max({bounds[1]-bounds[0], bounds[5]-bounds[4], bounds[2]-bounds[3]});
    double cam_distance = max_range / tan(45.0*vtkMath::Pi()/360.0);
    double cam_height = cam_distance * tan(vtkMath::Pi()/4.0);
    double position[3] = {center[0], center[1], center[2]-cam_height};
    double viewUp[3] = {0.0, -1.0, 0.0};
    tab2updateCameraView(center, position, viewUp);
}


void checkscandataDlg::on_tab3pushButton_1_clicked()//俯视图
{
//  vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab3widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab3widget->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    double center[3];
    polydata->GetCenter(center);
    double bounds[6];
    polydata->GetBounds(bounds);
    double max_range = std::max({bounds[1]-bounds[0], bounds[3]-bounds[2], bounds[5]-bounds[4]});
    double cam_distance = max_range / tan(45.0*vtkMath::Pi()/360.0);
    double cam_height = cam_distance * tan(vtkMath::Pi()/4.0);
    double position[3] = {center[0], center[1], center[2]+cam_height};
    double viewUp[3] = {0.0, 1.0, 0.0};
    tab3updateCameraView(center, position, viewUp);
}

void checkscandataDlg::on_tab3pushButton_2_clicked()//前视图
{
//  vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab3widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab3widget->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    double center[3];
    polydata->GetCenter(center);
    double bounds[6];
    polydata->GetBounds(bounds);
    double max_range = std::max({bounds[1]-bounds[0], bounds[3]-bounds[2], bounds[4]});
    double cam_distance = max_range / tan(45.0*vtkMath::Pi()/360.0);
    double position[3] = {center[0], center[1]-cam_distance, center[2]};
    double viewUp[3] = {0.0, 0.0, 1.0};
    tab3updateCameraView(center, position, viewUp);
}


void checkscandataDlg::on_tab3pushButton_3_clicked()//左视图
{
    //  vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab3widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab3widget->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    double center[3];
    polydata->GetCenter(center);
    double bounds[6];
    polydata->GetBounds(bounds);
    double max_range = std::max({bounds[0], bounds[3]-bounds[2], bounds[5]-bounds[4]});
    double cam_distance = max_range / tan(45.0*vtkMath::Pi()/360.0);
    double position[3] = {center[0]-cam_distance, center[1], center[2]};
    double viewUp[3] = {0.0, 0.0, 1.0};
    tab3updateCameraView(center, position, viewUp);
}

void checkscandataDlg::on_tab3pushButton_4_clicked()//后视图
{
    //  vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab3widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab3widget->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    double center[3];
    polydata->GetCenter(center);
    double bounds[6];
    polydata->GetBounds(bounds);
    double max_range = std::max({bounds[0]-bounds[1], bounds[3]-bounds[2], bounds[5]-bounds[4]});
    double cam_distance = max_range / tan(45.0*vtkMath::Pi()/360.0);
    double position[3] = {center[0], center[1]+cam_distance, center[2]};
    double viewUp[3] = {0.0, 0.0, 1.0};
    tab3updateCameraView(center, position, viewUp);
}


void checkscandataDlg::on_tab3pushButton_5_clicked()//右视图
{
    //  vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab3widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab3widget->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    double center[3];
    polydata->GetCenter(center);
    double bounds[6];
    polydata->GetBounds(bounds);
    double max_range = std::max({bounds[1], bounds[3]-bounds[2], bounds[5]-bounds[4]});
    double cam_distance = max_range / tan(45.0*vtkMath::Pi()/360.0);
    double position[3] = {center[0]+cam_distance, center[1], center[2]};
    double viewUp[3] = {0.0, 0.0, 1.0};
    tab3updateCameraView(center, position, viewUp);
}


void checkscandataDlg::on_tab3pushButton_6_clicked()//仰视图
{
    //  vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab3widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab3widget->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    double center[3];
    polydata->GetCenter(center);
    double bounds[6];
    polydata->GetBounds(bounds);
    double max_range = std::max({bounds[1]-bounds[0], bounds[5]-bounds[4], bounds[2]-bounds[3]});
    double cam_distance = max_range / tan(45.0*vtkMath::Pi()/360.0);
    double cam_height = cam_distance * tan(vtkMath::Pi()/4.0);
    double position[3] = {center[0], center[1], center[2]-cam_height};
    double viewUp[3] = {0.0, -1.0, 0.0};
    tab3updateCameraView(center, position, viewUp);
}


void checkscandataDlg::on_tab4pushButton_1_clicked()//俯视图
{
    //  vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab4widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab4widget->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    double center[3];
    polydata->GetCenter(center);
    double bounds[6];
    polydata->GetBounds(bounds);
    double max_range = std::max({bounds[1]-bounds[0], bounds[3]-bounds[2], bounds[5]-bounds[4]});
    double cam_distance = max_range / tan(45.0*vtkMath::Pi()/360.0);
    double cam_height = cam_distance * tan(vtkMath::Pi()/4.0);
    double position[3] = {center[0], center[1], center[2]+cam_height};
    double viewUp[3] = {0.0, 1.0, 0.0};
    tab4updateCameraView(center, position, viewUp);
}


void checkscandataDlg::on_tab4pushButton_2_clicked()//前视图
{
    //  vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab4widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab4widget->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    double center[3];
    polydata->GetCenter(center);
    double bounds[6];
    polydata->GetBounds(bounds);
    double max_range = std::max({bounds[1]-bounds[0], bounds[3]-bounds[2], bounds[4]});
    double cam_distance = max_range / tan(45.0*vtkMath::Pi()/360.0);
    double position[3] = {center[0], center[1]-cam_distance, center[2]};
    double viewUp[3] = {0.0, 0.0, 1.0};
    tab4updateCameraView(center, position, viewUp);
}


void checkscandataDlg::on_tab4pushButton_3_clicked()//左视图
{
    //  vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab4widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab4widget->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    double center[3];
    polydata->GetCenter(center);
    double bounds[6];
    polydata->GetBounds(bounds);
    double max_range = std::max({bounds[0], bounds[3]-bounds[2], bounds[5]-bounds[4]});
    double cam_distance = max_range / tan(45.0*vtkMath::Pi()/360.0);
    double position[3] = {center[0]-cam_distance, center[1], center[2]};
    double viewUp[3] = {0.0, 0.0, 1.0};
    tab4updateCameraView(center, position, viewUp);
}


void checkscandataDlg::on_tab4pushButton_4_clicked()//右视图
{
    //  vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab4widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab4widget->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    double center[3];
    polydata->GetCenter(center);
    double bounds[6];
    polydata->GetBounds(bounds);
    double max_range = std::max({bounds[1], bounds[3]-bounds[2], bounds[5]-bounds[4]});
    double cam_distance = max_range / tan(45.0*vtkMath::Pi()/360.0);
    double position[3] = {center[0]+cam_distance, center[1], center[2]};
    double viewUp[3] = {0.0, 0.0, 1.0};
    tab4updateCameraView(center, position, viewUp);
}


void checkscandataDlg::on_tab4pushButton_5_clicked()//右视图
{
    //  vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab4widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab4widget->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    double center[3];
    polydata->GetCenter(center);
    double bounds[6];
    polydata->GetBounds(bounds);
    double max_range = std::max({bounds[1], bounds[3]-bounds[2], bounds[5]-bounds[4]});
    double cam_distance = max_range / tan(45.0*vtkMath::Pi()/360.0);
    double position[3] = {center[0]+cam_distance, center[1], center[2]};
    double viewUp[3] = {0.0, 0.0, 1.0};
    tab4updateCameraView(center, position, viewUp);
}


void checkscandataDlg::on_tab4pushButton_6_clicked()//仰视图
{
    //  vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab4widget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    vtkPolyData* polydata = vtkPolyData::SafeDownCast(ui->tab4widget->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor()->GetMapper()->GetInput());
    double center[3];
    polydata->GetCenter(center);
    double bounds[6];
    polydata->GetBounds(bounds);
    double max_range = std::max({bounds[1]-bounds[0], bounds[5]-bounds[4], bounds[2]-bounds[3]});
    double cam_distance = max_range / tan(45.0*vtkMath::Pi()/360.0);
    double cam_height = cam_distance * tan(vtkMath::Pi()/4.0);
    double position[3] = {center[0], center[1], center[2]-cam_height};
    double viewUp[3] = {0.0, -1.0, 0.0};
    tab4updateCameraView(center, position, viewUp);
}

