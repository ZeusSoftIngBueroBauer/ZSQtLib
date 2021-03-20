@echo off

@SET VSINSTALLDIR=C:\Program Files (x86)\Microsoft Visual Studio 9.0
@SET VCINSTALLDIR=C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC

if not exist "%VCINSTALLDIR%\bin\vcvars32.bat" goto missing
call "%VCINSTALLDIR%\bin\vcvars32.bat"
goto eof

:missing
@echo missing "%VCINSTALLDIR%\bin\vcvars32.bat"

:eof
