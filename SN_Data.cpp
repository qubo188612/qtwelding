#include "SN_Data.h"

SN_Data::SN_Data()
{
    b_read_uncreat=false;
    read();
    read_SN2();

    GetPasswordUser();
    GetPasswordAdmin();
    GetPasswordSn();
}

SN_Data::~SN_Data()
{
    save();
}

void SN_Data::GenerateSerial()
{
    QString cur=macIPinfo.getAllAdapterInfo();//查询网卡信息

    if(cur.isEmpty())
        return;

    std::vector<uchar> tempmacStr;

    macIPinfo.split_mac(cur,&tempmacStr);

    Int32 Rand1=tempmacStr[0];
    Int32 Rand2=tempmacStr[1];
    Int32 Rand3=tempmacStr[2];
    Int32 Rand4=tempmacStr[3];
    Int32 Rand5=tempmacStr[4]+tempmacStr[5];

    Uint16 SerialFirstTime = (Uint16)(((Rand1+Rand2)*Rand3) & 0x00009fff);
    Uint16 SerialSecondTime = (Uint16)((Rand4 * Rand5) & 0x00007fff);

    Uint32 nTemp1, nTemp2, nTemp3, i;
    //时间各自平方
    nTemp1 = (Uint32)SerialFirstTime;
    nTemp2 = (Uint32)SerialSecondTime;

    nTemp2 = nTemp2 * nTemp2;
    nTemp1 = nTemp1 * nTemp1;
    //second的16进制末四位变成前四个，若有0则变为B
    i = 0;
    while (i < 4)
    {
        nTemp3 = nTemp2 & 0x0000000F;
        nTemp2 = (nTemp2 >> 4);
        if (nTemp3 == 0)
            SerialNo[i] = 'B';
        else if ((nTemp3 >= 1) && (nTemp3 <= 9))
            SerialNo[i] = 0x30 + nTemp3;
        else if ((nTemp3 >= 10) && (nTemp3 <= 15))
            SerialNo[i] = 'A' + nTemp3 - 10;
        i ++;
    }
    SerialNo[4] = 'X';
    //first的16进制末四位变成后四个，若有0则变为5
    i = 5;
    while (i < 9)
    {
        nTemp3 = nTemp1 & 0x0000000F;
        nTemp1 = (nTemp1 >> 4);
        if (nTemp3 == 0)
            SerialNo[i] = 'B';
        else if ((nTemp3 >= 1) && (nTemp3 <= 9))
            SerialNo[i] = 0x30 + nTemp3;
        else if ((nTemp3 >= 10) && (nTemp3 <= 15))
            SerialNo[i] = 'A' + nTemp3 - 10;
        i ++;
    }
    SerialNo[9]='\0';
}

void SN_Data::checkcanshu()
{

}

void SN_Data::save()
{
    srand((unsigned)time(NULL));

    Uint16 u16_rand_1=rand();
    Uint16 u16_rand_2=rand();
    Uint16 u16_rand_3=rand();
    Uint16 u16_rand_4=rand();
    Int32 i32_TempnLeftHours=nLeftHours+u16_rand_1+u16_rand_2+u16_rand_3+u16_rand_4;

    checkcanshu();
    Uint8 *buff=new Uint8[SN_SAVEBUFF];
    if(buff==NULL)
        return;

    Uint8 *u8_p;
    Uint16 *u16_p;
    Int32 *i32_p;

    u8_p = (Uint8*)buff;
    for(int i=0;i<10;i++)
    {
        *u8_p = SerialNo[i];
        u8_p ++;
    }
    u16_p = (Uint16*)(&buff[11]);
    *u16_p = SerialFlag;
    u16_p ++;
    u8_p = (Uint8*)(&buff[13]);
    *u8_p=PasswordUserFlag;
    u8_p++;
    i32_p = (Int32*)(&buff[14]);
    *i32_p=customPasswordUser;
    i32_p++;

    *i32_p = i32_TempnLeftHours;
    i32_p++;

    u16_p = (Uint16*)i32_p;
    *u16_p = u16_rand_1;
    u16_p++;

    *u16_p = u16_rand_2;
    u16_p++;

    *u16_p = u16_rand_3;
    u16_p++;

    *u16_p = u16_rand_4;
    u16_p++;

    CFileOut fo;
    fo.WriteFile(SN_SYSPATH_MOTO,buff,SN_SAVEBUFF);

    if(buff!=NULL)
        delete []buff;
}

