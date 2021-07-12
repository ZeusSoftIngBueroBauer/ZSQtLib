#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T12:36:25
#
#-------------------------------------------------

QT += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

MODULE = ZSSysGUI

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
    COMPILERLIBINFIX = gcc
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
DEFINES += ZSSYSGUIDLL_EXPORTS

INCLUDEPATH += ../../../Include/Libs

LIBS += -L../../../Lib/$$COMPILERLIBINFIX"_"$$PLATFORM

CONFIG(release, release|debug) {
    LIBS += -lZSSys"Qt"$$QT_MAJOR_VERSION"_"$$COMPILERLIBINFIX"_"$$PLATFORM
}
CONFIG(debug, release|debug) {
    LIBS += -lZSSys"Qt"$$QT_MAJOR_VERSION"_"$$COMPILERLIBINFIX"_"$$PLATFORM"_d"
}

SOURCES += \
    ../../../Source/Libs/ZSSysGUI/ZSSysTrcAdminObjIdxTreeWdgt.cpp \
    ../../../Source/Libs/ZSSysGUI/ZSSysTrcAdminObjIdxTreeView.cpp \
    ../../../Source/Libs/ZSSysGUI/ZSSysTrcAdminObjIdxTreeModel.cpp \
    ../../../Source/Libs/ZSSysGUI/ZSSysTrcAdminObjIdxTreeDlg.cpp \
    ../../../Source/Libs/ZSSysGUI/ZSSysSepLine.cpp \
    ../../../Source/Libs/ZSSysGUI/ZSSysRequestSequencerWdgt.cpp \
    ../../../Source/Libs/ZSSysGUI/ZSSysRequestSequencerModel.cpp \
    ../../../Source/Libs/ZSSysGUI/ZSSysRequestSequencerDlg.cpp \
    ../../../Source/Libs/ZSSysGUI/ZSSysRequestExecTreeWdgt.cpp \
    ../../../Source/Libs/ZSSysGUI/ZSSysRequestExecTreeModel.cpp \
    ../../../Source/Libs/ZSSysGUI/ZSSysRequestExecTreeDlg.cpp \
    ../../../Source/Libs/ZSSysGUI/ZSSysPushButtonCloseWidget.cpp \
    ../../../Source/Libs/ZSSysGUI/ZSSysPushButton.cpp \
    ../../../Source/Libs/ZSSysGUI/ZSSysProgressBar.cpp \
    ../../../Source/Libs/ZSSysGUI/ZSSysObjPoolModel.cpp \
    ../../../Source/Libs/ZSSysGUI/ZSSysIdxTreeModel.cpp \
    ../../../Source/Libs/ZSSysGUI/ZSSysIdxTreeModelEntries.cpp \
    ../../../Source/Libs/ZSSysGUI/ZSSysIdxTreeModelBranchContent.cpp \
    ../../../Source/Libs/ZSSysGUI/ZSSysIdxTreeView.cpp \
    ../../../Source/Libs/ZSSysGUI/ZSSysIdxTreeTableViewBranchContent.cpp \
    ../../../Source/Libs/ZSSysGUI/ZSSysIdxTreeWdgt.cpp \
    ../../../Source/Libs/ZSSysGUI/ZSSysGUIDllMain.cpp \
    ../../../Source/Libs/ZSSysGUI/ZSSysGUIAux.cpp \
    ../../../Source/Libs/ZSSysGUI/ZSSysGUIApp.cpp \
    ../../../Source/Libs/ZSSysGUI/ZSSysFindTextDlg.cpp \
    ../../../Source/Libs/ZSSysGUI/ZSSysErrLogWdgt.cpp \
    ../../../Source/Libs/ZSSysGUI/ZSSysErrLogModel.cpp \
    ../../../Source/Libs/ZSSysGUI/ZSSysErrLogDlg.cpp \
    ../../../Source/Libs/ZSSysGUI/ZSSysErrDlg.cpp \
    ../../../Source/Libs/ZSSysGUI/ZSSysDialog.cpp

