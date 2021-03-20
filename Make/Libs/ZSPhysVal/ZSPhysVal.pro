#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T13:06:39
#
#-------------------------------------------------

QT += network xml
QT -= gui

MODULE = ZSPhysVal

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
    MOC_DIR = ../../../Tmp/Moc/Libs/$$MODULE/$$PLATFORM/Release
    OBJECTS_DIR = ../../../Tmp/Int/Libs/$$MODULE/$$PLATFORM/Release
}
CONFIG(debug, release|debug) {
    TARGET = $$MODULE"d"
    MOC_DIR = ../../../Tmp/Moc/Libs/$$MODULE/$$PLATFORM/Debug
    OBJECTS_DIR = ../../../Tmp/Int/Libs/$$MODULE/$$PLATFORM/Debug
}
DESTDIR = ../../../Lib/$$PLATFORM

CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
}
DEFINES += _ZSTRACE
DEFINES += ZSPHYSVALDLL_EXPORTS

INCLUDEPATH += ../../../Include/Libs

LIBS += -L../../../Lib/$$PLATFORM

CONFIG(release, release|debug) {
    LIBS += -lZSSys
}
CONFIG(debug, release|debug) {
    LIBS += -lZSSysd
}

SOURCES += \
    ../../../Source/Libs/ZSPhysVal/ZSPhysValDllMain.cpp \
    ../../../Source/Libs/ZSPhysVal/ZSPhysValRes.cpp \
    ../../../Source/Libs/ZSPhysVal/ZSPhysValExceptions.cpp \
    ../../../Source/Libs/ZSPhysVal/ZSPhysValArr.cpp \
    ../../../Source/Libs/ZSPhysVal/ZSPhysVal.cpp \
    ../../../Source/Libs/ZSPhysVal/ZSPhysUnitsRatio.cpp \
    ../../../Source/Libs/ZSPhysVal/ZSPhysUnitsPool.cpp \
    ../../../Source/Libs/ZSPhysVal/ZSPhysUnitsGrp.cpp \
    ../../../Source/Libs/ZSPhysVal/ZSPhysUnitsFctConvert.cpp \
    ../../../Source/Libs/ZSPhysVal/ZSPhysUnitsDataQuantity.cpp \
    ../../../Source/Libs/ZSPhysVal/ZSPhysUnits.cpp \
    ../../../Source/Libs/ZSPhysVal/ZSPhysSize.cpp

HEADERS += \
    ../../../Include/Libs/ZSPhysVal/ZSPhysValDllMain.h \
    ../../../Include/Libs/ZSPhysVal/ZSPhysValRes.h \
    ../../../Include/Libs/ZSPhysVal/ZSPhysValExceptions.h \
    ../../../Include/Libs/ZSPhysVal/ZSPhysValArr.h \
    ../../../Include/Libs/ZSPhysVal/ZSPhysVal.h \
    ../../../Include/Libs/ZSPhysVal/ZSPhysUnitsRatio.h \
    ../../../Include/Libs/ZSPhysVal/ZSPhysUnitsPool.h \
    ../../../Include/Libs/ZSPhysVal/ZSPhysUnitsGrp.h \
    ../../../Include/Libs/ZSPhysVal/ZSPhysUnitsFctConvert.h \
    ../../../Include/Libs/ZSPhysVal/ZSPhysUnitsDataQuantity.h \
    ../../../Include/Libs/ZSPhysVal/ZSPhysUnits.h \
    ../../../Include/Libs/ZSPhysVal/ZSPhysSize.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
