#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

uint64_t splitmix64(uint64_t *seed) {
	uint64_t r;
	r = (*seed += 0x9e3779b97f4a7c15);
	r = (r ^ (r >> 30)) * 0xbf58476d1ce4e5b9;
	r = (r ^ (r >> 27)) * 0x94d049bb133111eb;
	return (r ^ (r >> 31));
}

uint64_t xorshift128plus(uint64_t *s) {
    uint64_t s0, s1, r;

	s1 = s[0];
	s0 = s[1];
	r = s0 + s1;
	s[0] = s0;
	s1 ^= s1 << 23;
	s[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5);

    return r;
}

int main() {
    int i, j;
    uint64_t seed;
    uint64_t s[2];

    srand((unsigned int) time(NULL));

    // 64bit
    printf("%s\n", "#64");
    for (i = 0; i < 1000; i++) {
        seed = rand();
        printf("%llu\t", seed);
        s[0] = splitmix64(&seed);
        s[1] = splitmix64(&seed);
        for (j = 0; j < 1000; j++) {
            printf("%llu\t", xorshift128plus(s) >> 1);
        }
        printf("\n");
    }
    // 32bit
    printf("%s\n", "#32");
    for (i = 0; i < 1000; i++) {
        seed = rand();
        printf("%llu\t", seed);
        s[0] = splitmix64(&seed);
        s[1] = splitmix64(&seed);
        for (j = 0; j < 1000; j++) {
            printf("%u\t", (uint32_t) (xorshift128plus(s) << 1) >> 1);
        }
        printf("\n");
    }

    return 0;
}
