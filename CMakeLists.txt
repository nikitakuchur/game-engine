cmake_minimum_required(VERSION 3.16)
project(roguelike C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

file(GLOB SOURCES src/*.c)
file(GLOB ECS_SOURCES src/ecs/*.c)
file(GLOB COMPONENTS_SOURCES src/ecs/components/*.c)
file(GLOB SYSTEMS_SOURCES src/ecs/systems/*.c)
file(GLOB MATH_SOURCES src/math/*.c)
file(GLOB MODELS_SOURCES src/mesh/*.c)
file(GLOB PRIMITIVES_SOURCES src/primitives/*.c)
file(GLOB UTIL_SOURCES src/utils/*.c)
add_executable(
        ${CMAKE_PROJECT_NAME}
        ${SOURCES}
        ${ECS_SOURCES}
        ${COMPONENTS_SOURCES}
        ${SYSTEMS_SOURCES}
        ${MATH_SOURCES}
        ${MODELS_SOURCES}
        ${PRIMITIVES_SOURCES}
        ${UTIL_SOURCES}
)

add_subdirectory(deps/glad)
target_link_libraries(${CMAKE_PROJECT_NAME} glad)

add_subdirectory(deps/glfw)
target_link_libraries(${CMAKE_PROJECT_NAME} glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} OpenGL::GL)

add_subdirectory(deps/stb_image)
target_link_libraries(${CMAKE_PROJECT_NAME} stb_image)
