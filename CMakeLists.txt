cmake_minimum_required(VERSION 3.15...4.2)
# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_FLAGS -fpermissive)

project(Caduq VERSION 0.0.4
              DESCRIPTION "Simple CAD software"
              LANGUAGES CXX)

# Vendors
add_subdirectory(Vizir/vendor/glad)
add_subdirectory(Vizir/vendor/glm)
add_subdirectory(Vizir/vendor/GLFW)
add_subdirectory(Vizir/vendor/imgui)
add_subdirectory(Vizir/vendor/spdlog)
add_subdirectory(Vizir/vendor/stb_image)


# My projects
add_subdirectory(Vizir)
add_subdirectory(Geotrup)
add_subdirectory(Bob)
add_subdirectory(Caduq)

target_include_directories(Geotrup PUBLIC Vizir/vendor/Eigen)
target_include_directories(Caduq PUBLIC Vizir/vendor/Eigen)

target_link_libraries(Caduq 
    PRIVATE glad glm glfw imgui spdlog stb_image
)
