#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define LARGE_PRIME 10000019
#define ITERATIONS 100000

uint32_t secure_hash(int T, int ID) {
    uint64_t h = ((uint64_t)T ^ 0xDEADBEEF) * ((uint64_t)ID | 0xCAFEBABE);
    h ^= h >> 23;
    h *= 0x2127599BF4325C37ULL;
    h ^= h >> 47;
    return (uint32_t)(h % LARGE_PRIME);
}

int main() {
    int T = 123456789;
    int ID = 42;
    uint32_t result;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < ITERATIONS; i++) {
        result = secure_hash(T, ID);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double duration = (end.tv_sec - start.tv_sec) * 1000.0 +
                      (end.tv_nsec - start.tv_nsec) / 1e6;

    printf("Average time per secure_hash: %.5f ms\n", duration / ITERATIONS);
    return 0;
}
