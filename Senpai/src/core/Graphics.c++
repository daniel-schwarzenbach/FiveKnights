#include <core/Graphics.h++>
#include <sdl/SDLBackend.h++>

namespace Senpai {
f32 Color::to_f32(u8 color) { return f32(color) / 255.0f; }

u8 Color::from_f32(f32 color) { return u8(std::round(color * 255.0f)); }

Color Color::operator-(const Color &other) const {
   return Color{from_f32(to_f32(r) * to_f32(other.r)),
                from_f32(to_f32(g) * to_f32(other.g)),
                from_f32(to_f32(b) * to_f32(other.b)),
                from_f32(to_f32(a) * to_f32(other.a))};
}

Color Color::operator+(const Color &other) const {
   return Color{
       u8(255 - int(from_f32(to_f32(u8(255) - r) * to_f32(u8(255) - other.r)))),
       u8(255 - int(from_f32(to_f32(u8(255) - g) * to_f32(u8(255) - other.g)))),
       u8(255 - int(from_f32(to_f32(u8(255) - b) * to_f32(u8(255) - other.b)))),
       u8(255 -
          int(from_f32(to_f32(u8(255) - a) * to_f32(u8(255) - other.a))))};
}

Vec2<f32> get_render_scale() {
   Vec2<int> wSize = Window::get_size();
   return Vec2<f32>{f32(wSize.x) / 1920.0f, f32(wSize.y) / 1080.0f};
}

}  // namespace Senpai