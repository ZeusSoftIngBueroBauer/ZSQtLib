In order to create python bindings for C++ modules using the Qt library shiboken is the recommended tool.

Shiboken has been chosen as the binding generator for the ZSQtLib because:

- PySide (Python for Qt) is used as the python bindings for the Qt framework as PySide is a documented
  open source software project and is provided by the Qt project itself.
- PySide has been generated using shiboken. Even more the shiboken binding generator has been more or
  less developed for the PySide project to provide a complete toolchain for rapidly generating bindings
  for any Qt-based C++ class hierarchies.
- The shiboken generator together with the complete type system files and all header files necessary to
  create python bindings for C++ modules using the Qt framework are installed together with PySide.

During the development phase of creating python bindings it is very likely necessary to also have a debugging
version of python and the PySide modules available. As the installable packages of Python and PySide only provide
release versions you need to compile and build Python and PySide on yourself. Also packages Python and PySide
depend on must be compiled and build to get a debuggable version.

This document is split into three sections:

- The first section describes how to create the ZSQtLib with the remote command server and ..

TODO: first -> second

- The first section describes how to generate python bindings for C++ modules using the Qt framework
  and install them as a python site package on the target PC.
- The second section describes how to setup a PC in order to be able to debug the bindings.
  This includes creating debug versions of Python and PySide and all dependencies of the Python and
  PySide packages.


Create python bindings
**********************

Prerequisites
=============

Overview
--------

TODO: Liste der Tools und deren Abhaengigkeiten ... (warum welches Tool in welcher Reihenfolge)

Qt
--

Python Versions | Qt Versions
----------------+---------------------------------------
2.6, 2.7        | qt-opensource-windows-x86-vs2008-4.8.7
3.3, 3.4        | qt-opensource-windows-x86-vs2010-4.8.7

PySide
------

PySide is the Python Qt bindings project, providing access the complete Qt 4.8 framework as well
as to generator tools for rapidly generating bindings for any C++ libraries.

PySide requires Python 2.6 or later and Qt 4.6 or better (Qt 5.x is currently (Jan. 2017) not supported).

Use latest version of pip to install the wheel binary packages:

    pip install PySide

The site package PySide will be created containing release versions of the Qt Dlls and shiboken as well
as header files and typesystem XML files necessary to create python bindings using shiboken.

You must verify that in "pyside_global.h" (in directory e.g. "C:\Python27\Lib\site-packages\PySide\include\PySide")
the correct include path to the Qt header files is used. After newly installing PySide the Qt include paths are very
likely set to be below "D:/Qt/qt-4.8.7-msvc2008-x32" (which very likely is not the correct path). You manually need
to replace all the absolute Qt-Include paths in "pyside_global.h" to point to the correct installation directory of Qt.

In addition the "pyside_global.h" files includes pyside header files which have not been installed with pyside
and are not existing. Those header files have to be uncommented. Those are:
    "pysidewtypes.h"
    "pysideqtesttouch.h"
    "phonon/pyside_phonon.h"

Compiler
--------

The Dlls for which python bindings should be created must be compiled and linked
with the appropriate Visual Studio Version.

Python Versions | Visual Studio Versions
----------------+------------------------------------
2.6, 2.7        | 2008 (Express Edition can be used)
3.3, 3.4        | 2010 (Express Edition can be used)
3.5             | 2015 (Express Edition can be used)

In order to install VS 2008 Express Edition on a Windows 2008 Server you may add the
windows feature .NET Framework 3.5.1 in advance:

    Control Panel - Programs and Features - Turn Windows Features on or off - Features - Add Features.

Also the "vcredist_x86.exe" program is needed to distribute the visual studio runtime dlls
on installing C++ applications on the target computer.

7-Zip
-----

To unzip the VS 2008 iso image an appropriate Zip program is needed.

GNU Make
--------

To build the python bindings for the ZSQtLib Dlls GNU make files are used.

MinGW GCC
---------

Auto Dependency .... (Dependency Walker)


For this you need to download and install the GnuWin32 utility package. Get the "GetGnuWin32-0.6.3.exe" from
https://sourceforge.net/projects/getgnuwin32/ and execute the program.

After installation open a command window, switch to the GetGnuWin32 directory and run "download.bat"
selecting a folder in which to place the GetGnuWin32 subfolder (e.g. "C:\GnuWin32").
If "download.bat" completes successfully you will be able to run the installer.

