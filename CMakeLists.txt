cmake_minimum_required(VERSION 3.10)

project(trying_space_shooter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm REQUIRED)
find_package(assimp CONFIG REQUIRED)



file(GLOB_RECURSE PROJECT_SOURCES "src/*.hpp" "src/*.cpp")

# set(PROJECT_SOURCES "src/classes" "src/headers" "src")

add_executable(trying_space_shooter ${PROJECT_SOURCES})

target_link_libraries(trying_space_shooter PRIVATE OpenGL::GL)
target_link_libraries(trying_space_shooter PRIVATE glfw)
target_link_libraries(trying_space_shooter PRIVATE glad::glad)
target_link_libraries(trying_space_shooter PRIVATE glm::glm-header-only)
target_link_libraries(trying_space_shooter PRIVATE assimp::assimp)

target_include_directories(trying_space_shooter PUBLIC "src/" "src/headers/" "src/classes/")


add_custom_command(
        TARGET trying_space_shooter POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/src/models/
                ${CMAKE_CURRENT_BINARY_DIR}/models/)
add_custom_command(
    TARGET trying_space_shooter POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/src/shaders/
            ${CMAKE_CURRENT_BINARY_DIR}/shaders/)
                
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

