#include "soptopcamera.h"
#if _MSC_VER||WINDOWS_TCP
#else
Camshow::Camshow(SoptopCamera *statci_p): Node("my_eyes")
{
  _p=statci_p;

  _p->b_int_show_image_inlab=false;

  _p->_param_camera = std::make_shared<rclcpp::AsyncParametersClient>(this, "camera_tis_node");
  _p->_param_camera_get = std::make_shared<rclcpp::AsyncParametersClient>(this, "camera_tis_node");
  _p->_param_gpio = std::make_shared<rclcpp::AsyncParametersClient>(this, "gpio_raspberry_node");
  _p->_param_homography_matrix =  std::make_shared<rclcpp::AsyncParametersClient>(this, "line_center_reconstruction_node");
  _p->_param_homography_matrix_get = std::make_shared<rclcpp::AsyncParametersClient>(this, "line_center_reconstruction_node");
  _p->_pub_config=this->create_publisher<std_msgs::msg::String>("config_tis_node/config", 10);

  if(_p->b_connetc_noimage==false)
  {
#ifdef DEBUG_MYINTERFACES
  subscription_ = this->create_subscription<tutorial_interfaces::msg::IfAlgorhmitmsg>(
        "/laser_imagepos_node/result", rclcpp::SensorDataQoS(), std::bind(&Camshow::topic_callback, this, _1));
#else
  subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
        "/rotate_image_node/image_rotated", rclcpp::SensorDataQoS(), std::bind(&Camshow::topic_callback, this, _1));
#endif
  }
  else
  {
  subscricloud_ = this->create_subscription<tutorial_interfaces::msg::IfAlgorhmitcloud>(
        "line_center_reconstruction_node/cloud_task100_199", rclcpp::SensorDataQoS(), std::bind(&Camshow::cloud_callback, this, _1));
  }

  _p->_param_camera_get->wait_for_service();
  _p->_param_camera_get->get_parameters(
                  {"exposure_time"},
                  std::bind(&Camshow::callbackGlobalParam, this, std::placeholders::_1));

  _p->_param_homography_matrix_get->wait_for_service();
  _p->_param_homography_matrix_get->get_parameters(
                  {"homography_matrix"},
                  std::bind(&Camshow::callbackMatrixParam, this, std::placeholders::_1));


}

Camshow::~Camshow()
{
    _p->_param_camera.reset();
    _p->_param_camera_get.reset();
    _p->_param_gpio.reset();
    _p->_param_homography_matrix.reset();
    _p->_param_homography_matrix_get.reset();
    _p->_pub_config.reset();
}

#ifdef DEBUG_MYINTERFACES
    void Camshow::topic_callback(const tutorial_interfaces::msg::IfAlgorhmitmsg msg)  const
    {
      if(_p->b_connect==true)
      {
        if(_p->b_int_show_image_inlab==false&&_p->b_updataimage_finish==false)
        {
            _p->b_int_show_image_inlab=true;
            _p->cv_ptr = cv_bridge::toCvCopy(msg.imageout, msg.imageout.encoding);
            if(!_p->cv_ptr->image.empty())
            {
                _p->cv_image=_p->cv_ptr->image;
                _p->b_updataimage_finish=true;
                _p->callbacknumber++;
                if(_p->luzhi==true)
                {
                    _p->writer << _p->cv_ptr->image;
                }
            }
        }
      }
      else
      {
        rclcpp::shutdown();
        _p->stop_b_connect=true;
      }
    }
#else
    void Camshow::topic_callback(const sensor_msgs::msg::Image msg)  const
    {
      if(_p->b_connect==true)
      {
        if(_p->b_int_show_image_inlab==false)
        {
            cv_bridge::CvImagePtr cv_ptr;
            cv_ptr = cv_bridge::toCvCopy(msg, "mono8");
            if(!cv_ptr->image.empty())
            {
                (_p->cv_image)=cv_ptr->image.clone();
                _p->b_updataimage_finish=true;
                _p->callbacknumber++;
                if(_p->luzhi==true)
                {
                    _p->writer << cv_ptr->image;
                }
            }
        }
      }
      else
      {
        rclcpp::shutdown();
        _p->stop_b_connect=true;
      }
    }
#endif

void Camshow::cloud_callback(const tutorial_interfaces::msg::IfAlgorhmitcloud msg)  const
{
    if(_p->b_connect==true)
    {
        if(msg.lasertrackoutcloud.size()>0)
        {
            (*(_p->ros_line))=msg;
            _p->b_ros_lineEn=true;
        }
        else
        {
            _p->b_ros_lineEn=false;
        }
        _p->b_updatacloud_finish=true;
        _p->callbacknumber++;
    }
    else
    {
        rclcpp::shutdown();
        _p->stop_b_connect=true;
    }
}

