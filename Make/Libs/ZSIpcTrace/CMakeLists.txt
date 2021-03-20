# Call CMake from build directory (e.g. ./bin/msvc2019_x64) with:
# - for VS-2017: cmake -G "Visual Studio 15 2017" -A x64 -DCMAKE_BUILD_TYPE=Debug ../Make/Libs/ZSSys
# - for VS-2019: cmake -G "Visual Studio 16 2019" -A x64 -DCMAKE_BUILD_TYPE=Debug ../Make/Libs/ZSSys

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSIpcTrace)

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
    ../../../Include/Libs/ZSIpcTrace/ZSIpcTrcClient.h             ../../../Source/Libs/ZSIpcTrace/ZSIpcTrcClient.cpp
    ../../../Include/Libs/ZSIpcTrace/ZSIpcTrcDllIf.h              ../../../Source/Libs/ZSIpcTrace/ZSIpcTrcDllIf.cpp
    ../../../Include/Libs/ZSIpcTrace/ZSIpcTrcDllIfServerThread.h  ../../../Source/Libs/ZSIpcTrace/ZSIpcTrcDllIfServerThread.cpp
    ../../../Include/Libs/ZSIpcTrace/ZSIpcTrcDllMain.h            ../../../Source/Libs/ZSIpcTrace/ZSIpcTrcDllMain.cpp
    ../../../Include/Libs/ZSIpcTrace/ZSIpcTrcServer.h             ../../../Source/Libs/ZSIpcTrace/ZSIpcTrcServer.cpp
)

# Got to create the following target name:
# (ProjectName)Qt$(QT_VERSION_MAJOR)_$(Compiler)_$(Platform)[_d]
# e.g.: "ZSIpcTraceQt5_msvc2019_x64_d"

set(TargetOutputName ${PROJECT_NAME}Qt${QT_VERSION_MAJOR}_${Compiler}_${Platform})

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(TargetOutputName ${TargetOutputName}_d)
endif()

message(STATUS "TargetOutputName: ${TargetOutputName}")

add_library(${PROJECT_NAME} SHARED ${_Sources})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TargetOutputName})

target_compile_definitions(${PROJECT_NAME} PRIVATE ZSIPCTRACEDLL_EXPORTS)

target_link_libraries(${PROJECT_NAME} PUBLIC ZSSys ZSIpc)
