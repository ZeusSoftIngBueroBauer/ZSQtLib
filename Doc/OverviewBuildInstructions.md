@page _PAGE_Overview_BuildInstructions Build Instructions

# Quick Startup

## Create and Install ZSQtLib Libraries, Products and Test Applications

> **Please verify that your compiler supports the desired architecture (platform).
VS-2008 express edition for example does not support the X64 architecture.**

> **Please ensure that the path to the repository and the build directory don't include
white spaces as this may break the build with MinGW.**

To build the ZSQtLib the Qt Framework must have been installed in advance.
Please refer to main CMakeList file which versions of the Qt Framework are supported.

On generating the build tree with the first cmake call the version numbers will be automatically created
from the repository's git commit and written into `Include\Libs\ZSSys\ZSSysVersion.h`.

**Recommended Build Directory**

- ./Build/CompilerName_Platform_BuildType

**Recommended Install Directory**

> **The CMake build process modifies the CMake OUTPUT_DIRECTORIES so that the linker input libraries
will be created in directory `./Lib/CompilerName_Platform` and the runtime binaries will be created
in directory `./Bin/CompilerName_Platform`.**

The main CMakeList file sets the following CMAKE variables for this:

    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../Lib/${Compiler}_${Platform})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../Lib/${Compiler}_${Platform})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../Bin/${Compiler}_${Platform})

This should simplify the development process and it is not necessary to install the binaries
for this as all necessary dlls (also the Qt Dlls) will be copied to those directories. Meaning
that you can simple start the applications from this bin directories.

If you want to use the ZSQtLib libraries in your own applications you may desire to install
the binaries and header files to a different location.

As default the commands below would install the ZSQtLib libraries, products and test applications into:

- Windows Machines:
  > "C:\Program Files (x86)\ZSQtLib"

- Linux Machines:
  > "\usr\bin" (or something else depending on linux distribution and version)

This install directories may be providing the CMAKE_INSTALL_PREFIX to the CMake call when
generating the build tree.

You may choose the same install directory for different compilers and platforms as additional
information about the used compiler, used platform (32 or 64 bit) and the used Qt framework is
included in the name of the created libraries. Also a `d` is appended to all created binaries
in Debug build. The same install folder may be used therefore for different build pipelines.

**Generator: Visual Studio 16 (2019)

To generate, build and install a release build with x64 platform use the following commands:

- Release Build

      cmake -G "Visual Studio 16 2019" -A x64 .\Make -B .\Build\msvc2019_x64_Release -DCMAKE_CONFIGURATION_TYPES="Release" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=.\Bin
      cmake --build .\Build\msvc2019_x64_Release --config=Release -j8
      cmake --install .\Build\msvc2019_x61_Release --config=Release

- Debug Build

      cmake -G "Visual Studio 16 2019" -A x64 .\Make -B .\Build\msvc2019_x64_Debug -DCMAKE_CONFIGURATION_TYPES="Debug" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=.\Bin
      cmake --build .\Build\msvc2019_x64_Debug --config=Debug -j8
      cmake --install .\Build\msvc2019_x64_Debug --config=Debug

**Generator: MinGW Makefiles**

To generate, build and install a release build use the following commands (x64 is used as default):

- Release Build

      cmake -G "MinGW Makefiles" .\Make -B .\Build\mingw81_x64_Release -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=.\Bin
      cmake --build .\Build\mingw81_x64_Release -j8
      cmake --install .\Build\mingw81_x64_Release

- Debug Build

      cmake -G "MinGW Makefiles" .\Make -B .\Build\mingw81_x64_Debug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=.\Bin
      cmake --build .\Build\mingw81_x64_Debug -j8
      cmake --install .\Build\mingw81_x64_Debug

## Use ZSQtLib Libraries in other Packages

To use the libraries and to include the header files in other packages you need to add a `find_package` call.

If you want to link to the libraries at compile time you need to add the libraries to the
`target_link_libraries` in `CMakeList.txt`.

If you don't want to link to the libraries at compile time but instead want to use a Dll interface
to load the libraries during runtime you need to add the ZS package to the "target_include_directories"
in `CMakeList.txt`.

### find_package

If you want to use the libraries in other packages you need to add a `find_package` call
to the `CMakeList.txt` file of that package.

