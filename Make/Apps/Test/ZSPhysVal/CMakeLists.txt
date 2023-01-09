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
    ${_HeadersPath}/ZSPhysVal/UnitModels.h
    ${_HeadersPath}/ZSPhysVal/UnitsWdgt.h
    ${_HeadersPath}/ZSPhysVal/UnitViewWdgts.h
)

list(APPEND _HeadersPhysSize
    ${_HeadersPath}/ZSPhysVal/PhysSizes/PhysSizes.h
)

list(APPEND _HeadersPhysSizeElectricity
    ${_HeadersPath}/ZSPhysVal/PhysSizes/Electricity/Electricity.h
    ${_HeadersPath}/ZSPhysVal/PhysSizes/Electricity/PhysSizeCurrent.h
    ${_HeadersPath}/ZSPhysVal/PhysSizes/Electricity/PhysSizePower.h
    ${_HeadersPath}/ZSPhysVal/PhysSizes/Electricity/PhysSizeResistance.h
    ${_HeadersPath}/ZSPhysVal/PhysSizes/Electricity/PhysSizeVoltage.h
)

list(APPEND _HeadersPhysSizeGeometry
    ${_HeadersPath}/ZSPhysVal/PhysSizes/Geometry/Geometry.h
    ${_HeadersPath}/ZSPhysVal/PhysSizes/Geometry/PhysSizeAngle.h
    ${_HeadersPath}/ZSPhysVal/PhysSizes/Geometry/PhysSizeGraphDevice.h
    ${_HeadersPath}/ZSPhysVal/PhysSizes/Geometry/PhysSizeLength.h
)

list(APPEND _HeadersPhysSizeKinematics
    ${_HeadersPath}/ZSPhysVal/PhysSizes/Kinematics/Kinematics.h
    ${_HeadersPath}/ZSPhysVal/PhysSizes/Kinematics/PhysSizeBitRate.h
    ${_HeadersPath}/ZSPhysVal/PhysSizes/Kinematics/PhysSizeFrequency.h
    ${_HeadersPath}/ZSPhysVal/PhysSizes/Kinematics/PhysSizeSamples.h
    ${_HeadersPath}/ZSPhysVal/PhysSizes/Kinematics/PhysSizeTime.h
    ${_HeadersPath}/ZSPhysVal/PhysSizes/Kinematics/PhysSizeVelocity.h
)

list(APPEND _Headers
    ${_HeadersCommon}
    ${_HeadersPhysSize}
    ${_HeadersPhysSizeElectricity}
    ${_HeadersPhysSizeGeometry}
    ${_HeadersPhysSizeKinematics}
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
    ${_SourcesPath}/ZSPhysVal/UnitModels.cpp
    ${_SourcesPath}/ZSPhysVal/UnitsWdgt.cpp
    ${_SourcesPath}/ZSPhysVal/UnitViewWdgts.cpp
)

list(APPEND _SourcesPhysSize
    ${_SourcesPath}/ZSPhysVal/PhysSizes/PhysSizes.cpp
)

list(APPEND _SourcesPhysSizeElectricity
    ${_SourcesPath}/ZSPhysVal/PhysSizes/Electricity/Electricity.cpp
    ${_SourcesPath}/ZSPhysVal/PhysSizes/Electricity/PhysSizeCurrent.cpp
    ${_SourcesPath}/ZSPhysVal/PhysSizes/Electricity/PhysSizePower.cpp
    ${_SourcesPath}/ZSPhysVal/PhysSizes/Electricity/PhysSizeResistance.cpp
    ${_SourcesPath}/ZSPhysVal/PhysSizes/Electricity/PhysSizeVoltage.cpp
)

list(APPEND _SourcesPhysSizeGeometry
    ${_SourcesPath}/ZSPhysVal/PhysSizes/Geometry/Geometry.cpp
    ${_SourcesPath}/ZSPhysVal/PhysSizes/Geometry/PhysSizeAngle.cpp
    ${_SourcesPath}/ZSPhysVal/PhysSizes/Geometry/PhysSizeGraphDevice.cpp
    ${_SourcesPath}/ZSPhysVal/PhysSizes/Geometry/PhysSizeLength.cpp
)

list(APPEND _SourcesPhysSizeKinematics
    ${_SourcesPath}/ZSPhysVal/PhysSizes/Kinematics/Kinematics.cpp
    ${_SourcesPath}/ZSPhysVal/PhysSizes/Kinematics/PhysSizeBitRate.cpp
    ${_SourcesPath}/ZSPhysVal/PhysSizes/Kinematics/PhysSizeFrequency.cpp
    ${_SourcesPath}/ZSPhysVal/PhysSizes/Kinematics/PhysSizeSamples.cpp
    ${_SourcesPath}/ZSPhysVal/PhysSizes/Kinematics/PhysSizeTime.cpp
    ${_SourcesPath}/ZSPhysVal/PhysSizes/Kinematics/PhysSizeVelocity.cpp
)

list(APPEND _Sources
    ${_SourcesCommon}
    ${_SourcesPhysSize}
    ${_SourcesPhysSizeElectricity}
    ${_SourcesPhysSizeGeometry}
    ${_SourcesPhysSizeKinematics}
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

source_group("Header Files/Physical Sizes" FILES ${_HeadersPhysSize})
source_group("Header Files/Physical Sizes/Electricity" FILES ${_HeadersPhysSizeElectricity})
source_group("Header Files/Physical Sizes/Geometry" FILES ${_HeadersPhysSizeGeometry})
source_group("Header Files/Physical Sizes/Kinematics" FILES ${_HeadersPhysSizeKinematics})
source_group("Header Files" FILES ${_HeadersCommon})
source_group("Source Files/Physical Sizes" FILES ${_SourcesPhysSize})
source_group("Source Files/Physical Sizes/Electricity" FILES ${_SourcesPhysSizeElectricity})
source_group("Source Files/Physical Sizes/Geometry" FILES ${_SourcesPhysSizeGeometry})
source_group("Source Files/Physical Sizes/Kinematics" FILES ${_SourcesPhysSizeKinematics})
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
