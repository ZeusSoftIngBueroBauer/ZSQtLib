cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSAppTestIpcTraceDllIfWinApp)

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
    ${_HeadersPath}/ZSIpcTraceDllIfWinApp/stdafx.h
    ${_HeadersPath}/ZSIpcTraceDllIfWinApp/TestModule1.h
    ${_HeadersPath}/ZSIpcTraceDllIfWinApp/TestModule2.h
    ${_HeadersPath}/ZSIpcTraceDllIfWinApp/ZSAppTestIpcTraceDllIfWinApp.h
)

list(APPEND _Sources
    ${_SourcesPath}/ZSIpcTraceDllIfWinApp/Main.cpp
    ${_SourcesPath}/ZSIpcTraceDllIfWinApp/stdafx.cpp
    ${_SourcesPath}/ZSIpcTraceDllIfWinApp/TestModule1.cpp
    ${_SourcesPath}/ZSIpcTraceDllIfWinApp/TestModule2.cpp
)

list(APPEND _Interfaces
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../Include/Libs/ZSIpcTrace/ZSIpcTrcDllIf.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../Source/Libs/ZSIpcTrace/ZSIpcTrcDllIf.cpp
)

list(APPEND _Resources
)

if(WIN32)
    # Add version info as meta information to Dll.
    set(_Resources ${_Resources} ${_ResourcesPath}/../../Libs/ZSSys/ZSSysVersion.rc)
    # Icons.
    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        set(_Headers ${_Headers}
            ${_HeadersPath}/ZSIpcTraceDllIfWinApp/resource.h
            ${_HeadersPath}/ZSIpcTraceDllIfWinApp/targetver.h)
        set(_Resources ${_Resources} ${_ResourcesPath}/ZSIpcTraceDllIfWinApp/${PROJECT_NAME}.rc)
    endif()
endif()

# Got to create the following target name:
# (ProjectName)[d]
# e.g.: "ZSAppTestIpcTraceDllIfWinAppd"

set(TargetOutputName ${PROJECT_NAME})
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(TargetOutputName ${TargetOutputName}d)
endif()
message(STATUS "TargetOutputName:         ${TargetOutputName}")

add_executable(${PROJECT_NAME} WIN32 ${_Headers} ${_Sources} ${_Interfaces} ${_Resources})

set_target_properties(${PROJECT_NAME} PROPERTIES
    COMPILE_DEFINITIONS _AFXDLL,_UNICODE,UNICODE,_BIND_TO_CURRENT_CRT_VERSION,_BIND_TO_CURRENT_MFC_VERSION
    LINK_FLAGS "/ENTRY:\"wWinMainCRTStartup\""
    OUTPUT_NAME ${TargetOutputName})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Tests)

source_group("Header Files" FILES ${_Headers})
source_group("Source Files" FILES ${_Sources})
source_group("Resource Files" FILES ${_Resources})

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wno-cast-function-type")
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE USE_ZS_IPTRACE_DLL_IF)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/../Include/Apps/Test/ZSIpcTraceDllIfWinApp)

target_link_libraries(${PROJECT_NAME}
    PRIVATE ZSSys ZSSysGUI ZSTest ZSTestGUI ZSIpc ZSIpcGUI ZSIpcTrace ZSIpcTraceGUI)

# The test application needs user interaction and cannot be executed automatically.
#enable_testing()
#include(CTest)
#include(Dart)
#add_test(ZSIpcTraceDllIfWinApp ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TargetOutputName} "AutoStartTest")

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME} EXPORT ${CMAKE_PROJECT_NAME}Targets RUNTIME DESTINATION .)

message(STATUS "")
message(STATUS "<- ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "---------------------------------------------------------------------------------------------")
message(STATUS "")