Add the following lines to the "CMakeList.txt" file of the package in which you want to integrate
the libraries. Please note that if you want to use Dll interface modules also the path to the
installed source files must become known (ZSQTLIBS_SOURCE_DIRS).

    find_package(ZSQtLibs VERSION_MAJOR.VERSION_MINOR.VERSION_PATCH CONFIG REQUIRED ZSSys ZSIpc ZSIpcTrace)
    if (ZSQtLibs_FOUND)
        message(STATUS "ZSQtLibs found")
    else ()
        message(STATUS "ZSQtLibs not found")
    endif ()

    # Workaround to set binary, include and source directory:
    if(NOT ZSQTLIBS_BINARY_DIR)
        get_target_property(ZSQtLibs_ZSSys_BinPath ZSQtLibs::ZSSys LOCATION)
        message(STATUS "ZSQtLibs_ZSSys_BinPath:   ${ZSQtLibs_ZSSys_BinPath}")
        get_filename_component(ZSQTLIBS_BINARY_DIR ${ZSQtLibs_ZSSys_BinPath} DIRECTORY)
    endif()
    if(NOT ZSQTLIBS_INCLUDE_DIRS)
        set(ZSQTLIBS_INCLUDE_DIRS "${ZSQTLIBS_BINARY_DIR}/include")
    endif()
    if(NOT ZSQTLIBS_SOURCE_DIRS)
        set(ZSQTLIBS_SOURCE_DIRS "${ZSQTLIBS_BINARY_DIR}/source")
    endif()
    message(STATUS "ZSQTLIBS_BINARY_DIR:      ${ZSQTLIBS_BINARY_DIR}")
    message(STATUS "ZSQTLIBS_INCLUDE_DIRS:    ${ZSQTLIBS_INCLUDE_DIRS}")
    message(STATUS "ZSQTLIBS_SOURCE_DIRS:     ${ZSQTLIBS_SOURCE_DIRS}")
    message(STATUS "")

Replace the required version numbers VERSION_MAJOR.VERSION_MINOR.VERSION_PATCH with the version
numbers of the installed package.

> The version number of the **installed** ZSQtLibs may be retrieved from
the installation directory in file `./include/ZSSys/ZSSysVersion.h`.

The `ZSSysVersion.h` file may contain the following code lines (in the following code extract
the comments have been removed from the file):

    #ifndef ZSSys_Version_h
    #define ZSSys_Version_h

    #include "QString"

    namespace ZS
    {
    namespace System
    {
    const int ZSQTLIB_VERSION_MAJOR = 0;
    const int ZSQTLIB_VERSION_MINOR = 01;
    const long int ZSQTLIB_VERSION_PATCH = 2;

    const QString ZSQTLIB_GIT_VERSION_INFO = "v0.01.2-42-g4728566";

    } // namespace System

    } // namespace ZS

    #endif // #ifndef ZSSys_Version_h

On generating the build tree and installing the libraries the version numbers have been automatically
created from the repository's git commit and were written into `include\ZSSys\ZSSysVersion.h`.

Copy the integer numbers for the MAJOR, MINOR and PATCH version to the 'find_package' call
of the `CMakeList.txt` file in which you want to use the installed libraries.
Please note that `find_package` will check for an exact version match also for PATCH_VERSION.

In this `ZSSysVersion.h` file beside the defined MAJOR, MINOR and PATCH version numbers
you will find GIT_VERSION_INFO defined as follows:

    const QString ZSQTLIB_GIT_VERSION_INFO = "v0.01.2-42-g4728566";

- `v0.01.2` corresponds to the most recent tag. VERSION_MAJOR, VERSION_MINOR and VERSION_PATCH is contained in this tag.
- `42` defines the number of commits on top of the most recent tag. This information might be useful to find
  the corresponding git commit in the repository from which the libraries have been generated and build.
- `g` is the object name of the most recent commit and can be ignored.
- `4728566` specifies the abbreviated Git commit hash (usually the first seven digits of the git commit hash).
  This information can be used to find the corresponding git commit in the repository from which the libraries
  has been generated and build.

### target_link_libraries

If you want to link to the libraries at compile time you need to add the libraries to the
`target_link_libraries` in `CMakeList.txt`.

To link the libraries to the target package add a dependency in `target_link_libraries` as follows:

    target_link_libraries(${MY_PROJECT_NAME}
        PUBIC|PRIVATE|..
        OTHER_LIBS
        ...
        ZSSys ZSIpc ZSTrace
    )

### target_include_libraries

