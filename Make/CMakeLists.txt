# Build instructions
# ==================
#
# Recommended Build Directories:
# ------------------------------
# Please verify that your compiler supports the desired architecture (platform).
# VS-2008 express edition for example does not support the X64 architecture.
# - for VS-2008: ./ZSQtLib/CMake-Build/msvc2008/<Platform>/<Debug|Release>
# - for VS-2010: ./ZSQtLib/CMake-Build/msvc2010/<Platform>/<Debug|Release>
# - for VS-2012: ./ZSQtLib/CMake-Build/msvc2012/<Platform>/<Debug|Release>
# - for VS-2013: ./ZSQtLib/CMake-Build/msvc2013/<Platform>/<Debug|Release>
# - for VS-2015: ./ZSQtLib/CMake-Build/msvc2015/<Platform>/<Debug|Release>
# - for VS-2017: ./ZSQtLib/CMake-Build/msvc2017/<Platform>/<Debug|Release>
# - for VS-2019: ./ZSQtLib/CMake-Build/msvc2019/<Platform>/<Debug|Release>
#
# Call CMake from build directory:
# --------------------------------
# You need to specify the path to the Qt install directory. Depending on the Qt version the install directory is different.
# For Qt4 you need to specify the directory where "qmake" is located.
# For Qt5 you need to specify the directory where "QtConfig.cmake" is located.
# You may have installed a precompiled Qt version or you may have compiled Qt on your own either from the Git repository
# or from the downloaded zipped source files.
# Make sure you use the same generator to compile ZSQtLib as it has been used to compile Qt.
# Change into the build directory to invoke cmake for building ZSQtLib.
# For your convience some examples are shown below.
# - for VS-2008, Qt-Version 4.8.4 (Qt installed on drive D in Bin-Directory):
#   cmake -G "Visual Studio 9 2008" -A Win32 -DCMAKE_BUILD_TYPE=Debug -DQT_VERSION_MAJOR=4 -DCMAKE_PREFIX_PATH=D:/Qt/Bin/4.8.4/msvc2008/Win32/bin ../../../../Make
# - for VS-2008, Qt-Version 4.8.8 (Qt installed on drive C in Build-Directory):
#   cmake -G "Visual Studio 9 2008" -A Win32 -DCMAKE_BUILD_TYPE=Debug -DQT_VERSION_MAJOR=4 -DCMAKE_PREFIX_PATH="C:/Qt/Build/4.8.8/msvc2008/Win32/bin" ../../../../Make
# - for VS-2019, Qt-Version 5.15.2 (Qt installed on same drive as invoking CMake):
#   cmake -G "Visual Studio 16 2019" -A x64 -DCMAKE_BUILD_TYPE=Debug -DQT_VERSION_MAJOR=5 -DQt5_DIR="/Qt/Bin/5.15.2/msvc2019/x64/lib/cmake/Qt5" ../../../../Make
#
# Build packages:
# ---------------
# cmake --build . --config DEBUG -v

# SOURCE_DIR Variables
# ====================
#
# These variables contain the paths to various source code directories.
# Note how every one of them refers to a directory that contains a CMakeLists.txt file.
#
# CMAKE_SOURCE_DIR
# ----------------
# The path to the top level of the source tree.
# This is the directory that contains the top-level CMakeLists.txt file.
# That is, this is the source directory you specify to the cmake command.
#
# CMAKE_CURRENT_SOURCE_DIR
# ------------------------
# The path to the directory containing the CMakeLists.txt file that is currently being processed.
#
# PROJECT_SOURCE_DIR
# ------------------
# Top level source directory for the current project.
# Not every CMakeLists.txt file defines a project–this is the directory that contains
# the most recent CMakeLists.txt file that defined a project.
#
# projectName_SOURCE_DIR
# ----------------------
# Source directory for the named project.
# This is the directory that contains the CMakeLists.txt file that contains the project(projectName) definition.
# Every CMakeLists.txt file need not define a project, but one reason to define a project is to create this
# variable so you can refer to its source files later, in other CMakeLists.txt files.