Int8 SN_Data::read()
{
    Uint16 u16_rand_1;
    Uint16 u16_rand_2;
    Uint16 u16_rand_3;
    Uint16 u16_rand_4;
    Int32 i32_TempnLeftHours;

    Uint8 *buff=new Uint8[SN_SAVEBUFF];
    if(buff==NULL)
    {
        b_read_uncreat=true;
        return 1;
    }
    CFileOut fo;
    if(0 > fo.ReadFile(SN_SYSPATH_MOTO,buff,SN_SAVEBUFF))
    {
        GenerateSerial();
        SerialFlag=0x55AA;
        PasswordUserFlag=0;
        customPasswordUser=0;
        i32_TempnLeftHours=0;
        u16_rand_1=0;
        u16_rand_2=0;
        u16_rand_3=0;
        u16_rand_4=0;
        nLeftHours=0;
        save();
        if(0 > fo.ReadFile(SN_SYSPATH_MOTO2,buff,SN2_SAVEBUFF))
        {
            QFile::remove(SN_SYSPATH_MOTO2);
        }
        if(buff!=NULL)
            delete []buff;
        return 1;
    }
    Uint8 *u8_p;
    Uint16 *u16_p;
    Int32 *i32_p;
    u8_p = (Uint8*)buff;
    for(int i=0;i<10;i++)
    {
        SerialNo[i]=*u8_p;
        u8_p ++;
    }
    u16_p = (Uint16*)(&buff[11]);
    SerialFlag=*u16_p;
    u16_p ++;
    u8_p = (Uint8*)(&buff[13]);
    PasswordUserFlag=*u8_p;
    u8_p++;
    i32_p = (Int32*)(&buff[14]);
    customPasswordUser=*i32_p;
    i32_p++;

    i32_TempnLeftHours=*i32_p;
    i32_p++;

    u16_p = (Uint16*)i32_p;
    u16_rand_1 = *u16_p;
    u16_p++;

    u16_rand_2 = *u16_p;
    u16_p++;

    u16_rand_3 = *u16_p;
    u16_p++;

    u16_rand_4 = *u16_p;
    u16_p++;

    nLeftHours = i32_TempnLeftHours-u16_rand_1-u16_rand_2-u16_rand_3-u16_rand_4;

    if(SerialFlag!=0x55AA)
    {
        if(buff!=NULL)
            delete []buff;
        GenerateSerial();
        SerialFlag=0x55AA;
        PasswordUserFlag=0;
        customPasswordUser=0;
        i32_TempnLeftHours=0;
        u16_rand_1=0;
        u16_rand_2=0;
        u16_rand_3=0;
        u16_rand_4=0;
        nLeftHours=0;
        save();
        if(0 > fo.ReadFile(SN_SYSPATH_MOTO2,buff,SN2_SAVEBUFF))
        {
            QFile::remove(SN_SYSPATH_MOTO2);
        }
        return 1;
    }
    if(buff!=NULL)
        delete []buff;
    return 0;
}

void SN_Data::save_SN2()
{
    Uint8 *buff=new Uint8[SN2_SAVEBUFF];
    if(buff==NULL)
        return;

    Int32 *i32_p;
    Uint8 *u8_p;
    Uint16 *u16_p;

    i32_p = (Int32*)buff;

    *i32_p = nCurrentSerialCount;
    i32_p++;
    u8_p = (Uint8*)i32_p;

    for(int i=0;i<nCurrentSerialCount;i++)
    {
        memcpy(u8_p,groupSavedSerialNo[i].SerialNo,SN2_SINGID_LONG);
        u8_p = u8_p+SN2_SINGID_LONG;
    }

    CFileOut fo;
    fo.WriteFile(SN_SYSPATH_MOTO2,buff,SN2_SAVEBUFF);

    if(buff!=NULL)
        delete []buff;
    return;
}