For example:

    Assuming you have chosen the Destination folder "C:\Users\<User>\Downloads\Gnu" on executing the
    "GetGnuWin32-0.6.3.exe" program:

        C:\Users\<User>\Downloads\Gnu>cd GetGnuWin32
        C:\Users\<User>\Downloads\Gnu\GetGnuWin32>download
        C:\Users\<User>\Downloads\Gnu\GetGnuWin32>install C:\GnuWin32

The GNU make utility "make.exe" will be installed into the directoy "C:\GnuWin32\bin". Please ensure that
this bin directory is in your PATH environment variable.

Inno Setup
----------


Generate Bindings
=================

Please note that this chapter only relates only to create release versions of the bindings.
To create debug versions you need a lot more prerequisites and must set up you PC accordingly.
E.g. you must be able to compile python and PySide in debug version. How you can set up your
PC to get a debuggable version of the bindings is treated in more detail in another chapter.

Compile C++ Dlls
----------------

The Dlls for which python bindings should be created must be compiled and linked
with the appropriate Visual Studio Version.

    Python Versions | Visual Studio Versions
    ----------------+------------------------------------
    2.6, 2.7        | 2008 (Express Edition can be used)
    3.3, 3.4        | 2010 (Express Edition can be used)
    3.5             | 2015 (Express Edition can be used)

Lets assume we are using python version 2.7 and we want to create python bindings of the ZSSys dll.

To get a compiled version of the ZSSys dll we can either:

Build from Visual Studio GUI
++++++++++++++++++++++++++++

Open the visual studio solution "ZSQtLibBindings4Python_9.0.sln" (9.0 is the version number of VS 2008),
select "Release" configuration and "Win32" platform, right click on the ZSSys project and build the project.

Build from windows command prompt via make file
+++++++++++++++++++++++++++++++++++++++++++++++

You may open the visual studio command prompt via

  - Start
    - Programs
      - Microsoft Visual Studio 2008 Express Edition
        - Visual Studio Tools
          - Visual Studio 2008 Command Prompt

and change to the directory "ZSQtLib\Make\ZSys\Windows".

You may also open a command window and execute "vcvarsall.bat x86" located in directory
"C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC" manually in order to set up some
environment variables necessary for the Visual Studio 2008 make utility. There is also a
batch command file "vcvarsall_9.0_x86.bat" in the "ZSQtLib\Make\Windows" directory which
you may use.

To build the target "ZSSys.dll" invoke "make -f ZSSys.mk release".

Outputs of the build process of ZSSys:

    Directory         | File
    ------------------+-----------
    ZSQtLib\Lib\Win32 | ZSSys.lib
    ZSQtLib\Bin\Win32 | ZSSys.dll
    ZSQtLib\Bin\Win32 | ZSSys.pdb

Create python bindings for the C++ modules
++++++++++++++++++++++++++++++++++++++++++

- Create the global header file for the Dll for which bindings have to be created
  (e.g. "ZSSysBinding4Python.h" for the "ZSSys" dll).
  This global header file must include all header files for all C modules for which
  bindings have to be created (see "ZSSysBinding4Python.h" as an example).

- Create type system XML file for the Dll for which bindings have to be created.
  (e.g. "ZSSysBinding4Python.xml" for the "ZSSys" dll).

- A GNU make file has been provided to create the python bindings for the ZSSys modules.
  Again open a visual studio command prompt as described above, change to the directory
  "ZSQtLib\Make\ZSys\Windows" and invoke "make -f ZSSysBinding4Python.mk release".

  On executing this make file shiboken is called to create header and cpp files for the
  bindings. To simplify this job the batch command "CreateBinding4Python.bat" is provided.
  This batch file will be implicitly called by the make command providing three command
  arguments as follows:

    ..\..\CreateBinding4Python.bat $(Platform) $(Config) $(Module)

        Platform = [Win32]
        Config   = [Release, Debug]
        Module   = [ZSSys, ZSSysGUI, ZSIpc, ZSIpcGUI, ZSTrace, ZSTraceGUI, ZSRemCmd, ZSRemCmdGUI]

  To create bindings for "ZSSys" the batch job is called for example:

    ..\..\CreateBinding4Python.bat Win32 Release ZSSys

  If you get a lot of warnings like "... is specified in typesystem, but not declared .." very likely
  the include paths in "pyside_global.h" are wrong (see "troubleshooting" section on creating bindings
  using shiboken later on).

