cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSAppDraw)

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
set(_HeadersPath ${CMAKE_CURRENT_SOURCE_DIR}/../../../../Include/Apps/Products)
set(_SourcesPath ${CMAKE_CURRENT_SOURCE_DIR}/../../../../Source/Apps/Products)
set(_ResourcesPath ${CMAKE_CURRENT_SOURCE_DIR}/../../../../Resources/Apps/Products)

list(APPEND _Headers
    ${_HeadersPath}/../../Libs/ZSSys/ZSSysVersion.h
    ${_HeadersPath}/ZSDraw/App.h
    ${_HeadersPath}/ZSDraw/MainWindow.h
    ${_HeadersPath}/ZSDraw/ZSDrawGraphObjElectricity.h
    ${_HeadersPath}/ZSDraw/ZSDrawGraphObjElectricityCapacitor.h
    ${_HeadersPath}/ZSDraw/ZSDrawGraphObjElectricityDiode.h
    ${_HeadersPath}/ZSDraw/ZSDrawGraphObjElectricityInductor.h
    ${_HeadersPath}/ZSDraw/ZSDrawGraphObjElectricityResistor.h
    ${_HeadersPath}/ZSDraw/ZSDrawGraphObjElectricitySwitch.h
    ${_HeadersPath}/ZSDraw/ZSDrawGraphObjElectricityTransistor.h
    ${_HeadersPath}/ZSDraw/ZSDrawGraphObjElectricityVoltageSource.h
    ${_HeadersPath}/ZSDraw/ZSDrawGraphObjWdgt.h
    ${_HeadersPath}/ZSDraw/ZSDrawGraphObjWdgtCheckBox.h
    ${_HeadersPath}/ZSDraw/ZSDrawGraphObjWdgtComboBox.h
    ${_HeadersPath}/ZSDraw/ZSDrawGraphObjWdgtGroupBox.h
    ${_HeadersPath}/ZSDraw/ZSDrawGraphObjWdgtLabel.h
    ${_HeadersPath}/ZSDraw/ZSDrawGraphObjWdgtLineEdit.h
    ${_HeadersPath}/ZSDraw/ZSDrawGraphObjWdgtPushButton.h
    ${_HeadersPath}/ZSDraw/ZSDrawObjFactoryElectricityCapacitor.h
    ${_HeadersPath}/ZSDraw/ZSDrawObjFactoryElectricityDiode.h
    ${_HeadersPath}/ZSDraw/ZSDrawObjFactoryElectricityInductor.h
    ${_HeadersPath}/ZSDraw/ZSDrawObjFactoryElectricityResistor.h
    ${_HeadersPath}/ZSDraw/ZSDrawObjFactoryElectricitySwitch.h
    ${_HeadersPath}/ZSDraw/ZSDrawObjFactoryElectricityTransistor.h
    ${_HeadersPath}/ZSDraw/ZSDrawObjFactoryElectricityVoltageSource.h
    ${_HeadersPath}/ZSDraw/ZSDrawObjFactoryWdgtCheckBox.h
    ${_HeadersPath}/ZSDraw/ZSDrawObjFactoryWdgtComboBox.h
    ${_HeadersPath}/ZSDraw/ZSDrawObjFactoryWdgtGroupBox.h
    ${_HeadersPath}/ZSDraw/ZSDrawObjFactoryWdgtLabel.h
    ${_HeadersPath}/ZSDraw/ZSDrawObjFactoryWdgtLineEdit.h
    ${_HeadersPath}/ZSDraw/ZSDrawObjFactoryWdgtPushButton.h
    ${_HeadersPath}/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityCapacitor.h
    ${_HeadersPath}/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityDiode.h
    ${_HeadersPath}/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityInductor.h
    ${_HeadersPath}/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityResistor.h
    ${_HeadersPath}/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricitySwitch.h
    ${_HeadersPath}/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityTransistor.h
    ${_HeadersPath}/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityVoltageSource.h
    ${_HeadersPath}/ZSDraw/Test.h
)

configure_file(
    ${_HeadersPath}/ZSDraw/TestConfig.h.in
    ${_HeadersPath}/ZSDraw/TestConfig.h)
set(_HeadersCommon ${_HeadersCommon} ${_HeadersPath}/ZSDraw/TestConfig.h)

