#ifndef QTWELDINGDLG_H
#define QTWELDINGDLG_H

#include <QDialog>
#include <QFileDialog>
#include <QTextCodec>
#include "my_parameters.h"
#include "qthread.h"
#if _MSC_VER
#include <QDir>
#include <synchapi.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#endif
#include <vector>
#include "qtmysunnydlg.h"
#include "demarcatedlg.h"


namespace Ui {
class qtweldingDlg;
}

class qtweldingThread;


class sent_info_leaser              //发送相机数据
{
public:
    modbus_t *ctx;                  //sock
    int addr;                       //发送寄存器
    std::vector<uint16_t> data;     //发送数据
};

class qtweldingDlg : public QDialog
{
    Q_OBJECT

public:
    explicit qtweldingDlg(QWidget *parent = nullptr);
    ~qtweldingDlg();

    my_parameters *m_mcs;

    qtweldingThread *thread;
    bool b_thread;
    bool b_stop_thread;
    volatile bool b_init_show_ui_list;
    volatile bool b_init_sent_leaser;    //相机往外发送

    qtmysunnyDlg *qtmysunny;
    demarcateDlg *demarcate;

    int ctx_result_dosomeing;   //1502端口忙

    std::vector<sent_info_leaser> send_group_leaser;    //发送相机数据队列
    unsigned short leaser_rcv_data[15];
    unsigned short leaser_rcv_data2[4];

private slots:
    void on_importprojectBtn_clicked();

    void on_runprojectBtn_clicked();

    void on_editprojectBtn_clicked();

    void on_editweldprocessBtn_clicked();

    void on_setlaserheadBtn_clicked();

    void on_setrobotBtn_clicked();

    void on_setwelderBtn_clicked();

    void init_show_ui_list();

    void init_sent_leaser();

    void on_demarcateBtn_clicked();

private:
    Ui::qtweldingDlg *ui;

    void ConnectCamer();       //连接相机
    void DisconnectCamer();    //断开相机

    bool b_RunAlgCamer;
    void RunAlgCamer();        //运行算法
    void StopAlgCamer();       //停止算法

};

class qtweldingThread : public QThread
{
    Q_OBJECT

public:
    qtweldingThread(qtweldingDlg *statci_p);
    void Stop();
protected:
    void run();
private:
    qtweldingDlg *_p;
signals:
    // 自定义信号
    void Send_show_ui_list();
    void Send_sent_leaser();
};

#endif // QTWELDINGDLG_H
