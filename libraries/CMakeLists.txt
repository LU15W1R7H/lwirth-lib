set(EIGEN_DIR ${PROJECT_SOURCE_DIR}/libraries/eigen)

set(GLFW_DIR ${PROJECT_SOURCE_DIR}/libraries/glfw)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

if(WIN32)
    set(Vulkan_LIBRARY ${PROJECT_SOURCE_DIR}/libraries/vulkan/windows/Lib32/vulkan-1.lib)
    set(Vulkan_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/libraries/vulkan/windows/Include)
elseif(APPLE)
    set(VULKAN_SDK ${PROJECT_SOURCE_DIR}/libraries/vulkan/macOS)
    set(Vulkan_LIBRARY ${PROJECT_SOURCE_DIR}/libraries/vulkan/macOS/macOS/lib/libvulkan.1.dylib)
    set(Vulkan_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/libraries/vulkan/macOS/macOS/include)
endif()

add_subdirectory(${GLFW_DIR})
include_directories(${GLFW_DIR}/include)

include_directories(${EIGEN_DIR}/include)

include_directories(${STB_DIR}/include)