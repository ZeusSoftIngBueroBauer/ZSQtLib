see https://wiki.qt.io/Building_Qt_5_from_Git

The following example uses:

QtVersion      (e.g. 5.15.2)
StudioVersion  [msvc2015, msvc2017, msvc2019]
Platform       [Win32, x64]

1. Repository

   - Clone Qt git repository from "git://code.qt.io/qt/qt5.git" to "C:\Qt\Repos\qt5".
   - Checkout Qt Version Label v<QtVersion>

2. Out of source build

  2.1 Prerequisites

    - Open Visual Studio Command Prompt
      - If StudioVersion = msvc2015
        - If Platform = Win32: "Visual Studio 2015 - x86 Native Tools Command Prompt"
        - If Platform = x64:   "Visual Studio 2015 - x64 Native Tools Command Prompt"
        - !! Ensure that "rc" is in path (call "where rc"). If not copy "RC.exe" and "RcDll.dll":
          from C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Bin     to C:\Program Files (x86)\Microsoft SDKs\Windows\v10.0A\bin\NETFX 4.6.1 Tools
          from C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Bin\x64 to C:\Program Files (x86)\Microsoft SDKs\Windows\v10.0A\bin\NETFX 4.6.1 Tools\x64
      - If StudioVersion = msvc2017
        - If Platform = Win32: "Visual Studio 2017 - x86 Native Tools Command Prompt"
        - If Platform = x64:   "Visual Studio 2017 - x64 Native Tools Command Prompt"
      - If StudioVersion = msvc2019
        - If Platform = Win32: "Visual Studio 2019 - x86 Native Tools Command Prompt"
        - If Platform = x64:   "Visual Studio 2019 - x64 Native Tools Command Prompt"

  2.2 Build directory:

    - mkdir C:\Qt\Build\<QtVersion>\<StudioVersion>\<Platform> (e.g. mkdir C:\Qt\Build\5.15.2\msvc2019\x64)
    - cd C:\Qt\Build\<QtVersion>\<StudioVersion>\<Platform> (e.g. cd C:\Qt\Build\5.15.2\msvc2019\x64)

  2.3 Configure Qt

    - C:\Qt\Repos\qt5\configure -nomake examples -nomake tests -skip qtwebengine -opensource -prefix C:\Qt\Bin\<QtVersion>\<StudioVersion>\<Platform>
      (e.g.: C:\Qt\Repos\qt5\configure -nomake examples -nomake tests -skip qtwebengine -opensource -prefix C:\Qt\Bin\5.15.2\msvc2019\x64)
      If no OpenGL support is possible:
      (e.g.: C:\Qt\Repos\qt5\configure -nomake examples -nomake tests -skip qtwebengine -opensource -no-opengl -prefix C:\Qt\Bin\5.15.2\msvc2015\x64)

    On Errors:

    - MakeSpecs Toolchain broken:
      see !! Ensure that "rc" is in path.

  2.4 nmake

    On Errors:

    - Cannot open include file "EGL/egl.h":
      - If OpenGL is not necessary:
        - Configure Qt with flags "-no-opengl":
          - C:\Qt\Repos\qt5\configure -nomake examples -nomake tests -skip qtwebengine -opensource -no-opengl -prefix C:\Qt\Bin\<QtVersion>\<StudioVersion>\<Platform>      
            (e.g.: C:\Qt\Repos\qt5\configure -nomake examples -nomake tests -skip qtwebengine -opensource -no-opengl -prefix C:\Qt\Bin\5.15.2\msvc2015\x64)
      - If OpenGL is necessary try:
        - Install Latest Windows SDK Version.
        - Add location of "fxc.exe" to PATH (e.g. "set PATH=%PATH%;C:\Program Files (x86)\Windows Kits\10\bin\10.0.19041.0\<Platform>").
        - Configure Qt with flags "-opengl dynamic" and "-angle:
          - C:\Qt\Repos\qt5\configure -nomake examples -nomake tests -skip qtwebengine -opensource -opengl dynamic -angle -prefix C:\Qt\Bin\<QtVersion>\<StudioVersion>\<Platform>      
            (e.g.: C:\Qt\Repos\qt5\configure -nomake examples -nomake tests -skip qtwebengine -opensource -opengl dynamic -angle -prefix C:\Qt\Bin\5.15.2\msvc2015\x64)

3. Install Qt binaries, inclulde files, etc.

   - nmake install
