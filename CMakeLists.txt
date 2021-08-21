cmake_minimum_required(VERSION 3.5)

project(maeparser)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

include(CMakePackageConfigHelpers)
include(set_project_version)

set_project_version(1.2.4)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Options
option(MAEPARSER_RIGOROUS_BUILD "Abort the build if the compiler issues any warnings" OFF )
option(MAEPARSER_BUILD_TESTS "Whether test executables should be built" ON)
option(MAEPARSER_BUILD_SHARED_LIBS "Build maeparser as a shared library (turn off for a static one)" ON)

if(MSVC)
    # C4251 disables warnings for export STL containers as arguments (returning a vector of things)
    add_compile_options(/wd4251 /wd4275 /wd4996 /D_SCL_SECURE_NO_WARNINGS /D_CRT_SECURE_NO_WARNINGS)

    set(CMAKE_INSTALL_BINDIR bin)
    set(CMAKE_INSTALL_LIBDIR lib)
    set(CMAKE_INSTALL_INCLUDEDIR include)
else()
    include(GNUInstallDirs)
endif()

if(MAEPARSER_RIGOROUS_BUILD)
    if(MSVC)
        add_compile_options(/WX)
    else()
        add_compile_options(-Wall -Wextra -Werror)
    endif()
endif()

# Source files & headers
file(GLOB maeparser_sources "*.cpp")

# Build Targets & Configuration
if(MAEPARSER_BUILD_SHARED_LIBS)
    add_library(maeparser SHARED ${maeparser_sources})
    target_compile_definitions(maeparser PRIVATE "IN_MAEPARSER")
    target_link_libraries(maeparser PUBLIC Boost::dynamic_linking)
    set_property(TARGET maeparser PROPERTY CXX_VISIBILITY_PRESET "hidden")
else()
    add_library(maeparser STATIC ${maeparser_sources})
    target_compile_definitions(maeparser PUBLIC "STATIC_MAEPARSER")
    if(MSVC)
        set(Boost_USE_STATIC_LIBS ON)
    endif()
endif()

# Prevent from trying to use the "lib" prefix on windows.
# We want to propagate it to targets down the road.
if(MSVC)
    target_link_libraries(maeparser PUBLIC Boost::disable_autolinking)
endif()

find_package(Boost COMPONENTS iostreams REQUIRED)
target_link_libraries(maeparser PUBLIC Boost::iostreams)

# If boost has the zlib component, then iostreams will probably
# depend on it, so try it first before resorting to ZLIB.
find_package(Boost COMPONENTS zlib)
if(Boost_zlib_FOUND)
    message(STATUS "Using Boost::zlib for iostreams dependency.")
    target_link_libraries(maeparser PUBLIC Boost::zlib)
else()
    find_package(ZLIB REQUIRED)
    message(STATUS "Using ZLIB::ZLIB library for iostreams dependency.")
    target_link_libraries(maeparser PUBLIC ZLIB::ZLIB)
endif()

target_include_directories(maeparser PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

set_target_properties(maeparser
    PROPERTIES
       VERSION ${maeparser_VERSION}
       SOVERSION ${maeparser_VERSION_MAJOR}
       INTERFACE_maeparser_MAJOR_VERSION ${maeparser_VERSION_MAJOR}
       COMPATIBLE_INTERFACE_STRING ${maeparser_VERSION_MAJOR}
)

# Install configuration
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/maeparser-config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/maeparser-config-version.cmake"
    VERSION ${maeparser_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(TARGETS maeparser
    EXPORT maeparser-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

INSTALL(EXPORT maeparser-targets
    FILE maeparser-targets.cmake
    NAMESPACE maeparser::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake)

file(GLOB mae_headers "*.hpp")
install(FILES ${mae_headers} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/maeparser)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/maeparser-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/maeparser-config-version.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake
)

# Tests
if (MAEPARSER_BUILD_TESTS)
    set(MEMORYCHECK_COMMAND_OPTIONS "${MEMORYCHECK_COMMAND_OPTIONS} --tool=memcheck \
            --time-stamp=yes --num-callers=20 --gen-suppressions=all --leak-check=full \
            --show-reachable=yes --trace-children=yes --error-exitcode=29")

    include(CTest)
    add_subdirectory(test)
endif()

