#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T15:57:36
#
#-------------------------------------------------

QT += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

MODULE = ZSAppTestAsyncReqs

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

INCLUDEPATH += ../../../../Include/Apps/Test/ZSAsyncReqs
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
    ../../../../Source/Apps/Test/ZSAsyncReqs/App.cpp \
    ../../../../Source/Apps/Test/ZSAsyncReqs/Database.cpp \
    ../../../../Source/Apps/Test/ZSAsyncReqs/DatabaseGateway.cpp \
    ../../../../Source/Apps/Test/ZSAsyncReqs/DatabaseThread.cpp \
    ../../../../Source/Apps/Test/ZSAsyncReqs/DbClient.cpp \
    ../../../../Source/Apps/Test/ZSAsyncReqs/DbClientGateway.cpp \
    ../../../../Source/Apps/Test/ZSAsyncReqs/DbClientThread.cpp \
    ../../../../Source/Apps/Test/ZSAsyncReqs/Main.cpp \
    ../../../../Source/Apps/Test/ZSAsyncReqs/MainWindow.cpp \
    ../../../../Source/Apps/Test/ZSAsyncReqs/Msg.cpp \
    ../../../../Source/Apps/Test/ZSAsyncReqs/WidgetCentral.cpp

HEADERS += \
    ../../../../Include/Apps/Test/ZSAsyncReqs/App.h \
    ../../../../Include/Apps/Test/ZSAsyncReqs/Database.h \
    ../../../../Include/Apps/Test/ZSAsyncReqs/DatabaseGateway.h \
    ../../../../Include/Apps/Test/ZSAsyncReqs/DatabaseThread.h \
    ../../../../Include/Apps/Test/ZSAsyncReqs/DbClient.h \
    ../../../../Include/Apps/Test/ZSAsyncReqs/DbClientGateway.h \
    ../../../../Include/Apps/Test/ZSAsyncReqs/DbClientThread.h \
    ../../../../Include/Apps/Test/ZSAsyncReqs/MainWindow.h \
    ../../../../Include/Apps/Test/ZSAsyncReqs/Msg.h \
    ../../../../Include/Apps/Test/ZSAsyncReqs/WidgetCentral.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/bin
    } else {
        target.path = /usr/bin
    }
    INSTALLS += target
}
