#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T15:57:36
#
#-------------------------------------------------

QT += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

MODULE = ZSAppTestIpc

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

INCLUDEPATH += ../../../../Include/Apps/Test/ZSIpc
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
    LIBS += -lZSSysGUI
    LIBS += -lZSTestGUI
    LIBS += -lZSIpcGUI
    LIBS += -lZSTraceGUI
}
CONFIG(debug, release|debug) {
    LIBS += -lZSSysd
    LIBS += -lZSTestd
    LIBS += -lZSIpcd
    LIBS += -lZSTraced
    LIBS += -lZSSysGUId
    LIBS += -lZSTestGUId
    LIBS += -lZSIpcGUId
    LIBS += -lZSTraceGUId
}

SOURCES += \
    ../../../../Source/Apps/Test/ZSIpc/App.cpp \
    ../../../../Source/Apps/Test/ZSIpc/Main.cpp \
    ../../../../Source/Apps/Test/ZSIpc/MainWindow.cpp \
    ../../../../Source/Apps/Test/ZSIpc/Test.cpp \
    ../../../../Source/Apps/Test/ZSIpc/TestAux.cpp \
    ../../../../Source/Apps/Test/ZSIpc/TSGrpClientBlockingCalls.cpp \
    ../../../../Source/Apps/Test/ZSIpc/TSGrpClientRequestQueue.cpp \
    ../../../../Source/Apps/Test/ZSIpc/TSGrpSendDataTcpBlkTypeE.cpp \
    ../../../../Source/Apps/Test/ZSIpc/TSGrpSendDataTcpBlkTypeL.cpp \
    ../../../../Source/Apps/Test/ZSIpc/TSGrpSendDataTcpBlkTypeSL.cpp \
    ../../../../Source/Apps/Test/ZSIpc/TSGrpSendDataTcpBlkTypeTLV.cpp \
    ../../../../Source/Apps/Test/ZSIpc/TSGrpServerBlockingCalls.cpp \
    ../../../../Source/Apps/Test/ZSIpc/TSGrpServerRequestQueue.cpp \
    ../../../../Source/Apps/Test/ZSIpc/WidgetCentral.cpp

HEADERS += \
    ../../../../Include/Apps/Test/ZSIpc/App.h \
    ../../../../Include/Apps/Test/ZSIpc/MainWindow.h \
    ../../../../Include/Apps/Test/ZSIpc/Test.h \
    ../../../../Include/Apps/Test/ZSIpc/WidgetCentral.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/bin
    } else {
        target.path = /usr/bin
    }
    INSTALLS += target
}
