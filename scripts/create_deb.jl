#!/usr/bin/env julia

function create_deb(package_name::String="example-package",
   version::String="1.0",
   architecture::String="all",
   maintainer::String="maintainer@example.com",
   description::String="Example package";
   program::String="example-binary", # main executable
   libs::Vector{String}=String[],   # paths to .so files
   assets::String="assets"     # path to your assets folder
  )

# -- existing staging dirs --
pkg_dir = "private/$(package_name)-$(version)"
if isdir(pkg_dir) rm(pkg_dir; force=true, recursive=true) end
mkpath(pkg_dir)
debian_dir = joinpath(pkg_dir, "DEBIAN")
mkpath(debian_dir)

# your executables
bin_dir = joinpath(pkg_dir, "usr","local","bin")
mkpath(bin_dir)

# add a lib directory for your .so’s
lib_dir = joinpath(pkg_dir, "usr","local","lib")
mkpath(lib_dir)

# add a shared data folder
share_dir  = joinpath(pkg_dir, "usr","share", package_name)
assets_dir = joinpath(share_dir, "assets")
mkpath(assets_dir)

# -- write control file (unchanged) --
control_text = """
Package: $package_name
Version: $version
Architecture: $architecture
Maintainer: $maintainer
Description: $description
Depends: libc6 (>= 2.7), libstdc++6 (>= 4.1.1)
Section: games
Priority: optional
"""

# Depends: libc6 (>= 2.7), libstdc++6 (>= 4.1.1), 
#           libSDL3 (>= 3.0.0), libSDL3_image (>= 3.0.0),
#           libSDL3_ttf (>= 3.0.0), libSDL3_mixer (>= 3.0.0)

open(joinpath(debian_dir, "control"), "w") do io
write(io, control_text)
end

# -- copy in your main binary --
if isfile(program)
   cp(program, joinpath(bin_dir, basename(program) * ".bin"); force=true)
   # now write a tiny wrapper that cd's into /usr/share/<pkg> before exec
   wrapper = joinpath(bin_dir, basename(program))
   open(wrapper, "w") do io
      println(io, "#!/usr/bin/env sh")
      println(io, "cd /usr/share/$(package_name)")
      println(io, "exec \"\$(dirname \"\$0\")/$(basename(program)).bin\" \"\$@\"")
   end
   run(`chmod 755 $wrapper`)
else
   @error "Main binary not found: $program"
   return
end

# -- copy in your .so libraries --
for lib in libs
   if isfile(lib)
      cp(lib, joinpath(lib_dir, basename(lib)); force=true)
   else
      @warn "Library not found: $lib"
   end
end

# -- copy your entire assets folder --
if isdir(assets)
   run(`cp -a $assets/. $assets_dir`)
else
   @warn "Assets folder not found: $assets"
end

run(`chmod 755 $(joinpath(bin_dir, basename(program)))`)

# -- finally build the .deb --
deb_filename = "$(package_name)_$(version).deb"
run(`dpkg-deb --build $pkg_dir $deb_filename`)
println("DEB package created: ", deb_filename)
end

# Example invocation, pointing at your .so’s and assets folder:
create_deb(
"Five-Knights", "1.0", "amd64",
"daniel.schwarzenbach@outlook.com",
"Five Knights against King Fredric Game",
program = "build/Game/FiveKnights",
libs = [
   "build/Senpai/libSenpai.so", 
   "build/Senpai/lib/SDL/libSDL3.so", 
   "build/Senpai/lib/SDL_image/libSDL3_image.so",
   "build/Senpai/lib/SDL_ttf/libSDL3_ttf.so",
   "build/Senpai/lib/SDL_mixer/libSDL3_mixer.so"
],
assets = "build/Game/assets"
)
