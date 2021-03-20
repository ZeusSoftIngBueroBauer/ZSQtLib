cls

@echo off

if "%QTDIR%"=="" goto ERROR_QTDIR_NOTDEFINED
if "%MSVSDIR%"=="" goto ERROR_MSVSDIR_NOTDEFINED

set VCBUILD_PATH="%MSVSDIR%"\VC\vcpackages
set DEVENV_PATH="%MSVSDIR%"\Common7\IDE

if not exist error.log goto UPD_VERSIONS
del error.log

:UPD_VERSIONS
@echo.
@echo ----------------------------------------------------------------
@echo updating version files ...
@echo ----------------------------------------------------------------
@echo.
WScript updVersions.vbs

:CHECK_REBUILD
set dorebuild=
if "%1%"=="" goto BUILD_START
if %1%==rebuild goto DEF_REBUILD
goto BUILD_START

:DEF_REBUILD
@echo ----------------------------------------------------------------
@echo cleaning ...
@echo ----------------------------------------------------------------
set dorebuild=rebuild
call clean
goto BUILD_START

:BUILD_START
@echo on

:BUILD_START_ZSSys
@if not exist ..\ZSSys goto BUILD_START_ZSIpc
@echo.
@echo ----------------------------------------------------------------
@echo building ZSSys ...
@echo ----------------------------------------------------------------
@echo.
@cd  ..\ZSSys\Make\Windows
call build all %dorebuild%
@if not exist error.log goto BUILD_END_ZSSys
@echo Error on building ZSSys >> ..\..\..\ZSBuild\error.log
:BUILD_END_ZSSys
@cd  ..\..\..\ZSBuild

:BUILD_START_ZSIpc
@if not exist ..\ZSIpc goto BUILD_START_ZSTrace
@echo.
@echo ----------------------------------------------------------------
@echo building ZSIpc ...
@echo ----------------------------------------------------------------
@echo.
@cd  ..\ZSIpc\Make\Windows
call build all %dorebuild%
@if not exist error.log goto BUILD_END_ZSIpc
@echo Error on building ZSIpc >> ..\..\..\ZSBuild\error.log
:BUILD_END_ZSIpc
@cd  ..\..\..\ZSBuild

:BUILD_START_ZSTrace
@if not exist ..\ZSTrace goto BUILD_START_ZSPhysVal
@echo.
@echo ----------------------------------------------------------------
@echo building ZSTrace ...
@echo ----------------------------------------------------------------
@echo.
@cd  ..\ZSTrace\Make\Windows
call build all %dorebuild%
@if not exist error.log goto BUILD_END_ZSTrace
@echo Error on building ZSTrace >> ..\..\..\ZSBuild\error.log
:BUILD_END_ZSTrace
@cd  ..\..\..\ZSBuild

:BUILD_START_ZSPhysVal
@if not exist ..\ZSPhysVal goto BUILD_START_ZSPhysSizes
@echo.
@echo ----------------------------------------------------------------
@echo building ZSPhysVal ...
@echo ----------------------------------------------------------------
@echo.
@cd  ..\ZSPhysVal\Make\Windows
call build all %dorebuild%
@if not exist error.log goto BUILD_END_ZSPhysVal
@echo Error on building ZSPhysVal >> ..\..\..\ZSBuild\error.log
:BUILD_END_ZSPhysVal
@cd  ..\..\..\ZSBuild

:BUILD_START_ZSPhysSizes
@if not exist ..\ZSPhysSizes goto BUILD_START_ZSDiagram

:BUILD_START_ZSPhysSizes_Electricity
@if not exist ..\ZSPhysSizes\Electricity goto BUILD_START_ZSPhysSizes_Geometry
@echo.
@echo ----------------------------------------------------------------
@echo building ZSPhysSizes_Electricity ...
@echo ----------------------------------------------------------------
@echo.
@cd  ..\ZSPhysSizes\Electricity\Make\Windows
call build all %dorebuild%
@if not exist error.log goto BUILD_END_ZSPhysSizes_Electricity
@echo Error on building ZSPhysSizes_Electricity >> ..\..\..\..\ZSBuild\error.log
:BUILD_END_ZSPhysSizes_Electricity
@cd  ..\..\..\..\ZSBuild

:BUILD_START_ZSPhysSizes_Geometry
@if not exist ..\ZSPhysSizes\Geometry goto BUILD_START_ZSPhysSizes_Kinematics
@echo.
@echo ----------------------------------------------------------------
@echo building ZSPhysSizes_Geometry ...
@echo ----------------------------------------------------------------
@echo.
@cd  ..\ZSPhysSizes\Geometry\Make\Windows
call build all %dorebuild%
@if not exist error.log goto BUILD_END_ZSPhysSizes_Geometry
@echo Error on building ZSPhysSizes_Geometry >> ..\..\..\..\ZSBuild\error.log
:BUILD_END_ZSPhysSizes_Geometry
@cd  ..\..\..\..\ZSBuild