Int8 SN_Data::read_SN2()
{
    Uint8 *buff=new Uint8[SN2_SAVEBUFF];
    if(buff==NULL)
        return 1;

    if(buff==NULL)
    {
        b_read_uncreat=true;
        return 1;
    }
    CFileOut fo;
    if(0 > fo.ReadFile(SN_SYSPATH_MOTO2,buff,SN2_SAVEBUFF))
    {
        nCurrentSerialCount=0;
        groupSavedSerialNo.clear();
        save_SN2();
        if(buff!=NULL)
            delete []buff;
        return 1;
    }

    Int32 *i32_p;
    Uint8 *u8_p;
    Uint16 *u16_p;

    i32_p = (Int32*)buff;

    nCurrentSerialCount = *i32_p;
    i32_p++;
    u8_p = (Uint8*)i32_p;

    groupSavedSerialNo.clear();
    for(int i=0;i<nCurrentSerialCount;i++)
    {
        SavedSerialNo sing;
        memcpy(sing.SerialNo,u8_p,SN2_SINGID_LONG);
        groupSavedSerialNo.push_back(sing);
        u8_p = u8_p+SN2_SINGID_LONG;
    }
    if(buff!=NULL)
        delete []buff;
    return 0;
}

void SN_Data::SavePasswordUser()
{
    PasswordUserFlag=0x5A;
    PasswordUser = customPasswordUser;
    save();
}

void SN_Data::GetPasswordUser()
{
    Uint32 Num[6];
    Uint8 i;

    if(SerialNo[4] != 'X'|| SerialNo[9] != '\0')//读取长度
    {
        return;//错误
    }
    else
    {
        if(PasswordUserFlag!=0x5A)
        {
            for(i = 0;i < 4;i ++)//变成数字
            {
                if(SerialNo[i] >= '0' && SerialNo[i] <= '9')
                {
                    Num[i] = SerialNo[i] - '0' + 1;
                }
                else
                {
                    Num[i] = SerialNo[i] - 'A' + 2;
                }
                if(SerialNo[i + 5] >= '0' && SerialNo[i + 5] <= '9')
                {
                    Num[i] += SerialNo[i + 5] - '0' + 3;
                }
                else
                {
                    Num[i] += SerialNo[i + 5] - 'A' + 4;
                }
            }

            Num[4] = Num[0] + Num[1] + Num[2] + 5;
            Num[5] = Num[2] + Num[3] + Num[4] + 6;

            if(Num[0] % 10 == 0)
            {
                Num[0] = 1;
            }

            PasswordUser = Num[0]%10*100000+Num[1]%10*10000+Num[2]%10*1000+Num[3]%10*100+Num[4]%10*10+Num[5]%10;
        }
        else
        {
            PasswordUser = customPasswordUser;
        }
    }
}

void SN_Data::GetPasswordAdmin()
{
    Uint32 Num[6];
    Uint8 i;

    if(SerialNo[4] != 'X' || SerialNo[9] != '\0')//读取长度
    {
        return;//错误
    }
    else
    {
        for(i = 0;i < 4;i ++)//变成数字
        {
            if(SerialNo[i] >= '0' && SerialNo[i] <= '9')
            {
                Num[i] = SerialNo[i] - '0' + 3;
            }
            else
            {
                Num[i] = SerialNo[i] - 'A' + 4;
            }
            if(SerialNo[i + 5] >= '0' && SerialNo[i + 5] <= '9')
            {
                Num[i] += SerialNo[i + 5] - '0' + 5;
            }
            else
            {
                Num[i] += SerialNo[i + 5] - 'A' + 6;
            }
        }

        Num[4] = Num[0] + Num[1] + Num[2] + Num[3] + Num[3] + 7;
        Num[5] = Num[0] + Num[1] + Num[2] + Num[3] + Num[4] + 8;

        if(Num[0] % 10 == 0)
        {
            Num[0] = 2;
        }

        PasswordAdmin = Num[0]%10*100000+Num[1]%10*10000+Num[2]%10*1000+Num[3]%10*100+Num[4]%10*10+Num[5]%10;
    }
    return;
}

