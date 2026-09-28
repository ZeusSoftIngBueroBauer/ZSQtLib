cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSAppDraw)

message(STATUS "")
message(STATUS "---------------------------------------------------------------------------------------------")
message(STATUS "-> ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "")

message(STATUS "CMAKE_PROJECT_NAME:       ${CMAKE_PROJECT_NAME}")
message(STATUS "PROJECT_NAME:             ${PROJECT_NAME}")

message(STATUS "CMAKE_SOURCE_DIR:         ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "PROJECT_SOURCE_DIR:       ${PROJECT_SOURCE_DIR}")

message(STATUS "CMAKE_BINARY_DIR:         ${CMAKE_BINARY_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "PROJECT_BINARY_DIR:       ${PROJECT_BINARY_DIR}")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# In source code the headers are included together with the ${PROJECT_NAME} as follows:
# include "${PROJECT_NAME}/HeaderFileName.h"
# e.g. #include "ZSSys/ZSSysIdxTree.h"
# For "target_include_directories" the header path without ${PROJECT_NAME} must be
# passed and therefore _HeadersPath will be defined without ${PROJECT_NAME} so that
# this define can be used further one in this CMake list file.
# _SourcePaths will be defined in the same way.
set(_HeadersPath ${CMAKE_CURRENT_SOURCE_DIR}/../../../../Include/Apps/Products)
set(_SourcesPath ${CMAKE_CURRENT_SOURCE_DIR}/../../../../Source/Apps/Products)
set(_ResourcesPath ${CMAKE_CURRENT_SOURCE_DIR}/../../../../Resources/Apps/Products)

list(APPEND _HeadersMain
    ${_HeadersPath}/../../Libs/ZSSys/ZSSysVersion.h
    ${_HeadersPath}/ZSDraw/App.h
    ${_HeadersPath}/ZSDraw/MainWindow.h
    ${_HeadersPath}/ZSDraw/WidgetCentral.h
)

list(APPEND _HeadersTest
    ${_HeadersPath}/ZSDraw/Test/Test.h
)

configure_file(
    ${_HeadersPath}/ZSDraw/Test/TestConfig.h.in
    ${_HeadersPath}/ZSDraw/Test/TestConfig.h)
set(_HeadersTest ${_HeadersTest} ${_HeadersPath}/ZSDraw/Test/TestConfig.h)

list(APPEND _Headers
    ${_HeadersMain}
    ${_HeadersTest}
)

list(APPEND _SourcesMain
    ${_SourcesPath}/ZSDraw/Main.cpp
    ${_SourcesPath}/ZSDraw/App.cpp
    ${_SourcesPath}/ZSDraw/MainWindow.cpp
    ${_SourcesPath}/ZSDraw/WidgetCentral.cpp
)

list(APPEND _SourcesTest
    ${_SourcesPath}/ZSDraw/Test/Test.cpp
    ${_SourcesPath}/ZSDraw/Test/TestCreateGroupAddObjects.cpp
    ${_SourcesPath}/ZSDraw/Test/TestCreateGroupDrawObjects.cpp
    ${_SourcesPath}/ZSDraw/Test/TestCreateGroupImageSizeAndObjectCoordinates.cpp
)

list(APPEND _Sources
    ${_SourcesMain}
    ${_SourcesTest}
)

list(APPEND _Resources
    ${_ResourcesPath}/ZSDraw/ZSDraw.qrc
)

if(WIN32)
    # Add version info as meta information to Dll.
    set(_Resources ${_Resources} ${_ResourcesPath}/../../Libs/ZSSys/ZSSysVersion.rc)
    # Icons.
    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        set(_Headers ${_Headers}
            ${_HeadersPath}/ZSDraw/resource.h
            ${_HeadersPath}/ZSDraw/targetver.h)
        set(_Resources ${_Resources} ${_ResourcesPath}/ZSDraw/${PROJECT_NAME}.rc)
    endif()
endif()

# Got to create the following target name:
# (ProjectName)[d]
# e.g.: "ZSAppDrawd"
set(TargetOutputName ${PROJECT_NAME})
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(TargetOutputName ${TargetOutputName}d)
endif()
message(STATUS "TargetOutputName:         ${TargetOutputName}")

if(WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${_Headers} ${_Sources} ${_Resources} ${_Images})
else()
    add_executable(${PROJECT_NAME} ${_Headers} ${_Sources} ${_Resources} ${_Images})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TargetOutputName})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Apps/Products)

source_group("Header Files/Main" FILES ${_HeadersMain})
source_group("Header Files/Test" FILES ${_HeadersTest})
source_group("Source Files/Main" FILES ${_SourcesMain})
source_group("Source Files/Test" FILES ${_SourcesTest})
source_group("Resource Files" FILES ${_Resources})

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/../Include/Plugins/ZSDraw
    PRIVATE ${CMAKE_SOURCE_DIR}/../Include/Apps/Products/ZSDraw)

set(_Libs ZSIpcTraceGUI ZSPhysValGUI ZSTestGUI ZSDraw)

target_link_libraries(${PROJECT_NAME} ${_Libs})

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME} EXPORT ${CMAKE_PROJECT_NAME}Targets RUNTIME DESTINATION .)

# Prepare installer files

# Get Current Date Time
string(TIMESTAMP RELEASE_DATE_YEAR "%Y" UTC)
string(TIMESTAMP RELEASE_DATE_TIME "%Y-%m-%d %H:%m" UTC)
message(STATUS "RELEASE_DATE:             ${RELEASE_DATE_TIME}")

if(WIN32)
    set(ApplicationsDir "$ENV{ProgramFiles}")
    set(AppDataDir "$ENV{APPDATA}")
    # Need to replace \ with /. Otherwise a very strange StartMenu path is created.
    string(REPLACE "\\" "/" ApplicationsDir ${ApplicationsDir})
    string(REPLACE "\\" "/" AppDataDir ${AppDataDir})
    message(STATUS "ApplicationsDir:          ${ApplicationsDir}")
    message(STATUS "AppDataDir:               ${AppDataDir}")

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Installer/config/config.xml.in
                   ${CMAKE_CURRENT_SOURCE_DIR}/Installer/config/config.xml)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Installer/packages/de.zeussoft.Draw/meta/license.txt.in
                   ${CMAKE_CURRENT_SOURCE_DIR}/Installer/packages/de.zeussoft.Draw/meta/license.txt)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Installer/packages/de.zeussoft.Draw/meta/package.xml.in
                   ${CMAKE_CURRENT_SOURCE_DIR}/Installer/packages/de.zeussoft.Draw/meta/package.xml)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Installer/packages/de.zeussoft.Draw/meta/installscript.qs.in
                   ${CMAKE_CURRENT_SOURCE_DIR}/Installer/packages/de.zeussoft.Draw/meta/installscript.qs)
endif()

message(STATUS "")
message(STATUS "<- ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "---------------------------------------------------------------------------------------------")
message(STATUS "")
