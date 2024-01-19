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
    ${_HeadersPath}/${PROJECT_NAME}/ZSPhysTreeEntryGrpBase.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSPhysTreeEntryGrpPhysUnits.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSPhysTreeEntryGrpScienceField.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSPhysTreeEntryPhysUnit.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSPhysTreeEntryUnitBase.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSPhysTreeEntryUnitQuantity.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSPhysTreeEntryUnitRatio.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSPhysUnitsIdxTree.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSPhysUnit.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSPhysUnitsFctConvert.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSPhysVal.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSPhysValArr.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSPhysValDllMain.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSPhysValExceptions.h
    ${_HeadersPath}/${PROJECT_NAME}/ZSPhysValRes.h
)

list(APPEND _Sources
    ${_SourcesPath}/${PROJECT_NAME}/ZSPhysTreeEntryGrpBase.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSPhysTreeEntryGrpPhysUnits.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSPhysTreeEntryGrpScienceField.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSPhysTreeEntryPhysUnit.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSPhysTreeEntryUnitBase.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSPhysTreeEntryUnitQuantity.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSPhysTreeEntryUnitRatio.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSPhysUnitsIdxTree.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSPhysUnit.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSPhysUnitsFctConvert.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSPhysVal.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSPhysValArr.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSPhysValDllMain.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSPhysValExceptions.cpp
    ${_SourcesPath}/${PROJECT_NAME}/ZSPhysValRes.cpp
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
