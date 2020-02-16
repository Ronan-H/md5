#include<stdio.h>
#include <inttypes.h>

// RFC: a "word" is a 32 bit quantity
typedef uint32_t word;

// function-like macro
#define F(x, y, z) (x & y | (~x & z))

int main() {
    // entry point of the program
    printf("Hello World!\n");

    word test = 0;
    printf("Test value: %" PRIu32 "\n", test);
    test++;
    printf("Test value: %" PRIu32 "\n", test);

    word x = 5;
    word y = 10;
    word z = 123;

    word fx = F(F(x, y, z), y, z);

    printf("fx: %" PRIu32 "\n", fx);

    return 0;
}