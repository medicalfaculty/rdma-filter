#ifndef __BLOOMFILTER_H__
#define __BLOOMFILTER_H__

#include <stdint.h>
#include <stdlib.h>

struct bloomfilter
{
    unsigned int m;
    unsigned int k;
    uint8_t *bit_vector;
};

void bloomfilter_init(struct bloomfilter *bloomfilter, unsigned int n, double fpr);

void bloomfilter_destroy(struct bloomfilter *bloomfilter);

int bloomfilter_insert(struct bloomfilter *bloomfilter, uint64_t key);

int bloomfilter_lookup(struct bloomfilter *bloomfilter, uint64_t key);

int bloomfilter_bytes(struct bloomfilter *bloomfilter);

void bloomfilter_info(struct bloomfilter *bloomfilter);

#endif /* __BLOOMFILTER_H__ */