:BUILD_START_ZSPhysSizes_Kinematics
@if not exist ..\ZSPhysSizes\Kinematics goto BUILD_START_ZSDiagram
@echo.
@echo ----------------------------------------------------------------
@echo building ZSPhysSizes_Kinematics ...
@echo ----------------------------------------------------------------
@echo.
@cd  ..\ZSPhysSizes\Kinematics\Make\Windows
call build all %dorebuild%
@if not exist error.log goto BUILD_END_ZSPhysSizes_Kinematics
@echo Error on building ZSPhysSizes_Kinematics >> ..\..\..\..\ZSBuild\error.log
:BUILD_END_ZSPhysSizes_Kinematics
@cd  ..\..\..\..\ZSBuild

:BUILD_START_ZSDiagram
@if not exist ..\ZSDiagram goto BUILD_START_ZSData
@echo.
@echo ----------------------------------------------------------------
@echo building ZSDiagram ...
@echo ----------------------------------------------------------------
@echo.
@cd  ..\ZSDiagram\Make\Windows
call build all %dorebuild%
@if not exist error.log goto BUILD_END_ZSDiagram
@echo Error on building ZSDiagram >> ..\..\..\ZSBuild\error.log
:BUILD_END_ZSDiagram
@cd  ..\..\..\ZSBuild

:BUILD_START_ZSData
@if not exist ..\ZSData goto BUILD_START_ZSSNMP
@echo.
@echo ----------------------------------------------------------------
@echo building ZSData ...
@echo ----------------------------------------------------------------
@echo.
@cd  ..\ZSData\Make\Windows
call build all %dorebuild%
@if not exist error.log goto BUILD_END_ZSData
@echo Error on building ZSData >> ..\..\..\ZSBuild\error.log
:BUILD_END_ZSData
@cd  ..\..\..\ZSBuild

:BUILD_START_ZSSNMP
@if not exist ..\ZSSNMP goto BUILD_START_ZSAppExamples
@echo.
@echo ----------------------------------------------------------------
@echo building ZSSNMP ...
@echo ----------------------------------------------------------------
@echo.
@cd  ..\ZSSNMP\Make\Windows
call build all %dorebuild%
@if not exist error.log goto BUILD_END_ZSSNMP
@echo Error on building ZSSNMP >> ..\..\..\ZSBuild\error.log
:BUILD_END_ZSSNMP
@cd  ..\..\..\ZSBuild

:BUILD_START_ZSAppExamples
@if not exist ..\ZSAppExamples goto BUILD_START_ZSApps

:BUILD_START_ZSApp_Examples_DataSet
@if not exist ..\ZSAppExamples\ZSAppData goto BUILD_START_ZSApp_Examples_Diagram
@echo.
@echo ----------------------------------------------------------------
@echo building ZSApp_Examples_Data
@echo ----------------------------------------------------------------
@echo.
@cd  ..\ZSAppExamples\ZSAppData\Make\Windows
call build all %dorebuild%
@if not exist error.log goto BUILD_END_ZSApp_Examples_Data
@echo Error on building ZSAppExamples_Data >> ..\..\..\..\ZSBuild\error.log
:BUILD_END_ZSApp_Examples_Data
@cd  ..\..\..\..\ZSBuild

:BUILD_START_ZSApp_Examples_Diagram
@if not exist ..\ZSAppExamples\ZSAppDiagram goto BUILD_START_ZSApp_Examples_ObjPoolModel
@echo.
@echo ----------------------------------------------------------------
@echo building ZSApp_Examples_Diagram
@echo ----------------------------------------------------------------
@echo.
@cd  ..\ZSAppExamples\ZSAppDiagram\Make\Windows
call build all %dorebuild%
@if not exist error.log goto BUILD_END_ZSApp_Examples_Diagram
@echo Error on building ZSAppExamples_Diagram >> ..\..\..\..\ZSBuild\error.log
:BUILD_END_ZSApp_Examples_Diagram
@cd  ..\..\..\..\ZSBuild

:BUILD_START_ZSApp_Examples_ObjPoolModel
@if not exist ..\ZSAppExamples\ZSAppObjPoolModel goto BUILD_START_ZSApp_Examples_PhysVal
@echo.
@echo ----------------------------------------------------------------
@echo building ZSApp_Examples_ObjPoolModel
@echo ----------------------------------------------------------------
@echo.
@cd  ..\ZSAppExamples\ZSAppObjPoolModel\Make\Windows
call build all %dorebuild%
@if not exist error.log goto BUILD_END_ZSApp_Examples_ObjPoolModel
@echo Error on building ZSAppExamples_ObjPoolModel >> ..\..\..\..\ZSBuild\error.log
:BUILD_END_ZSApp_Examples_ObjPoolModel
@cd  ..\..\..\..\ZSBuild

