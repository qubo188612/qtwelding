#include "cambuilddlg.h"
#include "ui_cambuilddlg.h"

template<typename _Tp>
std::vector<_Tp> convertMat2Vector(cv::Mat &mat)
{
    return (std::vector<_Tp>)(mat.reshape(1, 1));
}

cambuilddlg::cambuilddlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cambuilddlg)
{
    ui->setupUi(this);

    adoubleValidator_2 = new QDoubleValidator(-9999,9999,2,this);//限制3位小数
    ui->x1_Edit->setValidator(adoubleValidator_2);
    ui->y1_Edit->setValidator(adoubleValidator_2);
    ui->x2_Edit->setValidator(adoubleValidator_2);
    ui->y2_Edit->setValidator(adoubleValidator_2);
    ui->x3_Edit->setValidator(adoubleValidator_2);
    ui->y3_Edit->setValidator(adoubleValidator_2);
    ui->x4_Edit->setValidator(adoubleValidator_2);
    ui->y4_Edit->setValidator(adoubleValidator_2);


    m_mcs=mcs;

    b_key_1_push=false;
    b_key_2_push=false;
    b_key_3_push=false;
    b_key_4_push=false;
    b_key_left_pull=false;
    b_key_right_pull=false;
    b_key_up_pull=false;
    b_key_down_pull=false;

    connect(ui->cambuildBtn,&QPushButton::clicked,[=](){
        m_mcs->e2proomdata.camdlg_modposX1=ui->x1_Edit->text().toFloat()*100;
        m_mcs->e2proomdata.camdlg_modposY1=ui->y1_Edit->text().toFloat()*100;
        m_mcs->e2proomdata.camdlg_modposX2=ui->x2_Edit->text().toFloat()*100;
        m_mcs->e2proomdata.camdlg_modposY2=ui->y2_Edit->text().toFloat()*100;
        m_mcs->e2proomdata.camdlg_modposX3=ui->x3_Edit->text().toFloat()*100;
        m_mcs->e2proomdata.camdlg_modposY3=ui->y3_Edit->text().toFloat()*100;
        m_mcs->e2proomdata.camdlg_modposX4=ui->x4_Edit->text().toFloat()*100;
        m_mcs->e2proomdata.camdlg_modposY4=ui->y4_Edit->text().toFloat()*100;
        m_mcs->e2proomdata.write_camdlg_para();

        std::vector<cv::Point2f> points_trans(4);
        std::vector<cv::Point2f> corners_trans(4);
        corners_trans[0] = cv::Point2f(m_mcs->e2proomdata.camdlg_modposX1/100.0,m_mcs->e2proomdata.camdlg_modposY1/100.0);
        corners_trans[1] = cv::Point2f(m_mcs->e2proomdata.camdlg_modposX2/100.0,m_mcs->e2proomdata.camdlg_modposY2/100.0);
        corners_trans[2] = cv::Point2f(m_mcs->e2proomdata.camdlg_modposX3/100.0,m_mcs->e2proomdata.camdlg_modposY3/100.0);
        corners_trans[3] = cv::Point2f(m_mcs->e2proomdata.camdlg_modposX4/100.0,m_mcs->e2proomdata.camdlg_modposY4/100.0);
        points_trans[0] = cv::Point2f(m_mcs->e2proomdata.camdlg_cvimg_posX1,m_mcs->e2proomdata.camdlg_cvimg_posY1);
        points_trans[1] = cv::Point2f(m_mcs->e2proomdata.camdlg_cvimg_posX2,m_mcs->e2proomdata.camdlg_cvimg_posY2);
        points_trans[2] = cv::Point2f(m_mcs->e2proomdata.camdlg_cvimg_posX3,m_mcs->e2proomdata.camdlg_cvimg_posY3);
        points_trans[3] = cv::Point2f(m_mcs->e2proomdata.camdlg_cvimg_posX4,m_mcs->e2proomdata.camdlg_cvimg_posY4);

        cv::Mat transform = getPerspectiveTransform(points_trans,corners_trans);
        transform.convertTo(transform,CV_64FC1);
        m_mcs->cam->sop_cam[0].ros_Params.homography_matrix=convertMat2Vector<double>(transform);

        if(m_mcs->cam->sop_cam[0].ros_Params.homography_matrix.size()==9)
        {
            ui->a11_label->setText(QString::number(m_mcs->cam->sop_cam[0].ros_Params.homography_matrix[0],'f',3));
            ui->a12_label->setText(QString::number(m_mcs->cam->sop_cam[0].ros_Params.homography_matrix[1],'f',3));
            ui->a13_label->setText(QString::number(m_mcs->cam->sop_cam[0].ros_Params.homography_matrix[2],'f',3));
            ui->a21_label->setText(QString::number(m_mcs->cam->sop_cam[0].ros_Params.homography_matrix[3],'f',3));
            ui->a22_label->setText(QString::number(m_mcs->cam->sop_cam[0].ros_Params.homography_matrix[4],'f',3));
            ui->a23_label->setText(QString::number(m_mcs->cam->sop_cam[0].ros_Params.homography_matrix[5],'f',3));
            ui->a31_label->setText(QString::number(m_mcs->cam->sop_cam[0].ros_Params.homography_matrix[6],'f',3));
            ui->a32_label->setText(QString::number(m_mcs->cam->sop_cam[0].ros_Params.homography_matrix[7],'f',3));
            ui->a33_label->setText(QString::number(m_mcs->cam->sop_cam[0].ros_Params.homography_matrix[8],'f',3));
        }

    });

    connect(ui->confimbuildBtn,&QPushButton::clicked,[=](){
        m_mcs->cam->sop_cam[0].ros_set_homography_matrix(m_mcs->cam->sop_cam[0].ros_Params);
        ui->record->append(QString::fromLocal8Bit("标定矩阵导入成功"));
    });
}

