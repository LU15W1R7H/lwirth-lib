file(GLOB_RECURSE lwirth-testing_SRC
        "*.hpp"
        "*.cpp")


add_executable(lwirth-testing ${lwirth-testing_SRC})
target_link_libraries(lwirth-testing lwirth-lib)
target_link_libraries(lwirth-testing Vulkan::Vulkan)
target_link_libraries(lwirth-testing glfw)

install(TARGETS lwirth-testing DESTINATION bin)
#install(FILES ${PROJECT_BINARY_DIR}/lwirth-testing-config.hpp DESTINATION include)