cmake_minimum_required(VERSION 3.10)
project(SDLTutorials)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directory (relative to project root)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

# Find SDL2 and extensions
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)  # Adjust based on your needs

# Get all .add_executable(09_the_viewport ...)cpp files in src/
file(GLOB SOURCE_FILES "*.cpp")

# Create one executable per source file
foreach(SRC_FILE ${SOURCE_FILES})
    # Get filename without extension (e.g., "03_event_driven_programming")
    get_filename_component(EXEC_NAME ${SRC_FILE} NAME_WE)
    
    # Create executable
    add_executable(${EXEC_NAME} ${SRC_FILE})
   file(COPY ${CMAKE_SOURCE_DIR}/Pics DESTINATION ${CMAKE_BINARY_DIR}) 
    # Link libraries
    target_link_libraries(${EXEC_NAME} 
        SDL2::SDL2 
        SDL2_image::SDL2_image  # Add others as needed
    )
    
    # Include directories
    target_include_directories(${EXEC_NAME} PRIVATE
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
    )
endforeach()
