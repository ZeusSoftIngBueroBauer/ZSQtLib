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

list(APPEND _HeadersCommon
    ${_HeadersPath}/${PROJECT_NAME}/Common/ZSDrawAux.h
    ${_HeadersPath}/${PROJECT_NAME}/Common/ZSDrawCommon.h
    ${_HeadersPath}/${PROJECT_NAME}/Common/ZSDrawDllMain.h
)

list(APPEND _HeadersDrawing
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/ZSDrawingScene.h
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/ZSDrawingView.h
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/ZSDrawSettings.h
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/ZSDrawUnits.h
)

list(APPEND _HeadersDrawingPageSetup
    ${_HeadersPath}/${PROJECT_NAME}/DrawingPageSetup/ZSDrawDlgPageSetup.h
    ${_HeadersPath}/${PROJECT_NAME}/DrawingPageSetup/ZSDrawingViewPageSetup.h
    ${_HeadersPath}/${PROJECT_NAME}/DrawingPageSetup/ZSDrawPageSetup.h
    ${_HeadersPath}/${PROJECT_NAME}/DrawingPageSetup/ZSDrawWdgtDrawingPaperPageSetup.h
    ${_HeadersPath}/${PROJECT_NAME}/DrawingPageSetup/ZSDrawWdgtDrawingScenePageSetup.h
    ${_HeadersPath}/${PROJECT_NAME}/DrawingPageSetup/ZSDrawWdgtDrawingViewPageSetup.h
    ${_HeadersPath}/${PROJECT_NAME}/DrawingPageSetup/ZSDrawWdgtPageSetup.h
)

list(APPEND _HeadersGraphObjFactories
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjFactories/ZSDrawObjFactoriesModel.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjFactories/ZSDrawObjFactory.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjFactories/ZSDrawObjFactoryConnectionLine.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjFactories/ZSDrawObjFactoryConnectionPoint.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjFactories/ZSDrawObjFactoryEllipse.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjFactories/ZSDrawObjFactoryGroup.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjFactories/ZSDrawObjFactoryImage.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjFactories/ZSDrawObjFactoryLine.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjFactories/ZSDrawObjFactoryPoint.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjFactories/ZSDrawObjFactoryPolygon.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjFactories/ZSDrawObjFactoryPolyline.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjFactories/ZSDrawObjFactoryRect.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjFactories/ZSDrawObjFactoryText.h
)

list(APPEND _HeadersGraphObjFormat
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjFormat/ZSDrawDlgFormatGraphObjs.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjFormat/ZSDrawWdgtFormatGraphObjs.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjFormat/ZSDrawWdgtFormatGraphObjsFillStyle.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjFormat/ZSDrawWdgtFormatGraphObjsGeometry.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjFormat/ZSDrawWdgtFormatGraphObjsLineStyle.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjFormat/ZSDrawWdgtFormatGraphObjsLabels.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjFormat/ZSDrawWdgtFormatGraphObjsLabelVisibilities.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjFormat/ZSDrawWdgtFormatGraphObjsText.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjFormat/ZSDrawWdgtFormatGraphObjsTextStyle.h
)

