.. _appendix_tools:

Tools
=====

This chapter describes in alphabetic order the tools to be used to build the libraries and
applications based on the ZSQtLib.

7-Zip
-----

.. _Appendix-Tools-7-Zip:

To unzip the Visual Studio 2008 iso image an appropriate Zip program is needed.

7-Zip is a free software which you can download from http://www.7-zip.de/download.html.

GNU Make
--------

.. _Appendix-Tools-GNU Make:

To build the python bindings for the ZSQtLib Dlls GNU make files are used.

For this you need to download and install the GnuWin32 utility package. Get the "GetGnuWin32-0.6.3.exe"
from https://sourceforge.net/projects/getgnuwin32/ and execute the program.

After installation open a command window, switch to the GetGnuWin32 directory and run "download.bat"
selecting a folder in which to place the GetGnuWin32 subfolder (e.g. "C:\\GnuWin32").
If "download.bat" completes successfully you will be able to run the installer.

Assuming you have chosen the Destination folder "C:\\Users\\<User>\\Downloads\\Gnu" on executing the
"GetGnuWin32-0.6.3.exe" program::

        C:\Users\<User>\Downloads\Gnu>cd GetGnuWin32
        C:\Users\<User>\Downloads\Gnu\GetGnuWin32>download
        C:\Users\<User>\Downloads\Gnu\GetGnuWin32>install C:\GnuWin32

The GNU make utility "make.exe" will be installed into the directoy "C:\GnuWin32\bin". Please ensure
that this bin directory is in your PATH environment variable.

Inno Setup
----------

.. _Appendix-Tools-Inno Setup:

Inno Setup is a free installer for Windows programs and is used throughout the whole LTT system
and also by ZSQtLib to create windows installer packages.

You may get the Inno Setup from http://www.jrsoftware.org/isinfo.php.

Microsoft Visual Studio C++ Compiler
------------------------------------

.. _Appendix-Tools-Microsoft Visual Studio C++ Compiler:

In order to build the library and the applications you need at least a C++ compiler. Taking into
account that python bindings should be created for the ZSQtLib the installed python interpreter
defines which compiler must be used. This is because the binaries to be loaded by the python
interpreter must have been compiled with the same compiler and C runtime libraries as the python
interpreter itself.

Asuming you have used the standard way to install the python interpreter using the corresponding
installers the following table shows which compiler has to be used.

+----------------+------------------------------------+------------------------------+
|Python Versions | Visual Studio Product Name         | Visual Studio Version Number |
+================+====================================+==============================+
|2.6, 2.7        | 2008 (Express Edition can be used) | 9.0                          |
+----------------+------------------------------------+------------------------------+
|3.3, 3.4        | 2010 (Express Edition can be used) | 10.0                         |
+----------------+------------------------------------+------------------------------+
|3.5             | 2015 (Express Edition can be used) | 14.0                         |
+----------------+------------------------------------+------------------------------+

.. Note::
   Please note that at this time (January 2017) Python Versions 3.3, 3.4 and 3.5 are not yet
   supported. To support Python Version 3.3 and 3.4 the GNU make files and the visual studio
   project and solution files to build the ZSQtLib, the Python Bindings and the Applications
   must be copied and renamed and adapted correspondingly. To support Python Version 3.5 PySide
   would have to be completely rebuild with Visual Studio 2015 - and this a lot of effort.

.. Note::
   In order to install VS 2008 Express Edition on a Windows 2008 Server you may need to add the
   windows feature .NET Framework 3.5.1 in advance via::

        Control Panel - Programs and Features - Turn Windows Features on or off - Features - Add Features

Also the "vcredist_x86.exe" program is needed to distribute the visual studio runtime dlls on
installing C++ applications on the target computer.

MinGW GCC
---------

.. _Appendix-Tools-MinGW GCC:

For generating list of included header files (Auto Dependency) the GCC compiler is used. Please
download the compiler from http://tdm-gcc.tdragon.net/download and install the package.

Python
------

.. _Appendix-Tools-Python:

You may download python from https://www.python.org/downloads/. Please follow the instructions
provided by the python communicate to install the python interpreter.

PySide
------

.. _Appendix-Tools-PySide:

PySide is a python site package and can simply be installed using pip as follows::

    C:\Python27\Scripts>pip install pyside

For a more detailled description about PySide and its installation and build process you may visit
http://pyside.readthedocs.io/en/latest/index.html.

For more details about the PySide project you may visit https://wiki.qt.io/PySide. On this page
you may also find the link https://setanta.wordpress.com/binding-c/ which (hopefully) leads you to
a webpage where you can find the distillation of design decisions made by the original PySide
developers on how to create the C++ Bindings as well as developing and using Shiboken.

Qt Framework
------------

.. _Appendix-Tools-Qt Framework:

Taking into account that python bindings should be created for the ZSQtLib also the binaries of the
Qt Framework must have been compiled using one of the compilers mentioned above.

You may download the Qt package from the Qt Download Site https://www.qt.io/download/. Answer a view
questions and then choose to "View All Downloads" https://www.qt.io/download-open-source/. Scroll
down to *Source packages & Other releases* and choose the link http://download.qt.io/archive/qt/
pointing to the archive of all older versions of Qt. From there you can select Qt version 4.8
and below Qt version 4.8.6. From the next page you can select and download the installer suitable
for your compiler and platform (see table below).

But there is also on official release for "qt-opensource-windows-x86-vs2008-4.8.7" available.
Unfortunately it was not possible to get to the official releases web site from the official
Qt download site. But you may try the link https://download.qt.io/official_releases/qt/.
If this link is working you can select Qt version 4.8 and below Qt version 4.8.7. From the next#
page you can select and download the installer suitable for your compiler and platform:

+----------------+----------------------------------------+
|Python Versions | Visual Studio Versions                 |
+================+========================================+
|2.6, 2.7        | qt-opensource-windows-x86-vs2008-4.8.7 |
+----------------+----------------------------------------+
|3.3, 3.4        | qt-opensource-windows-x86-vs2010-4.8.7 |
+----------------+----------------------------------------+
|3.5             | not (yet) supported                    |
+----------------+----------------------------------------+

To install the Qt framework simply execute the dowloaded executable file.

Shiboken
--------

.. _Appendix-Tools-Shiboken:

Shiboken is a GeneratorRunner plugin that outputs C++ code for CPython extensions and is used
to create Python language bindings for the Qt framework. Shiboken is installed together with
PySide and available in the site package sub directory. But you may also download and install
shiboken separately following the instructions on https://pypi.python.org/pypi/Shiboken/1.2.2.
But there is really absolutely no need to do so as also the documentation of shiboken is installed
below the PySide site package directory. Unfortunately the shiboken documentation is not up to date.
E.g. the documentation of the generator still refers to Boost.Python in spite of Boost.python has
been replaced by Shiboken. In some points the documentation can rather be more confusing than
explanatory. Also a Step by Step tutorial for more complex modules than a simply foo class is
missing.
