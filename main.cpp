#include <QApplication>
#include <QTextCodec>
#include "qtweldingdlg.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#if _MSC_VER
    QTextCodec *codec = QTextCodec::codecForName("GBK");
#else
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
#endif
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif
    qtweldingDlg main_window;
    main_window.setWindowTitle(QString::fromLocal8Bit("自动焊缝跟踪软件"));
    main_window.show();
#if _MSC_VER||WINDOWS_TCP
#else
    main_window.m_mcs->cam->sop_cam[0].argc=argc;
    main_window.m_mcs->cam->sop_cam[0].argv=argv;
#endif
    return a.exec();
}
