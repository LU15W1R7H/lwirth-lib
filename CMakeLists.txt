cmake_minimum_required(VERSION 3.9)

project(lwirth VERSION 0.0.0 DESCRIPTION "lwirth-lib")

set(lwirth_VERSION_MAJOR 0)
set(lwirth_VERSION_MINOR 1)
set(lwirth_VERSION PATCH 0)

# configure_file(${PROJECT_SOURCE_DIR}/lwirth.pc.in ${PROJECT_BINARY_DIR}/lwirth.pc) #or .h better?

# include_directories(${PROJECT_BINARY_DIR})

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/lwirth-lib)
add_subdirectory(lwirth-lib)

include_directories(${PROJECT_SOURCE_DIR}/lwirth-testing)
add_subdirectory(lwirth-testing)

#set_target_properties(lwirth PROPERTIES
#        VERSION ${PROJECT_VERSION}
#        SOVERSION 1
#        PUBLIC_HEADER lwirth.hpp)
#target_include_directories(lwirth PRIVATE .)

