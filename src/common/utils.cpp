#include <random>
#include <algorithm>
#include <functional>
// #include <openssl/rand.h>
#include <stdexcept>
#include "utils.h"

std::vector<uint64_t> GenerateRandom64Cuckoo(std::size_t count) {
  std::vector<uint64_t> result(count);
  std::random_device random;
  // To generate random keys to lookup, this uses ::std::random_device which is slower but
  // stronger than some other pseudo-random alternatives. The reason is that some of these
  // alternatives (like libstdc++'s ::std::default_random, which is a linear congruential
  // generator) behave non-randomly under some hash families like Dietzfelbinger's
  // multiply-shift.
  auto genrand = [&random]() {
    return random() + (static_cast<uint64_t>(random()) << 32);
  };
  std::generate(result.begin(), result.end(), std::ref(genrand));
  return result;
}

// Deepseek + Kimi
std::vector<uint64_t> GenerateRandom64(std::size_t count) {
    std::mt19937_64 generator(std::random_device{}());
    
    std::vector<uint64_t> result(count);
    std::generate(result.begin(), result.end(), std::ref(generator));
    return result;
}

// std::vector<uint64_t> GenerateRandomOpenSSL(std::size_t count) {
//     std::vector<uint64_t> vals(count);
//     if (RAND_bytes((unsigned char *)vals.data(), sizeof(uint64_t) * count) != 1) {
//         throw std::runtime_error("OpenSSL RAND_bytes failed");
//     }
//     return vals;
// }