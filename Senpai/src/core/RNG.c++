#include <random>
#include <limits>
#include <core/RNG.h++>

namespace Senpai
{
   namespace rng{
      std::random_device rd;
      std::mt19937 generator(rd());
      std::uniform_real_distribution<f64> f64_dist(0.0, 1.0);
      std::uniform_int_distribution<UInt> UInt_dist(1, std::numeric_limits<UInt>::max());

      // x ∈ [0,1)
      f64 get_f64(){
         return f64_dist(generator);
      }
      // get a random Adress
      UInt get_UInt(){
         return UInt_dist(generator);
      }
   };
} // namespace Senpai