list(APPEND _HeadersGraphObjs
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObj.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjConnectionLine.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjConnectionPoint.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjEllipse.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjEventFct.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjGroup.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjImage.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjLabel.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjLine.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjModelEntry.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjPoint.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjPolygon.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjPolyline.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjRect.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjSelectionPoint.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjsTreeModel.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjsTreeView.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjsTreeViewWdgt.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjsTreeWdgt.h
    ${_HeadersPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjText.h
)

list(APPEND _Headers
    ${_HeadersCommon}
    ${_HeadersDrawing}
    ${_HeadersDrawingPageSetup}
    ${_HeadersGraphObjFactories}
    ${_HeadersGraphObjFormat}
    ${_HeadersGraphObjs}
)

list(APPEND _SourcesCommon
    ${_SourcesPath}/${PROJECT_NAME}/Common/ZSDrawAux.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Common/ZSDrawCommon.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Common/ZSDrawDllMain.cpp
)

list(APPEND _SourcesDrawing
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/ZSDrawingScene.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/ZSDrawingView.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/ZSDrawSettings.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/ZSDrawUnits.cpp
)

list(APPEND _SourcesDrawingPageSetup
    ${_SourcesPath}/${PROJECT_NAME}/DrawingPageSetup/ZSDrawDlgPageSetup.cpp
    ${_SourcesPath}/${PROJECT_NAME}/DrawingPageSetup/ZSDrawingViewPageSetup.cpp
    ${_SourcesPath}/${PROJECT_NAME}/DrawingPageSetup/ZSDrawPageSetup.cpp
    ${_SourcesPath}/${PROJECT_NAME}/DrawingPageSetup/ZSDrawWdgtDrawingPaperPageSetup.cpp
    ${_SourcesPath}/${PROJECT_NAME}/DrawingPageSetup/ZSDrawWdgtDrawingScenePageSetup.cpp
    ${_SourcesPath}/${PROJECT_NAME}/DrawingPageSetup/ZSDrawWdgtDrawingViewPageSetup.cpp
    ${_SourcesPath}/${PROJECT_NAME}/DrawingPageSetup/ZSDrawWdgtPageSetup.cpp
)

list(APPEND _SourcesGraphObjFactories
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjFactories/ZSDrawObjFactoriesModel.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjFactories/ZSDrawObjFactory.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjFactories/ZSDrawObjFactoryConnectionLine.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjFactories/ZSDrawObjFactoryConnectionPoint.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjFactories/ZSDrawObjFactoryEllipse.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjFactories/ZSDrawObjFactoryGroup.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjFactories/ZSDrawObjFactoryImage.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjFactories/ZSDrawObjFactoryLine.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjFactories/ZSDrawObjFactoryPoint.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjFactories/ZSDrawObjFactoryPolygon.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjFactories/ZSDrawObjFactoryPolyline.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjFactories/ZSDrawObjFactoryRect.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjFactories/ZSDrawObjFactoryText.cpp
)

list(APPEND _SourcesGraphObjFormat
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjFormat/ZSDrawDlgFormatGraphObjs.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjFormat/ZSDrawWdgtFormatGraphObjs.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjFormat/ZSDrawWdgtFormatGraphObjsFillStyle.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjFormat/ZSDrawWdgtFormatGraphObjsGeometry.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjFormat/ZSDrawWdgtFormatGraphObjsLineStyle.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjFormat/ZSDrawWdgtFormatGraphObjsLabels.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjFormat/ZSDrawWdgtFormatGraphObjsLabelVisibilities.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjFormat/ZSDrawWdgtFormatGraphObjsText.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjFormat/ZSDrawWdgtFormatGraphObjsTextStyle.cpp
)

list(APPEND _SourcesGraphObjs
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObj.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjConnectionLine.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjConnectionPoint.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjEllipse.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjGroup.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjImage.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjLabel.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjLine.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjModelEntry.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjPoint.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjPolygon.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjPolyline.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjRect.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjSelectionPoint.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjsTreeModel.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjsTreeView.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjsTreeViewWdgt.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjsTreeWdgt.cpp
    ${_SourcesPath}/${PROJECT_NAME}/GraphObjs/ZSDrawGraphObjText.cpp
)

list(APPEND _Sources
    ${_SourcesCommon}
    ${_SourcesDrawing}
    ${_SourcesDrawingPageSetup}
    ${_SourcesGraphObjFactories}
    ${_SourcesGraphObjFormat}
    ${_SourcesGraphObjs}
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

source_group("Header Files/Common" FILES ${_HeadersCommon})
source_group("Header Files/Drawing" FILES ${_HeadersDrawing})
source_group("Header Files/DrawingPageSetup" FILES ${_HeadersDrawingPageSetup})
source_group("Header Files/GraphObjFactories" FILES ${_HeadersGraphObjFactories})
source_group("Header Files/GraphObjFormat" FILES ${_HeadersGraphObjFormat})
source_group("Header Files/GraphObjs" FILES ${_HeadersGraphObjs})
source_group("Source Files/Common" FILES ${_SourcesCommon})
source_group("Source Files/Drawing" FILES ${_SourcesDrawing})
source_group("Source Files/DrawingPageSetup" FILES ${_SourcesDrawingPageSetup})
source_group("Source Files/GraphObjFactories" FILES ${_SourcesGraphObjFactories})
source_group("Source Files/GraphObjFormat" FILES ${_SourcesGraphObjFormat})
source_group("Source Files/GraphObjs" FILES ${_SourcesGraphObjs})
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
