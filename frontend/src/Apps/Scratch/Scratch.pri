include($$PWD/Blocks/Blocks.pri)

resources.files += \
    $$PWD/qmldir \
    $$PWD/BasicBlock.qml \
    $$PWD/BlocksPanel.qml \
    $$PWD/BooleanBlock.qml \
    $$PWD/BooleanSocket.qml \
    $$PWD/ContentBlock.qml \
    $$PWD/ListItem.qml \
    $$PWD/ValueBlock.qml \
    $$PWD/ValueInput.qml \
    $$PWD/RepeatBlock.qml \
    $$PWD/Scratch.qml \
    $$PWD/ScratchScrollBar.qml \
    $$PWD/ScratchText.qml \
    $$PWD/ScratchWorkspace.qml \
    $$PWD/Socket.qml \
    $$PWD/StackBlock.qml

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/abstractblock.h

SOURCES += \
    $$PWD/abstractblock.cpp
