file(GLOB_RECURSE lwirth-lib_SRC
        "*.hpp"
        "*.cpp")

add_library(lwirth-lib STATIC ${lwirth-lib_SRC})

target_include_directories(lwirth-lib PUBLIC ${GLFW_DIR}/include)
target_include_directories(lwirth-lib PUBLIC ${Eigen_INCLUDE_DIR})
target_include_directories(lwirth-lib PUBLIC ${STB_DIR}/include)

target_link_libraries(lwirth-lib Vulkan::Vulkan)
target_link_libraries(lwirth-lib glfw)

install(TARGETS lwirth-lib DESTINATION bin)
install(FILES lwirth.hpp DESTINATION include)