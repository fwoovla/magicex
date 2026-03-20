workspace "MagicEX"
   configurations { "Debug", "Release" }

project "magicex"
   kind "ConsoleApp"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"

   files {
          "src/**.h",
          "src/**.hpp",
          "src/**.cpp",
         }
   
   links {
      "raylib"
   }

   postbuildcommands {
        'cp -r assets "%{cfg.targetdir}/assets"'
    } 

filter "configurations:Debug"
    kind "ConsoleApp"
    symbols "Full"
    optimize "Off"

    buildoptions {
        "-g3",
        "-O0",
        "-fno-omit-frame-pointer",
        "-fsanitize=address",
        "-fsanitize=leak"
    }

    linkoptions {
        "-fsanitize=address",
        "-fsanitize=leak"
    }

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
