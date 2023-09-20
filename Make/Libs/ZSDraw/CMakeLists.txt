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
    ${_HeadersPath}/${PROJECT_NAME}/Common/ZSDrawingSize.h
    ${_HeadersPath}/${PROJECT_NAME}/Common/ZSDrawPhysValLine.h
    ${_HeadersPath}/${PROJECT_NAME}/Common/ZSDrawPhysValPoint.h
    ${_HeadersPath}/${PROJECT_NAME}/Common/ZSDrawPhysValRect.h
    ${_HeadersPath}/${PROJECT_NAME}/Common/ZSDrawPhysValSize.h
    ${_HeadersPath}/${PROJECT_NAME}/Common/ZSDrawSettings.h
    ${_HeadersPath}/${PROJECT_NAME}/Common/ZSDrawUnits.h
)

list(APPEND _HeadersDrawing
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/ZSDrawGridSettings.h
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/ZSDrawingScene.h
)

list(APPEND _HeadersDrawingGraphObjs
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/GraphObjs/ZSDrawGraphObj.h
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/GraphObjs/ZSDrawGraphObjConnectionLine.h
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/GraphObjs/ZSDrawGraphObjConnectionPoint.h
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/GraphObjs/ZSDrawGraphObjEllipse.h
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/GraphObjs/ZSDrawGraphObjEventFct.h
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/GraphObjs/ZSDrawGraphObjGroup.h
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/GraphObjs/ZSDrawGraphObjImage.h
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/GraphObjs/ZSDrawGraphObjLabel.h
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/GraphObjs/ZSDrawGraphObjLine.h
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/GraphObjs/ZSDrawGraphObjPoint.h
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/GraphObjs/ZSDrawGraphObjPolygon.h
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/GraphObjs/ZSDrawGraphObjPolyline.h
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/GraphObjs/ZSDrawGraphObjRect.h
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/GraphObjs/ZSDrawGraphObjSelectionPoint.h
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/GraphObjs/ZSDrawGraphObjText.h
)

list(APPEND _HeadersDrawingObjFactories
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/ObjFactories/ZSDrawObjFactory.h
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/ObjFactories/ZSDrawObjFactoryConnectionLine.h
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/ObjFactories/ZSDrawObjFactoryConnectionPoint.h
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/ObjFactories/ZSDrawObjFactoryEllipse.h
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/ObjFactories/ZSDrawObjFactoryGroup.h
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/ObjFactories/ZSDrawObjFactoryImage.h
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/ObjFactories/ZSDrawObjFactoryLine.h
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/ObjFactories/ZSDrawObjFactoryPoint.h
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/ObjFactories/ZSDrawObjFactoryPolygon.h
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/ObjFactories/ZSDrawObjFactoryPolyline.h
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/ObjFactories/ZSDrawObjFactoryRect.h
    ${_HeadersPath}/${PROJECT_NAME}/Drawing/ObjFactories/ZSDrawObjFactoryText.h
)

list(APPEND _HeadersWidgetsDrawing
    ${_HeadersPath}/${PROJECT_NAME}/Widgets/Drawing/ZSDrawDlgDrawingViewSetup.h
    ${_HeadersPath}/${PROJECT_NAME}/Widgets/Drawing/ZSDrawingView.h
    ${_HeadersPath}/${PROJECT_NAME}/Widgets/Drawing/ZSDrawingViewPropertiesWdgt.h
    ${_HeadersPath}/${PROJECT_NAME}/Widgets/Drawing/ZSDrawingWidget.h
)

list(APPEND _HeadersWidgetsGraphObjFormat
    ${_HeadersPath}/${PROJECT_NAME}/Widgets/GraphObjFormat/ZSDrawDlgFormatGraphObjs.h
    ${_HeadersPath}/${PROJECT_NAME}/Widgets/GraphObjFormat/ZSDrawWdgtFormatGraphObjs.h
    ${_HeadersPath}/${PROJECT_NAME}/Widgets/GraphObjFormat/ZSDrawWdgtFormatGraphObjsFillStyle.h
    ${_HeadersPath}/${PROJECT_NAME}/Widgets/GraphObjFormat/ZSDrawWdgtFormatGraphObjsGeometry.h
    ${_HeadersPath}/${PROJECT_NAME}/Widgets/GraphObjFormat/ZSDrawWdgtFormatGraphObjsLineStyle.h
    ${_HeadersPath}/${PROJECT_NAME}/Widgets/GraphObjFormat/ZSDrawWdgtFormatGraphObjsLabels.h
    ${_HeadersPath}/${PROJECT_NAME}/Widgets/GraphObjFormat/ZSDrawWdgtFormatGraphObjsLabelVisibilities.h
    ${_HeadersPath}/${PROJECT_NAME}/Widgets/GraphObjFormat/ZSDrawWdgtFormatGraphObjsText.h
    ${_HeadersPath}/${PROJECT_NAME}/Widgets/GraphObjFormat/ZSDrawWdgtFormatGraphObjsTextStyle.h
)

