cmake_minimum_required(VERSION 3.15...4.2)

project(Caduq VERSION 0.0.4
              DESCRIPTION "Simple CAD software"
              LANGUAGES CXX)

# Vendors
add_subdirectory(Vizir/vendor/glad)
add_subdirectory(Vizir/vendor/glm)
add_subdirectory(Vizir/vendor/GLFW)
add_subdirectory(Vizir/vendor/imgui)
add_subdirectory(Vizir/vendor/stb_image)
add_subdirectory(Vizir/vendor/tracy)

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
