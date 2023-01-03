# Retrieve version information with "git describe --always --tags --long".
# ------------------------------------------------------------------------
#
# This CMake macro generates the version info out of the most recent git tag.
#
# Using --long will always output the long format even when it matches a tag
# (the tag, the number of commits and the abbreviated commit name).
#
# If you are using a commit with doesn't match a tag the number of commits since
# the last recent tag is 0. You can check the corresponding output variable
# (see below) to determine wheter you have checkout out an official release or
# an unofficial temporary version used for development.
#
# This allows identifying the exaclty used git tag in the configured "version.h"
# and "version.rc" file and my be shown either as the "About" info of your application
# or as meta information in the created binaries - of the operating system supports
# such meta information.
#
# The result returned by "git describe --always --tags" will have the following format:
#   v0.01.02-0-g835e9e5
#
# - The substring before the first '-' corresponds to the defined tag.
#   - 0  becomes VERSION_MAJOR
#   - 01 becomes VERSION_MINOR
#   - 02 becomes VERSION_PATCH
# - The substring after the first '-' corresponds to the number of commits on top of the tag.
# - The substring after the second '-' corresponds to the abbreviated commit hash
#   (usually the first seven digits of the hash) preceded by the abbreviated object name 'g' for 'git'.
#   The 'g' will be removed and the abbreviated commit hash will also be converted to an integer value
#   (CMake expects integer values for version numbers in decimal format).
#   This automatically generated integer value will be used as VERSION_PATCH.

# Output Variables:
# -----------------
# ZSQTLIB_GIT_VERSION_INFO ....... The git version info as result of the git describe call.
# ZSQTLIB_GIT_COMMITS_SINCE_TAG .. Number of commits on top of tag.
#                                  If 0 the commit matches a tag and the commit is an official release.
#                                  If greater than 0 the commit is a temporary development version.
# ZSQTLIB_GIT_COMMIT_HASH_HEX .... Abbreviated git commit hash (hexadecimal value as used by git).
# ZSQTLIB_GIT_COMMIT_HASH_INT .... Abbreviated git commit hash converted into integer value.
# ZSQTLIB_VERSION_MAJOR .......... Major version number retrieved from git tag.
# ZSQTLIB_VERSION_MINOR .......... Minor version number retrieved from git tag.
# ZSQTLIB_VERSION_PATCH .......... Patch version number retrieved from git tag.
# ZSQTLIB_VERSION ................ Complete version number consisting of <MAJOR>.<MINOR>.<PATCH>.

message(STATUS "")
message(STATUS "---------------------------------------------------------------------------------------------")
message(STATUS "-> ZSQtLib/Make/ZSQtLibsGetGitVersionInfo.cmake")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "")

find_package(Git REQUIRED)
if (GIT_FOUND)
    message(STATUS "Git found")
else ()
    message(STATUS "Git not found")