cambuilddlg::~cambuilddlg()
{
    delete adoubleValidator_2;
    delete ui;
}

void cambuilddlg::init_dlg_show()
{
    b_key_1_push=false;
    b_key_2_push=false;
    b_key_3_push=false;
    b_key_4_push=false;
    b_key_left_pull=false;
    b_key_right_pull=false;
    b_key_up_pull=false;
    b_key_down_pull=false;

    this->setFocusPolicy(Qt::ClickFocus);//设置键盘捕捉不与光标焦点争夺优先权
    ui->widget->setMouseTracking(true); //设置不用按键按下也能在鼠标移动的时候触发

    b_init_show_cambuild_inlab_finish=true;

    ui->x1_Edit->setText(QString::number(m_mcs->e2proomdata.camdlg_modposX1/100.0,'f',2));
    ui->y1_Edit->setText(QString::number(m_mcs->e2proomdata.camdlg_modposY1/100.0,'f',2));
    ui->x2_Edit->setText(QString::number(m_mcs->e2proomdata.camdlg_modposX2/100.0,'f',2));
    ui->y2_Edit->setText(QString::number(m_mcs->e2proomdata.camdlg_modposY2/100.0,'f',2));
    ui->x3_Edit->setText(QString::number(m_mcs->e2proomdata.camdlg_modposX3/100.0,'f',2));
    ui->y3_Edit->setText(QString::number(m_mcs->e2proomdata.camdlg_modposY3/100.0,'f',2));
    ui->x4_Edit->setText(QString::number(m_mcs->e2proomdata.camdlg_modposX4/100.0,'f',2));
    ui->y4_Edit->setText(QString::number(m_mcs->e2proomdata.camdlg_modposY4/100.0,'f',2));
    if(m_mcs->cam->sop_cam[0].ros_Params.homography_matrix.size()==9)
    {
        ui->a11_label->setText(QString::number(m_mcs->cam->sop_cam[0].ros_Params.homography_matrix[0],'f',3));
        ui->a12_label->setText(QString::number(m_mcs->cam->sop_cam[0].ros_Params.homography_matrix[1],'f',3));
        ui->a13_label->setText(QString::number(m_mcs->cam->sop_cam[0].ros_Params.homography_matrix[2],'f',3));
        ui->a21_label->setText(QString::number(m_mcs->cam->sop_cam[0].ros_Params.homography_matrix[3],'f',3));
        ui->a22_label->setText(QString::number(m_mcs->cam->sop_cam[0].ros_Params.homography_matrix[4],'f',3));
        ui->a23_label->setText(QString::number(m_mcs->cam->sop_cam[0].ros_Params.homography_matrix[5],'f',3));
        ui->a31_label->setText(QString::number(m_mcs->cam->sop_cam[0].ros_Params.homography_matrix[6],'f',3));
        ui->a32_label->setText(QString::number(m_mcs->cam->sop_cam[0].ros_Params.homography_matrix[7],'f',3));
        ui->a33_label->setText(QString::number(m_mcs->cam->sop_cam[0].ros_Params.homography_matrix[8],'f',3));
    }
    thread1 = new cambuildThread(this);
    connect(thread1, SIGNAL(Send_show_cambuild_inlab(cv::Mat)), this, SLOT(init_show_cambuild_inlab(cv::Mat)));

    b_thread1=true;
    thread1->start();
}

