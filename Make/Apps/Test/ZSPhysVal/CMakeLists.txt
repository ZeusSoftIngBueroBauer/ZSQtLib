# Call CMake from build directory (e.g. ./bin/msvc2019_x64) with:
# - for VS-2017: cmake -G "Visual Studio 15 2017" -A x64 -DCMAKE_BUILD_TYPE=Debug ../Make/Libs/ZSSys
# - for VS-2019: cmake -G "Visual Studio 16 2019" -A x64 -DCMAKE_BUILD_TYPE=Debug ../Make/Libs/ZSSys

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSAppTestPhysVal)

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
                                                            ../../../../Source/Apps/Test/ZSPhysVal/Main.cpp
    ../../../../Include/Apps/Test/ZSPhysVal/App.h           ../../../../Source/Apps/Test/ZSPhysVal/App.cpp
    ../../../../Include/Apps/Test/ZSPhysVal/MainWindow.h    ../../../../Source/Apps/Test/ZSPhysVal/MainWindow.cpp
    ../../../../Include/Apps/Test/ZSPhysVal/WidgetCentral.h ../../../../Source/Apps/Test/ZSPhysVal/WidgetCentral.cpp
    ../../../../Include/Apps/Test/ZSPhysVal/Test.h          ../../../../Source/Apps/Test/ZSPhysVal/Test.cpp
    ../../../../Include/Apps/Test/ZSPhysVal/UnitModels.h    ../../../../Source/Apps/Test/ZSPhysVal/UnitModels.cpp
    ../../../../Include/Apps/Test/ZSPhysVal/UnitsWdgt.h     ../../../../Source/Apps/Test/ZSPhysVal/UnitsWdgt.cpp
    ../../../../Include/Apps/Test/ZSPhysVal/UnitViewWdgts.h ../../../../Source/Apps/Test/ZSPhysVal/UnitViewWdgts.cpp
)

# Got to create the following target name:
# (ProjectName)[_d]
# e.g.: "ZSAppTestPhysVal_d"

set(TargetOutputName ${PROJECT_NAME})

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(TargetOutputName ${TargetOutputName}_d)
endif()

message(STATUS "TargetOutputName: ${TargetOutputName}")

add_executable(${PROJECT_NAME} ${_Sources})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TargetOutputName})

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/../Include/Apps/Test/ZSPhysVal)

target_link_libraries(${PROJECT_NAME}
    PRIVATE ZSSys ZSSysGUI
    PRIVATE ZSTest ZSTestGUI
    PRIVATE ZSPhysVal ZSPhysValGUI ZSPhysSizesElectricity ZSPhysSizesGeometry ZSPhysSizesKinematics
    PRIVATE ZSIpc ZSIpcGUI ZSIpcTrace ZSIpcTraceGUI)

add_test(
    NAME ZSPhysVal
    COMMAND $<TARGET_FILE:${PROJECT_NAME}>
    COMMENT "Running ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TargetOutputName}")