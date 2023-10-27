#include "MyQrFunction.h"

#include <QDebug>



Myqr::Myqr()
{

}

Myqr::~Myqr()
{

}

int Myqr::Qr_to_QrInfo(QImage fileImage,QrInfo &msg)
{
    if (fileImage.isNull()) {
        return 1;
    }
    auto hints = ZXingQt::DecodeHints()
                     .setFormats(ZXing::BarcodeFormat::Any)
                     .setTryRotate(false)
                     .setMaxNumberOfSymbols(10);

    auto results = ZXingQt::ReadBarcodes(fileImage, hints);

    for (auto& result : results) {
        msg.Text=My_u8string_to_Chinese(result.text().toStdString());
        msg.Format=result.format();
        msg.Content=result.contentType();
    }

    return results.isEmpty() ? 1 : 0;
}


int Myqr::QrInfo_to_Qr(QString msg,QString format,QImage &fileImage)
{
    auto myformat = ZXing::BarcodeFormatFromString(format.toStdString());

    fileImage = WriteBarcode(msg, myformat);

    return 0;
}


QImage Myqr::WriteBarcode(QString text, ZXing::BarcodeFormat format)
{
    int width=271,height=271;
    auto writer = ZXing::MultiFormatWriter(format);
    std::string str=My_Chinese_to_u8string(text);
    auto matrix = writer.encode(str, width, height);
    auto bitmap = ZXing::ToMatrix<uint8_t>(matrix);
    return QImage(bitmap.data(), bitmap.width(), bitmap.height(), bitmap.width(), QImage::Format::Format_Grayscale8).copy();
}

std::string Myqr::My_Chinese_to_u8string(QString text)
{
    QByteArray byte = text.toLocal8Bit();
    std::string str;
    for(int n=0;n<byte.size();n++)
    {
        char buffer[3];
        uint8_t u8=byte[n];
        sprintf(buffer,"%02x",u8);
        str=str+buffer;
    }
    return str;
}

QString Myqr::My_u8string_to_Chinese(std::string str)
{
    QByteArray byte;
    for(int n=0;n<str.size();n=n+2)
    {
        char buffer[3];
        buffer[0]=str[n];
        buffer[1]=str[n+1];
        buffer[2]='\0';
        int number;
        sscanf(buffer, "%x", &number);
        char u=(char)number;
        byte.push_back(u);
    }
    QString msg=byte;
    return msg;
}
