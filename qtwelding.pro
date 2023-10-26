CONFIG += c++17
CONFIG -= app_bundle

CONFIG += console


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT       += core gui network xml openglwidgets


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        CConnectionForSshClient.cpp \
        E2proomData.cpp \
        FileOut.cpp \
        MyCheckIPinfo.cpp \
        MyPlcFunction.cpp \
        MyQrFunction.cpp \
        MytracerealtimeFunction.cpp \
        PictureBox.cpp \
        ResultData.cpp \
        SN_Data.cpp \
        TimeFunction.cpp \
        XTcp.cpp \
        arrvepointtestdlg.cpp \
        calibration.cpp \
        cam_sen.cpp \
        cambuilddlg.cpp \
        ccoordchange.cpp \
        checkscandatadlg.cpp \
        craft.cpp \
        cweldtarject.cpp \
        demarcatedlg.cpp \
        editcraftdlg.cpp \
        editprojectdlg.cpp \
        edittextdlg.cpp \
        fitlineside.cpp \
        global.cpp \
        infodlg.cpp \
        ipaddress.cpp \
        keyaoutdlg.cpp \
        keycamdlg.cpp \
        keycoorddlg.cpp \
        keycreatadddlg.cpp \
        keycreataddpdlg.cpp \
        keycreatcdlg.cpp \
        keycreatdlg.cpp \
        keycreatfdlg.cpp \
        keycreatpdlg.cpp \
        keydelaydlg.cpp \
        keyfilterdlg.cpp \
        keygetposdlg.cpp \
        keygettcppos2dlg.cpp \
        keygettcpposdlg.cpp \
        keygotodlg.cpp \
        keygowelddlg.cpp \
        keyiodlg.cpp \
        keymovdlg.cpp \
        keyplcdlg.cpp \
        keyplotposdlg.cpp \
        keysampledlg.cpp \
        keysavepcddlg.cpp \
        keyscandlg.cpp \
        keysearchdlg.cpp \
        keysearchenddlg.cpp \
        keysetposedlg.cpp \
        keysmovdlg.cpp \
        keysscandlg.cpp \
        keytrace2dlg.cpp \
        keytraceadddlg.cpp \
        keytracecontinuedlg.cpp \
        keytracedlg.cpp \
        keytracerealtime2dlg.cpp \
        keytracerealtimedlg.cpp \
        keytracingdlg.cpp \
        keywavedlg.cpp \
        keywelddlg.cpp \
        main.cpp \
        my_cmd.cpp \
        my_parameters.cpp \
        newcraftdlg.cpp \
        newprojectdlg.cpp \
        othercmddlg.cpp \
        plcsetdlg.cpp \
        plotedit0dlg.cpp \
        plotedit1dlg.cpp \
        plotedit2dlg.cpp \
        process.cpp \
        process1_scanbeforetrace.cpp \
        project.cpp \
        pshowdlg.cpp \
        qtmysunnydlg.cpp \
        qtweldingdlg.cpp \
        robotcontrol.cpp \
        robotdata.cpp \
        robotsetdlg.cpp \
        setcraft0dlg.cpp \
        setcraft1dlg.cpp \
        setcraft2dlg.cpp \
        setcraft3dlg.cpp \
        setmovecdlg.cpp \
        setprojectdlg.cpp \
        showtasknumdlg.cpp \
        sndatadlg.cpp \
        soptocameratcpip.cpp \
        soptopcamera.cpp \
        sshpassworddlg.cpp \
        synchronous.cpp \
        taskcleardlg.cpp \
        tosendbuffer.cpp \
        traceedit0dlg.cpp \
        traceedit1dlg.cpp \
        traceedit2dlg.cpp \
        weldsetdlg.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    arrvepointtestdlg.ui \
    cambuilddlg.ui \
    checkscandatadlg.ui \
    demarcatedlg.ui \
    editcraftdlg.ui \
    editprojectdlg.ui \
    edittextdlg.ui \
    infodlg.ui \
    keyaoutdlg.ui \
    keycamdlg.ui \
    keycoorddlg.ui \
    keycreatadddlg.ui \
    keycreataddpdlg.ui \
    keycreatcdlg.ui \
    keycreatdlg.ui \
    keycreatfdlg.ui \
    keycreatpdlg.ui \
    keydelaydlg.ui \
    keyfilterdlg.ui \
    keygetposdlg.ui \
    keygettcppos2dlg.ui \
    keygettcpposdlg.ui \
    keygotodlg.ui \
    keygowelddlg.ui \
    keyiodlg.ui \
    keymovdlg.ui \
    keyplcdlg.ui \
    keyplotposdlg.ui \
    keysampledlg.ui \
    keysavepcddlg.ui \
    keyscandlg.ui \
    keysearchdlg.ui \
    keysearchenddlg.ui \
    keysetposedlg.ui \
    keysmovdlg.ui \
    keysscandlg.ui \
    keytrace2dlg.ui \
    keytraceadddlg.ui \
    keytracecontinuedlg.ui \
    keytracedlg.ui \
    keytracerealtime2dlg.ui \
    keytracerealtimedlg.ui \
    keytracingdlg.ui \
    keywavedlg.ui \
    keywelddlg.ui \
    newcraftdlg.ui \
    newprojectdlg.ui \
    othercmddlg.ui \
    plcsetdlg.ui \
    plotedit0dlg.ui \
    plotedit1dlg.ui \
    plotedit2dlg.ui \
    pshowdlg.ui \
    qtmysunnydlg.ui \
    qtweldingdlg.ui \
    robotsetdlg.ui \
    setcraft0dlg.ui \
    setcraft1dlg.ui \
    setcraft2dlg.ui \
    setcraft3dlg.ui \
    setmovecdlg.ui \
    setprojectdlg.ui \
    showtasknumdlg.ui \
    sndatadlg.ui \
    sshpassworddlg.ui \
    taskcleardlg.ui \
    traceedit0dlg.ui \
    traceedit1dlg.ui \
    traceedit2dlg.ui \
    weldsetdlg.ui

