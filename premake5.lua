workspace "Caduq"
    architecture "x64"
    startproject "Caduq"
    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Vizir/vendor/GLFW/include"
IncludeDir["glad"] = "Vizir/vendor/glad/include"
IncludeDir["ImGui"] = "Vizir/vendor/imgui"
IncludeDir["glm"] = "Vizir/vendor/glm"
IncludeDir["stb_image"] = "Vizir/vendor/stb_image"
IncludeDir["Eigen"] = "Vizir/vendor/Eigen"
IncludeDir["spdlog"] = "Vizir/vendor/spdlog/include"

-- Configuration for whole workspace
externalanglebrackets ("on")
externalwarnings "Off"

group "Dependencies"
    warnings ("off")

	include "Vizir/vendor/GLFW"
	include "Vizir/vendor/glad"
	include "Vizir/vendor/imgui"

group ""
    project "Vizir"
        location "Vizir"
        kind "StaticLib"
        language "C++"
        cppdialect "C++20"
        staticruntime "on"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        pchheader "vzpch.h"
        pchsource "Vizir/src/vzpch.cpp"

        files
        {
            -- Internal files
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",

            -- External files
            "%{prj.name}/vendor/stb_image/**.h",
            "%{prj.name}/vendor/stb_image/**.cpp",
            "%{prj.name}/vendor/glm/glm/**.hpp",
            "%{prj.name}/vendor/glm/glm/**.inl",
        }

        defines
        {
            "_CRT_SECURE_NO_WARNINGS"
        }

        includedirs
        {
            -- Internal files
            "%{prj.name}/src",
            "%{prj.name}/src/Vizir",

            "Bob/src",

            -- External files
            "%{IncludeDir.GLFW}",
            "%{IncludeDir.glad}",
            "%{IncludeDir.ImGui}",
            "%{IncludeDir.glm}",
            "%{IncludeDir.stb_image}",
            "%{IncludeDir.spdlog}"
        }

        links 
        { 
            "Bob",
            "opengl32.lib"
        }

        defines
        {
            "VZ_BUILD_DLL",
            "GLFW_INCLUDE_NONE",
            "VZ_ENABLE_ASSERT",
        }

        filter "system:windows"
            systemversion "latest"

            defines
            {
                "VZ_PLATFORM_WINDOWS"
            }

            links 
            { 
                "GLFW",
                "glad",
                "ImGui",
            }

        filter { "system:linux", "action:gmake" }
            buildoptions { "-fpermissive"}
        
        filter "configurations:Debug"
            defines "VZ_DEBUG"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            defines "VZ_RELEASE"
            runtime "Release"
            optimize "on"

        filter "configurations:Dist"
            defines "VZ_DIST"
            runtime "Release"
            optimize "on"

    project "Geotrup"
        location "Geotrup"
        kind "StaticLib"
        language "C++"
        cppdialect "C++20"
        staticruntime "on"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        files
        {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",
        }

        defines
        {
            "_CRT_SECURE_NO_WARNINGS"
        }

        includedirs
        {
            -- Internal files
            "%{prj.name}/src",

            -- External files
            "%{IncludeDir.Eigen}",
            "%{IncludeDir.ImGui}",
        }

        filter "system:windows"
            systemversion "latest"

        filter { "system:linux", "action:gmake" }
            buildoptions { "-fpermissive"}
        
        filter "configurations:Debug"
            defines "VZ_DEBUG"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            defines "VZ_RELEASE"
            runtime "Release"
            optimize "on"

        filter "configurations:Dist"
            defines "VZ_DIST"
            runtime "Release"
            optimize "on"

    project "Caduq"
        location "Caduq"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++20"
        staticruntime "on"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        files
        {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp"
        }

        includedirs
        {
            "Caduq/src",

            -- External files
            "Vizir/src",
            "Geotrup/src",
            "Bob/src",

            "%{IncludeDir.ImGui}",
            "%{IncludeDir.glm}",
            "%{IncludeDir.Eigen}",
            "%{IncludeDir.spdlog}"
        }

        links
        {
            "Vizir",
            "Geotrup"
        }

        filter "system:windows"
            systemversion "latest"

        filter { "system:linux", "action:gmake" }
            buildoptions { "-fpermissive"}

            links 
            { 
				"Bob",
                "GLFW",
                "glad",
                "ImGui",
            }
        
        filter "configurations:Debug"
            defines "VZ_DEBUG"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            defines "VZ_RELEASE"
            runtime "Release"
            optimize "on"

        filter "configurations:Dist"
            defines "VZ_DIST"
            runtime "Release"
            optimize "on"

    project "Bob"
        location "Bob"
        kind "StaticLib"
        language "C++"
        cppdialect "C++20"
        staticruntime "on"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        files
        {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp"
        }

        includedirs
        {
            -- Internal files
            "%{prj.name}/src",

            -- External files
            "%{IncludeDir.spdlog}"
        }

        defines
        {
            "BOB_BUILD_DLL",
        }

        filter "system:windows"
            systemversion "latest"

 
        filter { "system:linux", "action:gmake" }
            buildoptions { "-fpermissive"}
        
        filter "configurations:Debug"
            defines "BOB_DEBUG"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            defines "BOB_RELEASE"
            runtime "Release"
            optimize "on"

        filter "configurations:Dist"
            defines "BOB_DIST"
            runtime "Release"

            optimize "on"
