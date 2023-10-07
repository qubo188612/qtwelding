#include "E2proomData.h"

E2proomData::E2proomData()
{
#if _MSC_VER||WINDOWS_TCP
    QString filePath =  "./SAVE";
    QDir dir(filePath);
    if(!dir.exists())
        QDir().mkdir(filePath);
#else
    std::string dir = "./SAVE";
    if (access(dir.c_str(), 0) == -1)
    {
      mkdir("./SAVE",S_IRWXU);
    }
#endif

    matrix_camera2plane=cv::Mat::zeros(3,3,CV_64F);
    matrix_plane2robot=cv::Mat::zeros(3,3,CV_64F);

    camdlg_modposX1_min=E2POOM_CAMDLG_MODPOSX1_MIN;
    camdlg_modposX1_max=E2POOM_CAMDLG_MODPOSX1_MAX;
    camdlg_modposX1_use=E2POOM_CAMDLG_MODPOSX1_USE;
    camdlg_modposY1_min=E2POOM_CAMDLG_MODPOSY1_MIN;
    camdlg_modposY1_max=E2POOM_CAMDLG_MODPOSY1_MAX;
    camdlg_modposY1_use=E2POOM_CAMDLG_MODPOSY1_USE;
    camdlg_modposX2_min=E2POOM_CAMDLG_MODPOSX2_MIN;
    camdlg_modposX2_max=E2POOM_CAMDLG_MODPOSX2_MAX;
    camdlg_modposX2_use=E2POOM_CAMDLG_MODPOSX2_USE;
    camdlg_modposY2_min=E2POOM_CAMDLG_MODPOSY2_MIN;
    camdlg_modposY2_max=E2POOM_CAMDLG_MODPOSY2_MAX;
    camdlg_modposY2_use=E2POOM_CAMDLG_MODPOSY2_USE;
    camdlg_modposX3_min=E2POOM_CAMDLG_MODPOSX3_MIN;
    camdlg_modposX3_max=E2POOM_CAMDLG_MODPOSX3_MAX;
    camdlg_modposX3_use=E2POOM_CAMDLG_MODPOSX3_USE;
    camdlg_modposY3_min=E2POOM_CAMDLG_MODPOSY3_MIN;
    camdlg_modposY3_max=E2POOM_CAMDLG_MODPOSY3_MAX;
    camdlg_modposY3_use=E2POOM_CAMDLG_MODPOSY3_USE;
    camdlg_modposX4_min=E2POOM_CAMDLG_MODPOSX4_MIN;
    camdlg_modposX4_max=E2POOM_CAMDLG_MODPOSX4_MAX;
    camdlg_modposX4_use=E2POOM_CAMDLG_MODPOSX4_USE;
    camdlg_modposY4_min=E2POOM_CAMDLG_MODPOSY4_MIN;
    camdlg_modposY4_max=E2POOM_CAMDLG_MODPOSY4_MAX;
    camdlg_modposY4_use=E2POOM_CAMDLG_MODPOSY4_USE;

    camdlg_cvimg_posX1_min=E2POOM_CAMDLG_CVIMG_POSX1_MIN;
    camdlg_cvimg_posX1_max=E2POOM_CAMDLG_CVIMG_POSX1_MAX;
    camdlg_cvimg_posX1_use=E2POOM_CAMDLG_CVIMG_POSX1_USE;
    camdlg_cvimg_posY1_min=E2POOM_CAMDLG_CVIMG_POSY1_MIN;
    camdlg_cvimg_posY1_max=E2POOM_CAMDLG_CVIMG_POSY1_MAX;
    camdlg_cvimg_posY1_use=E2POOM_CAMDLG_CVIMG_POSY1_USE;
    camdlg_cvimg_posX2_min=E2POOM_CAMDLG_CVIMG_POSX2_MIN;
    camdlg_cvimg_posX2_max=E2POOM_CAMDLG_CVIMG_POSX2_MAX;
    camdlg_cvimg_posX2_use=E2POOM_CAMDLG_CVIMG_POSX2_USE;
    camdlg_cvimg_posY2_min=E2POOM_CAMDLG_CVIMG_POSY2_MIN;
    camdlg_cvimg_posY2_max=E2POOM_CAMDLG_CVIMG_POSY2_MAX;
    camdlg_cvimg_posY2_use=E2POOM_CAMDLG_CVIMG_POSY2_USE;
    camdlg_cvimg_posX3_min=E2POOM_CAMDLG_CVIMG_POSX3_MIN;
    camdlg_cvimg_posX3_max=E2POOM_CAMDLG_CVIMG_POSX3_MAX;
    camdlg_cvimg_posX3_use=E2POOM_CAMDLG_CVIMG_POSX3_USE;
    camdlg_cvimg_posY3_min=E2POOM_CAMDLG_CVIMG_POSY3_MIN;
    camdlg_cvimg_posY3_max=E2POOM_CAMDLG_CVIMG_POSY3_MAX;
    camdlg_cvimg_posY3_use=E2POOM_CAMDLG_CVIMG_POSY3_USE;
    camdlg_cvimg_posX4_min=E2POOM_CAMDLG_CVIMG_POSX4_MIN;
    camdlg_cvimg_posX4_max=E2POOM_CAMDLG_CVIMG_POSX4_MAX;
    camdlg_cvimg_posX4_use=E2POOM_CAMDLG_CVIMG_POSX4_USE;
    camdlg_cvimg_posY4_min=E2POOM_CAMDLG_CVIMG_POSY4_MIN;
    camdlg_cvimg_posY4_max=E2POOM_CAMDLG_CVIMG_POSY4_MAX;
    camdlg_cvimg_posY4_use=E2POOM_CAMDLG_CVIMG_POSY4_USE;

    demdlg_radio_mod_min=E2POOM_DEMDLG_RADIO_MOD_MIN;
    demdlg_radio_mod_max=E2POOM_DEMDLG_RADIO_MOD_MAX;
    demdlg_radio_mod_use=E2POOM_DEMDLG_RADIO_MOD_USE;

    maindlg_SaveDatacheckBox_min=E2POOM_MAINDLG_SAVEDATA_MIN;
    maindlg_SaveDatacheckBox_max=E2POOM_MAINDLG_SAVEDATA_MAX;
    maindlg_SaveDatacheckBox_use=E2POOM_MAINDLG_SAVEDATA_USE;
    maindlg_Weldeled_min=E2POOM_MAINDLG_WELDELED_MIN;
    maindlg_Weldeled_use=E2POOM_MAINDLG_WELDELED_USE;
    maindlg_Weldelev_min=E2POOM_MAINDLG_WELDELEV_MIN;
    maindlg_Weldelev_use=E2POOM_MAINDLG_WELDELEV_USE;
    maindlg_Weldelem_min=E2POOM_MAINDLG_WELDELEM_MIN;
    maindlg_Weldelem_max=E2POOM_MAINDLG_WELDELEM_MAX;
    maindlg_Weldelem_use=E2POOM_MAINDLG_WELDELEM_USE;
    maindlg_movespeed_min=E2POOM_MAINDLG_MOVESPEED_MIN;
    maindlg_movespeed_max=E2POOM_MAINDLG_MOVESPEED_MAX;
    maindlg_movespeed_use=E2POOM_MAINDLG_MOVESPEED_USE;
    maindlg_movetcp_min=E2POOM_MAINDLG_MOVETCP_MIN;
    maindlg_movetcp_max=E2POOM_MAINDLG_MOVETCP_MAX;
    maindlg_movetcp_use=E2POOM_MAINDLG_MOVETCP_USE;
    maindlg_circlerun_min=E2POOM_MAINDLG_CIRCLERUN_MIN;
    maindlg_circlerun_max=E2POOM_MAINDLG_CIRCLERUN_MAX;
    maindlg_circlerun_use=E2POOM_MAINDLG_CIRCLERUN_USE;

    read_para();
}

