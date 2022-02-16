# Retrieve version information with "git describe --always --tags --long".
# ------------------------------------------------------------------------
# Using --long will always output the long format even when it matches a tags
# (the tag, the number of commits and the abbreviated commit name).
#
# The result returned by "git describe --always --tags --long" will have the following format:
#   v0.01-0-g835e9e5
#
# - The substring before the first '-' corresponds to the defined tag.
#   In order to retrieve VERSION_MAJOR and VERSION_MINOR from the tag it must follow the
#   regular expression "[v]\d{1,}[.]\d{1,}".
#   - 0  should become VERSION_MAJOR
#   - 01 should become VERSION_MINOR
# - The substring after the first '-' corresponds to the number of commits on top of the tag.
#   This value is not extracted from the GIT_VERSION_INFO string and is not saved in a separate variable.
# - The substring after the second '-' corresponds to the abbreviated commit hash
#   (usually the first seven digits of the hash) preceded by the abbreviated object name 'g' for 'git'.
#   The 'g' will be removed and the abbreviated commit hash will be converted to an integer value
#   (CMake expects integer values for version numbers in decimal format).
#   This automatically generated integer value will be used as VERSION_PATCH.
#
# Output Variables:
# -----------------
# ZSQTLIB_GIT_VERSION_INFO .. The git version info as result of the git describe call.
# ZSQTLIB_VERSION_MAJOR ..... Major version number retrieved from git tag.
# ZSQTLIB_VERSION_MINOR ..... Minor version number retrieved from git tag.
# ZSQTLIB_VERSION_PATCH ..... Integer value of the abbreviated git commit hash (usually the first seven digits).
# ZSQTLIB_VERSION ........... Complete version number consisting of <MAJOR>.<MINOR>.<PATCH>.

message(STATUS "**************************************************************")
message(STATUS "Processing ZSQtLib/cmake/git_version_info.cmake")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "**************************************************************")
message(STATUS "")

find_package(Git REQUIRED)
if (GIT_FOUND)
    message(STATUS "Git found")
else ()
    message(STATUS "Git not found")
endif ()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")
    set(GIT_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
else()
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../.git")
        set(GIT_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/..")
    else()
        message(FATAL_ERROR "Cannot create version file for ZSQtLib as ${CMAKE_CURRENT_SOURCE_DIR}/.git does not exist")
    endif()
endif()
message(STATUS "GIT_WORKING_DIRECTORY: ${GIT_WORKING_DIRECTORY}")

execute_process(COMMAND ${GIT_EXECUTABLE} describe --always --tags --long
    WORKING_DIRECTORY "${GIT_WORKING_DIRECTORY}"
    OUTPUT_VARIABLE ZSQTLIB_GIT_VERSION_INFO
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "ZSQTLIB_GIT_VERSION_INFO: ${ZSQTLIB_GIT_VERSION_INFO}")

# Split ZSQTLIB_GIT_VERSION_INFO into list containing git tag, number of commits and commit hash value.
string(REPLACE "-" ";" ZSQTLIB_GIT_VERSION_INFO_LIST ${ZSQTLIB_GIT_VERSION_INFO})
list(LENGTH ZSQTLIB_GIT_VERSION_INFO_LIST versionInfoListLen)
# Abort CMake if version info does not follow the expected format with 3 sections separated by '-'.
if (NOT versionInfoListLen EQUAL 3)
    message(FATAL_ERROR "Cannot create version file for ZSQtLib as ${ZSQTLIB_GIT_VERSION_INFO} has wrong format")
endif()
list(GET ZSQTLIB_GIT_VERSION_INFO_LIST 0 ZSQTLIB_GIT_TAG)
list(GET ZSQTLIB_GIT_VERSION_INFO_LIST 2 ZSQTLIB_GIT_COMMIT_HASH)

# Remove starting "v" character from first git tag and split into list with VERSION_MAJOR and VERSION_MINOR.
string(REPLACE "v" "" ZSQTLIB_GIT_TAG ${ZSQTLIB_GIT_TAG})
string(REPLACE "." ";" ZSQTLIB_GIT_TAG_LIST ${ZSQTLIB_GIT_TAG})
# Abort CMake if git tag does not follow the expected format with 2 sections separated by '.'.
list(LENGTH ZSQTLIB_GIT_TAG_LIST tagListLen)
if (NOT tagListLen EQUAL 2)
    message(FATAL_ERROR "Cannot create version file for ZSQtLib as ${ZSQTLIB_GIT_VERSION_INFO} has wrong format")
endif()
list(GET ZSQTLIB_GIT_TAG_LIST 0 ZSQTLIB_VERSION_MAJOR)
list(GET ZSQTLIB_GIT_TAG_LIST 1 ZSQTLIB_VERSION_MINOR)

# Convert hexadecimal value of git hash into integer value by removing abbreviated object name 'g'.
string(REPLACE "g" "" ZSQTLIB_GIT_COMMIT_HASH ${ZSQTLIB_GIT_COMMIT_HASH})
math(EXPR ZSQTLIB_VERSION_PATCH "0x${ZSQTLIB_GIT_COMMIT_HASH}" OUTPUT_FORMAT DECIMAL)

set(ZSQTLIB_VERSION ${ZSQTLIB_VERSION_MAJOR}.${ZSQTLIB_VERSION_MINOR}.${ZSQTLIB_VERSION_PATCH} CACHE STRING "ZSQtLibVersion" FORCE)

message(STATUS "ZSQTLIB_VERSION_MAJOR:    ${ZSQTLIB_VERSION_MAJOR}")
message(STATUS "ZSQTLIB_VERSION_MINOR:    ${ZSQTLIB_VERSION_MINOR}")
message(STATUS "ZSQTLIB_VERSION_PATCH:    ${ZSQTLIB_VERSION_PATCH}")
message(STATUS "ZSQTLIB_VERSION:          ${ZSQTLIB_VERSION}")
