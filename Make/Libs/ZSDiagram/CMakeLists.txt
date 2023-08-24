cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSDiagram)

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
    ${_HeadersPath}/${PROJECT_NAME}/ZSDiagObj.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDiagObjAxisLabel.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDiagObjCurve.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDiagObjErrInfo.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDiagObjGrid.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDiagObjHistogram.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDiagObjLabel.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDiagObjLine.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDiagObjMarker.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDiagObjProgressBar.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDiagObjRect.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDiagObjTable.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDiagObjValueProvider.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDiagramAux.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDiagramDllMain.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDiagramFrameStyles.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDiagramImageStyles.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDiagramLabelStyles.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDiagramLineStyles.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDiagramProcData.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDiagramProcPixmap.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDiagramProcWdgt.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDiagramToolTipStyles.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDiagScale.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSDiagTrace.h
)

list(APPEND _Sources
    ${_SourcesPath}/${PROJECT_NAME}/ZSDiagObj.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDiagObjAxisLabel.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDiagObjCurve.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDiagObjErrInfo.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDiagObjGrid.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDiagObjHistogram.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDiagObjLabel.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDiagObjLine.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDiagObjMarker.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDiagObjProgressBar.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDiagObjRect.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDiagObjTable.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDiagObjValueProvider.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDiagramAux.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDiagramDllMain.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDiagramFrameStyles.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDiagramImageStyles.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDiagramLabelStyles.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDiagramLineStyles.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDiagramProcData.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDiagramProcPixmap.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDiagramProcWdgt.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDiagramToolTipStyles.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDiagScale.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSDiagTrace.cpp
)

list(APPEND _Resources
    ${_ResourcesPath}/${PROJECT_NAME}/${PROJECT_NAME}.qrc
)

list(APPEND _Images
    ${_ResourcesPath}/${PROJECT_NAME}/Images/SplitHCursorEditing.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/SplitHCursorFocused.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/SplitVCursorEditing.bmp
    ${_ResourcesPath}/${PROJECT_NAME}/Images/SplitVCursorFocused.bmp
)

if(WIN32)
    # Add version info as meta information to Dll.
    set(_Resources ${_Resources} ${_ResourcesPath}/ZSSys/ZSSysVersion.rc)
endif()

# Got to create the following target name:
# (ProjectName)Qt$(QT_VERSION_MAJOR)$(QT_LIBINFIX)[d]
# e.g.: "ZSDiagramQt5d", "ZSDiagramQt5Swpd"
set(TargetOutputName ${PROJECT_NAME}Qt${QT_VERSION_MAJOR}${QT_LIBINFIX})
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(TargetOutputName ${TargetOutputName}d)
endif()
message(STATUS "TargetOutputName: ${TargetOutputName}")

add_library(${PROJECT_NAME} SHARED ${_Headers} ${_Sources} ${_Resources} ${_Images})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TargetOutputName})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Libs)

source_group("Header Files" FILES ${_Headers})
source_group("Source Files" FILES ${_Sources})
source_group("Resource Files" FILES ${_Resources})
source_group("Image Files" FILES ${_Images})

target_compile_definitions(${PROJECT_NAME} PRIVATE ZSDIAGRAMDLL_EXPORTS)

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
