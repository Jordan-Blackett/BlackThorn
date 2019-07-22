workspace "BlackThorn"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "BlackThorn/vendor/GLFW/include"

include "BlackThorn/vendor/GLFW"

-- BLACKTHORN
project "BlackThorn"
	location "BlackThorn"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "btpch.h"
	pchsource "BlackThorn/src/btpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}"
	}

	links 
	{ 
		"wsock32.lib",
		"GLFW",
		"opengl32.lib"
	}
	
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"BT_PLATFORM_WINDOWS",
			"BT_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/WhiteThorn")
		}

	filter "configurations:Debug"
		defines "BT_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "BT_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "BT_DIST"
		optimize "On"

-- WHITETHORN
project "WhiteThorn"
	location "WhiteThorn"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"BlackThorn/vendor/spdlog/include",
		"BlackThorn/src",
		"Hazel/vendor"
	}

	links 
	{
		"BlackThorn"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"BT_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "BT_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "BT_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "BT_DIST"
		optimize "On"