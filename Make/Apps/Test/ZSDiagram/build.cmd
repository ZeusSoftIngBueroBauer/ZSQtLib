@echo off
set PROJECTNAME=ZSAppDiagram_8.0

if "%QTDIR%"=="" goto ERROR_QTDIR_NOTDEFINED
if "%MSVSDIR%"=="" goto ERROR_MSVSDIR_NOTDEFINED

set VCBUILD_PATH="%MSVSDIR%"\VC\vcpackages
set DEVENV_PATH="%MSVSDIR%"\Common7\IDE

if not exist error.log goto :INIT
del error.log

:INIT
set debug=0
set debug_nogui=0
set release=0
set release_nogui=0

if "%1%"==""                goto DEF_ALL
if /i %1 == debug           goto DEF_DEBUG
if /i %1 == release         goto DEF_RELEASE
if /i %1 == all             goto DEF_ALL
goto USAGE

:DEF_DEBUG
set debug=1
goto CHECK_REBUILD

:DEF_RELEASE
set release=1
goto CHECK_REBUILD

:DEF_ALL
set debug=1
set release=1
goto CHECK_REBUILD

:CHECK_REBUILD
set dorebuild=
if "%2%"=="" goto BUILD_START
if %2%==rebuild goto DEF_REBUILD
if %2%==/rebuild goto DEF_REBUILD
goto USAGE

:DEF_REBUILD
set dorebuild=/rebuild
goto BUILD_START

:BUILD_START
@echo on

:BUILD_START_DEBUG
@if %debug%==0 goto BUILD_END_DEBUG
%VCBUILD_PATH%\vcbuild /htmllog:build_debug.html /logfile:build.log %dorebuild% %PROJECTNAME%.vcproj Debug
@if %errorlevel% == 0 goto BUILD_END_DEBUG
@echo Error on building %PROJECTNAME% Debug >> error.log
:BUILD_END_DEBUG

:BUILD_START_RELEASE
@if %release%==0 goto BUILD_END_RELEASE
%VCBUILD_PATH%\vcbuild /htmllog:build_release.html /logfile:build.log %dorebuild% %PROJECTNAME%.vcproj Release
@if %errorlevel% == 0 goto BUILD_END_RELEASE
@echo Error on building %PROJECTNAME% Release >> error.log
:BUILD_END_RELEASE

@goto BUILD_END

:USAGE
@echo Error in script usage for building %PROJECTNAME%.
@echo The correct usage is:
@echo     build [config] [rebuild]
@echo         where [config] is: all ^| debug ^| debug_nogui ^| release ^| release_nogui
@echo         and [rebuild] is optional
@echo:
@echo For example:
@echo     build all
@echo     build release_nogui rebuild
@PAUSE
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

:BUILD_END
:EOF
