cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSAppTestPhysVal)

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
set(_HeadersPath ${CMAKE_CURRENT_SOURCE_DIR}/../../../../Include/Apps/Test)
set(_SourcesPath ${CMAKE_CURRENT_SOURCE_DIR}/../../../../Source/Apps/Test)
set(_ResourcesPath ${CMAKE_CURRENT_SOURCE_DIR}/../../../../Resources/Apps/Test)

list(APPEND _HeadersCommon
    ${_HeadersPath}/../../Libs/ZSSys/ZSSysVersion.h
    ${_HeadersPath}/ZSPhysVal/App.h
    ${_HeadersPath}/ZSPhysVal/MainWindow.h
    ${_HeadersPath}/ZSPhysVal/WidgetCentral.h
    ${_HeadersPath}/ZSPhysVal/Test.h
)

list(APPEND _HeadersUnits
    ${_HeadersPath}/ZSPhysVal/Units/Units.h
)

list(APPEND _HeadersUnitsElectricity
    ${_HeadersPath}/ZSPhysVal/Units/Electricity/Electricity.h
    ${_HeadersPath}/ZSPhysVal/Units/Electricity/PhysSizeCurrent.h
    ${_HeadersPath}/ZSPhysVal/Units/Electricity/PhysSizePower.h
    ${_HeadersPath}/ZSPhysVal/Units/Electricity/PhysSizeResistance.h
    ${_HeadersPath}/ZSPhysVal/Units/Electricity/PhysSizeVoltage.h
)

list(APPEND _HeadersUnitsGeometry
    ${_HeadersPath}/ZSPhysVal/Units/Geometry/Geometry.h
    ${_HeadersPath}/ZSPhysVal/Units/Geometry/PhysSizeAngle.h
    ${_HeadersPath}/ZSPhysVal/Units/Geometry/PhysSizeGraphDevice.h
    ${_HeadersPath}/ZSPhysVal/Units/Geometry/PhysSizeLength.h
)

list(APPEND _HeadersUnitsKinematics
    ${_HeadersPath}/ZSPhysVal/Units/Kinematics/Kinematics.h
    ${_HeadersPath}/ZSPhysVal/Units/Kinematics/PhysSizeBitRate.h
    ${_HeadersPath}/ZSPhysVal/Units/Kinematics/PhysSizeFrequency.h
    ${_HeadersPath}/ZSPhysVal/Units/Kinematics/PhysSizeSamples.h
    ${_HeadersPath}/ZSPhysVal/Units/Kinematics/PhysSizeTime.h
    ${_HeadersPath}/ZSPhysVal/Units/Kinematics/PhysSizeVelocity.h
)

list(APPEND _HeadersUnitsRatios
    ${_HeadersPath}/ZSPhysVal/Units/Ratios/Ratios.h
)

list(APPEND _HeadersUnitsQuantities
    ${_HeadersPath}/ZSPhysVal/Units/Quantities/Quantities.h
    ${_HeadersPath}/ZSPhysVal/Units/Quantities/Bytes.h
)

list(APPEND _Headers
    ${_HeadersCommon}
    ${_HeadersUnits}
    ${_HeadersUnitsElectricity}
    ${_HeadersUnitsGeometry}
    ${_HeadersUnitsKinematics}
    ${_HeadersUnitsRatios}
    ${_HeadersUnitsQuantities}
)

configure_file(
    ${_HeadersPath}/ZSPhysVal/TestConfig.h.in
    ${_HeadersPath}/ZSPhysVal/TestConfig.h)
set(_Headers ${_Headers} ${_HeadersPath}/ZSPhysVal/TestConfig.h)

list(APPEND _SourcesCommon
    ${_SourcesPath}/ZSPhysVal/Main.cpp
    ${_SourcesPath}/ZSPhysVal/App.cpp
    ${_SourcesPath}/ZSPhysVal/MainWindow.cpp
    ${_SourcesPath}/ZSPhysVal/WidgetCentral.cpp
    ${_SourcesPath}/ZSPhysVal/Test.cpp
)

list(APPEND _SourcesUnits
    ${_SourcesPath}/ZSPhysVal/Units/Units.cpp
)

list(APPEND _SourcesUnitsElectricity
    ${_SourcesPath}/ZSPhysVal/Units/Electricity/Electricity.cpp
    ${_SourcesPath}/ZSPhysVal/Units/Electricity/PhysSizeCurrent.cpp
    ${_SourcesPath}/ZSPhysVal/Units/Electricity/PhysSizePower.cpp
    ${_SourcesPath}/ZSPhysVal/Units/Electricity/PhysSizeResistance.cpp
    ${_SourcesPath}/ZSPhysVal/Units/Electricity/PhysSizeVoltage.cpp
)

