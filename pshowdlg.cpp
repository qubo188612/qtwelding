#include "pshowdlg.h"
#include "ui_pshowdlg.h"

pshowdlg::pshowdlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pshowdlg)
{
    ui->setupUi(this);
    m_mcs=mcs;
    setFixedSize(this->width(), this->height());//禁止拉伸
}

pshowdlg::~pshowdlg()
{
    delete ui;
}

void pshowdlg::init_dlg_show()
{
    ui->a00_label->setText(QString::number(m_mcs->resultdata.pData_demdlg_R[0],'f',3));
    ui->a01_label->setText(QString::number(m_mcs->resultdata.pData_demdlg_R[1],'f',3));
    ui->a02_label->setText(QString::number(m_mcs->resultdata.pData_demdlg_R[2],'f',3));
    ui->a10_label->setText(QString::number(m_mcs->resultdata.pData_demdlg_R[3],'f',3));
    ui->a11_label->setText(QString::number(m_mcs->resultdata.pData_demdlg_R[4],'f',3));
    ui->a12_label->setText(QString::number(m_mcs->resultdata.pData_demdlg_R[5],'f',3));
    ui->a20_label->setText(QString::number(m_mcs->resultdata.pData_demdlg_R[6],'f',3));
    ui->a21_label->setText(QString::number(m_mcs->resultdata.pData_demdlg_R[7],'f',3));
    ui->a22_label->setText(QString::number(m_mcs->resultdata.pData_demdlg_R[8],'f',3));
    ui->t0_label->setText(QString::number(m_mcs->resultdata.pData_demdlg_T[0],'f',3));
    ui->t1_label->setText(QString::number(m_mcs->resultdata.pData_demdlg_T[1],'f',3));
    ui->t2_label->setText(QString::number(m_mcs->resultdata.pData_demdlg_T[2],'f',3));
    ui->c00_label->setText(QString::number(m_mcs->resultdata.pData_matrix_camera2plane[0],'f',3));
    ui->c01_label->setText(QString::number(m_mcs->resultdata.pData_matrix_camera2plane[1],'f',3));
    ui->c02_label->setText(QString::number(m_mcs->resultdata.pData_matrix_camera2plane[2],'f',3));
    ui->c10_label->setText(QString::number(m_mcs->resultdata.pData_matrix_camera2plane[3],'f',3));
    ui->c11_label->setText(QString::number(m_mcs->resultdata.pData_matrix_camera2plane[4],'f',3));
    ui->c12_label->setText(QString::number(m_mcs->resultdata.pData_matrix_camera2plane[5],'f',3));
    ui->c20_label->setText(QString::number(m_mcs->resultdata.pData_matrix_camera2plane[6],'f',3));
    ui->c21_label->setText(QString::number(m_mcs->resultdata.pData_matrix_camera2plane[7],'f',3));
    ui->c22_label->setText(QString::number(m_mcs->resultdata.pData_matrix_camera2plane[8],'f',3));
    ui->p00_label->setText(QString::number(m_mcs->resultdata.pData_matrix_plane2robot[0],'f',3));
    ui->p01_label->setText(QString::number(m_mcs->resultdata.pData_matrix_plane2robot[1],'f',3));
    ui->p02_label->setText(QString::number(m_mcs->resultdata.pData_matrix_plane2robot[2],'f',3));
    ui->p10_label->setText(QString::number(m_mcs->resultdata.pData_matrix_plane2robot[3],'f',3));
    ui->p11_label->setText(QString::number(m_mcs->resultdata.pData_matrix_plane2robot[4],'f',3));
    ui->p12_label->setText(QString::number(m_mcs->resultdata.pData_matrix_plane2robot[5],'f',3));
    ui->p20_label->setText(QString::number(m_mcs->resultdata.pData_matrix_plane2robot[6],'f',3));
    ui->p21_label->setText(QString::number(m_mcs->resultdata.pData_matrix_plane2robot[7],'f',3));
    ui->p22_label->setText(QString::number(m_mcs->resultdata.pData_matrix_plane2robot[8],'f',3));

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
    thread1->Stop();
    thread1->quit();
    thread1->wait();

    delete thread1;

    if(link_pshow_state==true)
    {
        modbus_close(ctx_pshow);
    }
    link_pshow_state=false;
    modbus_free(ctx_pshow);
}

