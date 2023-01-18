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

list(APPEND _HeadersCommon
    ${_HeadersPath}/../../Libs/ZSSys/ZSSysVersion.h
    ${_HeadersPath}/ZSDraw/App.h
    ${_HeadersPath}/ZSDraw/MainWindow.h
)

list(APPEND _HeadersElectricity
    ${_HeadersPath}/ZSDraw/Electricity/ZSDrawGraphObjElectricity.h
    ${_HeadersPath}/ZSDraw/Electricity/ZSDrawGraphObjElectricityCapacitor.h
    ${_HeadersPath}/ZSDraw/Electricity/ZSDrawGraphObjElectricityDiode.h
    ${_HeadersPath}/ZSDraw/Electricity/ZSDrawGraphObjElectricityInductor.h
    ${_HeadersPath}/ZSDraw/Electricity/ZSDrawGraphObjElectricityResistor.h
    ${_HeadersPath}/ZSDraw/Electricity/ZSDrawGraphObjElectricitySwitch.h
    ${_HeadersPath}/ZSDraw/Electricity/ZSDrawGraphObjElectricityTransistor.h
    ${_HeadersPath}/ZSDraw/Electricity/ZSDrawGraphObjElectricityVoltageSource.h
    ${_HeadersPath}/ZSDraw/Electricity/ZSDrawObjFactoryElectricityCapacitor.h
    ${_HeadersPath}/ZSDraw/Electricity/ZSDrawObjFactoryElectricityDiode.h
    ${_HeadersPath}/ZSDraw/Electricity/ZSDrawObjFactoryElectricityInductor.h
    ${_HeadersPath}/ZSDraw/Electricity/ZSDrawObjFactoryElectricityResistor.h
    ${_HeadersPath}/ZSDraw/Electricity/ZSDrawObjFactoryElectricitySwitch.h
    ${_HeadersPath}/ZSDraw/Electricity/ZSDrawObjFactoryElectricityTransistor.h
    ${_HeadersPath}/ZSDraw/Electricity/ZSDrawObjFactoryElectricityVoltageSource.h
    ${_HeadersPath}/ZSDraw/Electricity/ZSDrawWdgtFormatGraphObjsElectricityCapacitor.h
    ${_HeadersPath}/ZSDraw/Electricity/ZSDrawWdgtFormatGraphObjsElectricityDiode.h
    ${_HeadersPath}/ZSDraw/Electricity/ZSDrawWdgtFormatGraphObjsElectricityInductor.h
    ${_HeadersPath}/ZSDraw/Electricity/ZSDrawWdgtFormatGraphObjsElectricityResistor.h
    ${_HeadersPath}/ZSDraw/Electricity/ZSDrawWdgtFormatGraphObjsElectricitySwitch.h
    ${_HeadersPath}/ZSDraw/Electricity/ZSDrawWdgtFormatGraphObjsElectricityTransistor.h
    ${_HeadersPath}/ZSDraw/Electricity/ZSDrawWdgtFormatGraphObjsElectricityVoltageSource.h
)

list(APPEND _HeadersQtWidgets
    ${_HeadersPath}/ZSDraw/QtWidgets/ZSDrawGraphObjWdgt.h
    ${_HeadersPath}/ZSDraw/QtWidgets/ZSDrawGraphObjWdgtCheckBox.h
    ${_HeadersPath}/ZSDraw/QtWidgets/ZSDrawGraphObjWdgtComboBox.h
    ${_HeadersPath}/ZSDraw/QtWidgets/ZSDrawGraphObjWdgtGroupBox.h
    ${_HeadersPath}/ZSDraw/QtWidgets/ZSDrawGraphObjWdgtLabel.h
    ${_HeadersPath}/ZSDraw/QtWidgets/ZSDrawGraphObjWdgtLineEdit.h
    ${_HeadersPath}/ZSDraw/QtWidgets/ZSDrawGraphObjWdgtPushButton.h
    ${_HeadersPath}/ZSDraw/QtWidgets/ZSDrawObjFactoryWdgtCheckBox.h
    ${_HeadersPath}/ZSDraw/QtWidgets/ZSDrawObjFactoryWdgtComboBox.h
    ${_HeadersPath}/ZSDraw/QtWidgets/ZSDrawObjFactoryWdgtGroupBox.h
    ${_HeadersPath}/ZSDraw/QtWidgets/ZSDrawObjFactoryWdgtLabel.h
    ${_HeadersPath}/ZSDraw/QtWidgets/ZSDrawObjFactoryWdgtLineEdit.h
    ${_HeadersPath}/ZSDraw/QtWidgets/ZSDrawObjFactoryWdgtPushButton.h
)

