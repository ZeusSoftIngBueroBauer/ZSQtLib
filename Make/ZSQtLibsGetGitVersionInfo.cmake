# Retrieve version information with "git describe --always --tags --long".
# ------------------------------------------------------------------------
#
# This CMake macro generates the version info out of the most recent git tag.
# Only the git tag is evaluated. This means that for all commits after a valid
# git tag will get the same version number. This is also the intention as only
# commits with an assigned tag are considered as release versions.
#
# If you are using a commit with doesn't match a tag the commit hash will
# be set at the output variable ZSQTLIB_GIT_VERSION_INFO. If a tag is assigned
# to the commit the ZSQTLIB_GIT_VERSION_INFO is empty.
#
# This allows identifying the exaclty used git tag in the configured "version.h"
# and "version.rc" file and my be shown either as the "About" info of your application
# or as meta information in the created binaries - of the operating system supports
# such meta information.
#
# The result returned by "git describe --always --tags" will have the following format:
#   v0.01.02[-0-g835e9e5]
#
# - The substring before the first '-' corresponds to the defined tag.
#   In order to retrieve VERSION_MAJOR, VERSION_MINOR and VERSION_PATCH from the tag it must follow the
#   regular expression "[v]\d{1,}[.]\d{1,}[.]\d{1,}".
#   - 0  becomes VERSION_MAJOR
#   - 01 becomes VERSION_MINOR
#   - 02 becomes VERSION_PATCH
# - The string including and following the first '-' is only output if the commit does
#   not match a tag. If a tag is not hit:
#   - The first substring corresponds to the number of commits on top of the tag.
#     This value is not extracted from the GIT_VERSION_INFO string and is not saved in a separate variable.
#   - The substring after the second '-' corresponds to the abbreviated commit hash
#     (usually the first seven digits of the hash) preceded by the abbreviated object name 'g' for 'git'.
#
# Output Variables:
# -----------------
# ZSQTLIB_GIT_VERSION_INFO .. The git version info as result of the git describe call.
# ZSQTLIB_VERSION_MAJOR ..... Major version number retrieved from git tag.
# ZSQTLIB_VERSION_MINOR ..... Minor version number retrieved from git tag.
# ZSQTLIB_VERSION_PATCH ..... Patch version number retrieved from git tag.
# ZSQTLIB_VERSION ........... Complete version number consisting of <MAJOR>.<MINOR>.<PATCH>.

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

execute_process(COMMAND ${GIT_EXECUTABLE} describe --always --tags
    WORKING_DIRECTORY "${GIT_WORKING_DIRECTORY}"
    OUTPUT_VARIABLE ZSQTLIB_GIT_VERSION_INFO
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "ZSQTLIB_GIT_VERSION_INFO: ${ZSQTLIB_GIT_VERSION_INFO}")

# Split ZSQTLIB_GIT_VERSION_INFO into list containing git tag, number of commits and commit hash value.
string(REPLACE "-" ";" ZSQTLIB_GIT_VERSION_INFO_LIST ${ZSQTLIB_GIT_VERSION_INFO})
list(LENGTH ZSQTLIB_GIT_VERSION_INFO_LIST versionInfoListLen)
if (versionInfoListLen EQUAL 1)
    list(GET ZSQTLIB_GIT_VERSION_INFO_LIST 0 ZSQTLIB_GIT_TAG)
    set(ZSQTLIB_GIT_COMMIT_HASH "")
elseif (versionInfoListLen EQUAL 3)
    list(GET ZSQTLIB_GIT_VERSION_INFO_LIST 0 ZSQTLIB_GIT_TAG)
    list(GET ZSQTLIB_GIT_VERSION_INFO_LIST 2 ZSQTLIB_GIT_COMMIT_HASH)
else()
    # Abort CMake if version info does not follow the expected format either
    # with just one section or with 3 sections.
    message(FATAL_ERROR "Cannot create version file for ZSQtLib as ${ZSQTLIB_GIT_VERSION_INFO} has wrong format")
endif()

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

message(STATUS "ZSQTLIB_VERSION_MAJOR:    ${ZSQTLIB_VERSION_MAJOR}")
message(STATUS "ZSQTLIB_VERSION_MINOR:    ${ZSQTLIB_VERSION_MINOR}")
message(STATUS "ZSQTLIB_VERSION_PATCH:    ${ZSQTLIB_VERSION_PATCH}")
message(STATUS "ZSQTLIB_VERSION:          ${ZSQTLIB_VERSION}")

message(STATUS "")
message(STATUS "<- ZSQtLib/Make/ZSQtLibsGetGitVersionInfo.cmake")
message(STATUS "---------------------------------------------------------------------------------------------")
message(STATUS "")
