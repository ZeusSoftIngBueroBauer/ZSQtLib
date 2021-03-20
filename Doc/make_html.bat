@echo off

if "%1" == "" goto help

if "%1" == "help" (
    :help
    echo.Please use `make_html ^<PyVersion^>` where ^<PyVersion^> is one of
    if exist C:\Python26 (
        echo   26 .. for Python 2.6
    )
    if exist C:\Python27 (
        echo   27 .. for Python 2.7
    )
    if exist C:\Python33 (
        echo   33 .. for Python 3.3
    )
    if exist C:\Python34 (
        echo   34 .. for Python 3.4
    )
    goto end
)

if %1% == 26 goto PYTHON26
if %1% == 27 goto PYTHON27
if %1% == 33 goto PYTHON33
if %1% == 34 goto PYTHON34
goto end

:PYTHON26
if exist C:\Python26 (
    set PYTHONVERSION=26
    set PYTHONPATH=C:\Python26
    goto SETPATH
)
echo Python 26 is not installed in C:\Python26
goto end

:PYTHON27
if exist C:\Python27 (
    set PYTHONVERSION=27
    set PYTHONPATH=C:\Python27
    goto SETPATH
)
echo Python 27 is not installed in C:\Python27
goto end

:PYTHON33
if exist C:\Python33 (
    set PYTHONVERSION=33
    set PYTHONPATH=C:\Python33
    goto SETPATH
)
echo Python 33 is not installed in C:\Python33
goto end

:PYTHON34
if exist C:\Python34 (
    set PYTHONVERSION=34
    set PYTHONPATH=C:\Python34
    goto SETPATH
)
echo Python 33 is not installed in C:\Python33
goto end

:SETPATH
echo Using Python Version: %PYTHONVERSION%
echo Using Python Path:    %PYTHONPATH%
echo Inserting %PYTHONPATH% to system path
set PATH=%PYTHONPATH%;"%PATH%"
if "%SPHINXBUILD%" == "" (
    set SPHINXBUILD=%PYTHONPATH%\Scripts\sphinx-build
)
goto MAKE

:MAKE
color 0F
if exist ./build goto RMDIR_BUILD
goto CALL_MAKE_HTML
:RMDIR_BUILD
echo Recursively remove directory
rmdir build /s
:CALL_MAKE_HTML
call make html
pause

GOTO END

:END
