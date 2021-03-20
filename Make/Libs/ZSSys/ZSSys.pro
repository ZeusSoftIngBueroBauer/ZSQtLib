#-------------------------------------------------
#
# Project created by QtCreator 2018-04-03T14:47:16
#
#-------------------------------------------------

QT += xml
QT -= gui

MODULE = ZSSys

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
DEFINES += ZSSYSDLL_EXPORTS

INCLUDEPATH += ../../../Include/Libs

SOURCES += \
    ../../../Source/Libs/ZSSys/ZSSysTrcServer.cpp \
    ../../../Source/Libs/ZSSys/ZSSysTrcMthFile.cpp \
    ../../../Source/Libs/ZSSys/ZSSysTrcMethod.cpp \
    ../../../Source/Libs/ZSSys/ZSSysTrcData.cpp \
    ../../../Source/Libs/ZSSys/ZSSysTrcAdminObjPool.cpp \
    ../../../Source/Libs/ZSSys/ZSSysTrcAdminObj.cpp \
    ../../../Source/Libs/ZSSys/ZSSysTime.cpp \
    ../../../Source/Libs/ZSSys/ZSSysSleeperThread.cpp \
    ../../../Source/Libs/ZSSys/ZSSysRequestSequencer.cpp \
    ../../../Source/Libs/ZSSys/ZSSysRequestQueue.cpp \
    ../../../Source/Libs/ZSSys/ZSSysRequestExecTree.cpp \
    ../../../Source/Libs/ZSSys/ZSSysRequest.cpp \
    ../../../Source/Libs/ZSSys/ZSSysObjPool.cpp \
    ../../../Source/Libs/ZSSys/ZSSysMsgProtocol.cpp \
    ../../../Source/Libs/ZSSys/ZSSysMsg.cpp \
    ../../../Source/Libs/ZSSys/ZSSysMemLeakDump.cpp \
    ../../../Source/Libs/ZSSys/ZSSysMath.cpp \
    ../../../Source/Libs/ZSSys/ZSSysLogFile.cpp \
    ../../../Source/Libs/ZSSys/ZSSysIdxTree.cpp \
    ../../../Source/Libs/ZSSys/ZSSysIdxTreeEntries.cpp \
    ../../../Source/Libs/ZSSys/ZSSysException.cpp \
    ../../../Source/Libs/ZSSys/ZSSysErrResult.cpp \
    ../../../Source/Libs/ZSSys/ZSSysErrLog.cpp \
    ../../../Source/Libs/ZSSys/ZSSysErrCode.cpp \
    ../../../Source/Libs/ZSSys/ZSSysEnumValArr.cpp \
    ../../../Source/Libs/ZSSys/ZSSysEnumVal.cpp \
    ../../../Source/Libs/ZSSys/ZSSysEnum.cpp \
    ../../../Source/Libs/ZSSys/ZSSysDllMain.cpp \
    ../../../Source/Libs/ZSSys/ZSSysAux.cpp \
    ../../../Source/Libs/ZSSys/ZSSysApp.cpp

HEADERS += \
    ../../../Include/Libs/ZSSys/ZSSysVersion.h \
    ../../../Include/Libs/ZSSys/ZSSysTrcServer.h \
    ../../../Include/Libs/ZSSys/ZSSysTrcMthFile.h \
    ../../../Include/Libs/ZSSys/ZSSysTrcMethod.h \
    ../../../Include/Libs/ZSSys/ZSSysTrcData.h \
    ../../../Include/Libs/ZSSys/ZSSysTrcAdminObjPool.h \
    ../../../Include/Libs/ZSSys/ZSSysTrcAdminObj.h \
    ../../../Include/Libs/ZSSys/ZSSysTime.h \
    ../../../Include/Libs/ZSSys/ZSSysSleeperThread.h \
    ../../../Include/Libs/ZSSys/ZSSysRequestSequencer.h \
    ../../../Include/Libs/ZSSys/ZSSysRequestQueue.h \
    ../../../Include/Libs/ZSSys/ZSSysRequestExecTree.h \
    ../../../Include/Libs/ZSSys/ZSSysRequest.h \
    ../../../Include/Libs/ZSSys/ZSSysObjPool.h \
    ../../../Include/Libs/ZSSys/ZSSysMsgProtocol.h \
    ../../../Include/Libs/ZSSys/ZSSysMsg.h \
    ../../../Include/Libs/ZSSys/ZSSysMemLeakDump.h \
    ../../../Include/Libs/ZSSys/ZSSysMath.h \
    ../../../Include/Libs/ZSSys/ZSSysLogFile.h \
    ../../../Include/Libs/ZSSys/ZSSysIdxTree.h \
    ../../../Include/Libs/ZSSys/ZSSysIdxTreeEntries.h \
    ../../../Include/Libs/ZSSys/ZSSysException.h \
    ../../../Include/Libs/ZSSys/ZSSysErrResult.h \
    ../../../Include/Libs/ZSSys/ZSSysErrLog.h \
    ../../../Include/Libs/ZSSys/ZSSysErrCode.h \
    ../../../Include/Libs/ZSSys/ZSSysEnumValArr.h \
    ../../../Include/Libs/ZSSys/ZSSysEnumVal.h \
    ../../../Include/Libs/ZSSys/ZSSysEnum.h \
    ../../../Include/Libs/ZSSys/ZSSysDllMain.h \
    ../../../Include/Libs/ZSSys/ZSSysAux.h \
    ../../../Include/Libs/ZSSys/ZSSysApp.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
