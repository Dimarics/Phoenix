INCLUDEPATH += $$PWD

resources.files += \
    $$PWD/qmldir \
    $$PWD/Script.qml

HEADERS += \
    $$PWD/cpphighlighter.h \
    $$PWD/pythonhighlighter.h \
    $$PWD/scriptbackend.h

SOURCES += \
    $$PWD/cpphighlighter.cpp \
    $$PWD/pythonhighlighter.cpp \
    $$PWD/scriptbackend.cpp
