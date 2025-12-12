QT += core serialport websockets httpserver

include(src/src.pri)

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#INCLUDEPATH += include

win32: {
    #RC_FILE = resources.rc
    INCLUDEPATH += D:\Programs\OpenCV-4.12.0\include
    LIBS += -LD:\Programs\OpenCV-4.12.0\bin
    LIBS += -lopencv_core4120
    LIBS += -lopencv_highgui4120
    LIBS += -lopencv_imgcodecs4120
    LIBS += -lopencv_imgproc4120
    #LIBS += -lopencv_video4120
    LIBS += -lopencv_videoio4120
    LIBS += -lopencv_objdetect4120
}
unix: {
    INCLUDEPATH += /home/mes/OpenCV-4.12.0/include/opencv4
    LIBS += -L/home/mes/OpenCV-4.12.0/lib
    LIBS += -lopencv_core
    LIBS += -lopencv_highgui
    LIBS += -lopencv_imgcodecs
    LIBS += -lopencv_imgproc
    LIBS += -lopencv_videoio
    LIBS += -lopencv_objdetect
    LIBS += -lopencv_calib3d
    LIBS += -lopencv_dnn
    LIBS += -lopencv_features2d
    LIBS += -lopencv_flann
}
