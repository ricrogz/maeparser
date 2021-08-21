cmake_minimum_required(VERSION 3.5)

project(maeparser_export_check)

if(MSVC)
    # C4251 disables warnings for export STL containers as arguments (returning a vector of things)
    add_compile_options(/wd4251 /wd4275 /wd4996 /D_SCL_SECURE_NO_WARNINGS /D_CRT_SECURE_NO_WARNINGS)
endif()

find_package(maeparser REQUIRED)

add_executable(maeparser_export_check main.cpp)

target_link_libraries(maeparser_export_check maeparser::maeparser)

