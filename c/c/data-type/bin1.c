#include <stdio.h>
#include <stdlib.h>

#define TEST_CLEAN_BITS(a, b, mask)  ( (a & mask) & ((a & mask) ^ (b & mask)))
#define TEST_SET_BITS(a, b, mask)    (~(a & mask) & ((a & mask) ^ (b & mask)))


unsigned int mask(unsigned int ofst, unsigned int bits) {
    unsigned int i, max, mask = 0;
    max = (32 < (ofst + bits)) ? 32 : (ofst + bits);
    for (i = 0; i < max - 1; i++) {
        mask |= (ofst < max - i) ? 0x1 : 0x0;
        mask = mask << 1;
    }
    if (!ofst) mask |= 0x1;
    return mask;
}

unsigned int mask2bits (unsigned int mask) {
    unsigned int ofst = 0, bits = 0;

    while (mask) {
        (bits || (mask & 0x1)) ? bits++ : ofst++;
        mask = mask >> 1;
    }

    return bits;
}


unsigned int mask2ofst (unsigned int mask) {
    unsigned int ofst = 0, bits = 0;

    while (mask) {
        (bits || (mask & 0x1)) ? bits++ : ofst++;
        mask = mask >> 1;
    }

    return ofst;
}

int main (int argc, char *argv[]) {
    int a, b;
    unsigned int n;

    if (!argv[1])
        return 0;
    n = (unsigned int) atoi(argv[1]);
    a = mask2ofst(n);
    b = mask2bits(n);

    printf("mask %08X, ofst: %d, bits: %d, mask: %08X\n", n, a, b, mask(a, b));
    printf("set %08X, clear: %08X\n",TEST_SET_BITS(n, mask(a, b), mask(a, b)),TEST_CLEAN_BITS(mask(a, b), n, mask(a, b)));

    return 0;
}