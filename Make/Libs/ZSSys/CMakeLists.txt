cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSSys)

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
set(_HeadersPath ${CMAKE_CURRENT_SOURCE_DIR}/../../../Include/Libs)
set(_SourcesPath ${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/Libs)
set(_ResourcesPath ${CMAKE_CURRENT_SOURCE_DIR}/../../../Resources/Libs)

list(APPEND _Headers
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysApp.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysAux.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysCommon.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysDataRateCalculator.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysDllMain.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysEnumEntry.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysEnumeration.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysEnumerationIdxTree.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysEnumTemplate.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysEnumVal.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysEnumValArr.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysErrCode.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysErrLog.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysErrResult.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysException.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysIdxTree.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysIdxTreeEntry.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysLogFile.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysLogger.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysLoggerIdxTree.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysLogServer.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysMath.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysMemLeakDump.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysMsg.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysMsgProtocol.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysMutex.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysRequest.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysRequestExecTree.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysRequestQueue.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysRequestSequencer.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysSleeperThread.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysTime.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysTrcAdminObj.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysTrcAdminObjIdxTree.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysTrcMethod.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysTrcMthFile.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysTrcServer.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysWaitCondition.h
)

list(APPEND _Sources
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysApp.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysAux.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysCommon.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysDataRateCalculator.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysDllMain.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysEnumEntry.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysEnumeration.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysEnumerationIdxTree.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysEnumVal.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysEnumValArr.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysErrCode.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysErrLog.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysErrResult.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysException.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysIdxTree.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysIdxTreeEntry.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysLogFile.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysLogger.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysLoggerIdxTree.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysLogServer.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysMath.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysMemLeakDump.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysMsg.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysMsgProtocol.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysMutex.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysRequest.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysRequestExecTree.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysRequestQueue.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysRequestSequencer.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysSleeperThread.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysTime.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysTrcAdminObj.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysTrcAdminObjIdxTree.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysTrcMethod.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysTrcMthFile.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysTrcServer.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysWaitCondition.cpp
)

list(APPEND _Resources
)

configure_file(
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysVersion.h.in
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysVersion.h)
set(_Headers ${_Headers} ${_HeadersPath}/${PROJECT_NAME}/ZSSysVersion.h)

if(WIN32)
    # Add version info as meta information to Dll.
    configure_file(
        ${_ResourcesPath}/${PROJECT_NAME}/ZSSysVersion.rc.in
        ${_ResourcesPath}/${PROJECT_NAME}/ZSSysVersion.rc
        @ONLY)
    set(_Resources ${_Resources} ${_ResourcesPath}/${PROJECT_NAME}/ZSSysVersion.rc)
endif()

# Got to create the following output name:
# (ProjectName)Qt$(QT_VERSION_MAJOR)[d]
# e.g.: "ZSSysQt5d"
set(TargetOutputName ${PROJECT_NAME}Qt${QT_VERSION_MAJOR})
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(TargetOutputName ${TargetOutputName}d)
endif()
message(STATUS "TargetOutputName:         ${TargetOutputName}")

add_library(${PROJECT_NAME} SHARED ${_Headers} ${_Sources} ${_Resources})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TargetOutputName})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Libs)

source_group("Header Files" FILES ${_Headers})
source_group("Source Files" FILES ${_Sources})
source_group("Resource Files" FILES ${_Resources})

target_compile_definitions(${PROJECT_NAME}
    PRIVATE ZSSYSDLL_EXPORTS
    PUBLIC MDd _UNICODE UNICODE)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${_HeadersPath}>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)

if(QT_VERSION_MAJOR EQUAL 4)
    if(QT_LIB)
        set(_Libs ${QT_LIB})
    else()
        set(_Libs Qt4::QtCore Qt4::QtXml Qt4::QtNetwork)
    endif()
elseif(QT_VERSION_MAJOR EQUAL 5)
    set(_Libs Qt5::Core Qt5::Xml Qt5::Network)
endif()
if(WIN32)
    set(_Libs ${_Libs} Dbghelp)
endif()

if(QT_VERSION_MAJOR EQUAL 4)
    if(QT_LIB)
        target_include_directories(${PROJECT_NAME}
            PUBLIC ${Qt4_INCLUDE_DIRS})
    endif()
endif()
target_link_libraries(${PROJECT_NAME} ${_Libs})

# Copy Qt Dlls to bin directory.
include(${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}CopyQtDlls.cmake)

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
