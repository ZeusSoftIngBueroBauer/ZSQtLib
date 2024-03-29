#*******************************************************************************
# ZSQtLibs Main CMake File
#*******************************************************************************
#
# Quick Startup Usage
# ===================
#
# For more detailled build instructions see "README.md".
#
# On Windows systems in order for CMake to find the Qt Installation the following
# environment variables got to be set:
#
# - PATH
#   - Add install directory of qmake.exe (e.g. C:\Qt\5.15.2\msvc2019_64\bin).
#     To show the current PATH setting in powershell use $Env:Path
#
# Change working directory to where this CMakeList file is located ("<root of the repository>/Make").
#
# Generator: Visual Studio 16 (2019)
#
# - Release Build
#
#   cmake -G "Visual Studio 16 2019" -A x64 . -B ..\Build\msvc2019_x64_Release -DCMAKE_CONFIGURATION_TYPES="Release" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="..\Bin\msvc2019_x64"
#   cmake --build ..\Build\msvc2019_x64_Release --config=Release -j8
#   cmake --install ..\Build\msvc2019_x64_Release --config=Release
#
# - Debug Build
#
#   cmake -G "Visual Studio 16 2019" -A x64 . -B ..\Build\msvc2019_x64_Debug -DCMAKE_CONFIGURATION_TYPES="Debug" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX="..\Bin\msvc2019_x64"
#   cmake --build ..\Build\msvc2019_x64_Debug --config=Debug -j8
#   cmake --install ..\Build\msvc2019_x64_Debug --config=Debug
#
# Generator: MinGW Makefiles
#
# - Release Build
#
#   cmake -G "MinGW Makefiles" . -B ..\Build\mingw81_x64_Release -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="..\Bin\mingw81_x64"
#   cmake --build ..\Build\mingw81_x64_Release -j8
#   cmake --install ..\Build\mingw81_x64_Release
#
# - Debug Build
#
#   cmake -G "MinGW Makefiles" . -B ..\Build\mingw81_x64_Debug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX="..\Bin\mingw81_x64"
#   cmake --build ..\Build\mingw81_x64_Debug -j8
#   cmake --install ..Build\mingw81_x64_Debug
#
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
# Not every CMakeLists.txt file defines a project.
# This is the directory that contains the most recent CMakeLists.txt file that defined a project.
#
# projectName_SOURCE_DIR
# ----------------------
# Source directory for the named project.
# This is the directory that contains the CMakeLists.txt file that contains the project(projectName) definition.
# Every CMakeLists.txt file need not define a project, but one reason to define a project is to create this
# variable so you can refer to its source files later, in other CMakeLists.txt files.
#
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
# Not every CMakeLists.txt file defines a project�this is the directory in the build tree that corresponds
# to the most recent CMakeLists.txt file that defined a project.
#
# projectName_BINARY_DIR
# ----------------------
# Binary directory for the named project.  This is the directory in the build tree that corresponds
# to the CMakeLists.txt file that contains a project(projectName) definition.
# Every CMakeLists.txt file need not define a project, but one reason to define a project is to create
# this variable so you can refer to its binary files later, in other CMakeLists.txt files.

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSQtLibs LANGUAGES CXX)

