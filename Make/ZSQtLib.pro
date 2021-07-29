TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += ./Libs/ZSSys
ZSSys.depends =

SUBDIRS += ./Libs/ZSTest
ZSTest.depends = ZSSys

SUBDIRS += ./Libs/ZSIpc
ZSIpc.depends = ZSSys

SUBDIRS += ./Libs/ZSIpcTrace
ZSIpcTrace.depends = ZSSys ZSIpc

SUBDIRS += ./Libs/ZSSysGUI
ZSSysGUI.depends = ZSSys

SUBDIRS += ./Libs/ZSTestGUI
ZSTestGUI.depends =  ZSSys ZSTest ZSSysGUI

SUBDIRS += ./Libs/ZSIpcGUI
ZSIpcGUI.depends = ZSSys ZSIpc ZSSysGUI

SUBDIRS += ./Libs/ZSIpcTraceGUI
ZSIpcTraceGUI.depends = ZSSys ZSIpc ZSIpcTrace ZSSysGUI ZSIpcGUI

SUBDIRS += ./Apps/Test/ZSIpc/ZSAppTestIpc.pro
ZSAppTestIpc.depends = ZSSys ZSTest ZSIpc ZSIpcTrace ZSSysGUI ZSTestGUI ZSIpcGUI ZSIpcTraceGUI

SUBDIRS += ./Apps/Test/ZSIpcTrace/ZSAppTestIpcTrace.pro
ZSAppTestIpcTrace.depends = ZSSys ZSTest ZSIpc ZSIpcTrace ZSSysGUI ZSTestGUI ZSIpcGUI ZSIpcTraceGUI

#Removed as currently not compilable on linux ubuntu 18.04.5 LTS:
#  usr/bin/ld: ../../../../Tmp/Int/Apps/Test/ZSAppTestIpcTraceDllIfQtApp/Linux/Debug/ZSIpcTrcDllIf.o: undefined reference to symbol 'dlclose@@GLIBC_2.2.5'
#  //lib/x86_64-linux-gnu/libdl.so.2: error adding symbols: DSO missing from command line
#SUBDIRS += ./Apps/Test/ZSIpcTraceDllIfQtApp/ZSAppTestIpcTraceDllIfQtApp.pro
#ZSAppTestIpcDllIfQtApp.depends = ZSSys ZSTest ZSIpc ZSIpcTrace ZSSysGUI ZSTestGUI ZSIpcGUI ZSIpcTraceGUI

SUBDIRS += ./Apps/Test/ZSSysEnums/ZSAppTestSysEnums.pro
ZSAppTestSysEnums.depends = ZSSys ZSTest ZSIpc ZSIpcTrace ZSSysGUI ZSTestGUI ZSIpcGUI ZSIpcTraceGUI

SUBDIRS += ./Apps/Test/ZSSysErrLog/ZSAppTestSysErrLog.pro
ZSAppTestSysErrLog.depends = ZSSys ZSTest ZSIpc ZSIpcTrace ZSSysGUI ZSTestGUI ZSIpcGUI ZSIpcTraceGUI

SUBDIRS += ./Apps/Test/ZSSysIdxTree/ZSAppTestSysIdxTree.pro
ZSAppTestSysIdxTree.depends = ZSSys ZSTest ZSIpc ZSIpcTrace ZSSysGUI ZSTestGUI ZSIpcGUI ZSIpcTraceGUI

SUBDIRS += ./Apps/Test/ZSSysLogFile/ZSAppTestSysLogFile.pro
ZSAppTestSysLogFile.depends = ZSSys ZSTest ZSIpc ZSIpcTrace ZSSysGUI ZSTestGUI ZSIpcGUI ZSIpcTraceGUI

SUBDIRS += ./Apps/Test/ZSTestTemplate/ZSAppTestTemplate.pro
ZSAppTestTemplate.depends = ZSSys ZSTest ZSSysGUI ZSTestGUI

SUBDIRS += ./Apps/Products/ZSTrcMthClient/ZSAppTrcMthClient.pro
ZSAppTrcMthClient.depends = ZSSys ZSTest ZSIpc ZSTrace ZSSysGUI ZSTestGUI ZSIpcGUI ZSIpcTraceGUI