# BINARY_DIR Variables
# ====================
#
# The build tree will contain a directory hierarchy corresponding to the hierarchy of directories
# in the source tree containing CMakeLists.txt files.
# Each of the following variables refers to a directory in the build tree corresponding to a source
# tree directory that contains a CMakeLists.txt file.
#
# CMAKE_BINARY_DIR
# ----------------
# The path to the top level of the build tree.
# This is the directory in which you ran the cmake command.
#
# CMAKE_CURRENT_BINARY_DIR
# ------------------------
# The path to the binary directory currently being processed.
# When an add_subdirectory command is encountered in a CMakeLists.txt file, a corresponding directory
# is created in the build directory. This variable contains that subdirectory.
#
# PROJECT_BINARY_DIR
# ------------------
# Top level binary directory for the current project.
# Not every CMakeLists.txt file defines a project–this is the directory in the build tree that corresponds
# to the most recent CMakeLists.txt file that defined a project.
#
# projectName_BINARY_DIR
# ----------------------
# Binary directory for the named project.  This is the directory in the build tree that corresponds
# to the CMakeLists.txt file that contains a project(projectName) definition.
# Every CMakeLists.txt file need not define a project, but one reason to define a project is to create
# this variable so you can refer to its binary files later, in other CMakeLists.txt files.

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSQtLibs)

message(STATUS "")
message(STATUS "---------------------------------------------------------------------------------------------")
message(STATUS "Processing: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "---------------------------------------------------------------------------------------------")

message(STATUS "CMAKE_PROJECT_NAME:       ${CMAKE_PROJECT_NAME}")
message(STATUS "CMAKE_SOURCE_DIR:         ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "PROJECT_SOURCE_DIR:       ${PROJECT_SOURCE_DIR}")

# Perform a Debug build by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "CMAKE_BINARY_DIR:         ${CMAKE_BINARY_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "CMAKE_PREFIX_PATH:        ${CMAKE_PREFIX_PATH}")
message(STATUS "PROJECT_BINARY_DIR:       ${PROJECT_BINARY_DIR}")
message(STATUS "")
message(STATUS "Generator:                ${CMAKE_GENERATOR}")
message(STATUS "Build Type:               ${CMAKE_BUILD_TYPE}")
message(STATUS "Platform-Id:              ${PLATFORM_ID}")
message(STATUS "")
message(STATUS "Compiler:                 ${CMAKE_CXX_COMPILER}")
message(STATUS "Compiler-Id:              ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Compiler-Version:         ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Compiler-Architecture:    ${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}")
message(STATUS "CXX-Standard:             ${CMAKE_CXX_STANDARD}")

add_compile_options(-DDONT_USE_PRAGMA_COMMENT_LIB_INCLUDE_IN_MAIN_MODULES)

if(CMAKE_GENERATOR MATCHES "Visual Studio 9 2008")
    set(Compiler "msvc2008")
    add_compile_options(/Gm /Zi)
elseif(CMAKE_GENERATOR MATCHES "Visual Studio 10 2010")
    set(Compiler "msvc2010")
elseif(CMAKE_GENERATOR MATCHES "Visual Studio 11 2012")
    set(Compiler "msvc2012")
elseif(CMAKE_GENERATOR MATCHES "Visual Studio 12 2013")
    set(Compiler "msvc2013")
elseif(CMAKE_GENERATOR MATCHES "Visual Studio 14 2015")
    set(Compiler "msvc2015")
elseif(CMAKE_GENERATOR MATCHES "Visual Studio 14 2015")
    set(Compiler "msvc2015")
elseif(CMAKE_GENERATOR MATCHES "Visual Studio 15 2017")
    set(Compiler "msvc2017")
elseif(CMAKE_GENERATOR MATCHES "Visual Studio 16 2019")
    set(Compiler "msvc2019")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    #add_compile_options(/W3 /WX)
    add_compile_options(/W3)
else()
    add_compile_options(-W -Wall -Werror)
    add_compile_options(-Wextra -Wpedantic)
endif()

message(STATUS "")
if(CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "C Flags, Debug:            ${CMAKE_C_FLAGS_DEBUG}")
    message(STATUS "C++ Flags, Debug:          ${CMAKE_CXX_FLAGS_DEBUG}")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    message(STATUS "C Flags, Release:          ${CMAKE_C_FLAGS_RELEASE}")
    message(STATUS "C++ Flags, Release:        ${CMAKE_CXX_FLAGS_RELEASE}")
elseif(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
    message(STATUS "C Flags, RelWithDebInfo:   ${CMAKE_C_FLAGS_RELWITHDEBINFO}")
    message(STATUS "C++ Flags, RelWithDebInfo: ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
elseif(CMAKE_BUILD_TYPE MATCHES MinSizeRel)
    message(STATUS "C Flags, MinRel:           ${CMAKE_C_FLAGS_MINSIZEREL}")
    message(STATUS "C++ Flags, MinRel:         ${CMAKE_CXX_FLAGS_MINSIZEREL}")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
    if(CMAKE_CXX_COMPILER_ARCHITECTURE_ID MATCHES x64)
        set(Platform "x64")
    else()
        set(Platform "Win32")
    endif()
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

message(STATUS "")
message(STATUS "")
message(STATUS "Platform: ${Platform}")
message(STATUS "Compiler: ${Compiler}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../Lib/${Compiler}_${Platform})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../Lib/${Compiler}_${Platform})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../Bin/${Compiler}_${Platform})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

message(STATUS "")
message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY:         ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}")
message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY:         ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG}")
message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY:         ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}")
message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:   ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}")
message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE: ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE}")
message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG:   ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG}")
message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE}")
message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG:   ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}")
message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

