cmake_minimum_required(VERSION 3.15...4.2)
add_library(Vizir)

set(_CRT_SECURE_NO_WARNING)
set(VZ_BUILD_DLL)
set(VZ_ENABLE_ASSERT)
target_compile_definitions(Vizir PUBLIC GLFW_INCLUDE_NONE=1)

file(GLOB_RECURSE HEADER_LIST 
    CONFIGURE_DEPENDS "src/*.h")
file(GLOB_RECURSE CPP_LIST 
    CONFIGURE_DEPENDS "src/*.cpp")
target_sources(Vizir
    PRIVATE
        ${CPP_LIST}
    PUBLIC
        ${HEADER_LIST}
)

target_include_directories(Vizir PUBLIC src src/Vizir)
target_precompile_headers(Vizir PRIVATE src/vzpch.h)
target_compile_options(Vizir PRIVATE -fpermissive)

target_link_libraries(Vizir PUBLIC Bob glm glfw imgui stb_image)
