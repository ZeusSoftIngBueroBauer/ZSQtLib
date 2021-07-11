#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T13:06:39
#
#-------------------------------------------------

QT += network xml
QT -= gui

MODULE = ZSIpc

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
    COMPILERLIBINFIX = "linux" # should become gcc whatever
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
DEFINES += ZSIPCDLL_EXPORTS

INCLUDEPATH += ../../../Include/Libs

LIBS += -L../../../Lib/$$COMPILERLIBINFIX"_"$$PLATFORM

CONFIG(release, release|debug) {
    LIBS += -lZSSys"Qt"$$QT_MAJOR_VERSION"_"$$COMPILERLIBINFIX"_"$$PLATFORM
}
CONFIG(debug, release|debug) {
    LIBS += -lZSSys"Qt"$$QT_MAJOR_VERSION"_"$$COMPILERLIBINFIX"_"$$PLATFORM"_d"
}

SOURCES += \
    ../../../Source/Libs/ZSIpc/ZSIpcSrvCltMsg.cpp \
    ../../../Source/Libs/ZSIpc/ZSIpcSrvCltBaseGateway.cpp \
    ../../../Source/Libs/ZSIpc/ZSIpcSocketWrapperTcp.cpp \
    ../../../Source/Libs/ZSIpc/ZSIpcSocketWrapperShm.cpp \
    ../../../Source/Libs/ZSIpc/ZSIpcSocketWrapperInProcMsg.cpp \
    ../../../Source/Libs/ZSIpc/ZSIpcSocketWrapper.cpp \
    ../../../Source/Libs/ZSIpc/ZSIpcServerWrapperTcp.cpp \
    ../../../Source/Libs/ZSIpc/ZSIpcServerWrapperShm.cpp \
    ../../../Source/Libs/ZSIpc/ZSIpcServerWrapperInProcMsg.cpp \
    ../../../Source/Libs/ZSIpc/ZSIpcServerWrapper.cpp \
    ../../../Source/Libs/ZSIpc/ZSIpcServerGateway.cpp \
    ../../../Source/Libs/ZSIpc/ZSIpcServer.cpp \
    ../../../Source/Libs/ZSIpc/ZSIpcInProcMsgSocket.cpp \
    ../../../Source/Libs/ZSIpc/ZSIpcInProcMsgServer.cpp \
    ../../../Source/Libs/ZSIpc/ZSIpcDllMain.cpp \
    ../../../Source/Libs/ZSIpc/ZSIpcCommon.cpp \
    ../../../Source/Libs/ZSIpc/ZSIpcClientGateway.cpp \
    ../../../Source/Libs/ZSIpc/ZSIpcClient.cpp \
    ../../../Source/Libs/ZSIpc/ZSIpcBlkTypeUser.cpp \
    ../../../Source/Libs/ZSIpc/ZSIpcBlkTypeTLV.cpp \
    ../../../Source/Libs/ZSIpc/ZSIpcBlkTypeSL.cpp \
    ../../../Source/Libs/ZSIpc/ZSIpcBlkTypeNone.cpp \
    ../../../Source/Libs/ZSIpc/ZSIpcBlkTypeL.cpp \
    ../../../Source/Libs/ZSIpc/ZSIpcBlkTypeE.cpp \
    ../../../Source/Libs/ZSIpc/ZSIpcBlkType.cpp

HEADERS += \
    ../../../Include/Libs/ZSIpc/ZSIpcSrvCltMsg.h \
    ../../../Include/Libs/ZSIpc/ZSIpcSrvCltBaseGateway.h \
    ../../../Include/Libs/ZSIpc/ZSIpcSocketWrapperTcp.h \
    ../../../Include/Libs/ZSIpc/ZSIpcSocketWrapperShm.h \
    ../../../Include/Libs/ZSIpc/ZSIpcSocketWrapperInProcMsg.h \
    ../../../Include/Libs/ZSIpc/ZSIpcSocketWrapper.h \
    ../../../Include/Libs/ZSIpc/ZSIpcServerWrapperTcp.h \
    ../../../Include/Libs/ZSIpc/ZSIpcServerWrapperShm.h \
    ../../../Include/Libs/ZSIpc/ZSIpcServerWrapperInProcMsg.h \
    ../../../Include/Libs/ZSIpc/ZSIpcServerWrapper.h \
    ../../../Include/Libs/ZSIpc/ZSIpcServerGateway.h \
    ../../../Include/Libs/ZSIpc/ZSIpcServer.h \
    ../../../Include/Libs/ZSIpc/ZSIpcInProcMsgSocket.h \
    ../../../Include/Libs/ZSIpc/ZSIpcInProcMsgServer.h \
    ../../../Include/Libs/ZSIpc/ZSIpcCommon.h \
    ../../../Include/Libs/ZSIpc/ZSIpcDllMain.h \
    ../../../Include/Libs/ZSIpc/ZSIpcClientGateway.h \
    ../../../Include/Libs/ZSIpc/ZSIpcClient.h \
    ../../../Include/Libs/ZSIpc/ZSIpcBlkTypeUser.h \
    ../../../Include/Libs/ZSIpc/ZSIpcBlkTypeTLV.h \
    ../../../Include/Libs/ZSIpc/ZSIpcBlkTypeSL.h \
    ../../../Include/Libs/ZSIpc/ZSIpcBlkTypeNone.h \
    ../../../Include/Libs/ZSIpc/ZSIpcBlkTypeL.h \
    ../../../Include/Libs/ZSIpc/ZSIpcBlkTypeE.h \
    ../../../Include/Libs/ZSIpc/ZSIpcBlkType.h

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
