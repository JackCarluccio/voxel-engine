cmake_minimum_required(VERSION 3.10)

# Project Name
project(VoxelEngine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# FetchContent allows us to download and include external dependencies
include(FetchContent)

# Download GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)
FetchContent_MakeAvailable(glfw)

# Download GLAD
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v0.1.36
)
FetchContent_MakeAvailable(glad)

# Download GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG master
)
FetchContent_MakeAvailable(glm)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Add executable
add_executable(VoxelEngine 
    src/main.cpp
    "src/graphics/ShaderProgram.cpp"
    "src/graphics/VAO.cpp"
    "src/graphics/VBO.cpp"
    "src/graphics/EBO.cpp"
    "src/graphics/Mesh.cpp"
)

# Include directories
target_include_directories(VoxelEngine 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${glfw_SOURCE_DIR}/include 
    ${glad_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries(VoxelEngine 
    PRIVATE 
    OpenGL::GL 
    glfw 
    glad 
    glm
)