E2proomData::~E2proomData()
{

}

void E2proomData::check_para()
{
    if(camdlg_modposX1<camdlg_modposX1_min||camdlg_modposX1>camdlg_modposX1_max)
        camdlg_modposX1=camdlg_modposX1_use;
    if(camdlg_modposY1<camdlg_modposY1_min||camdlg_modposY1>camdlg_modposY1_max)
        camdlg_modposY1=camdlg_modposY1_use;
    if(camdlg_modposX2<camdlg_modposX2_min||camdlg_modposX2>camdlg_modposX2_max)
        camdlg_modposX2=camdlg_modposX2_use;
    if(camdlg_modposY2<camdlg_modposY2_min||camdlg_modposY2>camdlg_modposY2_max)
        camdlg_modposY2=camdlg_modposY2_use;
    if(camdlg_modposX3<camdlg_modposX3_min||camdlg_modposX3>camdlg_modposX3_max)
        camdlg_modposX3=camdlg_modposX3_use;
    if(camdlg_modposY3<camdlg_modposY3_min||camdlg_modposY3>camdlg_modposY3_max)
        camdlg_modposY3=camdlg_modposY3_use;
    if(camdlg_modposX4<camdlg_modposX4_min||camdlg_modposX4>camdlg_modposX4_max)
        camdlg_modposX4=camdlg_modposX4_use;
    if(camdlg_modposY4<camdlg_modposY4_min||camdlg_modposY4>camdlg_modposY4_max)
        camdlg_modposY4=camdlg_modposY4_use;
    if(camdlg_cvimg_posX1<camdlg_cvimg_posX1_min||camdlg_cvimg_posX1>camdlg_cvimg_posX1_max)
        camdlg_cvimg_posX1=camdlg_cvimg_posX1_use;
    if(camdlg_cvimg_posY1<camdlg_cvimg_posY1_min||camdlg_cvimg_posY1>camdlg_cvimg_posY1_max)
        camdlg_cvimg_posY1=camdlg_cvimg_posY1_use;
    if(camdlg_cvimg_posX2<camdlg_cvimg_posX2_min||camdlg_cvimg_posX2>camdlg_cvimg_posX2_max)
        camdlg_cvimg_posX2=camdlg_cvimg_posX2_use;
    if(camdlg_cvimg_posY2<camdlg_cvimg_posY2_min||camdlg_cvimg_posY2>camdlg_cvimg_posY2_max)
        camdlg_cvimg_posY2=camdlg_cvimg_posY2_use;
    if(camdlg_cvimg_posX3<camdlg_cvimg_posX3_min||camdlg_cvimg_posX3>camdlg_cvimg_posX3_max)
        camdlg_cvimg_posX3=camdlg_cvimg_posX3_use;
    if(camdlg_cvimg_posY3<camdlg_cvimg_posY3_min||camdlg_cvimg_posY3>camdlg_cvimg_posY3_max)
        camdlg_cvimg_posY3=camdlg_cvimg_posY3_use;
    if(camdlg_cvimg_posX4<camdlg_cvimg_posX4_min||camdlg_cvimg_posX4>camdlg_cvimg_posX4_max)
        camdlg_cvimg_posX4=camdlg_cvimg_posX4_use;
    if(camdlg_cvimg_posY4<camdlg_cvimg_posY4_min||camdlg_cvimg_posY4>camdlg_cvimg_posY4_max)
        camdlg_cvimg_posY4=camdlg_cvimg_posY4_use;

    if(demdlg_radio_mod<demdlg_radio_mod_min||demdlg_radio_mod>demdlg_radio_mod_max)
        demdlg_radio_mod=(Eye_Hand_calibrationmode)demdlg_radio_mod_use;

    if(maindlg_SaveDatacheckBox<maindlg_SaveDatacheckBox_min||maindlg_SaveDatacheckBox>maindlg_SaveDatacheckBox_max)
        maindlg_SaveDatacheckBox=maindlg_SaveDatacheckBox_use;
    if(maindlg_Weldeled<maindlg_Weldeled_min)
        maindlg_Weldeled=maindlg_Weldeled_use;
    if(maindlg_Weldelev<maindlg_Weldelev_min)
        maindlg_Weldelev=maindlg_Weldelev_use;
    if(maindlg_Weldelem<maindlg_Weldelem_min||maindlg_Weldelem>maindlg_Weldelem_max)
        maindlg_Weldelem=(Alternatingcurrent_ID)maindlg_Weldelem_use;
    if(maindlg_movespeed<maindlg_movespeed_min||maindlg_movespeed>maindlg_movespeed_max)
        maindlg_movespeed=maindlg_movespeed_use;
    if(maindlg_movetcp<maindlg_movetcp_min||maindlg_movetcp>maindlg_movetcp_max)
        maindlg_movetcp=maindlg_movetcp_use;
    if(maindlg_circlerun<maindlg_circlerun_min||maindlg_circlerun>maindlg_circlerun_max)
        maindlg_circlerun=maindlg_circlerun_use;
}