Outputs of the build process of ZSSysBinding4Python:

    The output of shiboken is stored in directory "ZSQtLib\Tmp\Shiboken\Python27\Win32\Release\ZSQtLib\ZSSys":

        - For each exported namespace a header and a cpp file named "<namespace>_[<namespace>_]wrapper" is created.
          Which namespace has to be exported is defined in the typesystem file "ZSSysBinding4Python.xml".
          Examples:
            For the namespace ZS::Trace:
                "zs_trace_wrapper.h" and "zs_trace_wrapper.cpp"
        - For each exported type (class or structure) a header and a cpp file named
          "<namespace>_[<namespace>_]<typename>_wrapper" is created.
          Examples:
            For class "ZS::CEnumeration" defined and implemented in module "ZSSysEnum":
                "zs_cenumeration_wrapper.h" and "zs_cenumeration_wrapper.cpp"
            For struct "ZS::SEnumEntry" defined and implemented in module "ZSSysEnum":
                "zs_cenumeration_wrapper.h" and "zs_cenumeration_wrapper.cpp"
            For class "ZS::Trace::CTrcAdminObj" defined and implemented in module "ZSSysTrcAdminObj":
                "zs_senumentry_wrapper.h" and "zs_senumentry_wrapper.cpp"
        - For each exported enumeration conversion routines are added to the global array
          "SbkZSQtLib_ZSSysTypes" in module "zssys_module_wrapper.cpp".

    Each generated shiboken wrapper module will be compiled into an C++ object file and linked
    into the library "ZSSysBinding4Python". The resulting target "ZSSysBinding4Python.dll" is stored
    in "ZSQtLib\Tmp\Out\ZSSysBinding4Python\Win32\Release".

    In order to import the generated binding the target dll got to be renamed by removing "Binding4Python"
    and changing the extension into "*.pyd" (e.g. copy "ZSSysBinding4Python.dll" "ZSSys.pyd").

Problems during the development phase creating bindings using shiboken
----------------------------------------------------------------------

- Using wrong Qt include path in "pyside_global.h"

  The compilation error ""qt_metaobject" not defined" very likely indicates that the wrong Qt include paths are used.

  You must verify that in "pyside_global.h" (in directory e.g. "C:\Python27\Lib\site-packages\PySide\include\PySide")
  the correct include path the Qt header files is used. After newly installing PySide the Qt include paths are very likely
  set to be below "D:/Qt/qt-4.8.7-msvc2008-x32" (which is very likely is wrong). You need to manually replace all the
  absolute Qt-Include paths in "pyside_global.h" to point to the correct installation directory of Qt.

  Qt files must be included via "pyside_global.h" but not via "global.h" (which has been renamed to "$(ProjectName)Binding4Python.h" in our case).

- Using wrong Qt API version on creating bindings:

  On importing the binding in a python module getting the Exception "ImportError: DLL load failed: the specified procedure could not be found."
  very likely indicates that the wrong Qt API version was used by shiboken on parsing the typesystem XML files. The Qt API version must be
  explicitly set (even if only Qt 4.8.7 is installed on your computer).

  Shiboken needed the following options to create bindings with Qt 4.8.7:

    --enable-pyside-extensions --avoid-protected-hack --api-version=4.8.7

  A hint to an invalid Qt API version is also if on creating the bindings with shiboken many warnings are output.
  If e.g. (among many other warning) the warning
  "'QPersistentModelIndex' is specified in typesystem, but not defined. This could potentially lead to compilation errors"
  is output this indicates that in the type system XML file the type 'QPersistentModelIndex' has been defined but the corresponding
  definition in a header file cannot be found and the wrong Qt API version is assumed on parsig the typesystem XML files by shiboken
  on creating the bindings.

- Incompatible compiler and Qt versions

  It is important that the bindings are compiled with the same compiler as Python.
  This implies that the installed PySide Dlls are also compiled with the same compiler as Python.
  It is also important the the bindings are compiled and linked with a binary compatible Qt version as the installed PySide Dlls.

    Python Versions | Visual Studio Versions
    ----------------+------------------------------------
    2.6, 2.7        | 2008 (Express Edition can be used)
    3.3, 3.4        | 2010 (Express Edition can be used)
    3.5             | 2015 (Express Edition can be used)

