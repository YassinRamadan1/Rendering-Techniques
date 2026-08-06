cmake_minimum_required(VERSION 3.5)

project("graphics-effects")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(PRODUCTION_BUILD "Make this a production build" OFF)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION FALSE)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS  
    "${CMAKE_CURRENT_SOURCE_DIR}/common/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/common/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/techniques/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/app/*.cpp"
)

add_executable("${CMAKE_PROJECT_NAME}" ${SOURCES})

target_include_directories("${CMAKE_PROJECT_NAME}" PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/common/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/techniques"
    "${CMAKE_CURRENT_SOURCE_DIR}/app"
)

target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=0) 

target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC TECHNIQUES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/techniques/")
target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=0)

target_include_directories("${CMAKE_PROJECT_NAME}" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/GLAD/include")

target_include_directories("${CMAKE_PROJECT_NAME}" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/assimp/include")

target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/assimp/lib/assimp-vc143-mtd.lib")

target_include_directories("${CMAKE_PROJECT_NAME}" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/GLFW/include")

target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/GLFW/lib-vc2022/glfw3.lib")

add_subdirectory(thirdparty/GLM/glm)

target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE glm)