#include "bloomfilter.h"

#include <iostream>
#include <math.h>
#include <string.h>
#include <unistd.h>

#include "common/murmur3.h"

using namespace std;

#define bit_set(v, n) ((v)[(n) >> 3] |= (0x1 << (0x7 - ((n)&0x7))))
#define bit_get(v, n) ((v)[(n) >> 3] & (0x1 << (0x7 - ((n)&0x7))))
#define bit_clr(v, n) ((v)[(n) >> 3] &= ~(0x1 << (0x7 - ((n)&0x7))))

void bloomfilter_init(struct bloomfilter *bloomfilter, unsigned int n, double fpr)
{
    double m = ((-1.0) * n * log(fpr)) / ((log(2)) * (log(2)));
    double k = (1.0 * m * log(2)) / n;

    // cout << "k:" << k << endl;

    memset(bloomfilter, 0, sizeof(*bloomfilter));
    bloomfilter->m = ((int(m) >> 3) + 1) << 3;
    bloomfilter->k = ceil(k);
    bloomfilter->bit_vector = (uint8_t *)calloc(bloomfilter->m >> 3, sizeof(uint8_t));

    return;
}

void bloomfilter_destroy(struct bloomfilter *bloomfilter)
{
    free(bloomfilter->bit_vector);
}

int bloomfilter_insert(struct bloomfilter *bloomfilter, uint64_t key_)
{
    void *key = &key_;

    for (uint32_t i = 0; i < bloomfilter->k; i++)
    {
        uint32_t hash;
        murmur3_hash32(key, 8, i, &hash);

        hash %= bloomfilter->m;
        bit_set(bloomfilter->bit_vector, hash);
    }
    return 0;
}

int bloomfilter_lookup(struct bloomfilter *bloomfilter, uint64_t key_)
{
    void *key = &key_;

    for (uint32_t i = 0; i < bloomfilter->k; i++)
    {
        uint32_t hash;
        murmur3_hash32(key, 8, i, &hash);
        hash %= bloomfilter->m;
        if (!bit_get(bloomfilter->bit_vector, hash))
            return 1;
    }

    return 0;
}

int bloomfilter_bytes(struct bloomfilter *bloomfilter)
{
    return (bloomfilter->m >> 3);
}

void bloomfilter_info(struct bloomfilter *bloomfilter)
{
    cout << bloomfilter->m << " " << bloomfilter->k << endl;
    return;
}