set(EIGEN_DIR "${PROJECT_SOURCE_DIR}/libraries/eigen" PARENT_SCOPE)
set(STB_DIR "${PROJECT_SOURCE_DIR}/libraries/stb" PARENT_SCOPE)

set(GLFW_DIR "${PROJECT_SOURCE_DIR}/libraries/glfw")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
set(GLFW_VULKAN_STATIC OFF CACHE INTERNAL "Use the Vulkan loader statically linked into application")
add_subdirectory(${GLFW_DIR})
set(GLFW_LIBRARY "glfw" PARENT_SCOPE)

if(WIN32)
    set(Vulkan_LIBRARY "${PROJECT_SOURCE_DIR}/libraries/vulkan/windows/Lib32/vulkan-1.lib" PARENT_SCOPE)
    set(Vulkan_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/libraries/vulkan/windows/Include" PARENT_SCOPE)
elseif(APPLE)
    set(VULKAN_SDK "${PROJECT_SOURCE_DIR}/libraries/vulkan/macOS" PARENT_SCOPE)
    set(Vulkan_LIBRARY "${PROJECT_SOURCE_DIR}/libraries/vulkan/macOS/macOS/lib/libvulkan.1.dylib" PARENT_SCOPE)
    set(Vulkan_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/libraries/vulkan/macOS/macOS/include" PARENT_SCOPE)
endif()


