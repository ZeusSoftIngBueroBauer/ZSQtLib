@ECHO off
set CURRDIR=%CD%
set BATCHDIR=%~dp0%
set BIN_DIR=..\..\..\..\Bin\msvc2019_x64
ECHO BIN_DIR: %BIN_DIR%
set DIST_DIR=..\..\..\..\Dist\msvc2019_x64\Debug\ZSAppTrcMthClient
ECHO DIST_DIR: %DIST_DIR%

if not exist %BIN_DIR% mkdir %BIN_DIR%
if not exist %DIST_DIR% mkdir %DIST_DIR%

ECHO %QTDIR_MSVC2019_x64%\bin\qmake.exe -spec win32-msvc "CONFIG += debug" %BATCHDIR%ZSAppTrcMthClient-packaging.pro
%QTDIR_MSVC2019_x64%\bin\qmake.exe -spec win32-msvc "CONFIG += debug" %BATCHDIR%ZSAppTrcMthClient-packaging.pro

nmake qmake_all

pushd ZSAppTrcMthClient
nmake && popd

copy %BIN_DIR%\ZSSysQt5_msvc2019_x64_d.dll %DIST_DIR%
copy %BIN_DIR%\ZSSysGUIQt5_msvc2019_x64_d.dll %DIST_DIR%
copy %BIN_DIR%\ZSIpcQt5_msvc2019_x64_d.dll %DIST_DIR%
copy %BIN_DIR%\ZSIpcGUIQt5_msvc2019_x64_d.dll %DIST_DIR%
copy %BIN_DIR%\ZSIpcTraceQt5_msvc2019_x64_d.dll %DIST_DIR%
copy %BIN_DIR%\ZSIpcTraceGUIQt5_msvc2019_x64_d.dll %DIST_DIR%
copy %BIN_DIR%\ZSAppTrcMthClientd.exe %DIST_DIR%

%QTDIR_MSVC2019_x64%\bin\windeployqt ^
    %DIST_DIR%\ZSSysQt5_msvc2019_x64_d.dll ^
    %DIST_DIR%\ZSSysGUIQt5_msvc2019_x64_d.dll ^
    %DIST_DIR%\ZSIpcQt5_msvc2019_x64_d.dll ^
    %DIST_DIR%\ZSIpcGUIQt5_msvc2019_x64_d.dll ^
    %DIST_DIR%\ZSIpcTraceQt5_msvc2019_x64_d.dll ^
    %DIST_DIR%\ZSIpcTraceGUIQt5_msvc2019_x64_d.dll ^
    %DIST_DIR%\ZSAppTrcMthClientd.exe

popd

:END
