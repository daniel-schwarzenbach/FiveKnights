/*
   this file defines different geometric shapes and functions to work with them
*/
#pragma once
#include <core/Base.h++>
#include <core/container/Vec2.h++>

namespace Senpai {
   // simple 2D rectangle that cannot be rotated
   template <typename T>
   struct Rectangle {
      // the position of middle of the object
      Vec2<T> position;
      // the size of the object
      Vec2<T> size;

      bool overlaps(const Rectangle& other) const {
         return position.x < other.position.x + other.size.x &&
                position.x + size.x > other.position.x &&
                position.y < other.position.y + other.size.y &&
                position.y + size.y > other.position.y;
      }

      bool contains(Vec2<T> point) const {
         const T x2 = size.x/2.;
         const T y2 = size.y/2.;
         return position.x - x2 < point.x && point.x < position.x + x2 &&
                position.y  - y2 < point.y && point.y < position.y + y2;
      }

      Vec2<T> right_bottom() const {
         return {position.x + size.x/2, position.y - size.y/2};
      }

      Vec2<T> left_top() const {
         return {position.x - size.x/2, position.y + size.y/2};
      }

      Vec2<T> right_top() const {
         return {position.x + size.x/2, position.y + size.y/2};
      }

      Vec2<T> left_bottom() const {
         return {position.x - size.x/2, position.y - size.y/2};
      }

      Vec2<T> center() const {
         return position;
      }

      // get normal vectors in case of a collision
      static Tuple<Vec2<T>, Vec2<T>> get_normal_vectors(Rectangle const& r1, Rectangle const& r2) {
         // get the distance between the two rectangles relative to their size
         f32 dx = (r2.position.x - r1.position.x) / (r1.size.x + r2.size.x);
         f32 dy = r2.position.y - r1.position.y / (r1.size.y + r2.size.y);
         if(abs(dx) > abs(dy)) {
            if(dx > 0) {
               return {Vec2<T>{1, 0}, Vec2<T>{-1, 0}};
            } else {
               return {Vec2<T>{-1, 0}, Vec2<T>{1, 0}};
            }
         }
         else if(dy > 0) {
            return {Vec2<T>{0, 1}, Vec2<T>{0, -1}};
         } else {
            return {Vec2<T>{0, -1}, Vec2<T>{0, 1}};
         }
      }
   };

   template <typename T>
   Rectangle<T> operator*(Vec2<T> const& scale, Rectangle<T> const& rect) {
      return {rect.position * scale, rect.size * scale};
   }

   
   // A rotatable 2D rectangle
   template <typename T>
   struct Frame {
      // the position of the middle of the object
      Vec2<T> position = {0, 0};
      // size of the object
      Vec2<T> size = {1, 1};
      // the anchor point for rotation, relative to the position and scale
      Vec2<T> rotAnchor = {0, 0};
      // the rotation of the object ∈ [0, 360°)
      f32 rotation;

      Vec2<T> center() const {
         return position;
      }

      void scale(Vec2<T> const& scale) {
         size *= scale;
         rotAnchor *= scale;
      }

      void add_offset(Vec2<T> const& offset) {
         position += offset;
      }

      Frame apply_parrent_frame(Frame const& parent) const {
         Frame result;
         // relative position to the point of rotation
         Vec2<T> point_of_rotation = parent.rotAnchor * parent.size;
         Vec2<T> relativePos = position - point_of_rotation;
         // get rotation
         f32 sinA = 0, cosA = 1;
         // make a case distinction for more efficient calculation
         if(abs(parent.rotation - 180) < ε) {
            sinA = 0;
            cosA = -1;
         } else if(abs(parent.rotation - 90) < ε || abs(parent.rotation - 270) < ε) {
            sinA = parent.rotation < 180 ? 1 : -1;
            cosA = 0;
         } else {
            constexpr f32 degrefactor = π / 180.0f;
            sinA = sin(parent.rotation * degrefactor);
            cosA = cos(parent.rotation * degrefactor);
         }
         // apply rotation to relative position
         result.position = {
            (relativePos.x * cosA - relativePos.y * sinA),
            (relativePos.x * sinA + relativePos.y * cosA)
         };
         // apply parent position
         result.position += parent.position + parent.rotAnchor * parent.size;
         result.size = size*parent.size;
         result.rotAnchor = rotAnchor;
         result.rotation = rotation + parent.rotation;
         //result.scale(parent.size);
         return result;
      }

      // project a frame from a camera-rectangle to a canvas-rectangle
      Frame project(Rectangle<T> const& Camera, Rectangle<T> const& Canvas) const {
         // everythig should be scaled to the canvas
         f32 scalefactor =  Canvas.size.y / Camera.size.y;
         return Frame{
            .position = (position - Camera.position) * scalefactor + Canvas.position,
            .size = this->size * scalefactor,
            .rotAnchor = this->rotAnchor,
            .rotation = this->rotation
         };
      }
   };

   // Rectangle for Textures
   struct PixelArea {
      // left upper corner
      Vec2<f32> position = {0,0};
      // size in pixels
      Vec2<f32> size = {32, 32};
   };

   // linear interpolation between two Vec2
   template <typename T>
   Vec2<T> lin_interpolate(Vec2<T> const& a, Vec2<T> const& b, f32 t) {
      return a + (b - a) * t;
   }

   // get the angle in °
   inline f32 get_angle(Vec2<f32> vec) {
      constexpr f32 radToDegre = 180.0f / π;
      f32 angle = std::atan2(vec.y , -vec.x) * radToDegre;
      return angle;
   }

   // interpolate between to angle in degrees
   inline f32 interpolate_between_angles(f32 a, f32 b, f32 intFactor) {
      constexpr f32 degToRad = π / 180.0f;
      // interpolate their virtual positions
      Vec2<f32> α = {sin(a * degToRad), cos(a * degToRad)};
      Vec2<f32> β = {sin(b * degToRad), cos(b * degToRad)};
      Vec2<f32> γ = lin_interpolate(α,β,intFactor);
      // return the current angle
      return get_angle(γ);
   }
}