If you don't want to link to the libraries at compile time but instead want to use a Dll interface
to load the libraries during runtime you need to add the path to the ZSQtLibs header files
to "target_include_directories" in your `CMakeList.txt`.

    target_include_directories(${MY_PROJECT_NAME}
        PUBIC|PRIVATE|..
        OTHER_INCLUDES
        ...
        ${ZSQTLIBS_INCLUDE_DIRS}
    )

In addition you may need to set additional compiler preprocesser directives to include specific
code from the ZSQtLibs. E.g. to use the method tracing Dll interface you need to set the
preprocessor directive USE_ZS_IPTRACE_DLL_IF.

    target_compile_definitions(${MY_PROJECT_NAME} PUBIC|PRIVATE|.. USE_ZS_IPTRACE_DLL_IF)


***


# Additional Informations

## Repository Content

### ZSSysVersion.h

A `ZSSysVersion.h` is generated from the `.\Include\Libs\ZSSys\ZSSysVersion.h.in` file.
The generated version file is also put into the source directory `.\Include\Libs\ZSSys` but this
file is added to the list of files to be ignored by git.

For Windows, a resource file is generated from `.\Include\Libs\ZSSys\ZSSysVersion.rc.in`.<br/>
This resource file is added to the list of sources. This allows the version number to be made visible
as a file property under Windows.
The generated resource version file is also put into the source directory `.\Include\Libs\ZSSys` but this
file is added to the list of files to be ignored by git.

> The resource file will be compiled by a Windows resource compiler.
Qt provides an own implementation of the resource compiler for MinGW (`Qt\Tools\mingw..\windres.exe`).
This MinGW resource compiler does not work with paths including spaces.
This is the reason that the path to the ZSQtLib repository may not include white spaces.

(The resource compiler provided by Visual Studio or the Windows SDK does not have this limitation.)

### CMake Version Config

A `ZSQtLib-config-version.cmake.in` is provided. This file will be copied into
`ZSQtLib-config-version.cmake` replacing the version number tags with the current version
number as defined in the main CMakeList file. The generated cmake file must be exported so that a
**REQUIRED** version number can be specified in dependent projects.

## Naming of the Generated Binaries

For the libraries (so's on Linux Machines, dll's on Windows Machines) additional informations
is included in the target names. This way it is possible to create a set of shared ZSQtLib
libraries and install them on the target machine into the same target folder.
When including the interface header file into your applications source code during compile
time the used compiler, platform and build type is detected. This way it is possible to
load the correct version of the library no matter by which compiler the application
has been built with.

The following additional informations is included in the names of the libraries:

- Main version number of the used Qt Framework.
- Main and minor version number of ZSQtLib.
- The used compiler name.
- The platform for which the libraries have been created.

For the products and test applications of the ZSQtLib those additional information is not included
in the names of the applications.

But for both the libraries and the product and test applications a `d` is appended for debug builds
so that both release and debug versions of the created applications can be installed into the same
target directory.

**Example for a library**

    ZSSysQt5_msvc2019_x64_d.dll

**Example for an application**

    ZSAppTrcMthClientd.exe

Including the compiler name, the platform, the Qt major version and the build type into the
libraries file names allows installing the ZSQtLib libraries into one location and using
them by applications created by different compilers and platforms and built types.

- If you have an application created with Visual Studio 2008 using Qt Version 4 and you want
  to use the ZSIpcTraceDll Interface the dll `ZSIpcTraceQt5_msvc2008_win32.dll` has to be loaded.
- If you have an application created with MinGW Version 8.1 using Qt Version 5 and you want to
  use the ZSIpcTraceDll Interface the dll `ZSIpcTraceQt5_mingw81_x64.dll` has to be loaded.

> Of course you need to build and install the ZSQtLib using the correct build generator in advance.

## Create and Install ZSQtLib Libraries, Products and Test Applications

### What need to be installed to use the libraries by other packages

The binaries together with the header files containing the definitions of the exported symbols
must be installed at a location accessible by the package using the libraries.

There are several ways to make this path availabe for the CMake build pipeline when building
the dependent packages.

On Windows Machines you can modify (extend) the PATH environment variable to point to the
installation directory of the libraries.

As alternative you can use `CMAKE_INSTALL_PREFIX` together with `CMAKE_PREFIX_PATH` for
installing and referencing the libraries using CMake together with `find_package`.

### CMake

