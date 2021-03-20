#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T13:06:39
#
#-------------------------------------------------

QT += network xml
QT -= gui

MODULE = ZSPhysSizesKinematics

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
DEFINES += ZSPHYSSIZESKINEMATICSDLL_EXPORTS

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
    ../../../../Source/Libs/ZSPhysSizes/Kinematics/ZSPhysSizesKinematicsDllMain.cpp \
    ../../../../Source/Libs/ZSPhysSizes/Kinematics/ZSPhysSizeVelocity.cpp \
    ../../../../Source/Libs/ZSPhysSizes/Kinematics/ZSPhysSizeTime.cpp \
    ../../../../Source/Libs/ZSPhysSizes/Kinematics/ZSPhysSizeSamples.cpp \
    ../../../../Source/Libs/ZSPhysSizes/Kinematics/ZSPhysSizes.cpp \
    ../../../../Source/Libs/ZSPhysSizes/Kinematics/ZSPhysSizeFrequency.cpp \
    ../../../../Source/Libs/ZSPhysSizes/Kinematics/ZSPhysSizeBitRate.cpp

HEADERS += \
    ../../../../Include/Libs/ZSPhysSizes/Kinematics/ZSPhysSizesKinematicsDllMain.h \
    ../../../../Include/Libs/ZSPhysSizes/Kinematics/ZSPhysSizeVelocity.h \
    ../../../../Include/Libs/ZSPhysSizes/Kinematics/ZSPhysSizeTime.h \
    ../../../../Include/Libs/ZSPhysSizes/Kinematics/ZSPhysSizeSamples.h \
    ../../../../Include/Libs/ZSPhysSizes/Kinematics/ZSPhysSizes.h \
    ../../../../Include/Libs/ZSPhysSizes/Kinematics/ZSPhysSizeFrequency.h \
    ../../../../Include/Libs/ZSPhysSizes/Kinematics/ZSPhysSizeBitRate.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
