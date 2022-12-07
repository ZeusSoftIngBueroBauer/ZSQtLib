cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSSysGUI)

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
set(_QmlPath ${CMAKE_CURRENT_SOURCE_DIR}/../../../Qml/Libs)

list(APPEND _Headers
    ${_HeadersPath}/ZSSys/ZSSysVersion.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysDialog.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysEditEnumValueDlg.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysEditIntValueDlg.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysEditStringValueDlg.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysErrDlg.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysErrLogDlg.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysErrLogModel.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysErrLogWdgt.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysFindTextDlg.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysGUIApp.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysGUIAux.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysGUIDllMain.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysIdxTreeModel.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysIdxTreeModelBranchContent.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysIdxTreeModelEntry.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysIdxTreeTableViewBranchContent.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysIdxTreeView.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysIdxTreeWdgt.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysLoggerIdxTreeDlg.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysLoggerIdxTreeModel.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysLoggerIdxTreeView.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysLoggerIdxTreeWdgt.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysProgressBar.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysPushButton.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysPushButtonCloseWidget.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysRequestExecTreeDlg.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysRequestExecTreeModel.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysRequestExecTreeWdgt.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysRequestSequencerDlg.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysRequestSequencerModel.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysRequestSequencerWdgt.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysSepLine.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysTextEdit.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysTrcAdminObjIdxTreeDlg.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysTrcAdminObjIdxTreeModel.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysTrcAdminObjIdxTreeView.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSSysTrcAdminObjIdxTreeWdgt.h
)

list(APPEND _Sources
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysDialog.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysEditEnumValueDlg.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysEditIntValueDlg.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysEditStringValueDlg.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysErrDlg.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysErrLogDlg.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysErrLogModel.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysErrLogWdgt.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysFindTextDlg.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysGUIApp.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysGUIAux.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysGUIDllMain.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysIdxTreeModel.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysIdxTreeModelBranchContent.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysIdxTreeModelEntry.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysIdxTreeTableViewBranchContent.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysIdxTreeView.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysIdxTreeWdgt.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysLoggerIdxTreeDlg.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysLoggerIdxTreeModel.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysLoggerIdxTreeView.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysLoggerIdxTreeWdgt.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysProgressBar.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysPushButton.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysPushButtonCloseWidget.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysRequestExecTreeDlg.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysRequestExecTreeModel.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysRequestExecTreeWdgt.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysRequestSequencerDlg.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysRequestSequencerModel.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysRequestSequencerWdgt.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysSepLine.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysTextEdit.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysTrcAdminObjIdxTreeDlg.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysTrcAdminObjIdxTreeModel.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysTrcAdminObjIdxTreeView.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSSysTrcAdminObjIdxTreeWdgt.cpp
)

list(APPEND _Resources
    ${_ResourcesPath}/${PROJECT_NAME}/${PROJECT_NAME}.qrc
    ${_QmlPath}/${PROJECT_NAME}/${PROJECT_NAME}Qml.qrc
)

list(APPEND _AppImages
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ModeEdit16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ModeEdit24x24.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ModeEdit32x32.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ModeSimulation16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ModeSimulation24x24.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ModeSimulation32x32.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/RSIcon16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/RSIcon32x32.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ZeusSoft_16x16_256clr.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ZeusSoft_32x32_256clr.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ZeusSoft_48x48_256clr.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ZeusSoft_64x64_256clr.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ZeusSoft_256x256_256clr.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ZeusSoft_280x170_256clr.png
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ZeusWithMountain_1100x204_256clr.gif
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ZeusWithMountain_1100x204_256clr.jpg
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ZeusWithMountain_1100x204_256clr.png
    ${_ResourcesPath}/${PROJECT_NAME}/Images/Thermometer.png
    ${_ResourcesPath}/${PROJECT_NAME}/Images/Thermometer16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/Thermometer24x24.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/Thermometer32x32.bmp
)

list(APPEND _ButtonImages
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonAdd16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonAdd24x24.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonClose16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonClose24x24.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonCmdPause16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonCmdStart16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonCmdStepForward16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonCmdStop16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonCmdStepBackward16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonDelete16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonDelete24x24.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonFileNew16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonFileOpen16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonFileSave16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonFileSaveAs16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonMoveUp16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonMoveUp24x24.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonMoveDown16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonMoveDown24x24.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonMoveRight16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonMoveRight24x24.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonMoveLeft16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonMoveLeft24x24.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonRedo16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonRedo24x24.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonTableView16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonUndo16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonUndo24x24.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonZoomIn16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonZoomIn24x24.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonZoomOut16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ButtonZoomOut24x24.bmp
)

list(APPEND _CnctStateImages
    ${_ResourcesPath}/${PROJECT_NAME}/Images/LedCircleBlue.png
    ${_ResourcesPath}/${PROJECT_NAME}/Images/LedCircleGreen.png
    ${_ResourcesPath}/${PROJECT_NAME}/Images/LedCircleGrey.png
    ${_ResourcesPath}/${PROJECT_NAME}/Images/LedCircleRed.png
    ${_ResourcesPath}/${PROJECT_NAME}/Images/LedCircleYellow.png
)

