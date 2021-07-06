TEMPLATE = subdirs

#ZSSys.subdir = ./Libs/ZSSys
#ZSTest.subdir = ./Libs/ZSTest
#ZSPhysVal.subdir = ./Libs/ZSPhysVal
#ZSPhysSizesElectricity.subdir = ./Libs/ZSPhysSizes/Electricity/ZSPhysSizesElectricity.pro
#ZSPhysSizesGeometry.subdir = ./Libs/ZSPhysSizes/Geometry/ZSPhysSizesGeometry.pro
#ZSPhysSizesKinematics.subdir = ./Libs/ZSPhysSizes/Kinematics/ZSPhysSizesKinematics.pro
#ZSIpc.subdir = ./Libs/ZSIpc
#ZSTrace.subdir = ./Libs/ZSTrace
#ZSDiagram.subdir = ./Libs/ZSDiagram
#ZSDraw.subdir = ./Libs/ZSDraw
#ZSRemCmd.subdir = ./Libs/ZSRemCmd
#ZSData_old.subdir = ./Libs/ZSData.bak/ZSData_old.pro
#ZSData.subdir = ./Libs/ZSData
#ZSSysGUI.subdir = ./Libs/ZSSysGUI
#ZSTestGUI.subdir = ./Libs/ZSTestGUI
#ZSPhysValGUI.subdir = ./Libs/ZSPhysValGUI
#ZSIpcGUI.subdir = ./Libs/ZSIpcGUI
#ZSTraceGUI.subdir = ./Libs/ZSTraceGUI
#ZSRemCmdGUI.subdir = ./Libs/ZSRemCmdGUI
#ZSDataGUI_old.subdir = ./Libs/ZSDataGUI.bak/ZSDataGUI_old.pro
#ZSDataGUI.subdir = ./Libs/ZSDataGUI
#ZSAppTestTemplate.subdir = ./Apps/Test/ZSTestTemplate/ZSAppTestTemplate.pro
#ZSAppTestAsyncReqs.subdir = ./Apps/Test/ZSAsyncReqs/ZSAppTestAsyncReqs.pro
#ZSAppTestErrLogModel.subdir = ./Apps/Test/ZSErrLogModel/ZSAppTestErrLogModel.pro
#ZSAppTestIdxTree.subdir = ./Apps/Test/ZSIdxTree/ZSAppTestIdxTree.pro
#ZSAppTestObjPoolModel.subdir = ./Apps/Test/ZSObjPoolModel/ZSAppTestObjPoolModel.pro
#ZSAppTestPhysVal.subdir = ./Apps/Test/ZSPhysVal/ZSAppTestPhysVal.pro
#ZSAppTestIpc.subdir = ./Apps/Test/ZSIpc/ZSAppTestIpc.pro
#ZSAppTestTrcServer.subdir = ./Apps/Test/ZSTrcServer/ZSAppTestTrcServer.pro
#ZSAppTestDiagram.subdir = ./Apps/Test/ZSDiagram/ZSAppTestDiagram.pro
#ZSAppTestRemCmdServer.subdir = ./Apps/Test/ZSRemCmdServer/ZSAppTestRemCmdServer.pro
#ZSAppTestData.subdir = ./Apps/Test/ZSData/ZSAppTestData.pro
#ZSAppTestData_old.subdir = ./Apps/Test/ZSData.bak/ZSAppTestData_old.pro
#ZSAppTrcMthClient.subdir = ./Apps/Products/ZSTrcMthClient/ZSAppTrcMthClient.pro
#ZSAppDraw.subdir = ./Apps/Products/ZSDraw/ZSAppDraw.pro
#ZSAppRemCmdClient.subdir = ./Apps/Products/ZSRemCmdClient/ZSAppRemCmdClient.pro

