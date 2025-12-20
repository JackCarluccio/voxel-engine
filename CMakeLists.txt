cmake_minimum_required(VERSION 3.20)

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

# Download GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG master
)
FetchContent_MakeAvailable(glm)

# Local GLAD (vendored)
add_library(glad STATIC
    external/glad/src/gl.c
)

target_include_directories(glad PUBLIC
    external/glad/include
)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Add executable
add_executable(VoxelEngine 
    src/main.cpp

    "src/engine/Input.cpp"
    
    "src/graphics/Camera.cpp"
    "src/graphics/ShaderProgram.cpp"
    "src/graphics/VAO.cpp"
    "src/graphics/VBO.cpp"
    "src/graphics/EBO.cpp"
    "src/graphics/ChunkMesh.cpp"
    "src/graphics/Window.cpp"
    "src/graphics/Renderer.cpp"
    
    "src/world/biomes/Biomes.cpp"
    "src/world/blocks/Blocks.cpp"
    "src/world/chunks/ChunkManager.cpp"
    "src/world/chunks/ChunkScheduler.cpp"
    "src/world/generation/Generator.cpp"
    "src/world/generation/PerlinNoise.cpp"
    "src/world/generation/Splines.cpp"

    "src/util/IndexableMinHeap.cpp")

# Include directories
target_include_directories(VoxelEngine 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${glfw_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/glad/include
)

# Link libraries
target_link_libraries(VoxelEngine 
    PRIVATE 
    OpenGL::GL 
    glfw 
    glad 
    glm
)

add_custom_command(TARGET VoxelEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:VoxelEngine>/assets
)

add_custom_command(TARGET VoxelEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/shaders
            $<TARGET_FILE_DIR:VoxelEngine>/shaders
)