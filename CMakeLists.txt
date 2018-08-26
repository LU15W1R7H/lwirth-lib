cmake_minimum_required(VERSION 3.9)
project(lwirth)

set(lwirth_VERSION_MAJOR 1)
set(lwirth_VERSION_MINOR 0)
set(lwirth_VERSION_PATCH 0)

set(EIGEN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libraries/eigen)

set(GLFW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libraries/glfw)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

if(WIN32)
    set(Vulkan_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/libraries/vulkan/windows/Lib32)
    set(Vulkan_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libraries/vulkan/windows/Include)
elseif(UNIX AND APPLE)
    set(Vulkan_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/libraries/vulkan/macOS/macOS/lib)
    set(Vulkan_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libraries/vulkan/macOS/macOS/include)
endif()

add_subdirectory(${GLFW_DIR})
include_directories(${GLFW_DIR}/include)

include_directories(${EIGEN_DIR}/include)

include_directories(${STB_DIR}/include)


# configure_file(${PROJECT_SOURCE_DIR}/lwirth.pc.in ${PROJECT_BINARY_DIR}/lwirth.pc) #or .h better?

set(CMAKE_CXX_STANDARD 17)


include_directories(${PROJECT_SOURCE_DIR}/lwirth-lib)
add_subdirectory(lwirth-lib)

include_directories(${PROJECT_SOURCE_DIR}/lwirth-testing)
add_subdirectory(lwirth-testing)

# for executing: export VK_ICD_FILENAMES=$VULKAN_SDK/etc/vulkan/icd.d/MoltenVK_icd.json
#./vulkaninfo
