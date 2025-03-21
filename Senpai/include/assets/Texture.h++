#pragma once
#include <core/Core.h++>

namespace Senpai::Assets {
struct Texture final : public Asset {
 private:
   // can only be loaded by the main-thread
   SharedPtr<void> sdlTexture = nullptr;
   // will be loaded by a seperate thread
   SharedPtr<void> sdlSurface = nullptr;

 public:
   String filename;
   f32 height = 1;
   f32 width = 1;

   Texture(const String &filename, String const &name);
   GenericPtr get_sdl_texture();
};

} // namespace Senpai::Assets