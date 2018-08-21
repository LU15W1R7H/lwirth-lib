#project()

ADD_EXECUTABLE(lwirth-testing Main.cpp)
TARGET_LINK_LIBRARIES(lwirth-testing lwirth-lib)

install(TARGETS lwirth-testing DESTINATION bin)
install(FILES ${PROJECT_BINARY_DIR}/lwirth-testing-config.hpp DESTINATION include)