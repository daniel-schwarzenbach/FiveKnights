#pragma once
#include "../Base.h++"

/*
Simple mathematical 2D vector struct.
```cpp
   Vec2{x,y}
```
*/
template <typename T>
struct Vec2 {
   T x, y;

   inline Vec2 operator+(const Vec2& other) const {
      return Vec2(x + other.x, y + other.y);
   }

   inline Vec2 operator-(const Vec2& other) const {
      return Vec2(x - other.x, y - other.y);
   }

   template <typename T2>
   inline Vec2 operator*(const Vec2<T2>& other) const {
      return Vec2(x * other.x, y * other.y);
   }

   inline Vec2 operator/(const Vec2& other) const {
      return Vec2(x / other.x, y / other.y);
   }

   inline Vec2 operator+(T scalar) const {
      return Vec2(x + scalar, y + scalar);
   }

   inline Vec2 operator-(T scalar) const {
      return Vec2(x - scalar, y - scalar);
   }

   inline Vec2 operator*(T scalar) const {
      return Vec2(x * scalar, y * scalar);
   }

   inline Vec2 operator/(T scalar) const {
      return Vec2(x / scalar, y / scalar);
   }

   // scalar product
   inline T operator|(Vec2 const& other) const {
      return x * other.x + y * other.y;
   }

   // cross product
   inline T operator%(Vec2 const& other) const {
      return x * other.y - y * other.x;
   }

   inline Vec2& operator+=(const Vec2& other) {
      x += other.x;
      y += other.y;
      return *this;
   }

   inline Vec2& operator-=(const Vec2& other) {
      x -= other.x;
      y -= other.y;
      return *this;
   }

   inline Vec2& operator*=(const Vec2& other) {
      x *= other.x;
      y *= other.y;
      return *this;
   }

   inline Vec2& operator/=(const Vec2& other) {
      x /= other.x;
      y /= other.y;
      return *this;
   }

   inline Vec2& operator+=(T scalar) {
      x += scalar;
      y += scalar;
      return *this;
   }

   inline Vec2& operator-=(T scalar) {
      x -= scalar;
      y -= scalar;
      return *this;
   }

   inline Vec2& operator*=(T scalar) {
      x *= scalar;
      y *= scalar;
      return *this;
   }

   inline Vec2& operator/=(T scalar) {
      x /= scalar;
      y /= scalar;
      return *this;
   }

   template <typename T2>
   Vec2<T>& operator=(const Vec2<T2>& other) {
      x = T(other.x);
      y = T(other.y);
      return *this;
   }

   template <typename T2>
   bool operator==(const Vec2<T2>& other) const {
      return x == other.x && y == other.y;
   }

   template <typename T2>
   bool similar_to(Vec2<T2>& other) const {
      return abs(x - other.x) < ε && abs(y - other.y) < ε;
   }
};

template <typename T1, typename T2>
Vec2<T1> convert(Vec2<T2> const& other) {
   return Vec2<T1>{T1(other.x), T1(other.y)};
}

template <typename T>
inline OS& operator<<(OS& os, const Vec2<T>& vec) {
   os << "Vec2{" << vec.x << ", " << vec.y << "}";
   return os;
}

template <typename T>
inline IS& operator>>(IS& is, Vec2<T>& vec) {
   char Vec2, comma, end;
   is >> "Vec2{" >> vec.x >> "," >> vec.y >> "}";
   return is;
}