HEADERS += \
    ../../../Include/Libs/ZSSysGUI/ZSSysTrcAdminObjIdxTreeWdgt.h \
    ../../../Include/Libs/ZSSysGUI/ZSSysTrcAdminObjIdxTreeView.h \
    ../../../Include/Libs/ZSSysGUI/ZSSysTrcAdminObjIdxTreeModel.h \
    ../../../Include/Libs/ZSSysGUI/ZSSysTrcAdminObjIdxTreeDlg.h \
    ../../../Include/Libs/ZSSysGUI/ZSSysSepLine.h \
    ../../../Include/Libs/ZSSysGUI/ZSSysRequestSequencerWdgt.h \
    ../../../Include/Libs/ZSSysGUI/ZSSysRequestSequencerModel.h \
    ../../../Include/Libs/ZSSysGUI/ZSSysRequestSequencerDlg.h \
    ../../../Include/Libs/ZSSysGUI/ZSSysRequestExecTreeWdgt.h \
    ../../../Include/Libs/ZSSysGUI/ZSSysRequestExecTreeModel.h \
    ../../../Include/Libs/ZSSysGUI/ZSSysRequestExecTreeDlg.h \
    ../../../Include/Libs/ZSSysGUI/ZSSysPushButtonCloseWidget.h \
    ../../../Include/Libs/ZSSysGUI/ZSSysPushButton.h \
    ../../../Include/Libs/ZSSysGUI/ZSSysProgressBar.h \
    ../../../Include/Libs/ZSSysGUI/ZSSysObjPoolModel.h \
    ../../../Include/Libs/ZSSysGUI/ZSSysIdxTreeModel.h \
    ../../../Include/Libs/ZSSysGUI/ZSSysIdxTreeModelEntries.h \
    ../../../Include/Libs/ZSSysGUI/ZSSysIdxTreeModelBranchContent.h \
    ../../../Include/Libs/ZSSysGUI/ZSSysIdxTreeView.h \
    ../../../Include/Libs/ZSSysGUI/ZSSysIdxTreeTableViewBranchContent.h \
    ../../../Include/Libs/ZSSysGUI/ZSSysIdxTreeWdgt.h \
    ../../../Include/Libs/ZSSysGUI/ZSSysGUIDllMain.h \
    ../../../Include/Libs/ZSSysGUI/ZSSysGUIAux.h \
    ../../../Include/Libs/ZSSysGUI/ZSSysGUIApp.h \
    ../../../Include/Libs/ZSSysGUI/ZSSysFindTextDlg.h \
    ../../../Include/Libs/ZSSysGUI/ZSSysErrLogWdgt.h \
    ../../../Include/Libs/ZSSysGUI/ZSSysErrLogModel.h \
    ../../../Include/Libs/ZSSysGUI/ZSSysErrLogDlg.h \
    ../../../Include/Libs/ZSSysGUI/ZSSysErrDlg.h \
    ../../../Include/Libs/ZSSysGUI/ZSSysDialog.h

RESOURCES = ../../../Images/Libs/ZSSys/ZSSys.qrc

windows {
    SOURCEDIR = ..\..\..\Lib\\$$COMPILERLIBINFIX"_"$$PLATFORM
    TARGETDIR = ..\..\..\Bin\\$$COMPILERLIBINFIX"_"$$PLATFORM
    QMAKE_POST_LINK=copy /Y $$SOURCEDIR"\\"$$TARGET".dll" $$TARGETDIR"\\*.dll"
}
linux {
    SOURCEDIR = ../../../Lib/$$COMPILERLIBINFIX"_"$$PLATFORM
    TARGETDIR = ../../../Bin/$$COMPILERLIBINFIX"_"$$PLATFORM
    QMAKE_POST_LINK=cp $$SOURCEDIR"/lib"$$TARGET"*.so*" $$TARGETDIR
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
