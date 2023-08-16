cmake_minimum_required(VERSION 3.12)

project(FastBT VERSION 1.0.0)

# Add the include directories
include_directories(include)

# Create a variable to store all the source files recursively
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Build the dynamic link library
add_library(FastBT SHARED ${SOURCES})

# Set the library version
set_target_properties(FastBT PROPERTIES VERSION ${PROJECT_VERSION})

#target_link_libraries(FastBT OTHER_LIBRARY)

# Install the library
install(TARGETS FastBT
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)