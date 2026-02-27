cmake_minimum_required(VERSION 3.15...4.2)
add_library(Geotrup)

file(GLOB_RECURSE HEADER_LIST 
    CONFIGURE_DEPENDS 
        "src/Geometry/*.h"
        "src/XPBD/*.h"
        "src/Tools/*.h"
)
file(GLOB_RECURSE CPP_LIST 
    CONFIGURE_DEPENDS  
        "src/Geometry/*.cpp"
        "src/XPBD/*.cpp"
        "src/Tools/*.cpp"
)
target_sources(Geotrup
    PRIVATE
        ${CPP_LIST}
    PUBLIC
        ${HEADER_LIST}
)
target_include_directories(Geotrup PUBLIC src)

target_link_libraries(Geotrup PRIVATE imgui)
