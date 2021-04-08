# Call CMake from build directory (e.g. ./bin/msvc2019_x64) with:
# - for VS-2017: cmake -G "Visual Studio 15 2017" -A x64 -DCMAKE_BUILD_TYPE=Debug ../Make/Libs/ZSSys
# - for VS-2019: cmake -G "Visual Studio 16 2019" -A x64 -DCMAKE_BUILD_TYPE=Debug ../Make/Libs/ZSSys

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSSys)

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
    ../../../Include/Libs/ZSSys/ZSSysApp.h                ../../../Source/Libs/ZSSys/ZSSysApp.cpp
    ../../../Include/Libs/ZSSys/ZSSysAux.h                ../../../Source/Libs/ZSSys/ZSSysAux.cpp
    ../../../Include/Libs/ZSSys/ZSSysCommon.h             ../../../Source/Libs/ZSSys/ZSSysCommon.cpp
    ../../../Include/Libs/ZSSys/ZSSysDllMain.h            ../../../Source/Libs/ZSSys/ZSSysDllMain.cpp
    ../../../Include/Libs/ZSSys/ZSSysEnumEntry.h          ../../../Source/Libs/ZSSys/ZSSysEnumEntry.cpp
    ../../../Include/Libs/ZSSys/ZSSysEnumeration.h        ../../../Source/Libs/ZSSys/ZSSysEnumeration.cpp
    ../../../Include/Libs/ZSSys/ZSSysEnumerationIdxTree.h ../../../Source/Libs/ZSSys/ZSSysEnumerationIdxTree.cpp
    ../../../Include/Libs/ZSSys/ZSSysEnumTemplate.h
    ../../../Include/Libs/ZSSys/ZSSysEnumVal.h            ../../../Source/Libs/ZSSys/ZSSysEnumVal.cpp
    ../../../Include/Libs/ZSSys/ZSSysEnumValArr.h         ../../../Source/Libs/ZSSys/ZSSysEnumValArr.cpp
    ../../../Include/Libs/ZSSys/ZSSysErrCode.h            ../../../Source/Libs/ZSSys/ZSSysErrCode.cpp
    ../../../Include/Libs/ZSSys/ZSSysErrLog.h             ../../../Source/Libs/ZSSys/ZSSysErrLog.cpp
    ../../../Include/Libs/ZSSys/ZSSysErrResult.h          ../../../Source/Libs/ZSSys/ZSSysErrResult.cpp
    ../../../Include/Libs/ZSSys/ZSSysException.h          ../../../Source/Libs/ZSSys/ZSSysException.cpp
    ../../../Include/Libs/ZSSys/ZSSysIdxTree.h            ../../../Source/Libs/ZSSys/ZSSysIdxTree.cpp
    ../../../Include/Libs/ZSSys/ZSSysIdxTreeEntries.h     ../../../Source/Libs/ZSSys/ZSSysIdxTreeEntries.cpp
    ../../../Include/Libs/ZSSys/ZSSysLogFile.h            ../../../Source/Libs/ZSSys/ZSSysLogFile.cpp
    ../../../Include/Libs/ZSSys/ZSSysMath.h               ../../../Source/Libs/ZSSys/ZSSysMath.cpp
    ../../../Include/Libs/ZSSys/ZSSysMemLeakDump.h        ../../../Source/Libs/ZSSys/ZSSysMemLeakDump.cpp
    ../../../Include/Libs/ZSSys/ZSSysMsg.h                ../../../Source/Libs/ZSSys/ZSSysMsg.cpp
    ../../../Include/Libs/ZSSys/ZSSysMsgProtocol.h        ../../../Source/Libs/ZSSys/ZSSysMsgProtocol.cpp
    ../../../Include/Libs/ZSSys/ZSSysObjPool.h            ../../../Source/Libs/ZSSys/ZSSysObjPool.cpp
    ../../../Include/Libs/ZSSys/ZSSysRequest.h            ../../../Source/Libs/ZSSys/ZSSysRequest.cpp
    ../../../Include/Libs/ZSSys/ZSSysRequestExecTree.h    ../../../Source/Libs/ZSSys/ZSSysRequestExecTree.cpp
    ../../../Include/Libs/ZSSys/ZSSysRequestQueue.h       ../../../Source/Libs/ZSSys/ZSSysRequestQueue.cpp
    ../../../Include/Libs/ZSSys/ZSSysRequestSequencer.h   ../../../Source/Libs/ZSSys/ZSSysRequestSequencer.cpp
    ../../../Include/Libs/ZSSys/ZSSysSleeperThread.h      ../../../Source/Libs/ZSSys/ZSSysSleeperThread.cpp
    ../../../Include/Libs/ZSSys/ZSSysTime.h               ../../../Source/Libs/ZSSys/ZSSysTime.cpp
    ../../../Include/Libs/ZSSys/ZSSysTrcAdminObj.h        ../../../Source/Libs/ZSSys/ZSSysTrcAdminObj.cpp
    ../../../Include/Libs/ZSSys/ZSSysTrcAdminObjIdxTree.h ../../../Source/Libs/ZSSys/ZSSysTrcAdminObjIdxTree.cpp
    ../../../Include/Libs/ZSSys/ZSSysTrcMethod.h          ../../../Source/Libs/ZSSys/ZSSysTrcMethod.cpp
    ../../../Include/Libs/ZSSys/ZSSysTrcMthFile.h         ../../../Source/Libs/ZSSys/ZSSysTrcMthFile.cpp
    ../../../Include/Libs/ZSSys/ZSSysTrcServer.h          ../../../Source/Libs/ZSSys/ZSSysTrcServer.cpp
    ../../../Include/Libs/ZSSys/ZSSysVersion.h
)

