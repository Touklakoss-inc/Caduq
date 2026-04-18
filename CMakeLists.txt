cmake_minimum_required(VERSION 3.15...4.2)

project(Caduq VERSION 0.0.4
              DESCRIPTION "Simple CAD software"
              LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

function(add_ext_subdirectory path)
  add_subdirectory(${path})
  foreach(target IN LISTS ARGN)
        if(TARGET ${target})
        get_target_property(folder ${target} FOLDER)
        if(NOT (${folder} STREQUAL "folder-NOTFOUND"))
            set_target_properties(${target} PROPERTIES FOLDER "ext/${folder}")
        else()
            set_target_properties(${target} PROPERTIES FOLDER "ext")
        endif()
    endif()
  endforeach()
endfunction()

# Vendors
add_ext_subdirectory(Vizir/vendor/glad glad)
add_ext_subdirectory(Vizir/vendor/GLFW glfw uninstall update_mappings)
add_ext_subdirectory(Vizir/vendor/imgui imgui)
add_ext_subdirectory(Vizir/vendor/stb_image stb_image)
add_ext_subdirectory(Vizir/vendor/tracy TracyClient)

# My projects
add_subdirectory(Vizir)
add_subdirectory(Geotrup)
add_subdirectory(Bob)
add_subdirectory(Caduq)


if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(glad      PRIVATE -w)
    target_compile_options(glfw      PRIVATE -w)
    target_compile_options(imgui     PRIVATE -w)
    target_compile_options(stb_image PRIVATE -w)
elseif (MSVC)
    target_compile_options(glad      PRIVATE /W0)
    target_compile_options(glfw      PRIVATE /W0)
    target_compile_options(imgui     PRIVATE /W0)
    target_compile_options(stb_image PRIVATE /W0)
endif()

target_link_libraries(Caduq 
    PRIVATE glad glfw imgui stb_image Tracy::TracyClient
)
