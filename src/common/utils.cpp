#include <random>
#include "utils.h"

std::vector<::std::uint64_t> GenerateRandom64(::std::size_t count) {
  ::std::vector<::std::uint64_t> result(count);
  ::std::random_device random;
  // To generate random keys to lookup, this uses ::std::random_device which is slower but
  // stronger than some other pseudo-random alternatives. The reason is that some of these
  // alternatives (like libstdc++'s ::std::default_random, which is a linear congruential
  // generator) behave non-randomly under some hash families like Dietzfelbinger's
  // multiply-shift.
  auto genrand = [&random]() {
    return random() + (static_cast<::std::uint64_t>(random()) << 32);
  };
  ::std::generate(result.begin(), result.end(), ::std::ref(genrand));
  return result;
}