void E2proomData::read_para()
{
    read_camdlg_para();
    read_demdlg_para();
    read_maindlg_para();

    check_para();
}

void E2proomData::read_camdlg_para()
{
    Uint8 *buff=NULL;
    CFileOut fo;

    buff=new Uint8[E2POOM_CAMDLG_SAVEBUFF];
    if(buff==NULL)
        return;
    if(0 > fo.ReadFile((char*)E2POOM_CAMDLG_SYSPATH_MOTO,buff,E2POOM_CAMDLG_SAVEBUFF))
    {
        init_camdlg_para();
        if(buff!=NULL)
        {
          delete []buff;
          buff=NULL;
        }
    }
    else
    {
      Int32 *i32_p;

      i32_p = (Int32*)buff;
      camdlg_modposX1=*i32_p;
      i32_p++;
      camdlg_modposY1=*i32_p;
      i32_p++;
      camdlg_modposX2=*i32_p;
      i32_p++;
      camdlg_modposY2=*i32_p;
      i32_p++;
      camdlg_modposX3=*i32_p;
      i32_p++;
      camdlg_modposY3=*i32_p;
      i32_p++;
      camdlg_modposX4=*i32_p;
      i32_p++;
      camdlg_modposY4=*i32_p;
      i32_p++;
      camdlg_cvimg_posX1=*i32_p;
      i32_p++;
      camdlg_cvimg_posY1=*i32_p;
      i32_p++;
      camdlg_cvimg_posX2=*i32_p;
      i32_p++;
      camdlg_cvimg_posY2=*i32_p;
      i32_p++;
      camdlg_cvimg_posX3=*i32_p;
      i32_p++;
      camdlg_cvimg_posY3=*i32_p;
      i32_p++;
      camdlg_cvimg_posX4=*i32_p;
      i32_p++;
      camdlg_cvimg_posY4=*i32_p;
      i32_p++;
    }
    if(buff!=NULL)
    {
      delete []buff;
      buff=NULL;
    }
}