list(APPEND _Sources
    ${_SourcesPath}/ZSDraw/Main.cpp
    ${_SourcesPath}/ZSDraw/App.cpp
    ${_SourcesPath}/ZSDraw/MainWindow.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawGraphObjElectricity.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawGraphObjElectricityCapacitor.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawGraphObjElectricityDiode.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawGraphObjElectricityInductor.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawGraphObjElectricityResistor.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawGraphObjElectricitySwitch.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawGraphObjElectricityTransistor.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawGraphObjElectricityVoltageSource.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawGraphObjWdgt.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawGraphObjWdgtCheckBox.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawGraphObjWdgtComboBox.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawGraphObjWdgtGroupBox.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawGraphObjWdgtLabel.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawGraphObjWdgtLineEdit.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawGraphObjWdgtPushButton.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawObjFactoryElectricityCapacitor.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawObjFactoryElectricityDiode.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawObjFactoryElectricityInductor.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawObjFactoryElectricityResistor.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawObjFactoryElectricitySwitch.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawObjFactoryElectricityTransistor.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawObjFactoryElectricityVoltageSource.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawObjFactoryWdgtCheckBox.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawObjFactoryWdgtComboBox.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawObjFactoryWdgtGroupBox.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawObjFactoryWdgtLabel.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawObjFactoryWdgtLineEdit.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawObjFactoryWdgtPushButton.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityCapacitor.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityDiode.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityInductor.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityResistor.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricitySwitch.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityTransistor.cpp
    ${_SourcesPath}/ZSDraw/ZSDrawWdgtFormatGraphObjsElectricityVoltageSource.cpp
    ${_SourcesPath}/ZSDraw/Test.cpp
    ${_SourcesPath}/ZSDraw/TestStepElectricity.cpp
    ${_SourcesPath}/ZSDraw/TestStepElectricityCapacitors.cpp
    ${_SourcesPath}/ZSDraw/TestStepElectricityInductors.cpp
    ${_SourcesPath}/ZSDraw/TestStepElectricityResistors.cpp
    ${_SourcesPath}/ZSDraw/TestStepElectricitySwitches.cpp
    ${_SourcesPath}/ZSDraw/TestStepElectricityTransistors.cpp
    ${_SourcesPath}/ZSDraw/TestStepStandardShapes.cpp
    ${_SourcesPath}/ZSDraw/TestStepStandardShapesConnections.cpp
    ${_SourcesPath}/ZSDraw/TestStepStandardShapesEllipses.cpp
    ${_SourcesPath}/ZSDraw/TestStepStandardShapesGroups.cpp
    ${_SourcesPath}/ZSDraw/TestStepStandardShapesImages.cpp
    ${_SourcesPath}/ZSDraw/TestStepStandardShapesLines.cpp
    ${_SourcesPath}/ZSDraw/TestStepStandardShapesPoints.cpp
    ${_SourcesPath}/ZSDraw/TestStepStandardShapesPolygons.cpp
    ${_SourcesPath}/ZSDraw/TestStepStandardShapesPolylines.cpp
    ${_SourcesPath}/ZSDraw/TestStepStandardShapesRectangles.cpp
    ${_SourcesPath}/ZSDraw/TestStepStandardShapesTexts.cpp
)

list(APPEND _Resources
    ${_ResourcesPath}/ZSDraw/ZSDraw.qrc
)

list(APPEND _ElectricityImages
    ${_ResourcesPath}/ZSDraw/Images/Electricity/Capacitor16x16.bmp
    ${_ResourcesPath}/ZSDraw/Images/Electricity/Diode16x16.bmp
    ${_ResourcesPath}/ZSDraw/Images/Electricity/Inductor16x10.bmp
    ${_ResourcesPath}/ZSDraw/Images/Electricity/Inductor16x16.bmp
    ${_ResourcesPath}/ZSDraw/Images/Electricity/Resistor16x16.bmp
    ${_ResourcesPath}/ZSDraw/Images/Electricity/Switch16x16.bmp
    ${_ResourcesPath}/ZSDraw/Images/Electricity/Transistor16x16.bmp
    ${_ResourcesPath}/ZSDraw/Images/Electricity/VoltageSource16x16.bmp
)

