# Call CMake from build directory (e.g. ./bin/msvc2019_x64) with:
# - for VS-2017: cmake -G "Visual Studio 15 2017" -A x64 -DCMAKE_BUILD_TYPE=Debug ../Make/Libs/ZSSys
# - for VS-2019: cmake -G "Visual Studio 16 2019" -A x64 -DCMAKE_BUILD_TYPE=Debug ../Make/Libs/ZSSys

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSAppTestIpcTraceDllIfWinApp)

message(STATUS "")
message(STATUS "---------------------------------------------------------------------------------------------")
message(STATUS "Processing: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "---------------------------------------------------------------------------------------------")

message(STATUS "CMAKE_PROJECT_NAME:       ${CMAKE_PROJECT_NAME}")
message(STATUS "PROJECT_NAME:             ${PROJECT_NAME}")

message(STATUS "CMAKE_SOURCE_DIR:         ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "PROJECT_SOURCE_DIR:       ${PROJECT_SOURCE_DIR}")

message(STATUS "CMAKE_BINARY_DIR:         ${CMAKE_BINARY_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "PROJECT_BINARY_DIR:       ${PROJECT_BINARY_DIR}")

list(APPEND _Sources
                                                                                        ../../../../Source/Apps/Test/ZSIpcTraceDllIfWinApp/Main.cpp
    ../../../../Include/Apps/Test/ZSIpcTraceDllIfWinApp/Resource.h
    ../../../../Include/Apps/Test/ZSIpcTraceDllIfWinApp/ZSAppTestIpcTraceDllIfWinApp.rc
    ../../../../Include/Apps/Test/ZSIpcTraceDllIfWinApp/stdafx.h                        ../../../../Source/Apps/Test/ZSIpcTraceDllIfWinApp/stdafx.cpp
    ../../../../Include/Apps/Test/ZSIpcTraceDllIfWinApp/TestModule1.h                   ../../../../Source/Apps/Test/ZSIpcTraceDllIfWinApp/TestModule1.cpp
    ../../../../Include/Apps/Test/ZSIpcTraceDllIfWinApp/TestModule2.h                   ../../../../Source/Apps/Test/ZSIpcTraceDllIfWinApp/TestModule2.cpp
    ../../../../Include/Apps/Test/ZSIpcTraceDllIfWinApp/ZSAppTestIpcTraceDllIfWinApp.h
    ../../../../Include/Libs/ZSIpcTrace/ZSIpcTrcDllIf.h                                 ../../../../Source/Libs/ZSIpcTrace/ZSIpcTrcDllIf.cpp
)

if(WIN32)
    set(_Sources ${_Sources} ${CMAKE_CURRENT_BINARY_DIR}/../../../Libs/ZSSys/ZSSysVersion.rc)
endif()

# Got to create the following target name:
# (ProjectName)[_d]
# e.g.: "ZSAppTestIpcTraceDllIfWinApp_d"

set(TargetOutputName ${PROJECT_NAME})

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(TargetOutputName ${TargetOutputName}_d)
endif()

message(STATUS "TargetOutputName: ${TargetOutputName}")

add_executable(${PROJECT_NAME} WIN32 ${_Sources})

set_target_properties(${PROJECT_NAME} PROPERTIES
    COMPILE_DEFINITIONS _AFXDLL,_UNICODE,UNICODE,_BIND_TO_CURRENT_CRT_VERSION,_BIND_TO_CURRENT_MFC_VERSION
    LINK_FLAGS "/ENTRY:\"wWinMainCRTStartup\""
    OUTPUT_NAME ${TargetOutputName})

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wno-cast-function-type")
endif()

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/../Include/Apps/Test/ZSIpcTraceDllIfWinApp)

target_link_libraries(${PROJECT_NAME}
    PRIVATE ZSSys ZSSysGUI ZSTest ZSTestGUI ZSIpc ZSIpcGUI ZSIpcTrace ZSIpcTraceGUI)

add_test(
    NAME ZSIpcTraceDllIfWinApp
    COMMAND $<TARGET_FILE:${PROJECT_NAME}>
    COMMENT "Running ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TargetOutputName}")