void SN_Data::GetPasswordSn()
{
    Uint32 Num[10];
    Uint8 i;

    if(SerialNo[4] != 'X' || SerialNo[9] != '\0')//读取长度
    {
        return;//错误
    }
    else
    {
        for(i = 0;i < 4;i ++)//变成数字
        {
            if(SerialNo[i] >= '0' && SerialNo[i] <= '9')
            {
                Num[i] = SerialNo[i] - '0' + 6;
            }
            else
            {
                Num[i] = SerialNo[i] - 'A' + 7;
            }
            if(SerialNo[i + 5] >= '0' && SerialNo[i + 5] <= '9')
            {
                Num[i + 5] = SerialNo[i + 5] - '0' + 8;
            }
            else
            {
                Num[i + 5] = SerialNo[i + 5] - 'A' + 9;
            }
        }

        Num[4] = Num[0] + Num[2] + Num[6] + Num[7] + Num[8] + 4;
        Num[9] = Num[1] + Num[3] + Num[5] + Num[6] + Num[8] + 9;

        Num[0] = Num[0] + Num[2] + Num[4] + Num[7] + Num[8] + 1;
        Num[1] = Num[1] + Num[3] + Num[5] + Num[6] + Num[8] + 2;
        Num[2] = Num[1] + Num[4] + Num[6] + Num[5] + Num[8] + 3;
        Num[3] = Num[0] + Num[1] + Num[7] + Num[4] + Num[8] + 4;
        Num[4] = Num[0] + Num[5] + Num[2] + Num[3] + Num[8] + 5;
        Num[5] = Num[4] + Num[1] + Num[2] + Num[3] + Num[8] + 6;
        Num[6] = Num[5] + Num[0] + Num[2] + Num[2] + Num[8] + 7;
        Num[7] = Num[3] + Num[2] + Num[5] + Num[1] + Num[8] + 8;

        if(Num[0] % 10 == 0)
        {
            Num[0] = 3;
        }

        PasswordSn = Num[0]%10*10000000+Num[1]%10*1000000+Num[2]%10*100000
                    +Num[3]%10*10000+Num[4]%10*1000+Num[5]%10*100+Num[6]%10*10+Num[7]%10*1;
    }
    return;
}

bool SN_Data::checkSN()
{
    if(b_read_uncreat==true)//无SN信息
        return 1;
    bool have=0;
    QString cur=macIPinfo.getAllAdapterInfo();//查询网卡信息

    if(cur.isEmpty())
        return 1;

    std::vector<uchar> tempmacStr;

    macIPinfo.split_mac(cur,&tempmacStr);

    Int32 Rand1=tempmacStr[0];
    Int32 Rand2=tempmacStr[1];
    Int32 Rand3=tempmacStr[2];
    Int32 Rand4=tempmacStr[3];
    Int32 Rand5=tempmacStr[4]+tempmacStr[5];

    Uint8 check_SerialNo[10];			//序列号

    Uint16 SerialFirstTime = (Uint16)(((Rand1+Rand2)*Rand3) & 0x00009fff);
    Uint16 SerialSecondTime = (Uint16)((Rand4 * Rand5) & 0x00007fff);

    Uint32 nTemp1, nTemp2, nTemp3, i;
    //时间各自平方
    nTemp1 = (Uint32)SerialFirstTime;
    nTemp2 = (Uint32)SerialSecondTime;

    nTemp2 = nTemp2 * nTemp2;
    nTemp1 = nTemp1 * nTemp1;
    //second的16进制末四位变成前四个，若有0则变为B
    i = 0;
    while (i < 4)
    {
        nTemp3 = nTemp2 & 0x0000000F;
        nTemp2 = (nTemp2 >> 4);
        if (nTemp3 == 0)
            check_SerialNo[i] = 'B';
        else if ((nTemp3 >= 1) && (nTemp3 <= 9))
            check_SerialNo[i] = 0x30 + nTemp3;
        else if ((nTemp3 >= 10) && (nTemp3 <= 15))
            check_SerialNo[i] = 'A' + nTemp3 - 10;
        i ++;
    }
    check_SerialNo[4] = 'X';
    //first的16进制末四位变成后四个，若有0则变为5
    i = 5;
    while (i < 9)
    {
        nTemp3 = nTemp1 & 0x0000000F;
        nTemp1 = (nTemp1 >> 4);
        if (nTemp3 == 0)
            check_SerialNo[i] = 'B';
        else if ((nTemp3 >= 1) && (nTemp3 <= 9))
            check_SerialNo[i] = 0x30 + nTemp3;
        else if ((nTemp3 >= 10) && (nTemp3 <= 15))
            check_SerialNo[i] = 'A' + nTemp3 - 10;
        i ++;
    }
    check_SerialNo[9]='\0';

    if(0!=strcmp((char*)check_SerialNo,(char*)SerialNo))
        return 1;
    return 0;
}

