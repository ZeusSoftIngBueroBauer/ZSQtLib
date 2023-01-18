cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSDraw)

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
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawAux.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawCommon.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawDlgFormatGraphObjs.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawDlgPageSetup.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawDllMain.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawGraphicsItemsModel.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawGraphObj.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawGraphObjConnectionLine.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawGraphObjConnectionPoint.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawGraphObjEllipse.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawGraphObjGroup.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawGraphObjImage.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawGraphObjLabel.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawGraphObjLine.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawGraphObjPoint.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawGraphObjPolygon.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawGraphObjPolyline.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawGraphObjRect.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawGraphObjSelectionPoint.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawGraphObjText.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawGraphObjModelEntry.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawGraphObjsTreeModel.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawGraphObjsTreeView.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawingScene.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawingView.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawObjFactoriesModel.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawObjFactory.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawObjFactoryConnectionLine.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawObjFactoryConnectionPoint.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawObjFactoryEllipse.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawObjFactoryGroup.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawObjFactoryImage.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawObjFactoryLine.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawObjFactoryPoint.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawObjFactoryPolygon.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawObjFactoryPolyline.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawObjFactoryRect.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawObjFactoryText.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawPageSetup.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawSettings.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawUnits.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawWdgtDrawingPaperPageSetup.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawWdgtDrawingScenePageSetup.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawWdgtDrawingViewPageSetup.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawWdgtFormatGraphObjs.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawWdgtFormatGraphObjsFillStyle.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawWdgtFormatGraphObjsGeometry.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawWdgtFormatGraphObjsLineStyle.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawWdgtFormatGraphObjsLabels.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawWdgtFormatGraphObjsLabelVisibilities.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawWdgtFormatGraphObjsText.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawWdgtFormatGraphObjsTextStyle.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDrawWdgtPageSetup.h
)

list(APPEND _Sources
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawAux.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawCommon.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawDlgFormatGraphObjs.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawDlgPageSetup.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawDllMain.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawGraphicsItemsModel.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawGraphObj.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawGraphObjConnectionLine.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawGraphObjConnectionPoint.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawGraphObjEllipse.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawGraphObjGroup.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawGraphObjImage.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawGraphObjLabel.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawGraphObjLine.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawGraphObjPoint.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawGraphObjPolygon.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawGraphObjPolyline.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawGraphObjRect.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawGraphObjSelectionPoint.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawGraphObjText.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawGraphObjModelEntry.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawGraphObjsTreeModel.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawGraphObjsTreeView.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawingScene.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawingView.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawObjFactoriesModel.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawObjFactory.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawObjFactoryConnectionLine.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawObjFactoryConnectionPoint.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawObjFactoryEllipse.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawObjFactoryGroup.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawObjFactoryImage.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawObjFactoryLine.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawObjFactoryPoint.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawObjFactoryPolygon.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawObjFactoryPolyline.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawObjFactoryRect.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawObjFactoryText.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawPageSetup.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawSettings.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawUnits.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawWdgtDrawingPaperPageSetup.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawWdgtDrawingScenePageSetup.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawWdgtDrawingViewPageSetup.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawWdgtFormatGraphObjs.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawWdgtFormatGraphObjsFillStyle.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawWdgtFormatGraphObjsGeometry.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawWdgtFormatGraphObjsLineStyle.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawWdgtFormatGraphObjsLabels.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawWdgtFormatGraphObjsLabelVisibilities.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawWdgtFormatGraphObjsText.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawWdgtFormatGraphObjsTextStyle.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDrawWdgtPageSetup.cpp
)

list(APPEND _Resources
    ${_ResourcesPath}/${PROJECT_NAME}/${PROJECT_NAME}.qrc
)

list(APPEND _Images
    ${_ResourcesPath}/${PROJECT_NAME}/Images/CursorPin16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/CursorPin32x32.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/CursorRotateFree16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/CursorSelect16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/DrawConnectionLine16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/DrawConnectionPoint16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/DrawEllipse16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/DrawImage16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/DrawLabel16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/DrawLine16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/DrawPoint16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/DrawPolygon16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/DrawPolyline16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/DrawRect16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/DrawSelectionPoint16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/DrawText16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/DrawToolBrush16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/DrawToolGeometry16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/DrawToolLine16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/DrawToolMirrorHorizontal16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/DrawToolMirrorVertical16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/DrawToolPen16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/DrawToolRotateLeftBy90Degrees16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/DrawToolRotateRightBy90Degrees16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/DrawToolText16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/EditGroup16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/EditUngroup16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/FilePageSetup16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/Key16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/Refresh16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/Refresh24x24.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/Refresh32x32.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ToolBtn24x16.bmp
)

if(WIN32)
    # Add version info as meta information to Dll.
    set(_Resources ${_Resources} ${_ResourcesPath}/ZSSys/ZSSysVersion.rc)
endif()

# Got to create the following target name:
# (ProjectName)Qt$(QT_VERSION_MAJOR)$(QT_LIBINFIX)[d]
# e.g.: "ZSDrawQt5d", "ZSDrawQt5Swpd"
set(TargetOutputName ${PROJECT_NAME}Qt${QT_VERSION_MAJOR}${QT_LIBINFIX})
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(TargetOutputName ${TargetOutputName}d)
endif()
message(STATUS "TargetOutputName: ${TargetOutputName}")

add_library(${PROJECT_NAME} SHARED ${_Headers} ${_Sources} ${_Resources} ${_ImageFiles})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TargetOutputName})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Libs)

source_group("Header Files" FILES ${_Headers})
source_group("Source Files" FILES ${_Sources})
source_group("Resource Files" FILES ${_Resources})
source_group("Image Files" FILES ${_Images})

target_compile_definitions(${PROJECT_NAME} PRIVATE ZSDRAWDLL_EXPORTS)

set(_Libs ZSSysGUI ZSIpcTraceGUI ZSPhysValGUI)

target_link_libraries(${PROJECT_NAME} PUBLIC ${_Libs})

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
