cmake_minimum_required(VERSION 3.15...4.2)

project(Caduq VERSION 0.0.4
              DESCRIPTION "Simple CAD software"
              LANGUAGES CXX)

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
add_ext_subdirectory(Vizir/vendor/glm glm)
add_ext_subdirectory(Vizir/vendor/GLFW glfw uninstall update_mappings)
add_ext_subdirectory(Vizir/vendor/imgui imgui)
add_ext_subdirectory(Vizir/vendor/stb_image stb_image)
add_ext_subdirectory(Vizir/vendor/tracy TracyClient)

# My projects
add_subdirectory(Vizir)
add_subdirectory(Geotrup)
add_subdirectory(Bob)
add_subdirectory(Caduq)

target_include_directories(Geotrup PUBLIC Vizir/vendor/Eigen)
target_include_directories(Caduq PUBLIC Vizir/vendor/Eigen)
target_include_directories(Bob PUBLIC Vizir/vendor/spdlog/include)

target_link_libraries(Caduq 
    PRIVATE glad glm glfw imgui stb_image Tracy::TracyClient
)
