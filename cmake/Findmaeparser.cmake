# Try to find the CMake configuration for Schrodinger's MaeParser library.
#
# If the package is found, This will set up the maeparser::maeparser target
# and the following variables:
#
# maeparser_ROOT           - absolute path to maeparser's top directory
#
# maeparser_INCLUDE_DIRS  - maeparser's includes directory
# maeparser_LIBRARIES     - maeparser's shared libraries
#
# maeparser_VERSION       - maeparser's version string
# maeparser_VERSION_MAJOR - maeparser's major version number
# maeparser_VERSION_MINOR - maeparser's minor version number
# maeparser_VERSION_PATCH - maeparser's patch version number
#

include(FindPackageHandleStandardArgs)

find_package(maeparser NO_MODULE
    PATHS ${maeparser_DIR} ${MAEPARSER_DIR} ${maeparser_ROOT} ${MAEPARSER_ROOT}
    PATH_SUFFIXES "lib/cmake" "lib64/cmake")

find_path(maeparser_INCLUDE_DIRS
    NAMES "maeparser/Reader.hpp"
    PATHS ${maeparser_ROOT}
    PATH_SUFFIXES "include"
    DOC "include path for maeparser")

find_library(maeparser_LIBRARIES
    NAMES maeparser
    PATHS ${maeparser_ROOT}
    PATH_SUFFIXES "lib" "lib64"
    DOC "libraries for maeparser")

find_package_handle_standard_args(maeparser
    REQUIRED_VARS maeparser_INCLUDE_DIRS maeparser_LIBRARIES
    VERSION_VAR maeparser_VERSION)

if (maeparser-FOUND AND (NOT TARGET maeparser::maeparser))
    add_library(maeparser INTERFACE)
    add_library(maeparser::maeparser ALIAS maeparser)

    target_include_directories(maeparser INTERFACE
    $<BUILD_INTERFACE:${maeparser_INCLUDE_DIRS}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

    target_link_libraries(maeparser INTERFACE ${maeparser_LIBRARIES})
endif()
