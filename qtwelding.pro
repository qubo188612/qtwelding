QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat
QT       += core gui network

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        E2proomData.cpp \
        FileOut.cpp \
        PictureBox.cpp \
        ResultData.cpp \
        TimeFunction.cpp \
        XTcp.cpp \
        calibration.cpp \
        cam_sen.cpp \
        cambuilddlg.cpp \
        demarcatedlg.cpp \
        global.cpp \
        ipaddress.cpp \
        main.cpp \
        my_parameters.cpp \
        project.cpp \
        qtmysunnydlg.cpp \
        qtweldingdlg.cpp \
        robotdata.cpp \
        robotsetdlg.cpp \
        showtasknumdlg.cpp \
        soptocameratcpip.cpp \
        soptopcamera.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    cambuilddlg.ui \
    demarcatedlg.ui \
    qtmysunnydlg.ui \
    qtweldingdlg.ui \
    robotsetdlg.ui \
    showtasknumdlg.ui

HEADERS += \
    E2proomData.h \
    FileOut.h \
    PictureBox.h \
    ResultData.h \
    TimeFunction.h \
    XTcp.h \
    calibration.h \
    cam_sen.h \
    cambuilddlg.h \
    demarcatedlg.h \
    global.h \
    ipaddress.h \
    my_parameters.h \
    project.h \
    qtmysunnydlg.h \
    qtweldingdlg.h \
    robotdata.h \
    robotsetdlg.h \
    showtasknumdlg.h \
    soptocameratcpip.h \
    soptopcamera.h \
    tistdtypes.h


win32{
#opencv库的添加
INCLUDEPATH += D:/opencv-4.6.0/build/include \

LIBS += D:/opencv-4.6.0/build/x64/vc16/lib/opencv*.lib \

#自定义ROS接口添加
INCLUDEPATH += C:/Users/34638/Documents/ros2topic/myRos2test/install/tutorial_interfaces/include \

LIBS += C:/Users/34638/Documents/ros2topic/myRos2test/install/tutorial_interfaces/lib/*.lib \

#ROS库添加
INCLUDEPATH += C:/opt/ros/foxy/x64/include \

LIBS += C:/opt/ros/foxy/x64/Lib/*.lib \


#modbustcp库的添加
INCLUDEPATH += D:/libmodbus/include \

LIBS += D:/libmodbus/x64/lib/*.lib
}

unix {
#opencv库的添加
INCLUDEPATH += /usr/local/OpenCV/Release/include/opencv4 \

LIBS += /usr/local/OpenCV/Release/lib/libopencv_*

#ROS库添加
INCLUDEPATH +=/opt/ros/galactic/include \

LIBS += /opt/ros/galactic/lib/lib*.so
LIBS += /opt/ros/galactic/lib/x86_64-linux-gnu/lib*.so

#自定义ROS接口添加
INCLUDEPATH +=/home/qubo/myRos2test/install/tutorial_interfaces/include \

LIBS += /home/qubo/myRos2test/install/tutorial_interfaces/lib/libtutorial_interfaces*.so

#modbustcp库的添加
INCLUDEPATH += /home/qubo/modbus/libmodbus/install/include \

LIBS += /home/qubo/modbus/libmodbus/install/lib/libmodbus.so

#Eigen库的添加
INCLUDEPATH += /usr/include/eigen3
}
