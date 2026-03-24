cmake_minimum_required(VERSION 3.15...4.2)
add_library(Bob)

file(GLOB_RECURSE HEADER_LIST 
    CONFIGURE_DEPENDS "src/*.h")
file(GLOB_RECURSE CPP_LIST 
    CONFIGURE_DEPENDS "src/*.cpp")
target_sources(Bob PRIVATE ${CPP_LIST} ${HEADER_LIST})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "" FILES ${CPP_LIST} ${HEADER_LIST})

target_include_directories(Bob PUBLIC src)
