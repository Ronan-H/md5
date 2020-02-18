#include<stdio.h>
#include<inttypes.h>

// RFC: a "word" is a 32 bit quantity
typedef uint32_t word;

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

int main() {
    // entry point of the program
    printf("Hello World!\n");

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