#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T15:57:36
#
#-------------------------------------------------

QT += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

MODULE = ZSAppRemCmdClient

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
    MOC_DIR = ../../../../Tmp/Moc/Apps/Products/$$MODULE/$$PLATFORM/Release
    OBJECTS_DIR = ../../../../Tmp/Int/Apps/Products/$$MODULE/$$PLATFORM/Release
}
CONFIG(debug, release|debug) {
    TARGET = $$MODULE"d"
    MOC_DIR = ../../../../Tmp/Moc/Apps/Products/$$MODULE/$$PLATFORM/Debug
    OBJECTS_DIR = ../../../../Tmp/Int/Apps/Products/$$MODULE/$$PLATFORM/Debug
}
DESTDIR = ../../../../Bin/$$PLATFORM

CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
}
DEFINES += _ZSTRACE

INCLUDEPATH += ../../../../Include/Apps/Products/ZSRemCmdClient
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
    LIBS += -lZSIpc
    LIBS += -lZSTrace
    LIBS += -lZSRemCmd
    LIBS += -lZSSysGUI
    LIBS += -lZSIpcGUI
    LIBS += -lZSTraceGUI
    LIBS += -lZSRemCmdGUI
}
CONFIG(debug, release|debug) {
    LIBS += -lZSSysd
    LIBS += -lZSIpcd
    LIBS += -lZSTraced
    LIBS += -lZSRemCmdd
    LIBS += -lZSSysGUId
    LIBS += -lZSIpcGUId
    LIBS += -lZSTraceGUId
    LIBS += -lZSRemCmdGUId
}

SOURCES += \
    ../../../../Source/Apps/Products/ZSRemCmdClient/App.cpp \
    ../../../../Source/Apps/Products/ZSRemCmdClient/Main.cpp \
    ../../../../Source/Apps/Products/ZSRemCmdClient/MainWindow.cpp \
    ../../../../Source/Apps/Products/ZSRemCmdClient/WidgetCentral.cpp

HEADERS += \
    ../../../../Include/Apps/Products/ZSRemCmdClient/App.h \
    ../../../../Include/Apps/Products/ZSRemCmdClient/MainWindow.h \
    ../../../../Include/Apps/Products/ZSRemCmdClient/WidgetCentral.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/bin
    } else {
        target.path = /usr/bin
    }
    INSTALLS += target
}