Uint8 SN_Data::CheckVerify(Uint8 *strSource, Uint8 nSourceLen,  Uint8 *strFirst, Int16 nFirstLen,
    Uint8 *strMiddle, Int16 nMiddleLen, Uint8 *strSecond, Int16 nSecondLen)
{
    volatile	Uint8 nTemp1, nTemp2, nTemp3;
    nTemp1 = strFirst[nFirstLen - 2] - 0x30;
    nTemp2 = strFirst[nFirstLen - 1] - 0x30;
    nTemp3 = strSource[nSourceLen - 3] - 0x30;
    if ((nTemp1 + nTemp2) % 10 != nTemp3)
        return FALSE;

    nTemp1 = strMiddle[nMiddleLen - 2] - 0x30;
    nTemp2 = strMiddle[nMiddleLen - 1] - 0x30;
    nTemp3 = strSource[nSourceLen - 2] - 0x30;
    if ((nTemp1 + nTemp2) % 10 != nTemp3)
        return FALSE;

    nTemp1 = strSecond[nSecondLen - 2] - 0x30;
    nTemp2 = strSecond[nSecondLen - 1] - 0x30;
    nTemp3 = strSource[nSourceLen - 1] - 0x30;
    if ((nTemp1 + nTemp2) % 10 != nTemp3)
        return FALSE;

    return TRUE;
}

void SN_Data::Myltoa(long NumToConvert, Uint8 strResult[], Uint8 base2)
{
    Uint8 strTemp[30], i = 0, j;
    long nTemp, nTemp1;
    nTemp = NumToConvert;
    while (nTemp / base2)
    {
        nTemp1 = nTemp % base2;
        if ((nTemp1 >=0) && (nTemp1 <= 9))
            strTemp[i ++] = nTemp1 + 0x30;
        else
            strTemp[i ++] = nTemp1 - 10 + 'A';

        nTemp = nTemp / base2;

    }
    nTemp1 = nTemp % base2;
    if ((nTemp1 >=0) && (nTemp <= 9))
        strTemp[i ++] = nTemp + 0x30;
    else
        strTemp[i ++] = nTemp - 10 + 'A';

    strTemp[i] = 0;
    j = 0;
    while(strTemp[j] != 0)
    {
        strResult[j++] = strTemp[i - 1 - j];
    }
    strResult[j] = 0;
}

void SN_Data::TransNum(Uint8 num[], Uint8 base1, Uint8 base2, Uint8 szResult[])
{
    int		i,k,l,m,j;
    long	inum=0;
    Uint8	temp[20];
    double	r=0;

    i = (Int32)(strlen((char*)num));											//	数值某ざ?
    for (j=0; j<i; j++)
    {
    //	r = pow(base1,i-j-1);
        r = 1;
        for (k = 0; k < i - j - 1; k ++)
        {
            r = r * base1;
        }
//		r = base1^(i-j-1);									//	计算数基的幂指数
        if (base1<=10) l = '9' - (10 - base1);					//	计算有效的数范围
        else
        {
            m = 'a' + (base1 - 11);
            l = '9';
        }

        if ((num[j]>=48) && (num[j]<=l))						// 求每位数字的十进制值
            k = num[j]-48;
        else if (base1>10)
        {														// 求每个字母所代表的十进制值
            if ((num[j]>='A') && (num[j]<=m - 32))
                k = num[j] - 'A'+10;
            else if ((num[j]>='a') && (num[j]<=m))
                k = num[j] - 'a'+10;
        }
        inum += k * (Int32) r;									//	累加计算结果
    }

    Myltoa(inum,temp,base2);
    strcpy((char*)szResult, (char*)temp);
//	szResult.Format("%s(%d) = %s(%d)\n",num,base1,temp,base2);	//	输出转换结果

}

