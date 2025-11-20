#include "rdma_bf.h"
#include "rdma_bbf.h"
#include "rdma_ohbbf.h"
#include "utils.h"

#include <iostream>
#include <unordered_set>
#include <chrono>


#define SERVER_IP "10.10.1.1"
#define INSERT_COUNT (1 << 26)
#define FALSE_POSITIVE_RATE ((double)1.0 / 512)
#define BLOCK_SIZE (4)
#define LOOKUP_COUNT (1 << 16)

#define REAL_INSERT_COUNT (1 << 16)

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
    RdmaBF_Cli_init(&rdma_bf_cli, INSERT_COUNT, FALSE_POSITIVE_RATE, SERVER_IP);

    std::cout << "= Inserting =" << std::endl;
    start_time = std::chrono::high_resolution_clock::now();
    for (auto i : to_insert) {
        RdmaBF_Cli_insert(&rdma_bf_cli, i);
    }
    end_time = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Inserted " << REAL_INSERT_COUNT << " items." << std::endl;
    std::cout << "Time(s): " << duration.count() / 1000.0 << std::endl;

    // debug
    std::cout << "Type of duration.count(): " << typeid(duration.count()).name() << std::endl;

    std::cout << "Throughput(op/s): " << 1.0 * REAL_INSERT_COUNT / duration.count() * 1000.0 << std::endl;
    std::cout << "Payload-Bandwidth(MB/s): " << 1.0 * REAL_INSERT_COUNT * 2 * rdma_bf_cli.k / duration.count() * 1000.0 / 1024 / 1024 << std::endl;

    // std::cout << std::endl << "= Lookingup existing items =" << std::endl;
    // start_time = std::chrono::high_resolution_clock::now();
    // for (auto i : to_insert) {
    //     if (RdmaBF_Cli_lookup(&rdma_bf_cli, i)) {
    //         true_positive_count++;
    //     }
    // }
    // end_time = std::chrono::high_resolution_clock::now();
    // duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    // std::cout << "Lookup " << REAL_INSERT_COUNT << " existing items." <<  std::endl;
    // std::cout << "Time(s): " << duration.count() / 1000.0 << std::endl;
    // std::cout << "Throughput(op/s): " << REAL_INSERT_COUNT / duration.count() * 1000.0 << std::endl;
    // std::cout << "Payload-Bandwidth(MB/s): " << REAL_INSERT_COUNT * 1 * rdma_bf_cli.k / duration.count() * 1000.0 / 1024 / 1024 << std::endl;
    // std::cout << "True Positive Count: " << true_positive_count << std::endl;
    // std::cout << "True Positive Rate: " << (double)true_positive_count / REAL_INSERT_COUNT << std::endl;

    std::cout << std::endl << "= Lookingup non-existing items =" << std::endl;
    start_time = std::chrono::high_resolution_clock::now();
    for (auto i : to_lookup) {
        if (RdmaBF_Cli_lookup(&rdma_bf_cli, i)) {
            false_positive_count++;
            // debug
            std::cout << "Positive" << std::endl;
        } else {
            true_negative_count++;
            // debug
            std::cout << "Negative" << std::endl;
        }
    }
    end_time = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Lookup " << LOOKUP_COUNT << " non-existing items." <<  std::endl;
    std::cout << "Time(s): " << duration.count() / 1000.0 << std::endl;
    std::cout << "Throughput(op/s): " << LOOKUP_COUNT / duration.count() * 1000.0 << std::endl;
    std::cout << "True Negative Count: " << true_negative_count << std::endl;
    std::cout << "True Negative Rate: " << 1.0 * true_negative_count / LOOKUP_COUNT << std::endl;
    std::cout << "False Positive Count: " << false_positive_count << std::endl;
    std::cout << "False Positive Rate: " << 1.0 * false_positive_count / LOOKUP_COUNT << std::endl;
    

    reliable_send(rdma_bf_cli.sockfd, "EXIT", 5);
    RdmaBF_Cli_destroy(&rdma_bf_cli);

// ----------------------------------------------------------------------------------------------

    // struct RdmaBBF_Cli rdma_bbf_cli;
    // RdmaBBF_Cli_init(&rdma_bbf_cli, INSERT_COUNT, FALSE_POSITIVE_RATE, BLOCK_SIZE, SERVER_IP);

    // recv(rdma_bbf_cli.sockfd, cmd, 6, 0);

    // for (uint64_t i = 0; i < 10000; i++) {
    //     RdmaBBF_Cli_insert(&rdma_bbf_cli, i);
    // }

    // false_positive = 0;
    // for (uint64_t i = 10000; i < 20000; i++) {
    //     if (!RdmaBBF_Cli_lookup(&rdma_bbf_cli, i)) {
    //         ;
    //     }
    // }

    // send(rdma_bbf_cli.sockfd, "EXIT", 5, 0);
    // RdmaBBF_Cli_destroy(&rdma_bbf_cli);

// ----------------------------------------------------------------------------------------------

    // struct RdmaOHBBF_Cli rdma_ohbbf_cli;
    // RdmaOHBBF_Cli_init(&rdma_ohbbf_cli, INSERT_COUNT, FALSE_POSITIVE_RATE, BLOCK_SIZE, SERVER_IP);

    // recv(rdma_ohbbf_cli.sockfd, cmd, 6, 0);

    // for (uint64_t i = 0; i < 10000; i++) {
    //     RdmaOHBBF_Cli_insert(&rdma_ohbbf_cli, i);
    // }

    // false_positive = 0;
    // for (uint64_t i = 10000; i < 20000; i++) {
    //     if (!RdmaOHBBF_Cli_lookup(&rdma_ohbbf_cli, i)) {
    //         ;
    //     }
    // }

    // send(rdma_ohbbf_cli.sockfd, "EXIT", 5, 0);
    // RdmaOHBBF_Cli_destroy(&rdma_ohbbf_cli);

    return 0;
}