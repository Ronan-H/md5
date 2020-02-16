#include<stdio.h>

// RFC: a "word" is a 32 bit quantity
typedef uint32_t word;

// function-like macro
#define F(x, y, z) (x & y | (~x & z))

int main() {
    // entry point of the program
    printf("Hello World!\n");

    

    return 0;
}