list(APPEND _MenuImages
    ${_ResourcesPath}/${PROJECT_NAME}/Images/MenuFileAdd16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/MenuFileEdit16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/MenuFileNew16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/MenuFileOpen16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/MenuFileSave16x16.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/MenuFileSaveAs16x16.bmp
)

list(APPEND _TreeViewImages
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewBranchEntry.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewLeaveEntry.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewRootEntry.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewCollapseAll.png
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewExpandAll.png
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewResizeToContents.png
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewSortOrderConfig.png
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewSortOrderAscending.png
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewViewModeNavPanelOnly.png
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewViewModeNavPanelAndLeaveContent.png
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewViewModeNavPanelAndBranchContent.png
)

list(APPEND _TreeViewFileExplorerImages
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewFileExplorerBranchEntryNormalOff.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewFileExplorerBranchEntryNormalOn.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewFileExplorerBranchEntryDisabledOff.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewFileExplorerBranchEntryDisabledOn.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewFileExplorerBranchEntryActiveOff.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewFileExplorerBranchEntryActiveOn.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewFileExplorerBranchEntrySelectedOff.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewFileExplorerBranchEntrySelectedOn.bmp
)

list(APPEND _TreeViewRequestsImages
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewRequestsBranchEntryNormalOff.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewRequestsBranchEntrySelectedOff.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewRequestsLeaveEntryNormalOff.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewRequestsLeaveEntrySelectedOff.bmp
)

list(APPEND _TreeViewTrcAdminObjsImages
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewTrcAdminObjsBranchEntryNormalOff.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewTrcAdminObjsBranchEntrySelectedOff.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewTrcAdminObjsLeaveEntryNormalOff.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewTrcAdminObjsLeaveEntrySelectedOff.bmp
)

list(APPEND _TreeViewLoggersImages
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewLoggersBranchEntryNormalOff.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewLoggersBranchEntrySelectedOff.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewLoggersLeaveEntryNormalOff.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/TreeViewLoggersLeaveEntrySelectedOff.bmp
)

list(APPEND _ResultImages
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ResultSeverityCritical.png
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ResultSeverityError.png
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ResultSeverityInfo.png
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ResultSeveritySuccess.png
    ${_ResourcesPath}/${PROJECT_NAME}/Images/ResultSeverityWarning.png
)

list(APPEND _ImageFiles
    ${_AppImages}
    ${_ButtonImages}
    ${_CnctStateImages}
    ${_MenuImages}
    ${_TreeViewImages}
    ${_TreeViewFileExplorerImages}
    ${_TreeViewRequestsImages}
    ${_TreeViewTrcAdminObjsImages}
    ${_TreeViewLoggersImages}
    ${_ResultImages}
)

list(APPEND _QmlFiles
    ${_QmlPath}/${PROJECT_NAME}/qmldir
    ${_QmlPath}/${PROJECT_NAME}/ZSSysErrLogDlg.qml
    ${_QmlPath}/${PROJECT_NAME}/ZSSysErrLogIcon.qml
    ${_QmlPath}/${PROJECT_NAME}/ZSSysErrLogWdgt.qml
    ${_QmlPath}/${PROJECT_NAME}/ZSSysIdxTreeWdgt.qml
    ${_QmlPath}/${PROJECT_NAME}/ZSSysPushButton.qml
)

if(WIN32)
    # Add version info as meta information to Dll.
    set(_Resources ${_Resources} ${_ResourcesPath}/ZSSys/ZSSysVersion.rc)
endif()

# Got to create the following target name:
# (ProjectName)Qt$(QT_VERSION_MAJOR)[d]
# e.g.: "ZSSysGUIQt5d"
set(TargetOutputName ${PROJECT_NAME}Qt${QT_VERSION_MAJOR})
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(TargetOutputName ${TargetOutputName}d)
endif()
message(STATUS "TargetOutputName:         ${TargetOutputName}")

add_library(${PROJECT_NAME} SHARED ${_Headers} ${_Sources} ${_Resources} ${_QmlFiles} ${_ImageFiles})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TargetOutputName})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Libs)

source_group("Header Files" FILES ${_Headers})
source_group("Source Files" FILES ${_Sources})
source_group("Resource Files" FILES ${_Resources})
source_group("Image Files/App" FILES ${_AppImages})
source_group("Image Files/Button" FILES ${_ButtonImages})
source_group("Image Files/CnctState" FILES ${_CnctStateImages})
source_group("Image Files/Menu" FILES ${_MenuImages})
source_group("Image Files/TreeView/Common" FILES ${_TreeViewImages})
source_group("Image Files/TreeView/FileExplorer" FILES ${_TreeViewFileExplorerImages})
source_group("Image Files/TreeView/Requests" FILES ${_TreeViewRequestsImages})
source_group("Image Files/TreeView/TrcAdminObjs" FILES ${_TreeViewTrcAdminObjsImages})
source_group("Image Files/TreeView/Loggers" FILES ${_TreeViewLoggersImages})
source_group("Image Files/Result" FILES ${_ResultImages})
source_group("Qml Files" FILES ${_QmlFiles})

target_compile_definitions(${PROJECT_NAME} PRIVATE ZSSYSGUIDLL_EXPORTS)

set(_Libs Qt5::Gui Qt5::Widgets Qt5::Quick Qt5::QuickControls2 ZSSys)

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
