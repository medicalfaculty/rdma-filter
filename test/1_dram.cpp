#include "utils.h"
#include "dram_bf.h"
#include "dram_bbf.h"
#include "dram_ohbbf.h"

#include <iostream>
#include <unordered_set>
#include <chrono>

#define INSERT_COUNT        (1 << 26)
#define FALSE_POSITIVE_RATE (double)(1.0 / 512)
#define BLOCK_SIZE          4
#define LOOKUP_COUNT_MAX    (1 << 26)
#define REAL_INSERT_COUNT   (1 << 26)
#define SINGLE_ROUND_COUNT  (REAL_INSERT_COUNT / 20)

int main(int argc, char **argv) {

    std::cout << "==== Experiment Begin ====" << std::endl;
    int false_positive_count = 0, true_positive_count = 0, true_negative_count = 0;
    int lookup_count = 0;
    auto start_time = std::chrono::high_resolution_clock::now();
    auto end_time = start_time;
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << "= Dataset Preparing... =" << std::endl;
    std::vector<uint64_t> to_insert = {}, to_lookup = {}, lookup_temp = {};
    to_insert = GenerateRandom64(REAL_INSERT_COUNT);
    lookup_temp = GenerateRandom64(LOOKUP_COUNT_MAX);
    std::unordered_set<uint64_t> to_insert_set(to_insert.begin(), to_insert.end());
    for (auto i : lookup_temp) {
        if (to_insert_set.find(i) == to_insert_set.end()) {
            to_lookup.push_back(i);
        }
    }
    lookup_count = to_lookup.size();
    std::cout << "Prepared " << lookup_count << " negative items." << std::endl;

    // debug
    for (int i = 0; i < to_insert.size(); i++) {
        to_insert[i] = i;
    }
    for (int i = 0; i < to_lookup.size(); i++) {
        to_lookup[i] = i + to_insert.size();
    }
    

    std::cout << "=== DramBF Experiment ===" << std::endl;
    struct DramBF dram_bf;
    DramBF_init(&dram_bf, INSERT_COUNT, FALSE_POSITIVE_RATE);

    for (int i = 0; i < 20; i++) {
        start_time = std::chrono::high_resolution_clock::now();
        for (int j = 0; j < SINGLE_ROUND_COUNT; j++) {
            DramBF_insert(&dram_bf, to_insert[i * SINGLE_ROUND_COUNT + j]);
        }
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cout << "= Inserted " << SINGLE_ROUND_COUNT << " items =" << std::endl;
        std::cout << "Time(s): " << duration.count() / 1000.0 << std::endl;
        std::cout << "Throughput(op/s): " << SINGLE_ROUND_COUNT / duration.count() * 1000.0 << std::endl;

        std::cout << "== When Load " << (i + 1) * 5 << " percent elements ==" << std::endl;

        true_positive_count = 0;
        true_negative_count = 0;
        false_positive_count = 0;
        start_time = std::chrono::high_resolution_clock::now();
        for (int j = 0; j < SINGLE_ROUND_COUNT; j++) {
            if (DramBF_lookup(&dram_bf, to_insert[i * SINGLE_ROUND_COUNT + j])) {
                true_positive_count++;
            }
        }
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cout << "= Lookuped " << SINGLE_ROUND_COUNT << " existing items =" << std::endl;
        std::cout << "Time(s): " << duration.count() / 1000.0 << std::endl;
        std::cout << "Throughput(op/s): " << SINGLE_ROUND_COUNT / duration.count() * 1000.0 << std::endl;
        std::cout << "True Positive Count: " << true_positive_count << std::endl;
        std::cout << "True Positive Rate: " << 1.0 * true_positive_count / SINGLE_ROUND_COUNT << std::endl;

        start_time = std::chrono::high_resolution_clock::now();
        for (int j = 0; j < lookup_count; j++) {
            if (!DramBF_lookup(&dram_bf, to_lookup[j])) {
                true_negative_count++;
            } else {
                false_positive_count++;
            }
        }
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cout << "= Lookuped " << lookup_count << " non-existing items =" << std::endl;
        std::cout << "Time(s): " << duration.count() / 1000.0 << std::endl;
        std::cout << "Throughput(op/s): " << lookup_count / duration.count() * 1000.0 << std::endl;
        std::cout << "True Negative Count: " << true_negative_count << std::endl;
        std::cout << "True Negative Rate: " << 1.0 * true_negative_count / lookup_count << std::endl;
        std::cout << "False Positive Count: " << false_positive_count << std::endl;
        std::cout << "False Positive Rate: " << 1.0 * false_positive_count / lookup_count << std::endl;

    }

    DramBF_destroy(&dram_bf);


    // struct DramBBF dram_bbf;
    // DramBBF_init(&dram_bbf, INSERT_COUNT, FALSE_POSITIVE_RATE, BLOCK_SIZE);
    // for (auto i : to_insert) {
    //     DramBBF_insert(&dram_bbf, i);
    // }
    // std::cout << "DramBBF Inserted " << INSERT_COUNT << " items." << std::endl;
    // std::cout << "DramBBF lookup positive keys..." << std::endl;
    // for (auto i : to_insert) {
    //     if (DramBBF_lookup(&dram_bbf, i)) {
    //         true_positive_count++;
    //     }
    // }
    // std::cout << "DramBBF True Positive Rate: " << (double)true_positive_count / INSERT_COUNT << std::endl;
    // std::cout << "DramBBF lookup negative keys..." << std::endl;
    // for (auto i : to_lookup) {
    //     if (!DramBBF_lookup(&dram_bbf, i)) {
    //         true_negative_count++;
    //     } else {
    //         false_positive_count++;
    //     }
    // }
    // std::cout << "DramBBF True Negative Rate: " << (double)true_negative_count / LOOKUP_COUNT << std::endl;
    // std::cout << "DramBBF False Positive Rate: " << (double)false_positive_count / LOOKUP_COUNT << std::endl;

    // DramBBF_destroy(&dram_bbf);

    // false_positive_count = 0;
    // true_positive_count = 0;
    // true_negative_count = 0;
    // struct DramOHBBF dram_ohbbf;
    // DramOHBBF_init(&dram_ohbbf, INSERT_COUNT, FALSE_POSITIVE_RATE, BLOCK_SIZE);

    // for (auto i : to_insert) {
    //     DramOHBBF_insert(&dram_ohbbf, i);
    // }
    // std::cout << "DramOHBBF Inserted " << INSERT_COUNT << " items." << std::endl;
    // std::cout << "DramOHBBF lookup positive keys..." << std::endl;
    // for (auto i : to_insert) {
    //     if (DramOHBBF_lookup(&dram_ohbbf, i)) {
    //         true_positive_count++;
    //     }
    // }
    // std::cout << "DramOHBBF True Positive Rate: " << (double)true_positive_count / INSERT_COUNT << std::endl;
    // std::cout << "DramOHBBF lookup negative keys..." << std::endl;
    // for (auto i : to_lookup) {
    //     if (!DramOHBBF_lookup(&dram_ohbbf, i)) {
    //         true_negative_count++;
    //     } else {
    //         false_positive_count++;
    //     }
    // }
    // std::cout << "DramOHBBF True Negative Rate: " << (double)true_negative_count / LOOKUP_COUNT << std::endl;
    // std::cout << "DramOHBBF False Positive Rate: " << (double)false_positive_count / LOOKUP_COUNT << std::endl;

    // DramOHBBF_destroy(&dram_ohbbf);

    return 0;
}