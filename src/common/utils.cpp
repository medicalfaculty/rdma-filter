#include <random>
#include <algorithm>
#include <functional>
// #include <openssl/rand.h>
#include <stdexcept>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
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

int reliable_send(int sockfd, const void *data, std::size_t length) {
    std::size_t total_sent = 0;
    const char *buffer = static_cast<const char *>(data);

    while (total_sent < length) {
        std::size_t sent = send(sockfd, buffer + total_sent, length - total_sent, 0);
        if (sent < 0) {
            if (errno == EINTR) continue; // 被信号中断，重试
                perror("send failed");
                return -1;
        }
        else if (sent == 0) {
            std::cerr << "Connection closed by peer during send" << std::endl;
            return -1;
        }
        total_sent += sent;
    }
    return total_sent;
}

int reliable_recv(int sockfd, void *data, std::size_t length) {
    std::size_t total_received = 0;
    char *buffer = static_cast<char *>(data);

    while (total_received < length) {
        std::size_t received = recv(sockfd, buffer + total_received, length - total_received, 0);
        if (received < 0) {
            if (errno == EINTR) continue; // 被信号中断，重试
                perror("recv failed");
                return -1;
        }
        else if (received == 0) {
            std::cerr << "Connection closed by peer during recv" << std::endl;
            return -1;
        }
        total_received += received;
    }
    return total_received;
}