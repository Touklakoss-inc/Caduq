cmake_minimum_required(VERSION 3.15...4.2)
add_library(Bob)

file(GLOB_RECURSE HEADER_LIST 
    CONFIGURE_DEPENDS 
        "src/*.h"
)
file(GLOB_RECURSE CPP_LIST 
    CONFIGURE_DEPENDS 
    "src/*.cpp"
)
target_sources(Bob
    PRIVATE
        ${CPP_LIST}
    PUBLIC
        ${HEADER_LIST}
)
target_include_directories(Bob PUBLIC src)
