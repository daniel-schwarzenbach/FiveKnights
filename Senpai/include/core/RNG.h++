#include <core/Base.h++>

namespace Senpai {
namespace rng {
// x ∈ [0,1)
f32 get();
// get a random Adress
int get_in_range(int a, int b);
// x ∈ [0,∞)
f32 get_gaussian();
};  // namespace rng
}  // namespace Senpai
