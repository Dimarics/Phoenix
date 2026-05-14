INCLUDEPATH += $$PWD

resources.files += \
    $$PWD/qmldir \
    $$PWD/Script.qml

HEADERS += \
    $$PWD/cpphighlighter.h \
    $$PWD/pythonhighlighter.h \
    $$PWD/scriptcontroller.h \
    $$PWD/scriptworker.h

SOURCES += \
    $$PWD/cpphighlighter.cpp \
    $$PWD/pythonhighlighter.cpp \
    $$PWD/scriptcontroller.cpp \
    $$PWD/scriptworker.cpp
