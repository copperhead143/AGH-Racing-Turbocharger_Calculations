cmake_minimum_required(VERSION 3.16)
project(turbo_calc_app VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTING "Build tests" ON)

# add lib

#add exec

if(BUILD_TESTING)
    enable_testing()
    add_executable(turbocalc_tests tests/test_basic.cpp)
    target_link_libraries(turbocalc_tests PRIVATE turbocalc)
    add_test(NAME turbocalc_tests COMMAND turbocalc_tests)
endif()
