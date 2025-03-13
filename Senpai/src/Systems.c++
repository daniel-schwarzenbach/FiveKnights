#include <systems/Systems.h++>
#include <assets/Assets.h++>
#include <sdl/SDLBackend.h++>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>

namespace Senpai {

   void Systems::SpriteRenderer::update(f32 δt) {
      Vector<Entity*> cameras = view<Components::Camera>();
      // if ther is no camera display pink screen
      if(cameras.size() == 0){
         debug_log("No Camera found!");
         SDL_SetRenderDrawColor(
            Renderer::get(),
            255, 0, 255, 255);
         return;
      }
      // render all cameras
      for (auto& camera : cameras){
         if(!camera->has_component<Components::Transform>()){
            debug_log("Entity with Camera does not have a Transform component!");
            continue;
         }
         auto trCam = camera->get_transform();
         auto& cam = camera->get_component<Components::Camera>();
         // get viewport
         SDL_Rect viewport = to_sdl_viewport(cam.viewport);
         // set camera viewport
         SDL_SetRenderViewport(
            Renderer::get(),
            &viewport
         );
         Rectangle<f32> cameraRect = cam.get_rect(trCam);
         Rectangle<f32> canvasRect = cam.get_canvas();
         // render all sprites
         for (auto& entity : view<Components::Sprite>()){
            // check if entity has a transform component
            if(!entity->has_component<Components::Transform>()){
               debug_log("Entity does not have a Transform component!");
               continue;
            }
            const auto& trSprite = entity->get_transform();
            const auto& sprite = entity->get_component<Components::Sprite>();
            const auto& texture = *sprite.texturePtr;
            // get the effective size of the sprite and the camera
            Frame<f32> spriteFrame = sprite.get_frame(trSprite);
            // porject sprite from camera to the canvas
            Frame<f32> projection = spriteFrame.project(cameraRect, canvasRect);
            // render the projection sprite
            SDL_RenderFrame(projection, (SDL_Texture*)texture.sdlTexture, sprite.flip);
         }
      }
   }

   Tuple<SDL_Texture*, f32, f32> get_texture_from_text(Components::Text const& text) {
      auto& font = *text.fontPtr;
      // create surface
      SDL_Surface* surface = TTF_RenderText_Blended(
         (TTF_Font*)font.sdlFont,
         text.text.c_str(),
         text.text.size(),
         {text.color.r, text.color.g, text.color.b, text.color.a}
      );
      if (!surface) {
         debug_log("Couldn't Create sdlsurface");
         return {nullptr,0,0};
      }
      // create texture from surface
      SDL_Texture* texture = SDL_CreateTextureFromSurface(
         (SDL_Renderer*)Renderer::get(),
         surface
      );
      if (!texture) {
         debug_log("Couldn't Create sdltexture");
         return {nullptr,0,0};
      }
      f32 width, height;
      SDL_GetTextureSize(texture, &width, &height); 
      return {texture, width, height};
   }

   void Systems::TextRenderer::update(f32 δt) {
      // render all cameras
      for (auto& camera : view<Components::Camera>()){
         if(!camera->has_component<Components::Transform>()){
            debug_log("Entity with Camera does not have a Transform component!");
            continue;
         }
         auto trCam = camera->get_transform();
         auto& cam = camera->get_component<Components::Camera>();
         // get viewport
         SDL_Rect viewport = to_sdl_viewport(cam.viewport);
         // set camera viewport
         SDL_SetRenderViewport(
            Renderer::get(),
            &viewport
         );
         Rectangle<f32> cameraRect = cam.get_rect(trCam);
         Rectangle<f32> canvasRect = cam.get_canvas();
         // render all texts
         for (auto& entity : view<Components::Text>()){
            if(!entity->has_component<Components::Transform>()){
               debug_log("Entity does not have a Transform component!");
               continue;
            }
            auto trText = entity->get_transform();
            auto& text = entity->get_component<Components::Text>();
            auto [texture, width, height] = get_texture_from_text(text);
            trText.scale({width, height});
            Frame projection = trText.project(cameraRect, canvasRect);
            // render the projection text
            SDL_RenderFrame(projection, texture, text.flip);
         }
      }
   }

   void Systems::SpriteAnimator::update(f32 δt) {
      for(auto entity : view<Components::Animator>()) {
         if(!entity->has_component<Components::Sprite>()){
            debug_log("Entity with Animator does not have a Sprit component!");
            continue;
         }
         auto& animator = entity->get_component<Components::Animator>();
         auto& sprite = entity->get_component<Components::Sprite>();
         auto& animation = *animator.animationPtr;
         
         if(animator.flip_frame(δt)){
            sprite.texturePtr = animator.next_frame();
         }
      }
   }

