--
-- Copyright (c) Catalin Ionescu 2024. All rights reserved.
-- Copyright (c) Robert Bengulescu 2024. All rights reserved.
-- Copyright (c) Traian Avram 2024. All rights reserved.
-- 
-- SPDX-License-Identifier: Apache-2.0.
--

function setup_project_configuration_settings()
    filter "configurations:Debug"
        optimize "off"
        symbols "on"
        defines { "CAVE_CONFIGURATION_DEBUG=1" }
    filter {}

    filter "configurations:Development"
        optimize "on"
        symbols "on"
        defines { "CAVE_CONFIGURATION_DEVELOPMENT=1" }
    filter {}

    filter "configurations:Shipping"
        optimize "speed"
        symbols "off"
        defines { "CAVE_CONFIGURATION_SHIPPING=1" }
    filter {}
end

workspace "CaveGame"
    configurations
    {
        "Debug",
        "Development",
        "Shipping"
    }

    platforms
    {
        "Windows"
    }

    filter "platforms:windows"
        system "windows"
        architecture "x64"
    filter {}

    startproject "CaveGame"

    project "Engine"
        kind "StaticLib"
        location "%{wks.location}/Engine/Source"

        language "c++"
        cppdialect "c++20"

        staticruntime "off"
        exceptionhandling "off"
        rtti "off"
        characterset "unicode"

        targetdir "%{wks.location}/Binaries/%{cfg.buildcfg}"
        objdir "%{wks.location}/Intermediate"

        files
        {
            "%{wks.location}/Engine/Source/**.cpp",
            "%{wks.location}/Engine/Source/**.h"
        }

        includedirs
        {
            "%{wks.location}/Engine/Source"
        }

        setup_project_configuration_settings()
        filter "platforms:windows"
            systemversion "latest"    
            defines { "CAVE_PLATFORM_WINDOWS=1" }
        filter {}
    -- endproject "Engine"

    project "CaveGame"
        kind "ConsoleApp"
        location "%{wks.location}/Source"

        language "c++"
        cppdialect "c++20"

        staticruntime "off"
        exceptionhandling "off"
        rtti "off"
        characterset "unicode"

        targetdir "%{wks.location}/Binaries/%{cfg.buildcfg}"
        objdir "%{wks.location}/Intermediate"

        files
        {
            "%{wks.location}/Source/**.cpp",
            "%{wks.location}/Source/**.h"
        }

        includedirs
        {
            "%{wks.location}/Source",
            "%{wks.location}/Engine/Source"
        }

        links
        {
            "Engine"
        }

        setup_project_configuration_settings()
        filter "platforms:windows"
            systemversion "latest"    
            defines { "CAVE_PLATFORM_WINDOWS=1" }
        filter {}
    -- endproject "CaveGame"
