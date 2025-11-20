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

group "Dependencies"
	include "Vizir/vendor/GLFW"
	include "Vizir/vendor/glad"
	include "Vizir/vendor/imgui"

group ""

project "Vizir"
    location "Vizir"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "vzpch.h"
	pchsource "Vizir/src/vzpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
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
	    "%{prj.name}/src",
        "%{prj.name}/src/Vizir",
	    "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
	    "%{IncludeDir.glad}",
        "%{IncludeDir.ImGui}",
	    "%{IncludeDir.glm}",
	    "%{IncludeDir.Eigen}",
	    "%{IncludeDir.stb_image}"
    }
    links 
    { 
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "VZ_PLATFORM_WINDOWS",
            "VZ_BUILD_DLL",
            "GLFW_INCLUDE_NONE",
            "VZ_ENABLE_ASSERT",
        }
        links 
        { 
            "GLFW",
            "glad",
            "ImGui",
        }

    filter { "system:linux", "action:gmake" }
        buildoptions { "-fpermissive"}
        -- buildoptions { "-pedantic-errors"}
        --"-Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion"}
        -- -Werror to treat warning as error
        defines
        {
            "VZ_PLATFORM_LINUX",
            "VZ_BUILD_DLL",
            "GLFW_INCLUDE_NONE",
            "VZ_ENABLE_ASSERT",
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

project "Geotrup"
    location "Geotrup"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
	    "%{prj.name}/vendor/Eigne/Eigen/**"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs
    {
	    "%{prj.name}/src",
	    "%{IncludeDir.Eigen}",
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            -- "VZ_PLATFORM_WINDOWS",
            -- "VZ_BUILD_DLL",
            -- "GLFW_INCLUDE_NONE",
            -- "VZ_ENABLE_ASSERT",
        }

    filter { "system:linux", "action:gmake" }
        buildoptions { "-fpermissive"}
        -- buildoptions { "-pedantic-errors"}
        --"-Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion"}
        -- -Werror to treat warning as error
        defines
        {
            -- "VZ_PLATFORM_LINUX",
            -- "VZ_BUILD_DLL",
            -- "GLFW_INCLUDE_NONE",
            -- "VZ_ENABLE_ASSERT",
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

project "Caduq"
    location "Caduq"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
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
	    "Vizir/vendor/spdlog/include",
        "Vizir/src",
	    "Vizir/vendor",
        "Geotrup/src",
	    "%{IncludeDir.glm}",
	    "%{IncludeDir.Eigen}"
    }

    links
    {
        "Vizir",
        "Geotrup"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "VZ_PLATFORM_WINDOWS",
        }

    filter { "system:linux", "action:gmake" }
        buildoptions { "-fpermissive"}
        -- buildoptions { "-pedantic-errors"}
        --"-Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion"}
        -- -Werror to treat warning as error
        defines
        {
            "VZ_PLATFORM_LINUX",
        }

        links 
        { 
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
