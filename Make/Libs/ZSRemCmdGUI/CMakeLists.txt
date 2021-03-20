# Call CMake from build directory (e.g. ./bin/msvc2019_x64) with:
# - for VS-2017: cmake -G "Visual Studio 15 2017" -A x64 -DCMAKE_BUILD_TYPE=Debug ../Make/Libs/ZSSys
# - for VS-2019: cmake -G "Visual Studio 16 2019" -A x64 -DCMAKE_BUILD_TYPE=Debug ../Make/Libs/ZSSys

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSRemCmdGUI)

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
    ../../../Include/Libs/ZSRemCmdGUI/ZSRemCmdGUIDllMain.h   ../../../Source/Libs/ZSRemCmdGUI/ZSRemCmdGUIDllMain.cpp
    ../../../Include/Libs/ZSRemCmdGUI/ZSRemCmdObjPoolModel.h ../../../Source/Libs/ZSRemCmdGUI/ZSRemCmdObjPoolModel.cpp
    ../../../Include/Libs/ZSRemCmdGUI/ZSRemCmdObjPoolWdgt.h  ../../../Source/Libs/ZSRemCmdGUI/ZSRemCmdObjPoolWdgt.cpp
    ../../../Include/Libs/ZSRemCmdGUI/ZSRemCmdsDlg.h         ../../../Source/Libs/ZSRemCmdGUI/ZSRemCmdsDlg.cpp
)

# Got to create the following target name:
# (ProjectName)Qt$(QT_VERSION_MAJOR)_$(Compiler)_$(Platform)[_d]
# e.g.: "ZSRemCmdGUIQt5_msvc2019_x64_d"

set(TargetOutputName ${PROJECT_NAME}Qt${QT_VERSION_MAJOR}_${Compiler}_${Platform})

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(TargetOutputName ${TargetOutputName}_d)
endif()

message(STATUS "TargetOutputName: ${TargetOutputName}")

add_library(${PROJECT_NAME} SHARED ${_Sources})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TargetOutputName})

target_compile_definitions(${PROJECT_NAME} PRIVATE ZSREMCMDGUIDLL_EXPORTS)

target_link_libraries(${PROJECT_NAME} PUBLIC ZSSys ZSSysGUI ZSIpc ZSIpcGUI ZSIpcTrace ZSIpcTraceGUI ZSRemCmd)

