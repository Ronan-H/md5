#ifndef DEFINITIONS
#define DEFINITIONS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

// RFC: a "word" is a 32 bit quantity
typedef uint32_t word;
// 64 bit unsigned integer
typedef unsigned long long ull;
// unsigned byte
typedef unsigned char ubyte;

// integer constants
// 2^32 (constant from RFC to generate T)
#define T_MULTIPLIER 4294967296
// single padding bit
#define FIRST_PADDING_BYTE 128;

// function-like macros
#define MIN(a, b) (a < b ? a : b)

// auxiliary functions
#define F(x, y, z) (x & y | (~x & z))
#define G(x, y, z) (x & z | (y & ~z))
#define H(x, y, z) (x ^ y ^ z)
#define I(x, y, z) (y ^ (x | ~z))

// rotate bits x left s places
// (shift left and add bits that wrapped around)
#define ROTL(x, s) ((x << s) + (x >> (32 - s)))

// MD5 round functions
#define R1_OP(a, b, c, d, xk, s, ti) a = (b + ROTL((a + F(b, c, d) + xk + ti), s))
#define R2_OP(a, b, c, d, xk, s, ti) a = (b + ROTL((a + G(b, c, d) + xk + ti), s))
#define R3_OP(a, b, c, d, xk, s, ti) a = (b + ROTL((a + H(b, c, d) + xk + ti), s))
#define R4_OP(a, b, c, d, xk, s, ti) a = (b + ROTL((a + I(b, c, d) + xk + ti), s))

// function declarations
word * generateT();
struct Blocks * makeBlocks(ubyte *bytes, int length);
struct Blocks * readFileAsBlocks(char *filePath);
void printWordBits(word w);
void printBlocks(struct Blocks *);

typedef struct Blocks {
    word **words;
    int numBlocks;
} Blocks;

#endif