- Wrong include paths of header files for the project to be bound

  If you get a shiboken error like

  "type 'ZS::System::CObjPoolListEntry' is specified in typesystem, but not defined. This could potentially lead to compilation errors."

  then very likely the include path to the header files is wrong and the corresponding header file
  could not be included in the globar binding 4 python header file (e.g. "ZSSysBinding4Python.h").
  Unfortunately shiboken does not indicate an error if the include file cannot be opened.
  To fix this issue you have to adapt the used include path passed to shiboken via --include-paths.

- Compiler error "<EnumValue> does not belong to <class>" on setting default values for method parameters.
  The whole qualified name of the enum value must be specified to initialize default arguments.
  E.g. "int i_idxAliasStr = ZS::System::EEnumEntryAliasStrIdxName" (see "ZSSysEnum.h").


2. Setting up a PC for developing python bindings
*************************************************

The following prerequisites must be installed in order to develop python bindings for C++ modules
using the Qt framework.

1.1 C++ Compiler
================

To get a debug version of the Python interpreter you need to compile and link the program on yourself.
For this you need to install the appropriate C compiler.

Python 3.5 and later use Microsoft Visual Studio 2015.
You can download and use any of the free or paid versions of Visual Studio 2015.
Installing the latest updates is also recommended.

Python 3.3 and 3.4 use Microsoft Visual Studio 2010.
You can download Microsoft Visual Studio 2010 Express with an MSDN subscription.
You’ll also need to install the Visual Studio Service Pack 1 (SP1).
If you don’t install this service pack, you may receive errors like the following
during linking:
    LINK : fatal error LNK1123: failure during conversion to COFF:file invalid or corrupt.

Most Python versions prior to 3.3 use Microsoft Visual Studio 2015.
You can download Microsoft Visual C++ 2008 Express Edition with SP1 from a new
location yet to be determined.

Python Versions | Visual Studio Versions
----------------+------------------------------------
2.6, 2.7        | 2008 (Express Edition can be used)
3.3, 3.4        | 2010 (Express Edition can be used)
3.5             | 2015 (Express Edition can be used)

1.2. Python
===========

