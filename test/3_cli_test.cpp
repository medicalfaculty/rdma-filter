#include "rdma_bf.h"
#include "rdma_bbf.h"
#include "rdma_ohbbf.h"
#include "utils.h"

#include <iostream>
#include <unordered_set>
#include <chrono>


#define SERVER_IP "10.10.1.1"
#define RNIC_NAME "mlx4_0"
#define RNIC_PORT (2)
#define GID_INDEX (2)
#define TCP_PORT (18515)

#define INSERT_COUNT (1 << 26)
#define FALSE_POSITIVE_RATE ((double)1.0 / 512)
#define MUTEX_GRAN (1024)     // 每个锁占 8 字节，每个锁锁了 (粒度) 字节

#define BLOCK_SIZE (4)
#define MUTEX_GRAN_BLOCK (256)

#define LOOKUP_COUNT (1 << 18)
#define REAL_INSERT_COUNT (1 << 18)

int main(int argc, char **argv) {

    std::cout << "==== Experiment Begin ====" << std::endl;
    int false_positive_count = 0, true_positive_count = 0, true_negative_count = 0;
    auto start_time = std::chrono::high_resolution_clock::now();
    auto end_time = start_time;
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << "= Dataset Preparing =" << std::endl;
    std::vector<uint64_t> to_insert = GenerateRandom64(REAL_INSERT_COUNT), to_lookup = {};
    std::unordered_set<uint64_t> to_insert_set(to_insert.begin(), to_insert.end());
    while (to_lookup.size() < LOOKUP_COUNT) {
        auto lookup_temp = GenerateRandom64(LOOKUP_COUNT - to_lookup.size());
        for (auto i : lookup_temp) {
            if (to_insert_set.find(i) == to_insert_set.end()) {
                to_lookup.push_back(i);
            }
        }
    }

    std::cout << "=== RdmaBF Experiment ===" << std::endl;
    struct RdmaBF_Cli rdma_bf_cli;
    RdmaBF_Cli_init(&rdma_bf_cli, INSERT_COUNT, FALSE_POSITIVE_RATE, SERVER_IP, RNIC_NAME, RNIC_PORT, TCP_PORT, GID_INDEX, MUTEX_GRAN);

    RdmaBF_Cli_insert(&rdma_bf_cli, 1);
    auto lookup_result = RdmaBF_Cli_lookup(&rdma_bf_cli, 2);
    std::cout << "Lookup result: " << lookup_result << std::endl;

    reliable_send(rdma_bf_cli.sockfd, "EXIT", 5);
    RdmaBF_Cli_destroy(&rdma_bf_cli);

    return 0;
}