void cambuilddlg::close_dlg_show()
{
    ui->widget->setMouseTracking(false);
    this->clearFocus();

    thread1->Stop();
    thread1->quit();
    thread1->wait();

    delete thread1;
}

void cambuilddlg::drow_point(cv::Mat cvimg)
{
    double d_zoomx=(double)cvimg.cols/ui->widget->width();
    double d_zoomy=(double)cvimg.rows/ui->widget->height();
    if(b_key_1_push==true)
    {
        if(b_key_left_pull==true)
        {
            b_key_left_pull=false;
            if(m_mcs->e2proomdata.camdlg_cvimg_posX1>m_mcs->e2proomdata.camdlg_cvimg_posX1_min)
            {
                m_mcs->e2proomdata.camdlg_cvimg_posX1=m_mcs->e2proomdata.camdlg_cvimg_posX1-1;
            }
        }
        else if(b_key_right_pull==true)
        {
            b_key_right_pull=false;
            if(m_mcs->e2proomdata.camdlg_cvimg_posX1<m_mcs->e2proomdata.camdlg_cvimg_posX1_max)
            {
                m_mcs->e2proomdata.camdlg_cvimg_posX1=m_mcs->e2proomdata.camdlg_cvimg_posX1+1;
            }
        }
        if(b_key_up_pull==true)
        {
            b_key_up_pull=false;
            if(m_mcs->e2proomdata.camdlg_cvimg_posY1>m_mcs->e2proomdata.camdlg_cvimg_posY1_min)
            {
                m_mcs->e2proomdata.camdlg_cvimg_posY1=m_mcs->e2proomdata.camdlg_cvimg_posY1-1;
            }
        }
        else if(b_key_down_pull==true)
        {
            b_key_down_pull=false;
            if(m_mcs->e2proomdata.camdlg_cvimg_posY1<m_mcs->e2proomdata.camdlg_cvimg_posY1_max)
            {
                m_mcs->e2proomdata.camdlg_cvimg_posY1=m_mcs->e2proomdata.camdlg_cvimg_posY1+1;
            }
        }
        if(ui->widget->b_mouse_push==true)
        {
            m_mcs->e2proomdata.camdlg_cvimg_posX1=ui->widget->mousePos.rx()*d_zoomx;
            m_mcs->e2proomdata.camdlg_cvimg_posY1=ui->widget->mousePos.ry()*d_zoomy;
        }
    }
    if(b_key_2_push==true)
    {
        if(b_key_left_pull==true)
        {
            b_key_left_pull=false;
            if(m_mcs->e2proomdata.camdlg_cvimg_posX2>m_mcs->e2proomdata.camdlg_cvimg_posX2_min)
            {
                m_mcs->e2proomdata.camdlg_cvimg_posX2=m_mcs->e2proomdata.camdlg_cvimg_posX2-1;
            }
        }
        else if(b_key_right_pull==true)
        {
            b_key_right_pull=false;
            if(m_mcs->e2proomdata.camdlg_cvimg_posX2<m_mcs->e2proomdata.camdlg_cvimg_posX2_max)
            {
                m_mcs->e2proomdata.camdlg_cvimg_posX2=m_mcs->e2proomdata.camdlg_cvimg_posX2+1;
            }
        }
        if(b_key_up_pull==true)
        {
            b_key_up_pull=false;
            if(m_mcs->e2proomdata.camdlg_cvimg_posY2>m_mcs->e2proomdata.camdlg_cvimg_posY2_min)
            {
                m_mcs->e2proomdata.camdlg_cvimg_posY2=m_mcs->e2proomdata.camdlg_cvimg_posY2-1;
            }
        }
        else if(b_key_down_pull==true)
        {
            b_key_down_pull=false;
            if(m_mcs->e2proomdata.camdlg_cvimg_posY2<m_mcs->e2proomdata.camdlg_cvimg_posY2_max)
            {
                m_mcs->e2proomdata.camdlg_cvimg_posY2=m_mcs->e2proomdata.camdlg_cvimg_posY2+1;
            }
        }
        if(ui->widget->b_mouse_push==true)
        {
            m_mcs->e2proomdata.camdlg_cvimg_posX2=ui->widget->mousePos.rx()*d_zoomx;
            m_mcs->e2proomdata.camdlg_cvimg_posY2=ui->widget->mousePos.ry()*d_zoomy;
        }
    }
    if(b_key_3_push==true)
    {
        if(b_key_left_pull==true)
        {
            b_key_left_pull=false;
            if(m_mcs->e2proomdata.camdlg_cvimg_posX3>m_mcs->e2proomdata.camdlg_cvimg_posX3_min)
            {
                m_mcs->e2proomdata.camdlg_cvimg_posX3=m_mcs->e2proomdata.camdlg_cvimg_posX3-1;
            }
        }
        else if(b_key_right_pull==true)
        {
            b_key_right_pull=false;
            if(m_mcs->e2proomdata.camdlg_cvimg_posX3<m_mcs->e2proomdata.camdlg_cvimg_posX3_max)
            {
                m_mcs->e2proomdata.camdlg_cvimg_posX3=m_mcs->e2proomdata.camdlg_cvimg_posX3+1;
            }
        }
        if(b_key_up_pull==true)
        {
            b_key_up_pull=false;
            if(m_mcs->e2proomdata.camdlg_cvimg_posY3>m_mcs->e2proomdata.camdlg_cvimg_posY3_min)
            {
                m_mcs->e2proomdata.camdlg_cvimg_posY3=m_mcs->e2proomdata.camdlg_cvimg_posY3-1;
            }
        }
        else if(b_key_down_pull==true)
        {
            b_key_down_pull=false;
            if(m_mcs->e2proomdata.camdlg_cvimg_posY3<m_mcs->e2proomdata.camdlg_cvimg_posY3_max)
            {
                m_mcs->e2proomdata.camdlg_cvimg_posY3=m_mcs->e2proomdata.camdlg_cvimg_posY3+1;
            }
        }
        if(ui->widget->b_mouse_push==true)
        {
            m_mcs->e2proomdata.camdlg_cvimg_posX3=ui->widget->mousePos.rx()*d_zoomx;
            m_mcs->e2proomdata.camdlg_cvimg_posY3=ui->widget->mousePos.ry()*d_zoomy;
        }
    }
    if(b_key_4_push==true)
    {
        if(b_key_left_pull==true)
        {
            b_key_left_pull=false;
            if(m_mcs->e2proomdata.camdlg_cvimg_posX4>m_mcs->e2proomdata.camdlg_cvimg_posX4_min)
            {
                m_mcs->e2proomdata.camdlg_cvimg_posX4=m_mcs->e2proomdata.camdlg_cvimg_posX4-1;
            }
        }
        else if(b_key_right_pull==true)
        {
            b_key_right_pull=false;
            if(m_mcs->e2proomdata.camdlg_cvimg_posX4<m_mcs->e2proomdata.camdlg_cvimg_posX4_max)
            {
                m_mcs->e2proomdata.camdlg_cvimg_posX4=m_mcs->e2proomdata.camdlg_cvimg_posX4+1;
            }
        }
        if(b_key_up_pull==true)
        {
            b_key_up_pull=false;
            if(m_mcs->e2proomdata.camdlg_cvimg_posY4>m_mcs->e2proomdata.camdlg_cvimg_posY4_min)
            {
                m_mcs->e2proomdata.camdlg_cvimg_posY4=m_mcs->e2proomdata.camdlg_cvimg_posY4-1;
            }
        }
        else if(b_key_down_pull==true)
        {
            b_key_down_pull=false;
            if(m_mcs->e2proomdata.camdlg_cvimg_posY4<m_mcs->e2proomdata.camdlg_cvimg_posY4_max)
            {
                m_mcs->e2proomdata.camdlg_cvimg_posY4=m_mcs->e2proomdata.camdlg_cvimg_posY4+1;
            }
        }
        if(ui->widget->b_mouse_push==true)
        {
            m_mcs->e2proomdata.camdlg_cvimg_posX4=ui->widget->mousePos.rx()*d_zoomx;
            m_mcs->e2proomdata.camdlg_cvimg_posY4=ui->widget->mousePos.ry()*d_zoomy;
        }
    }

    std::vector<cv::Point2f> points_trans(1),corners_trans;
    points_trans[0]=cv::Point2f(ui->widget->mousePos.rx()*d_zoomx,ui->widget->mousePos.ry()*d_zoomy);
    auto _homo = cv::Mat(m_mcs->cam->sop_cam[0].ros_Params.homography_matrix, true).reshape(1, 3);
    cv::perspectiveTransform(points_trans, corners_trans, _homo);
    ui->posY_label->setText(QString::number(corners_trans[0].x,'f',2));
    ui->posZ_label->setText(QString::number(corners_trans[0].y,'f',2));


    cv::Point2f p1(m_mcs->e2proomdata.camdlg_cvimg_posX1,m_mcs->e2proomdata.camdlg_cvimg_posY1);
    cv::Point2f p1_1=p1;
    p1_1.y=p1_1.y-25;
    cv::putText(cvimg,"1",p1_1,cv::FONT_HERSHEY_SCRIPT_SIMPLEX,2,cv::Scalar(0,0,255),3);
    cv::circle(cvimg,p1,20,cv::Scalar(0,0,255),3);
    cv::circle(cvimg,p1,1,cv::Scalar(0,0,255),3);

    cv::Point2f p2(m_mcs->e2proomdata.camdlg_cvimg_posX2,m_mcs->e2proomdata.camdlg_cvimg_posY2);
    cv::Point2f p2_2=p2;
    p2_2.y=p2_2.y-25;
    cv::putText(cvimg,"2",p2_2,cv::FONT_HERSHEY_SCRIPT_SIMPLEX,2,cv::Scalar(0,255,0),3);
    cv::circle(cvimg,p2,20,cv::Scalar(0,255,0),3);
    cv::circle(cvimg,p2,1,cv::Scalar(0,255,0),3);

    cv::Point2f p3(m_mcs->e2proomdata.camdlg_cvimg_posX3,m_mcs->e2proomdata.camdlg_cvimg_posY3);
    cv::Point2f p3_3=p3;
    p3_3.y=p3_3.y-25;
    cv::putText(cvimg,"3",p3_3,cv::FONT_HERSHEY_SCRIPT_SIMPLEX,2,cv::Scalar(255,0,0),3);
    cv::circle(cvimg,p3,20,cv::Scalar(255,0,0),3);
    cv::circle(cvimg,p3,1,cv::Scalar(255,0,0),3);

    cv::Point2f p4(m_mcs->e2proomdata.camdlg_cvimg_posX4,m_mcs->e2proomdata.camdlg_cvimg_posY4);
    cv::Point2f p4_4=p4;
    p4_4.y=p4_4.y-25;
    cv::putText(cvimg,"4",p4_4,cv::FONT_HERSHEY_SCRIPT_SIMPLEX,2,cv::Scalar(255,255,0),3);
    cv::circle(cvimg,p4,20,cv::Scalar(255,255,0),3);
    cv::circle(cvimg,p4,1,cv::Scalar(255,255,0),3);
}