endif ()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")
    set(GIT_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
elseif (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../.git")
    set(GIT_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/..")
elseif (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../../.git")
    set(GIT_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../..")
elseif (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../../../.git")
    set(GIT_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../..")
else()
    message(FATAL_ERROR "Cannot create version file for ZSQtLib as ${CMAKE_CURRENT_SOURCE_DIR}/.git does not exist")
endif()
message(STATUS "GIT_WORKING_DIRECTORY: ${GIT_WORKING_DIRECTORY}")

execute_process(COMMAND ${GIT_EXECUTABLE} describe --always --tags --long
    WORKING_DIRECTORY "${GIT_WORKING_DIRECTORY}"
    OUTPUT_VARIABLE ZSQTLIB_GIT_VERSION_INFO
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "ZSQTLIB_GIT_VERSION_INFO:      ${ZSQTLIB_GIT_VERSION_INFO}")

# Split ZSQTLIB_GIT_VERSION_INFO into list containing git tag, number of commits and commit hash value.
string(REPLACE "-" ";" ZSQTLIB_GIT_VERSION_INFO_LIST ${ZSQTLIB_GIT_VERSION_INFO})
list(LENGTH ZSQTLIB_GIT_VERSION_INFO_LIST versionInfoListLen)
# Abort CMake if version info does not follow the expected format with 3 sections separated by '-'.
if (NOT versionInfoListLen EQUAL 3)
    message(FATAL_ERROR "Cannot create version file for ZSQtLib as ${ZSQTLIB_GIT_VERSION_INFO} has wrong format")
endif()
list(GET ZSQTLIB_GIT_VERSION_INFO_LIST 0 ZSQTLIB_GIT_TAG)
list(GET ZSQTLIB_GIT_VERSION_INFO_LIST 1 ZSQTLIB_GIT_COMMITS_SINCE_TAG)
list(GET ZSQTLIB_GIT_VERSION_INFO_LIST 2 ZSQTLIB_GIT_COMMIT_HASH_HEX)

# Remove leading 'g' from git commit hash.
string(REPLACE "g" "" ZSQTLIB_GIT_COMMIT_HASH_HEX ${ZSQTLIB_GIT_COMMIT_HASH_HEX})
# Convert hexadecimal value of git hash into integer value.
math(EXPR ZSQTLIB_GIT_COMMIT_HASH_INT "0x${ZSQTLIB_GIT_COMMIT_HASH_HEX}" OUTPUT_FORMAT DECIMAL)

# Remove starting "v" character from first git tag and split into list with
# VERSION_MAJOR, VERSION_MINOR and VERSION_PATCH.
string(REPLACE "v" "" ZSQTLIB_GIT_TAG ${ZSQTLIB_GIT_TAG})
string(REPLACE "." ";" ZSQTLIB_GIT_TAG_LIST ${ZSQTLIB_GIT_TAG})
# Abort CMake if git tag does not follow the expected format with 3 sections separated by '.'.
list(LENGTH ZSQTLIB_GIT_TAG_LIST tagListLen)
if (NOT tagListLen EQUAL 3)
    message(FATAL_ERROR "Cannot create version file for ZSQtLib as ${ZSQTLIB_GIT_VERSION_INFO} has wrong format")
endif()
list(GET ZSQTLIB_GIT_TAG_LIST 0 ZSQTLIB_VERSION_MAJOR)
list(GET ZSQTLIB_GIT_TAG_LIST 1 ZSQTLIB_VERSION_MINOR)
list(GET ZSQTLIB_GIT_TAG_LIST 2 ZSQTLIB_VERSION_PATCH)

set(ZSQTLIB_VERSION ${ZSQTLIB_VERSION_MAJOR}.${ZSQTLIB_VERSION_MINOR}.${ZSQTLIB_VERSION_PATCH} CACHE STRING "ZSQtLibVersion" FORCE)

message(STATUS "ZSQTLIB_GIT_COMMITS_SINCE_TAG: ${ZSQTLIB_GIT_COMMITS_SINCE_TAG}")
message(STATUS "ZSQTLIB_GIT_COMMIT_HASH_HEX:   ${ZSQTLIB_GIT_COMMIT_HASH_HEX}")
message(STATUS "ZSQTLIB_GIT_COMMIT_HASH_INT:   ${ZSQTLIB_GIT_COMMIT_HASH_INT}")
message(STATUS "ZSQTLIB_VERSION_MAJOR:         ${ZSQTLIB_VERSION_MAJOR}")
message(STATUS "ZSQTLIB_VERSION_MINOR:         ${ZSQTLIB_VERSION_MINOR}")
message(STATUS "ZSQTLIB_VERSION_PATCH:         ${ZSQTLIB_VERSION_PATCH}")
message(STATUS "ZSQTLIB_VERSION:               ${ZSQTLIB_VERSION}")

message(STATUS "")
message(STATUS "<- ZSQtLib/Make/ZSQtLibsGetGitVersionInfo.cmake")
message(STATUS "---------------------------------------------------------------------------------------------")
message(STATUS "")
