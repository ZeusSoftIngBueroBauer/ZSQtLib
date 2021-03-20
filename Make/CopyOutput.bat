@echo off

REM ..\..\CopyOutput.bat $(MSVCVersion) $(Platform) $(OutDir) $(TargetName) [$(QTBinBasePath)]
REM
REM    %1: MSVCVersion = [msvc2015, msvc2017, msvc2019]
REM    %2: Platform    = [Win32, x64]
REM    %3: OutDir      = [..\..\..\Tmp\Out\ZSSys\Win32\Debug, ..]
REM    %4: TargetName  = [ZSSys, ZSSysd, ZSSysGUI, ZSSysGUId, ..]
REM    %5: QtBasePath  = [C:\Qt\Bin\5.15.2]

set MSVC_Version=%1
set Platform=%2
set OutDir=%3
set TargetName=%4
set QTBinBasePath=%5
set QtBinPath=%QTBinBasePath%\%MSVC_Version%\%Platform%\bin

echo MSVC_Version=%MSVC_Version%
echo Platform=%Platform%
echo OutDir=%OutDir%
echo TargetName=%TargetName%
echo QTBinBasePath=%QTBinBasePath%
echo QtBinPath=%QtBinPath%

set CURRDIR=%CD%
set BATCHDIR=%~dp0%

if not defined BinDir set BinDir=%BATCHDIR%\..\Bin
if not defined LibDir set LibDir=%BATCHDIR%\..\Lib

if not exist %LibDir% mkdir %LibDir%
if not exist %LibDir%\%MSVC_Version%_%Platform% mkdir %LibDir%\%MSVC_Version%_%Platform%
if not exist %BinDir% mkdir %BinDir%
if not exist %BinDir%\%MSVC_Version%_%Platform% mkdir %BinDir%\%MSVC_Version%_%Platform%

:COPY_QTCORE_DLL
set QtDll=Qt5Core
if not exist %QtBinPath%\%QtDll%.dll goto COPY_LIB
echo copy %QtBinPath%\%QtDll%.dll to %BinDir%\%MSVC_Version%_%Platform%:
if exist %BinDir%\%MSVC_Version%_%Platform%\%QtDll%.dll attrib -R %BinDir%\%MSVC_Version%_%Platform%\%QtDll%.dll
if exist %QtBinPath%\%QtDll%.dll copy %QtBinPath%\%QtDll%.dll %BinDir%\%MSVC_Version%_%Platform%\%QtDll%.dll
if exist %BinDir%\%MSVC_Version%_%Platform%\%QtDll%.dll attrib -R %BinDir%\%MSVC_Version%_%Platform%\%QtDll%.dll
if not exist %QtBinPath%\%QtDll%d.dll goto COPY_QTXML_DLL
echo copy %QtBinPath%\%QtDll%d.dll to %BinDir%\%MSVC_Version%_%Platform%:
if exist %BinDir%\%MSVC_Version%_%Platform%\%QtDll%d.dll attrib -R %BinDir%\%MSVC_Version%_%Platform%\%QtDll%d.dll
if exist %QtBinPath%\%QtDll%d.dll copy %QtBinPath%\Qt5Cored.dll %BinDir%\%MSVC_Version%_%Platform%\%QtDll%d.dll
if exist %BinDir%\%MSVC_Version%_%Platform%\%QtDll%d.dll attrib -R %BinDir%\%MSVC_Version%_%Platform%\%QtDll%d.dll

:COPY_QTXML_DLL
set QtDll=Qt5Xml
if not exist %QtBinPath%\%QtDll%.dll goto COPY_LIB
echo copy %QtBinPath%\%QtDll%.dll to %BinDir%\%MSVC_Version%_%Platform%:
if exist %BinDir%\%MSVC_Version%_%Platform%\%QtDll%.dll attrib -R %BinDir%\%MSVC_Version%_%Platform%\%QtDll%.dll
if exist %QtBinPath%\%QtDll%.dll copy %QtBinPath%\%QtDll%.dll %BinDir%\%MSVC_Version%_%Platform%\%QtDll%.dll
if exist %BinDir%\%MSVC_Version%_%Platform%\%QtDll%.dll attrib -R %BinDir%\%MSVC_Version%_%Platform%\%QtDll%.dll
if not exist %QtBinPath%\%QtDll%d.dll goto COPY_QTNETWORK_DLL
echo copy %QtBinPath%\%QtDll%d.dll to %BinDir%\%MSVC_Version%_%Platform%:
if exist %BinDir%\%MSVC_Version%_%Platform%\%QtDll%d.dll attrib -R %BinDir%\%MSVC_Version%_%Platform%\%QtDll%d.dll
if exist %QtBinPath%\%QtDll%d.dll copy %QtBinPath%\Qt5Cored.dll %BinDir%\%MSVC_Version%_%Platform%\%QtDll%d.dll
if exist %BinDir%\%MSVC_Version%_%Platform%\%QtDll%d.dll attrib -R %BinDir%\%MSVC_Version%_%Platform%\%QtDll%d.dll