list(APPEND _QtWidgetsImages
    ${_ResourcesPath}/ZSDraw/Images/QtWidgets/CheckBox16x16.bmp
    ${_ResourcesPath}/ZSDraw/Images/QtWidgets/ComboBox16x16.bmp
    ${_ResourcesPath}/ZSDraw/Images/QtWidgets/GroupBox16x16.bmp
    ${_ResourcesPath}/ZSDraw/Images/QtWidgets/Label16x16.bmp
    ${_ResourcesPath}/ZSDraw/Images/QtWidgets/LineEdit16x16.bmp
    ${_ResourcesPath}/ZSDraw/Images/QtWidgets/PushButton16x16.bmp
)

list(APPEND _Images
    ${_ElectricityImages}
    ${_QtWidgetsImages}
)
if(WIN32)
    # Add version info as meta information to Dll.
    set(_Resources ${_Resources} ${_ResourcesPath}/../../Libs/ZSSys/ZSSysVersion.rc)
    # Icons.
    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        set(_Headers ${_Headers}
            ${_HeadersPath}/ZSDraw/resource.h
            ${_HeadersPath}/ZSDraw/targetver.h)
        set(_Resources ${_Resources} ${_ResourcesPath}/ZSDraw/${PROJECT_NAME}.rc)
    endif()
endif()

# Got to create the following target name:
# (ProjectName)[d]
# e.g.: "ZSAppDrawd"
set(TargetOutputName ${PROJECT_NAME})
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(TargetOutputName ${TargetOutputName}d)
endif()
message(STATUS "TargetOutputName:         ${TargetOutputName}")

if(WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${_Headers} ${_Sources} ${_Resources} ${_Images})
else()
    add_executable(${PROJECT_NAME} ${_Headers} ${_Sources} ${_Resources} ${_Images})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TargetOutputName})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Products)

source_group("Header Files" FILES ${_Headers})
source_group("Source Files" FILES ${_Sources})
source_group("Resource Files" FILES ${_Resources})
source_group("Image Files/Electricity" FILES ${_ElectricityImages})
source_group("Image Files/QtWidgets" FILES ${_QtWidgetsImages})

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/../Include/Apps/Products/ZSDraw)

set(_Libs ZSIpcTraceGUI ZSPhysValGUI ZSTestGUI ZSDraw)

target_link_libraries(${PROJECT_NAME} ${_Libs})

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME} EXPORT ${CMAKE_PROJECT_NAME}Targets RUNTIME DESTINATION .)

# Prepare installer files

# Get Current Date Time
string(TIMESTAMP RELEASE_DATE_YEAR "%Y" UTC)
string(TIMESTAMP RELEASE_DATE_TIME "%Y-%m-%d %H:%m" UTC)
message(STATUS "RELEASE_DATE:             ${RELEASE_DATE_TIME}")

if(WIN32)
    set(ApplicationsDir "$ENV{ProgramFiles}")
    set(AppDataDir "$ENV{APPDATA}")
    # Need to replace \ with /. Otherwise a very strange StartMenu path is created.
    string(REPLACE "\\" "/" ApplicationsDir ${ApplicationsDir})
    string(REPLACE "\\" "/" AppDataDir ${AppDataDir})
    message(STATUS "ApplicationsDir:          ${ApplicationsDir}")
    message(STATUS "AppDataDir:               ${AppDataDir}")

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Installer/config/config.xml.in
                   ${CMAKE_CURRENT_SOURCE_DIR}/Installer/config/config.xml)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Installer/packages/de.zeussoft.Draw/meta/license.txt.in
                   ${CMAKE_CURRENT_SOURCE_DIR}/Installer/packages/de.zeussoft.Draw/meta/license.txt)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Installer/packages/de.zeussoft.Draw/meta/package.xml.in
                   ${CMAKE_CURRENT_SOURCE_DIR}/Installer/packages/de.zeussoft.Draw/meta/package.xml)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Installer/packages/de.zeussoft.Draw/meta/installscript.qs.in
                   ${CMAKE_CURRENT_SOURCE_DIR}/Installer/packages/de.zeussoft.Draw/meta/installscript.qs)
endif()

message(STATUS "")
message(STATUS "<- ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "---------------------------------------------------------------------------------------------")
message(STATUS "")
