QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cpu.cpp \
    disassembler.cpp \
    main.cpp \
    mainwindow.cpp \
    memoryviewer.cpp \
    mmu.cpp \
    register.cpp \
    registerviewer.cpp \
    stackviewer.cpp

HEADERS += \
    cpu.h \
    defs.h \
    disassembler.h \
    mainwindow.h \
    memoryviewer.h \
    mmu.h \
    register.h \
    registerviewer.h \
    stackviewer.h

FORMS += \
    disassembler.ui \
    mainwindow.ui \
    memoryviewer.ui \
    registerviewer.ui \
    stackviewer.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    TETRIS.gb
