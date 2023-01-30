#ifndef SSHPASSWORDDLG_H
#define SSHPASSWORDDLG_H

#include <QFileDialog>
#include <QDialog>
#include "my_parameters.h"

#ifdef DEBUS_SSH

namespace Ui {
class sshpasswordDlg;
}

class sshpasswordDlg : public QDialog
{
    Q_OBJECT

public:
    explicit sshpasswordDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~sshpasswordDlg();

    my_parameters *m_mcs;

    void init_dlg_show();
    void close_dlg_show();
    void SshConnect();
    void createSSHConnection();

private:
    Ui::sshpasswordDlg *ui;
private slots:
    void slotConnectStateChanged(bool bState,QString strIp,int nPort);
    void slotDataArrived(QString strMsg,QString strIp, int nPort);
    void slotsshFileScpfinish(bool bState);
signals:
    void sigSend(QString strMsg);
    void sigDisconnected();
    void sigSendFile(QString file,QString scpto);
};

#endif

#endif // SSHPASSWORDDLG_H