To compile also debug versions of the python bindings for the ZSQtLib dlls you need to get
a working copy of the source code of the CPython interpreter (CPython is the version of Python
available from http://www.python.org/) and compile the CPython interpreter with the appropriate
Visual Studio compiler.

1.2.1. Version Control Setup
----------------------------

CPython is developed using Mercurial. The Mercurial command line program is named hg; this is also used to refer
to Mercurial itself. Mercurial is easily available for common Unix systems by way of the standard package manager;
under Windows, you might want to use the TortoiseHg graphical client, but the build system still prefers hg.exe
to be on your PATH.

To get the version control software download e.g. "tortoisehg-4.0.1-x64.msi" from http://tortoisehg.bitbucket.org/
and install the package.

In order to install the build depenendencies (e.g. sqlite) you need the command line tool "svn.exe".
For this download "TortoiseSVN" (e.g. "TortoiseSVN-1.9.5.27581-x64-svn-1.9.5.msi") from
https://tortoisesvn.net/downloads.html/ and install the package. On installing the package you need to select
"command line client tools" (this option is deselected as default). Please note that "TortoiseSVN" replaces some
(or a lot) functions from "tortoisehg" installed before (especially the build in tools for the Windows Explorer).

1.2.2. Getting the Source Code
------------------------------

One should always work from a working copy of the CPython source code. While it may be tempting to work from the
copy of Python you already have installed on your machine, it is very likely that you will be working from
out-of-date code as the Python core developers are constantly updating and fixing things in their VCS.
It also means you will have better tool support through the VCS as it will provide a diff tool, etc.
To get a working copy of the in-development branch of CPython (core developers use a different URL as outlined
in How to Become a Core Developer), run:

    cd C:\Python27
    hg clone https://hg.python.org/cpython

If you want a working copy of an already-released version of Python, i.e., a version in maintenance mode, you
can update your working copy. For instance, to update your working copy to Python 2.7, do:

    cd C:\Python27\cpython
    hg update 2.7

1.2.3. Compiling (for debugging)
--------------------------------

1.2.3.1. Dependencies
'''''''''''''''''''''

In order to install the build depenendencies (e.g. sqlite) you need to invoke "get_externals.bat" which is located
in "C:\Python27\cpython\PCbuild". If you get the error that "svn.exe" must be in your path you need to install
"svn.exe" (see Version Control Setup above). If you have already done so but still get the error "svn.exe" must be
on your path on executing "get_externals.bat" rerun the installer, select Modify, and select command line tools
for installation.

1.2.3.2. Python Binaries
''''''''''''''''''''''''

Open the solution "pcbuild.sln" located in "C:\Python27\cpython\PC\VS9.0" and build the solution.

To build from the Visual Studio GUI, open the "pcbuild.sln" solution file located in "C:\Python27\cpython\PC\VS9.0"
with Visual Studio. Choose the Build Solution option under the Build menu. Be sure that “Debug” was chosen as the
active solution configuration (e.g. under Build - Configuration Manager...).

When building you may see a number of build errors related to missing files or directories. These do not necessarily
mean that Python failed to build. If you prefer, you can exclude the offending projects from the build process by
unchecking them inside the Build - Configuration Manager... settings. You can also use the script
"PCbuild\get_externals.bat" to download missing dependencies (see Build dependencies above).

Once built you might want to set Python as a startup project. Pressing F5 in Visual Studio, or choosing Start
Debugging from the Debug menu, will launch the interpreter.

To provide the debug version of the executable of the python interpreter you need to copy
"python_d.exe" and "python27_d.dll" from "C:\Python27\cpython\PC\VS9.0" to "C:\Python27".

To provide the debug version of the python library you need to copy "python27_d.lib"
from "C:\Python27\cpython\PC\VS9.0" to "C:\Python27\libs".

1.3. PySide
===========

The following is an extract from http://pyside.readthedocs.io/en/latest/building/windows.html.
Please read this web site and click on the corresponding links to download the mentioned prerequisites.

1.3.1. Install Qt 4.8 libraries
-------------------------------

- for Windows VS 2008 edition when building against Python 2.6, 2.7 or 3.2.
- for Windows VS 2010 edition when building against Python 3.3 or 3.4.

Python Versions | Qt Versions
----------------+------------------------------------
2.6, 2.7, 3.2   | 4.8.7 (VS 2008)
3.3, 3.4        | 4.8.7 (VS 2010)

1.3.2. Install Cmake
--------------------

1.3.4  Install Windows SDK
--------------------------

- v7.0 when building against Python 2.6, 2.7 or 3.2.
- v7.1 when building against Python 3.3 or 3.4.

1.3.5. Install Git
------------------

1.3.6. Install OpenSSL (NOT optional)
-------------------------------------

1.3.7. Building PySide distribution
-----------------------------------

Clone PySide setup scripts from git repository:

    Create directory: C:\PySide-1.2.4
    cd C:\PySide-1.2.4
    git clone https://github.com/PySide/pyside-setup.git pyside-setup

Build and install release version:

    - Switch to the pyside-setup directory and build the wheel binary distribution (this may take a while ...):

        C:\PySide-1.2.4\pyside-setup>c:\Python27\python setup.py bdist_wheel --version=1.2.4 --qmake=c:\Qt\4.8.7\bin\qmake.exe

    - After the successfull build, install the release distribution with pip:

        C:\Python27\Scripts>pip install C:\PySide-1.2.4\pyside-setup\dist\PySide-1.2.4-cp27-none-win32.whl

Build and install debug version:

    - Switch to the pyside-setup directory and build the wheel binary distribution (this may take a while ...).
      To build the Debug version add the --debug option:

        C:\PySide-1.2.4\pyside-setup>c:\Python27\python setup.py bdist_wheel --version=1.2.4 --debug --qmake=c:\Qt\4.8.7\bin\qmake.exe

        If getting error message "directory build/... is not empty" simply remove the sub-directories of the "build" directory.

    - After the successfull build, install the debug distribution with pip:

        C:\Python27\Scripts>pip install C:\PySide-1.2.4\pyside-setup\dist\PySide-1.2.4-cp27-none-win32.whl


Debugging Possibilities
-----------------------

For debugging purposes the following lines may be inserted into the generated cpp file:

    QString strPath = "C:\\Dialog Semiconductor\\RemCmdClient\\Examples\\Log\\ZSRemCmdBinding4Python";
    QDir trcDir;
    if( !trcDir.exists(strPath) )
    {
        trcDir.mkpath(strPath);
    }

    QFile trcFile(strPath + "\\zs_remcmd_remcmdclient_wrapper.log");

    if( trcFile.open(QIODevice::WriteOnly|QIODevice::Text) )
    {
        QTextStream strmTrcFile(&trcFile);
        strmTrcFile << "Line 1";
        trcFile.close();
    }

For debugging purposes the body of the typeDiscovery functions of message classes may be replaced as follows:

    static void* Sbk_ZS_CMsg_typeDiscovery(void* cptr, SbkObjectType* instanceType)
    {
        QString strPath = "C:\\Dialog Semiconductor\\RemCmdClient\\Examples\\Log\\ZSRemCmdBinding4Python";
        QDir trcDir;
        if( !trcDir.exists(strPath) )
        {
            trcDir.mkpath(strPath);
        }

        QFile trcFile(strPath + "\\zs_remcmd_remcmdclient_wrapper.log");

        void* cptrTmp = 0;

        ::QEvent*   pEv = 0;
        ::ZS::CMsg* pMsgEv = 0;
        ::QObject*  pObj = 0;
        ::ZS::CMsg* pMsgObj = 0;

        QString strAddTrcInfo = "Sbk_ZS_CMsg_typeDiscovery( cptr: " + ZS::pointer2Str(cptr) + ", instanceType: " + ZS::pointer2Str(instanceType) + " )";

        if (instanceType == reinterpret_cast<SbkObjectType*>(Shiboken::SbkType< ::QEvent >()))
        {
            pEv = reinterpret_cast< ::QEvent*>(cptr);
            pMsgEv = dynamic_cast< ::ZS::CMsg*>(pEv);
        }
        if (instanceType == reinterpret_cast<SbkObjectType*>(Shiboken::SbkType< ::QObject >()))
        {
            pObj = reinterpret_cast< ::QObject*>(cptr);
            pMsgObj = dynamic_cast< ::ZS::CMsg*>(pObj);
        }

        if( pEv != 0 )
        {
            strAddTrcInfo += ", EvType: " + QString::number(pEv->type());
        }
        if( pObj != 0 )
        {
            strAddTrcInfo += ", ObjName: " + pObj->objectName();
        }

        if( pMsgEv != 0 )
        {
            strAddTrcInfo += ", EvIsBaseMsgType: " + ::ZS::bool2Str(pMsgEv->isBaseMsgType());
            strAddTrcInfo += ", EvSystemMsgType: " + QString::number(pMsgEv->getSystemMsgType());
            strAddTrcInfo += ", EvMsgType: " + QString::number(pMsgEv->getMsgType());

            if ( pMsgEv->isBaseMsgType() && pMsgEv->getMsgType() == ZS::EBaseMsgTypeUndefined)
                cptrTmp = cptr;
        }

        if( pMsgObj != 0 )
        {
            strAddTrcInfo += ", ObjIsBaseMsgType: " + ::ZS::bool2Str(pMsgObj->isBaseMsgType());
            strAddTrcInfo += ", ObjSystemMsgType: " + QString::number(pMsgObj->getSystemMsgType());
            strAddTrcInfo += ", ObjMsgType: " + QString::number(pMsgObj->getMsgType());

            if ( pMsgObj->isBaseMsgType() && pMsgObj->getMsgType() == ZS::EBaseMsgTypeUndefined)
                cptrTmp = cptr;
        }

        if( trcFile.open(QIODevice::Append|QIODevice::Text) )
        {
            strAddTrcInfo += ", cptr = " + ZS::pointer2Str(cptr);
            strAddTrcInfo += ", pEv = " + ZS::pointer2Str(pEv);
            strAddTrcInfo += ", pObj = " + ZS::pointer2Str(pObj);
            strAddTrcInfo += ", pMsgEv = " + ZS::pointer2Str(pMsgEv);
            strAddTrcInfo += ", pMsgObj = " + ZS::pointer2Str(pMsgObj);
            strAddTrcInfo += ", cptrRet = " + ZS::pointer2Str(cptrTmp);

            QTextStream strmTrcFile(&trcFile);
            strmTrcFile << strAddTrcInfo;
            trcFile.close();
        }

        return cptrTmp;
    }
