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
        msg.Text=result.text();
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
//    text=QStringLiteral("我就不信了");
    int width=271,height=271;
    auto writer = ZXing::MultiFormatWriter(format);
    std::wstring str = text.toStdWString();
    auto matrix = writer.encode(str, width, height);
    auto bitmap = ZXing::ToMatrix<uint8_t>(matrix);
    return QImage(bitmap.data(), bitmap.width(), bitmap.height(), bitmap.width(), QImage::Format::Format_Grayscale8).copy();
}
