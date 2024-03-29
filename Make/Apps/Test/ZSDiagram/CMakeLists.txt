cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSAppTestDiagram)

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
set(_HeadersPath ${CMAKE_CURRENT_SOURCE_DIR}/../../../../Include/Apps/Test)
set(_SourcesPath ${CMAKE_CURRENT_SOURCE_DIR}/../../../../Source/Apps/Test)
set(_ResourcesPath ${CMAKE_CURRENT_SOURCE_DIR}/../../../../Resources/Apps/Test)

list(APPEND _HeadersCommon
    ${_HeadersPath}/../../Libs/ZSSys/ZSSysVersion.h
    ${_HeadersPath}/ZSDiagram/App.h
    ${_HeadersPath}/ZSDiagram/MainWindow.h
    ${_HeadersPath}/ZSDiagram/WidgetCentral.h
    ${_HeadersPath}/ZSDiagram/WdgtTestOutput.h
    ${_HeadersPath}/ZSDiagram/Test.h
)

configure_file(
    ${_HeadersPath}/ZSDiagram/TestConfig.h.in
    ${_HeadersPath}/ZSDiagram/TestConfig.h)
set(_HeadersCommon ${_HeadersCommon} ${_HeadersPath}/ZSDiagram/TestConfig.h)

list(APPEND _HeadersUnits
    ${_HeadersPath}/ZSDiagram/Units/Frequency.h
    ${_HeadersPath}/ZSDiagram/Units/Power.h
    ${_HeadersPath}/ZSDiagram/Units/Units.h
)

list(APPEND _Headers
    ${_HeadersCommon}
    ${_HeadersUnits}
)

list(APPEND _SourcesCommon
    ${_SourcesPath}/ZSDiagram/Main.cpp
    ${_SourcesPath}/ZSDiagram/App.cpp
    ${_SourcesPath}/ZSDiagram/MainWindow.cpp
    ${_SourcesPath}/ZSDiagram/WidgetCentral.cpp
    ${_SourcesPath}/ZSDiagram/WdgtTestOutput.cpp
    ${_SourcesPath}/ZSDiagram/Test.cpp
)

list(APPEND _SourcesUnits
    ${_SourcesPath}/ZSDiagram/Units/Frequency.cpp
    ${_SourcesPath}/ZSDiagram/Units/Power.cpp
    ${_SourcesPath}/ZSDiagram/Units/Units.cpp
)

list(APPEND _Resources
)

list(APPEND _Sources
    ${_SourcesCommon}
    ${_SourcesUnits}
)

if(WIN32)
    # Add version info as meta information to Dll.
    set(_Resources ${_Resources} ${_ResourcesPath}/../../Libs/ZSSys/ZSSysVersion.rc)
    # Icons.
    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        set(_Headers ${_Headers}
            ${_HeadersPath}/ZSDiagram/resource.h
            ${_HeadersPath}/ZSDiagram/targetver.h)
        set(_Resources ${_Resources} ${_ResourcesPath}/ZSDiagram/${PROJECT_NAME}.rc)
    endif()
endif()

# Got to create the following target name:
# (ProjectName)[_d]
# e.g.: "ZSAppTestDiagram_d"
set(TargetOutputName ${PROJECT_NAME})
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(TargetOutputName ${TargetOutputName}d)
endif()
message(STATUS "TargetOutputName: ${TargetOutputName}")

if(WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${_Headers} ${_Sources} ${_Resources})
else()
    add_executable(${PROJECT_NAME} ${_Headers} ${_Sources} ${_Resources})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TargetOutputName})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Tests)

source_group("Header Files/Units" FILES ${_HeadersUnits})
source_group("Header Files" FILES ${_HeadersCommon})
source_group("Source Files/Units" FILES ${_SourcesUnits})
source_group("Source Files" FILES ${_SourcesCommon})
source_group("Resource Files" FILES ${_Resources})

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/../Include/Apps/Test/ZSDiagram)

set(_Libs ZSTestGUI ZSDiagramGUI)

target_link_libraries(${PROJECT_NAME} ${_Libs})

enable_testing()
include(CTest)
include(Dart)
add_test(ZSDiagram ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TargetOutputName} "AutoStartTest")

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME} EXPORT ${CMAKE_PROJECT_NAME}Targets RUNTIME DESTINATION .)

message(STATUS "")
message(STATUS "<- ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "---------------------------------------------------------------------------------------------")
message(STATUS "")
