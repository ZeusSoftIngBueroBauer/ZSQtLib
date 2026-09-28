cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSDrawPluginElectricity)

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
set(_HeadersPath ${CMAKE_CURRENT_SOURCE_DIR}/../../../../Include/Plugins/ZSDraw/Electricity)
set(_SourcesPath ${CMAKE_CURRENT_SOURCE_DIR}/../../../../Source/Plugins/ZSDraw/Electricity)
set(_ResourcesPath ${CMAKE_CURRENT_SOURCE_DIR}/../../../../Resources/Plugins/ZSDraw/Electricity)

list(APPEND _Headers
    ${_HeadersPath}/../ZSDrawPluginInterface.h
    ${_HeadersPath}/ZSDrawPluginElectricity.h
    ${_HeadersPath}/ZSDrawGraphObjElectricity.h
    ${_HeadersPath}/ZSDrawGraphObjElectricityCapacitor.h
    ${_HeadersPath}/ZSDrawGraphObjElectricityDiode.h
    ${_HeadersPath}/ZSDrawGraphObjElectricityInductor.h
    ${_HeadersPath}/ZSDrawGraphObjElectricityResistor.h
    ${_HeadersPath}/ZSDrawGraphObjElectricitySwitch.h
    ${_HeadersPath}/ZSDrawGraphObjElectricityTransistor.h
    ${_HeadersPath}/ZSDrawGraphObjElectricityVoltageSource.h
    ${_HeadersPath}/ZSDrawObjFactoryElectricityCapacitor.h
    ${_HeadersPath}/ZSDrawObjFactoryElectricityDiode.h
    ${_HeadersPath}/ZSDrawObjFactoryElectricityInductor.h
    ${_HeadersPath}/ZSDrawObjFactoryElectricityResistor.h
    ${_HeadersPath}/ZSDrawObjFactoryElectricitySwitch.h
    ${_HeadersPath}/ZSDrawObjFactoryElectricityTransistor.h
    ${_HeadersPath}/ZSDrawObjFactoryElectricityVoltageSource.h
    ${_HeadersPath}/ZSDrawWdgtFormatGraphObjsElectricityCapacitor.h
    ${_HeadersPath}/ZSDrawWdgtFormatGraphObjsElectricityDiode.h
    ${_HeadersPath}/ZSDrawWdgtFormatGraphObjsElectricityInductor.h
    ${_HeadersPath}/ZSDrawWdgtFormatGraphObjsElectricityResistor.h
    ${_HeadersPath}/ZSDrawWdgtFormatGraphObjsElectricitySwitch.h
    ${_HeadersPath}/ZSDrawWdgtFormatGraphObjsElectricityTransistor.h
    ${_HeadersPath}/ZSDrawWdgtFormatGraphObjsElectricityVoltageSource.h
)

list(APPEND _Sources
    ${_SourcesPath}/ZSDrawPluginElectricity.cpp
    ${_SourcesPath}/ZSDrawGraphObjElectricity.cpp
    ${_SourcesPath}/ZSDrawGraphObjElectricityCapacitor.cpp
    ${_SourcesPath}/ZSDrawGraphObjElectricityDiode.cpp
    ${_SourcesPath}/ZSDrawGraphObjElectricityInductor.cpp
    ${_SourcesPath}/ZSDrawGraphObjElectricityResistor.cpp
    ${_SourcesPath}/ZSDrawGraphObjElectricitySwitch.cpp
    ${_SourcesPath}/ZSDrawGraphObjElectricityTransistor.cpp
    ${_SourcesPath}/ZSDrawGraphObjElectricityVoltageSource.cpp
    ${_SourcesPath}/ZSDrawObjFactoryElectricityCapacitor.cpp
    ${_SourcesPath}/ZSDrawObjFactoryElectricityDiode.cpp
    ${_SourcesPath}/ZSDrawObjFactoryElectricityInductor.cpp
    ${_SourcesPath}/ZSDrawObjFactoryElectricityResistor.cpp
    ${_SourcesPath}/ZSDrawObjFactoryElectricitySwitch.cpp
    ${_SourcesPath}/ZSDrawObjFactoryElectricityTransistor.cpp
    ${_SourcesPath}/ZSDrawObjFactoryElectricityVoltageSource.cpp
    ${_SourcesPath}/ZSDrawWdgtFormatGraphObjsElectricityCapacitor.cpp
    ${_SourcesPath}/ZSDrawWdgtFormatGraphObjsElectricityDiode.cpp
    ${_SourcesPath}/ZSDrawWdgtFormatGraphObjsElectricityInductor.cpp
    ${_SourcesPath}/ZSDrawWdgtFormatGraphObjsElectricityResistor.cpp
    ${_SourcesPath}/ZSDrawWdgtFormatGraphObjsElectricitySwitch.cpp
    ${_SourcesPath}/ZSDrawWdgtFormatGraphObjsElectricityTransistor.cpp
    ${_SourcesPath}/ZSDrawWdgtFormatGraphObjsElectricityVoltageSource.cpp
)

list(APPEND _Resources
    ${_ResourcesPath}/ZSDrawPluginElectricity.qrc
)

list(APPEND _Images
    ${_ResourcesPath}/Images/Capacitor16x16.bmp
    ${_ResourcesPath}/Images/Diode16x16.bmp
    ${_ResourcesPath}/Images/Inductor16x10.bmp
    ${_ResourcesPath}/Images/Inductor16x16.bmp
    ${_ResourcesPath}/Images/Resistor16x16.bmp
    ${_ResourcesPath}/Images/Switch16x16.bmp
    ${_ResourcesPath}/Images/Transistor16x16.bmp
    ${_ResourcesPath}/Images/VoltageSource16x16.bmp
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