list(APPEND _HeadersTest
    ${_HeadersPath}/ZSDraw/Test/Test.h
)

configure_file(
    ${_HeadersPath}/ZSDraw/Test/TestConfig.h.in
    ${_HeadersPath}/ZSDraw/Test/TestConfig.h)
set(_HeadersTest ${_HeadersTest} ${_HeadersPath}/ZSDraw/Test/TestConfig.h)

list(APPEND _Headers
    ${_HeadersCommon}
    ${_HeadersElectricity}
    ${_HeadersQtWidgets}
    ${_HeadersTest}
)

list(APPEND _SourcesCommon
    ${_SourcesPath}/ZSDraw/Main.cpp
    ${_SourcesPath}/ZSDraw/App.cpp
    ${_SourcesPath}/ZSDraw/MainWindow.cpp
)

list(APPEND _SourcesElectricity
    ${_SourcesPath}/ZSDraw/Electricity/ZSDrawGraphObjElectricity.cpp
    ${_SourcesPath}/ZSDraw/Electricity/ZSDrawGraphObjElectricityCapacitor.cpp
    ${_SourcesPath}/ZSDraw/Electricity/ZSDrawGraphObjElectricityDiode.cpp
    ${_SourcesPath}/ZSDraw/Electricity/ZSDrawGraphObjElectricityInductor.cpp
    ${_SourcesPath}/ZSDraw/Electricity/ZSDrawGraphObjElectricityResistor.cpp
    ${_SourcesPath}/ZSDraw/Electricity/ZSDrawGraphObjElectricitySwitch.cpp
    ${_SourcesPath}/ZSDraw/Electricity/ZSDrawGraphObjElectricityTransistor.cpp
    ${_SourcesPath}/ZSDraw/Electricity/ZSDrawGraphObjElectricityVoltageSource.cpp
    ${_SourcesPath}/ZSDraw/Electricity/ZSDrawObjFactoryElectricityCapacitor.cpp
    ${_SourcesPath}/ZSDraw/Electricity/ZSDrawObjFactoryElectricityDiode.cpp
    ${_SourcesPath}/ZSDraw/Electricity/ZSDrawObjFactoryElectricityInductor.cpp
    ${_SourcesPath}/ZSDraw/Electricity/ZSDrawObjFactoryElectricityResistor.cpp
    ${_SourcesPath}/ZSDraw/Electricity/ZSDrawObjFactoryElectricitySwitch.cpp
    ${_SourcesPath}/ZSDraw/Electricity/ZSDrawObjFactoryElectricityTransistor.cpp
    ${_SourcesPath}/ZSDraw/Electricity/ZSDrawObjFactoryElectricityVoltageSource.cpp
    ${_SourcesPath}/ZSDraw/Electricity/ZSDrawWdgtFormatGraphObjsElectricityCapacitor.cpp
    ${_SourcesPath}/ZSDraw/Electricity/ZSDrawWdgtFormatGraphObjsElectricityDiode.cpp
    ${_SourcesPath}/ZSDraw/Electricity/ZSDrawWdgtFormatGraphObjsElectricityInductor.cpp
    ${_SourcesPath}/ZSDraw/Electricity/ZSDrawWdgtFormatGraphObjsElectricityResistor.cpp
    ${_SourcesPath}/ZSDraw/Electricity/ZSDrawWdgtFormatGraphObjsElectricitySwitch.cpp
    ${_SourcesPath}/ZSDraw/Electricity/ZSDrawWdgtFormatGraphObjsElectricityTransistor.cpp
    ${_SourcesPath}/ZSDraw/Electricity/ZSDrawWdgtFormatGraphObjsElectricityVoltageSource.cpp
)

