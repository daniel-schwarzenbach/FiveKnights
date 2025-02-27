#pragma once
#include <type_traits>
#include <cmath>
#include <numeric>
#include <stdint.h>

#define let(...) template<typename __VA_ARGS__>
#define $ const&

template<typename T>
concept Any = true;

template <typename T, typename U>
concept Convertible = requires(U&& u) {
  static_cast<T>(std::forward<U>(u));
};

// Functions
template<typename T>
concept Function = std::is_function_v<T>;

// Numbers
let(T)
concept Reel = std::is_integral_v<T> || std::is_floating_point_v<T>;


// Integer
template<typename T>
concept Integer = std::is_integral_v<T>;


using Int = signed long;
// replacement for size_t
using UInt = unsigned long;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

template<typename T>
concept Float = std::is_floating_point_v<T>;

using f32 = float;
using f64 = double;



// Pointers
#include <memory>

using std::move;

let(T)
using Ptr = T*;

let(T)
using UniquePtr = std::unique_ptr<T>;

let(T)
using SharedPtr = std::shared_ptr<T>;

#include <iostream>
using IS = std::istream;
using OS = std::ostream;
using std::cout;
using std::cin;
using std::endl;

#include <string>
using String = std::string;
using std::to_string;

#include <sstream>
template <typename T>
String str(T const& in) {
    std::ostringstream oss;
    oss << in;
    return oss.str();      
}

#include <tuple>

template<typename... Types>
using Tuple = std::tuple<Types...>;


// for loop macros
#define for_(i,n) for(UInt i = 1; i <= static_cast<UInt>(n); i++)
#define for_wards(i,a,b) for(Int i = static_cast<Int>(a); i <= static_cast<Int>(b); i++)
#define back_wards(i,a,b) for(Int i = static_cast<Int>(a); i >= static_cast<Int>(b); i--)

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
constexpr T min(const T& first, const Args&... args) {
    T m = first;
    ([&]() { if (m > args) m = args; }(), ...);
    return m;
}

template <typename T, typename... Args>
constexpr T max(const T& first, const Args&... args) {
    T m = first;
    ([&]() { if (m < args) m = args; }(), ...);
    return m;
}

#include <exception>
using Exception = std::exception;

//#define NDEBUG
#ifndef NDEBUG
// on Debugging
    #define debug_assert(expr, err) if(!(expr))\ 
            throw Exception(String("[DEBUG]: ") + String(err));
    #define debug_log(message) cout << "[DEBUG]: " << message << endl;
// End Debugging
#else
// No Debugging
    #define debug_assert(expr, err) // noghting
    #define debug_log(message) // nothing
// End No Debugging
#endif

// assert that doesnt depend on Debug mode
constexpr void always_assert(bool expression, String $ errormessage = "") {
    if(!expression){
        throw errormessage;
    }
}

template<typename T>
concept Container = requires(T a, typename T::value_type val) {
    typename T::value_type;
    typename T::iterator;
    { a.begin() } -> std::same_as<typename T::iterator>;
    { a.end() } -> std::same_as<typename T::iterator>;
    { a.size() } -> std::convertible_to<UInt>;
};

template<Container C> 
void fill(C& toFill, typename C::value_type const& constant){
    for(auto& element: toFill){
        element = constant;
    }
}