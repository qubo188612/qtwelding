#ifndef SOPTOCAMERATCPIP_H
#define SOPTOCAMERATCPIP_H
#include "global.h"
#if _MSC_VER||WINDOWS_TCP
#include "PictureBox.h"
#include <opencv2/opencv.hpp>
#include <QThread>
#include "XTcp.h"
#include "opencv2/imgcodecs/legacy/constants_c.h"
#if _MSC_VER

#else
#include <unistd.h>
#include <sys/stat.h>
#endif

#define RECVBUFFER_MAX      CAMBUILD_IMAGE_HEIGHT*CAMBUILD_IMAGE_WIDTH*3

class tcprcvThread;

class Soptocameratcpip
{
public:
    Soptocameratcpip();
    ~Soptocameratcpip();

    QString ipaddress;

    void InitConnect(PictureBox *lab_show,QString hostName,int port);
    void DisConnect();
    bool b_connect;

    cv::Mat cv_image;

    void StartRecord(QString filename);
    void StopRecord();

    volatile bool b_int_show_image_inlab;

    volatile bool b_updataimage_finish;

    volatile int callbacknumber;

    XTcp m_client;
    tcprcvThread *rcv_thread;
    bool b_rcv_thread;
    bool b_stop_rcv_thread;

    uchar *rcv_buf;

    bool luzhi;
    cv::VideoWriter writer;

protected:

    PictureBox *m_lab_show;

    double Getfps();
};

class tcprcvThread : public QThread
{
    Q_OBJECT

public:
    tcprcvThread(Soptocameratcpip *statci_p);
    void Stop();
protected:
    void run();
private:
    Soptocameratcpip *_p;

    int myimgtcp(std::vector<uchar> decode,cv::Mat &image);

    int myimgtcp2(std::vector<uchar> decode,cv::Mat &image);

    cv::Mat image;
};

#endif
#endif // SOPTOCAMERATCPIP_H