:BUILD_START_ZSApp_Examples_PhysVal
@if not exist ..\ZSAppExamples\ZSAppPhysVal goto BUILD_START_ZSApp_Examples_TrcServer
@echo.
@echo ----------------------------------------------------------------
@echo building ZSApp_Examples_PhysVal
@echo ----------------------------------------------------------------
@echo.
@cd  ..\ZSAppExamples\ZSAppPhysVal\Make\Windows
call build all %dorebuild%
@if not exist error.log goto BUILD_END_ZSApp_Examples_PhysVal
@echo Error on building ZSAppExamples_PhysVal >> ..\..\..\..\ZSBuild\error.log
:BUILD_END_ZSApp_Examples_PhysVal
@cd  ..\..\..\..\ZSBuild

:BUILD_START_ZSApp_Examples_TrcServer
@if not exist ..\ZSAppExamples\ZSAppTrcServer goto BUILD_START_ZSApps
@echo.
@echo ----------------------------------------------------------------
@echo building ZSApp_Examples_TrcServer
@echo ----------------------------------------------------------------
@echo.
@cd  ..\ZSAppExamples\ZSAppTrcServer\Make\Windows
call build all %dorebuild%
@if not exist error.log goto BUILD_END_ZSApp_Examples_TrcServer
@echo Error on building ZSAppExamples_TrcServer >> ..\..\..\..\ZSBuild\error.log
:BUILD_END_ZSApp_Examples_TrcServer
@cd  ..\..\..\..\ZSBuild

:BUILD_START_ZSApps

:BUILD_START_ZSApp_CmdClient
@if not exist ..\ZSAppCmdClient goto BUILD_START_ZSApp_TrcCtxClient
@echo.
@echo ----------------------------------------------------------------
@echo building ZSApp_CmdClient
@echo ----------------------------------------------------------------
@echo.
@cd  ..\ZSAppCmdClient\Make\Windows
call build all %dorebuild%
@if not exist error.log goto BUILD_END_ZSApp_CmdClient
@echo Error on building ZSAppCmdClient >> ..\..\..\ZSBuild\error.log
:BUILD_END_ZSApp_CmdClient
@cd  ..\..\..\ZSBuild

:BUILD_START_ZSApp_TrcCtxClient
@if not exist ..\ZSAppTrcCtxClient goto BUILD_START_ZSApp_TrcMsgClient
@echo.
@echo ----------------------------------------------------------------
@echo building ZSApp_TrcCtxClient
@echo ----------------------------------------------------------------
@echo.
@cd  ..\ZSAppTrcCtxClient\Make\Windows
call build all %dorebuild%
@if not exist error.log goto BUILD_END_ZSApp_TrcCtxClient
@echo Error on building ZSAppTrcCtxClient >> ..\..\..\ZSBuild\error.log
:BUILD_END_ZSApp_TrcCtxClient
@cd  ..\..\..\ZSBuild

:BUILD_START_ZSApp_TrcMsgClient
@if not exist ..\ZSAppTrcMsgClient goto BUILD_START_ZSApp_TrcMthClient
@echo.
@echo ----------------------------------------------------------------
@echo building ZSApp_TrcMsgClient
@echo ----------------------------------------------------------------
@echo.
@cd  ..\ZSAppTrcMsgClient\Make\Windows
call build all %dorebuild%
@if not exist error.log goto BUILD_END_ZSApp_TrcMsgClient
@echo Error on building ZSAppTrcMsgClient >> ..\..\..\ZSBuild\error.log
:BUILD_END_ZSApp_TrcMsgClient
@cd  ..\..\..\ZSBuild

:BUILD_START_ZSApp_TrcMthClient
@if not exist ..\ZSAppTrcMthClient goto BUILD_START_ZSApp_SNMPExplorer
@echo.
@echo ----------------------------------------------------------------
@echo building ZSApp_TrcMthClient
@echo ----------------------------------------------------------------
@echo.
@cd  ..\ZSAppTrcMthClient\Make\Windows
call build all %dorebuild%
@if not exist error.log goto BUILD_END_ZSApp_TrcMthClient
@echo Error on building ZSAppTrcMthClient >> ..\..\..\ZSBuild\error.log
:BUILD_END_ZSApp_TrcMthClient
@cd  ..\..\..\ZSBuild

