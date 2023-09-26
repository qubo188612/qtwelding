#include <QApplication>
#include "qtweldingdlg.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qtweldingDlg main_window;
    main_window.setWindowTitle(QStringLiteral("自动焊缝跟踪软件"));
    main_window.show();
#if _MSC_VER||WINDOWS_TCP
#else
    main_window.m_mcs->cam->sop_cam[0].argc=argc;
    main_window.m_mcs->cam->sop_cam[0].argv=argv;
#endif
    return a.exec();
}
