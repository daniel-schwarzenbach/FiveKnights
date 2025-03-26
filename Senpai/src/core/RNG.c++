#include <random>
#include <limits>
#include <core/RNG.h++>

namespace Senpai
{
   namespace rng{
      std::random_device rd;
      std::mt19937 generator(rd());
      std::uniform_real_distribution<f32> f32_dist(0.0, 1.0);
      std::uniform_int_distribution<UInt> UInt_dist(1, std::numeric_limits<UInt>::max());
      std::normal_distribution<f32> f32_gaussian_dist(0.0, 1.0);

      // x ∈ [0,1)
      f32 get(){
         return f32_dist(generator);
      }
      // get a random Adress
      int get_in_range(int a, int b) {
         std::uniform_int_distribution<int> dist(a, b);
         return dist(generator);
      }

      // x ∈ [0,∞)
      f32 get_gaussian(){
         return f32_gaussian_dist(generator);
      }
   };
} // namespace Senpai