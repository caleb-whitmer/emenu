local final_target          = "emenu"
local src_dir               = "src/"
local default_font_path     = "./assets/ProggyClean.ttf"
local default_font_var_name = "ProggyClean_ttf_raw"
local default_font_header   = src_dir.."ProggyClean.h"

add_requires("sfml")

target(final_target)
  set_languages("c++23")
  set_kind("binary")
  add_files("src/*.cpp")
  add_files("src/*.cppm")
  set_policy("build.c++.modules", true)
  add_packages("sfml")

  before_prepare(function (target) 
    -- Create a C source file from the ProggyClean font assuming it doesn't
    -- already exist
    if not os.isfile(default_font_header) then
      print("Converting font...")
      local xxd_out, xxd_err = os.iorunv("xxd", {
        "-u",
        "-c", "13", --[[Octets per line]]
        "-i",
        "-n", default_font_var_name,
        default_font_path
      })
      io.writefile(default_font_header, xxd_out)
      print("Font successfully converted!")
    end
  end)
  after_build(function (target) 
    -- Copy the target to the root directory of the repo
    os.cp(target:targetfile(), target:name())
  end)
  before_clean(function (target)
    -- Remove the target
    print("Removing target...")
    os.rm(final_target)
    -- Remove the font header
    print("Removing font header...")
    os.rm(default_font_header)
  end)