HEADERS += \
    CConnectionForSshClient.h \
    E2proomData.h \
    FileOut.h \
    MyCheckIPinfo.h \
    MyPlcFunction.h \
    MyQrFunction.h \
    MytracerealtimeFunction.h \
    PictureBox.h \
    QrCode.h \
    ResultData.h \
    SN_Data.h \
    TCProsinterface.h \
    TimeFunction.h \
    XTcp.h \
    arrvepointtestdlg.h \
    calibration.h \
    cam_sen.h \
    cambuilddlg.h \
    ccoordchange.h \
    checkscandatadlg.h \
    craft.h \
    cweldtarject.h \
    demarcatedlg.h \
    editcraftdlg.h \
    editprojectdlg.h \
    edittextdlg.h \
    fitlineside.h \
    global.h \
    infodlg.h \
    ipaddress.h \
    keyaoutdlg.h \
    keycamdlg.h \
    keycoorddlg.h \
    keycreatadddlg.h \
    keycreataddpdlg.h \
    keycreatcdlg.h \
    keycreatdlg.h \
    keycreatfdlg.h \
    keycreatpdlg.h \
    keydelaydlg.h \
    keyfilterdlg.h \
    keygetposdlg.h \
    keygettcppos2dlg.h \
    keygettcpposdlg.h \
    keygotodlg.h \
    keygowelddlg.h \
    keyiodlg.h \
    keymovdlg.h \
    keyplcdlg.h \
    keyplotposdlg.h \
    keysampledlg.h \
    keysavepcddlg.h \
    keyscandlg.h \
    keysearchdlg.h \
    keysearchenddlg.h \
    keysetposedlg.h \
    keysmovdlg.h \
    keysscandlg.h \
    keytrace2dlg.h \
    keytraceadddlg.h \
    keytracecontinuedlg.h \
    keytracedlg.h \
    keytracerealtime2dlg.h \
    keytracerealtimedlg.h \
    keytracingdlg.h \
    keywavedlg.h \
    keywelddlg.h \
    my_cmd.h \
    my_parameters.h \
    myfont.h \
    newcraftdlg.h \
    newprojectdlg.h \
    othercmddlg.h \
    plcsetdlg.h \
    plotedit0dlg.h \
    plotedit1dlg.h \
    plotedit2dlg.h \
    process.h \
    process1_scanbeforetrace.h \
    project.h \
    pshowdlg.h \
    qtmysunnydlg.h \
    qtweldingdlg.h \
    robotcontrol.h \
    robotdata.h \
    robotsetdlg.h \
    setcraft0dlg.h \
    setcraft1dlg.h \
    setcraft2dlg.h \
    setcraft3dlg.h \
    setmovecdlg.h \
    setprojectdlg.h \
    showtasknumdlg.h \
    sndatadlg.h \
    soptocameratcpip.h \
    soptopcamera.h \
    sshpassworddlg.h \
    synchronous.h \
    taskcleardlg.h \
    tistdtypes.h \
    tosendbuffer.h \
    traceedit0dlg.h \
    traceedit1dlg.h \
    traceedit2dlg.h \
    weldsetdlg.h


