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
    COMPILERLIBINFIX = msvc2019
    PLATFORM = x64
}
win32-msvc2013 {
    message("win32-msvc2013") # not executed !!!
}
win32-msvc2015 {
    message("win32-msvc2015") # not executed !!!
}
win32-msvc2017 {
    message("win32-msvc2017") # not executed !!!
}
win32-msvc2019 {
    message("win32-msvc2019") # not executed !!!
}
linux {
    DEFINES += __linux__
    COMPILERLIBINFIX = linux # should become gcc whatever
    PLATFORM = x64
}

CONFIG(release, release|debug) {
    TARGET = $$MODULE"Qt"$$QT_MAJOR_VERSION"_"$$COMPILERLIBINFIX"_"$$PLATFORM
    MOC_DIR = ../../../Tmp/Moc/Libs/$$MODULE/Release_$$COMPILERLIBINFIX/$$PLATFORM
    OBJECTS_DIR = ../../../Tmp/Int/Libs/$$MODULE/Release_$$COMPILERLIBINFIX/$$PLATFORM
}
CONFIG(debug, release|debug) {
    TARGET = $$MODULE"Qt"$$QT_MAJOR_VERSION"_"$$COMPILERLIBINFIX"_"$$PLATFORM"_d"
    MOC_DIR = ../../../Tmp/Moc/Libs/$$MODULE/Debug_$$COMPILERLIBINFIX/$$PLATFORM
    OBJECTS_DIR = ../../../Tmp/Int/Libs/$$MODULE/Debug_$$COMPILERLIBINFIX/$$PLATFORM
}
DESTDIR = ../../../Lib/$$COMPILERLIBINFIX"_"$$PLATFORM

CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
}
DEFINES += _ZSTRACE
DEFINES += ZSSYSDLL_EXPORTS

INCLUDEPATH += ../../../Include/Libs

SOURCES += \
    ../../../Source/Libs/ZSSys/ZSSysEnumTemplate.cpp \
    ../../../Source/Libs/ZSSys/ZSSysTrcServer.cpp \
    ../../../Source/Libs/ZSSys/ZSSysTrcMthFile.cpp \
    ../../../Source/Libs/ZSSys/ZSSysTrcMethod.cpp \
    ../../../Source/Libs/ZSSys/ZSSysTrcAdminObjIdxTree.cpp \
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
    ../../../Source/Libs/ZSSys/ZSSysEnumerationIdxTree.cpp \
    ../../../Source/Libs/ZSSys/ZSSysEnumeration.cpp \
    ../../../Source/Libs/ZSSys/ZSSysEnumEntry.cpp \
    ../../../Source/Libs/ZSSys/ZSSysDllMain.cpp \
    ../../../Source/Libs/ZSSys/ZSSysCommon.cpp \
    ../../../Source/Libs/ZSSys/ZSSysAux.cpp \
    ../../../Source/Libs/ZSSys/ZSSysApp.cpp

HEADERS += \
    ../../../Include/Libs/ZSSys/ZSSysVersion.h \
    ../../../Include/Libs/ZSSys/ZSSysTrcServer.h \
    ../../../Include/Libs/ZSSys/ZSSysTrcMthFile.h \
    ../../../Include/Libs/ZSSys/ZSSysTrcMethod.h \
    ../../../Include/Libs/ZSSys/ZSSysTrcAdminObjIdxTree.h \
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
    ../../../Include/Libs/ZSSys/ZSSysEnumeration.h \
    ../../../Include/Libs/ZSSys/ZSSysEnumEntry.h \
    ../../../Include/Libs/ZSSys/ZSSysEnumTemplate.h \
    ../../../Include/Libs/ZSSys/ZSSysDllMain.h \
    ../../../Include/Libs/ZSSys/ZSSysCommon.h \
    ../../../Include/Libs/ZSSys/ZSSysAux.h \
    ../../../Include/Libs/ZSSys/ZSSysApp.h

windows {
    SOURCEDIR = ..\..\..\Lib\\$$COMPILERLIBINFIX"_"$$PLATFORM
    TARGETDIR = ..\..\..\Bin\\$$COMPILERLIBINFIX"_"$$PLATFORM
    QMAKE_POST_LINK=copy /Y $$SOURCEDIR"\\"$$TARGET".dll" $$TARGETDIR"\\*.dll"
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