void E2proomData::write_camdlg_para()
{
    Uint8 *buff=NULL;
    CFileOut fo;

    check_para();
    buff=new Uint8[E2POOM_CAMDLG_SAVEBUFF];
    if(buff==NULL)
      return;

    Int32 *i32_p;

    i32_p = (Int32*)buff;
    *i32_p=camdlg_modposX1;
    i32_p++;
    *i32_p=camdlg_modposY1;
    i32_p++;
    *i32_p=camdlg_modposX2;
    i32_p++;
    *i32_p=camdlg_modposY2;
    i32_p++;
    *i32_p=camdlg_modposX3;
    i32_p++;
    *i32_p=camdlg_modposY3;
    i32_p++;
    *i32_p=camdlg_modposX4;
    i32_p++;
    *i32_p=camdlg_modposY4;
    i32_p++;
    *i32_p=camdlg_cvimg_posX1;
    i32_p++;
    *i32_p=camdlg_cvimg_posY1;
    i32_p++;
    *i32_p=camdlg_cvimg_posX2;
    i32_p++;
    *i32_p=camdlg_cvimg_posY2;
    i32_p++;
    *i32_p=camdlg_cvimg_posX3;
    i32_p++;
    *i32_p=camdlg_cvimg_posY3;
    i32_p++;
    *i32_p=camdlg_cvimg_posX4;
    i32_p++;
    *i32_p=camdlg_cvimg_posY4;
    i32_p++;

    fo.WriteFile((char*)E2POOM_CAMDLG_SYSPATH_MOTO,buff,E2POOM_CAMDLG_SAVEBUFF);

    if(buff!=NULL)
    {
      delete []buff;
      buff=NULL;
    }
}

