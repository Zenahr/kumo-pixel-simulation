workspace "Pixelsim"
    architecture "x64"
    startproject "Pixelsim"
    configurations {
        "Debug",
        "Profile",
        "Release"
    }

    project "Glad"
        location      "dependencies/Glad"
        kind          "StaticLib"
        language      "C"
        staticruntime "on"
        systemversion "latest"
        warnings      "off"
        targetdir     "_bin/%{cfg.buildcfg}/%{prj.name}"
        objdir        "_obj/%{cfg.buildcfg}/%{prj.name}"
        files {
            "dependencies/Glad/include/glad/glad.h",
            "dependencies/Glad/include/KHR/khrplatform.h",
            "dependencies/Glad/src/glad.c"
        }
        includedirs {
            "dependencies/Glad/include"
        }
        filter "configurations:Debug"
            runtime  "Debug"
            symbols  "on"
            optimize "off"
        filter "configurations:Profile"
            runtime  "Release"
            symbols  "on"
            optimize "on"
        filter "configurations:Release"
            runtime  "Release"
            symbols  "off"
            optimize "on"

    project "GLFW"
        location      "dependencies/GLFW"
        kind          "StaticLib"
        language      "C"
        staticruntime "on"
        systemversion "latest"
        warnings      "off"
        targetdir     "_bin/%{cfg.buildcfg}/%{prj.name}"
        objdir        "_obj/%{cfg.buildcfg}/%{prj.name}"
        files {
            "dependencies/GLFW/include/GLFW/glfw3.h",
            "dependencies/GLFW/include/GLFW/glfw3native.h",
            "dependencies/GLFW/src/glfw_config.h",
            "dependencies/GLFW/src/context.c",
            "dependencies/GLFW/src/init.c",
            "dependencies/GLFW/src/input.c",
            "dependencies/GLFW/src/monitor.c",
            "dependencies/GLFW/src/vulkan.c",
            "dependencies/GLFW/src/window.c"
        }
        filter "system:windows"
            files {
                "dependencies/GLFW/src/win32_init.c",
                "dependencies/GLFW/src/win32_joystick.c",
                "dependencies/GLFW/src/win32_monitor.c",
                "dependencies/GLFW/src/win32_time.c",
                "dependencies/GLFW/src/win32_thread.c",
                "dependencies/GLFW/src/win32_window.c",
                "dependencies/GLFW/src/wgl_context.c",
                "dependencies/GLFW/src/egl_context.c",
                "dependencies/GLFW/src/osmesa_context.c"
            }
            defines {
                "_GLFW_WIN32"
            }
        filter "configurations:Debug"
            runtime  "Debug"
            symbols  "on"
            optimize "off"
        filter "configurations:Profile"
            runtime  "Release"
            symbols  "on"
            optimize "on"
        filter "configurations:Release"
            runtime  "Release"
            symbols  "off"
            optimize "on"

    project "ImGui"
        location      "dependencies/ImGui"
        kind          "StaticLib"
        language      "C++"
        staticruntime "on"
        systemversion "latest"
        warnings      "off"
        targetdir     "_bin/%{cfg.buildcfg}/%{prj.name}"
        objdir        "_obj/%{cfg.buildcfg}/%{prj.name}"
        files {
            "dependencies/ImGui/imconfig.h",
            "dependencies/ImGui/imgui_demo.cpp",
            "dependencies/ImGui/imgui_draw.cpp",
            "dependencies/ImGui/imgui_internal.h",
            "dependencies/ImGui/imgui_widgets.cpp",
            "dependencies/ImGui/imgui.cpp",
            "dependencies/ImGui/imgui.h",
            "dependencies/ImGui/imstb_rectpack.h",
            "dependencies/ImGui/imstb_textedit.h",
            "dependencies/ImGui/imstb_truetype.h"
        }
        includedirs {
            "dependencies/ImGui"
        }
        filter "configurations:Debug"
            runtime  "Debug"
            symbols  "on"
            optimize "off"
        filter "configurations:Profile"
            runtime  "Release"
            symbols  "on"
            optimize "on"
        filter "configurations:Release"
            runtime  "Release"
            symbols  "off"
            optimize "on"

    project "Pixelsim"
        location      "pixelsim"
        kind          "ConsoleApp"
        language      "C++"
        cppdialect    "C++17"
        staticruntime "on"
        systemversion "latest"
        pchheader     "Common.hpp"
        pchsource     "pixelsim/src/Common.cpp"
        warnings      "extra"
        targetdir     "_bin/%{cfg.buildcfg}/%{prj.name}"
        objdir        "_obj/%{cfg.buildcfg}/%{prj.name}"
        defines {
            "GLFW_INCLUDE_NONE",
            "GLM_FORCE_RADIANS"
        }
        files {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.hpp",
            "%{prj.name}/src/**.cpp",
            -- dependencies
            "dependencies/STB/stb/image.cpp",
            "dependencies/tinyobjloader/tinyobj/tiny_obj_loader.cpp"
        }
        includedirs {
            "%{prj.name}/src",
            "dependencies/GLFW/include",
            "dependencies/Glad/include",
            "dependencies/ImGui",
            "dependencies/GLM",
            "dependencies/OpenCL",
            "dependencies/STB",
            "dependencies/tinyobjloader"
        }
        links {
            "GLFW",
            "Glad",
            "ImGui",
            "dependencies/OpenCL/OpenCL"
        }
        filter "configurations:Debug"
            runtime  "Debug"
            symbols  "on"
            optimize "off"
        filter "configurations:Profile"
            runtime  "Release"
            symbols  "on"
            optimize "on"
        filter "configurations:Release"
            runtime  "Release"
            symbols  "off"
            optimize "on"

