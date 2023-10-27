#ifndef MYQRFUNCTION_H
#define MYQRFUNCTION_H

#include "QrCode.h"
#include <QString>
#include <BarcodeFormat.h>
#include <BitMatrix.h>
#include <MultiFormatWriter.h>

class QrInfo
{
public:
    QString Text;       //内容
    ZXing::BarcodeFormat Format;     //格式
    ZXing::ContentType Content;
};

class Myqr
{
public:
    Myqr();
    ~Myqr();

    static int Qr_to_QrInfo(QImage fileImage,QrInfo &msg);

    static int QrInfo_to_Qr(QString msg,QString format,QImage &fileImage);

private:
    static QImage WriteBarcode(QString text, ZXing::BarcodeFormat format);
};

#endif