Uint32 SN_Data::JudgeSN(Uint8 *strRegisterNo,Uint8 nRegisterNoLen,QString *err_msg)
{   
    Uint16 nLeftTime;

    volatile Int16 nIndicator1, i, nIndicator2, nLen, j, nLen1;
    Uint8 strTemp[30], FirstP[15], MiddleP[20], SecondP[15], First4[20], Second4[20], strProductID[10],strSavedSerialNo[64];
    Int32 seed1, seed2, seed3, tmpNum, nDays;

    if ((nRegisterNoLen < 20) || (nRegisterNoLen > 30))
    {
        *err_msg=QString::fromLocal8Bit("序列号长度不正确");
        return 1;//密码长度不正确
    }

    for (i = 0; i < nRegisterNoLen; i ++)
    {
        strSN[i] = strRegisterNo[i] + 100;
    }
    //1.检查该串密码是否被使用过,用一个字节表示当前有多少字节空间,然后一个个读出来比较
    if(nCurrentSerialCount > 1000)//序列号个数超过阈值
    {
        *err_msg=QString::fromLocal8Bit("序列号个数超过阈值");
        return 7;
    }

    if (nCurrentSerialCount > 0)
    {
        i = 0;
        while (i < nCurrentSerialCount)
        {
            memcpy(strSavedSerialNo,groupSavedSerialNo[i].SerialNo,nRegisterNoLen);
            for (j = 0; j < nRegisterNoLen - 3; j ++)
            {
                if (strSavedSerialNo[j] != strSN[j])
                    break;
            }
            if (j == (nRegisterNoLen - 3))
            {
                *err_msg=QString::fromLocal8Bit("序列号重复");
                return 6;//序列号重复
            }
            i++;
        }
    }

    //2.1检查有效性
    for (i = 0; i < nRegisterNoLen; i ++)
    {
        strTemp[i] = strRegisterNo[i];
    }

    nIndicator1 = -1;
    for (i = 0; i < nRegisterNoLen; i ++)
    {
        if (strTemp[i] == '.')
        {
            nIndicator1 = i;
            break;
        }
    }
    if (nIndicator1 <= 0)
    {
        *err_msg=QString::fromLocal8Bit("没有第一个'.' 分隔符");
        return 2;//没有第一个'.' 分隔符
    }

    for (i = 0; i < nIndicator1; i ++)
    {
        FirstP[i] = strTemp[i];
    }
    FirstP[i] = 0;//加入字符串终止符

    seed1 = atoi((char*)FirstP);

    //2.2消除前面一段
    nIndicator2 = -1;
    for (i = nIndicator1 + 1; i < nRegisterNoLen - 3; i ++)
    {
        if (strTemp[i] == '.')
        {
            nIndicator2 = i;
            break;
        }
    }

    if (nIndicator2 <= 0)
    {
        *err_msg=QString::fromLocal8Bit("没有第二个'.' 分隔符");
        return 3;//没有第二个'.'分隔符
    }

    for (i = nIndicator1 + 1; i < nIndicator2; i ++)
    {
        MiddleP[i - nIndicator1 - 1] = strTemp[i];
    }
    MiddleP[i - nIndicator1 - 1] = 0;

    tmpNum = atoi((char*)MiddleP) / 10;

    for(i = nIndicator2 + 1; i < nRegisterNoLen - 3; i ++)
    {
        SecondP[i - nIndicator2 - 1] = strTemp[i];
    }
    SecondP[i - nIndicator2 - 1] = 0;
    seed2 = atoi((char*)SecondP);

    seed3 = abs(seed1 - seed2);

    nDays = tmpNum - seed3;

    if (CheckVerify(strTemp, nRegisterNoLen, FirstP, nIndicator1, MiddleP, nIndicator2 - nIndicator1 - 1,
        SecondP, nRegisterNoLen - 3 - nIndicator2 - 1) == FALSE)
    {
        *err_msg=QString::fromLocal8Bit("解码不正确");
        return 5;
    }

    if ((nDays > 10000) || (nDays < 1))
    {
        *err_msg=QString::fromLocal8Bit("解码后剩余时间不正确");
        return 4;//解码后剩余时间不正确
    }

    //3.核对序列号
    TransNum(FirstP, 10, 19, First4);
    TransNum(SecondP, 10, 17, Second4);

    //First4取后4位
    nLen = strlen((char*)First4);
    j = 0;
    for (i = nLen - 4; i < nLen; i ++)
    {
        strProductID[i - nLen + 4] =  First4[i];
        j ++;
    }
    strProductID[j++] = 'X';
    //Second4取后4位
    nLen = strlen((char*)Second4);
    for (i = nLen - 4; i < nLen; i ++)
    {
        strProductID[j++] =  Second4[i];
    }
    strProductID[j] = 0;

    //4.比较序列号
    nLen = strlen((char*)strProductID);

    for (i = 0; i < nLen; i ++)
    {
        if (strProductID[i] != SerialNo[i])
            break;
    }
    if (i == nLen)
    {
        //保存当前序列号
        nCurrentSerialCount ++;

        SavedSerialNo sing;
        memcpy(sing.SerialNo,strSN,SN2_SINGID_LONG);
        groupSavedSerialNo.push_back(sing);

        save_SN2();

        nLeftHours = nDays/8*24*60;//单位（分钟）

        save();
        return 0;//返回成功
    }
    else
    {
        *err_msg=QString::fromLocal8Bit("序列号核对失败");
        return 5;// 序列号核对失败
    }
}