void cambuilddlg::init_show_cambuild_inlab(cv::Mat cvimg)
{
    if(!cvimg.empty())
    {
         if(cvimg.rows!=CAMBUILD_IMAGE_HEIGHT||
            cvimg.cols!=CAMBUILD_IMAGE_WIDTH)
            cv::resize(cvimg,cvimg,cv::Size(CAMBUILD_IMAGE_WIDTH,CAMBUILD_IMAGE_HEIGHT));
         if(cvimg.type()==CV_8UC1)
            cv::cvtColor(cvimg,cvimg,cv::COLOR_GRAY2BGR);

         drow_point(cvimg);
         QImage::Format format = QImage::Format_RGB888;
         switch (cvimg.type())
         {
         case CV_8UC1:
           format = QImage::Format_RGB888;
           cv::cvtColor(cvimg,cvimg,cv::COLOR_GRAY2BGR);
           break;
         case CV_8UC3:
           format = QImage::Format_RGB888;
           break;
         case CV_8UC4:
           format = QImage::Format_ARGB32;
           break;
         }
         QImage img = QImage((const uchar*)cvimg.data,
                                           cvimg.cols,
                                           cvimg.rows,
                                           cvimg.cols * cvimg.channels(), format);
         img = img.scaled(ui->widget->width(),ui->widget->height(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//图片自适应lab大小
         ui->widget->setImage(img);
    }
    b_init_show_cambuild_inlab_finish=true;
}

void cambuilddlg::keyPressEvent(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_1)
    {
        b_key_1_push=true;
        return;
    }
    if(ev->key() == Qt::Key_2)
    {
        b_key_2_push=true;
        return;
    }
    if(ev->key() == Qt::Key_3)
    {
        b_key_3_push=true;
        return;
    }
    if(ev->key() == Qt::Key_4)
    {
        b_key_4_push=true;
        return;
    }
    QWidget::keyPressEvent(ev);
}

void cambuilddlg::keyReleaseEvent(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_1)
    {
        b_key_1_push=false;
        return;
    }
    if(ev->key() == Qt::Key_2)
    {
        b_key_2_push=false;
        return;
    }
    if(ev->key() == Qt::Key_3)
    {
        b_key_3_push=false;
        return;
    }
    if(ev->key() == Qt::Key_4)
    {
        b_key_4_push=false;
        return;
    }
    if(ev->key() == Qt::Key_Left)
    {
        b_key_left_pull=true;
        return;
    }
    if(ev->key() == Qt::Key_Right)
    {
        b_key_right_pull=true;
        return;
    }
    if(ev->key() == Qt::Key_Up)
    {
        b_key_up_pull=true;
        return;
    }
    if(ev->key() == Qt::Key_Down)
    {
        b_key_down_pull=true;
        return;
    }
    QWidget::keyReleaseEvent(ev);
}

cambuildThread::cambuildThread(cambuilddlg *statci_p)
{
    _p=statci_p;
}

void cambuildThread::run()
{
    while (1)
    {
        if(_p->b_thread1==true)
        {
            if(_p->b_init_show_cambuild_inlab_finish==true)
            {
                _p->b_init_show_cambuild_inlab_finish=false;
                qRegisterMetaType< cv::Mat >("cv::Mat"); //传递自定义类型信号时要添加注册
                emit Send_show_cambuild_inlab(_p->m_mcs->cam->sop_cam[0].cv_image);
            }
            sleep(0);
        }
        else
        {
            _p->b_stop_thread1=true;
            break;
        }
    }
}

void cambuildThread::Stop()
{
  if(_p->b_thread1==true)
  {
    _p->b_stop_thread1=false;
    _p->b_thread1=false;
    while (_p->b_stop_thread1==false)
    {
      sleep(0);
    }
  }
}
