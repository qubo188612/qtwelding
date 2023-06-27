#ifndef SETPROJECTDLG_H
#define SETPROJECTDLG_H

#include <QDialog>
#include <QListWidgetItem>
#include <QDoubleValidator>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextCodec>
#include "my_parameters.h"
#include "my_cmd.h"
#include "setmovecdlg.h"
#include "othercmddlg.h"
#include "edittextdlg.h"
#include "keycamdlg.h"
#include "keycreatdlg.h"
#include "keydelaydlg.h"
#include "keyiodlg.h"
#include "keyplcdlg.h"
#include "keymovdlg.h"
#include "keyscandlg.h"
#include "keytracedlg.h"
#include "keywelddlg.h"
#include "keysearchdlg.h"
#include "keycoorddlg.h"
#include "keygetposdlg.h"
#include "keysmovdlg.h"
#include "keysscandlg.h"
#include "keysearchenddlg.h"
#include "keysampledlg.h"
#include "keytracingdlg.h"
#include "keytraceadddlg.h"
#include "keygowelddlg.h"
#include "keywavedlg.h"
#include "keyaoutdlg.h"
#include "keycreatpdlg.h"
#include "keysetposedlg.h"
#include "keygettcpposdlg.h"
#include "keycreatfdlg.h"
#include "keyplotposdlg.h"
#include "keycreatcdlg.h"
#include "keygettcppos2dlg.h"
#include "keytracecontinuedlg.h"
#include "arrvepointtestdlg.h"
#include "traceedit0dlg.h"
#include "traceedit1dlg.h"
#include "traceedit2dlg.h"

namespace Ui {
class setprojectDlg;
}

class setprojectThread;

class setprojectDlg : public QDialog
{
    Q_OBJECT

public:
    explicit setprojectDlg(my_parameters *mcs,QWidget *parent = nullptr);
    ~setprojectDlg();

    edittextDlg *edittext;
    setmovecDlg *setmovec;
    othercmdDlg *othercmd;
    arrvepointtestDlg *arrvepointtest;
    keycamDlg *keycam;
    keycreatDlg *keycreat;
    keydelayDlg *keydelay;
    keyioDlg *keyio;
    keyplcDlg *keyplc;
    keymovDlg *keymov;
    keyscanDlg *keyscan;
    keytraceDlg *keytrace;
    keyweldDlg *keyweld;
    keysearchDlg *keysearch;
    keycoordDlg *keycoord;
    keygetposDlg *keygetpos;
    keysmovDlg *keysmov;
    keysscanDlg *keysscan;
    keysearchendDlg *keysearchend;
    keysampleDlg *keysample;
    keytracingDlg *keytracing;
    keytraceaddDlg *keytraceadd;
    keygoweldDlg *keygoweld;
    keywaveDlg *keywave;
    keyaoutDlg *keyaout;
    keycreatpDlg *keycreatp;
    keysetposeDlg *keysetpose;
    keygettcpposDlg *keygettcppos;
    keycreatfDlg *keycreatf;
    keyplotposDlg *keyplotpos;
    keycreatcDlg *keycreatc;
    keygettcppos2Dlg *keygettcppos2;
    keytracecontinueDlg *keytracecontinue;

    traceedit0Dlg *traceedit0;
    traceedit1Dlg *traceedit1;
    traceedit2Dlg *traceedit2;

    my_parameters *m_mcs;

    void init_dlg_show();
    void close_dlg_show();

    setprojectThread *thread1;
    bool b_thread1;
    bool b_stop_thread1;
    bool b_init_show_setproject_inlab_finish;

private slots:
    void on_moveaddBtn_clicked();

    void on_delaytimeBtn_clicked();

    void on_leaseropenBtn_clicked();

    void on_leasercloseBtn_clicked();

    void on_welderarcingBtn_clicked();

    void on_welderarcoutBtn_clicked();

    void on_scanaddBtn_clicked();

    void on_tracecmdaddBtn_clicked();

    void on_traceeditBtn_clicked();

    void on_customaddBtn_clicked();

    void on_customcheckBtn_clicked();

    void on_cmdlistdeleteBtn_clicked();

    void on_cmdlistclearBtn_clicked();

    void on_OKBtn_clicked();

    void on_cmdlist_itemClicked(QListWidgetItem *item);

    void on_customreplaceBtn_clicked();

    void on_tracefilepathBtn_clicked();

    void on_ConnectCamBtn_clicked();

    void on_setCamtaskBtn_clicked();

    void init_show_setproject_inlab(cv::Mat);

    void on_IOoutputBtn_clicked();

    void on_IOinputBtn_clicked();

    void on_othercmdaddBtn_clicked();

    void on_SaveAsBtn_clicked();

//  void on_ctlrobotEncheckBox_stateChanged(int arg1);

    void on_ctlposXsubBtn_pressed();

    void on_ctlposXsubBtn_released();

    void on_ctlposXaddBtn_pressed();

    void on_ctlposXaddBtn_released();

    void on_ctlposYsubBtn_pressed();

    void on_ctlposYsubBtn_released();

    void on_ctlposYaddBtn_pressed();

    void on_ctlposYaddBtn_released();

    void on_ctlposZsubBtn_pressed();

    void on_ctlposZsubBtn_released();

    void on_ctlposZaddBtn_pressed();

    void on_ctlposZaddBtn_released();

    void on_ctlposRXsubBtn_pressed();

    void on_ctlposRXsubBtn_released();

    void on_ctlposRXaddBtn_pressed();

    void on_ctlposRXaddBtn_released();

    void on_ctlposRYsubBtn_pressed();

    void on_ctlposRYsubBtn_released();

    void on_ctlposRYaddBtn_pressed();

    void on_ctlposRYaddBtn_released();

    void on_ctlposRZsubBtn_pressed();

    void on_ctlposRZsubBtn_released();

    void on_ctlposRZaddBtn_pressed();

    void on_ctlposRZaddBtn_released();

    void on_ctlposOut1subBtn_pressed();

    void on_ctlposOut1subBtn_released();

    void on_ctlposOut1addBtn_pressed();

    void on_ctlposOut1addBtn_released();

    void on_ctlposOut2subBtn_pressed();

    void on_ctlposOut2subBtn_released();

    void on_ctlposOut2addBtn_pressed();

    void on_ctlposOut2addBtn_released();

    void on_ctlposOut3subBtn_pressed();

    void on_ctlposOut3subBtn_released();

    void on_ctlposOut3addBtn_pressed();

    void on_ctlposOut3addBtn_released();

    void on_ctlmovetcpcombo_currentIndexChanged(int index);

    void on_arrvepointtestBtn_clicked();

    void on_ctlrobotEncheckBox_clicked();

private:
    Ui::setprojectDlg *ui;
    int now_cmdline;         //当前指向CMD行数

    void updatacmdlistUi();

    int task_setmaindlgtcp(int tcp);    //设置tcp

    QDoubleValidator *adoubleValidator_speed;    //机器人速度小数
};

class setprojectThread : public QThread
{
    Q_OBJECT

public:
    setprojectThread(setprojectDlg *statci_p);
    void Stop();
protected:
    void run();
private:
    setprojectDlg *_p;

signals:
    // 自定义信号
    void Send_show_setproject_inlab(cv::Mat);
};


#endif // SETPROJECTDLG_H
