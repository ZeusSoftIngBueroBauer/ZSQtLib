cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSQuickControls)

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
    ${_HeadersPath}/${PROJECT_NAME}/ZSQuickControlsDllMain.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSQuickControlsThemeWindowsStyle.h
)

list(APPEND _Sources
    ${_SourcesPath}/${PROJECT_NAME}/ZSQuickControlsDllMain.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSQuickControlsThemeWindowsStyle.cpp
)

list(APPEND _Resources
    ${_QmlPath}/${PROJECT_NAME}/${PROJECT_NAME}Qml.qrc
)

list(APPEND _ImageFiles
)

list(APPEND _ZSStyleWindowsQmlFiles
    ${_QmlPath}/${PROJECT_NAME}/ZSStyleWindows/qmldir
    ${_QmlPath}/${PROJECT_NAME}/ZSStyleWindows/Theme.qml
    ${_QmlPath}/${PROJECT_NAME}/ZSStyleWindows/Button.qml
    ${_QmlPath}/${PROJECT_NAME}/ZSStyleWindows/CheckBox.qml
    ${_QmlPath}/${PROJECT_NAME}/ZSStyleWindows/Switch.qml
    ${_QmlPath}/${PROJECT_NAME}/ZSStyleWindows/ToolButton.qml
)

list(APPEND _QmlFiles
    ${_ZSStyleWindowsQmlFiles}
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
source_group("Qml Files/Style Windows" FILES ${_ZSStyleWindowsQmlFiles})

target_compile_definitions(${PROJECT_NAME} PRIVATE ZSQUICKCONTROLSDLL_EXPORTS)

set(_Libs ZSSysGUI)

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
