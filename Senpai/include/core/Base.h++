#pragma once
#include <cmath>
#include <numeric>
#include <stdint.h>
#include <type_traits>

template <typename T>
concept AnyType = true;

template <typename T, typename U>
concept Convertible = requires(U &&u) { static_cast<T>(std::forward<U>(u)); };

// Functions
template <typename T>
concept FunctionType = std::is_function_v<T>;

// Numbers
template <typename T>
concept ReelType = std::is_integral_v<T> || std::is_floating_point_v<T>;

// Integer
template <typename T>
concept IntType = std::is_integral_v<T>;

using Int = signed long;
// replacement for size_t
using UInt = unsigned long;
using uint = unsigned int;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

template <typename T>
concept Float = std::is_floating_point_v<T>;

using f32 = float;
using f64 = double;

#include <functional>

template <typename T> using Function = std::function<T>;

// Pointers
#include <memory>

using std::move;

template <typename T> using Ptr = T *;

using GenericPtr = void *;

template <typename T> using UniquePtr = std::unique_ptr<T>;

template <typename T> using SharedPtr = std::shared_ptr<T>;

using std::make_shared;
using std::make_unique;
using std::move;
using std::swap;

#include <iostream>
using IS = std::istream;
using OS = std::ostream;
using std::cerr;
using std::cin;
using std::cout;
using std::endl;

// IS& operator>>(IS& is, Ptr<const char>);
IS &operator>>(IS &is, const char toRead[]);
IS &operator>>(IS &is, const char *toRead);

#include <string>
using String = std::string;
using std::to_string;

#include <sstream>
template <typename T> String str(T const &in) {
   std::ostringstream oss;
   oss << in;
   return oss.str();
}

#include <tuple>

template <typename... Types> using Tuple = std::tuple<Types...>;

template <typename T1,typename T2> using Pair = std::pair<T1, T2>;

// mathmatical mod
constexpr Int mod(Int a, Int b) {
   // Handle negative values of a
   Int mod = a % b;
   if (mod < 0) {
      mod += b;
   }
   return mod;
}

template <typename T, typename... Args>
constexpr T min(const T &first, const Args &...args) {
   T m = first;
   (
       [&]() {
          if (m > args)
             m = args;
       }(),
       ...);
   return m;
}

template <typename T, typename... Args>
constexpr T max(const T &first, const Args &...args) {
   T m = first;
   (
       [&]() {
          if (m < args)
             m = args;
       }(),
       ...);
   return m;
}

#include <exception>
using Exception = std::exception;

class SenpaiException : public Exception {
 private:
   String message;

 public:
   inline SenpaiException(String const &message)
       : Exception(), message(message) {}
   inline const char *what() const noexcept override { return message.c_str(); }
};

// #define NDEBUG
#ifndef NDEBUG
// on Debugging
#define debug_assert(expr, err)                                                \
   if (!(expr)) throw SenpaiException(String("[DEBUG]: ") + String(err));
#define debug_log(message) cout << "\033[1;32m" << "[Log]: " << message << "\033[0m" << endl;
#define debug_warning(message) cout << "\033[1;33m" << "[Warning]: " << message << "\033[0m" << endl;
#define debug_error(message) cout << "\033[1;31m" << "[Error]: " << message << "\033[0m" << endl;
// End Debugging
#else
// No Debugging
#define debug_assert(expr, err) // noghting
#define debug_log(message)      // nothing
#define debug_warning(message)      // nothing
#define debug_error(message)      // nothing
// End No Debugging
#endif

// assert that doesnt depend on Debug mode
constexpr void always_assert(bool expression, String const &errormessage = "") {
   if (!expression) {
      throw errormessage;
   }
}

template <typename T>
concept Container = requires(T a, typename T::value_type val) {
   typename T::value_type;
   typename T::iterator;
   { a.begin() } -> std::same_as<typename T::iterator>;
   { a.end() } -> std::same_as<typename T::iterator>;
   { a.size() } -> std::convertible_to<UInt>;
};

template <Container C>
void fill(C &toFill, typename C::value_type const &constant) {
   for (auto &element : toFill) {
      element = constant;
   }
}

#include <list>

template <typename T> using List = std::list<T>;

#include <deque>

template <typename T> using Deque = std::deque<T>;

#include <set>

template <typename T> using Set = std::set<T>;

#include <unordered_map>

template <typename T, typename U> using Map = std::unordered_map<T, U>;

#include <bitset>

template <UInt N> using Bitset = std::bitset<N>;

namespace Senpai {
template <typename T> constexpr u32 get_subtype_id() {
   static u32 id = 0u;
   return id++;
}
}; // namespace Senpai

static constexpr f32 ε = 1e-5f;
static constexpr f32 π = 3.14159265358979323846f;
static constexpr f32 inf = std::numeric_limits<f32>::infinity();
using std::abs;
using std::cos;
using std::sin;
using std::round;
using std::pow;