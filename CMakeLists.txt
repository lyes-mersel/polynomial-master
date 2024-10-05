# Set the cmake minimum version
cmake_minimum_required(VERSION 3.10)

# Define the project name
project(polynomial-master C)

# Set the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Find the SDL2, SDL2_image, and SDL2_ttf packages
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

# Include the directories for SDL2, SDL2_image, and SDL2_ttf
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

# Add all header files under the include directory
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add all C source files under the src directory
file(GLOB SOURCES "src/*.c")

# Add the executable
add_executable(polynomial-master ${SOURCES})

# Link the SDL2, SDL2_image, SDL2_ttf, and math libraries
target_link_libraries(polynomial-master SDL2::SDL2 SDL2_ttf::SDL2_ttf SDL2_image::SDL2_image m)

# Copy assets
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
