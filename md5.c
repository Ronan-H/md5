#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <inttypes.h>

// RFC: a "word" is a 32 bit quantity
typedef uint32_t word;

// integer constants
#define T_MULTIPLIER 4294967296

// function-like macros
#define F(x, y, z) (x & y | (~x & z))
#define G(x, y, z) (x & y | (y & ~z)
#define H(x, y, z) (x ^ y ^ z)
#define I(x, y, z) (y ^ (x | ~z))

// rotate bits x left s places
// (shift left and add bits that wrapped around)
#define ROTL(x, s) ((x << s) + (x >> (32 - s)))

// function declarations (not sure if I actually need these?)
void printWordBits(word w);
word * generateT();

int main() {
    // entry point of the program

    word test = 0;
    printf("Test value: %" PRIu32 "\n", test);
    test++;
    printf("Test value: %" PRIu32 "\n", test);

    word x = 5231;
    word y = 104321;
    word z = 125433;

    word fx = F(F(x, y, z), y, z);

    printf("fx: %" PRIu32 "\n", fx);

    printf("%-17s", "fx bits: ");
    printWordBits(fx);

    printf("%-17s", "ROTL fx by 17: ");
    fx = ROTL(fx, 17);
    printWordBits(fx);

    word *T = generateT();

    return 0;
}

void printWordBits(word w) {
    for (int i = 0; i < 32; i++) {
        word bit = (w & 0x80000000) != 0;
        printf("%" PRIu32, bit);
        w = w << 1;
    }

    printf("\n");
}

// generate the 64 element array T, as per the RFC
word * generateT() {
    // use static so T can be used when out of scope
    static word T[64];

    for (int i = 1; i <= 64; i++) {
        T[i] = (word)(T_MULTIPLIER * fabs(sin(i)));
        printf("T[i]: %" PRIu32 "\n", T[i]);
    }

    return T;
}