:COPY_QTNETWORK_DLL
set QtDll=Qt5Network
if not exist %QtBinPath%\%QtDll%.dll goto COPY_LIB
echo copy %QtBinPath%\%QtDll%.dll to %BinDir%\%MSVC_Version%_%Platform%:
if exist %BinDir%\%MSVC_Version%_%Platform%\%QtDll%.dll attrib -R %BinDir%\%MSVC_Version%_%Platform%\%QtDll%.dll
if exist %QtBinPath%\%QtDll%.dll copy %QtBinPath%\%QtDll%.dll %BinDir%\%MSVC_Version%_%Platform%\%QtDll%.dll
if exist %BinDir%\%MSVC_Version%_%Platform%\%QtDll%.dll attrib -R %BinDir%\%MSVC_Version%_%Platform%\%QtDll%.dll
if not exist %QtBinPath%\%QtDll%d.dll goto COPY_QTGUI_DLL
echo copy %QtBinPath%\%QtDll%d.dll to %BinDir%\%MSVC_Version%_%Platform%:
if exist %BinDir%\%MSVC_Version%_%Platform%\%QtDll%d.dll attrib -R %BinDir%\%MSVC_Version%_%Platform%\%QtDll%d.dll
if exist %QtBinPath%\%QtDll%d.dll copy %QtBinPath%\Qt5Cored.dll %BinDir%\%MSVC_Version%_%Platform%\%QtDll%d.dll
if exist %BinDir%\%MSVC_Version%_%Platform%\%QtDll%d.dll attrib -R %BinDir%\%MSVC_Version%_%Platform%\%QtDll%d.dll

:COPY_QTGUI_DLL
set QtDll=Qt5Gui
if not exist %QtBinPath%\%QtDll%.dll goto COPY_LIB
echo copy %QtBinPath%\%QtDll%.dll to %BinDir%\%MSVC_Version%_%Platform%:
if exist %BinDir%\%MSVC_Version%_%Platform%\%QtDll%.dll attrib -R %BinDir%\%MSVC_Version%_%Platform%\%QtDll%.dll
if exist %QtBinPath%\%QtDll%.dll copy %QtBinPath%\%QtDll%.dll %BinDir%\%MSVC_Version%_%Platform%\%QtDll%.dll
if exist %BinDir%\%MSVC_Version%_%Platform%\%QtDll%.dll attrib -R %BinDir%\%MSVC_Version%_%Platform%\%QtDll%.dll
if not exist %QtBinPath%\%QtDll%d.dll goto COPY_QTWIDGETS_DLL
echo copy %QtBinPath%\%QtDll%d.dll to %BinDir%\%MSVC_Version%_%Platform%:
if exist %BinDir%\%MSVC_Version%_%Platform%\%QtDll%d.dll attrib -R %BinDir%\%MSVC_Version%_%Platform%\%QtDll%d.dll
if exist %QtBinPath%\%QtDll%d.dll copy %QtBinPath%\Qt5Cored.dll %BinDir%\%MSVC_Version%_%Platform%\%QtDll%d.dll
if exist %BinDir%\%MSVC_Version%_%Platform%\%QtDll%d.dll attrib -R %BinDir%\%MSVC_Version%_%Platform%\%QtDll%d.dll

:COPY_QTWIDGETS_DLL
set QtDll=Qt5Widgets
if not exist %QtBinPath%\%QtDll%.dll goto COPY_LIB
echo copy %QtBinPath%\%QtDll%.dll to %BinDir%\%MSVC_Version%_%Platform%:
if exist %BinDir%\%MSVC_Version%_%Platform%\%QtDll%.dll attrib -R %BinDir%\%MSVC_Version%_%Platform%\%QtDll%.dll
if exist %QtBinPath%\%QtDll%.dll copy %QtBinPath%\%QtDll%.dll %BinDir%\%MSVC_Version%_%Platform%\%QtDll%.dll
if exist %BinDir%\%MSVC_Version%_%Platform%\%QtDll%.dll attrib -R %BinDir%\%MSVC_Version%_%Platform%\%QtDll%.dll
if not exist %QtBinPath%\%QtDll%d.dll goto COPY_LIB
echo copy %QtBinPath%\%QtDll%d.dll to %BinDir%\%MSVC_Version%_%Platform%:
if exist %BinDir%\%MSVC_Version%_%Platform%\%QtDll%d.dll attrib -R %BinDir%\%MSVC_Version%_%Platform%\%QtDll%d.dll
if exist %QtBinPath%\%QtDll%d.dll copy %QtBinPath%\Qt5Cored.dll %BinDir%\%MSVC_Version%_%Platform%\%QtDll%d.dll
if exist %BinDir%\%MSVC_Version%_%Platform%\%QtDll%d.dll attrib -R %BinDir%\%MSVC_Version%_%Platform%\%QtDll%d.dll