The `ZSQtLib` libraries are installed using CMake.

After cloning the git repository into a path like

    C:\Projekte\ZeusSoft\ZSQtLib

you will find in the projects root directory a `CMakeLists.txt` file which is used as input for CMake
to build and install the library.

**Please ensure that the path to the repository does NOT contain any spaces**.

Additional CMakeLists files are located in sub directories which are added by a
"add_subdirectory" entry in the main CMakeLists file.

#### Building the ZSQtLib

When building the ZSQtLib you should choose the sub directory `.\Build\CompilerId_Platform_BuildType`
to contain the generated build tree.

- `CompilerId` corresponds to the used compiler which may be for example `mingw81` or `msvc2019`.
- `Platform` specifies the used platform (e.g. x64, win32).
- `BuildType` may be either `Release` or `Debug`.

The build directory is defined with argument -B followed by a relative or absolute path
to the directory where the main CMakeLists file is located.

Assuming that the ZSQtLib's project root is

    /Projects/ZeusSoft/ZSQtLib

the build directory should become

    /Projects/ZeusSoft/ZSQtLib/Build/CompilerId_Platform_BuildType

The relative path to the build directory is

    ./Build/CompilerId_Platform_BuildType

The default installation directory for the `ZSQtLib` binaries on Windows Machines is
`C:\Program Files (x86)\ZSQtLib`.<br/>

This is not the standard installation directory for Windows Applications including the company name.<br/>
Moreover installing to "Program Files" for development purposes may conflict with the installation of
already installed packages.

A more suitable bin directory for development packages would be something like

    /Projects/ZeusSoft/ZSQtLib/Bin

The target binaries of all projects for development purposes should be put into this directory so that when
starting an application the dependent dynamic libraries can be found and loaded from one location.

> **The CMake build process modifies the CMake OUTPUT_DIRECTORIES so that the linker input libraries
will be created in directory `./Lib/CompilerName_Platform` and the runtime binaries will be created
in directory `./Bin/CompilerName_Platform`.**

The main CMakeList file sets the following CMAKE variables for this:

    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../Lib/${Compiler}_${Platform})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../Lib/${Compiler}_${Platform})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../Bin/${Compiler}_${Platform})

This should simplify the development process and it is not necessary to install the binaries
for this as all necessary dlls (also the Qt Dlls) will be copied to those directories. Meaning
that you can simple start the applications from this bin directories.

If you want to use the ZSQtLib libraries in your own applications you may desire to install
the binaries and header files to a different location.

**But this might be confusing if you have chosen ./Bin as the installation directory as in this
case you will find the binaries at least two times in the ./Bin directory. In the root and in
the CompilerName_Platform subdirectories.**

##### Generate make files, build and install project

To let CMake generate the build tree, build and install the packages you may invoke the following commands
from command line.

Open a command line and change to the root directory of the project.

> If you intend to install the ZSQtLib binaries below `Program Files` you must open the command prompt
with administrator priviliges.

###### Generator: Visual Studio 16 (2019)

Visual Studio is a multiple-configuration generator. The available configuration types can be tweaked with
the `CMAKE_CONFIGURATION_TYPES` variable. To create a build tree for both Release and Debug build but skipping
Release_RelWithDebInfo and Release_MinSizeRel) you could pass as argument `-DCMAKE_CONFIGURATION_TYPES="Release;Debug"`.

But a separate build tree for both Debug and Release is recommended and the desired build type
should be passed to the CMake call.

Selecting either a Debug or Release build can be done after the build tree has been generated to build and install
the project binaries by using the `--config` flag.

**Applying the `--config` flag is mandatory when building the sources using Visual Studio and the command line.**

- Release Build

      cmake -G "Visual Studio 16 2019" -A x64 .\Make -B .\Build\msvc2019_x64_Release -DCMAKE_CONFIGURATION_TYPES="Release" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=C:\Projekte\ZeusSoft\ZSQtLib\Bin
      cmake --build .\Build\msvc2019_x64_Release --config=Release -j8
      cmake --install .\Build\msvc2019_x64_Release --config=Release

- Debug Build

      cmake -G "Visual Studio 16 2019" -A x64 .\Make -B .\Build\msvc2019_x64_Debug -DCMAKE_CONFIGURATION_TYPES="Debug" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=C:\Projekte\ZeusSoft\ZSQtLib\Bin
      cmake --build .\Build\msvc2019_x64_Debug --config=Debug -j8
      cmake --install .\Build\msvc2019_x64_Debug --config=Debug

