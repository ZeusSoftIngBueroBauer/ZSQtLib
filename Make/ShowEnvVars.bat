@echo off

REM ..\..\ShowEnvVars.bat
REM

REM ..\..\CopyOutput.bat $(MSVCVersion) $(Platform) $(OutDir) $(TargetName) $(IncludePath) $(VC_IncludePath) $(WindowsSDK_IncludePath) $(VC_ExecutablePath_x86) $(WindowsSDK_ExecutablePath) $(VS_ExecutablePath) $(MSBuild_ExecutablePath) $(VC_LibraryPath_x86)
REM
REM    %1: MSVCVersion = [msvc2015, msvc2017, msvc2019]
REM    %2: Platform    = [Win32, x64]
REM    %3: OutDir      = [..\..\..\Tmp\Out\ZSSys\Win32\Debug, ..]
REM    %4: TargetName  = [ZSSys, ZSSysd, ZSSysGUI, ZSSysGUId, ..]

set MSVC_Version=%1
set Platform=%2
set OutDir=%3
set TargetName=%4
set IncludePath=%5%
set VC_IncludePath=%6%
set WindowsSDK_IncludePath=%7%
set VC_ExecutablePath_x86=%8%
set WindowsSDK_ExecutablePath=%9%
set VS_ExecutablePath=%10%
set MSBuild_ExecutablePath=%11%
set VC_LibraryPath_x86=%12%

echo MSVC_Version=%MSVC_Version%
echo Platform=%Platform%
echo OutDir=%OutDir%
echo TargetName=%TargetName%
echo IncludePath=%IncludePath%
echo VC_IncludePath=%VC_IncludePath%
echo WindowsSDK_IncludePath=%WindowsSDK_IncludePath%
echo VC_ExecutablePath_x86=%VC_ExecutablePath_x86%
echo WindowsSDK_ExecutablePath=%WindowsSDK_ExecutablePath%
echo VS_ExecutablePath=%VS_ExecutablePath%
echo MSBuild_ExecutablePath=%MSBuild_ExecutablePath%
echo VC_LibraryPath_x86=%VC_LibraryPath_x86%
