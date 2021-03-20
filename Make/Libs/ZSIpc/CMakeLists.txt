# Call CMake from build directory (e.g. ./bin/msvc2019_x64) with:
# - for VS-2017: cmake -G "Visual Studio 15 2017" -A x64 -DCMAKE_BUILD_TYPE=Debug ../Make/Libs/ZSSys
# - for VS-2019: cmake -G "Visual Studio 16 2019" -A x64 -DCMAKE_BUILD_TYPE=Debug ../Make/Libs/ZSSys

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSIpc)

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
    ../../../Include/Libs/ZSIpc/ZSIpcBlkType.h                ../../../Source/Libs/ZSIpc/ZSIpcBlkType.cpp
    ../../../Include/Libs/ZSIpc/ZSIpcBlkTypeE.h               ../../../Source/Libs/ZSIpc/ZSIpcBlkTypeE.cpp
    ../../../Include/Libs/ZSIpc/ZSIpcBlkTypeL.h               ../../../Source/Libs/ZSIpc/ZSIpcBlkTypeL.cpp
    ../../../Include/Libs/ZSIpc/ZSIpcBlkTypeNone.h            ../../../Source/Libs/ZSIpc/ZSIpcBlkTypeNone.cpp
    ../../../Include/Libs/ZSIpc/ZSIpcBlkTypeSL.h              ../../../Source/Libs/ZSIpc/ZSIpcBlkTypeSL.cpp
    ../../../Include/Libs/ZSIpc/ZSIpcBlkTypeTLV.h             ../../../Source/Libs/ZSIpc/ZSIpcBlkTypeTLV.cpp
    ../../../Include/Libs/ZSIpc/ZSIpcBlkTypeUser.h            ../../../Source/Libs/ZSIpc/ZSIpcBlkTypeUser.cpp
    ../../../Include/Libs/ZSIpc/ZSIpcClient.h                 ../../../Source/Libs/ZSIpc/ZSIpcClient.cpp
    ../../../Include/Libs/ZSIpc/ZSIpcClientGateway.h          ../../../Source/Libs/ZSIpc/ZSIpcClientGateway.cpp
    ../../../Include/Libs/ZSIpc/ZSIpcCommon.h                 ../../../Source/Libs/ZSIpc/ZSIpcCommon.cpp
    ../../../Include/Libs/ZSIpc/ZSIpcDllMain.h                ../../../Source/Libs/ZSIpc/ZSIpcDllMain.cpp
    ../../../Include/Libs/ZSIpc/ZSIpcInProcMsgServer.h        ../../../Source/Libs/ZSIpc/ZSIpcInProcMsgServer.cpp
    ../../../Include/Libs/ZSIpc/ZSIpcInProcMsgSocket.h        ../../../Source/Libs/ZSIpc/ZSIpcInProcMsgSocket.cpp
    ../../../Include/Libs/ZSIpc/ZSIpcServer.h                 ../../../Source/Libs/ZSIpc/ZSIpcServer.cpp
    ../../../Include/Libs/ZSIpc/ZSIpcServerGateway.h          ../../../Source/Libs/ZSIpc/ZSIpcServerGateway.cpp
    ../../../Include/Libs/ZSIpc/ZSIpcServerWrapper.h          ../../../Source/Libs/ZSIpc/ZSIpcServerWrapper.cpp
    ../../../Include/Libs/ZSIpc/ZSIpcServerWrapperInProcMsg.h ../../../Source/Libs/ZSIpc/ZSIpcServerWrapperInProcMsg.cpp
    ../../../Include/Libs/ZSIpc/ZSIpcServerWrapperShm.h       ../../../Source/Libs/ZSIpc/ZSIpcServerWrapperShm.cpp
    ../../../Include/Libs/ZSIpc/ZSIpcServerWrapperTcp.h       ../../../Source/Libs/ZSIpc/ZSIpcServerWrapperTcp.cpp
    ../../../Include/Libs/ZSIpc/ZSIpcSocketWrapper.h          ../../../Source/Libs/ZSIpc/ZSIpcSocketWrapper.cpp
    ../../../Include/Libs/ZSIpc/ZSIpcSocketWrapperInProcMsg.h ../../../Source/Libs/ZSIpc/ZSIpcSocketWrapperInProcMsg.cpp
    ../../../Include/Libs/ZSIpc/ZSIpcSocketWrapperShm.h       ../../../Source/Libs/ZSIpc/ZSIpcSocketWrapperShm.cpp
    ../../../Include/Libs/ZSIpc/ZSIpcSocketWrapperTcp.h       ../../../Source/Libs/ZSIpc/ZSIpcSocketWrapperTcp.cpp
    ../../../Include/Libs/ZSIpc/ZSIpcSrvCltBaseGateway.h      ../../../Source/Libs/ZSIpc/ZSIpcSrvCltBaseGateway.cpp
    ../../../Include/Libs/ZSIpc/ZSIpcSrvCltMsg.h              ../../../Source/Libs/ZSIpc/ZSIpcSrvCltMsg.cpp
)

# Got to create the following target name:
# (ProjectName)Qt$(QT_VERSION_MAJOR)_$(Compiler)_$(Platform)[_d]
# e.g.: "ZSIpcQt5_msvc2019_x64_d"

set(TargetOutputName ${PROJECT_NAME}Qt${QT_VERSION_MAJOR}_${Compiler}_${Platform})

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(TargetOutputName ${TargetOutputName}_d)
endif()

message(STATUS "TargetOutputName: ${TargetOutputName}")

add_library(${PROJECT_NAME} SHARED ${_Sources})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TargetOutputName})

target_compile_definitions(${PROJECT_NAME} PRIVATE ZSIPCDLL_EXPORTS)

target_link_libraries(${PROJECT_NAME} PUBLIC ZSSys)
