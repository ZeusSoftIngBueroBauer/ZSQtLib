# Call CMake from build directory (e.g. ./bin/msvc2019_x64) with:
# - for VS-2017: cmake -G "Visual Studio 15 2017" -A x64 -DCMAKE_BUILD_TYPE=Debug ../Make/Libs/ZSSys
# - for VS-2019: cmake -G "Visual Studio 16 2019" -A x64 -DCMAKE_BUILD_TYPE=Debug ../Make/Libs/ZSSys

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSSysGUI)

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
    ../../../Include/Libs/ZSSysGUI/ZSSysDialog.h                        ../../../Source/Libs/ZSSysGUI/ZSSysDialog.cpp
    ../../../Include/Libs/ZSSysGUI/ZSSysErrDlg.h                        ../../../Source/Libs/ZSSysGUI/ZSSysErrDlg.cpp
    ../../../Include/Libs/ZSSysGUI/ZSSysErrLogDlg.h                     ../../../Source/Libs/ZSSysGUI/ZSSysErrLogDlg.cpp
    ../../../Include/Libs/ZSSysGUI/ZSSysErrLogModel.h                   ../../../Source/Libs/ZSSysGUI/ZSSysErrLogModel.cpp
    ../../../Include/Libs/ZSSysGUI/ZSSysErrLogWdgt.h                    ../../../Source/Libs/ZSSysGUI/ZSSysErrLogWdgt.cpp
    ../../../Include/Libs/ZSSysGUI/ZSSysFindTextDlg.h                   ../../../Source/Libs/ZSSysGUI/ZSSysFindTextDlg.cpp
    ../../../Include/Libs/ZSSysGUI/ZSSysGUIApp.h                        ../../../Source/Libs/ZSSysGUI/ZSSysGUIApp.cpp
    ../../../Include/Libs/ZSSysGUI/ZSSysGUIAux.h                        ../../../Source/Libs/ZSSysGUI/ZSSysGUIAux.cpp
    ../../../Include/Libs/ZSSysGUI/ZSSysGUIDllMain.h                    ../../../Source/Libs/ZSSysGUI/ZSSysGUIDllMain.cpp
    ../../../Include/Libs/ZSSysGUI/ZSSysIdxTreeModel.h                  ../../../Source/Libs/ZSSysGUI/ZSSysIdxTreeModel.cpp
    ../../../Include/Libs/ZSSysGUI/ZSSysIdxTreeModelBranchContent.h     ../../../Source/Libs/ZSSysGUI/ZSSysIdxTreeModelBranchContent.cpp
    ../../../Include/Libs/ZSSysGUI/ZSSysIdxTreeModelEntries.h           ../../../Source/Libs/ZSSysGUI/ZSSysIdxTreeModelEntries.cpp
    ../../../Include/Libs/ZSSysGUI/ZSSysIdxTreeTableViewBranchContent.h ../../../Source/Libs/ZSSysGUI/ZSSysIdxTreeTableViewBranchContent.cpp
    ../../../Include/Libs/ZSSysGUI/ZSSysIdxTreeView.h                   ../../../Source/Libs/ZSSysGUI/ZSSysIdxTreeView.cpp
    ../../../Include/Libs/ZSSysGUI/ZSSysIdxTreeWdgt.h                   ../../../Source/Libs/ZSSysGUI/ZSSysIdxTreeWdgt.cpp
    ../../../Include/Libs/ZSSysGUI/ZSSysObjPoolModel.h                  ../../../Source/Libs/ZSSysGUI/ZSSysObjPoolModel.cpp
    ../../../Include/Libs/ZSSysGUI/ZSSysProgressBar.h                   ../../../Source/Libs/ZSSysGUI/ZSSysProgressBar.cpp
    ../../../Include/Libs/ZSSysGUI/ZSSysPushButton.h                    ../../../Source/Libs/ZSSysGUI/ZSSysPushButton.cpp
    ../../../Include/Libs/ZSSysGUI/ZSSysPushButtonCloseWidget.h         ../../../Source/Libs/ZSSysGUI/ZSSysPushButtonCloseWidget.cpp
    ../../../Include/Libs/ZSSysGUI/ZSSysRequestExecTreeDlg.h            ../../../Source/Libs/ZSSysGUI/ZSSysRequestExecTreeDlg.cpp
    ../../../Include/Libs/ZSSysGUI/ZSSysRequestExecTreeModel.h          ../../../Source/Libs/ZSSysGUI/ZSSysRequestExecTreeModel.cpp
    ../../../Include/Libs/ZSSysGUI/ZSSysRequestExecTreeWdgt.h           ../../../Source/Libs/ZSSysGUI/ZSSysRequestExecTreeWdgt.cpp
    ../../../Include/Libs/ZSSysGUI/ZSSysRequestSequencerDlg.h           ../../../Source/Libs/ZSSysGUI/ZSSysRequestSequencerDlg.cpp
    ../../../Include/Libs/ZSSysGUI/ZSSysRequestSequencerModel.h         ../../../Source/Libs/ZSSysGUI/ZSSysRequestSequencerModel.cpp
    ../../../Include/Libs/ZSSysGUI/ZSSysRequestSequencerWdgt.h          ../../../Source/Libs/ZSSysGUI/ZSSysRequestSequencerWdgt.cpp
    ../../../Include/Libs/ZSSysGUI/ZSSysSepLine.h                       ../../../Source/Libs/ZSSysGUI/ZSSysSepLine.cpp
    ../../../Include/Libs/ZSSysGUI/ZSSysTrcAdminObjIdxTreeDlg.h         ../../../Source/Libs/ZSSysGUI/ZSSysTrcAdminObjIdxTreeDlg.cpp
    ../../../Include/Libs/ZSSysGUI/ZSSysTrcAdminObjIdxTreeModel.h       ../../../Source/Libs/ZSSysGUI/ZSSysTrcAdminObjIdxTreeModel.cpp
    ../../../Include/Libs/ZSSysGUI/ZSSysTrcAdminObjIdxTreeView.h        ../../../Source/Libs/ZSSysGUI/ZSSysTrcAdminObjIdxTreeView.cpp
    ../../../Include/Libs/ZSSysGUI/ZSSysTrcAdminObjIdxTreeWdgt.h        ../../../Source/Libs/ZSSysGUI/ZSSysTrcAdminObjIdxTreeWdgt.cpp
    ../../../Images/Libs/ZSSys/ZSSys.qrc
)

# Got to create the following target name:
# (ProjectName)Qt$(QT_VERSION_MAJOR)_$(Compiler)_$(Platform)[_d]
# e.g.: "ZSSysGUIQt5_msvc2019_x64_d"

set(TargetOutputName ${PROJECT_NAME}Qt${QT_VERSION_MAJOR}_${Compiler}_${Platform})

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(TargetOutputName ${TargetOutputName}_d)
endif()

message(STATUS "TargetOutputName: ${TargetOutputName}")

add_library(${PROJECT_NAME} SHARED ${_Sources})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TargetOutputName})

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
