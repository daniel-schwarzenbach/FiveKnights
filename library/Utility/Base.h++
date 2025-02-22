#include <type_traits>
#include <cmath>
#include <numeric>

namespace std
{
    #include <stdint.h>
} // namespace std

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

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

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
Int mod(Int a, Int b) {
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

//#define NDEBUG
#ifndef NDEBUG
// on Debugging
    #define debug_assert(expr, err) if(!(expr))\ 
            throw String("[DEBUG]: ") + String(err);
    #define debug_log(message) cout << "[DEBUG]: " << message << endl;  
// End Debugging
#else
// No Debugging
    #define debug_assert(expr, err) // noghting
    #define debug_log(message) // nothing
// End No Debugging
#endif

// assert that doesnt depend on Debug mode
void always_assert(bool expression, String $ errormessage = "") {
    if(!expression){
        throw errormessage;
    }
}