:BUILD_START_ZSApp_SNMPExplorer
@if not exist ..\ZSAppSNMPExplorer goto BUILD_START_ZSApp_TMI
@echo.
@echo ----------------------------------------------------------------
@echo building ZSApp_SNMPExplorer
@echo ----------------------------------------------------------------
@echo.
@cd  ..\ZSAppSNMPExplorer\Make\Windows
call build all %dorebuild%
@if not exist error.log goto BUILD_END_ZSApp_SNMPExplorer
@echo Error on building ZSAppSNMPExplorer >> ..\..\..\ZSBuild\error.log
:BUILD_END_ZSApp_SNMPExplorer
@cd  ..\..\..\ZSBuild

:BUILD_START_ZSApp_TMI
@if not exist ..\ZSAppTMI goto BUILD_END
@echo.
@echo ----------------------------------------------------------------
@echo building ZSApp_TMI
@echo ----------------------------------------------------------------
@echo.
@cd  ..\ZSAppTMI

:BUILD_START_ZSApp_TMI_Common
@if not exist ..\Common goto BUILD_START_ZSApp_TMI_Database
@echo.
@echo ----------------------------------------------------------------
@echo building ZSApp_TMI_Common
@echo ----------------------------------------------------------------
@echo.
@cd  Common\Make\Windows
call build all %dorebuild%
@if not exist error.log goto BUILD_END_ZSApp_TMI_Common
@echo Error on building ZSAppTMI_Common >> ..\..\..\..\ZSBuild\error.log
:BUILD_END_ZSApp_TMI_Common
@cd  ..\..\..

:BUILD_START_ZSApp_TMI_Database
@if not exist ..\Database goto BUILD_START_ZSApp_TMI_FSWSigGen
@echo.
@echo ----------------------------------------------------------------
@echo building ZSApp_TMI_Database
@echo ----------------------------------------------------------------
@echo.
@cd  Database\Make\Windows
call build all %dorebuild%
@if not exist error.log goto BUILD_END_ZSApp_TMI_Database
@echo Error on building ZSAppTMI_Database >> ..\..\..\..\ZSBuild\error.log
:BUILD_END_ZSApp_TMI_Database
@cd  ..\..\..

:BUILD_START_ZSApp_TMI_FSWSigGen
@if not exist ..\FSWSigGen goto BUILD_START_ZSApp_TMI_FSWCapture
@echo.
@echo ----------------------------------------------------------------
@echo building ZSApp_TMI_FSWSigGen
@echo ----------------------------------------------------------------
@echo.
@cd  FSWSigGen\Make\Windows
call build all %dorebuild%
@if not exist error.log goto BUILD_END_ZSApp_TMI_FSWSigGen
@echo Error on building ZSAppTMI_FSWSigGen >> ..\..\..\..\ZSBuild\error.log
:BUILD_END_ZSApp_TMI_FSWSigGen
@cd  ..\..\..

:BUILD_START_ZSApp_TMI_FSWCapture
@if not exist ..\FSWCapture goto BUILD_START_ZSApp_TMI_GUI
@echo.
@echo ----------------------------------------------------------------
@echo building ZSApp_TMI_FSWCapture
@echo ----------------------------------------------------------------
@echo.
@cd  FSWCapture\Make\Windows
call build all %dorebuild%
@if not exist error.log goto BUILD_END_ZSApp_TMI_FSWCapture
@echo Error on building ZSAppTMI_FSWCapture >> ..\..\..\..\ZSBuild\error.log
:BUILD_END_ZSApp_TMI_FSWCapture
@cd  ..\..\..

:BUILD_START_ZSApp_TMI_GUI
@if not exist ..\GUI goto BUILD_END
@echo.
@echo ----------------------------------------------------------------
@echo building ZSApp_TMI_GUI
@echo ----------------------------------------------------------------
@echo.
@cd  GUI\Make\Windows
call build all %dorebuild%
@if not exist error.log goto BUILD_END_ZSApp_TMI_GUI
@echo Error on building ZSAppTMI_GUI >> ..\..\..\..\ZSBuild\error.log
:BUILD_END_ZSApp_TMI_GUI
@cd  ..\..\..

:BUILD_END_ZSApp_TMI
@cd  ..\ZSBuild

:BUILD_END

@if not exist error.log goto EOF
@echo !
@echo ----------------------------------------------
@echo An error occured in one of the build processes.
@echo The following projects did not build correctly:
@type error.log
@echo ----------------------------------------------
@echo !
@goto EOF

:ERROR_QTDIR_NOTDEFINED
@echo !
@echo -----------------------------------------------------------------------
@echo The environment variable QTDIR is not defined.
@echo May be set for example to "C:\Qt\4.5.1"
@echo -----------------------------------------------------------------------
@echo !
@goto EOF

:ERROR_MSVSDIR_NOTDEFINED
@echo !
@echo -----------------------------------------------------------------------
@echo The environment variable MSVSDIR is not defined.
@echo Should be usually set to "C:\[Program Files]\Microsoft Visual Studio 8"
@echo -----------------------------------------------------------------------
@echo !
@goto EOF

:EOF
@PAUSE