####### Visual Studio IDE

If you want to proceed with Visual Studio to build the project instead of the command line you could open the
solution `ZSQtLib.sln` located in `.\Build\msvc2019_x64_Debug|Release`.

To be able to run and/or debug the executables (e.g. the tests) you need to

- set the project of the executable as the startup up project (e.g. ZSAppTrcMthClient),
- open the properties dialog of the project,
- select the Debugging Tab of the dialog,
- in the "Command" edit line enter the path to the installed executable (e.g. "C:\Projekte\ZeusSoft\ZSQtLib\Bin\$(TargetName).exe") and
- enter the directory of the installed executable as the working directory (e.g. "C:\Projekte\ZeusSoft\ZSQtLib\Bin").

###### Generator: MinGW Makefiles

MinGW is a single configuration generator. To build the project either in Debug or Release build
requires running CMake twice, which is a full reconfiguration of the project.

The following example calls are creating the build tree, build the binaries and install the binaries.

- Release Build

      cmake -G "MinGW Makefiles" .\Make -B .\Build\mingw81_x64_Release -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=C:\Projekte\ZeusSoft\ZSQtLib\Bin
      cmake --build .\Build\mingw81_x64_Release -j8
      cmake --install .\Build\mingw81_x64_Release

- Debug Build

      cmake -G "MinGW Makefiles" .\Make -B .\Build\mingw81_X64_Debug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=C:\Projekte\ZeusSoft\ZSQtLib\Bin
      cmake --build .\Build\mingw81_x64_Debug -j8
      cmake --install .\Build\mingw81_x64_Debug

#### Using (Referencing) the ZSQtLib Libraries in other Packages

##### C++ Internals

In order to use the defined methods, constants, type definitions, classes etc. of a library in dependent
packages the main header file of the library got to be included in all files which want to use the exported
symbols of the library.

    #include "...DllMain.h"

Each of the header files (like `ZSIpcServer.h`) are already including the corresponding `DllMain.h` as the
ZSQtLib header files need the definition of the API macro to export symbols. So it is sufficient by the
package module using the ZSQtLib library class `ZS::Ipc::CIpcServer` just to include `ZSIpcServer.h`.

    #include "ZSIpcServer.h"

##### CMake Usage

If the libraries were successfully installed, to include the libraries in dependent projects, you can use
the following code lines in the CMakeList file:

    find_package(ZSQtLib VERSION_MAJOR.VERSION_MINOR REQUIRED)
    if (ZSQtLib_FOUND)
        message(STATUS "ZSQtLib found")
    else ()
        message(STATUS "ZSQtLib not found")
    endif ()

Replace the required version numbers `VERSION_MAJOR.VERSION_MINOR` with the desired version number.

> The version number of the installed ZSQtLib may be retrieved from
the libraries installation directory in file `./Include/ZSQtLib/ZSSysVersion.h`.

In this `ZSSysVersion.h` file beside the defined library MAJOR, MINOR and PATCH version numbers
you will find a define as following:

    #define GIT_COMMIT v1.03-39-gde0fc36

- `v1.03` corresponds to the most recent tag.
- `39` defines the number of commits on top of the most recent tag.
- `g` is the object name of the most recent commit and can be ignored.
- `de0fc36` specifies the Git commit SHA.

To link a ZSQtLib library to the target package add a dependency in `target_link_libraries` as follows:

    target_link_libraries(${PROJECT_NAME} OTHERLIBS ZSSys)

So that it is not necessary to adjust the PATH environment variable under Windows, the installation path of
the library can be specified with

    -DCMAKE_PREFIX_PATH=PATH_TO_ZSQTLIB_INSTALL_DIR

when calling CMake to build your package using the ZSQtLib libraries

To generate the build tree for the target package using the ZSQtLib libraries open a command window,
change to the root directory of the target package you want to build using the ZSQtLib libraries and invoke CMake
similar as described below.

The commands shown will install the target package to `C:\Projekte\MyCompanyName\MyProductName\Bin`.