:COPY_LIB
if not exist %OutDir%\%TargetName%.lib goto COPY_DLL
echo copy %OutDir%\%TargetName%.lib to %LibDir%\%MSVC_Version%_%Platform%:
if exist %LibDir%\%MSVC_Version%_%Platform%\%TargetName%.lib attrib -R %LibDir%\%MSVC_Version%_%Platform%\%TargetName%.lib
if exist %OutDir%\%TargetName%.lib copy %OutDir%\%TargetName%.lib %LibDir%\%MSVC_Version%_%Platform%\%TargetName%.lib
if exist %LibDir%\%MSVC_Version%_%Platform%\%TargetName%.lib attrib -R %LibDir%\%MSVC_Version%_%Platform%\%TargetName%.lib

:COPY_DLL
if not exist %OutDir%\%TargetName%.dll goto COPY_PYD
echo copy %OutDir%\%TargetName%.dll to %BinDir%\%MSVC_Version%_%Platform%:
if exist %BinDir%\%MSVC_Version%_%Platform%\%TargetName%.dll attrib -R %BinDir%\%MSVC_Version%_%Platform%\%TargetName%.dll
if exist %OutDir%\%TargetName%.dll copy %OutDir%\%TargetName%.dll %BinDir%\%MSVC_Version%_%Platform%\%TargetName%.dll
if exist %BinDir%\%MSVC_Version%_%Platform%\%TargetName%.dll attrib -R %BinDir%\%MSVC_Version%_%Platform%\%TargetName%.dll

:COPY_PYD
if not exist %OutDir%\%TargetName%.pyd goto COPY_EXE
echo copy %OutDir%\%TargetName%.pyd to %BinDir%\%MSVC_Version%_%Platform%:
if exist %BinDir%\%MSVC_Version%_%Platform%\%TargetName%.pyd attrib -R %BinDir%\%MSVC_Version%_%Platform%\%TargetName%.pyd
if exist %OutDir%\%TargetName%.pyd copy %OutDir%\%TargetName%.pyd %BinDir%\%MSVC_Version%_%Platform%\%TargetName%.pyd
if exist %BinDir%\%MSVC_Version%_%Platform%\%TargetName%.pyd attrib -R %BinDir%\%MSVC_Version%_%Platform%\%TargetName%.pyd

:COPY_EXE
if not exist %OutDir%\%TargetName%.exe goto COPY_PDB
echo copy %OutDir%\%TargetName%.exe to %BinDir%\%MSVC_Version%_%Platform%:
if exist %BinDir%\%MSVC_Version%_%Platform%\%TargetName%.exe attrib -R %BinDir%\%MSVC_Version%_%Platform%\%TargetName%.exe
if exist %OutDir%\%TargetName%.exe copy %OutDir%\%TargetName%.exe %BinDir%\%MSVC_Version%_%Platform%\%TargetName%.exe
if exist %BinDir%\%MSVC_Version%_%Platform%\%TargetName%.exe attrib -R %BinDir%\%MSVC_Version%_%Platform%\%TargetName%.exe

:COPY_PDB
if not exist %OutDir%\%TargetName%.pdb goto EXIT
echo copy %OutDir%\%TargetName%.pdb to %BinDir%\%MSVC_Version%_%Platform%:
if exist %BinDir%\%MSVC_Version%_%Platform%\%TargetName%.pdb attrib -R %BinDir%\%MSVC_Version%_%Platform%\%TargetName%.pdb
if exist %OutDir%\%TargetName%.pdb copy %OutDir%\%TargetName%.pdb %BinDir%\%MSVC_Version%_%Platform%\%TargetName%.pdb
if exist %BinDir%\%MSVC_Version%_%Platform%\%TargetName%.pdb attrib -R %BinDir%\%MSVC_Version%_%Platform%\%TargetName%.pdb

goto EXIT_0

:Dir_NOT_DEFINED
echo Either Environment Variable BinDir or LibDir is not defined
EXIT 1

:EXIT_0
EXIT 0