message(STATUS "")
message(STATUS "---------------------------------------------------------------------------------------------")
message(STATUS "-> ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "")

message(STATUS "CMAKE_PREFIX_PATH:        ${CMAKE_PREFIX_PATH}")
message(STATUS "CMAKE_INSTALL_PREFIX:     ${CMAKE_INSTALL_PREFIX}")
message(STATUS "CMAKE_PROJECT_NAME:       ${CMAKE_PROJECT_NAME}")
message(STATUS "CMAKE_SOURCE_DIR:         ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "PROJECT_SOURCE_DIR:       ${PROJECT_SOURCE_DIR}")

set(TARGET_DEVICE "Desktop" CACHE STRING "Target Device like Kit in Qt Creator")
option(TRACE_MUTEXES "TRACE_MUTEXES" ON)
option(TRACE_GUI_MODELS "TRACE_GUI_MODELS" ON)

message(STATUS "")
message(STATUS "TARGET_DEVICE:            ${TARGET_DEVICE}")
message(STATUS "TRACE_MUTEXES:            ${TRACE_MUTEXES}")
message(STATUS "TRACE_GUI_MODELS:         ${TRACE_GUI_MODELS}")
message(STATUS "")

# Retrieve version information from current git commit.
# The version numbers will be used
# - to configure "ZSQtLibs-config-version.cmake" below and
# - to configure "ZSSysVersion.h" and "ZSSysVersion.rc".
# !! The version header and resource files are configured by the CMakeList file of subdirectory ZSSys. !!
include(${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}GetGitVersionInfo.cmake)

# Perform a Debug build by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

message(STATUS "")
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
elseif(CMAKE_GENERATOR MATCHES "Unix Makefiles")
    string(TOLOWER ${CMAKE_CXX_COMPILER_ID} Compiler)
    string(REPLACE "." ";" CompilerVersionList ${CMAKE_CXX_COMPILER_VERSION})
    list(LENGTH CompilerVersionList listLen)
    if(listLen LESS 2)
        message(FATAL_ERROR "Compiler Version Number ${CompilerVersion} with invalid format")
    endif()
    list(GET CompilerVersionList 0 CompilerVersionMajor)
    list(GET CompilerVersionList 1 CompilerVersionMinor)
    set(Compiler ${Compiler}${CompilerVersionMajor}${CompilerVersionMinor})
elseif(CMAKE_GENERATOR MATCHES "MinGW Makefiles")
    set(Compiler "mingw81")
else()
    message(FATAL_ERROR "Unsupported Generator ${CMAKE_GENERATOR}")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    #add_compile_options(/W3 /WX)
    add_compile_options(/W3)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_CXX_COMPILER_VERSION GREATER_EQUAL 9)
    add_compile_options(-Wno-psabi)
    add_link_options(-zmuldefs)
else()
    add_compile_options(-W -Wall)
    #add_compile_options(-Werror) not yet as long as Qt4 is supported
    add_compile_options(-Wno-deprecated)
    add_compile_options(-Wextra -Wpedantic)
endif()

if (TRACE_MUTEXES)
    add_compile_options(-DZS_TRACE_MUTEXES)
endif()
if (TRACE_GUI_MODELS)
    add_compile_options(-DZS_TRACE_GUI_MODELS)
endif()

message(STATUS "")
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
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
    if(CMAKE_CXX_COMPILER_ARCHITECTURE_ID MATCHES Win32)
        set(Platform "Win32")
    else()
        set(Platform "x64")
    endif()
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
elseif(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    if(CMAKE_CXX_COMPILER_ARCHITECTURE_ID MATCHES Win32)
        set(Platform "Win32")
    else()
        set(Platform "x64")
    endif()
else()
    message(ERROR "Unsupported Compiler")
endif()

message(STATUS "")
message(STATUS "")
message(STATUS "Platform: ${Platform}")
message(STATUS "Compiler: ${Compiler}")

if(WIN32)
    if(NOT TARGET_DEVICE)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../Lib/${Compiler}_${Platform})
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../Lib/${Compiler}_${Platform})
    else()
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../Lib/${TARGET_DEVICE}/${Compiler}_${Platform})
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../Lib/${TARGET_DEVICE}/${Compiler}_${Platform})
    endif()
else()
    if(NOT TARGET_DEVICE)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../Bin/${Compiler}_${Platform})
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../Bin/${Compiler}_${Platform})
    else()
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../Bin/${TARGET_DEVICE}/${Compiler}_${Platform})
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../Bin/${TARGET_DEVICE}/${Compiler}_${Platform})
    endif()
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

#message(STATUS "")
message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY:         ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}")
message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY:         ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG}")
message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY:         ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}")
message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:   ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG}")
message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE: ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE}")
message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG:   ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG}")
message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE}")

