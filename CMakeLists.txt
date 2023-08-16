cmake_minimum_required(VERSION 3.0.0)
project(FastBT VERSION 0.1.0)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -stdlib=libc++")

include(CTest)

# Set the C++ compiler to clang++
#set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

include_directories(FastBT/include)

include_directories(googletest/googletest/include)

add_subdirectory(googletest)

add_subdirectory(FastBT)
# Add executable target for main application
add_executable(FastBTCMD main.cpp)
target_link_libraries(FastBTCMD FastBT)

# Find all source files in the 'tests' directory that start with 'lc'
file(GLOB TEST_SOURCES "tests/test_*.cpp")
add_executable(unittests ${TEST_SOURCES}
        tests/test_ohlc.cpp)


# Include directories for header files
target_include_directories(unittests PUBLIC
        /usr/local/include
        ./
)

# Link against Google Test library and other required libraries
target_link_libraries(unittests
        gtest
        gtest_main
        pthread
        FastBT
)
# Enable testing and define test target
enable_testing()
add_test(NAME mytest COMMAND unittests)

include(CPack)
