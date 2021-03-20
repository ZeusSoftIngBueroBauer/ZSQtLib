# Call CMake from build directory (e.g. ./bin/msvc2019_x64) with:
# - for VS-2017: cmake -G "Visual Studio 15 2017" -A x64 -DCMAKE_BUILD_TYPE=Debug ../Make/Libs/ZSSys
# - for VS-2019: cmake -G "Visual Studio 16 2019" -A x64 -DCMAKE_BUILD_TYPE=Debug ../Make/Libs/ZSSys

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSAppDraw)

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
                                                                                                 ../../../../Source/Apps/Products/ZSDraw/Main.cpp
    ../../../../Include/Apps/Products/ZSDraw/App.h                                               ../../../../Source/Apps/Products/ZSDraw/App.cpp
    ../../../../Include/Apps/Products/ZSDraw/MainWindow.h                                        ../../../../Source/Apps/Products/ZSDraw/MainWindow.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjElectricity.h                         ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjElectricity.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjElectricityCapacitor.h                ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjElectricityCapacitor.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjElectricityDiode.h                    ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjElectricityDiode.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjElectricityInductor.h                 ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjElectricityInductor.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjElectricityResistor.h                 ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjElectricityResistor.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjElectricitySwitch.h                   ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjElectricitySwitch.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjElectricityTransistor.h               ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjElectricityTransistor.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjElectricityVoltageSource.h            ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjElectricityVoltageSource.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjWdgt.h                                ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjWdgt.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjWdgtCheckBox.h                        ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjWdgtCheckBox.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjWdgtComboBox.h                        ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjWdgtComboBox.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjWdgtGroupBox.h                        ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjWdgtGroupBox.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjWdgtLabel.h                           ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjWdgtLabel.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjWdgtLineEdit.h                        ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjWdgtLineEdit.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawGraphObjWdgtPushButton.h                      ../../../../Source/Apps/Products/ZSDraw/ZSDrawGraphObjWdgtPushButton.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawObjFactoryElectricityCapacitor.h              ../../../../Source/Apps/Products/ZSDraw/ZSDrawObjFactoryElectricityCapacitor.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawObjFactoryElectricityDiode.h                  ../../../../Source/Apps/Products/ZSDraw/ZSDrawObjFactoryElectricityDiode.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawObjFactoryElectricityInductor.h               ../../../../Source/Apps/Products/ZSDraw/ZSDrawObjFactoryElectricityInductor.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawObjFactoryElectricityResistor.h               ../../../../Source/Apps/Products/ZSDraw/ZSDrawObjFactoryElectricityResistor.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawObjFactoryElectricitySwitch.h                 ../../../../Source/Apps/Products/ZSDraw/ZSDrawObjFactoryElectricitySwitch.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawObjFactoryElectricityTransistor.h             ../../../../Source/Apps/Products/ZSDraw/ZSDrawObjFactoryElectricityTransistor.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawObjFactoryElectricityVoltageSource.h          ../../../../Source/Apps/Products/ZSDraw/ZSDrawObjFactoryElectricityVoltageSource.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawObjFactoryWdgtCheckBox.h                      ../../../../Source/Apps/Products/ZSDraw/ZSDrawObjFactoryWdgtCheckBox.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawObjFactoryWdgtComboBox.h                      ../../../../Source/Apps/Products/ZSDraw/ZSDrawObjFactoryWdgtComboBox.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawObjFactoryWdgtGroupBox.h                      ../../../../Source/Apps/Products/ZSDraw/ZSDrawObjFactoryWdgtGroupBox.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawObjFactoryWdgtLabel.h                         ../../../../Source/Apps/Products/ZSDraw/ZSDrawObjFactoryWdgtLabel.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawObjFactoryWdgtLineEdit.h                      ../../../../Source/Apps/Products/ZSDraw/ZSDrawObjFactoryWdgtLineEdit.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawObjFactoryWdgtPushButton.h                    ../../../../Source/Apps/Products/ZSDraw/ZSDrawObjFactoryWdgtPushButton.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityCapacitor.h     ../../../../Source/Apps/Products/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityCapacitor.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityDiode.h         ../../../../Source/Apps/Products/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityDiode.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityInductor.h      ../../../../Source/Apps/Products/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityInductor.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityResistor.h      ../../../../Source/Apps/Products/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityResistor.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricitySwitch.h        ../../../../Source/Apps/Products/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricitySwitch.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityTransistor.h    ../../../../Source/Apps/Products/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityTransistor.cpp
    ../../../../Include/Apps/Products/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityVoltageSource.h ../../../../Source/Apps/Products/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityVoltageSource.cpp
    ../../../../Include/Apps/Products/ZSDraw/Test.h                                              ../../../../Source/Apps/Products/ZSDraw/Test.cpp
                                                                                                 ../../../../Source/Apps/Products/ZSDraw/TestGroupInterferenceChecks.cpp
                                                                                                 ../../../../Source/Apps/Products/ZSDraw/TestStepElectricity.cpp
                                                                                                 ../../../../Source/Apps/Products/ZSDraw/TestStepElectricityCapacitors.cpp
                                                                                                 ../../../../Source/Apps/Products/ZSDraw/TestStepElectricityInductors.cpp
                                                                                                 ../../../../Source/Apps/Products/ZSDraw/TestStepElectricityResistors.cpp
                                                                                                 ../../../../Source/Apps/Products/ZSDraw/TestStepElectricitySwitches.cpp
                                                                                                 ../../../../Source/Apps/Products/ZSDraw/TestStepElectricityTransistors.cpp
                                                                                                 ../../../../Source/Apps/Products/ZSDraw/TestStepStandardShapes.cpp
                                                                                                 ../../../../Source/Apps/Products/ZSDraw/TestStepStandardShapesConnections.cpp
                                                                                                 ../../../../Source/Apps/Products/ZSDraw/TestStepStandardShapesEllipses.cpp
                                                                                                 ../../../../Source/Apps/Products/ZSDraw/TestStepStandardShapesGroups.cpp
                                                                                                 ../../../../Source/Apps/Products/ZSDraw/TestStepStandardShapesImages.cpp
                                                                                                 ../../../../Source/Apps/Products/ZSDraw/TestStepStandardShapesLines.cpp
                                                                                                 ../../../../Source/Apps/Products/ZSDraw/TestStepStandardShapesPoints.cpp
                                                                                                 ../../../../Source/Apps/Products/ZSDraw/TestStepStandardShapesPolygons.cpp
                                                                                                 ../../../../Source/Apps/Products/ZSDraw/TestStepStandardShapesPolylines.cpp
                                                                                                 ../../../../Source/Apps/Products/ZSDraw/TestStepStandardShapesRectangles.cpp
                                                                                                 ../../../../Source/Apps/Products/ZSDraw/TestStepStandardShapesTexts.cpp
    ../../../../Images/Apps/Products/ZSDraw/ZSDraw.qrc
)

# Got to create the following target name:
# (ProjectName)[_d]
# e.g.: "ZSAppDraw_d"

set(TargetOutputName ${PROJECT_NAME})

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(TargetOutputName ${TargetOutputName}_d)
endif()

message(STATUS "TargetOutputName: ${TargetOutputName}")

add_executable(${PROJECT_NAME} ${_Sources})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TargetOutputName})

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/../Include/Apps/Products/ZSDraw)

target_link_libraries(${PROJECT_NAME}
    PRIVATE ZSSys ZSSysGUI ZSTest ZSTestGUI ZSIpc ZSIpcGUI ZSIpcTrace ZSIpcTraceGUI ZSPhysVal ZSPhysValGUI ZSPhysSizesGeometry ZSPhysSizesKinematics ZSDraw)
