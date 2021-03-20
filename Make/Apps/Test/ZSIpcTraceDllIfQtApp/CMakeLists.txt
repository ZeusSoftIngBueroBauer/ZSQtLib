# Call CMake from build directory (e.g. ./bin/msvc2019_x64) with:
# - for VS-2017: cmake -G "Visual Studio 15 2017" -A x64 -DCMAKE_BUILD_TYPE=Debug ../Make/Libs/ZSSys
# - for VS-2019: cmake -G "Visual Studio 16 2019" -A x64 -DCMAKE_BUILD_TYPE=Debug ../Make/Libs/ZSSys

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(ZSAppTestIpcTraceDllIfQtApp)

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
                                                                       ../../../../Source/Apps/Test/ZSIpcTraceDllIfQtApp/Main.cpp
    ../../../../Include/Apps/Test/ZSIpcTraceDllIfQtApp/App.h           ../../../../Source/Apps/Test/ZSIpcTraceDllIfQtApp/App.cpp
    ../../../../Include/Apps/Test/ZSIpcTraceDllIfQtApp/MainWindow.h    ../../../../Source/Apps/Test/ZSIpcTraceDllIfQtApp/MainWindow.cpp
    ../../../../Include/Apps/Test/ZSIpcTraceDllIfQtApp/WidgetCentral.h ../../../../Source/Apps/Test/ZSIpcTraceDllIfQtApp/WidgetCentral.cpp
    ../../../../Include/Apps/Test/ZSIpcTraceDllIfQtApp/MsgTest.h       ../../../../Source/Apps/Test/ZSIpcTraceDllIfQtApp/MsgTest.cpp
    ../../../../Include/Apps/Test/ZSIpcTraceDllIfQtApp/Test.h          ../../../../Source/Apps/Test/ZSIpcTraceDllIfQtApp/Test.cpp
    ../../../../Include/Apps/Test/ZSIpcTraceDllIfQtApp/TestModule1.h   ../../../../Source/Apps/Test/ZSIpcTraceDllIfQtApp/TestModule1.cpp
    ../../../../Include/Apps/Test/ZSIpcTraceDllIfQtApp/TestModule2.h   ../../../../Source/Apps/Test/ZSIpcTraceDllIfQtApp/TestModule2.cpp
    ../../../../Include/Libs/ZSIpcTrace/ZSIpcTrcDllIf.h                ../../../../Source/Libs/ZSIpcTrace/ZSIpcTrcDllIf.cpp
)

# Got to create the following target name:
# (ProjectName)[_d]
# e.g.: "ZSAppTestIpcTraceDllIfQtApp_d"

set(TargetOutputName ${PROJECT_NAME})

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(TargetOutputName ${TargetOutputName}_d)
endif()

message(STATUS "TargetOutputName: ${TargetOutputName}")

add_executable(${PROJECT_NAME} ${_Sources})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${TargetOutputName})

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/../Include/Apps/Test/ZSIpcTraceDllIfQtApp)

target_link_libraries(${PROJECT_NAME}
    PRIVATE ZSSys ZSSysGUI ZSTest ZSTestGUI ZSIpc ZSIpcGUI ZSIpcTrace ZSIpcTraceGUI)

add_test(
    NAME ZSIpcTraceDllIfQtApp
    COMMAND $<TARGET_FILE:${PROJECT_NAME}>
    COMMENT "Running ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TargetOutputName}")