void E2proomData::init_camdlg_para()
{
    camdlg_modposX1=camdlg_modposX1_use;
    camdlg_modposY1=camdlg_modposY1_use;
    camdlg_modposX2=camdlg_modposX2_use;
    camdlg_modposY2=camdlg_modposY2_use;
    camdlg_modposX3=camdlg_modposX3_use;
    camdlg_modposY3=camdlg_modposY3_use;
    camdlg_modposX4=camdlg_modposX4_use;
    camdlg_modposY4=camdlg_modposY4_use;
    camdlg_cvimg_posX1=camdlg_cvimg_posX1_use;
    camdlg_cvimg_posY1=camdlg_cvimg_posY1_use;
    camdlg_cvimg_posX2=camdlg_cvimg_posX2_use;
    camdlg_cvimg_posY2=camdlg_cvimg_posY2_use;
    camdlg_cvimg_posX3=camdlg_cvimg_posX3_use;
    camdlg_cvimg_posY3=camdlg_cvimg_posY3_use;
    camdlg_cvimg_posX4=camdlg_cvimg_posX4_use;
    camdlg_cvimg_posY4=camdlg_cvimg_posY4_use;
}

void E2proomData::read_demdlg_para()
{
    QJsonDocument jsonDoc;
    if(0!=Loadjsonfile(E2POOM_DEMDLG_SYSPATH_MOTO,jsonDoc))
    {
        return;
    }
    QJsonObject rootObj = jsonDoc.object();
    QJsonObject::Iterator it;
    for(it=rootObj.begin();it!=rootObj.end();it++)//遍历Key
    {
        QString keyString=it.key();
        if(keyString=="demdlg_radio_mod")
        {
            demdlg_radio_mod=(Eye_Hand_calibrationmode)it.value().toInt();
        }
        else if(keyString=="demdlg_R")
        {
            QJsonArray arrData=it.value().toArray();
            if(arrData.size()!=9)
            {
                continue;
            }
            int n=0;
            for(int j=0;j<3;j++)
            {
                for(int i=0;i<3;i++)
                {
                    demdlg_R(j,i)=arrData[n++].toDouble();
                }
            }
        }
        else if(keyString=="demdlg_T")
        {
            QJsonArray arrData=it.value().toArray();
            if(arrData.size()!=3)
            {
                continue;
            }
            for(int i=0;i<3;i++)
            {
                demdlg_T(i)=arrData[i].toDouble();
            }
        }
        else if(keyString=="demdlg_Robotpos")
        {
            QJsonObject rootObj = it.value().toObject();
            QJsonObject::Iterator it;
            int size=rootObj.size();
            demdlg_Robotpos.resize(size);
            for(it=rootObj.begin();it!=rootObj.end();it++)//遍历Key
            {
                Int32 num =it.key().toInt();
                QJsonObject rootObj = it.value().toObject();
                QJsonObject::Iterator it;
                for(it=rootObj.begin();it!=rootObj.end();it++)//遍历Key
                {
                    QString keyString=it.key();
                    if(keyString=="X")
                    {
                        demdlg_Robotpos[num].X=it.value().toDouble();
                    }
                    else if(keyString=="Y")
                    {
                        demdlg_Robotpos[num].Y=it.value().toDouble();
                    }
                    else if(keyString=="Z")
                    {
                        demdlg_Robotpos[num].Z=it.value().toDouble();
                    }
                    else if(keyString=="RX")
                    {
                        demdlg_Robotpos[num].RX=it.value().toDouble();
                    }
                    else if(keyString=="RY")
                    {
                        demdlg_Robotpos[num].RY=it.value().toDouble();
                    }
                    else if(keyString=="RZ")
                    {
                        demdlg_Robotpos[num].RZ=it.value().toDouble();
                    }
                }
            }
        }
        else if(keyString=="demdlg_Leaserpos")
        {
            QJsonObject rootObj = it.value().toObject();
            QJsonObject::Iterator it;
            int size=rootObj.size();
            demdlg_Leaserpos.resize(size);
            for(it=rootObj.begin();it!=rootObj.end();it++)//遍历Key
            {
                Int32 num =it.key().toInt();
                QJsonObject rootObj = it.value().toObject();
                QJsonObject::Iterator it;
                for(it=rootObj.begin();it!=rootObj.end();it++)//遍历Key
                {
                    QString keyString=it.key();
                    if(keyString=="camY")
                    {
                        demdlg_Leaserpos[num].leaserpos.Y=it.value().toDouble();
                    }
                    else if(keyString=="camZ")
                    {
                        demdlg_Leaserpos[num].leaserpos.Z=it.value().toDouble();
                    }
                    else if(keyString=="robX")
                    {
                        demdlg_Leaserpos[num].robotpos.X=it.value().toDouble();
                    }
                    else if(keyString=="robY")
                    {
                        demdlg_Leaserpos[num].robotpos.Y=it.value().toDouble();
                    }
                    else if(keyString=="robZ")
                    {
                        demdlg_Leaserpos[num].robotpos.Z=it.value().toDouble();
                    }
                    else if(keyString=="robRX")
                    {
                        demdlg_Leaserpos[num].robotpos.RX=it.value().toDouble();
                    }
                    else if(keyString=="robRY")
                    {
                        demdlg_Leaserpos[num].robotpos.RY=it.value().toDouble();
                    }
                    else if(keyString=="robRZ")
                    {
                        demdlg_Leaserpos[num].robotpos.RZ=it.value().toDouble();
                    }
                }
            }
        }
        else if(keyString=="matrix_camera2plane")
        {
            QJsonArray arrData=it.value().toArray();
            if(arrData.size()!=9)
            {
                continue;
            }
            int n=0;
            for(int j=0;j<3;j++)
            {
                double *d= matrix_camera2plane.ptr<double>(j);
                for(int i=0;i<3;i++)
                {
                    d[i]=arrData[n++].toDouble();
                }
            }
        }
        else if(keyString=="matrix_plane2robot")
        {
            QJsonArray arrData=it.value().toArray();
            if(arrData.size()!=9)
            {
                continue;
            }
            int n=0;
            for(int j=0;j<3;j++)
            {
                double *d= matrix_plane2robot.ptr<double>(j);
                for(int i=0;i<3;i++)
                {
                    d[i]=arrData[n++].toDouble();
                }
            }
        }
    }
}

