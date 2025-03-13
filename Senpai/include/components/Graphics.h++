#pragma once
#include <core/ECS.h++>
#include <assets/Assets.h++>

namespace Senpai {
   enum BlendMode {
      Normal = 0x0,
      Add = 0x2,
      Multiply = 0x8,
      Blend = 0x1
   };

   enum Flip {
      None = 0x0,
      Horizontal = 0x1,
      Vertical = 0x2
   };

   struct Color {
      u8 r = 0;
      u8 g = 0;
      u8 b = 0;
      u8 a = 255;

      Color operator+(const Color& other) const;
      Color operator-(const Color& other) const;
      Color operator+=(const Color& other);
      Color operator-=(const Color& other);
   };

} // namespace Senpai

namespace Senpai::Components {

   // a simple non rotating camera
   struct Camera final : public Component {
      // camera offset form the rest of the entitie
      Vec2<f32> offset = {0,0};
      // camera zoom
      f32 zoom = 1;
      // need to fit the aspect ratio
      Vec2<f32> size = {1920,1080};
      // relative viewport
      Rectangle<f32> viewport = {{0.5, 0.5}, {1, 1}};
      // the target rect before zoom
      
      // constuctor
      Camera(Vec2<f32> offset = {0,0}, f32 zoom = 1, Rectangle<f32> viewport = {{0.5, 0.5},{1, 1}});
      Rectangle<f32> get_rect(Frame<f32> const& transform) const;
      Rectangle<f32> get_canvas() const;
   };
   REGISTER_COMPONENT_TYPE(Camera);

   struct Sprite final : public Component {
      u8 flip = 0;
      u8 blendMode = 0;
      Ptr<Assets::Texture> texturePtr = nullptr;

      Sprite(Ptr<Assets::Texture> assetPtr = nullptr, Set<Flip> flips = {}, Set<BlendMode> blendModes = {});
      void flip_vertical();
      void flip_horizontal();
      // frame of the entire sprite from the transfomation frame
      Frame<f32> get_frame(Frame<f32> const& transform) const;
   };
   REGISTER_COMPONENT_TYPE(Sprite);

   struct Text final : public Component {
      String text = "";
      Color color = {0,0,0,255};
      u8 flip = 0;
      u8 blendMode = 0;
      Ptr<Assets::Font> fontPtr;

      Text(String text = "", Color color = {0,0,0,255}, Ptr<Assets::Font> assetPtr = nullptr, Set<Flip> flips = {}, Set<BlendMode> blendModes = {});
      void flip_vertical();
      void flip_horizontal();
   };
   REGISTER_COMPONENT_TYPE(Text);

   // requires sprite component
   struct Animator final : public Component {
      private:
         // the time since the last flip
         f32 Δt = 0;
         // the current frame index
         u32 frameId = 0;
      public:
         Ptr<Assets::Animation> animationPtr = nullptr;
         // the time it takes to flip the Animation
         f32 flipTime = 1;
         // returns true if it has to flip the frame
         bool flip_frame(f32 δt);
         Ptr<Assets::Texture> next_frame();


         Animator(f32 flipTime = 1,  Ptr<Assets::Animation> animationPtr = nullptr); 
   };
   REGISTER_COMPONENT_TYPE(Animator);

   struct TileMap final : public Component {
      //virtual ~TileMap() noexcept override = default;
      Tuple<uint, uint> get_tile_at_point(Frame<f32> const& transform, Vec2<f32> point) const;
      Tuple<Range,Range> get_render_ranges(Frame<f32> const& tilemapTransform, Rectangle<f32> const& camera) const;
      Ptr<Assets::TileSet> tileSetPtr = nullptr;
      Matrix<u8> tiles;

      TileMap(Matrix<u8> tiles = Matrix<u8>(0,0), Ptr<Assets::TileSet> tileSetPtr = nullptr);
      // get the spriteframe of a tile at a specific position
      Frame<f32> get_tile(Frame<f32> const& transfom, uint x, uint y) const;
   };
   REGISTER_COMPONENT_TYPE(TileMap);
}