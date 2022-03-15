cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSAppTrcMthClient)

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

list(APPEND _Headers
    ${_HeadersPath}/ZSTrcMthClient/App.h
    ${_HeadersPath}/ZSTrcMthClient/MainWindow.h
    ${_HeadersPath}/ZSTrcMthClient/WidgetCentral.h
)

list(APPEND _Sources
    ${_SourcesPath}/ZSTrcMthClient/Main.cpp
    ${_SourcesPath}/ZSTrcMthClient/App.cpp
    ${_SourcesPath}/ZSTrcMthClient/MainWindow.cpp
    ${_SourcesPath}/ZSTrcMthClient/WidgetCentral.cpp
)

list(APPEND _Resources
)

if(WIN32)
    # Add version info as meta information to Dll.
    set(_Resources ${_Resources} ${_SourcesPath}/../../Libs/ZSSys/ZSSysVersion.rc)
endif()

# Got to create the following target name:
# (ProjectName)[_d]
# e.g.: "ZSAppDraw_d"
set(TargetOutputName ${PROJECT_NAME})
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(TargetOutputName ${TargetOutputName}_d)
endif()
message(STATUS "TargetOutputName:         ${TargetOutputName}")

if(WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${_Headers} ${_Sources} ${_Resources})
else()
    add_executable(${PROJECT_NAME} ${_Headers} ${_Sources} ${_Resources})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TargetOutputName})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Products)

source_group(TREE ${_HeadersPath}/ZSTrcMthClient PREFIX "Header Files" FILES ${_Headers})
source_group(TREE ${_SourcesPath}/ZSTrcMthClient PREFIX "Source Files" FILES ${_Sources})

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/../Include/Apps/Products/ZSTrcMthClient)

target_link_libraries(${PROJECT_NAME}
    PRIVATE ZSSys ZSSysGUI ZSTest ZSTestGUI ZSIpc ZSIpcGUI ZSIpcTrace ZSIpcTraceGUI)

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME} EXPORT ${CMAKE_PROJECT_NAME}Targets RUNTIME DESTINATION .)

message(STATUS "")
message(STATUS "<- ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "---------------------------------------------------------------------------------------------")
message(STATUS "")
