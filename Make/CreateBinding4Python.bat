@echo off

REM $(ZSQTLIB_DIR)\Make\CreateBinding4Python.bat $(PLATFORM) $(CONFIG) $(MODULE)
REM 
REM    %1: PLATFORM = [Win32]
REM    %2: CONFIG   = [Release, Debug]
REM    %3: MODULE   = [ZSSys, ZSSysGUI, ZSIpc, ZSIpcGUI, ZSTrace, ZSTraceGUI, ZSRemCmd, ZSRemCmdGUI]

set PLATFORM=%1
set CONFIG=%2
set MODULE=%3

set CURR_DIR=%CD%
set BATCH_DIR=%~dp0%

if not defined PYTHONDIR set PYTHONDIR=C:\Python27
if not defined PYTHONVERSION set PYTHONVERSION=27
if not defined ZSQTLIB_DIR set ZSQTLIB_DIR=%BATCH_DIR:\Make=%

echo PLATFORM=%PLATFORM%
echo CONFIG=%CONFIG%
echo MODULE=%MODULE%
echo CURR_DIR=%CURR_DIR%
echo BATCH_DIR=%BATCH_DIR%
echo PYTHONDIR=%PYTHONDIR%
echo PYTHONVERSION=%PYTHONVERSION%
echo ZSQTLIB_DIR=%ZSQTLIB_DIR%

goto EXE

Shiboken commad line options:
-----------------------------

--avoid-protected-hack

--disable-verbose-error-messages
  Disable verbose error messages. Turn the CPython code hard to debug but saves a few kilobytes in the generated binding.

--enable-parent-ctor-heuristic
  This flag enable an useful heuristic which can save a lot of work related to object ownership when writing the typesystem. For more info, check Parentship heuristics.

--enable-pyside-extensions
  Enable pyside extensions like support for signal/slots. Use this if you are creating a binding based on PySide.

--enable-return-value-heuristic
  Enable heuristics to detect parent relationship on return values. For more info, check Return value heuristics.

--api-version=<version>
  Specify the supported api version used to generate the bindings.

--debug-level=[sparse|medium|full]
  Set the debug level.

--documentation-only
  Do not generate any code, just the documentation.

--drop-type-entries="<TypeEntry0>[;TypeEntry1;...]"
  Semicolon separated list of type system entries (classes, namespaces, global functions and enums) to be dropped from generation.

--generation-set
  Generator set to be used (e.g. qtdoc).

--help
  Display this help and exit.

--include-paths=<path>[:<path>:...]
  Include paths used by the C++ parser.

--license-file=[license-file]
  File used for copyright headers of generated files.

--no-suppress-warnings
  Show all warnings.

--output-directory=[dir]
  The directory where the generated files will be written.

--silent
  Avoid printing any message.

--typesystem-paths=<path>[:<path>:...]
  Paths used when searching for type system files.

--version
  Output version information and exit.

:EXE

set OPTIONS=--enable-pyside-extensions --avoid-protected-hack --api-version=4.8.7

set INCPATHs=
set INCPATHs=%INCPATHs%;%ZSQTLIB_DIR%\Include
set INCPATHs=%INCPATHs%;%ZSQTLIB_DIR%\Make
set INCPATHs=%INCPATHs%;%ZSQTLIB_DIR%\Make\%MODULE%
set INCPATHs=%INCPATHs%;%PYTHONDIR%\Lib\site-packages\PySide\include\PySide
set INCPATHs=%INCPATHs%;%QTDIR%\include

set TYPESYSPATHs=%PYTHONDIR%\Lib\site-packages\PySide\typesystems
set TYPESYSPATHs=%TYPESYSPATHs%;%ZSQTLIB_DIR%\Make\ZSSys
set TYPESYSPATHs=%TYPESYSPATHs%;%ZSQTLIB_DIR%\Make\ZSSysGUI
set TYPESYSPATHs=%TYPESYSPATHs%;%ZSQTLIB_DIR%\Make\ZSIpc
set TYPESYSPATHs=%TYPESYSPATHs%;%ZSQTLIB_DIR%\Make\ZSIpcGUI
set TYPESYSPATHs=%TYPESYSPATHs%;%ZSQTLIB_DIR%\Make\ZSTrace
set TYPESYSPATHs=%TYPESYSPATHs%;%ZSQTLIB_DIR%\Make\ZSTraceGUI
set TYPESYSPATHs=%TYPESYSPATHs%;%ZSQTLIB_DIR%\Make\ZSTest
set TYPESYSPATHs=%TYPESYSPATHs%;%ZSQTLIB_DIR%\Make\ZSTestGUI
set TYPESYSPATHs=%TYPESYSPATHs%;%ZSQTLIB_DIR%\Make\ZSRemCmd
set TYPESYSPATHs=%TYPESYSPATHs%;%ZSQTLIB_DIR%\Make\ZSRemCmdGUI

set OUTDIR=%ZSQTLIB_DIR%\Tmp\Shiboken\Python27\%PLATFORM%\%CONFIG%

echo %PYTHONDIR%\lib\site-packages\PySide\shiboken.exe %OPTIONs% --include-paths=%INCPATHS% --typesystem-paths=%TYPESYSPATHs% --output-directory=%OUTDIR% %ZSQTLIB_DIR%\Make\%MODULE%\%MODULE%Binding4Python.h %ZSQTLIB_DIR%\Make\%MODULE%\%MODULE%Binding4Python.xml
%PYTHONDIR%\lib\site-packages\PySide\shiboken.exe %OPTIONs% --include-paths=%INCPATHS% --typesystem-paths=%TYPESYSPATHs% --output-directory=%OUTDIR% %ZSQTLIB_DIR%\Make\%MODULE%\%MODULE%Binding4Python.h %ZSQTLIB_DIR%\Make\%MODULE%\%MODULE%Binding4Python.xml
