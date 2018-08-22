file(GLOB_RECURSE lwirth-lib_SRC
        "*.hpp"
        "*.cpp")

add_library(lwirth-lib STATIC ${lwirth-lib_SRC})

find_package(vulkan REQUIRED)
target_link_libraries(lwirth-lib Vulkan::Vulkan)
target_link_libraries(lwirth-lib glfw)

install(TARGETS lwirth-lib DESTINATION bin)
install(FILES lwirth.hpp DESTINATION include)