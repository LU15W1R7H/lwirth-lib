file(GLOB_RECURSE lwirth-lib_SRC
        "*.hpp"
        "*.cpp")

add_library(lwirth-lib STATIC ${lwirth-lib_SRC})

target_link_libraries(lwirth-lib glfw3.lib vulkan-1.lib)

install(TARGETS lwirth-lib DESTINATION bin)
install(FILES lwirth.hpp DESTINATION include)