cmake_minimum_required(VERSION 3.15...4.2)
add_executable(Caduq)

file(GLOB_RECURSE HEADER_LIST 
    CONFIGURE_DEPENDS "src/*.h")
file(GLOB_RECURSE CPP_LIST 
    CONFIGURE_DEPENDS "src/*.cpp")

target_sources(Caduq
    PRIVATE
        ${CPP_LIST}
        ${HEADER_LIST}
)

target_include_directories(Caduq PRIVATE src)
target_precompile_headers(Caduq PRIVATE src/cqpch.h)

target_link_libraries(Caduq PRIVATE Vizir Geotrup Bob)
