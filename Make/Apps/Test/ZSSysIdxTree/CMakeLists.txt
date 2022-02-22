cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSAppTestSysIdxTree)

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

# In source code the headers are included together with the ${PROJECT_NAME} as follows:
# include "${PROJECT_NAME}/HeaderFileName.h"
# e.g. #include "ZSSys/ZSSysIdxTree.h"
# For "target_include_directories" the header path without ${PROJECT_NAME} must be
# passed and therefore _HeadersPath will be defined without ${PROJECT_NAME} so that
# this define can be used further one in this CMake list file.
# _SourcePaths will be defined in the same way.
set(_HeadersPath ${CMAKE_CURRENT_SOURCE_DIR}/../../../../Include/Apps/Test)
set(_SourcesPath ${CMAKE_CURRENT_SOURCE_DIR}/../../../../Source/Apps/Test)

list(APPEND _Headers
    ${_HeadersPath}/ZSSysIdxTree/App.h
    ${_HeadersPath}/ZSSysIdxTree/MainWindow.h
    ${_HeadersPath}/ZSSysIdxTree/WidgetCentral.h
    ${_HeadersPath}/ZSSysIdxTree/Test.h
)

list(APPEND _Sources
    ${_SourcesPath}/ZSSysIdxTree/Main.cpp
    ${_SourcesPath}/ZSSysIdxTree/App.cpp
    ${_SourcesPath}/ZSSysIdxTree/MainWindow.cpp
    ${_SourcesPath}/ZSSysIdxTree/WidgetCentral.cpp
    ${_SourcesPath}/ZSSysIdxTree/Test.cpp
)

if(WIN32)
    # Add version info as meta information to exe.
    set(_Sources ${_Sources} ${CMAKE_CURRENT_BINARY_DIR}/../../../Libs/ZSSys/ZSSysVersion.rc)
endif()

# Got to create the following target name:
# (ProjectName)[_d]
# e.g.: "ZSAppTestIdxTree_d"

set(TargetOutputName ${PROJECT_NAME})
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(TargetOutputName ${TargetOutputName}_d)
endif()
message(STATUS "TargetOutputName:         ${TargetOutputName}")

if(WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${_Headers} ${_Sources})
else()
    add_executable(${PROJECT_NAME} ${_Headers} ${_Sources})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TargetOutputName})

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/../Include/Apps/Test/ZSSysIdxTree)

target_link_libraries(${PROJECT_NAME}
    PRIVATE ZSSys ZSSysGUI ZSTest ZSTestGUI ZSIpc ZSIpcGUI ZSIpcTrace ZSIpcTraceGUI)

add_test(
    NAME ZSSysIdxTree
    COMMAND $<TARGET_FILE:${PROJECT_NAME}>
    COMMENT "Running ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TargetOutputName}")

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME} EXPORT ${CMAKE_PROJECT_NAME}Targets RUNTIME DESTINATION .)

message(STATUS "")
message(STATUS "<- ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "---------------------------------------------------------------------------------------------")
message(STATUS "")
