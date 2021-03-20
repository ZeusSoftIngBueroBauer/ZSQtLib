#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T15:57:36
#
#-------------------------------------------------

QT += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

MODULE = ZSAppTestRemCmdServer

VERSION = 4.6.3

TEMPLATE = app

windows {
    CONFIG += skip_target_version_ext
    DEFINES += _WINDOWS
    PLATFORM = x64
}
linux {
    DEFINES += __linux__
    PLATFORM = Linux
}

CONFIG(release, release|debug) {
    TARGET = $$MODULE
    MOC_DIR = ../../../../Tmp/Moc/Apps/Test/$$MODULE/$$PLATFORM/Release
    OBJECTS_DIR = ../../../../Tmp/Int/Apps/Test/$$MODULE/$$PLATFORM/Release
}
CONFIG(debug, release|debug) {
    TARGET = $$MODULE"d"
    MOC_DIR = ../../../../Tmp/Moc/Apps/Test/$$MODULE/$$PLATFORM/Debug
    OBJECTS_DIR = ../../../../Tmp/Int/Apps/Test/$$MODULE/$$PLATFORM/Debug
}
DESTDIR = ../../../../Bin/$$PLATFORM

CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
}
DEFINES += _ZSTRACE

INCLUDEPATH += ../../../../Include/Apps/Test/ZSRemCmdServer
INCLUDEPATH += ../../../../Include/Libs
linux {
    INCLUDEPATH += /usr/local/include
}

LIBS += -L../../../../Lib/$$PLATFORM
linux {
    LIBS += -L/usr/local/lib
}

CONFIG(release, release|debug) {
    LIBS += -lZSSys
    LIBS += -lZSTest
    LIBS += -lZSIpc
    LIBS += -lZSTrace
    LIBS += -lZSRemCmd
    LIBS += -lZSSysGUI
    LIBS += -lZSTestGUI
    LIBS += -lZSIpcGUI
    LIBS += -lZSTraceGUI
    LIBS += -lZSRemCmdGUI
}
CONFIG(debug, release|debug) {
    LIBS += -lZSSysd
    LIBS += -lZSTestd
    LIBS += -lZSIpcd
    LIBS += -lZSTraced
    LIBS += -lZSRemCmdd
    LIBS += -lZSSysGUId
    LIBS += -lZSTestGUId
    LIBS += -lZSIpcGUId
    LIBS += -lZSTraceGUId
    LIBS += -lZSRemCmdGUId
}

SOURCES += \
    ../../../../Source/Apps/Test/ZSRemCmdServer/App.cpp \
    ../../../../Source/Apps/Test/ZSRemCmdServer/Database.cpp \
    ../../../../Source/Apps/Test/ZSRemCmdServer/Main.cpp \
    ../../../../Source/Apps/Test/ZSRemCmdServer/MainWindow.cpp \
    ../../../../Source/Apps/Test/ZSRemCmdServer/MsgTest.cpp \
    ../../../../Source/Apps/Test/ZSRemCmdServer/SigAna.cpp \
    ../../../../Source/Apps/Test/ZSRemCmdServer/SigGen.cpp \
    ../../../../Source/Apps/Test/ZSRemCmdServer/TestModule1.cpp \
    ../../../../Source/Apps/Test/ZSRemCmdServer/TestModule2.cpp \
    ../../../../Source/Apps/Test/ZSRemCmdServer/WdgtTest.cpp \
    ../../../../Source/Apps/Test/ZSRemCmdServer/WidgetCentral.cpp

HEADERS += \
    ../../../../Include/Apps/Test/ZSRemCmdServer/App.h \
    ../../../../Include/Apps/Test/ZSRemCmdServer/Database.h \
    ../../../../Include/Apps/Test/ZSRemCmdServer/MainWindow.h \
    ../../../../Include/Apps/Test/ZSRemCmdServer/MsgTest.h \
    ../../../../Include/Apps/Test/ZSRemCmdServer/SigAna.h \
    ../../../../Include/Apps/Test/ZSRemCmdServer/SigGen.h \
    ../../../../Include/Apps/Test/ZSRemCmdServer/TestModule1.h \
    ../../../../Include/Apps/Test/ZSRemCmdServer/TestModule2.h \
    ../../../../Include/Apps/Test/ZSRemCmdServer/WdgtTest.h \
    ../../../../Include/Apps/Test/ZSRemCmdServer/WidgetCentral.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/bin
    } else {
        target.path = /usr/bin
    }
    INSTALLS += target
}
