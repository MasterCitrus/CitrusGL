workspace "CitrusGL"
    architecture "x64"
    startproject "CitrusGL"

    configurations
    {
        "Debug",
        "Release"
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    IncludeDir = {}
    IncludeDir["GLFW"] = "vendor/GLFW/include"
    IncludeDir["Glad"] = "vendor/glad/include"
    IncludeDir["glm"] = "vendor/glm/"
    IncludeDir["ImGui"] = "vendor/imgui"
    IncludeDir["spdlog"] = "vendor/spdlog/include"
    IncludeDir["nfd"] = "vendor/nfd/src/include"
    IncludeDir["stb"] = "vendor/stb"

group "Engine"

project "CitrusGLEngine"
    location "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("int/" .. outputdir .. "/%{prj.name}")

    pchheader = "cglpch.h"
    pchsource = "%{prj.location}/src/cglpch.cpp"

    files 
    {
        "%{prj.location}/src/**.h",
        "%{prj.location}/src/**.cpp"
    }

    defines
    {
        "GLFW_INCLUDE_NONE",
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs
    {
        "%{prj.location}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.nfd}",
        "%{IncludeDir.stb}",
        "vendor/glad/src"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "nfd",
        "opengl32.lib"
    }

    buildoptions
    {
        "/utf-8"
    }

    filter "configurations:Debug"
        defines "CGL_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "CGL_RELEASE"
        runtime "Release"
        optimize "on"

group ""

project "CitrusGL"
location "CitrusGL"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "vendor/spdlog/include",
        "vendor",
        "Engine/src",
        "%{IncludeDir.glm}"
    }

    links
    {
        "CitrusGLEngine"
    }

    buildoptions
    {
        "/utf-8"
    }

    filter "configurations:Debug"
        defines "CGL_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "CGL_RELEASE"
        runtime "Release"
        optimize "on"

group "Engine/Dependencies"

project "GLFW"
    location "vendor"
    kind "StaticLib"
    language "C"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "vendor/GLFW/src/*.c",
        "vendor/GLFW/include/*.h"
    }

    filter "system:windows"
        systemversion "latest"
        staticruntime "on"
        
        files
        {
            "vendor/GLFW/src/win32_init.c",
            "vendor/GLFW/src/win32_joystick.c",
            "vendor/GLFW/src/win32_monitor.c",
            "vendor/GLFW/src/win32_time.c",
            "vendor/GLFW/src/win32_thread.c",
            "vendor/GLFW/src/win32_window.c",
            "vendor/GLFW/src/wgl_context.c",
            "vendor/GLFW/src/egl_context.c",
            "vendor/GLFW/src/osmesa_context.c"
        }

        defines 
        { 
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
        }

project "Glad"
    location "vendor"
    kind "StaticLib"
    language "C"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "vendor/glad/include/glad/glad.h",
        "vendor/glad/include/KHR/khrplatform.h",
        "vendor/glad/src/glad.c"
    }

    includedirs
    {
        "vendor/glad/include"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

project "ImGUI"
    location "vendor"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "vendor/imgui/*.h",
        "vendor/imgui/*.cpp",
    }

    includedirs
    {
        "vendor/imgui"
    }

project "nfd"
    location "vendor"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "vendor/nfd/src/include/nfd.h",
        "vendor/nfd/src/include/nfd.hpp",
        "vendor/nfd/src/include/nfd_glfw3.h",
        "vendor/nfd/src/nfd_win.cpp"
    }

    includedirs
    {
        "vendor/nfd/src/include"
    }

    links
    {
        "GLFW"
    }