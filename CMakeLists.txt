# Set the cmake minimum version
cmake_minimum_required(VERSION 3.10)

# Define the project name
project(PolynomialMaster C)

# Set the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Find the SDL2, SDL2_image, and SDL2_ttf packages
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Include the directories for SDL2, SDL2_image, and SDL2_ttf
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

# Add all header files under the include directory
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add all C source files under the src directory
file(GLOB SOURCES "src/*.c")

# Add the executable
add_executable(PolynomialMaster ${SOURCES})

# Link the SDL2, SDL2_image, SDL2_ttf, and math libraries
# target_link_libraries(PolynomialMaster ${SDL2_LIBRARIES} SDL2::SDL2_image SDL2::SDL2_ttf m)
target_link_libraries(PolynomialMaster SDL2::SDL2 SDL2_ttf::SDL2_ttf SDL2_image::SDL2_image m)

# Copy assets
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})





# # Add SDL2 CMake modules
# list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)


# # Add compiler errors/warnings flags
# target_compile_options(${PROJECT_NAME} PRIVATE $<$<C_COMPILER_ID:MSVC>:/W4 /WX>)
# target_compile_options(${PROJECT_NAME} PRIVATE $<$<NOT:$<C_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>)


# # Add SDL2 library
# find_package(SDL2 REQUIRED)
# target_link_libraries(${PROJECT_NAME} SDL2::Main)

# # Add SDL2_image library
# find_package(SDL2_image REQUIRED)
# target_link_libraries(${PROJECT_NAME} SDL2::Image)

# Add SDL2_ttf library
#find_package(SDL2_ttf REQUIRED)
#target_link_libraries(${PROJECT_NAME} SDL2::TTF)

# Add SDL2_net library
#find_package(SDL2_net REQUIRED)
#target_link_libraries(${PROJECT_NAME} SDL2::Net)

# Add SDL2_mixer library
#find_package(SDL2_mixer REQUIRED)
#target_link_libraries(${PROJECT_NAME} SDL2::Mixer)

# # Add SDL2_gfx library
# find_package(SDL2_gfx REQUIRED)
# target_link_libraries(${PROJECT_NAME} SDL2::GFX)

