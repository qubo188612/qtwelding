#include "sshpassworddlg.h"
#include "ui_sshpassworddlg.h"

#ifdef DEBUS_SSH

sshpasswordDlg::sshpasswordDlg(my_parameters *mcs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sshpasswordDlg)
{
    ui->setupUi(this);
    m_mcs=mcs;

    connect(ui->sshlinkBtn,&QPushButton::clicked,[=](){
        m_mcs->e2proomdata.sshdlg_usename=ui->sshusename->text();
        m_mcs->e2proomdata.sshdlg_password=ui->sshpassword->text();
        m_mcs->e2proomdata.write_sshdlg_para();
        SshConnect();
    });

    connect(ui->sshfileBtn,&QPushButton::clicked,[=](){
        m_mcs->resultdata.setup_file = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("请选择要导入的升级文件"), "", "TAR(*.tar)");
        if(m_mcs->resultdata.setup_file.size()>0)
        {
            ui->record->append(QString::fromLocal8Bit("选择升级文件:"));
            ui->record->append(m_mcs->resultdata.setup_file);
        }
    });

    connect(ui->sshsetupBtn,&QPushButton::clicked,[=](){
        if(m_mcs->resultdata.setup_file.size()>0)
        {
            QString st="/home/"+m_mcs->e2proomdata.sshdlg_usename+"/sunny-tis.tar";
        //  QString strCmd = "sudo chmod -R 777 /dev";
        //  strCmd += "\n"; //添加回车
        //  emit sigSend(strCmd);
        //  QString st="/dev/sunny-tis.tar";
            ui->record->append(QString::fromLocal8Bit("开始传输升级文件..."));
            emit sigSendFile(m_mcs->resultdata.setup_file,st);
        }
        else
        {
            ui->record->append(QString::fromLocal8Bit("请先选择升级文件"));
        }
    });

    connect(ui->sshcmdBtn,&QPushButton::clicked,[=](){
//      if(m_mcs->resultdata.m_bFileState==true)
//      {
            if(m_mcs->resultdata.m_bConnectState)
            {
                m_mcs->resultdata.updata_step=0;
                QString strCmd = "sudo docker stop ros";
                strCmd += "\n"; //添加回车
                emit sigSend(strCmd);
            }
//      }
//      else
//      {
//          ui->record->append(QString::fromLocal8Bit("请先传输升级文件"));
//      }
    });
}

sshpasswordDlg::~sshpasswordDlg()
{
    delete ui;
}

void sshpasswordDlg::init_dlg_show()
{
    ui->record->clear();
    m_mcs->resultdata.setup_file="";
    m_mcs->resultdata.m_bConnectState=false;
    m_mcs->resultdata.m_bFileState=false;
    ui->sshusename->setText(m_mcs->e2proomdata.sshdlg_usename);
    ui->sshpassword->setText(m_mcs->e2proomdata.sshdlg_password);
}

void sshpasswordDlg::close_dlg_show()
{
    if(!m_mcs->resultdata.m_bConnectState)
    {
        m_mcs->resultdata.m_bConnectState = false;
        emit sigDisconnected();//断开连接
    }
}


void sshpasswordDlg::SshConnect()
{
    if(!m_mcs->resultdata.m_bConnectState)
    {
        ui->record->append(QString::fromLocal8Bit("ssh服务创建中..."));
        createSSHConnection();  //发起连接
    }
    else
    {
        ui->record->append(QString::fromLocal8Bit("ssh服务重启中..."));
        m_mcs->resultdata.m_bConnectState = false;
        emit sigDisconnected();//断开连接
        createSSHConnection();
    }
}

void sshpasswordDlg::createSSHConnection()
{
    QString strIp = m_mcs->ip->camer_ip[0].ip;
    QString strUser = m_mcs->e2proomdata.sshdlg_usename;
    QString strPwd = m_mcs->e2proomdata.sshdlg_password;

    m_mcs->resultdata.ctx_ssh = new CConnectionForSshClient(strIp,22,strPwd,strUser);
    m_mcs->resultdata.ctx_ssh->init();
    connect(m_mcs->resultdata.ctx_ssh,SIGNAL(sigConnectStateChanged(bool,QString,int)),
            this,SLOT(slotConnectStateChanged(bool,QString,int)));
    connect(m_mcs->resultdata.ctx_ssh,SIGNAL(sigDataArrived(QString ,QString , int )),
            this,SLOT(slotDataArrived(QString ,QString , int )));
    connect(this,SIGNAL(sigSend(QString)),m_mcs->resultdata.ctx_ssh,SLOT(slotSend(QString)));
    connect(this,SIGNAL(sigDisconnected()),m_mcs->resultdata.ctx_ssh,SLOT(slotDisconnected()));
    connect(this,SIGNAL(sigSendFile(QString,QString)),m_mcs->resultdata.ctx_ssh,SLOT(slotSendFile(QString,QString)));
    connect(m_mcs->resultdata.ctx_ssh,SIGNAL(sigsshFileScpfinish(bool)),this,SLOT(slotsshFileScpfinish(bool)));
}

