#ifndef __UTILS_H__
#define __UTILS_H__

#include <vector>
#include <cstdint>

std::vector<uint64_t> GenerateRandom64(std::size_t count);

// std::vector<uint64_t> GenerateRandomOpenSSL(std::size_t count);

int reliable_send(int sockfd, const void *data, std::size_t length);

int reliable_recv(int sockfd, void *data, std::size_t length);

#endif /* __UTILS_H__ */