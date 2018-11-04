cmake_minimum_required(VERSION 3.9)
project(lwirth)

set(lwirth_VERSION_MAJOR 1)
set(lwirth_VERSION_MINOR 0)
set(lwirth_VERSION_PATCH 0)

add_subdirectory(libraries)
find_package(vulkan REQUIRED)

# configure_file(${PROJECT_SOURCE_DIR}/lwirth.pc.in ${PROJECT_BINARY_DIR}/lwirth.pc) #or .h better?

set(CMAKE_CXX_STANDARD 17)


include_directories(${PROJECT_SOURCE_DIR}/lwirth-lib)
add_subdirectory(lwirth-lib)

include_directories(${PROJECT_SOURCE_DIR}/lwirth-testing)
add_subdirectory(lwirth-testing)

# for executing: export VK_ICD_FILENAMES=$VULKAN_SDK/etc/vulkan/icd.d/MoltenVK_icd.json
#./vulkaninfo
