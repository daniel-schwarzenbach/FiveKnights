#pragma once
#include <core/Core.h++>
#include <assets/Assets.h++>

namespace Senpai::Components {

   /*
   ```cpp
   { Vec2<f32> offset = {0, 0}, f32 zoom = 1, Rectangle<f32> viewport = {{0, 0}, {1, 1}} }
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
         Camera(Vec2<f32> offset = {0, 0}, f32 zoom = 1, Rectangle<f32> viewport = {{0, 0}, {1, 1}});
         // get the effective rect of the camera
         Rectangle<f32> get_rect(Frame<f32> const &transform) const;
         // get the effective canvas of the camera and the window size
         Rectangle<f32> get_canvas() const;
   };
   REGISTER_COMPONENT_TYPE(Camera);

   /*
   ```cpp
   { Ptr<Assets::Texture> assetPtr = nullptr, Set<Flip> flips = {}, Set<BlendMode> blendModes = {} }
   ```
   */
   struct Sprite final : public RenderComponent {
      Ptr<Assets::Texture> texturePtr = nullptr;

      Sprite(Ptr<Assets::Texture> assetPtr = nullptr, Set<Flip> flips = {}, Set<BlendMode> blendModes = {});
      // frame of the entire sprite from the transfomation frame
      Frame<f32> get_frame(Frame<f32> const &transform) const;
      // render the sprite
      void render(Rectangle<f32> const &camera,Rectangle<f32> const &canvas) override;
   };
   REGISTER_COMPONENT_TYPE(Sprite);

   // struct TextUI final : public UIComponent {
   //    Color color = {0, 0, 0, 255};
   //    Ptr<Assets::Font> fontPtr;

   //    TextUI(String text = "", Color color = {0, 0, 0, 255}, Ptr<Assets::Font> assetPtr = nullptr, Set<Flip> flips = {}, Set<BlendMode> blendModes = {});
   //    void flip_vertical();
   //    void flip_horizontal();
   // };
   // REGISTER_COMPONENT_TYPE(TextUI);

   /*
   used to easyly create buttons with text
   ```cpp
   ButtonUI{
      Ptr<Assets::Font> fontPtr = nullptr, 
      String text = "", 
      Color textColor = {0, 0, 0, 255}, 
      f32 textScale = 50, 
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
      // Text Size
      f32 textScale = 1.0f;
      // Button Color
      Color buttonColor = {255, 255, 255, 120};
      // Button Hover Color
      Color buttonHoverColor = {255, 255, 255, 200};
      // Button Size
      Vec2<f32> buttonScale = {100, 100};

      ButtonUI(Ptr<Assets::Font> fontPtr = nullptr, String text = "", Color textColor = {0, 0, 0, 255}, f32 textScale = 50, Color buttonColor = {255, 255, 255, 120}, Color buttonHoverColor = {255, 255, 255, 200}, Vec2<f32> buttonScale = {100, 100});
      // get the rectangle of the button
      Rectangle<f32> get_rect() const;
      
      // is on button
      bool is_on_button(Vec2<f32> point);

      // render the button
      void render() override;

   };
   REGISTER_COMPONENT_TYPE(ButtonUI);

       // requires sprite component
   struct Animator final : public Component {
   private:
      // the time since the last flip
      f32 δt = 0;
      // the current frame index
      u32 frameId = 0;

   public:
      Ptr<Assets::Animation> animationPtr = nullptr;
      // the time it takes to flip the Animation
      f32 flipTime = 1;
      // returns true if it has to flip the frame
      bool flip_frame(f32 Δt);
      Ptr<Assets::Texture> next_frame();

      Animator(f32 flipTime = 1, Ptr<Assets::Animation> animationPtr = nullptr);
   };
   REGISTER_COMPONENT_TYPE(Animator);

   struct TileMap final : public RenderComponent
   {
      // virtual ~TileMap() noexcept override = default;
      Tuple<uint, uint> get_tile_at_point(Frame<f32> const &transform, Vec2<f32> point) const;
      Tuple<Range, Range> get_render_ranges(Frame<f32> const &transform, Rectangle<f32> const &camera) const;
      Ptr<Assets::TileSet> tileSetPtr = nullptr;
      Matrix<u8> tiles;

      TileMap(Matrix<u8> tiles = Matrix<u8>(0, 0), Ptr<Assets::TileSet> tileSetPtr = nullptr);
      // get the spriteframe of a tile at a specific position
      Frame<f32> get_tile(Frame<f32> const &transfom, uint x, uint y) const;
      // render the tilemap
      void render(Rectangle<f32> const &camera, Rectangle<f32> const &canvas) override;
   };
   REGISTER_COMPONENT_TYPE(TileMap);
}