cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSAppTestIpcTrace)

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

list(APPEND _Headers
    ${_HeadersPath}/../../Libs/ZSSys/ZSSysVersion.h
    ${_HeadersPath}/ZSIpcTrace/App.h
    ${_HeadersPath}/ZSIpcTrace/MainWindow.h
    ${_HeadersPath}/ZSIpcTrace/WidgetCentral.h
    ${_HeadersPath}/ZSIpcTrace/MsgTest.h
    ${_HeadersPath}/ZSIpcTrace/Test.h
    ${_HeadersPath}/ZSIpcTrace/MyClass1.h
    ${_HeadersPath}/ZSIpcTrace/MyClass2.h
    ${_HeadersPath}/ZSIpcTrace/MyClass3.h
)

configure_file(
    ${_HeadersPath}/ZSIpcTrace/TestConfig.h.in
    ${_HeadersPath}/ZSIpcTrace/TestConfig.h)
set(_Headers ${_Headers} ${_HeadersPath}/ZSIpcTrace/TestConfig.h)

list(APPEND _Sources
    ${_SourcesPath}/ZSIpcTrace/Main.cpp
    ${_SourcesPath}/ZSIpcTrace/App.cpp
    ${_SourcesPath}/ZSIpcTrace/MainWindow.cpp
    ${_SourcesPath}/ZSIpcTrace/WidgetCentral.cpp
    ${_SourcesPath}/ZSIpcTrace/MsgTest.cpp
    ${_SourcesPath}/ZSIpcTrace/Test.cpp
    ${_SourcesPath}/ZSIpcTrace/MyClass1.cpp
    ${_SourcesPath}/ZSIpcTrace/MyClass2.cpp
    ${_SourcesPath}/ZSIpcTrace/MyClass3.cpp
)

list(APPEND _Resources
)

if(WIN32)
    set(_Headers ${_Headers}
        ${_HeadersPath}/ZSIpcTrace/resource.h
        ${_HeadersPath}/ZSIpcTrace/targetver.h)
    # Add version info as meta information to Dll.
    set(_Resources ${_Resources} ${_ResourcesPath}/../../Libs/ZSSys/ZSSysVersion.rc)
    # Icons.
    set(_Resources ${_Resources} ${_ResourcesPath}/ZSIpcTrace/${PROJECT_NAME}.rc)
endif()

# Got to create the following target name:
# (ProjectName)[d]
# e.g.: "ZSAppTestIpcTraced"
set(TargetOutputName ${PROJECT_NAME})
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(TargetOutputName ${TargetOutputName}d)
endif()
message(STATUS "TargetOutputName:         ${TargetOutputName}")

if(WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${_Headers} ${_Sources} ${_Resources})
else()
    add_executable(${PROJECT_NAME} ${_Headers} ${_Sources} ${_Resources})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TargetOutputName})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Tests)

source_group("Header Files" FILES ${_Headers})
source_group("Source Files" FILES ${_Sources})
source_group("Resource Files" FILES ${_Resources})

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/../Include/Apps/Test/ZSIpcTrace)

target_link_libraries(${PROJECT_NAME}
    PRIVATE ZSSys ZSSysGUI ZSTest ZSTestGUI ZSIpc ZSIpcGUI ZSIpcTrace ZSIpcTraceGUI)

enable_testing()
include(CTest)
include(Dart)
add_test(ZSIpcTrace ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TargetOutputName} "AutoStartTest")

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME} EXPORT ${CMAKE_PROJECT_NAME}Targets RUNTIME DESTINATION .)

message(STATUS "")
message(STATUS "<- ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "---------------------------------------------------------------------------------------------")
message(STATUS "")