void SoptopCamera::ros_open_laser(bool b)
{
    if(b==false)
        _param_gpio->set_parameters({rclcpp::Parameter("laser", false)});  //激光关闭
    else if(b==true)
        _param_gpio->set_parameters({rclcpp::Parameter("laser", true)});    //激光打开
}

void SoptopCamera::ros_open_camera(bool b)
{
    if(b==false)
        _param_camera->set_parameters({rclcpp::Parameter("power", false)});  //相机关闭
    else if(b==true)
        _param_camera->set_parameters({rclcpp::Parameter("power", true)});    //相机打开
}

void SoptopCamera::ros_set_exposure(int exposure)
{
    _param_camera->set_parameters({rclcpp::Parameter("exposure_time", exposure)});
}

void SoptopCamera::ros_set_homography_matrix(Params ros_Params)
{
    _param_homography_matrix->set_parameters({rclcpp::Parameter("homography_matrix", ros_Params.homography_matrix)});
}

void SoptopCamera::ros_config_set(std::string msg)
{
    std_msgs::msg::String::UniquePtr config_msg(new std_msgs::msg::String());

    config_msg->data=msg;

    _pub_config->publish(std::move(config_msg));
}

void Camshow::callbackGlobalParam(std::shared_future<std::vector<rclcpp::Parameter>> future)
{
    auto result = future.get();
    if(result.size()>=1)
    {
      _p->i32_exposure = result.at(0).as_int();
    }
}

void Camshow::callbackMatrixParam(std::shared_future<std::vector<rclcpp::Parameter>> future)
{
    auto result = future.get();
    if(result.size()>=1)
    {
      _p->ros_Params.homography_matrix = result.at(0).as_double_array();
    }
}

SoptopCamera::SoptopCamera()
{  
  ipaddress="192.168.1.2";

  i32_exposure_min=SOPTOPCAM_EXPOSURE_MIN;
  i32_exposure_max=SOPTOPCAM_EXPOSURE_MAX;
  i32_exposure_use=SOPTOPCAM_EXPOSURE_USE;

  read_para();

  b_connect=false;
  stop_b_connect=true;
  b_updataimage_finish=false;
  b_updatacloud_finish=false;
  StartCamera_thread = NULL;
//StartCamera_thread = new StartCameraThread(this);
  callbacknumber=0;
  oldcallbacknumber=0;
  callback_error=0;
  luzhi=false;
  b_stopthred=true;
  b_connetc_noimage=false;

  ros_line=new tutorial_interfaces::msg::IfAlgorhmitcloud;
}

SoptopCamera::~SoptopCamera()
{
  if(StartCamera_thread!=NULL)
  {
      StartCamera_thread->quit();
      StartCamera_thread->wait();
      delete StartCamera_thread;
      StartCamera_thread=NULL;
  }
  delete ros_line;
}

void SoptopCamera::timerEvent(QTimerEvent *event)
{
    if(event->timerId()==timerid1)
    {
        if(b_connect==true||stop_b_connect==false)
        {
            if(callbacknumber==oldcallbacknumber)
            {
                callback_error=1;
            }
            else
            {
                callback_error=0;
            }
            oldcallbacknumber=callbacknumber;
        }
    }
}

void SoptopCamera::read_para()
{
    uint8_t *buff=new uint8_t[SOPTOPCAM_SAVEBUFF];
    if(buff==NULL)
    {
      init_para();
      return;
    }
    CFileOut fo;
    if(0 > fo.ReadFile((char*)SOPTOPCAM_SYSPATH_MOTO,buff,SOPTOPCAM_SAVEBUFF))
    {
      init_para();
      return;
    }
    int32_t *i32_p;
    i32_p = (int32_t*)buff;
    i32_exposure = *i32_p;
    i32_p++;

    delete[] buff;
}

void SoptopCamera::write_para()
{
    check_para();
    uint8_t *buff=new uint8_t[SOPTOPCAM_SAVEBUFF];
    if(buff==NULL)
      return;

    int32_t *i32_p;

    i32_p = (int32_t*)buff;
    *i32_p = i32_exposure;
    i32_p ++;

    CFileOut fo;
    fo.WriteFile((char*)SOPTOPCAM_SYSPATH_MOTO,buff,SOPTOPCAM_SAVEBUFF);

    if(buff!=NULL)
      delete []buff;
}

void SoptopCamera::check_para()
{
    if(i32_exposure<i32_exposure_min||i32_exposure>i32_exposure_max)
    {
      i32_exposure=i32_exposure_use;
    }
}

void SoptopCamera::init_para()
{
    i32_exposure=i32_exposure_use;
}

