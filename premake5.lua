workspace "MagicEX"
   configurations { "Debug", "Release" }

project "magicex"
   kind "ConsoleApp"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"

   files {
          "**.h",
          "**.hpp",
          "**.cpp",
         }
   
   links {
      "raylib"
   }

   postbuildcommands {
        'cp -r assets "%{cfg.targetdir}/assets"'
    } 

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
