# Call CMake from build directory (e.g. ./bin/msvc2019_x64) with:
# - for VS-2017: cmake -G "Visual Studio 15 2017" -A x64 -DCMAKE_BUILD_TYPE=Debug ../Make/Libs/ZSSys
# - for VS-2019: cmake -G "Visual Studio 16 2019" -A x64 -DCMAKE_BUILD_TYPE=Debug ../Make/Libs/ZSSys

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSRemCmd)

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
    ../../../Include/Libs/ZSRemCmd/ZSRemCmdClient.h     ../../../Source/Libs/ZSRemCmd/ZSRemCmdClient.cpp
    ../../../Include/Libs/ZSRemCmd/ZSRemCmdDllMain.h    ../../../Source/Libs/ZSRemCmd/ZSRemCmdDllMain.cpp
    ../../../Include/Libs/ZSRemCmd/ZSRemCmdErrNumbers.h ../../../Source/Libs/ZSRemCmd/ZSRemCmdErrNumbers.cpp
    ../../../Include/Libs/ZSRemCmd/ZSRemCmdMsg.h        ../../../Source/Libs/ZSRemCmd/ZSRemCmdMsg.cpp
    ../../../Include/Libs/ZSRemCmd/ZSRemCmdObj.h        ../../../Source/Libs/ZSRemCmd/ZSRemCmdObj.cpp
    ../../../Include/Libs/ZSRemCmd/ZSRemCmdObjPool.h    ../../../Source/Libs/ZSRemCmd/ZSRemCmdObjPool.cpp
    ../../../Include/Libs/ZSRemCmd/ZSRemCmdParser.h     ../../../Source/Libs/ZSRemCmd/ZSRemCmdParser.cpp
    ../../../Include/Libs/ZSRemCmd/ZSRemCmdServer.h     ../../../Source/Libs/ZSRemCmd/ZSRemCmdServer.cpp
    ../../../Include/Libs/ZSRemCmd/ZSRemCmdsSyst.h      ../../../Source/Libs/ZSRemCmd/ZSRemCmdsSyst.cpp
    ../../../Include/Libs/ZSRemCmd/ZSRemCmdSubSysNode.h ../../../Source/Libs/ZSRemCmd/ZSRemCmdSubSysNode.cpp
)

# Got to create the following target name:
# (ProjectName)Qt$(QT_VERSION_MAJOR)_$(Compiler)_$(Platform)[_d]
# e.g.: "ZSRemCmdQt5_msvc2019_x64_d"

set(TargetOutputName ${PROJECT_NAME}Qt${QT_VERSION_MAJOR}_${Compiler}_${Platform})

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(TargetOutputName ${TargetOutputName}_d)
endif()

message(STATUS "TargetOutputName: ${TargetOutputName}")

add_library(${PROJECT_NAME} SHARED ${_Sources})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TargetOutputName})

target_compile_definitions(${PROJECT_NAME} PRIVATE ZSREMCMDDLL_EXPORTS)

target_link_libraries(${PROJECT_NAME} PUBLIC ZSSys ZSIpc ZSIpcTrace)
