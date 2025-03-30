#pragma once
#include <assets/Assets.h++>
#include <core/Core.h++>

namespace Senpai::Components {

/*
```cpp
{ Vec2<f32> offset = {0, 0}, f32 zoom = 1, Rectangle<f32> viewport = {{0, 0},
{1, 1}} }
```
*/
struct Camera final : public Component {
  private:
   // the size of the camera// need to fit the aspect ratio
   Vec2<f32> size = {1920, 1080};

  public:
   // camera offset form the rest of the entitie
   Vec2<f32> offset = {0, 0};
   // camera zoom
   f32 zoom = 1;
   // relative viewport
   Rectangle<f32> viewport = {{0.5, 0.5}, {1, 1}};
   // the target rect before zoom

   // constuctor
   Camera(Vec2<f32> offset = {0, 0}, f32 zoom = 1,
          Rectangle<f32> viewport = {{0, 0}, {1, 1}});
   // get the effective rect of the camera
   Rectangle<f32> get_rect(Frame<f32> const &transform) const;
   // get the effective canvas of the camera and the window size
   Rectangle<f32> get_canvas() const;
};
REGISTER_COMPONENT_TYPE(Camera);

/*
```cpp
{ Ptr<Assets::Texture> assetPtr = nullptr, Vec2<f32> offset = {0.0f, 0.0f},
Set<Flip> flips = {} }
```
*/
struct Sprite final : public RenderComponent {
   Ptr<Assets::Texture> texturePtr = nullptr;
   PixelArea scrArea = {0, 0, 0, 0};
   Vec2<f32> offset = {0, 0};

   Sprite(Ptr<Assets::Texture> assetPtr = nullptr,
          Vec2<f32> offset = {0.0f, 0.0f}, Set<Flip> flips = {});
   // frame of the entire sprite from the transfomation frame
   Frame<f32> get_frame(Frame<f32> const &transform) const;
   // render the sprite
   void render(Rectangle<f32> const &camera,
               Rectangle<f32> const &canvas) override;
};
REGISTER_COMPONENT_TYPE(Sprite);

/*
used to easyly create buttons with text
```cpp
ButtonUI{
   Ptr<Assets::Font> fontPtr = nullptr,
   String text = "",
   Color textColor = {0, 0, 0, 255},
   Color buttonColor = {255, 255, 255, 120},
   Color buttonHoverColor = {255, 255, 255, 200},
   Vec2<f32> buttonScale = {100, 100}
};
```

*/
struct ButtonUI final : public UIComponent {
   // is the button hovered set by the UI system
   bool hoverstate = false;
   // ptr to the font used by the Button, can be nullptr
   Ptr<Assets::Font> fontPtr;
   // text that is displayed
   String text = "";
   // Text Color
   Color textColor = {0, 0, 0, 255};
   // Button Color
   Color buttonColor = {255, 255, 255, 120};
   // Button Hover Color
   Color buttonHoverColor = {255, 255, 255, 200};
   // Button Size
   Vec2<f32> buttonScale = {100, 100};

   ButtonUI(Ptr<Assets::Font> fontPtr = nullptr, String text = "",
            Color textColor = {0, 0, 0, 255},
            Color buttonColor = {255, 255, 255, 120},
            Color buttonHoverColor = {255, 255, 255, 200},
            Vec2<f32> buttonScale = {100, 100});
   // get the rectangle of the button
   Rectangle<f32> get_rect() const;

   // is on button
   bool is_on_button(Vec2<f32> point);

   // render the button
   void render() override;
};
REGISTER_COMPONENT_TYPE(ButtonUI);

/*
```cpp
{Vector<Ptr<Assets::Animation>> animations = {}, Vector<u32> nextAnimations =
{}}
```
*/
struct Animator final : public Component {
  private:
   // the time since the last flip
   f32 lastFlipTime = 0;

  public:
   bool has_switched = false;
   // the current frame index
   u32 frameId = 0;
   // the current animation
   u32 animId = 0;
   // 1/(fps of the animation)
   f32 flipTime = 1.0f / 15.0f;
   // the animations and the id of the next animation
   Vector<Ptr<Assets::Animation>> animations = {};
   Vector<u32> nextAnimId = {};
   // change the current animation
   bool switch_animation(u32 id);
   // returns true if it has to flip the frame
   bool flip_frame(f32 dt);

   Animator(f32 fps = 15, Vector<Ptr<Assets::Animation>> animations = {},
            Vector<u32> nextAnimations = {});
};
REGISTER_COMPONENT_TYPE(Animator);

/*
```cpp
{Matrix<u16> tiles = Matrix<u16>(0, 0), Ptr<Assets::TileSet> tileSetPtr =
nullptr}
```
*/
struct TileMap final : public RenderComponent {
   // virtual ~TileMap() noexcept override = default;
   Tuple<uint, uint> get_tile_at_point(Frame<f32> const &transform,
                                       Vec2<f32> point) const;
   Tuple<Range, Range> get_render_ranges(Frame<f32> const &transform,
                                         Rectangle<f32> const &camera) const;
   Ptr<Assets::TileSet> tileSetPtr = nullptr;
   Matrix<u16> tiles;

   TileMap(Matrix<u16> tiles = Matrix<u16>(0, 0),
           Ptr<Assets::TileSet> tileSetPtr = nullptr);
   // get the spriteframe of a tile at a specific position
   Frame<f32> get_tile(Frame<f32> const &transfom, uint x, uint y) const;
   // render the tilemap
   void render(Rectangle<f32> const &camera,
               Rectangle<f32> const &canvas) override;
};
REGISTER_COMPONENT_TYPE(TileMap);

/*
```cpp
Ptr<Assets::Texture> assetPtr = nullptr, Set<Flip> flips = {}
```
*/
struct Light final : public Component {
   Ptr<Assets::Texture> texturePtr = nullptr;
   PixelArea scrArea = {0, 0, 0, 0};
   u8 flip = 0;
   Vec2<f32> offset = {0, 0};

   Light(Ptr<Assets::Texture> assetPtr = nullptr, Set<Flip> flips = {});
   Frame<f32> get_frame(Frame<f32> const &transform) const;
};
REGISTER_COMPONENT_TYPE(Light);

}  // namespace Senpai::Components