void E2proomData::write_demdlg_para()
{
    QVariantHash data;
    data.insert("demdlg_radio_mod", demdlg_radio_mod);

    QJsonArray arrData1;
    for (int j=0;j<3;j++)
        for(int i=0;i<3;i++)
            arrData1.append(demdlg_R(j,i));
    data.insert("demdlg_R",arrData1);

    QJsonArray arrData2;
    for (int i=0;i<3;i++)
        arrData2.append(demdlg_T(i));
    data.insert("demdlg_T",arrData2);

    QVariantHash arrData3;
    for (int i=0;i<demdlg_Robotpos.size();i++)
    {
        QVariantHash singpos;
        singpos.insert("X",demdlg_Robotpos[i].X);
        singpos.insert("Y",demdlg_Robotpos[i].Y);
        singpos.insert("Z",demdlg_Robotpos[i].Z);
        singpos.insert("RX",demdlg_Robotpos[i].RX);
        singpos.insert("RY",demdlg_Robotpos[i].RY);
        singpos.insert("RZ",demdlg_Robotpos[i].RZ);
        arrData3.insert(QString::number(i),singpos);
    }
    data.insert("demdlg_Robotpos",arrData3);

    QVariantHash arrData4;
    for (int i=0;i<demdlg_Leaserpos.size();i++)
    {
        QVariantHash singpos;
        singpos.insert("camY",demdlg_Leaserpos[i].leaserpos.Y);
        singpos.insert("camZ",demdlg_Leaserpos[i].leaserpos.Z);
        singpos.insert("robX",demdlg_Leaserpos[i].robotpos.X);
        singpos.insert("robY",demdlg_Leaserpos[i].robotpos.Y);
        singpos.insert("robZ",demdlg_Leaserpos[i].robotpos.Z);
        singpos.insert("robRX",demdlg_Leaserpos[i].robotpos.RX);
        singpos.insert("robRY",demdlg_Leaserpos[i].robotpos.RY);
        singpos.insert("robRZ",demdlg_Leaserpos[i].robotpos.RZ);
        arrData4.insert(QString::number(i),singpos);
    }
    data.insert("demdlg_Leaserpos",arrData4);

    QJsonArray arrData6;
    for (int j=0;j<3;j++)
    {
        double *d= matrix_camera2plane.ptr<double>(j);
        for (int i=0;i<3;i++)
        {
            arrData6.append(d[i]);
        }
    }
    data.insert("matrix_camera2plane",arrData6);

    QJsonArray arrData7;
    for (int j=0;j<3;j++)
    {
        double *d= matrix_plane2robot.ptr<double>(j);
        for (int i=0;i<3;i++)
        {
            arrData7.append(d[i]);
        }
    }
    data.insert("matrix_plane2robot",arrData7);

    Savejsonfile(E2POOM_DEMDLG_SYSPATH_MOTO,data);
}