list(APPEND _HeadersWidgetsGraphObjs
    ${_HeadersPath}/${PROJECT_NAME}/Widgets/GraphObjs/ZSDrawGraphObjLineGeometryPropertiesWdgt.h
    ${_HeadersPath}/${PROJECT_NAME}/Widgets/GraphObjs/ZSDrawGraphObjLinePropertiesWdgt.h
    ${_HeadersPath}/${PROJECT_NAME}/Widgets/GraphObjs/ZSDrawGraphObjPropertiesAbstractWdgt.h
    ${_HeadersPath}/${PROJECT_NAME}/Widgets/GraphObjs/ZSDrawGraphObjPropertiesLabelsWdgt.h
    ${_HeadersPath}/${PROJECT_NAME}/Widgets/GraphObjs/ZSDrawGraphObjsPropertiesWdgtStack.h
)

list(APPEND _HeadersWidgetsObjFactories
    ${_HeadersPath}/${PROJECT_NAME}/Widgets/ObjFactories/ZSDrawObjFactoriesModel.h
)

list(APPEND _HeadersWidgetsTreeView
    ${_HeadersPath}/${PROJECT_NAME}/Widgets/TreeView/ZSDrawGraphicsItemsModel.h
    ${_HeadersPath}/${PROJECT_NAME}/Widgets/TreeView/ZSDrawGraphObjModelEntry.h
    ${_HeadersPath}/${PROJECT_NAME}/Widgets/TreeView/ZSDrawGraphObjsTreeModel.h
    ${_HeadersPath}/${PROJECT_NAME}/Widgets/TreeView/ZSDrawGraphObjsTreeView.h
    ${_HeadersPath}/${PROJECT_NAME}/Widgets/TreeView/ZSDrawGraphObjsTreeWdgt.h
    ${_HeadersPath}/${PROJECT_NAME}/Widgets/TreeView/ZSDrawGraphObjsWdgt.h
)

list(APPEND _Headers
    ${_HeadersCommon}
    ${_HeadersDrawing}
    ${_HeadersDrawingGraphObjs}
    ${_HeadersDrawingObjFactories}
    ${_HeadersWidgetsDrawing}
    ${_HeadersWidgetsGraphObjFormat}
    ${_HeadersWidgetsGraphObjs}
    ${_HeadersWidgetsObjFactories}
    ${_HeadersWidgetsTreeView}
)

list(APPEND _SourcesCommon
    ${_SourcesPath}/${PROJECT_NAME}/Common/ZSDrawAux.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Common/ZSDrawCommon.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Common/ZSDrawDllMain.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Common/ZSDrawingSize.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Common/ZSDrawPhysValLine.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Common/ZSDrawPhysValPoint.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Common/ZSDrawPhysValRect.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Common/ZSDrawPhysValSize.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Common/ZSDrawSettings.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Common/ZSDrawUnits.cpp
)

list(APPEND _SourcesDrawingGraphObjs
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/GraphObjs/ZSDrawGraphObj.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/GraphObjs/ZSDrawGraphObjConnectionLine.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/GraphObjs/ZSDrawGraphObjConnectionPoint.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/GraphObjs/ZSDrawGraphObjEllipse.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/GraphObjs/ZSDrawGraphObjGroup.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/GraphObjs/ZSDrawGraphObjImage.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/GraphObjs/ZSDrawGraphObjLabel.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/GraphObjs/ZSDrawGraphObjLine.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/GraphObjs/ZSDrawGraphObjPoint.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/GraphObjs/ZSDrawGraphObjPolygon.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/GraphObjs/ZSDrawGraphObjPolyline.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/GraphObjs/ZSDrawGraphObjRect.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/GraphObjs/ZSDrawGraphObjSelectionPoint.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/GraphObjs/ZSDrawGraphObjText.cpp
)

list(APPEND _SourcesDrawingObjFactories
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/ObjFactories/ZSDrawObjFactory.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/ObjFactories/ZSDrawObjFactoryConnectionLine.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/ObjFactories/ZSDrawObjFactoryConnectionPoint.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/ObjFactories/ZSDrawObjFactoryEllipse.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/ObjFactories/ZSDrawObjFactoryGroup.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/ObjFactories/ZSDrawObjFactoryImage.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/ObjFactories/ZSDrawObjFactoryLine.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/ObjFactories/ZSDrawObjFactoryPoint.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/ObjFactories/ZSDrawObjFactoryPolygon.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/ObjFactories/ZSDrawObjFactoryPolyline.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/ObjFactories/ZSDrawObjFactoryRect.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/ObjFactories/ZSDrawObjFactoryText.cpp
)

list(APPEND _SourcesDrawing
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/ZSDrawGridSettings.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Drawing/ZSDrawingScene.cpp
)

list(APPEND _SourcesWidgetsDrawing
    ${_SourcesPath}/${PROJECT_NAME}/Widgets/Drawing/ZSDrawDlgDrawingViewSetup.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Widgets/Drawing/ZSDrawingView.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Widgets/Drawing/ZSDrawingViewPropertiesWdgt.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Widgets/Drawing/ZSDrawingWidget.cpp
)