   void Systems::TileMapRenderer::update(f32 δt) {
      for (auto& camera : view<Components::Camera>()){
         if(!camera->has_component<Components::Transform>()){
            debug_log("Entity with Camera does not have a Transform component!");
            continue;
         }
         auto trCam = camera->get_transform();
         auto& cam = camera->get_component<Components::Camera>();
         // get viewport
         SDL_Rect viewport = to_sdl_viewport(cam.viewport);
         // set camera viewport
         SDL_SetRenderViewport(
            Renderer::get(),
            &viewport
         );
         Rectangle<f32> cameraRect = cam.get_rect(trCam);
         Rectangle<f32> canvasRect = cam.get_canvas();
         // render all tilemaps
         for (auto& entity : view<Components::TileMap>()){
            if(!entity->has_component<Components::Transform>()){
               debug_log("Entity does not have a Transform component!");
               continue;
            }
            auto trTileMap = entity->get_transform();
            auto& tileMap = entity->get_component<Components::TileMap>();
            auto& tileSet = *tileMap.tileSetPtr;
            auto [xRange, yRange] = tileMap.get_render_ranges(trTileMap, cameraRect);
            // set the source size to the tile size
            SDL_FRect src = {0,0, f32(tileSet.tileSize), f32(tileSet.tileSize)};
            // iterate over all tiles in the range
            for (auto y : yRange){
               for (auto x : xRange){
                  Frame<f32> tileFrame = tileMap.get_tile(trTileMap, x, y);
                  u8 tileId = tileMap.tiles[uint(x), uint(y)];
                  auto& texture = tileSet.tiles[tileId];
                  Frame<f32> projection = tileFrame.project(cameraRect, canvasRect);
                  SDL_RenderFrame(projection, (SDL_Texture*)texture.sdlTexture, SDL_FlipMode(0), &src);
               }
            }
         }
      }
   }

   void Systems::Physics::update(f32 δt) {
      for (auto& entity : view<Components::Rigidbody>()) {
         if (!entity->has_component<Components::Transform>()) {
            debug_log("Entity with Rigidbody does not have a Transform component!");
            continue;
         }
         auto& tr = entity->get_component<Components::Transform>();
         auto& rb = entity->get_component<Components::Rigidbody>();
         rb.velocity += rb.acceleration * δt;
         // apply gravity
         rb.velocity.y -= gravity * δt;
         tr.frame.position += rb.velocity * δt;
         entity->get_transform().add_offset(rb.velocity * δt);
      }
   }

   // O(n²) collision detection
   void Systems::Collisions::update(f32 δt) {
      auto entityPtrs = view<Components::RectangleCollider>();
      for (UInt i = 0; i < entityPtrs.size(); i++) {
         auto& entity1 = entityPtrs[i];
         // get velocity and mass of the entity
         bool s1, s2;
         f32 m1, m2;
         Vec2<f32> v1, v2;
         if (!entity1->has_component<Components::Rigidbody>()) {
            v1 = {0,0};
            m1 = inf;
         } else {
            auto& rb1 = entity1->get_component<Components::Rigidbody>();
            m1 = rb1.mass;
            v1 = rb1.velocity;
         }
         Frame<f32> tr1 = entity1->get_transform();
         auto& coll1 = entity1->get_component<Components::RectangleCollider>
         ();
         Rectangle<f32> box1 = coll1.get_rect(tr1);
         // check for collision with other colliders
         for(UInt j = i+1; i < entityPtrs.size(); j++) {
            auto& entity2 = entityPtrs[j];
            if (!entity2->has_component<Components::Rigidbody>()) {
               v2 = {0,0};
               m2 = inf;
               // if both are static skip the collision
               if (m1 == inf) {
                  continue;
               }
            } else {
               auto& rb2 = entity2->get_component<Components::Rigidbody>();
               m2 = rb2.mass;
               v2 = rb2.velocity;
            }
            Frame<f32> tr2 = entity2->get_transform();
            auto& coll2 = entity2->get_component<Components::RectangleCollider>();
            Rectangle<f32> box2 = coll2.get_rect(tr2);
            // check for collision
            if (box1.overlaps(box2)) {
               if(!coll1.isSolid || !coll2.isSolid) {
                  continue;
               }
               // get the normal vectors
               auto [n1, n2] = Rectangle<f32>::get_normal_vectors(box1, box2);
               f32 e = coll2.bounciness * coll2.bounciness;
               f32 vn1 = v1 | n1;
               f32 vn2 = v2 | n2;
               // calculate the the new velocities
               if (vn1 > 0 && m1 != inf){
                  f32 un1;
                  if (m2 == inf) {
                     un1 = -e * vn1;
                  } else {
                     un1 = (m1*vn1 + m2*vn2 - m2*e*(vn1-vn2)) / (m1 + m2);
                  }
                  debug_log("Collision, u1=: " << un1 << " v1=: " << vn1);
                  auto& rb1 = entity1->get_component<Components::Rigidbody>();
                  rb1.velocity += n1 * (un1 - vn1);
               }
               if (vn2 > 0 && m2 != inf){
                  f32 un2;
                  if (m1 == inf) {
                     un2 = -e * vn2;
                  } else {
                     un2 = (m1*vn1 + m2*vn2 - m1*e*(vn2-vn1)) / (m1 + m2);
                  }
                  debug_log("Collision, u2=: " << un2 << " v2=: " << vn2);
                  auto& rb2 = entity2->get_component<Components::Rigidbody>();
                  rb2.velocity += n2 * (un2 - vn2);
               }
            }
         }
      }
   }

}