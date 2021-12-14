#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T13:06:39
#
#-------------------------------------------------

QT += network xml
QT -= gui

MODULE = ZSPhysSizesGeometry

VERSION = 4.6.3

TEMPLATE = lib

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
    MOC_DIR = ../../../../Tmp/Moc/Libs/$$MODULE/$$PLATFORM/Release
    OBJECTS_DIR = ../../../../Tmp/Int/Libs/$$MODULE/$$PLATFORM/Release
}
CONFIG(debug, release|debug) {
    TARGET = $$MODULE"d"
    MOC_DIR = ../../../../Tmp/Moc/Libs/$$MODULE/$$PLATFORM/Debug
    OBJECTS_DIR = ../../../../Tmp/Int/Libs/$$MODULE/$$PLATFORM/Debug
}
DESTDIR = ../../../../Lib/$$PLATFORM

CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
}
DEFINES += _ZSTRACE
DEFINES += ZSPHYSSIZESGEOMETRYDLL_EXPORTS

INCLUDEPATH += ../../../../Include/Libs

LIBS += -L../../../../Lib/$$PLATFORM

CONFIG(release, release|debug) {
    LIBS += -lZSSys
    LIBS += -lZSPhysVal
}
CONFIG(debug, release|debug) {
    LIBS += -lZSSysd
    LIBS += -lZSPhysVald
}

SOURCES += \
    ../../../../Source/Libs/ZSPhysSizes/Geometry/ZSPhysSizesGeometryDllMain.cpp \
    ../../../../Source/Libs/ZSPhysSizes/Geometry/ZSPhysSizes.cpp \
    ../../../../Source/Libs/ZSPhysSizes/Geometry/ZSPhysSizeLength.cpp \
    ../../../../Source/Libs/ZSPhysSizes/Geometry/ZSPhysSizeGraphDevice.cpp \
    ../../../../Source/Libs/ZSPhysSizes/Geometry/ZSPhysSizeAngle.cpp

HEADERS += \
    ../../../../Include/Libs/ZSPhysSizes/Geometry/ZSPhysSizesGeometryDllMain.h \
    ../../../../Include/Libs/ZSPhysSizes/Geometry/ZSPhysSizes.h \
    ../../../../Include/Libs/ZSPhysSizes/Geometry/ZSPhysSizeLength.h \
    ../../../../Include/Libs/ZSPhysSizes/Geometry/ZSPhysSizeGraphDevice.h \
    ../../../../Include/Libs/ZSPhysSizes/Geometry/ZSPhysSizeAngle.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
