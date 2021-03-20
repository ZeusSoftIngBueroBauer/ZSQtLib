@echo off
set PROJECTNAME=ZSSNMP_8.0

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
if /i %1 == debug_nogui     goto DEF_DEBUG_NOGUI
if /i %1 == release         goto DEF_RELEASE
if /i %1 == release_nogui   goto DEF_RELEASE_NOGUI
if /i %1 == all             goto DEF_ALL
goto USAGE

:DEF_DEBUG
set debug=1
goto CHECK_REBUILD

:DEF_DEBUG_NOGUI
set debug_nogui=1
goto CHECK_REBUILD

:DEF_RELEASE
set release=1
goto CHECK_REBUILD

:DEF_RELEASE_NOGUI
set release_nogui=1
goto CHECK_REBUILD

:DEF_ALL
set debug=1
set debug_nogui=1
set release=1
set release_nogui=1
goto CHECK_REBUILD

:CHECK_REBUILD
set dorebuild=
if "%2%"=="" goto BUILD
if %2%==rebuild goto DEF_REBUILD
if %2%==/rebuild goto DEF_REBUILD
goto USAGE

:DEF_REBUILD
set dorebuild=/rebuild
goto BUILD

:BUILD
@echo on

:BUILD_DEBUG
@if %debug%==0 goto BUILD_DEBUG_NOGUI
%VCBUILD_PATH%\vcbuild /htmllog:build_debug.html /logfile:build.log %dorebuild% %PROJECTNAME%.vcproj Debug
@if %errorlevel% == 0 goto BUILD_DEBUG_NOGUI
@echo Error on building %PROJECTNAME% Debug >> error.log

:BUILD_DEBUG_NOGUI
@if %debug_nogui%==0 goto BUILD_RELEASE
%VCBUILD_PATH%\vcbuild /htmllog:build_debug_nogui.html /logfile:build.log %dorebuild% %PROJECTNAME%.vcproj Debug_NoGui
@if %errorlevel% == 0 goto BUILD_RELEASE
@echo Error on building %PROJECTNAME% Debug_NoGui >> error.log

:BUILD_RELEASE
@if %release%==0 goto BUILD_RELEASE_NOGUI
%VCBUILD_PATH%\vcbuild /htmllog:build_release.html /logfile:build.log %dorebuild% %PROJECTNAME%.vcproj Release
@if %errorlevel% == 0 goto BUILD_RELEASE_NOGUI
@echo Error on building %PROJECTNAME% Release >> error.log

:BUILD_RELEASE_NOGUI
@if %release_nogui%==0 goto EOF
%VCBUILD_PATH%\vcbuild /htmllog:build_release_nogui.html /logfile:build.log %dorebuild% %PROJECTNAME%.vcproj Release_NoGui
@if %errorlevel% == 0 goto BUILD_END
@echo Error on building %PROJECTNAME% Release_NoGui >> error.log

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
