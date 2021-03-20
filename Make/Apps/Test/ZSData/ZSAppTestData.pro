#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T15:57:36
#
#-------------------------------------------------

QT += core gui network xml sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

MODULE = ZSAppTestData

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

INCLUDEPATH += ../../../../Include/Apps/Test/ZSData
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
    LIBS += -lZSPhysVal
    LIBS += -lZSPhysSizesKinematics
    LIBS += -lZSPhysSizesElectricity
    LIBS += -lZSIpc
    LIBS += -lZSTrace
    LIBS += -lZSData
    LIBS += -lZSSysGUI
    LIBS += -lZSTestGUI
    LIBS += -lZSIpcGUI
    LIBS += -lZSTraceGUI
    LIBS += -lZSDataGUI
}
CONFIG(debug, release|debug) {
    LIBS += -lZSSysd
    LIBS += -lZSTestd
    LIBS += -lZSPhysVald
    LIBS += -lZSPhysSizesKinematicsd
    LIBS += -lZSPhysSizesElectricityd
    LIBS += -lZSIpcd
    LIBS += -lZSTraced
    LIBS += -lZSDatad
    LIBS += -lZSSysGUId
    LIBS += -lZSTestGUId
    LIBS += -lZSIpcGUId
    LIBS += -lZSTraceGUId
    LIBS += -lZSDataGUId
}

SOURCES += \
    ../../../../Source/Apps/Test/ZSData/App.cpp \
    ../../../../Source/Apps/Test/ZSData/Main.cpp \
    ../../../../Source/Apps/Test/ZSData/MainWindow.cpp \
    ../../../../Source/Apps/Test/ZSData/Test.cpp \
    ../../../../Source/Apps/Test/ZSData/TestAux.cpp \
    ../../../../Source/Apps/Test/ZSData/TSGrpShutdown.cpp \
    ../../../../Source/Apps/Test/ZSData/TSGrpShutdownDb.cpp \
    ../../../../Source/Apps/Test/ZSData/TSGrpShutdownDbClt1.cpp \
    ../../../../Source/Apps/Test/ZSData/TSGrpShutdownDbClt2.cpp \
    ../../../../Source/Apps/Test/ZSData/TSGrpShutdownDbClt3.cpp \
    ../../../../Source/Apps/Test/ZSData/TSGrpShutdownDbClts.cpp \
    ../../../../Source/Apps/Test/ZSData/TSGrpStartup.cpp \
    ../../../../Source/Apps/Test/ZSData/TSGrpStartupDb.cpp \
    ../../../../Source/Apps/Test/ZSData/TSGrpStartupDbClt1.cpp \
    ../../../../Source/Apps/Test/ZSData/TSGrpStartupDbClt2.cpp \
    ../../../../Source/Apps/Test/ZSData/TSGrpStartupDbClt3.cpp \
    ../../../../Source/Apps/Test/ZSData/TSGrpStartupDbClts.cpp \
    ../../../../Source/Apps/Test/ZSData/WidgetCentral.cpp

HEADERS += \
    ../../../../Include/Apps/Test/ZSData/App.h \
    ../../../../Include/Apps/Test/ZSData/MainWindow.h \
    ../../../../Include/Apps/Test/ZSData/Test.h \
    ../../../../Include/Apps/Test/ZSData/TSGrpShutdown.h \
    ../../../../Include/Apps/Test/ZSData/TSGrpShutdownDb.h \
    ../../../../Include/Apps/Test/ZSData/TSGrpShutdownDbClt1.h \
    ../../../../Include/Apps/Test/ZSData/TSGrpShutdownDbClt2.h \
    ../../../../Include/Apps/Test/ZSData/TSGrpShutdownDbClt3.h \
    ../../../../Include/Apps/Test/ZSData/TSGrpShutdownDbClts.h \
    ../../../../Include/Apps/Test/ZSData/TSGrpStartup.h \
    ../../../../Include/Apps/Test/ZSData/TSGrpStartupDb.h \
    ../../../../Include/Apps/Test/ZSData/TSGrpStartupDbClt1.h \
    ../../../../Include/Apps/Test/ZSData/TSGrpStartupDbClt2.h \
    ../../../../Include/Apps/Test/ZSData/TSGrpStartupDbClt3.h \
    ../../../../Include/Apps/Test/ZSData/TSGrpStartupDbClts.h \
    ../../../../Include/Apps/Test/ZSData/WidgetCentral.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/bin
    } else {
        target.path = /usr/bin
    }
    INSTALLS += target
}
