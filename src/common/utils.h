#ifndef __UTILS_H__
#define __UTILS_H__

#include <vector>
#include <cstdint>
#include <string>

std::vector<uint64_t> GenerateRandom64(std::size_t count);

// std::vector<uint64_t> GenerateRandomOpenSSL(std::size_t count);

int reliable_send(int sockfd, const void *data, std::size_t length);

int reliable_recv(int sockfd, void *data, std::size_t length);

void assert_else(bool condition, const std::string& message);

#endif /* __UTILS_H__ */