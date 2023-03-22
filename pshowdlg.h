#ifndef PSHOWDLG_H
#define PSHOWDLG_H

#include <QDialog>
#include "my_parameters.h"
#include <modbus/modbus.h>

namespace Ui {
class pshowdlg;
}

class pshowdlgThread;

class pshowdlg : public QDialog
{
    Q_OBJECT

public:
    explicit pshowdlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~pshowdlg();

    my_parameters *m_mcs;

    void init_dlg_show();
    void close_dlg_show();

    pshowdlgThread *thread1;
    bool b_thread1;
    bool b_stop_thread1;
    bool b_init_show_pshow_text_finish;

    modbus_t *ctx_pshow;
    bool link_pshow_state;

    unsigned short pos_data[0x15];

private:
    Ui::pshowdlg *ui;

private slots:
    void init_show_pshow_text();
};

class pshowdlgThread : public QThread
{
    Q_OBJECT

public:
    pshowdlgThread(pshowdlg *statci_p);
    void Stop();
protected:
    void run();
private:
    pshowdlg *_p;

signals:
    // 自定义信号
    void Send_show_pshow_text();
};

#endif // PSHOWDLG_H
