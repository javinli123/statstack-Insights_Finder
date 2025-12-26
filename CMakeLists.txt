cmake_minimum_required(VERSION 3.10)
project(Insights_Finder)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

# Install LibCurl
# This tells CMake to look in "./libs/curl" first, then look elsewhere
set(CURL_ROOT_PATH "${CMAKE_SOURCE_DIR}/libs/curl")
list(APPEND CMAKE_PREFIX_PATH "$CURL_ROOT_PATH")
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

# Install Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Collect sources/headers.
file(GLOB_RECURSE PROJECT_HEADERS CONFIGURE_DEPENDS
        "${CMAKE_SOURCE_DIR}/src/*.h"
        "${CMAKE_SOURCE_DIR}/src/*.hpp") # for nlohmann/json.hpp
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Add executable target with sources; headers are listed for IDE visibility
add_executable(statstack ${PROJECT_SOURCES} ${PROJECT_HEADERS} main.cpp)

# This allows items outside src (main.cpp) to access /src/ files such as all the object classes
target_include_directories(statstack PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Link libcurl to app
target_link_libraries(statstack PRIVATE ${CURL_LIBRARIES})



# Compile tests

# Pick up all .cpp files under tests/
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/tests/*.cpp")
add_executable(run_tests
        test_main.cpp
        ${TEST_SOURCES}
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
)

# Link tests with ./src
target_include_directories(run_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Link tests with gtest
target_sources(run_tests PRIVATE ${STUDENT_SRCS})
target_link_libraries(run_tests
        PRIVATE
        GTest::GTest
        GTest::Main
        pthread
        ${CURL_LIBRARIES} # for test runner to resolve CURL calls
)

add_test(NAME runtests COMMAND run_tests --gtest_brief=0)