#include "pshowdlg.h"
#include "ui_pshowdlg.h"

pshowdlg::pshowdlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pshowdlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
}

pshowdlg::~pshowdlg()
{
    delete ui;
}

void pshowdlg::init_dlg_show()
{
    link_pshow_state=false;
    b_init_show_pshow_text_finish=true;

    QString server_ip=m_mcs->ip->camer_ip[0].ip;
    QString server_port2=QString::number(PORT_ALS_RESULT);
    ctx_pshow = modbus_new_tcp(server_ip.toUtf8(), server_port2.toInt());
    if (modbus_connect(ctx_pshow) == 0)
    {
        link_pshow_state=true;
    }

    thread1 = new pshowdlgThread(this);
    connect(thread1, SIGNAL(Send_show_pshow_text()), this, SLOT(init_show_pshow_text()));

    b_thread1=true;
    thread1->start();
}

void pshowdlg::close_dlg_show()
{
    if(link_pshow_state==true)
    {
        modbus_close(ctx_pshow);
    }
    link_pshow_state=false;
    modbus_free(ctx_pshow);

    thread1->Stop();
    thread1->quit();
    thread1->wait();

    delete thread1;
}

void pshowdlg::init_show_pshow_text()
{
    u_int16_t u16_data[2];
    int32_t *i32_data=(int32_t*)u16_data;
    float X,Y,Z,RX,RY,RZ;
    int32_t out1,out2,out3;
    u_int16_t tool,tcp,usertcp;
    u16_data[0]=pos_data[0];
    u16_data[1]=pos_data[1];
    X=*i32_data/1000.0;
    u16_data[0]=pos_data[2];
    u16_data[1]=pos_data[3];
    Y=*i32_data/1000.0;
    u16_data[0]=pos_data[4];
    u16_data[1]=pos_data[5];
    Z=*i32_data/1000.0;
    u16_data[0]=pos_data[6];
    u16_data[1]=pos_data[7];
    RX=*i32_data/10000.0;
    u16_data[0]=pos_data[8];
    u16_data[1]=pos_data[9];
    RY=*i32_data/10000.0;
    u16_data[0]=pos_data[10];
    u16_data[1]=pos_data[11];
    RZ=*i32_data/10000.0;
    u16_data[0]=pos_data[12];
    u16_data[1]=pos_data[13];
    out1=*i32_data;
    u16_data[0]=pos_data[14];
    u16_data[1]=pos_data[15];
    out2=*i32_data;
    u16_data[0]=pos_data[16];
    u16_data[1]=pos_data[17];
    out3=*i32_data;
    tool=pos_data[18];
    tcp=pos_data[19];
    usertcp=pos_data[20];

    ui->realposX->setText(QString::number(X,'f',3));
    ui->realposY->setText(QString::number(Y,'f',3));
    ui->realposZ->setText(QString::number(Z,'f',3));
    ui->realposRX->setText(QString::number(RX,'f',4));
    ui->realposRY->setText(QString::number(RY,'f',4));
    ui->realposRZ->setText(QString::number(RZ,'f',4));
    ui->realout1->setText(QString::number(out1));
    ui->realout2->setText(QString::number(out2));
    ui->realout3->setText(QString::number(out3));
    ui->realtool->setText(QString::number(tool));
    ui->realtcp->setText(QString::number(tcp));
    ui->realusertcp->setText(QString::number(usertcp));
    b_init_show_pshow_text_finish=true;
}

pshowdlgThread::pshowdlgThread(pshowdlg *statci_p)
{
    _p=statci_p;
}

void pshowdlgThread::run()
{
    while (1)
    {
        if(_p->b_thread1==true)
        {
            if(_p->link_pshow_state==true)
            {
                int real_readnum_1=modbus_read_registers(_p->ctx_pshow,ALS_REALTIME_POSX_REG_ADD,0x15,_p->pos_data);
                if(real_readnum_1==0x15)
                {
                    if(_p->b_init_show_pshow_text_finish==true)
                    {
                        _p->b_init_show_pshow_text_finish=false;

                        emit Send_show_pshow_text();
                    }
                }
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

void pshowdlgThread::Stop()
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