win32{

QMAKE_CXXFLAGS_RELEASE = -O2  -MD  -GL
QMAKE_CXXFLAGS_DEBUG  =  -Zi  -MDd

#opencv库的添加
INCLUDEPATH += D:/opencv/build/include

win32:CONFIG(release, debug|release): LIBS += -LD:/opencv/build/x64/vc16/lib/ -lopencv_world480
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/opencv/build/x64/vc16/lib/ -lopencv_world480d

#自定义ROS接口添加
#INCLUDEPATH += C:/Users/qubo/Documents/Qtproject/myRos2test/install/tutorial_interfaces/include

#LIBS += C:/Users/qubo/Documents/Qtproject/myRos2test/install/tutorial_interfaces/lib/*.lib

#ROS库添加
#INCLUDEPATH += C:/opt/ros/foxy/x64/include

#LIBS += C:/opt/ros/foxy/x64/Lib/*.lib \


#modbustcp库的添加
INCLUDEPATH += D:/libmodbus/include \

win32:CONFIG(release, debug|release):LIBS += -LD:/libmodbus/x64/lib/ -lmodbus
else:win32:CONFIG(debug, debug|release):LIBS += -LD:/libmodbus/x64/lib/ -lmodbus


#Eigen库的添加
INCLUDEPATH += D:/eigen3

#QSsh库的添加
win32:CONFIG(release, debug|release): LIBS += -LD:/Qt/6.2.4/msvc2019_64/lib/ -lQSsh
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/Qt/6.2.4/msvc2019_64/lib/ -lQSshd

INCLUDEPATH += D:/Qt/6.2.4/msvc2019_64/include/Qssh

INCLUDEPATH += C:/Botan/include/botan-2 \

LIBS += -LC:/Botan/lib/ -lbotan-2

#VTK库添加
win32:CONFIG(release, debug|release): LIBS += D:/VTK/lib/vtk*-9.2.lib
else:win32:CONFIG(debug, debug|release): LIBS += D:/VTK/lib/vtk*-9.2d.lib

INCLUDEPATH += D:/VTK/include/vtk-9.2

#boost库添加
win32:CONFIG(release, debug|release): LIBS += D:/boost/lib64-msvc-14.2/libboost*.lib
else:win32:CONFIG(debug, debug|release): LIBS += D:/boost/lib64-msvc-14.2/libboost*-gd-*.lib

INCLUDEPATH += D:/boost


#PCL库添加
win32:CONFIG(release, debug|release): LIBS += D:/PCL/lib/pcl_*.lib
else:win32:CONFIG(debug, debug|release): LIBS += D:/PCL/lib/pcl_*d.lib

INCLUDEPATH += D:/PCL/include/pcl-1.13

#flann库添加
win32:CONFIG(release, debug|release): LIBS += D:/PCL/3rdParty/FLANN/lib/flann*.lib
else:win32:CONFIG(debug, debug|release): LIBS += D:/PCL/3rdParty/FLANN/lib/flann*.lib

INCLUDEPATH += D:/PCL/3rdParty/FLANN/include

#ZXing库的添加

win32:CONFIG(release, debug|release): LIBS += -LD:/zxing/lib/ -lZXing
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/zxing/lib/ -lZXing

INCLUDEPATH += D:/zxing/include/ZXing

}

unix {
#opencv库的添加
INCLUDEPATH += /usr/local/include/opencv4 \

LIBS += /usr/local/lib/libopencv_*

#ROS库添加
INCLUDEPATH +=/opt/ros/galactic/include \

LIBS += /opt/ros/galactic/lib/lib*.so
LIBS += /opt/ros/galactic/lib/x86_64-linux-gnu/lib*.so

#Eigen库的添加
INCLUDEPATH += /usr/include/eigen3

#QSsh库的添加

INCLUDEPATH += /home/qubo/Qt/6.2.4/gcc_64/include/QSsh

unix:!macx: LIBS += -L /home/qubo/Qt/6.2.4/gcc_64/lib/ -lQSsh

#VTK库添加
INCLUDEPATH +=/usr/local/include/vtk-9.2 \

LIBS += /usr/local/lib/libvtk*

#Boost
INCLUDEPATH += /usr/include/boost \

LIBS += /usr/lib/x86_64-linux-gnu/libboost_*.so

#PCL库添加
INCLUDEPATH +=/usr/local/include/pcl-1.13 \

LIBS += /usr/local/lib/libpcl_*

#自定义ROS接口添加
INCLUDEPATH +=/home/qubo/myRos2test/install/tutorial_interfaces/include \

LIBS += /home/qubo/myRos2test/install/tutorial_interfaces/lib/libtutorial_interfaces*.so

#modbustcp库的添加
INCLUDEPATH += /usr/include \

LIBS += /usr/lib/x86_64-linux-gnu/libmodbus.so

#ZXing库的添加
INCLUDEPATH += /usr/local/include/ZXing \

unix:!macx: LIBS += -L/usr/local/lib/ -lZXing

}

RESOURCES += \
    rsc.qrc




