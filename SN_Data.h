#ifndef SN_DATA_H
#define SN_DATA_H

#include "tistdtypes.h"
#include <time.h>
#include <QFile>
#include "FileOut.h"
#include "MyCheckIPinfo.h"

#define SN_SAVEBUFF		32
#define SN_SYSPATH_MOTO	"./SAVE/SN.bsd"

#define SN2_SINGID_LONG		32
#define SN2_SAVEBUFF	SN2_SINGID_LONG*1000+16		//可存放1000个序列号,每个32字节
#define SN_SYSPATH_MOTO2 "./SAVE/SN2.bsd"

#define SN_PASSWORD_LONG	8	//用户密码长度

#define SN_TIME_SN          999*24*60  //永久使用时间

class SavedSerialNo
{
public:
    char SerialNo[SN2_SINGID_LONG];
};

class SN_Data
{
public:
    SN_Data();
    ~SN_Data();
    Int32 nLeftHours;			//剩余时间

    Uint8 SerialNo[10];			//序列号
    Int32 PasswordUser;			//用户密码
    Int32 PasswordAdmin;		//管理员密码
    Int32 PasswordSn;			//序列号密码

    Int32 SerialCount;			//当前序列号数

    void GenerateSerial();		//生成序列号
    Int32 customPasswordUser;	//自定义用户密码
    void SavePasswordUser();	//保存用户密码

    void save();				//保存
    Int8 read();				//读取
    Int8 read_SN2();			//读取ID
    void save_SN2();			//保存ID

    bool checkSN();				//核对SN

    Int32 nCurrentSerialCount;		//目前有几个序列号
    std::vector<SavedSerialNo> groupSavedSerialNo;

    Uint32 JudgeSN(Uint8 *strRegisterNo,Uint8 nRegisterNoLen,QString *err_msg);		//序列号解码(序列号,序列号长度)

    char macStr[20];//12+5+1	//Mac地址

    Uint8 strSN[SN2_SINGID_LONG];

    MyCheckIPinfo macIPinfo;			//当前电脑网卡IP信息

    void GetPasswordUser();		//生成用户密码
    void GetPasswordAdmin();	//生成管理员密码
    void GetPasswordSn();		//生成序列号密码

private:
    bool b_read_uncreat;		//无SN文件

    void checkcanshu();			//检查参数
    Uint16 SerialFlag;			//未生成SN文件标记位
    Uint8 PasswordUserFlag;		//存在自定义用户密码标记位

    Uint8 CheckVerify(Uint8 *strSource, Uint8 nSourceLen,  Uint8 *strFirst, Int16 nFirstLen,
    Uint8 *strMiddle, Int16 nMiddleLen, Uint8 *strSecond, Int16 nSecondLen);
    void TransNum(Uint8 num[], Uint8 base1, Uint8 base2, Uint8 szResult[]);
    void Myltoa(long NumToConvert, Uint8 strResult[], Uint8 base2);
};

#endif // SN_DATA_H
