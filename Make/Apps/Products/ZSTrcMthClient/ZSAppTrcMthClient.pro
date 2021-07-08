#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T15:57:36
#
#-------------------------------------------------

QT += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

MODULE = ZSAppTrcMthClient

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

INCLUDEPATH += ../../../../Include/Apps/Products/ZSTrcMthClient
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
    LIBS += -lZSIpcTrace
    LIBS += -lZSSysGUI
    LIBS += -lZSIpcGUI
    LIBS += -lZSIpcTraceGUI
}
CONFIG(debug, release|debug) {
    LIBS += -lZSSysd
    LIBS += -lZSIpcd
    LIBS += -lZSIpcTraced
    LIBS += -lZSSysGUId
    LIBS += -lZSIpcGUId
    LIBS += -lZSIpcTraceGUId
}

SOURCES += \
    ../../../../Source/Apps/Products/ZSTrcMthClient/App.cpp \
    ../../../../Source/Apps/Products/ZSTrcMthClient/Main.cpp \
    ../../../../Source/Apps/Products/ZSTrcMthClient/MainWindow.cpp \
    ../../../../Source/Apps/Products/ZSTrcMthClient/WidgetCentral.cpp

HEADERS += \
    ../../../../Include/Apps/Products/ZSTrcMthClient/App.h \
    ../../../../Include/Apps/Products/ZSTrcMthClient/MainWindow.h \
    ../../../../Include/Apps/Products/ZSTrcMthClient/WidgetCentral.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/bin
    } else {
        target.path = /usr/bin
    }
    INSTALLS += target
}
