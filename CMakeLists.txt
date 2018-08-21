cmake_minimum_required(VERSION 3.9)

project(lwirth VERSION 1.0.0 DESCRIPTION "lwirth-lib")

set(lwirth_VERSION_MAJOR 0)
set(lwirth_VERSION_MINOR 0)
set(lwirth_VERSION PATCH 0)

configure_file(${PROJECT_SOURCE_DIR}/lwirth.pc.in ${PROJECT_BINARY_DIR}/lwirth.pc) #or .h better?

include_directories(${PROJECT_BINARY_DIR})

include_directories(${PROJECT_SOURCE_DIR}/lwirth-lib)
add_subdirectory(lwirth-lib)

set(CMAKE_CXX_STANDARD 17)

add_library(lwirth-lib *.cpp *.hpp)
set_target_properties(lwirth PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION 1
        PUBLIC_HEADER lwirth.hpp)
target_include_directories(lwirth PRIVATE .)
install(TARGETS lwirth
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${CMAKE_BINARY_DIR}/lwirth.pc
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
