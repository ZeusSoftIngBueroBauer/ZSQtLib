cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSPhysVal)

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
    ${_HeadersPath}/ZSPhysVal/ZSPhysTreeEntryGrpBase.h
    ${_HeadersPath}/ZSPhysVal/ZSPhysTreeEntryGrpPhysUnits.h
    ${_HeadersPath}/ZSPhysVal/ZSPhysTreeEntryGrpScienceField.h
    ${_HeadersPath}/ZSPhysVal/ZSPhysTreeEntryPhysUnit.h
    ${_HeadersPath}/ZSPhysVal/ZSPhysTreeEntryUnitBase.h
    ${_HeadersPath}/ZSPhysVal/ZSPhysTreeEntryUnitQuantity.h
    ${_HeadersPath}/ZSPhysVal/ZSPhysTreeEntryUnitRatio.h
    ${_HeadersPath}/ZSPhysVal/ZSPhysUnitsIdxTree.h
    ${_HeadersPath}/ZSPhysVal/ZSPhysUnit.h
    ${_HeadersPath}/ZSPhysVal/ZSPhysUnitsFctConvert.h
    ${_HeadersPath}/ZSPhysVal/ZSPhysVal.h
    ${_HeadersPath}/ZSPhysVal/ZSPhysValArr.h
    ${_HeadersPath}/ZSPhysVal/ZSPhysValDllMain.h
    ${_HeadersPath}/ZSPhysVal/ZSPhysValExceptions.h
    ${_HeadersPath}/ZSPhysVal/ZSPhysValRes.h
)

list(APPEND _Sources
    ${_SourcesPath}/ZSPhysVal/ZSPhysTreeEntryGrpBase.cpp
    ${_SourcesPath}/ZSPhysVal/ZSPhysTreeEntryGrpPhysUnits.cpp
    ${_SourcesPath}/ZSPhysVal/ZSPhysTreeEntryGrpScienceField.cpp
    ${_SourcesPath}/ZSPhysVal/ZSPhysTreeEntryPhysUnit.cpp
    ${_SourcesPath}/ZSPhysVal/ZSPhysTreeEntryUnitBase.cpp
    ${_SourcesPath}/ZSPhysVal/ZSPhysTreeEntryUnitQuantity.cpp
    ${_SourcesPath}/ZSPhysVal/ZSPhysTreeEntryUnitRatio.cpp
    ${_SourcesPath}/ZSPhysVal/ZSPhysUnitsIdxTree.cpp
    ${_SourcesPath}/ZSPhysVal/ZSPhysUnit.cpp
    ${_SourcesPath}/ZSPhysVal/ZSPhysUnitsFctConvert.cpp
    ${_SourcesPath}/ZSPhysVal/ZSPhysVal.cpp
    ${_SourcesPath}/ZSPhysVal/ZSPhysValArr.cpp
    ${_SourcesPath}/ZSPhysVal/ZSPhysValDllMain.cpp
    ${_SourcesPath}/ZSPhysVal/ZSPhysValExceptions.cpp
    ${_SourcesPath}/ZSPhysVal/ZSPhysValRes.cpp
)

# Got to create the following target name:
# (ProjectName)Qt$(QT_VERSION_MAJOR)$(QT_LIBINFIX)[d]
# e.g.: "ZSPhysValQt5d", "ZSPhysValQt5Swpd"
set(TargetOutputName ${PROJECT_NAME}Qt${QT_VERSION_MAJOR}${QT_LIBINFIX})
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

target_compile_definitions(${PROJECT_NAME} PRIVATE ZSPHYSVALDLL_EXPORTS)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${_HeadersPath}>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)

set(_Libs ZSSys)

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
