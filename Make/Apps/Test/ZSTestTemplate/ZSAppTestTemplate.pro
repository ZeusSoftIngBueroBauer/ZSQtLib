#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T15:57:36
#
#-------------------------------------------------

QT += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

MODULE = ZSAppTestTemplate

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

INCLUDEPATH += ../../../../Include/Apps/Test/ZSTemplate
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
    ../../../../Source/Apps/Test/ZSTemplate/App.cpp \
    ../../../../Source/Apps/Test/ZSTemplate/Main.cpp \
    ../../../../Source/Apps/Test/ZSTemplate/MainWindow.cpp \
    ../../../../Source/Apps/Test/ZSTemplate/Test.cpp \
    ../../../../Source/Apps/Test/ZSTemplate/TSGrpTemplate.cpp \
    ../../../../Source/Apps/Test/ZSTemplate/WidgetCentral.cpp

HEADERS  += \
    ../../../../Include/Apps/Test/ZSTemplate/App.h \
    ../../../../Include/Apps/Test/ZSTemplate/MainWindow.h \
    ../../../../Include/Apps/Test/ZSTemplate/Test.h \
    ../../../../Include/Apps/Test/ZSTemplate/TSGrpTemplate.h \
    ../../../../Include/Apps/Test/ZSTemplate/WidgetCentral.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/bin
    } else {
        target.path = /usr/bin
    }
    INSTALLS += target
}
