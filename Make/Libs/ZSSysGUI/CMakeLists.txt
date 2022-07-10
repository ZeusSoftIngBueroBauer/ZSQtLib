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

add_library(${PROJECT_NAME} SHARED ${_Headers} ${_Sources} ${_Resources})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TargetOutputName})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Libs)

source_group("Header Files" FILES ${_Headers})
source_group("Source Files" FILES ${_Sources})
source_group("Resource Files" FILES ${_Resources})

target_compile_definitions(${PROJECT_NAME} PRIVATE ZSSYSGUIDLL_EXPORTS)

if(QT_VERSION_MAJOR EQUAL 4)
    if(QT_LIB)
        target_link_libraries(${PROJECT_NAME}
            PUBLIC ZSSys)
    else()
        target_link_libraries(${PROJECT_NAME}
            PUBLIC Qt4::QtGui
            PUBLIC ZSSys)
    endif()
elseif(QT_VERSION_MAJOR EQUAL 5)
    target_link_libraries(${PROJECT_NAME}
        PUBLIC Qt5::Gui Qt5::Widgets
        PUBLIC ZSSys)
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

message(STATUS "")
message(STATUS "<- ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "---------------------------------------------------------------------------------------------")
message(STATUS "")
