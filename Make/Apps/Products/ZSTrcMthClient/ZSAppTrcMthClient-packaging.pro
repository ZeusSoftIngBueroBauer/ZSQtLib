TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += ../../../Libs/ZSSys
ZSSys.depends =

SUBDIRS += ../../../Libs/ZSSysGUI
ZSSysGUI.depends = ZSSys

SUBDIRS += ../../../Libs/ZSIpc
ZSIpc.depends = ZSSys

SUBDIRS += ../../../Libs/ZSIpcGUI
ZSIpcGUI.depends = ZSSys ZSIpc ZSSysGUI

SUBDIRS += ../../../Libs/ZSIpcTrace
ZSIpcTrace.depends = ZSSys ZSIpc

SUBDIRS += ../../../Libs/ZSIpcTraceGUI
ZSIpcTraceGUI.depends = ZSSys ZSIpc ZSIpcTrace ZSSysGUI ZSIpcGUI

SUBDIRS += ../../../Apps/Products/ZSTrcMthClient/ZSAppTrcMthClient.pro
ZSAppTrcMthClient.depends = ZSSys ZSTest ZSIpc ZSTrace ZSSysGUI ZSTestGUI ZSIpcGUI ZSIpcTraceGUI
