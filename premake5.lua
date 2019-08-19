workspace "BlackThorn"
	architecture "x64"
	startproject "WhiteThorn"

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
IncludeDir["Glad"] = "BlackThorn/vendor/Glad/include"
IncludeDir["ImGui"] = "BlackThorn/vendor/imgui"
IncludeDir["glm"] = "BlackThorn/vendor/glm"

group "Dependencies"
	include "BlackThorn/vendor/GLFW"
	include "BlackThorn/vendor/Glad"
	include "BlackThorn/vendor/imgui"

group ""

-- BLACKTHORN
project "BlackThorn"
	location "BlackThorn"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "btpch.h"
	pchsource "BlackThorn/src/btpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links 
	{ 
		"wsock32.lib",
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}
	
	filter "system:windows"
		systemversion "latest"

		defines
		{
			"BT_PLATFORM_WINDOWS",
			"BT_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/WhiteThorn/\"")
		}

	filter "configurations:Debug"
		defines "BT_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "BT_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "BT_DIST"
		runtime "Release"
		optimize "On"

-- WHITETHORN
project "WhiteThorn"
	location "WhiteThorn"
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
		"BlackThorn/vendor/spdlog/include",
		"BlackThorn/src",
		"BlackThorn/vendor",
		"%{IncludeDir.glm}"
	}

	links 
	{
		"BlackThorn"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"BT_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "BT_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "BT_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "BT_DIST"
		runtime "Release"
		optimize "On"