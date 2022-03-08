cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSIpc)

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
set(_HeadersPath ${CMAKE_CURRENT_SOURCE_DIR}/../../../Include/Libs)
set(_SourcesPath ${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/Libs)

list(APPEND _Headers
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcBlkType.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcBlkTypeE.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcBlkTypeL.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcBlkTypeNone.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcBlkTypeSL.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcBlkTypeTLV.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcBlkTypeUser.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcClient.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcClientGateway.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcCommon.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcDllMain.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcInProcMsgServer.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcInProcMsgSocket.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcServer.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcServerGateway.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcServerWrapper.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcServerWrapperInProcMsg.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcServerWrapperShm.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcServerWrapperTcp.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcSocketWrapper.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcSocketWrapperInProcMsg.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcSocketWrapperShm.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcSocketWrapperTcp.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcSrvCltBaseGateway.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcSrvCltMsg.h
)

list(APPEND _Sources
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcBlkType.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcBlkTypeE.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcBlkTypeL.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcBlkTypeNone.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcBlkTypeSL.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcBlkTypeTLV.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcBlkTypeUser.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcClient.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcClientGateway.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcCommon.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcDllMain.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcInProcMsgServer.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcInProcMsgSocket.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcServer.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcServerGateway.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcServerWrapper.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcServerWrapperInProcMsg.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcServerWrapperShm.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcServerWrapperTcp.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcSocketWrapper.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcSocketWrapperInProcMsg.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcSocketWrapperShm.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcSocketWrapperTcp.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcSrvCltBaseGateway.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcSrvCltMsg.cpp
)

list(APPEND _Resources
)

if(WIN32)
    # Add version info as meta information to Dll.
    set(_Resources ${_Resources} ${_SourcesPath}/ZSSys/ZSSysVersion.rc)
endif()

# Got to create the following target name:
# (ProjectName)Qt$(QT_VERSION_MAJOR)_$(Compiler)_$(Platform)[_d]
# e.g.: "ZSIpcQt5_msvc2019_x64_d"
set(TargetOutputName ${PROJECT_NAME}Qt${QT_VERSION_MAJOR}_${Compiler}_${Platform})
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(TargetOutputName ${TargetOutputName}_d)
endif()
message(STATUS "TargetOutputName: ${TargetOutputName}")

# Create the source groups for source tree with root at CMAKE_CURRENT_SOURCE_DIR.
source_group(TREE ${_HeadersPath}/${PROJECT_NAME} FILES ${_Headers})
source_group(TREE ${_SourcesPath}/${PROJECT_NAME} FILES ${_Sources})
#source_group(TREE ${_ResourcesPath}/${PROJECT_NAME} FILES ${_Resources})

add_library(${PROJECT_NAME} SHARED ${_Headers} ${_Sources} ${_Resources})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TargetOutputName})

target_compile_definitions(${PROJECT_NAME}
    PRIVATE ZSIPCDLL_EXPORTS)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${_HeadersPath}>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)

target_link_libraries(${PROJECT_NAME} PUBLIC ZSSys)

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME} EXPORT ${CMAKE_PROJECT_NAME}Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION .
    INCLUDES DESTINATION include)

install(FILES ${_Headers}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
    COMPONENT Devel)

message(STATUS "")
message(STATUS "<- ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "---------------------------------------------------------------------------------------------")
message(STATUS "")