void E2proomData::init_demdlg_para()
{
    demdlg_radio_mod=(Eye_Hand_calibrationmode)demdlg_radio_mod_use;
}

void E2proomData::read_maindlg_para()
{
    Uint8 *buff=NULL;
    CFileOut fo;

    buff=new Uint8[E2POOM_MAINDLG_SAVEBUFF];
    if(buff==NULL)
        return;
    if(0 > fo.ReadFile((char*)E2POOM_MAINDLG_SYSPATH_MOTO,buff,E2POOM_MAINDLG_SAVEBUFF))
    {
        init_maindlg_para();
        if(buff!=NULL)
        {
          delete []buff;
          buff=NULL;
        }
    }
    else
    {
      Int32 *i32_p;
      float *f32_p;

      i32_p = (Int32*)buff;
      maindlg_SaveDatacheckBox=*i32_p;
      i32_p++;
      f32_p = (float*)i32_p;
      maindlg_Weldeled=*f32_p;
      f32_p++;
      maindlg_Weldelev=*f32_p;
      f32_p++;
      i32_p = (Int32*)f32_p;
      maindlg_Weldelem=(Alternatingcurrent_ID)*i32_p;
      i32_p++;
      f32_p = (float*)i32_p;
      maindlg_movespeed=*f32_p;
      f32_p++;
      i32_p = (Int32*)f32_p;
      maindlg_movetcp=*i32_p;
      i32_p++;
      maindlg_circlerun=*i32_p;
      i32_p++;
    }
    if(buff!=NULL)
    {
      delete []buff;
      buff=NULL;
    }
}

void E2proomData::write_maindlg_para()
{
    Uint8 *buff=NULL;
    CFileOut fo;

    check_para();
    buff=new Uint8[E2POOM_MAINDLG_SAVEBUFF];
    if(buff==NULL)
      return;

    Int32 *i32_p;
    float *f32_p;

    i32_p = (Int32*)buff;
    *i32_p=maindlg_SaveDatacheckBox;
    i32_p++;
    f32_p = (float*)i32_p;
    *f32_p=maindlg_Weldeled;
    f32_p++;
    *f32_p=maindlg_Weldelev;
    f32_p++;
    i32_p = (Int32*)f32_p;
    *i32_p=maindlg_Weldelem;
    i32_p++;
    f32_p = (float*)i32_p;
    *f32_p=maindlg_movespeed;
    f32_p++;
    i32_p = (Int32*)f32_p;
    *i32_p=maindlg_movetcp;
    i32_p++;
    *i32_p=maindlg_circlerun;
    i32_p++;

    fo.WriteFile((char*)E2POOM_MAINDLG_SYSPATH_MOTO,buff,E2POOM_MAINDLG_SAVEBUFF);

    if(buff!=NULL)
    {
      delete []buff;
      buff=NULL;
    }
}

