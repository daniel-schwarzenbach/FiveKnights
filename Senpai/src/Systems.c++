#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <core/Core.h++>
#include <sdl/SDLBackend.h++>
#include <systems/Systems.h++>

namespace Senpai {

void Systems::TestSystem::update(f32 Δt) {
   // Define rectangles
   SDL_FRect outlineRect = {100, 100, 200, 150}; // x, y, width, height
   SDL_FRect filledRect = {350, 100, 200, 150};

   // Draw outlined rectangle (red)
   SDL_SetRenderDrawColor(Renderer::get(), 255, 0, 0, 255);
   SDL_RenderRect(Renderer::get(), &outlineRect);

   // Draw filled rectangle (blue)
   SDL_SetRenderDrawColor(Renderer::get(), 0, 0, 255, 255);
   SDL_RenderFillRect(Renderer::get(), &filledRect);
   SDL_SetRenderDrawColor(Renderer::get(), 0, 0, 0, 255);
}

void Systems::CameraRenderSystem::update(f32 Δt) {
   for (auto &entityPtr : view<Components::Camera>()) {
      auto &camera = entityPtr->get_component<Components::Camera>();
      auto cameraRect = camera.get_rect(entityPtr->get_transform());
      auto canvas = camera.get_canvas();
      SDL_Rect viewport = SDL::to_sdl_viewport(camera.viewport);
      // SDL_SetRenderViewport(Renderer::get(), &viewport);
      for (auto &toRender : this->scenePtr->ecRegistry.componentsToRender) {
         toRender->render(cameraRect, canvas);
      }
   }
}

void Systems::UIRenderSystem::update(f32 Δt) {
   for (auto &toRender : this->scenePtr->ecRegistry.uiComponents) {
      toRender->render();
   }
}

void Systems::UIButtonSystem::update(f32 Δt) {
   Vec2<f32> mousePos = this->scenePtr->inputPtr->get_mouse_position();
   for (auto &entity : view<Components::ButtonUI>()) {
      auto &button = entity->get_component<Components::ButtonUI>();
      if (button.is_on_button(mousePos)) {
         button.hoverstate = true;
         if (this->scenePtr->inputPtr->get_mouse_button(1)) {
            if (entity->has_component<Components::ScriptsHolder>()) {
               auto &scriptsComp =
                   entity->get_component<Components::ScriptsHolder>();
               for (auto &script : scriptsComp.scripts) {
                  script->on_button_click();
               }
            }
         }
      } else {
         button.hoverstate = false;
      }
   }
}

void Systems::SpriteAnimator::start() {
   this->update(0);
}

void Systems::SpriteAnimator::update(f32 Δt) {
   for (auto entity : view<Components::Animator>()) {
      if (!entity->has_component<Components::Sprite>()) {
         debug_log("Entity with Animator does not have a Sprite component!");
         continue;
      }
      auto &animator = entity->get_component<Components::Animator>();
      auto &sprite = entity->get_component<Components::Sprite>();

      if (animator.flip_frame(Δt)) {
         auto animationPtr = animator.animations[animator.animId];
         ++animator.frameId;
         if (animator.frameId >= animationPtr->frameAreas.size()) {
            uint nextId = animator.nextAnimId[animator.animId];
            if (nextId != animator.animId) {
               animator.switch_animation(nextId);
            } else {
               animator.frameId = 0; 
            }
         } 
         sprite.scrArea = animationPtr->frameAreas[animator.frameId];
      }

      if (animator.has_switched) {
         animator.has_switched = false;
         sprite.scrArea = animator.animations[animator.animId]->frameAreas[0];
         sprite.texturePtr = animator.animations[animator.animId]->texturePtr;
      }
   }
}

void Systems::Physics::update(f32 Δt) {
   for (auto &entity : view<Components::Rigidbody>()) {
      if (!entity->has_component<Components::Transform>()) {
         debug_log(
             "Entity with Rigidbody does not have a Transform component!");
         continue;
      }
      auto &tr = entity->get_component<Components::Transform>();
      auto &rb = entity->get_component<Components::Rigidbody>();
      rb.velocity += rb.acceleration * Δt;
      // apply gravity
      rb.velocity.y -= gravity * Δt;
      tr.frame.position += rb.velocity * Δt;
      entity->get_transform().add_offset(rb.velocity * Δt);
   }
}

// O(n²) collision detection
void Systems::Collisions::update(f32 Δt) {
   auto entityPtrs = view<Components::RectangleCollider>();
   for (UInt i = 0; i < entityPtrs.size(); i++) {
      auto &entity1 = entityPtrs[i];
      // get velocity and mass of the entity
      bool s1, s2;
      f32 m1, m2;
      Vec2<f32> v1, v2;
      if (!entity1->has_component<Components::Rigidbody>()) {
         v1 = {0, 0};
         m1 = inf;
      } else {
         auto &rb1 = entity1->get_component<Components::Rigidbody>();
         m1 = rb1.mass;
         v1 = rb1.velocity;
      }
      Frame<f32> tr1 = entity1->get_transform();
      auto &coll1 = entity1->get_component<Components::RectangleCollider>();
      Rectangle<f32> box1 = coll1.get_rect(tr1);
      // check for collision with other colliders
      for (UInt j = i + 1; i < entityPtrs.size(); j++) {
         auto &entity2 = entityPtrs[j];
         if (!entity2->has_component<Components::Rigidbody>()) {
            v2 = {0, 0};
            m2 = inf;
            // if both are static skip the collision
            if (m1 == inf) {
               continue;
            }
         } else {
            auto &rb2 = entity2->get_component<Components::Rigidbody>();
            m2 = rb2.mass;
            v2 = rb2.velocity;
         }
         Frame<f32> tr2 = entity2->get_transform();
         auto &coll2 = entity2->get_component<Components::RectangleCollider>();
         Rectangle<f32> box2 = coll2.get_rect(tr2);
         // check for collision
         if (box1.overlaps(box2)) {
            // call the collision scripts
            if (entity1->has_component<Components::ScriptsHolder>()) {
               auto &scriptComp =
                   entity1->get_component<Components::ScriptsHolder>();
               for (auto &script : scriptComp.scripts) {
                  script->on_collision(entity2);
               }
            }
            if (entity2->has_component<Components::ScriptsHolder>()) {
               auto &scriptsComp =
                   entity2->get_component<Components::ScriptsHolder>();
               for (auto &script : scriptsComp.scripts) {
                  script->on_collision(entity1);
               }
            }
            if (!coll1.isSolid || !coll2.isSolid) {
               continue;
            }
            // get the normal vectors
            auto [n1, n2] = Rectangle<f32>::get_normal_vectors(box1, box2);
            f32 e = coll2.bounciness * coll2.bounciness;
            f32 vn1 = v1 | n1;
            f32 vn2 = v2 | n2;
            // calculate the the new velocities
            if (vn1 > 0 && m1 != inf) {
               f32 un1;
               if (m2 == inf) {
                  un1 = -e * vn1;
               } else {
                  un1 =
                      (m1 * vn1 + m2 * vn2 - m2 * e * (vn1 - vn2)) / (m1 + m2);
               }
               debug_log("Collision, u1=: " << un1 << " v1=: " << vn1);
               auto &rb1 = entity1->get_component<Components::Rigidbody>();
               rb1.velocity += n1 * (un1 - vn1);
            }
            if (vn2 > 0 && m2 != inf) {
               f32 un2;
               if (m1 == inf) {
                  un2 = -e * vn2;
               } else {
                  un2 =
                      (m1 * vn1 + m2 * vn2 - m1 * e * (vn2 - vn1)) / (m1 + m2);
               }
               debug_log("Collision, u2=: " << un2 << " v2=: " << vn2);
               auto &rb2 = entity2->get_component<Components::Rigidbody>();
               rb2.velocity += n2 * (un2 - vn2);
            }
         }
      }
   }
}

void Systems::ScriptRunner::start() {
   for (auto &entity : view<Components::ScriptsHolder>()) {
      auto &scriptsComponent =
          entity->get_component<Components::ScriptsHolder>();
      for (auto &script : scriptsComponent.scripts) {
         script->prepare(scenePtr, entity);
         script->on_start();
      }
   }
}

void Systems::ScriptRunner::update(f32 Δt) {
   for (auto &entity : view<Components::ScriptsHolder>()) {
      auto &scriptsComponent =
          entity->get_component<Components::ScriptsHolder>();
      for (auto &script : scriptsComponent.scripts) {
         script->on_update(Δt);
      }
   }
}

void Systems::LightingSystem::update(f32 Δt) {
   for (auto &entityPtr : view<Components::Camera>()) {
      auto &camera = entityPtr->get_component<Components::Camera>();
      auto cameraRect = camera.get_rect(entityPtr->get_transform());
      auto canvas = camera.get_canvas();
      SDL_Rect viewport = SDL::to_sdl_viewport(camera.viewport);
      //SDL_SetRenderViewport(Renderer::get(), &viewport);
      auto wSize = Window::get_size();
      SDL_Texture* lightTexture = SDL_CreateTexture(Renderer::get(),
                                             SDL_PIXELFORMAT_RGBA8888,
                                             SDL_TEXTUREACCESS_TARGET,
                                             wSize.x,
                                             wSize.y);
      if(!lightTexture) {
         debug_log("Failed to create light texture: " << SDL_GetError());
         return;
      }
      // Set the render target to the light texture.
      if(!SDL_SetRenderTarget(Renderer::get(), lightTexture)) {
         debug_log("Failed to set render target: " << SDL_GetError());
         SDL_DestroyTexture(lightTexture);
         return;
      }
      // Clear the light texture.
      SDL_SetRenderDrawColor(Renderer::get(), 0, 0, 0, 255);
      SDL_RenderFillRect(Renderer::get(), nullptr);
      // Render the lights.
      for (auto &entity : view<Components::Light>()) {
         Frame<f32> tr = entity->get_transform();
         auto &light = entity->get_component<Components::Light>();
         auto frame = light.get_frame(tr);
         auto projection = frame.project(cameraRect, canvas);
         SDL::RenderFrame(projection, (SDL_Texture *)light.texturePtr->get_sdl_texture(), light.flip, SDL_BLENDMODE_ADD, light.scrArea);
      }
      // Reset the render target to the window.
      SDL_SetRenderTarget(Renderer::get(), nullptr);
      // Set the blend mode to modulate the light texture.
      SDL_SetTextureBlendMode(lightTexture, SDL_BLENDMODE_MOD);
      // Render the light texture to the window.
      SDL_RenderTexture(Renderer::get(), lightTexture, nullptr, nullptr);
      // Free the light texture.
      SDL_DestroyTexture(lightTexture);
   }
}

} // namespace Senpai