set(QT_MINIMUM_VERSION 5.15.4)
find_program(QMAKE_EXECUTABLE NAMES qmake HINTS ${QTDIR} ENV QTDIR PATH_SUFFIXES bin)
execute_process(COMMAND ${QMAKE_EXECUTABLE} -query QT_VERSION OUTPUT_VARIABLE QT_VERSION)
message(STATUS "")
message(STATUS "QT_VERSION:                         ${QT_VERSION}")
message(STATUS "")
if(QT_VERSION LESS QT_MINIMUM_VERSION)
    MESSAGE(FATAL_ERROR "Minimum supported Qt version: ${QT_MINIMUM_VERSION}.
    Installed version: ${QT_VERSION}")
endif()
string(REPLACE "." ";" QT_VERSION_LIST ${QT_VERSION})

list(LENGTH QT_VERSION_LIST versionInfoListLen)
# Abort CMake if version info does not follow the expected format with 3 sections separated by '-'.
if (NOT versionInfoListLen EQUAL 3)
    message(FATAL_ERROR "Detected QT_VERSION ${QT_VERSION} has wrong format")
endif()
list(GET QT_VERSION_LIST 0 QT_VERSION_MAJOR)
list(GET QT_VERSION_LIST 1 QT_VERSION_MINOR)
list(GET QT_VERSION_LIST 2 QT_VERSION_PATCH)

message(STATUS "")
message(STATUS "---------------------------------------------------------------------------------------------")
message(STATUS "Find Package Qt ${QT_VERSION_MAJOR}")
message(STATUS "---------------------------------------------------------------------------------------------")
message(STATUS "")
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Positioning Xml Network Gui Widgets Qml Quick QuickWidgets QuickControls2 REQUIRED)
message(STATUS "Qt${QT_VERSION_MAJOR}_DIR:          ${Qt${QT_VERSION_MAJOR}_DIR}")
message(STATUS "Qt${QT_VERSION_MAJOR}Core_DIR:      ${Qt${QT_VERSION_MAJOR}Core_DIR}")
get_target_property(Qt${QT_VERSION_MAJOR}Core_Location Qt${QT_VERSION_MAJOR}::Core LOCATION)
if(NOT QT_BINARY_DIR)
    get_filename_component(QT_BINARY_DIR ${Qt${QT_VERSION_MAJOR}Core_Location} DIRECTORY)
endif()
message(STATUS "")
message(STATUS "QT_BINARY_DIR:                      ${QT_BINARY_DIR}")
message(STATUS "QT_DEFAULT_MAJOR_VERSION:           ${QT_DEFAULT_MAJOR_VERSION}")
message(STATUS "QT_DEFAULT_MINOR_VERSION:           ${QT_DEFAULT_MINOR_VERSION}")
message(STATUS "QT_DEFAULT_PATCH_VERSION:           ${QT_DEFAULT_PATCH_VERSION}")
message(STATUS "QT_VERSION_MAJOR:                   ${QT_VERSION_MAJOR}")
message(STATUS "QT_VERSION_MINOR:                   ${QT_VERSION_MINOR}")
message(STATUS "QT_VERSION_PATCH:                   ${QT_VERSION_PATCH}")
message(STATUS "QT_DEFAULT_MAJOR_VERSION:           ${QT_DEFAULT_MAJOR_VERSION}")
message(STATUS "QT_LIBINFIX:                        ${QT_LIBINFIX}")
message(STATUS "QT_NO_CREATE_VERSIONLESS_FUNCTIONS: ${QT_NO_CREATE_VERSIONLESS_FUNCTIONS}")
message(STATUS "QT_NO_CREATE_VERSIONLESS_TARGETS:   ${QT_NO_CREATE_VERSIONLESS_TARGETS}")
message(STATUS "QT_VISIBILITY_AVAILABLE:            ${QT_VISIBILITY_AVAILABLE}")

