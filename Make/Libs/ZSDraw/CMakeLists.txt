# Call CMake from build directory (e.g. ./bin/msvc2019_x64) with:
# - for VS-2017: cmake -G "Visual Studio 15 2017" -A x64 -DCMAKE_BUILD_TYPE=Debug ../Make/Libs/ZSSys
# - for VS-2019: cmake -G "Visual Studio 16 2019" -A x64 -DCMAKE_BUILD_TYPE=Debug ../Make/Libs/ZSSys

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSDraw)

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
    ../../../Include/Libs/ZSDraw/ZSDrawDlgFormatGraphObjs.h           ../../../Source/Libs/ZSDraw/ZSDrawDlgFormatGraphObjs.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawDlgPageSetup.h                 ../../../Source/Libs/ZSDraw/ZSDrawDlgPageSetup.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawDllMain.h                      ../../../Source/Libs/ZSDraw/ZSDrawDllMain.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawGraphicsItemsModel.h           ../../../Source/Libs/ZSDraw/ZSDrawGraphicsItemsModel.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObj.h                     ../../../Source/Libs/ZSDraw/ZSDrawGraphObj.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObjConnectionLine.h       ../../../Source/Libs/ZSDraw/ZSDrawGraphObjConnectionLine.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObjConnectionPoint.h      ../../../Source/Libs/ZSDraw/ZSDrawGraphObjConnectionPoint.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObjEllipse.h              ../../../Source/Libs/ZSDraw/ZSDrawGraphObjEllipse.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObjGroup.h                ../../../Source/Libs/ZSDraw/ZSDrawGraphObjGroup.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObjImage.h                ../../../Source/Libs/ZSDraw/ZSDrawGraphObjImage.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObjLabel.h                ../../../Source/Libs/ZSDraw/ZSDrawGraphObjLabel.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObjLine.h                 ../../../Source/Libs/ZSDraw/ZSDrawGraphObjLine.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObjModelEntry.h           ../../../Source/Libs/ZSDraw/ZSDrawGraphObjModelEntry.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObjPoint.h                ../../../Source/Libs/ZSDraw/ZSDrawGraphObjPoint.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObjPolygon.h              ../../../Source/Libs/ZSDraw/ZSDrawGraphObjPolygon.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObjPolyline.h             ../../../Source/Libs/ZSDraw/ZSDrawGraphObjPolyline.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObjRect.h                 ../../../Source/Libs/ZSDraw/ZSDrawGraphObjRect.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObjSelectionPoint.h       ../../../Source/Libs/ZSDraw/ZSDrawGraphObjSelectionPoint.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawGraphObjText.h                 ../../../Source/Libs/ZSDraw/ZSDrawGraphObjText.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawingScene.h                     ../../../Source/Libs/ZSDraw/ZSDrawingScene.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawingView.h                      ../../../Source/Libs/ZSDraw/ZSDrawingView.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawObjFactoriesModel.h            ../../../Source/Libs/ZSDraw/ZSDrawObjFactoriesModel.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawObjFactory.h                   ../../../Source/Libs/ZSDraw/ZSDrawObjFactory.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawObjFactoryConnectionLine.h     ../../../Source/Libs/ZSDraw/ZSDrawObjFactoryConnectionLine.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawObjFactoryConnectionPoint.h    ../../../Source/Libs/ZSDraw/ZSDrawObjFactoryConnectionPoint.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawObjFactoryEllipse.h            ../../../Source/Libs/ZSDraw/ZSDrawObjFactoryEllipse.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawObjFactoryGroup.h              ../../../Source/Libs/ZSDraw/ZSDrawObjFactoryGroup.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawObjFactoryImage.h              ../../../Source/Libs/ZSDraw/ZSDrawObjFactoryImage.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawObjFactoryLine.h               ../../../Source/Libs/ZSDraw/ZSDrawObjFactoryLine.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawObjFactoryPoint.h              ../../../Source/Libs/ZSDraw/ZSDrawObjFactoryPoint.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawObjFactoryPolygon.h            ../../../Source/Libs/ZSDraw/ZSDrawObjFactoryPolygon.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawObjFactoryPolyline.h           ../../../Source/Libs/ZSDraw/ZSDrawObjFactoryPolyline.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawObjFactoryRect.h               ../../../Source/Libs/ZSDraw/ZSDrawObjFactoryRect.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawObjFactoryText.h               ../../../Source/Libs/ZSDraw/ZSDrawObjFactoryText.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawPageSetup.h                    ../../../Source/Libs/ZSDraw/ZSDrawPageSetup.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawSettings.h                     ../../../Source/Libs/ZSDraw/ZSDrawSettings.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawWdgtFormatGraphObjs.h          ../../../Source/Libs/ZSDraw/ZSDrawWdgtFormatGraphObjs.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawWdgtFormatGraphObjsFillStyle.h ../../../Source/Libs/ZSDraw/ZSDrawWdgtFormatGraphObjsFillStyle.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawWdgtFormatGraphObjsGeometry.h  ../../../Source/Libs/ZSDraw/ZSDrawWdgtFormatGraphObjsGeometry.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawWdgtFormatGraphObjsLineStyle.h ../../../Source/Libs/ZSDraw/ZSDrawWdgtFormatGraphObjsLineStyle.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawWdgtFormatGraphObjsLabels.h   ../../../Source/Libs/ZSDraw/ZSDrawWdgtFormatGraphObjsLabels.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawWdgtFormatGraphObjsText.h      ../../../Source/Libs/ZSDraw/ZSDrawWdgtFormatGraphObjsText.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawWdgtFormatGraphObjsTextStyle.h ../../../Source/Libs/ZSDraw/ZSDrawWdgtFormatGraphObjsTextStyle.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawWdgtPageSetup.h                ../../../Source/Libs/ZSDraw/ZSDrawWdgtPageSetup.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawWdgtPageSetupDrawingPaper.h    ../../../Source/Libs/ZSDraw/ZSDrawWdgtPageSetupDrawingPaper.cpp
    ../../../Include/Libs/ZSDraw/ZSDrawWdgtPageSetupDrawingScene.h    ../../../Source/Libs/ZSDraw/ZSDrawWdgtPageSetupDrawingScene.cpp
    ../../../Images/Libs/ZSDraw/ZSDraw.qrc
)

# Got to create the following target name:
# (ProjectName)Qt$(QT_VERSION_MAJOR)_$(Compiler)_$(Platform)[_d]
# e.g.: "ZSDrawQt5_msvc2019_x64_d"

set(TargetOutputName ${PROJECT_NAME}Qt${QT_VERSION_MAJOR}_${Compiler}_${Platform})

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(TargetOutputName ${TargetOutputName}_d)
endif()

message(STATUS "TargetOutputName: ${TargetOutputName}")

add_library(${PROJECT_NAME} SHARED ${_Sources})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TargetOutputName})

target_compile_definitions(${PROJECT_NAME} PRIVATE ZSDRAWDLL_EXPORTS)

target_link_libraries(${PROJECT_NAME}
    PUBLIC Qt5::Gui Qt5::Widgets
    PUBLIC ZSSys ZSSysGUI ZSIpc ZSIpcGUI ZSIpcTrace ZSIpcTraceGUI ZSPhysVal ZSPhysValGUI ZSPhysSizesGeometry ZSPhysSizesKinematics)

