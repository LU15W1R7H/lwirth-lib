#project()

find_package(vulkan REQUIRED)

add_executable(lwirth-testing Main.cpp tests/UtilsTests.hpp)
target_link_libraries(lwirth-testing lwirth-lib)
target_link_libraries(lwirth-testing Vulkan::Vulkan)
target_link_libraries(lwirth-testing glfw)

install(TARGETS lwirth-testing DESTINATION bin)
install(FILES ${PROJECT_BINARY_DIR}/lwirth-testing-config.hpp DESTINATION include)