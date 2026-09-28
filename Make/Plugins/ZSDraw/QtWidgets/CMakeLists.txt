cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSDrawPluginQtWidgets)

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
set(_HeadersPath ${CMAKE_CURRENT_SOURCE_DIR}/../../../../Include/Plugins/ZSDraw/QtWidgets)
set(_SourcesPath ${CMAKE_CURRENT_SOURCE_DIR}/../../../../Source/Plugins/ZSDraw/QtWidgets)
set(_ResourcesPath ${CMAKE_CURRENT_SOURCE_DIR}/../../../../Resources/Plugins/ZSDraw/QtWidgets)

list(APPEND _Headers
    ${_HeadersPath}/../ZSDrawPluginInterface.h
    ${_HeadersPath}/ZSDrawPluginQtWidgets.h
    ${_HeadersPath}/ZSDrawGraphObjWdgt.h
    ${_HeadersPath}/ZSDrawGraphObjWdgtCheckBox.h
    ${_HeadersPath}/ZSDrawGraphObjWdgtComboBox.h
    ${_HeadersPath}/ZSDrawGraphObjWdgtGroupBox.h
    ${_HeadersPath}/ZSDrawGraphObjWdgtLabel.h
    ${_HeadersPath}/ZSDrawGraphObjWdgtLineEdit.h
    ${_HeadersPath}/ZSDrawGraphObjWdgtPushButton.h
    ${_HeadersPath}/ZSDrawObjFactoryWdgtCheckBox.h
    ${_HeadersPath}/ZSDrawObjFactoryWdgtComboBox.h
    ${_HeadersPath}/ZSDrawObjFactoryWdgtGroupBox.h
    ${_HeadersPath}/ZSDrawObjFactoryWdgtLabel.h
    ${_HeadersPath}/ZSDrawObjFactoryWdgtLineEdit.h
    ${_HeadersPath}/ZSDrawObjFactoryWdgtPushButton.h
)

list(APPEND _Sources
    ${_SourcesPath}/ZSDrawPluginQtWidgets.cpp
    ${_SourcesPath}/ZSDrawGraphObjWdgt.cpp
    ${_SourcesPath}/ZSDrawGraphObjWdgtCheckBox.cpp
    ${_SourcesPath}/ZSDrawGraphObjWdgtComboBox.cpp
    ${_SourcesPath}/ZSDrawGraphObjWdgtGroupBox.cpp
    ${_SourcesPath}/ZSDrawGraphObjWdgtLabel.cpp
    ${_SourcesPath}/ZSDrawGraphObjWdgtLineEdit.cpp
    ${_SourcesPath}/ZSDrawGraphObjWdgtPushButton.cpp
    ${_SourcesPath}/ZSDrawObjFactoryWdgtCheckBox.cpp
    ${_SourcesPath}/ZSDrawObjFactoryWdgtComboBox.cpp
    ${_SourcesPath}/ZSDrawObjFactoryWdgtGroupBox.cpp
    ${_SourcesPath}/ZSDrawObjFactoryWdgtLabel.cpp
    ${_SourcesPath}/ZSDrawObjFactoryWdgtLineEdit.cpp
    ${_SourcesPath}/ZSDrawObjFactoryWdgtPushButton.cpp
)

list(APPEND _Resources
    ${_ResourcesPath}/ZSDrawPluginQtWidgets.qrc
)

list(APPEND _Images
    ${_ResourcesPath}/Images/CheckBox16x16.bmp
    ${_ResourcesPath}/Images/ComboBox16x16.bmp
    ${_ResourcesPath}/Images/GroupBox16x16.bmp
    ${_ResourcesPath}/Images/Label16x16.bmp
    ${_ResourcesPath}/Images/LineEdit16x16.bmp
    ${_ResourcesPath}/Images/PushButton16x16.bmp
)

if(WIN32)
    # Add version info as meta information to Dll.
    set(_Resources ${_Resources} ${_ResourcesPath}/../../../Libs/ZSSys/ZSSysVersion.rc)
endif()

# Got to create the following target name:
# (ProjectName)Qt$(QT_VERSION_MAJOR)$(QT_LIBINFIX)[d]
# e.g.: "ZSDrawPluginElectricityQt5d"
set(TargetOutputName ${PROJECT_NAME}Qt${QT_VERSION_MAJOR}${QT_LIBINFIX})
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(TargetOutputName ${TargetOutputName}d)
endif()
message(STATUS "TargetOutputName:         ${TargetOutputName}")

add_library(${PROJECT_NAME} SHARED ${_Headers} ${_Sources} ${_Resources} ${_Images})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TargetOutputName})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Plugins/ZSDraw)

source_group("Header Files" FILES ${_Headers})
source_group("Source Files" FILES ${_Sources})
source_group("Resource Files" FILES ${_Resources})
source_group("Image Files" FILES ${_Images})

target_include_directories(${PROJECT_NAME}
    PRIVATE ${_HeadersPath}/..
    PRIVATE ${_HeadersPath})

set(_Libs ZSIpcTraceGUI ZSPhysValGUI ZSDraw)

target_link_libraries(${PROJECT_NAME} ${_Libs})

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME} EXPORT ${CMAKE_PROJECT_NAME}Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION plugins/zsdraw)

message(STATUS "")
message(STATUS "<- ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "---------------------------------------------------------------------------------------------")
message(STATUS "")
