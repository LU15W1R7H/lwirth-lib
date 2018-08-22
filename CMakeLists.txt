cmake_minimum_required(VERSION 3.9)

set(Vulkan_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/libraries/vulkan/macOS/macOS/lib)
set(Vulkan_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libraries/vulkan/macOS/macOS/include)

set(GLFW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libraries/glfw)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${GLFW_DIR})

set(lwirth_VERSION_MAJOR 1)
set(lwirth_VERSION_MINOR 0)
set(lwirth_VERSION_PATCH 0)

project(lwirth VERSION ${lwirth_VERSION_MAJOR}.${lwirth_VERSION_MINOR}.${lwirth_VERSION_PATCH} DESCRIPTION "lwirth-lib")

# configure_file(${PROJECT_SOURCE_DIR}/lwirth.pc.in ${PROJECT_BINARY_DIR}/lwirth.pc) #or .h better?


set(CMAKE_CXX_STANDARD 17)

include_directories(libraries/eigen/include)
include_directories(libraries/glfw/include)
include_directories(libraries/stb)
include_directories(libraries/vulkan/include)

include_directories(${PROJECT_SOURCE_DIR}/lwirth-lib)
add_subdirectory(lwirth-lib)

include_directories(${PROJECT_SOURCE_DIR}/lwirth-testing)
add_subdirectory(lwirth-testing)

# for executing: export VK_ICD_FILENAMES=$VULKAN_SDK/etc/vulkan/icd.d/MoltenVK_icd.json
#./vulkaninfo
