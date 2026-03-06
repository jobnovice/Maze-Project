cmake_minimum_required(VERSION 3.10)
project(SDLTutorials)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 1. Hint for Homebrew on Apple Silicon (M1/M2/M3)
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
# Mac Framework Path: Tells CMake to look in your Library for the .framework files
set(CMAKE_INSTALL_RPATH "/opt/homebrew/lib")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
# Output directory (relative to project root)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

# 1. Find SDL2 and extensions (Added SDL2_ttf here)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Get all .cpp files
file(GLOB SOURCE_FILES "*.cpp")

# Create one executable per source file
foreach(SRC_FILE ${SOURCE_FILES})
    get_filename_component(EXEC_NAME ${SRC_FILE} NAME_WE)
    
    add_executable(${EXEC_NAME} ${SRC_FILE})
    file(COPY ${CMAKE_SOURCE_DIR}/Pics DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    
    # 2. Link libraries (Added SDL2_ttf::SDL2_ttf here)
    target_link_libraries(${EXEC_NAME} 
        SDL2::SDL2 
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf
    )
    
    # 3. Include directories (Added SDL2_TTF_INCLUDE_DIRS here)
    target_include_directories(${EXEC_NAME} PRIVATE
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
    )
endforeach()