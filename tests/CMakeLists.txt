# Find all source files in the 'tests' directory that start with 'lc'
file(GLOB TEST_SOURCES "tests/test_*.cpp")

# Add a target for the tests
add_executable(my_tests ${TEST_SOURCES})

target_include_directories(my_tests PUBLIC 
        ../
        /usr/local/include
        ${PROJECT_SOURCE_DIR}/FastBT/include
)

link_directories(/usr/local/lib)
# Link with Google Test library and other required libraries
target_link_libraries(my_tests libBackTesting gtest gtest_main pthread)

# Register the test executable with CTest
include(GoogleTest)
gtest_discover_tests(my_tests)