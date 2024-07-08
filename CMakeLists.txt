cmake_minimum_required(VERSION 3.10)

set(CMAKE_BUILD_TYPE Debug)

project(trying_space_shooter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



find_package(OpenGL REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(Stb REQUIRED)



file(GLOB PROJECT_SOURCES "src/*")

add_executable(trying_space_shooter ${PROJECT_SOURCES})

target_link_libraries(trying_space_shooter PRIVATE OpenGL::GL)
target_link_libraries(trying_space_shooter PRIVATE glfw)
target_link_libraries(trying_space_shooter PRIVATE glad::glad)
target_link_libraries(trying_space_shooter PRIVATE glm::glm-header-only)
target_link_libraries(trying_space_shooter PRIVATE assimp::assimp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