ZSTest.depends = ZSSys
ZSPhysVal.depends = ZSSys
ZSPhysSizesElectricity.depends = ZSSys ZSPhysVal
ZSPhysSizesGeometry.depends = ZSSys ZSPhysVal
ZSPhysSizesKinematics.depends = ZSSys ZSPhysVal
ZSIpc.depends = ZSSys
ZSTrace.depends = ZSSys ZSIpc
ZSRemCmd.depends = ZSSys ZSIpc ZSTrace
ZSData_old.depends = ZSSys ZSIpc ZSTrace
ZSData.depends = ZSSys ZSIpc ZSTrace

ZSSysGUI.depends = ZSSys
ZSTestGUI.depends =  ZSSys ZSTest ZSSysGUI
ZSPhysValGUI.depends = ZSSys ZSPhysVal ZSSysGUI
ZSIpcGUI.depends = ZSSys ZSIpc ZSSysGUI
ZSTraceGUI.depends = ZSSys ZSIpc ZSTrace ZSSysGUI ZSIpcGUI
ZSDiagram.depends = ZSSys ZSIpc ZSTrace ZSSysGUI ZSIpcGUI ZSTraceGUI
ZSDraw.depends = ZSSys ZSIpc ZSTrace ZSSysGUI ZSIpcGUI ZSTraceGUI
ZSRemCmdGUI.depends = ZSSys ZSIpc ZSTrace ZSRemCmd ZSSysGUI ZSIpcGUI ZSTraceGUI
ZSDataGUI_old.depends = ZSSys ZSIpc ZSTrace ZSData_old ZSSysGUI ZSIpcGUI ZSTraceGUI
ZSDataGUI.depends = ZSSys ZSIpc ZSTrace ZSData ZSSysGUI ZSIpcGUI ZSTraceGUI

ZSAppTestTemplate.depends = ZSSys ZSTest ZSSysGUI ZSTestGUI
ZSAppTestAsyncReqs.depends = ZSSys ZSTest ZSIpc ZSTrace ZSSysGUI ZSTestGUI ZSIpcGUI ZSTraceGUI
ZSAppTestErrLogModel.depends = ZSSys ZSTest ZSIpc ZSTrace ZSSysGUI ZSTestGUI ZSIpcGUI ZSTraceGUI
ZSAppTestIdxTree.depends = ZSSys ZSTest ZSSysGUI ZSTestGUI
ZSAppTestObjPoolModel.depends = ZSSys ZSTest ZSSysGUI ZSTestGUI
ZSAppTestPhysVal.depends = ZSSys ZSTest ZSPhysVal ZSSysGUI ZSTestGUI ZSPhysValGUI
ZSAppTestIpc.depends = ZSSys ZSTest ZSIpc ZSTrace ZSSysGUI ZSTestGUI ZSIpcGUI ZSTraceGUI
ZSAppTestTrcServer.depends = ZSSys ZSTest ZSIpc ZSTrace ZSSysGUI ZSTestGUI ZSIpcGUI ZSTraceGUI
ZSAppTestDiagram.depends = ZSSys ZSTest ZSIpc ZSTrace ZSDiagram ZSSysGUI ZSTestGUI ZSIpcGUI ZSTraceGUI
ZSAppTestRemCmdServer.depends = ZSSys ZSTest ZSIpc ZSTrace ZSRemCmd ZSSysGUI ZSTestGUI ZSIpcGUI ZSTraceGUI ZSRemCmdGUI
ZSAppTestData.depends = ZSSys ZSTest ZSIpc ZSTrace ZSData ZSSysGUI ZSTestGUI ZSIpcGUI ZSTraceGUI ZSDataGUI
ZSAppTestData_old.depends = ZSSys ZSTest ZSIpc ZSTrace ZSData_old ZSSysGUI ZSTestGUI ZSIpcGUI ZSTraceGUI ZSDataGUI_old
ZSAppTrcMthClient.depends = ZSSys ZSTest ZSIpc ZSTrace ZSSysGUI ZSTestGUI ZSIpcGUI ZSTraceGUI
ZSAppDraw.depends = ZSSys ZSTest ZSIpc ZSTrace ZSDraw ZSSysGUI ZSTestGUI ZSIpcGUI ZSTraceGUI
ZSAppRemCmdClient.depends = ZSSys ZSTest ZSIpc ZSTrace ZSRemCmd ZSSysGUI ZSTestGUI ZSIpcGUI ZSTraceGUI ZSRemCmdGUI