if(CREATE_PYTHON_BINDINGS)
    message(STATUS "")
    message(STATUS "---------------------------------------------------------------------------------------------")
    message(STATUS "Find Package Python Interpreter")
    message(STATUS "---------------------------------------------------------------------------------------------")

    find_package(Python3)
    message(STATUS "PYTHON_EXECUTABLE:     ${PYTHON_EXECUTABLE}")
    message(STATUS "PYTHON_VERSION_STRING: ${PYTHON_VERSION_STRING}")
    message(STATUS "PYTHON_VERSION_MAJOR:  ${PYTHON_VERSION_MAJOR}")
    message(STATUS "PYTHON_VERSION_MINOR:  ${PYTHON_VERSION_MINOR}")
    message(STATUS "PYTHON_VERSION_PATCH:  ${PYTHON_VERSION_PATCH}")

    find_package(PythonLibs ${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR} EXACT REQUIRED)
    message(STATUS "PYTHON_INCLUDE_DIRS:   ${PYTHON_INCLUDE_DIRS}")
    message(STATUS "PYTHON_LIBRARIES:      ${PYTHON_LIBRARIES}")
endif()

message(STATUS "")
message(STATUS "---------------------------------------------------------------------------------------------")
message(STATUS "Processing sub directories")
message(STATUS "---------------------------------------------------------------------------------------------")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

enable_testing()
include(CTest)
include(Dart)

add_subdirectory(Libs/ZSSys)
add_subdirectory(Libs/ZSSysGUI)
add_subdirectory(Libs/ZSQuickControls)
add_subdirectory(Libs/ZSTest)
add_subdirectory(Libs/ZSTestGUI)
add_subdirectory(Libs/ZSIpc)
add_subdirectory(Libs/ZSIpcGUI)
add_subdirectory(Libs/ZSIpcLog)
add_subdirectory(Libs/ZSIpcLogGUI)
add_subdirectory(Libs/ZSIpcTrace)
add_subdirectory(Libs/ZSIpcTraceGUI)
add_subdirectory(Libs/ZSPhysVal)
add_subdirectory(Libs/ZSPhysValGUI)
add_subdirectory(Libs/ZSDiagram)
add_subdirectory(Libs/ZSDiagramGUI)
if(CREATE_PYTHON_BINDINGS)
    add_subdirectory(Libs/ZSIpcTracePy)
endif()

add_subdirectory(Apps/Products/ZSLogClient)
add_subdirectory(Apps/Products/ZSTrcMthClient)

add_subdirectory(Apps/Test/ZSDiagram)
#add_subdirectory(Apps/Test/ZSIpc) got to be revised
add_subdirectory(Apps/Test/ZSIpcLog)
add_subdirectory(Apps/Test/ZSIpcLogDllIfQtApp)
add_subdirectory(Apps/Test/ZSIpcTrace)
add_subdirectory(Apps/Test/ZSIpcTraceDllIfQtApp)
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_subdirectory(Apps/Test/ZSIpcTraceDllIfWinApp)
endif()
add_subdirectory(Apps/Test/ZSPhysVal)
add_subdirectory(Apps/Test/ZSQuickControls)
add_subdirectory(Apps/Test/ZSSysMath)
add_subdirectory(Apps/Test/ZSSysEnums)
add_subdirectory(Apps/Test/ZSSysErrLog)
add_subdirectory(Apps/Test/ZSSysIdxTree)
add_subdirectory(Apps/Test/ZSSysLogFile)
add_subdirectory(Apps/Test/ZSTestTemplate)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Generate the version file for the config file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${ZSQTLIB_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# Create config file
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

set(ConfigPackageLocation lib/cmake/${PROJECT_NAME})

install(EXPORT ${PROJECT_NAME}Targets
    NAMESPACE ZSQtLibs::
    DESTINATION ${ConfigPackageLocation}
)

# Install config files
install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

message(STATUS "")
message(STATUS "<- ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "---------------------------------------------------------------------------------------------")
message(STATUS "")