void pshowdlg::getrobinfo()
{
    u_int16_t u16_data[2];
    int32_t *i32_data=(int32_t*)u16_data;
    u16_data[0]=pos_data[0];
    u16_data[1]=pos_data[1];
    robposinfo.x=*i32_data/1000.0;
    u16_data[0]=pos_data[2];
    u16_data[1]=pos_data[3];
    robposinfo.y=*i32_data/1000.0;
    u16_data[0]=pos_data[4];
    u16_data[1]=pos_data[5];
    robposinfo.z=*i32_data/1000.0;
    u16_data[0]=pos_data[6];
    u16_data[1]=pos_data[7];
    robposinfo.rx=*i32_data/10000.0;
    u16_data[0]=pos_data[8];
    u16_data[1]=pos_data[9];
    robposinfo.ry=*i32_data/10000.0;
    u16_data[0]=pos_data[10];
    u16_data[1]=pos_data[11];
    robposinfo.rz=*i32_data/10000.0;
    u16_data[0]=pos_data[12];
    u16_data[1]=pos_data[13];
    robposinfo.out1=*i32_data;
    u16_data[0]=pos_data[14];
    u16_data[1]=pos_data[15];
    robposinfo.out2=*i32_data;
    u16_data[0]=pos_data[16];
    u16_data[1]=pos_data[17];
    robposinfo.out3=*i32_data;
    robposinfo.tool=pos_data[18];
    robposinfo.tcp=pos_data[19];
    robposinfo.usertcp=pos_data[20];
    b_robposfinduv=result_data[0];
    robposinfo.uy=((int16_t)result_data[1])/100.0;
    robposinfo.vz=((int16_t)result_data[2])/100.0;

    u16_data[0]=p_pos_data[0];
    u16_data[1]=p_pos_data[1];
    p_robposinfo.x=*i32_data/1000.0;
    u16_data[0]=p_pos_data[2];
    u16_data[1]=p_pos_data[3];
    p_robposinfo.y=*i32_data/1000.0;
    u16_data[0]=p_pos_data[4];
    u16_data[1]=p_pos_data[5];
    p_robposinfo.z=*i32_data/1000.0;
    u16_data[0]=p_pos_data[6];
    u16_data[1]=p_pos_data[7];
    p_robposinfo.rx=*i32_data/10000.0;
    u16_data[0]=p_pos_data[8];
    u16_data[1]=p_pos_data[9];
    p_robposinfo.ry=*i32_data/10000.0;
    u16_data[0]=p_pos_data[10];
    u16_data[1]=p_pos_data[11];
    p_robposinfo.rz=*i32_data/10000.0;
    u16_data[0]=p_pos_data[12];
    u16_data[1]=p_pos_data[13];
    p_robposinfo.out1=*i32_data;
    u16_data[0]=p_pos_data[14];
    u16_data[1]=p_pos_data[15];
    p_robposinfo.out2=*i32_data;
    u16_data[0]=p_pos_data[16];
    u16_data[1]=p_pos_data[17];
    p_robposinfo.out3=*i32_data;
    p_robposinfo.tool=p_pos_data[18];
    p_robposinfo.tcp=p_pos_data[19];
    p_robposinfo.usertcp=p_pos_data[20];

    p_robposinfo.uy=((int16_t)result_data[1])/100.0;
    p_robposinfo.vz=((int16_t)result_data[2])/100.0;
}

void pshowdlg::init_show_pshow_text()
{
    ui->realposX->setText(QString::number(robposinfo.x,'f',3));
    ui->realposY->setText(QString::number(robposinfo.y,'f',3));
    ui->realposZ->setText(QString::number(robposinfo.z,'f',3));
    ui->realposRX->setText(QString::number(robposinfo.rx,'f',4));
    ui->realposRY->setText(QString::number(robposinfo.ry,'f',4));
    ui->realposRZ->setText(QString::number(robposinfo.rz,'f',4));
    ui->realout1->setText(QString::number(robposinfo.out1));
    ui->realout2->setText(QString::number(robposinfo.out2));
    ui->realout3->setText(QString::number(robposinfo.out3));
    ui->realtool->setText(QString::number(robposinfo.tool));
    ui->realtcp->setText(QString::number(robposinfo.tcp));
    ui->realusertcp->setText(QString::number(robposinfo.usertcp));
    ui->realuy->setText(QString::number(robposinfo.uy));
    ui->realvz->setText(QString::number(robposinfo.vz));

    ui->p_realposX->setText(QString::number(p_robposinfo.x,'f',3));
    ui->p_realposY->setText(QString::number(p_robposinfo.y,'f',3));
    ui->p_realposZ->setText(QString::number(p_robposinfo.z,'f',3));
    ui->p_realposRX->setText(QString::number(p_robposinfo.rx,'f',4));
    ui->p_realposRY->setText(QString::number(p_robposinfo.ry,'f',4));
    ui->p_realposRZ->setText(QString::number(p_robposinfo.rz,'f',4));
    ui->p_realout1->setText(QString::number(p_robposinfo.out1));
    ui->p_realout2->setText(QString::number(p_robposinfo.out2));
    ui->p_realout3->setText(QString::number(p_robposinfo.out3));
    ui->p_realtool->setText(QString::number(p_robposinfo.tool));
    ui->p_realtcp->setText(QString::number(p_robposinfo.tcp));
    ui->p_realusertcp->setText(QString::number(p_robposinfo.usertcp));
    ui->p_realuy->setText(QString::number(p_robposinfo.uy));
    ui->p_realvz->setText(QString::number(p_robposinfo.vz));
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
                int real_readnum_2=modbus_read_registers(_p->ctx_pshow,ALS_STATE_REG_ADD,0x3,_p->result_data);
                int real_readnum_3=modbus_read_registers(_p->ctx_pshow,ALS_P_REALTIME_POSX_REG_ADD,0x15,_p->p_pos_data);
                if(real_readnum_1==0x15&&real_readnum_2==0x03&&real_readnum_3==0x15)
                {
                    _p->getrobinfo();
                }
                if(_p->b_init_show_pshow_text_finish==true)
                {
                    _p->b_init_show_pshow_text_finish=false;
                    emit Send_show_pshow_text();
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