message(STATUS "")
message(STATUS "---------------------------------------------------------------------------------------------")
message(STATUS "Find Package Qt ${QT_VERSION_MAJOR}")
message(STATUS "---------------------------------------------------------------------------------------------")

if(NOT QT_VERSION_MAJOR OR QT_VERSION_MAJOR GREATER_EQUAL 5)
    message(STATUS "")
    message(STATUS "Trying to find Qt Version 5")
    message(STATUS "---------------------------------------------------------------------------------------------")
    message(STATUS "Qt5_DIR: ${Qt5_DIR}")
    find_package(Qt5 COMPONENTS Core Xml Network Gui Widgets)
    message("Qt5Core_FOUND: ${Qt5Core_FOUND}")
    if(Qt5Core_FOUND)
        set(QT_FOUND 1)
    endif()
    message("QT_FOUND: ${QT_FOUND}")
endif()
message("QT_FOUND: ${QT_FOUND}")
if(NOT QT_FOUND)
    message(STATUS "")
    message(STATUS "Trying to find Qt Version 4")
    message(STATUS "---------------------------------------------------------------------------------------------")
    find_package(Qt4 REQUIRED QtCore QtXml QtNetwork QtGui)
endif()

if(QT_FOUND)
    if(Qt4_FOUND)
        get_target_property(Qt4Core_Location Qt4::QtCore LOCATION)
        get_filename_component(Qt4Core_LIB_DIRS ${Qt4Core_Location} DIRECTORY)
        if(NOT Qt4_INCLUDE_DIRS)
            set(Qt4_INCLUDE_DIRS "${Qt4Core_LIB_DIRS}/../include")
        endif()
        if(CMAKE_BUILD_TYPE MATCHES Debug)
            if(EXISTS "${Qt4Core_LIB_DIRS}/qt_d.lib")
                set(QT_LIB "${Qt4Core_LIB_DIRS}/qt_d.lib")
            endif()
        else()
            if(EXISTS "${Qt4Core_LIB_DIRS}/qt.lib")
                set(QT_LIB "${Qt4Core_LIB_DIRS}/qt.lib")
            endif()
        endif()
        message("Qt4Core_VERSION_STRING:             ${Qt4Core_VERSION_STRING}")
        message("Qt4Core_Location:                   ${Qt4Core_Location}")
        message("Qt4Core_INCLUDE_DIRS:               ${Qt4Core_INCLUDE_DIRS}")
        message("Qt4Core_LIB_DIRS:                   ${Qt4Core_LIB_DIRS}")
        message("Qt4Core_LIBRARIES:                  ${Qt4Core_LIBRARIES}")
        message("Qt4Core_PRIVATE_INCLUDE_DIRS:       ${Qt4Core_PRIVATE_INCLUDE_DIRS}")
        message("Qt4Core_COMPILE_DEFINITIONS:        ${Qt4Core_COMPILE_DEFINITIONS}")
        message("Qt4Core_DEFINITIONS:                ${Qt4Core_DEFINITIONS}")
        message("Qt4Core_EXECUTABLE_COMPILE_FLAGS:   ${Qt4Core_EXECUTABLE_COMPILE_FLAGS}")
        message("Qt4_INCLUDE_DIRS:                   ${Qt4_INCLUDE_DIRS}")
        if(QT_LIB)
            message("QT_LIB:                             ${QT_LIB}")
        else()
            message("QT_LIB:                             NOT EXISTING")
        endif()
        if(NOT QT_BINARY_DIR)
            get_filename_component(QT_BINARY_DIR ${Qt4Core_Location} DIRECTORY)
        endif()
    elseif(Qt5_FOUND)
        get_target_property(Qt5Core_Location Qt5::Core LOCATION)
        message("Qt5Core_VERSION_STRING:             ${Qt5Core_VERSION_STRING}")
        message("Qt5Core_Location:                   ${Qt5Core_Location}")
        message("Qt5Core_INCLUDE_DIRS:               ${Qt5Core_INCLUDE_DIRS}")
        message("Qt5Core_LIBRARIES:                  ${Qt5Core_LIBRARIES}")
        message("Qt5Core_PRIVATE_INCLUDE_DIRS:       ${Qt5Core_PRIVATE_INCLUDE_DIRS}")
        message("Qt5Core_COMPILE_DEFINITIONS:        ${Qt5Core_COMPILE_DEFINITIONS}")
        message("Qt5Core_DEFINITIONS:                ${Qt5Core_DEFINITIONS}")
        message("Qt5Core_EXECUTABLE_COMPILE_FLAGS:   ${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
        if(NOT QT_BINARY_DIR)
            get_filename_component(QT_BINARY_DIR ${Qt5Core_Location} DIRECTORY)
        endif()
    endif()
    message("QT_VERSION_MAJOR:                   ${QT_VERSION_MAJOR}")
    message("QT_VERSION_MINOR:                   ${QT_VERSION_MINOR}")
    message("QT_VERSION_PATCH:                   ${QT_VERSION_PATCH}")
    message("QT_DEFAULT_MAJOR_VERSION:           ${QT_DEFAULT_MAJOR_VERSION}")
    message("QT_BINARY_DIR:                      ${QT_BINARY_DIR}")
    message("QT_LIBINFIX:                        ${QT_LIBINFIX}")
    message("QT_NO_CREATE_VERSIONLESS_FUNCTIONS: ${QT_NO_CREATE_VERSIONLESS_FUNCTIONS}")
    message("QT_NO_CREATE_VERSIONLESS_TARGETS:   ${QT_NO_CREATE_VERSIONLESS_TARGETS}")
    message("QT_VISIBILITY_AVAILABLE:            ${QT_VISIBILITY_AVAILABLE}")
endif()

message(STATUS "")
message(STATUS "---------------------------------------------------------------------------------------------")
message(STATUS "Find Package Python Interpreter")
message(STATUS "---------------------------------------------------------------------------------------------")

if(CREATE_PYTHON_BINDINGS)
    find_package(Python3)
    message("PYTHON_EXECUTABLE:     ${PYTHON_EXECUTABLE}")
    message("PYTHON_VERSION_STRING: ${PYTHON_VERSION_STRING}")
    message("PYTHON_VERSION_MAJOR:  ${PYTHON_VERSION_MAJOR}")
    message("PYTHON_VERSION_MINOR:  ${PYTHON_VERSION_MINOR}")
    message("PYTHON_VERSION_PATCH:  ${PYTHON_VERSION_PATCH}")

    find_package(PythonLibs ${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR} EXACT REQUIRED)
    message("PYTHON_INCLUDE_DIRS:   ${PYTHON_INCLUDE_DIRS}")
    message("PYTHON_LIBRARIES:      ${PYTHON_LIBRARIES}")
endif()

message(STATUS "")
message(STATUS "---------------------------------------------------------------------------------------------")
message(STATUS "Processing sub directories")
message(STATUS "---------------------------------------------------------------------------------------------")

enable_testing()

add_subdirectory(Libs/ZSSys)
add_subdirectory(Libs/ZSSysGUI)
add_subdirectory(Libs/ZSTest)
add_subdirectory(Libs/ZSTestGUI)
add_subdirectory(Libs/ZSIpc)
add_subdirectory(Libs/ZSIpcGUI)
add_subdirectory(Libs/ZSIpcTrace)
add_subdirectory(Libs/ZSIpcTraceGUI)
if(CREATE_PYTHON_BINDINGS)
    add_subdirectory(Libs/ZSIpcTracePy)
endif()
add_subdirectory(Apps/Products/ZSTrcMthClient)
add_subdirectory(Apps/Test/ZSIpcTrace)
add_subdirectory(Apps/Test/ZSIpcTraceDllIfQtApp)
add_subdirectory(Apps/Test/ZSSysEnums)
add_subdirectory(Apps/Test/ZSSysErrLog)
add_subdirectory(Apps/Test/ZSSysIdxTree)
add_subdirectory(Apps/Test/ZSSysLogFile)
add_subdirectory(Apps/Test/ZSTestTemplate)
