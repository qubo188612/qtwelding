#ifndef XTCP_H
#define XTCP_H
#include <string>
#include <iostream>
#include "string.h"
#include <stdio.h>

#if _MSC_VER
#include <Windows.h>
#define socklen_t int
#pragma comment(lib, "Ws2_32.lib")
#else
#include <arpa/inet.h>
#define closesocket close    //宏定义替换函数
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#define strcpy_s strcpy
#endif

class XTcp
{
public:
    int CreateSocket();				 //创建套接字
    bool Bind(unsigned short port);  //绑定端口号
    bool Listen(unsigned short num); //监听端口号
    bool SetBlock(bool isblock);  //设置阻塞模式  （希望只有在connect的时候是非阻塞的，而接收数据时候是阻塞的）
    bool Connect(const char *ip = "192.168.1.2", unsigned short port = 8000, int sec = 3);
    bool Connect(int sec = 3);
    XTcp Accept();                   //返回XTcp对象，接收连接
    void Close();							//关闭连接
    int Recv(char *buf, int size);          //接收数据
    int Send(const char *buf, int size);	//发送数据
    int SetRecvTimeout(int sec = 1);			 //设置udp接收超时
    int SetSendTimeout(int sec = 1);		     //设置udp发送超时
    bool SetRcvBufferlong(int bufferlong);  //设置接收缓冲长度
    bool SetSentBufferlong(int bufferlong); //设置发送缓冲长度


    char clientip[16];						//存放接收到的client ip
    unsigned short clientport = 0;			//存放接收到的client port

    XTcp(unsigned short port = 8000);
    XTcp(char *ip,unsigned short port);
    virtual ~XTcp();


private:
    char tcp_serverip[16] = "";   //tcp_serverip 服务端ip
    int tsock = 0;							//tcp客户端的socket，create自动生成
    unsigned short tport = 0;				//接收main函数中的参数  要传入Connect
};
#endif // XTCP_H
