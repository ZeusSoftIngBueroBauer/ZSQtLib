#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T15:57:36
#
#-------------------------------------------------

QT += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

MODULE = ZSAppTestSysIdxTree

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

INCLUDEPATH += ../../../../Include/Apps/Test/ZSSysIdxTree
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
    LIBS += -lZSIpcTrace
    LIBS += -lZSSysGUI
    LIBS += -lZSTestGUI
    LIBS += -lZSIpcGUI
    LIBS += -lZSIpcTraceGUI
}
CONFIG(debug, release|debug) {
    LIBS += -lZSSysd
    LIBS += -lZSTestd
    LIBS += -lZSIpcd
    LIBS += -lZSIpcTraced
    LIBS += -lZSSysGUId
    LIBS += -lZSTestGUId
    LIBS += -lZSIpcGUId
    LIBS += -lZSIpcTraceGUId
}

SOURCES += \
    ../../../../Source/Apps/Test/ZSSysIdxTree/App.cpp \
    ../../../../Source/Apps/Test/ZSSysIdxTree/Main.cpp \
    ../../../../Source/Apps/Test/ZSSysIdxTree/MainWindow.cpp \
    ../../../../Source/Apps/Test/ZSSysIdxTree/Test.cpp \
    ../../../../Source/Apps/Test/ZSSysIdxTree/WidgetCentral.cpp

HEADERS  += \
    ../../../../Include/Apps/Test/ZSSysIdxTree/App.h \
    ../../../../Include/Apps/Test/ZSSysIdxTree/MainWindow.h \
    ../../../../Include/Apps/Test/ZSSysIdxTree/Test.h \
    ../../../../Include/Apps/Test/ZSSysIdxTree/WidgetCentral.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/bin
    } else {
        target.path = /usr/bin
    }
    INSTALLS += target
}
