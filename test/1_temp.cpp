#include "utils.h"
#include "dram_bf.h"
#include "dram_bbf.h"
#include "dram_ohbbf.h"

#include <iostream>
#include <unordered_set>

#define INSERT_COUNT (1 << 26)
#define LOOKUP_COUNT (1 << 20)
#define FALSE_POSITIVE_RATE (double)1.0 / 512
#define BLOCK_SIZE 4

int main(int argc, char **argv) {
    int false_positive_count = 0, true_positive_count = 0, true_negative_count = 0;

    std::vector<uint64_t> to_insert = GenerateRandom64(INSERT_COUNT), to_lookup = {};
    std::unordered_set<uint64_t> to_add_set(to_insert.begin(), to_insert.end());
    for (uint64_t i = 0; to_lookup.size() < LOOKUP_COUNT; i++) {
        if (to_add_set.find(i) == to_add_set.end()) {
            to_lookup.push_back(i);
        }
    }

    struct DramBF dram_bf;
    DramBF_init(&dram_bf, INSERT_COUNT, FALSE_POSITIVE_RATE);
    for (auto i : to_insert) {
        DramBF_insert(&dram_bf, i);
    }
    std::cout << "DramBF Inserted " << INSERT_COUNT << " items." << std::endl;
    std::cout << "DramBF lookup positive keys..." << std::endl;
    for (auto i : to_insert) {
        if (DramBF_lookup(&dram_bf, i)) {
            true_positive_count++;
        }
    }
    std::cout << "DramBF True Positive Rate: " << (double)true_positive_count / INSERT_COUNT << std::endl;
    std::cout << "DramBF lookup negative keys..." << std::endl;
    for (auto i : to_lookup) {
        if (!DramBF_lookup(&dram_bf, i)) {
            true_negative_count++;
        } else {
            false_positive_count++;
        }
    }
    std::cout << "DramBF True Negative Rate: " << (double)true_negative_count / LOOKUP_COUNT << std::endl;
    std::cout << "DramBF False Positive Rate: " << (double)false_positive_count / LOOKUP_COUNT << std::endl;    

    DramBF_destroy(&dram_bf);

    false_positive_count = 0;
    true_positive_count = 0;
    true_negative_count = 0;
    struct DramBBF dram_bbf;
    DramBBF_init(&dram_bbf, INSERT_COUNT, FALSE_POSITIVE_RATE, BLOCK_SIZE);
    for (auto i : to_insert) {
        DramBBF_insert(&dram_bbf, i);
    }
    std::cout << "DramBBF Inserted " << INSERT_COUNT << " items." << std::endl;
    std::cout << "DramBBF lookup positive keys..." << std::endl;
    for (auto i : to_insert) {
        if (DramBBF_lookup(&dram_bbf, i)) {
            true_positive_count++;
        }
    }
    std::cout << "DramBBF True Positive Rate: " << (double)true_positive_count / INSERT_COUNT << std::endl;
    std::cout << "DramBBF lookup negative keys..." << std::endl;
    for (auto i : to_lookup) {
        if (!DramBBF_lookup(&dram_bbf, i)) {
            true_negative_count++;
        } else {
            false_positive_count++;
        }
    }
    std::cout << "DramBBF True Negative Rate: " << (double)true_negative_count / LOOKUP_COUNT << std::endl;
    std::cout << "DramBBF False Positive Rate: " << (double)false_positive_count / LOOKUP_COUNT << std::endl;

    DramBBF_destroy(&dram_bbf);

    false_positive_count = 0;
    true_positive_count = 0;
    true_negative_count = 0;
    struct DramOHBBF dram_ohbbf;
    DramOHBBF_init(&dram_ohbbf, INSERT_COUNT, FALSE_POSITIVE_RATE, BLOCK_SIZE);

    for (auto i : to_insert) {
        DramOHBBF_insert(&dram_ohbbf, i);
    }
    std::cout << "DramOHBBF Inserted " << INSERT_COUNT << " items." << std::endl;
    std::cout << "DramOHBBF lookup positive keys..." << std::endl;
    for (auto i : to_insert) {
        if (DramOHBBF_lookup(&dram_ohbbf, i)) {
            true_positive_count++;
        }
    }
    std::cout << "DramOHBBF True Positive Rate: " << (double)true_positive_count / INSERT_COUNT << std::endl;
    std::cout << "DramOHBBF lookup negative keys..." << std::endl;
    for (auto i : to_lookup) {
        if (!DramOHBBF_lookup(&dram_ohbbf, i)) {
            true_negative_count++;
        } else {
            false_positive_count++;
        }
    }
    std::cout << "DramOHBBF True Negative Rate: " << (double)true_negative_count / LOOKUP_COUNT << std::endl;
    std::cout << "DramOHBBF False Positive Rate: " << (double)false_positive_count / LOOKUP_COUNT << std::endl;

    DramOHBBF_destroy(&dram_ohbbf);

    return 0;
}