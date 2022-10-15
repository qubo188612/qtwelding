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

namespace Ui {
class qtweldingDlg;
}

class qtweldingThread;

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

private slots:
    void on_importprojectBtn_clicked();

    void on_runprojectBtn_clicked();

    void on_editprojectBtn_clicked();

    void on_editweldprocessBtn_clicked();

    void on_setlaserheadBtn_clicked();

    void on_setrobotBtn_clicked();

    void on_setwelderBtn_clicked();

    void init_show_ui_list();

private:
    Ui::qtweldingDlg *ui;
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
};

#endif // QTWELDINGDLG_H
