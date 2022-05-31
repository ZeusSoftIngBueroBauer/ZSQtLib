# Copy Qt Dlls to the bin directory
# ---------------------------------
#
# The Qt Dlls QtCore, QtXml, QtNetword, QtGui and - for Qt5 - QtWidgets are copied to the
# binary directory below ZSQtLibs. Also the platform plugins dlls are copied if Qt5 is used.
#
# This macro needs a TARGET to be defined.
# So it may only be included by CMakeList files defining a target via
# add_executable or add_library.

message(STATUS "")
message(STATUS "---------------------------------------------------------------------------------------------")
message(STATUS "-> ZSQtLib/Make/Libs/ZSSys/ZSSysCopyQtDlls.cmake")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "")

if(WIN32)
    if(QT_VERSION_MAJOR EQUAL 4)
        if(CMAKE_BUILD_TYPE MATCHES Debug)
            message(STATUS "Copying Qt4 Debug Dlls")
            # Please note: Executed during build but not during configure process.
            add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
                COMMENT "Copy Qt Dlls"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QT_BINARY_DIR}/QtCored4.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/QtCored4.dll
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QT_BINARY_DIR}/QtXmld4.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/QtXmld4.dll
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QT_BINARY_DIR}/QtNetworkd4.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/QtNetworkd4.dll
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QT_BINARY_DIR}/QtGuid4.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/QtGuid4.dll)
        else()
            message(STATUS "Copying Qt4 Release Dlls")
            # Please note: Executed during build but not during configure process.
            add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
                COMMENT "Copy Qt Dlls"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QT_BINARY_DIR}/QtCore4.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/QtCore4.dll
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QT_BINARY_DIR}/QtXml4.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/QtXml4.dll
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QT_BINARY_DIR}/QtNetwork4.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/QtNetwork4.dll
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QT_BINARY_DIR}/QtGui4.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/QtGui4.dll)
        endif()
    elseif(QT_VERSION_MAJOR EQUAL 5)
        message(STATUS "Copying Qt5 Dlls")
        # Please note: Executed during build but not during configure process.
        add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
            COMMENT "Copy Qt Dlls"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Core> $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Xml> $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Network> $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Gui> $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Widgets> $<TARGET_FILE_DIR:${PROJECT_NAME}>)
        if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
            add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
                COMMENT "Copy Qt Platform Dlls"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QT_BINARY_DIR}/../plugins/platforms/qwindows.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins/platforms/qwindows.dll
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QT_BINARY_DIR}/../plugins/platforms/qwindowsd.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins/platforms/qwindowsd.dll)
        elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
            add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
                COMMENT "Copy Qt Platform Dlls"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QT_BINARY_DIR}/../plugins/platforms/qwindows.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins/platforms/qwindows.dll
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QT_BINARY_DIR}/../plugins/platforms/qwindows.dll.debug ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins/platforms/qwindows.dll.debug)
        endif()
    endif()
else()
    message(STATUS "Copying ignored as not WIN32 platform")
endif()

message(STATUS "")
message(STATUS "<- ZSQtLib/Make/Libs/ZSSys/ZSSysCopyQtDlls.cmake")
message(STATUS "---------------------------------------------------------------------------------------------")
message(STATUS "")
