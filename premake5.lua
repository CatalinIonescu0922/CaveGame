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
        location "%{wks.location}/Engine"

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

    project "CaveGame"
        kind "ConsoleApp"
        location "%{wks.location}"

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
