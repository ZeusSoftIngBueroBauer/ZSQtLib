# Call CMake from build directory (e.g. ./bin/msvc2019_x64) with:
# - for VS-2017: cmake -G "Visual Studio 15 2017" -A x64 -DCMAKE_BUILD_TYPE=Debug ../Make/Libs/ZSSys
# - for VS-2019: cmake -G "Visual Studio 16 2019" -A x64 -DCMAKE_BUILD_TYPE=Debug ../Make/Libs/ZSSys

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSDiagram)

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
    ../../../Include/Libs/ZSDiagram/ZSDiagObj.h              ../../../Source/Libs/ZSDiagram/ZSDiagObj.cpp
    ../../../Include/Libs/ZSDiagram/ZSDiagObjAxisLabel.h     ../../../Source/Libs/ZSDiagram/ZSDiagObjAxisLabel.cpp
    ../../../Include/Libs/ZSDiagram/ZSDiagObjCurve.h         ../../../Source/Libs/ZSDiagram/ZSDiagObjCurve.cpp
    ../../../Include/Libs/ZSDiagram/ZSDiagObjErrInfo.h       ../../../Source/Libs/ZSDiagram/ZSDiagObjErrInfo.cpp
    ../../../Include/Libs/ZSDiagram/ZSDiagObjGrid.h          ../../../Source/Libs/ZSDiagram/ZSDiagObjGrid.cpp
    ../../../Include/Libs/ZSDiagram/ZSDiagObjHistogram.h     ../../../Source/Libs/ZSDiagram/ZSDiagObjHistogram.cpp
    ../../../Include/Libs/ZSDiagram/ZSDiagObjLabel.h         ../../../Source/Libs/ZSDiagram/ZSDiagObjLabel.cpp
    ../../../Include/Libs/ZSDiagram/ZSDiagObjLine.h          ../../../Source/Libs/ZSDiagram/ZSDiagObjLine.cpp
    ../../../Include/Libs/ZSDiagram/ZSDiagObjMarker.h        ../../../Source/Libs/ZSDiagram/ZSDiagObjMarker.cpp
    ../../../Include/Libs/ZSDiagram/ZSDiagObjProgressBar.h   ../../../Source/Libs/ZSDiagram/ZSDiagObjProgressBar.cpp
    ../../../Include/Libs/ZSDiagram/ZSDiagObjRect.h          ../../../Source/Libs/ZSDiagram/ZSDiagObjRect.cpp
    ../../../Include/Libs/ZSDiagram/ZSDiagObjTable.h         ../../../Source/Libs/ZSDiagram/ZSDiagObjTable.cpp
    ../../../Include/Libs/ZSDiagram/ZSDiagObjValueProvider.h ../../../Source/Libs/ZSDiagram/ZSDiagObjValueProvider.cpp
    ../../../Include/Libs/ZSDiagram/ZSDiagram.h              ../../../Source/Libs/ZSDiagram/ZSDiagram.cpp
    ../../../Include/Libs/ZSDiagram/ZSDiagramAux.h           ../../../Source/Libs/ZSDiagram/ZSDiagramAux.cpp
    ../../../Include/Libs/ZSDiagram/ZSDiagramDllMain.h       ../../../Source/Libs/ZSDiagram/ZSDiagramDllMain.cpp
    ../../../Include/Libs/ZSDiagram/ZSDiagramFrameStyles.h   ../../../Source/Libs/ZSDiagram/ZSDiagramFrameStyles.cpp
    ../../../Include/Libs/ZSDiagram/ZSDiagramImageStyles.h   ../../../Source/Libs/ZSDiagram/ZSDiagramImageStyles.cpp
    ../../../Include/Libs/ZSDiagram/ZSDiagramLabelStyles.h   ../../../Source/Libs/ZSDiagram/ZSDiagramLabelStyles.cpp
    ../../../Include/Libs/ZSDiagram/ZSDiagramLineStyles.h    ../../../Source/Libs/ZSDiagram/ZSDiagramLineStyles.cpp
    ../../../Include/Libs/ZSDiagram/ZSDiagramMsg.h           ../../../Source/Libs/ZSDiagram/ZSDiagramMsg.cpp
    ../../../Include/Libs/ZSDiagram/ZSDiagramToolTipStyles.h ../../../Source/Libs/ZSDiagram/ZSDiagramToolTipStyles.cpp
    ../../../Include/Libs/ZSDiagram/ZSDiagScale.h            ../../../Source/Libs/ZSDiagram/ZSDiagScale.cpp
    ../../../Include/Libs/ZSDiagram/ZSDiagTrace.h            ../../../Source/Libs/ZSDiagram/ZSDiagTrace.cpp
    ../../../Images/Libs/ZSDiagram/ZSDiagram.qrc
)

# Got to create the following target name:
# (ProjectName)Qt$(QT_VERSION_MAJOR)_$(Compiler)_$(Platform)[_d]
# e.g.: "ZSDiagramQt5_msvc2019_x64_d"

set(TargetOutputName ${PROJECT_NAME}Qt${QT_VERSION_MAJOR}_${Compiler}_${Platform})

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(TargetOutputName ${TargetOutputName}_d)
endif()

message(STATUS "TargetOutputName: ${TargetOutputName}")

add_library(${PROJECT_NAME} SHARED ${_Sources})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TargetOutputName})

target_compile_definitions(${PROJECT_NAME} PRIVATE ZSDIAGRAMDLL_EXPORTS)

target_link_libraries(${PROJECT_NAME}
    PUBLIC Qt5::Gui Qt5::Widgets
    PUBLIC ZSSys ZSIpc ZSIpcTrace ZSPhysVal)

