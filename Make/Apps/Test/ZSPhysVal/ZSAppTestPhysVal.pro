#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T15:57:36
#
#-------------------------------------------------

QT += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

MODULE = ZSAppTestPhysVal

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

INCLUDEPATH += ../../../../Include/Apps/Test/ZSPhysVal
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
    LIBS += -lZSPhysSizesElectricity
    LIBS += -lZSPhysSizesGeometry
    LIBS += -lZSPhysSizesKinematics
    LIBS += -lZSSysGUI
    LIBS += -lZSTestGUI
    LIBS += -lZSPhysValGUI
}
CONFIG(debug, release|debug) {
    LIBS += -lZSSysd
    LIBS += -lZSTestd
    LIBS += -lZSPhysVald
    LIBS += -lZSPhysSizesElectricityd
    LIBS += -lZSPhysSizesGeometryd
    LIBS += -lZSPhysSizesKinematicsd
    LIBS += -lZSSysGUId
    LIBS += -lZSTestGUId
    LIBS += -lZSPhysValGUId
}

SOURCES += \
    ../../../../Source/Apps/Test/ZSPhysVal/Main.cpp \
    ../../../../Source/Apps/Test/ZSPhysVal/WidgetCentral.cpp \
    ../../../../Source/Apps/Test/ZSPhysVal/UnitViewWdgts.cpp \
    ../../../../Source/Apps/Test/ZSPhysVal/UnitsWdgt.cpp \
    ../../../../Source/Apps/Test/ZSPhysVal/UnitModels.cpp \
    ../../../../Source/Apps/Test/ZSPhysVal/Test.cpp \
    ../../../../Source/Apps/Test/ZSPhysVal/MainWindow.cpp \
    ../../../../Source/Apps/Test/ZSPhysVal/App.cpp

HEADERS += \
    ../../../../Include/Apps/Test/ZSPhysVal/App.h \
    ../../../../Include/Apps/Test/ZSPhysVal/WidgetCentral.h \
    ../../../../Include/Apps/Test/ZSPhysVal/UnitViewWdgts.h \
    ../../../../Include/Apps/Test/ZSPhysVal/UnitsWdgt.h \
    ../../../../Include/Apps/Test/ZSPhysVal/UnitModels.h \
    ../../../../Include/Apps/Test/ZSPhysVal/Test.h \
    ../../../../Include/Apps/Test/ZSPhysVal/MainWindow.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/bin
    } else {
        target.path = /usr/bin
    }
    INSTALLS += target
}
