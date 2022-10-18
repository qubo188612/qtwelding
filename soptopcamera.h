#ifndef SOPTOPCAMERA_H
#define SOPTOPCAMERA_H
#include "global.h"
#if _MSC_VER||WINDOWS_TCP
#else
#include "opencv2/core/mat.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <sensor_msgs/msg/image.hpp>
#include "tutorial_interfaces/msg/if_algorhmitmsg.hpp"
#include "tutorial_interfaces/msg/if_algorhmitcloud.hpp"
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <QThread>
#include <QLabel>
#include "FileOut.h"
#include <QTimer>
#include <QObject>
#include <QTimerEvent>
#include <QProcess>
#include <global.h>
#include "PictureBox.h"

#define SOPTOPCAM_SAVEBUFF		32
#define SOPTOPCAM_SYSPATH_MOTO	"./SAVE/SOPTOPCAM.bsd"

#define SOPTOPCAM_EXPOSURE_MAX    65535
#define SOPTOPCAM_EXPOSURE_USE    800
#define SOPTOPCAM_EXPOSURE_MIN    20

using std::placeholders::_1;
class StartCameraThread;

struct Params
{
  std::vector<double> homography_matrix;
};

class SoptopCamera : public QObject
{
public:
    SoptopCamera();
    ~SoptopCamera();

    QString ipaddress;

    int argc;
    char **argv;

    void InitConnect(PictureBox *lab_show); //连接相机显示图像
    void InitConnect();//连接相机不显示图像
    void DisConnect();
    bool b_connect;
    bool stop_b_connect;
    bool b_connetc_noimage;     //是否显示图像的连接

    int i32_exposure;
    int i32_exposure_max;
    int i32_exposure_min;
    int i32_exposure_use;

    Params ros_Params;

    void roscmd_set_exposure(int exposure);
    int roscmd_get_exposure(int *exposure);

    void roscmd_open_laser(bool b);
    void roscmd_open_camera(bool b);


    cv::Mat cv_image;       //相机图像

    tutorial_interfaces::msg::IfAlgorhmitcloud *ros_line;     //相机轮廓
    bool b_ros_lineEn;     //相机轮廓有效位

    PictureBox *m_lab_show;

    cv_bridge::CvImagePtr cv_ptr;

    void int_show_image_inlab();
    volatile bool b_int_show_image_inlab;

    void write_para();
    void init_para();

    volatile bool b_updataimage_finish;//相机图像更新完成
    volatile bool b_updatacloud_finish;//相机点云更新完成

    volatile int callbacknumber;

    bool callback_error;

    bool luzhi;
    cv::VideoWriter writer;
    double Getfps();
    void StartRecord(QString filename);
    void StopRecord();

    volatile bool b_stopthred;

    std::shared_ptr<rclcpp::AsyncParametersClient> _param_camera;
    std::shared_ptr<rclcpp::AsyncParametersClient> _param_gpio;
    std::shared_ptr<rclcpp::AsyncParametersClient> _param_camera_get;
    std::shared_ptr<rclcpp::AsyncParametersClient> _param_homography_matrix;
    std::shared_ptr<rclcpp::AsyncParametersClient> _param_homography_matrix_get;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr _pub_config;

    void ros_open_laser(bool b);
    void ros_open_camera(bool b);
    void ros_set_exposure(int exposure);
    void ros_set_homography_matrix(Params ros_Params);

    void ros_config_set(std::string msg);
protected:
    StartCameraThread *StartCamera_thread;

    void read_para();
    void check_para();

    virtual void timerEvent(QTimerEvent *event);
    int timerid1;

    volatile int oldcallbacknumber;

    QImage img,img2;
};

class StartCameraThread : public QThread
{

public:
    StartCameraThread(SoptopCamera *statci_p);
protected:
    void run();
private:
    SoptopCamera *_p;
};

typedef void (*pUpdateMainThreadQImage)(QImage image);

class Camshow : public rclcpp::Node
{
public:
    Camshow(SoptopCamera *statci_p);
    ~Camshow();

private:
    SoptopCamera *_p;

    void callbackGlobalParam(std::shared_future<std::vector<rclcpp::Parameter>> future);
    void callbackMatrixParam(std::shared_future<std::vector<rclcpp::Parameter>> future);

#ifdef DEBUG_MYINTERFACES
    rclcpp::Subscription<tutorial_interfaces::msg::IfAlgorhmitmsg>::SharedPtr subscription_;
#else
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;
#endif

#ifdef DEBUG_MYINTERFACES
    void topic_callback(const tutorial_interfaces::msg::IfAlgorhmitmsg msg)  const;
#else
    void topic_callback(const sensor_msgs::msg::Image msg)  const;
#endif

    rclcpp::Subscription<tutorial_interfaces::msg::IfAlgorhmitcloud>::SharedPtr subscricloud_;

    void cloud_callback(const tutorial_interfaces::msg::IfAlgorhmitcloud msg)  const;
public:

};
#endif
#endif // SOPTOPCAMERA_H
