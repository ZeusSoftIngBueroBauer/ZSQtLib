#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T15:57:36
#
#-------------------------------------------------

QT += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

MODULE = ZSAppTestSysLogFile

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

INCLUDEPATH += ../../../../Include/Apps/Test/ZSSysLogFile
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
    LIBS += -lZSSysGUI
    LIBS += -lZSTestGUI
}
CONFIG(debug, release|debug) {
    LIBS += -lZSSysd
    LIBS += -lZSTestd
    LIBS += -lZSSysGUId
    LIBS += -lZSTestGUId
}

SOURCES += \
    ../../../../Source/Apps/Test/ZSSysLogFile/App.cpp \
    ../../../../Source/Apps/Test/ZSSysLogFile/Main.cpp \
    ../../../../Source/Apps/Test/ZSSysLogFile/MainWindow.cpp \
    ../../../../Source/Apps/Test/ZSSysLogFile/Test.cpp \
    ../../../../Source/Apps/Test/ZSSysLogFile/WidgetCentral.cpp

HEADERS  += \
    ../../../../Include/Apps/Test/ZSSysLogFile/App.h \
    ../../../../Include/Apps/Test/ZSSysLogFile/MainWindow.h \
    ../../../../Include/Apps/Test/ZSSysLogFile/Test.h \
    ../../../../Include/Apps/Test/ZSSysLogFile/WidgetCentral.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/bin
    } else {
        target.path = /usr/bin
    }
    INSTALLS += target
}