list(APPEND _SourcesUnitsGeometry
    ${_SourcesPath}/ZSPhysVal/Units/Geometry/Geometry.cpp
    ${_SourcesPath}/ZSPhysVal/Units/Geometry/PhysSizeAngle.cpp
    ${_SourcesPath}/ZSPhysVal/Units/Geometry/PhysSizeGraphDevice.cpp
    ${_SourcesPath}/ZSPhysVal/Units/Geometry/PhysSizeLength.cpp
)

list(APPEND _SourcesUnitsKinematics
    ${_SourcesPath}/ZSPhysVal/Units/Kinematics/Kinematics.cpp
    ${_SourcesPath}/ZSPhysVal/Units/Kinematics/PhysSizeBitRate.cpp
    ${_SourcesPath}/ZSPhysVal/Units/Kinematics/PhysSizeFrequency.cpp
    ${_SourcesPath}/ZSPhysVal/Units/Kinematics/PhysSizeSamples.cpp
    ${_SourcesPath}/ZSPhysVal/Units/Kinematics/PhysSizeTime.cpp
    ${_SourcesPath}/ZSPhysVal/Units/Kinematics/PhysSizeVelocity.cpp
)

list(APPEND _SourcesUnitsRatios
    ${_SourcesPath}/ZSPhysVal/Units/Ratios/Ratios.cpp
)

list(APPEND _SourcesUnitsQuantities
    ${_SourcesPath}/ZSPhysVal/Units/Quantities/Quantities.cpp
    ${_SourcesPath}/ZSPhysVal/Units/Quantities/Bytes.cpp
)

list(APPEND _Sources
    ${_SourcesCommon}
    ${_SourcesUnits}
    ${_SourcesUnitsElectricity}
    ${_SourcesUnitsGeometry}
    ${_SourcesUnitsKinematics}
    ${_SourcesUnitsRatios}
    ${_SourcesUnitsQuantities}
)

list(APPEND _Resources
)

if(WIN32)
    # Add version info as meta information to Dll.
    set(_Resources ${_Resources} ${_ResourcesPath}/../../Libs/ZSSys/ZSSysVersion.rc)
    # Icons.
    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        set(_Headers ${_Headers}
            ${_HeadersPath}/ZSPhysVal/resource.h
            ${_HeadersPath}/ZSPhysVal/targetver.h)
        set(_Resources ${_Resources} ${_ResourcesPath}/ZSPhysVal/${PROJECT_NAME}.rc)
    endif()
endif()

# Got to create the following target name:
# (ProjectName)[_d]
# e.g.: "ZSAppTestPhysVal_d"
set(TargetOutputName ${PROJECT_NAME})
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(TargetOutputName ${TargetOutputName}d)
endif()
message(STATUS "TargetOutputName: ${TargetOutputName}")

if(WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${_Headers} ${_Sources} ${_Resources})
else()
    add_executable(${PROJECT_NAME} ${_Headers} ${_Sources} ${_Resources})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TargetOutputName})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Tests)

source_group("Header Files/Units" FILES ${_HeadersUnits})
source_group("Header Files/Units/Electricity" FILES ${_HeadersUnitsElectricity})
source_group("Header Files/Units/Geometry" FILES ${_HeadersUnitsGeometry})
source_group("Header Files/Units/Kinematics" FILES ${_HeadersUnitsKinematics})
source_group("Header Files/Units/Ratios" FILES ${_HeadersUnitsRatios})
source_group("Header Files/Units/Quantities" FILES ${_HeadersUnitsQuantities})
source_group("Header Files" FILES ${_HeadersCommon})
source_group("Source Files/Units" FILES ${_SourcesUnits})
source_group("Source Files/Units/Electricity" FILES ${_SourcesUnitsElectricity})
source_group("Source Files/Units/Geometry" FILES ${_SourcesUnitsGeometry})
source_group("Source Files/Units/Kinematics" FILES ${_SourcesUnitsKinematics})
source_group("Source Files/Units/Ratios" FILES ${_SourcesUnitsRatios})
source_group("Source Files/Units/Quantities" FILES ${_SourcesUnitsQuantities})
source_group("Source Files" FILES ${_SourcesCommon})
source_group("Resource Files" FILES ${_Resources})

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/../Include/Apps/Test/ZSPhysVal)

set(_Libs ZSPhysValGUI ZSTestGUI ZSIpcTraceGUI)

target_link_libraries(${PROJECT_NAME} ${_Libs})

enable_testing()
include(CTest)
include(Dart)
add_test(ZSPhysVal ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TargetOutputName} "AutoStartTest")

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME} EXPORT ${CMAKE_PROJECT_NAME}Targets RUNTIME DESTINATION .)

message(STATUS "")
message(STATUS "<- ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "---------------------------------------------------------------------------------------------")
message(STATUS "")