- Generator: Visual Studio 16 (2019)

  - Release Build

        cmake -G "Visual Studio 16 2019" -A x64 .\Make -B .\Build\msvc2019_x64_Release -DCMAKE_CONFIGURATION_TYPES="Release" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=C:\Projekte\ZeusSoft\ZSQtLib\Bin -DCMAKE_INSTALL_PREFIX=C:\Projekte\MyCompanyName\MyProductName\Bin
        cmake --build .\Build\msvc2019_x64_Release --config=Release -j8
        cmake --install .\Build\msvc2019_x64_Release --config=Release

  - Debug Build

        cmake -G "Visual Studio 16 2019" -A x64 .\Make -B .\Build\msvc2019_x64_Debug -DCMAKE_CONFIGURATION_TYPES="Debug" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=C:\Projekte\ZeusSoft\ZSQtLib\Bin -DCMAKE_INSTALL_PREFIX=C:\Projekte\MyCompanyName\MyProductName\Bin
        cmake --build .\Build\msvc2019_x64_Debug --config=Debug -j8
        cmake --install .\Build\msvc2019_x64_Debug --config=Debug

- Generator: MinGW Makefiles

  - Release Build

        cmake -G "MinGW Makefiles" .\Make -B .\Build\mingw81_x64_Release -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=C:\Projekte\ZeusSoft\ZSQtLib\Bin -DCMAKE_INSTALL_PREFIX=C:\Projekte\MyCompanyName\MyProductName\Bin
        cmake --build .\Build\mingw81\Release -j8
        cmake --install .\Build\mingw81\Release

  - Debug Build

        cmake -G "MinGW Makefiles" .\Make -B .\Build\mingw81_x64_Debug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=C:\Projekte\ZeusSoft\ZSQtLib\Bin -DCMAKE_INSTALL_PREFIX=C:\Projekte\MyCompanyName\MyProductName\Bin
        cmake --build .\Build\mingw81\Debug -j8
        cmake --install .\Build\mingw81\Debug



# Appendix

## Using Qt

For build instruction of the Qt Framework see `./Make/Readme_BuildQt.txt`.

You need to specify the path to the Qt install directory. Depending on the Qt version the install directory is different.
For Qt4 you need to specify the directory where "qmake" is located.
For Qt5 you need to specify the directory where "QtConfig.cmake" is located.

You may have installed a precompiled Qt version or you may have compiled Qt on your own either from the Git repository
or from the downloaded zipped source files.
Make sure you use the same generator to compile ZSQtLib as it has been used to compile Qt.
Change into the build directory to invoke cmake for building ZSQtLib.
For your convience some examples are shown below.
- for VS-2008, Qt-Version 4.8.4 (Qt binaries and qmake on drive D in D:/Qt/Bin/4.8.4/msvc2008/Win32/bin):
  cmake -G "Visual Studio 9 2008" -A Win32 -DCMAKE_BUILD_TYPE=Debug -DQT_VERSION_MAJOR=4 -DCMAKE_PREFIX_PATH=D:/Qt/Bin/4.8.4/msvc2008/Win32/bin ../../../../Make
- for VS-2008, Qt-Version 4.8.8 (Qt binaries and qmake on drive C in C:/Qt/Build/4.8.4/msvc2008/Win32/bin):
  cmake -G "Visual Studio 9 2008" -A Win32 -DCMAKE_BUILD_TYPE=Debug -DQT_VERSION_MAJOR=4 -DCMAKE_PREFIX_PATH="C:/Qt/Build/4.8.8/msvc2008/Win32/bin" ../../../../Make
- for VS-2013, Qt-Version 5.6.3 (QtConfig.cmake located in D:/Qt/5.6/5.6.3/msvc2013_x64_Swp/lib/cmake/Qt5):
  cmake -G "Visual Studio 12 2013" -A x64 -DCMAKE_BUILD_TYPE=Debug -DQT_VERSION_MAJOR=5 -DQt5_DIR="D:/Qt/5.6/5.6.3/msvc2013_64_Swp/lib/cmake" ../../../../Make
- for VS-2019, Qt-Version 5.15.2 (QtConfig.cmake located in /Qt/Bin/5.15.2/msvc2019/x64/lib/cmake/Qt5):
  cmake -G "Visual Studio 16 2019" -A x64 -DCMAKE_BUILD_TYPE=Debug -DQT_VERSION_MAJOR=5 -DQt5_DIR="/Qt/Bin/5.15.2/msvc2019/x64/lib/cmake/Qt5" ../../../../Make

## Linux Setup

see ./Make/Readme_SetupLinux.txt

## Install Visual Studio

see ./Make/Readme_InstallVS.txt
