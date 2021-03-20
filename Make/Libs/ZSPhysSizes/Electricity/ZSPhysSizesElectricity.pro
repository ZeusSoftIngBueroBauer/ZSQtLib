#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T13:06:39
#
#-------------------------------------------------

QT += network xml
QT -= gui

MODULE = ZSPhysSizesElectricity

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
DEFINES += ZSPHYSSIZESELECTRICITYDLL_EXPORTS

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
    ../../../../Source/Libs/ZSPhysSizes/Electricity/ZSPhysSizesElectricityDllMain.cpp \
    ../../../../Source/Libs/ZSPhysSizes/Electricity/ZSPhysSizeVoltage.cpp \
    ../../../../Source/Libs/ZSPhysSizes/Electricity/ZSPhysSizes.cpp \
    ../../../../Source/Libs/ZSPhysSizes/Electricity/ZSPhysSizeResistance.cpp \
    ../../../../Source/Libs/ZSPhysSizes/Electricity/ZSPhysSizePower.cpp \
    ../../../../Source/Libs/ZSPhysSizes/Electricity/ZSPhysSizeCurrent.cpp

HEADERS += \
    ../../../../Include/Libs/ZSPhysSizes/Electricity/ZSPhysSizesElectricityDllMain.h \
    ../../../../Include/Libs/ZSPhysSizes/Electricity/ZSPhysSizeVoltage.h \
    ../../../../Include/Libs/ZSPhysSizes/Electricity/ZSPhysSizes.h \
    ../../../../Include/Libs/ZSPhysSizes/Electricity/ZSPhysSizeResistance.h \
    ../../../../Include/Libs/ZSPhysSizes/Electricity/ZSPhysSizePower.h \
    ../../../../Include/Libs/ZSPhysSizes/Electricity/ZSPhysSizeCurrent.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