list(APPEND _SourcesWidgetsGraphObjFormat
    ${_SourcesPath}/${PROJECT_NAME}/Widgets/GraphObjFormat/ZSDrawDlgFormatGraphObjs.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Widgets/GraphObjFormat/ZSDrawWdgtFormatGraphObjs.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Widgets/GraphObjFormat/ZSDrawWdgtFormatGraphObjsFillStyle.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Widgets/GraphObjFormat/ZSDrawWdgtFormatGraphObjsGeometry.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Widgets/GraphObjFormat/ZSDrawWdgtFormatGraphObjsLineStyle.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Widgets/GraphObjFormat/ZSDrawWdgtFormatGraphObjsLabels.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Widgets/GraphObjFormat/ZSDrawWdgtFormatGraphObjsLabelVisibilities.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Widgets/GraphObjFormat/ZSDrawWdgtFormatGraphObjsText.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Widgets/GraphObjFormat/ZSDrawWdgtFormatGraphObjsTextStyle.cpp
)

list(APPEND _SourcesWidgetsGraphObjs
    ${_SourcesPath}/${PROJECT_NAME}/Widgets/GraphObjs/ZSDrawGraphObjLineGeometryPropertiesWdgt.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Widgets/GraphObjs/ZSDrawGraphObjLinePropertiesWdgt.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Widgets/GraphObjs/ZSDrawGraphObjPropertiesAbstractWdgt.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Widgets/GraphObjs/ZSDrawGraphObjPropertiesLabelsWdgt.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Widgets/GraphObjs/ZSDrawGraphObjsPropertiesWdgtStack.cpp
)

list(APPEND _SourcesWidgetsObjFactories
    ${_SourcesPath}/${PROJECT_NAME}/Widgets/ObjFactories/ZSDrawObjFactoriesModel.cpp
)

list(APPEND _SourcesWidgetsTreeView
    ${_SourcesPath}/${PROJECT_NAME}/Widgets/TreeView/ZSDrawGraphicsItemsModel.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Widgets/TreeView/ZSDrawGraphObjModelEntry.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Widgets/TreeView/ZSDrawGraphObjsTreeModel.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Widgets/TreeView/ZSDrawGraphObjsTreeView.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Widgets/TreeView/ZSDrawGraphObjsTreeWdgt.cpp
    ${_SourcesPath}/${PROJECT_NAME}/Widgets/TreeView/ZSDrawGraphObjsWdgt.cpp
)

list(APPEND _Sources
    ${_SourcesCommon}
    ${_SourcesDrawing}
    ${_SourcesDrawingGraphObjs}
    ${_SourcesDrawingObjFactories}
    ${_SourcesWidgetsDrawing}
    ${_SourcesWidgetsGraphObjFormat}
    ${_SourcesWidgetsGraphObjs}
    ${_SourcesWidgetsObjFactories}
    ${_SourcesWidgetsTreeView}
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
    ${_ResourcesPath}/${PROJECT_NAME}/Images/DrawToolGeometry16x16.png
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
    ${_ResourcesPath}/${PROJECT_NAME}/Images/Key16x16.png
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
source_group("Header Files/Drawing/GraphObjs" FILES ${_HeadersDrawingGraphObjs})
source_group("Header Files/Drawing/ObjFactories" FILES ${_HeadersDrawingObjFactories})
source_group("Header Files/Drawing" FILES ${_HeadersDrawing})
source_group("Header Files/Widgets/Drawing" FILES ${_HeadersWidgetsDrawing})
source_group("Header Files/Widgets/GraphObjFormat" FILES ${_HeadersWidgetsGraphObjFormat})
source_group("Header Files/Widgets/GraphObjs" FILES ${_HeadersWidgetsGraphObjs})
source_group("Header Files/Widgets/ObjFactories" FILES ${_HeadersWidgetsObjFactories})
source_group("Header Files/Widgets/TreeView" FILES ${_HeadersWidgetsTreeView})
source_group("Source Files/Common" FILES ${_SourcesCommon})
source_group("Source Files/Drawing/GraphObjs" FILES ${_SourcesDrawingGraphObjs})
source_group("Source Files/Drawing/ObjFactories" FILES ${_SourcesDrawingObjFactories})
source_group("Source Files/Drawing" FILES ${_SourcesDrawing})
source_group("Source Files/Widgets/Drawing" FILES ${_SourcesWidgetsDrawing})
source_group("Source Files/Widgets/GraphObjFormat" FILES ${_SourcesWidgetsGraphObjFormat})
source_group("Source Files/Widgets/GraphObjs" FILES ${_SourcesWidgetsGraphObjs})
source_group("Source Files/Widgets/ObjFactories" FILES ${_SourcesWidgetsObjFactories})
source_group("Source Files/Widgets/TreeView" FILES ${_SourcesWidgetsTreeView})
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
