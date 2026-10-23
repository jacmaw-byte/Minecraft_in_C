cmake_minimum_required(VERSION 4.2)

project(Minecraft_in_C C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(Minecraft_in_C
        src/main.c
        src/glad.c
        src/chunk_mesh.c
        src/camera.c
        src/chunk.c
        src/noise/value_noise.c
        src/chunk_manager.c
        src/chunk_map.c
        src/initialisers.c
        src/dda.c
        src/mesh.c
        src/player/destroy_block.c
        src/player/place_block.c
        src/glfw_stuff.c
)

target_include_directories(Minecraft_in_C PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

find_package(Freetype REQUIRED)
find_package(PNG REQUIRED)
find_package(ZLIB REQUIRED)

target_link_libraries(Minecraft_in_C PRIVATE
        glfw
        OpenGL::GL
        m
        Freetype::Freetype
        PNG::PNG
        ZLIB::ZLIB
)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(Minecraft_in_C PRIVATE
            $<$<CONFIG:Release>:-O3>
            $<$<CONFIG:Release>:-march=native>
            $<$<CONFIG:Release>:-flto>
    )
endif()