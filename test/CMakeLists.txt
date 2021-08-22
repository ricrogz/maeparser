add_executable(unittest MainTestSuite.cpp BufferTest.cpp MaeBlockTest.cpp MaeParserTest.cpp
ReaderTest.cpp WriterTest.cpp UsageDemo.cpp)

if(NOT MAEPARSER_BUILD_SHARED_LIBS)
    set(Boost_USE_STATIC_LIBS ON)
endif()

find_package(Boost COMPONENTS filesystem unit_test_framework REQUIRED)

get_filename_component(TEST_SAMPLES_PATH ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE)
target_compile_definitions(unittest PRIVATE "TEST_SAMPLES_PATH=\"${TEST_SAMPLES_PATH}\"")

target_link_libraries(unittest PRIVATE maeparser Boost::filesystem Boost::unit_test_framework)

add_test(NAME unittest COMMAND ${CMAKE_CURRENT_BINARY_DIR}/unittest
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

if(MSVC AND MAEPARSER_BUILD_SHARED_LIBS)
    file(TO_NATIVE_PATH "$<TARGET_FILE_DIR:maeparser>" DLL_PATH_ENV)
    set(DLL_PATH_ENV "${DLL_PATH_ENV};$ENV{PATH}")
    string(REPLACE ";" "\\;" DLL_PATH_ENV "${DLL_PATH_ENV}")
    set_tests_properties(unittest PROPERTIES ENVIRONMENT "PATH=${DLL_PATH_ENV}")
endif()