# Got to create the following output name:
# (ProjectName)Qt$(QT_VERSION_MAJOR)_$(Compiler)_$(Platform)[_d]
# e.g.: "ZSSysQt5_msvc2019_x64_d"

set(TargetOutputName ${PROJECT_NAME}Qt${QT_VERSION_MAJOR}_${Compiler}_${Platform})

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(TargetOutputName ${TargetOutputName}_d)
endif()

message(STATUS "TargetOutputName: ${TargetOutputName}")

add_library(${PROJECT_NAME} SHARED ${_Sources})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TargetOutputName})

target_compile_definitions(${PROJECT_NAME}
    PRIVATE ZSSYSDLL_EXPORTS
    PUBLIC MDd _UNICODE UNICODE)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_SOURCE_DIR}/../Include/Libs)

if(QT_VERSION_MAJOR EQUAL 4)
    target_link_libraries(${PROJECT_NAME}
        PUBLIC Qt4::QtCore
        PUBLIC Qt4::QtXml
        PUBLIC Qt4::QtNetwork)
    if(CMAKE_BUILD_TYPE MATCHES Debug)
        message(STATUS "Copying Qt4 Debug Dlls")
        # Please note: Executed during build but not during configure process.
        add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
            COMMENT "Copy Qt Dlls"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QT_BINARY_DIR}/QtCored4.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/QtCored4.dll
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QT_BINARY_DIR}/QtXmld4.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/QtXmld4.dll
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QT_BINARY_DIR}/QtNetworkd4.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/QtNetworkd4.dll
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QT_BINARY_DIR}/QtGuid4.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/QtGuid4.dll)
    else()
        message(STATUS "Copying Qt4 Release Dlls")
        # Please note: Executed during build but not during configure process.
        add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
            COMMENT "Copy Qt Dlls"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QT_BINARY_DIR}/QtCore4.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/QtCore4.dll
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QT_BINARY_DIR}/QtXml4.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/QtXml4.dll
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QT_BINARY_DIR}/QtNetwork4.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/QtNetwork4.dll
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QT_BINARY_DIR}/QtGui4.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/QtGui4.dll)
    endif()
elseif(QT_VERSION_MAJOR EQUAL 5)
    target_link_libraries(${PROJECT_NAME}
        PUBLIC Qt5::Core
        PUBLIC Qt5::Xml
        PUBLIC Qt5::Network)
    message(STATUS "Copying Qt5 Dlls")
    # Please note: Executed during build but not during configure process.
    add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
        COMMENT "Copy Qt Dlls"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Core> $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Xml> $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Network> $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Gui> $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Widgets> $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QT_BINARY_DIR}/../plugins/platforms/qwindows.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins/platforms/qwindows.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QT_BINARY_DIR}/../plugins/platforms/qwindowsd.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins/platforms/qwindowsd.dll)
endif()

