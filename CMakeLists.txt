cmake_minimum_required(VERSION 3.10)

# Project Name
project(VoxelEngine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# FetchContent allows us to download and include external dependencies automatically
include(FetchContent)

# Download GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8  # Change this to the latest stable version if needed
)
FetchContent_MakeAvailable(glfw)

# Download GLAD
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v0.1.36
)
FetchContent_MakeAvailable(glad)

# Ensure that OpenGL functions are properly linked
find_package(OpenGL REQUIRED)

# Add the source files **before linking**
add_executable(VoxelEngine src/main.cpp "include/shaderProgram.h" "src/shaderProgram.cpp" "include/VAO.h" "src/VAO.cpp" "include/VBO.h" "src/VBO.cpp" "include/EBO.h" "src/EBO.cpp" "include/Mesh.h" "src/Mesh.cpp")

# Include directories for GLFW and GLAD
target_include_directories(VoxelEngine PRIVATE ${glfw_SOURCE_DIR}/include ${glad_SOURCE_DIR}/include)

# Link OpenGL, GLFW, and GLAD to our executable
target_link_libraries(VoxelEngine PRIVATE OpenGL::GL glfw glad)

include_directories(${CMAKE_SOURCE_DIR}/include)