list(APPEND _SourcesQtWidgets
    ${_SourcesPath}/ZSDraw/QtWidgets/ZSDrawGraphObjWdgt.cpp
    ${_SourcesPath}/ZSDraw/QtWidgets/ZSDrawGraphObjWdgtCheckBox.cpp
    ${_SourcesPath}/ZSDraw/QtWidgets/ZSDrawGraphObjWdgtComboBox.cpp
    ${_SourcesPath}/ZSDraw/QtWidgets/ZSDrawGraphObjWdgtGroupBox.cpp
    ${_SourcesPath}/ZSDraw/QtWidgets/ZSDrawGraphObjWdgtLabel.cpp
    ${_SourcesPath}/ZSDraw/QtWidgets/ZSDrawGraphObjWdgtLineEdit.cpp
    ${_SourcesPath}/ZSDraw/QtWidgets/ZSDrawGraphObjWdgtPushButton.cpp
    ${_SourcesPath}/ZSDraw/QtWidgets/ZSDrawObjFactoryWdgtCheckBox.cpp
    ${_SourcesPath}/ZSDraw/QtWidgets/ZSDrawObjFactoryWdgtComboBox.cpp
    ${_SourcesPath}/ZSDraw/QtWidgets/ZSDrawObjFactoryWdgtGroupBox.cpp
    ${_SourcesPath}/ZSDraw/QtWidgets/ZSDrawObjFactoryWdgtLabel.cpp
    ${_SourcesPath}/ZSDraw/QtWidgets/ZSDrawObjFactoryWdgtLineEdit.cpp
    ${_SourcesPath}/ZSDraw/QtWidgets/ZSDrawObjFactoryWdgtPushButton.cpp
)

list(APPEND _SourcesTest
    ${_SourcesPath}/ZSDraw/Test/Test.cpp
    ${_SourcesPath}/ZSDraw/Test/TestStepElectricity.cpp
    ${_SourcesPath}/ZSDraw/Test/TestStepElectricityCapacitors.cpp
    ${_SourcesPath}/ZSDraw/Test/TestStepElectricityInductors.cpp
    ${_SourcesPath}/ZSDraw/Test/TestStepElectricityResistors.cpp
    ${_SourcesPath}/ZSDraw/Test/TestStepElectricitySwitches.cpp
    ${_SourcesPath}/ZSDraw/Test/TestStepElectricityTransistors.cpp
    ${_SourcesPath}/ZSDraw/Test/TestStepStandardShapes.cpp
    ${_SourcesPath}/ZSDraw/Test/TestStepStandardShapesConnections.cpp
    ${_SourcesPath}/ZSDraw/Test/TestStepStandardShapesEllipses.cpp
    ${_SourcesPath}/ZSDraw/Test/TestStepStandardShapesGroups.cpp
    ${_SourcesPath}/ZSDraw/Test/TestStepStandardShapesImages.cpp
    ${_SourcesPath}/ZSDraw/Test/TestStepStandardShapesLines.cpp
    ${_SourcesPath}/ZSDraw/Test/TestStepStandardShapesPoints.cpp
    ${_SourcesPath}/ZSDraw/Test/TestStepStandardShapesPolygons.cpp
    ${_SourcesPath}/ZSDraw/Test/TestStepStandardShapesPolylines.cpp
    ${_SourcesPath}/ZSDraw/Test/TestStepStandardShapesRectangles.cpp
    ${_SourcesPath}/ZSDraw/Test/TestStepStandardShapesTexts.cpp
)

list(APPEND _Sources
    ${_SourcesCommon}
    ${_SourcesElectricity}
    ${_SourcesQtWidgets}
    ${_SourcesTest}
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

source_group("Header Files/Common" FILES ${_HeadersCommon})
source_group("Header Files/Electricity" FILES ${_HeadersElectricity})
source_group("Header Files/QtWidgets" FILES ${_HeadersQtWidgets})
source_group("Header Files/Test" FILES ${_HeadersTest})
source_group("Source Files/Common" FILES ${_SourcesCommon})
source_group("Source Files/Electricity" FILES ${_SourcesElectricity})
source_group("Source Files/QtWidgets" FILES ${_SourcesQtWidgets})
source_group("Source Files/Test" FILES ${_SourcesTest})
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
