cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSPhysValGUI)

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
# e.g. #include "ZSPhysVal/ZSPhysVal.h"
# For "target_include_directories" the header path without ${PROJECT_NAME} must be
# passed and therefore _HeadersPath will be defined without ${PROJECT_NAME} so that
# this define can be used further one in this CMake list file.
# _SourcePaths will be defined in the same way.
set(_HeadersPath ${CMAKE_CURRENT_SOURCE_DIR}/../../../Include/Libs)
set(_SourcesPath ${CMAKE_CURRENT_SOURCE_DIR}/../../../Source/Libs)
set(_ResourcesPath ${CMAKE_CURRENT_SOURCE_DIR}/../../../Resources/Libs)

list(APPEND _Headers
    ${_HeadersPath}/ZSSys/ZSSysVersion.h
    ${_HeadersPath}/ZSPhysValGUI/ZSPhysUnitsDlg.h
    ${_HeadersPath}/ZSPhysValGUI/ZSPhysUnitsTreeView.h
    ${_HeadersPath}/ZSPhysValGUI/ZSPhysUnitsTreeWdgt.h
    ${_HeadersPath}/ZSPhysValGUI/ZSPhysUnitsWdgt.h
    ${_HeadersPath}/ZSPhysValGUI/ZSPhysTreeEntriesWdgt.h
    ${_HeadersPath}/ZSPhysValGUI/ZSPhysTreeEntryAbstractWdgt.h
    ${_HeadersPath}/ZSPhysValGUI/ZSPhysTreeEntryGrpPhysUnitsModel.h
    ${_HeadersPath}/ZSPhysValGUI/ZSPhysTreeEntryGrpPhysUnitsWdgt.h
    ${_HeadersPath}/ZSPhysValGUI/ZSPhysTreeEntryPhysUnitWdgt.h
    ${_HeadersPath}/ZSPhysValGUI/ZSPhysTreeEntryUnitQuantityWdgt.h
    ${_HeadersPath}/ZSPhysValGUI/ZSPhysTreeEntryUnitRatioWdgt.h
    ${_HeadersPath}/ZSPhysValGUI/ZSPhysTreeEntryTypeUndefinedWdgt.h
    ${_HeadersPath}/ZSPhysValGUI/ZSPhysUnitFctConvertInternalModel.h
    ${_HeadersPath}/ZSPhysValGUI/ZSPhysUnitFctConvertExternalModel.h
    ${_HeadersPath}/ZSPhysValGUI/ZSPhysUnitFctConvertRefValsModel.h
    ${_HeadersPath}/ZSPhysValGUI/ZSPhysValDlgEditPhysVal.h
    ${_HeadersPath}/ZSPhysValGUI/ZSPhysValGUIDllMain.h
    ${_HeadersPath}/ZSPhysValGUI/ZSPhysValWdgtEditPhysVal.h
)

list(APPEND _Sources
    ${_SourcesPath}/ZSPhysValGUI/ZSPhysUnitsDlg.cpp
    ${_SourcesPath}/ZSPhysValGUI/ZSPhysUnitsTreeView.cpp
    ${_SourcesPath}/ZSPhysValGUI/ZSPhysUnitsTreeWdgt.cpp
    ${_SourcesPath}/ZSPhysValGUI/ZSPhysUnitsWdgt.cpp
    ${_SourcesPath}/ZSPhysValGUI/ZSPhysTreeEntriesWdgt.cpp
    ${_SourcesPath}/ZSPhysValGUI/ZSPhysTreeEntryAbstractWdgt.cpp
    ${_SourcesPath}/ZSPhysValGUI/ZSPhysTreeEntryGrpPhysUnitsModel.cpp
    ${_SourcesPath}/ZSPhysValGUI/ZSPhysTreeEntryGrpPhysUnitsWdgt.cpp
    ${_SourcesPath}/ZSPhysValGUI/ZSPhysTreeEntryPhysUnitWdgt.cpp
    ${_SourcesPath}/ZSPhysValGUI/ZSPhysTreeEntryUnitQuantityWdgt.cpp
    ${_SourcesPath}/ZSPhysValGUI/ZSPhysTreeEntryUnitRatioWdgt.cpp
    ${_SourcesPath}/ZSPhysValGUI/ZSPhysTreeEntryTypeUndefinedWdgt.cpp
    ${_SourcesPath}/ZSPhysValGUI/ZSPhysUnitFctConvertInternalModel.cpp
    ${_SourcesPath}/ZSPhysValGUI/ZSPhysUnitFctConvertExternalModel.cpp
    ${_SourcesPath}/ZSPhysValGUI/ZSPhysUnitFctConvertRefValsModel.cpp
    ${_SourcesPath}/ZSPhysValGUI/ZSPhysValDlgEditPhysVal.cpp
    ${_SourcesPath}/ZSPhysValGUI/ZSPhysValGUIDllMain.cpp
    ${_SourcesPath}/ZSPhysValGUI/ZSPhysValWdgtEditPhysVal.cpp
)

list(APPEND _Resources
)

if(WIN32)
    # Add version info as meta information to Dll.
    set(_Resources ${_Resources} ${_ResourcesPath}/ZSSys/ZSSysVersion.rc)
endif()

# Got to create the following target name:
# (ProjectName)Qt$(QT_VERSION_MAJOR)$(QT_LIBINFIX)[d]
# e.g.: "ZSPhysValGUIQt5d", "ZSPhysValGUIQt5Swpd"
set(TargetOutputName ${PROJECT_NAME}Qt${QT_VERSION_MAJOR}${QT_LIBINFIX})
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(TargetOutputName ${TargetOutputName}d)
endif()
message(STATUS "TargetOutputName: ${TargetOutputName}")

add_library(${PROJECT_NAME} SHARED ${_Headers} ${_Sources} ${_Resources})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TargetOutputName})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Libs)

source_group("Header Files" FILES ${_Headers})
source_group("Source Files" FILES ${_Sources})
source_group("Resource Files" FILES ${_Resources})

target_compile_definitions(${PROJECT_NAME} PRIVATE ZSPHYSVALGUIDLL_EXPORTS)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${_HeadersPath}>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)

set(_Libs ZSSys ZSSysGUI ZSPhysVal)

target_link_libraries(${PROJECT_NAME} PUBLIC ZSSys ZSSysGUI ZSPhysVal)

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