#SUBDIRS += \
#    ZSSys \
#    ZSTest \
#    ZSPhysVal \
#    ZSPhysSizesElectricity \
#    ZSPhysSizesGeometry \
#    ZSPhysSizesKinematics \
#    ZSIpc \
#    ZSTrace \
#    ZSDiagram \
#    ZSDraw \
#    ZSRemCmd \
#    ZSData_old \
#    ZSData \
#    ZSSysGUI \
#    ZSTestGUI \
#    ZSPhysValGUI \
#    ZSIpcGUI \
#    ZSTraceGUI \
#    ZSRemCmdGUI \
#    ZSDataGUI_old \
#    ZSDataGUI \
#    ZSAppTestTemplate \
#    ZSAppTestAsyncReqs \
#    ZSAppTestErrLogModel \
#    ZSAppTestIdxTree \
#    ZSAppTestObjPoolModel \
#    ZSAppTestPhysVal \
#    ZSAppTestIpc \
#    ZSAppTestTrcServer \
#    ZSAppTestDiagram \
#    ZSAppTestRemCmdServer \
#    ZSAppTestData \
#    ZSAppTestData_old \
#    ZSAppTrcMthClient \
#    ZSAppDraw \
#    ZSAppRemCmdClient

SUBDIRS += \
    ./Libs/ZSSys \
    ./Libs/ZSTest \
    ./Libs/ZSPhysVal \
    ./Libs/ZSPhysSizes/Electricity/ZSPhysSizesElectricity.pro \
    ./Libs/ZSPhysSizes/Geometry/ZSPhysSizesGeometry.pro \
    ./Libs/ZSPhysSizes/Kinematics/ZSPhysSizesKinematics.pro \
    ./Libs/ZSIpc \
    ./Libs/ZSTrace \
    ./Libs/ZSDiagram \
    ./Libs/ZSDraw \
    ./Libs/ZSRemCmd \
    ./Libs/ZSData.bak/ZSData_old.pro \
    ./Libs/ZSData \
    ./Libs/ZSSysGUI \
    ./Libs/ZSTestGUI \
    ./Libs/ZSPhysValGUI \
    ./Libs/ZSIpcGUI \
    ./Libs/ZSTraceGUI \
    ./Libs/ZSRemCmdGUI \
    ./Libs/ZSDataGUI.bak/ZSDataGUI_old.pro \
    ./Libs/ZSDataGUI \
    ./Apps/Test/ZSTestTemplate/ZSAppTestTemplate.pro \
    ./Apps/Test/ZSAsyncReqs/ZSAppTestAsyncReqs.pro \
    ./Apps/Test/ZSErrLogModel/ZSAppTestErrLogModel.pro \
    ./Apps/Test/ZSIdxTree/ZSAppTestIdxTree.pro \
    ./Apps/Test/ZSObjPoolModel/ZSAppTestObjPoolModel.pro \
    ./Apps/Test/ZSPhysVal/ZSAppTestPhysVal.pro \
    ./Apps/Test/ZSIpc/ZSAppTestIpc.pro \
    ./Apps/Test/ZSTrcServer/ZSAppTestTrcServer.pro \
    ./Apps/Test/ZSDiagram/ZSAppTestDiagram.pro \
    ./Apps/Test/ZSRemCmdServer/ZSAppTestRemCmdServer.pro \
    ./Apps/Test/ZSData/ZSAppTestData.pro \
    ./Apps/Test/ZSData.bak/ZSAppTestData_old.pro \
    ./Apps/Products/ZSTrcMthClient/ZSAppTrcMthClient.pro \
    ./Apps/Products/ZSDraw/ZSAppDraw.pro \
    ./Apps/Products/ZSRemCmdClient/ZSAppRemCmdClient.pro
