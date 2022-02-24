cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSIpcTrace)

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
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcTrcClient.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcTrcDllIf.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcTrcDllIfServerThread.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcTrcDllMain.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSIpcTrcServer.h
)

list(APPEND _Sources
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcTrcClient.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcTrcDllIf.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcTrcDllIfServerThread.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcTrcDllMain.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSIpcTrcServer.cpp
)

if(WIN32)
    # Add version info as meta information to Dll.
    set(_Sources ${_Sources} ${CMAKE_CURRENT_BINARY_DIR}/../../Libs/ZSSys/ZSSysVersion.rc)
endif()

# Got to create the following target name:
# (ProjectName)Qt$(QT_VERSION_MAJOR)_$(Compiler)_$(Platform)[_d]
# e.g.: "ZSIpcTraceQt5_msvc2019_x64_d"
set(TargetOutputName ${PROJECT_NAME}Qt${QT_VERSION_MAJOR}_${Compiler}_${Platform})
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(TargetOutputName ${TargetOutputName}_d)
endif()
message(STATUS "TargetOutputName:         ${TargetOutputName}")

add_library(${PROJECT_NAME} SHARED ${_Headers} ${_Sources})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TargetOutputName})

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wno-cast-function-type")
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE ZSIPCTRACEDLL_EXPORTS)
target_compile_definitions(${PROJECT_NAME} PRIVATE USE_ZS_IPTRACE_DLL_IF)

target_link_libraries(${PROJECT_NAME} PUBLIC ZSSys ZSIpc)
if(UNIX)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_DL_LIBS})
endif()

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME} EXPORT ${CMAKE_PROJECT_NAME}Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION .
    INCLUDES DESTINATION include)

install(FILES ${_Headers}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
    COMPONENT Devel)

install(FILES ${_SourcesPath}/${PROJECT_NAME}/ZSIpcTrcDllIf.cpp
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/../Source/${PROJECT_NAME}"
    COMPONENT Devel)

message(STATUS "")
message(STATUS "<- ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "---------------------------------------------------------------------------------------------")
message(STATUS "")