void E2proomData::init_maindlg_para()
{
    maindlg_SaveDatacheckBox=maindlg_SaveDatacheckBox_use;
    maindlg_Weldeled=maindlg_Weldeled_use;
    maindlg_Weldelev=maindlg_Weldelev_use;
    maindlg_Weldelem=(Alternatingcurrent_ID)maindlg_Weldelem_use;
    maindlg_movespeed=maindlg_movespeed_use;
    maindlg_movetcp=maindlg_movetcp_use;
    maindlg_circlerun=maindlg_circlerun_use;
}


void E2proomData::write()
{
    write_camdlg_para();
    write_demdlg_para();
    write_maindlg_para();
}

int E2proomData::Loadjsonfile(char* filename,QJsonDocument &jsonDoc)
{
    QFile loadFile(filename);

    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Unable to load JSON file";
        return 1;
    }

    QByteArray allData = loadFile.readAll();
    loadFile.close();

    QJsonParseError json_error;
    jsonDoc=QJsonDocument::fromJson(allData, &json_error);

    if (json_error.error != QJsonParseError::NoError)
    {
        qDebug() << "JSON error!";
        return 1;
    }

    return 0;
}

int E2proomData::Savejsonfile(char* filename,QVariantHash data)
{
    QJsonObject rootObj = QJsonObject::fromVariantHash(data);
    QJsonDocument document;
    document.setObject(rootObj);

    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    QString json_str(byte_array);
    //根据实际填写路径
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "file error!";
        return 1;
    }
    QTextStream in(&file);
    in << json_str;

    file.close();   // 关闭file
    return 0;
}

void E2proomData::write_sshdlg_para()
{
    QVariantHash data=sshdlg_enjson();

    QJsonObject rootObj = QJsonObject::fromVariantHash(data);
    QJsonDocument document;
    document.setObject(rootObj);

    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    QString json_str(byte_array);
    //根据实际填写路径
    QFile file(E2POOM_SSHDLG_SYSPATH_MOTO);

    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "file error!";
        return;
    }
    QTextStream in(&file);
    in << json_str;

    file.close();   // 关闭file

    return;
}

void E2proomData::read_sshdlg_para()
{
    QFile loadFile(E2POOM_SSHDLG_SYSPATH_MOTO);

    if (!loadFile.open(QIODevice::ReadOnly))
    {
        init_sshdlg_para();
        return;
    }

    QByteArray allData = loadFile.readAll();
    loadFile.close();

    if(0!=sshdlg_dejson(allData))
    {
        init_sshdlg_para();
        return;
    }
}

void E2proomData::init_sshdlg_para()
{
    sshdlg_usename="pi";
    sshdlg_password="123456";
}

QVariantHash E2proomData::sshdlg_enjson()
{
    QVariantHash data;
    data.insert("sshdlg_usename", sshdlg_usename);
    data.insert("sshdlg_password", sshdlg_password);

    return data;
}

int E2proomData::sshdlg_dejson(QByteArray allData)
{
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

    if (json_error.error != QJsonParseError::NoError)
    {
        qDebug() << "JSON error!";
        return 1;
    }

    QJsonObject rootObj = jsonDoc.object();
    QJsonObject::Iterator it;
    for(it=rootObj.begin();it!=rootObj.end();it++)//遍历Key
    {
        QString keyString=it.key();
        if(keyString=="sshdlg_usename")//相机
        {
            sshdlg_usename=it.value().toString();
        }
        if(keyString=="sshdlg_password")//相机
        {
            sshdlg_password=it.value().toString();
        }
    }

    return 0;
}