void sshpasswordDlg::slotConnectStateChanged(bool bState, QString strIp, int nPort)
{
    Q_UNUSED(strIp)
    Q_UNUSED(nPort)

    m_mcs->resultdata.m_bConnectState = bState;
    if(m_mcs->resultdata.m_bConnectState)
    {
        ui->record->append(QString::fromLocal8Bit("ssh服务连接成功,请等待相机响应..."));
    }
    else
    {
        ui->record->append(QString::fromLocal8Bit("ssh服务连接失败"));
    }
}

void sshpasswordDlg::slotsshFileScpfinish(bool bState)
{
    if(bState==true)
    {
        ui->record->append(QString::fromLocal8Bit("升级文件传输完成,请开始升级"));
        m_mcs->resultdata.m_bFileState=true;
    }
    else
    {
        ui->record->append(QString::fromLocal8Bit("升级文件传输失败，请重试"));
    }
}

void sshpasswordDlg::slotDataArrived(QString strMsg, QString strIp, int nPort)
{
    Q_UNUSED(strIp)
    Q_UNUSED(nPort)

    ui->record->append(strMsg);

    if(strMsg.contains("password for"))
    {
        QString strCmd = m_mcs->e2proomdata.sshdlg_password;
        strCmd += "\n"; //添加回车
        emit sigSend(strCmd);
    }
    else if(m_mcs->resultdata.updata_step==0)
    {
        QStringList list = strMsg.split("\r\n");
        if(list.size()>0)
        {
            if(list[0]=="ros")
            {
                m_mcs->resultdata.updata_step=1;
                QString strCmd = "sudo docker ps -a";
                strCmd += "\n"; //添加回车
                emit sigSend(strCmd);
            }
        }
    }
    else if(m_mcs->resultdata.updata_step==1)
    {
        QStringList list = strMsg.split("NAMES\r\n");
        if(list.size()>1)
        {
            QStringList ID = list[1].split(" ");
            if(ID.size()>1)
            {
                m_mcs->resultdata.dockerid=ID[0];
                m_mcs->resultdata.updata_step=2;
                QString strCmd = "sudo docker rm "+ m_mcs->resultdata.dockerid;
                strCmd += "\n"; //添加回车
                emit sigSend(strCmd);
            }
        }
    }
    else if(m_mcs->resultdata.updata_step==2)
    {
        QStringList list = strMsg.split("\r\n");
        if(list.size()>0)
        {
            if(list[0]==m_mcs->resultdata.dockerid)
            {
                m_mcs->resultdata.updata_step=3;
                QString strCmd = "sudo docker rmi qubo188612/sunny-tis:deploy";
                strCmd += "\n"; //添加回车
                emit sigSend(strCmd);
            }
        }
    }
    else if(m_mcs->resultdata.updata_step==3)
    {
        if(strMsg.contains("Deleted:"))
        {
            m_mcs->resultdata.updata_step=4;
        }
    }
    else if(m_mcs->resultdata.updata_step==4)
    {
        if(strMsg.contains(m_mcs->e2proomdata.sshdlg_usename+"@"+m_mcs->e2proomdata.sshdlg_usename+":~$"))
        {
            m_mcs->resultdata.updata_step=5;
        //  QString strCmd = "sudo docker load < /dev/sunny-tis.tar";
            QString strCmd = "sudo docker load < sunny-tis.tar";
            strCmd += "\n"; //添加回车
            emit sigSend(strCmd);
        }
    }
    else if(m_mcs->resultdata.updata_step==5)
    {
        if(strMsg.contains("qubo188612/sunny-tis:deploy"))
        {
            m_mcs->resultdata.updata_step=6;
            QString strCmd = "sudo docker-compose up -d";
            strCmd += "\n"; //添加回车
            emit sigSend(strCmd);
        }
    }
    else if(m_mcs->resultdata.updata_step==6)
    {
        if(strMsg.contains("done"))
        {
            m_mcs->resultdata.updata_step=7;
            ui->record->append(QString::fromLocal8Bit("相机程序升级完成"));
        }
    }
}

#endif