void SoptopCamera::InitConnect(PictureBox *lab_show)
{
    if(b_connect==false)
    {
        b_connetc_noimage=false;
        callbacknumber=0;
        oldcallbacknumber=0;
        timerid1 = startTimer(1000);
        m_lab_show=lab_show;
        b_connect=true;
        if(StartCamera_thread==NULL)
        {
            StartCamera_thread = new StartCameraThread(this);
            StartCamera_thread->start();
        }
    }
}

void SoptopCamera::InitConnect()
{
    if(b_connect==false)
    {
        b_connetc_noimage=true;
        callbacknumber=0;
        oldcallbacknumber=0;
        timerid1 = startTimer(1000);
        b_connect=true;
        if(StartCamera_thread==NULL)
        {
            StartCamera_thread = new StartCameraThread(this);
            StartCamera_thread->start();
        }
    }
}

void SoptopCamera::DisConnect()
{
    if(b_connect==true)
    {
        stop_b_connect=false;
        b_connect=false;
        while (stop_b_connect==false||b_stopthred==false)
        {
          QThread::sleep(0);
          if(callback_error==1)
          {
              rclcpp::shutdown();
              stop_b_connect=true;
              break;
          }
        }
        killTimer(timerid1);

        if(StartCamera_thread!=NULL)
        {
            StartCamera_thread->quit();
            StartCamera_thread->wait();
            delete StartCamera_thread;
            StartCamera_thread=NULL;
        }
    }
}

void SoptopCamera::roscmd_set_exposure(int exposure)
{
  if(b_connect==true)
  {
    QString array="ros2 param set /camera_tis_node exposure_time ";
    QString data=QString::number(exposure);
    array=array+data;
    system(array.toUtf8());
  }
}

int SoptopCamera::roscmd_get_exposure(int *exposure)
{
    if(b_connect==true)
    {
      QString array="ros2 param get /camera_tis_node exposure_time ";
      QProcess process;
      process.start(array);
      process.waitForFinished();
      QString result = process.readAllStandardOutput();
      if(result.size()>0)
      {
          QString msg="Integer value is: ";
          if(result.contains(msg,Qt::CaseSensitive))
          {
              int a = result.indexOf(msg);
              int b = result.indexOf("\n",a+msg.size());
              QString data = result.mid(a+msg.size(),b-a-msg.size());
              *exposure=data.toInt();
          }
          else
          {
              return 1;
          }
      }
    }
    return 0;
}

void SoptopCamera::roscmd_open_laser(bool b)
{
    if(b==false)
        system("ros2 param set gpio_raspberry_node laser False");
    else if(b==true)
        system("ros2 param set gpio_raspberry_node laser True");
}

void SoptopCamera::roscmd_open_camera(bool b)
{
    if(b==false)
        system("ros2 param set /camera_tis_node power False");
    else if(b==true)
        system("ros2 param set /camera_tis_node power True");
}

void SoptopCamera::int_show_image_inlab()
{
  QImage::Format format = QImage::Format_RGB888;
  switch (cv_image.type())
  {
  case CV_8UC1:
    format = QImage::Format_RGB888;
    cv::cvtColor(cv_image,cv_image,cv::COLOR_GRAY2BGR);
    break;
  case CV_8UC3:
    format = QImage::Format_RGB888;
    break;
  case CV_8UC4:
    format = QImage::Format_ARGB32;
    break;
  }
  img = QImage((const uchar*)cv_image.data, cv_image.cols, cv_image.rows,
  cv_image.cols * cv_image.channels(), format);
  img2 = img.scaled(m_lab_show->width(),m_lab_show->height(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  m_lab_show->setImage(img2);
}

double SoptopCamera::Getfps()
{
    return 50.0;
}

void SoptopCamera::StartRecord(QString filename)
{
    bool isColor = (cv_image.type()==CV_8UC3);
    double fps     = Getfps();
    int frameW  = cv_image.cols;
    int frameH  = cv_image.rows;
    int codec=cv::VideoWriter::fourcc('X','V','I','D');
    writer.open(filename.toStdString(),codec,fps,cv::Size(frameW,frameH),isColor);
    luzhi=true;
}

void SoptopCamera::StopRecord()
{
    luzhi=false;
    writer.release();
}


StartCameraThread::StartCameraThread(SoptopCamera *statci_p)
{
  _p=statci_p;
}

void StartCameraThread::run()
{
  _p->b_stopthred=false;
  if(_p->b_connect==true)
  {
      rclcpp::init(_p->argc,_p->argv);
      rclcpp::spin(std::make_shared<Camshow>(_p));
  }
  _p->b_stopthred=true;
}
#endif
