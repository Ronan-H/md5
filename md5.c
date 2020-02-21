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
// 1 followed by 7 zeroes in binary
#define FIRST_PADDING_BYTE 128;

// function-like macros
#define MIN(a, b) (a < b ? a : b)

// auxiliary functions
#define F(x, y, z) (x & y | (~x & z))
#define G(x, y, z) (x & y | (y & ~z)
#define H(x, y, z) (x ^ y ^ z)
#define I(x, y, z) (y ^ (x | ~z))

// rotate bits x left s places
// (shift left and add bits that wrapped around)
#define ROTL(x, s) ((x << s) + (x >> (32 - s)))

// MD5 round functions
#define R1_OP(a, b, c, d, xk, ti, s) a = (b + ROTL((a + F(b, c, d) + xk + ti), s))
#define R2_OP(a, b, c, d, xk, ti, s) a = (b + ROTL((a + G(b, c, d) + xk + ti), s))
#define R3_OP(a, b, c, d, xk, ti, s) a = (b + ROTL((a + H(b, c, d) + xk + ti), s))
#define R4_OP(a, b, c, d, xk, ti, s) a = (b + ROTL((a + I(b, c, d) + xk + ti), s))

// function declarations
word * generateT();
struct Blocks * readFileAsBlocks(char *filePath);
void printWordBits(word w);
void printBlocks(struct Blocks *);

typedef struct Blocks {
    word **words;
    int numBlocks;
} Blocks;

int main() {
    // entry point of the program
    word *T = generateT();

    // gitignore file
    struct Blocks *blocks = readFileAsBlocks("./.gitignore");
    int numBlocks = blocks->numBlocks;
    word **M = blocks->words;

    // test files for edge cases
    // empty file
    //struct Blocks *M = readFileAsBlocks("./input/0_bytes.txt");
    // all bytes (including padding and input length) should fit in one block
    //struct Blocks *M = readFileAsBlocks("./input/7_bytes.txt");
    // all bytes (including padding and input length) should fit in two blocks
    //struct Blocks *M = readFileAsBlocks("./input/70_bytes.txt");
    // all bytes (including padding and input length) should just about fit in two blocks
    //struct Blocks *M = readFileAsBlocks("./input/119_bytes.txt");
    // padding space requirements forces creation of an extra block
    //struct Blocks *M = readFileAsBlocks("./input/120_bytes.txt");
    // same as above
    //struct Blocks *M = readFileAsBlocks("./input/121_bytes.txt");
    // input fills exactly 2 blocks, 3rd block is entirely padding and input length
    //struct Blocks *M = readFileAsBlocks("./input/128_bytes.txt");

    // large file (~90 MB)
    //struct Blocks *M = readFileAsBlocks("/home/ronan/Videos/video-project.mp4");

    printBlocks(blocks);

    // initialise MD buffer
    word A = 0x01234567;
    word B = 0x89abcdef;
    word C = 0xfedcba98;
    word D = 0x76543218;

    word AA, BB, CC, DD;

    for (int i = 0; i < numBlocks; i++) {
        word *X = M[i];

        AA = A;
        BB = B;
        CC = C;
        DD = D;

        // Round 1
        R1_OP(A, B, C, D, X[0], 7, T[1]);
        R1_OP(D, A, B, C, X[1], 12, T[2]);
        R1_OP(C, D, A, B, X[2], 17, T[3]);
        R1_OP(B, C, D, A, X[3], 22, T[4]);

        R1_OP(A, B, C, D, X[4], 7, T[5]);
        R1_OP(D, A, B, C, X[5], 12, T[6]);
        R1_OP(C, D, A, B, X[6], 17, T[7]);
        R1_OP(B, C, D, A, X[7], 22, T[8]);

        R1_OP(A, B, C, D, X[8], 7, T[9]);
        R1_OP(D, A, B, C, X[9], 12, T[10]);
        R1_OP(C, D, A, B, X[10], 17, T[11]);
        R1_OP(B, C, D, A, X[11], 22, T[12]);

        R1_OP(A, B, C, D, X[12], 7, T[13]);
        R1_OP(D, A, B, C, X[13], 12, T[14]);
        R1_OP(C, D, A, B, X[14], 17, T[15]);
        R1_OP(B, C, D, A, X[15], 22, T[16]);

        // Round 2

        // Round 3

        // Round 4

        AA += A;
        BB += B;
        CC += C;
        DD += D;
    }

    // TODO free allocated memory before exit?

    return 0;
}

// generate the 64 element array T, as per the RFC
word * generateT() {
    // use static so T can be used when out of scope
    static word T[64];

    for (int i = 1; i <= 64; i++) {
        T[i] = (word)(T_MULTIPLIER * fabs(sin(i)));
    }

    return T;
}

struct Blocks * readFileAsBlocks(char *filePath) {
    int byteIndex;
    int shiftPlaces;
    word currWord;
    ull fileBytes;
    ull paddingBytes;
    ull totalBytes;
    ull totalBits;
    FILE *filePtr;

    filePtr = fopen(filePath, "rb");

    if (!filePtr) {
        printf("File not found: %s\n", filePath);
        exit(1);
    }

    // reading binary files: https://stackoverflow.com/a/22059317
    // seek to end of the file to find it's length
    fseek(filePtr, 0, SEEK_END);
    fileBytes = ftell(filePtr);
    // compute exact number of padding bytes needed
    paddingBytes = 65 - ((fileBytes + 8) % 64 + 1);
    // total number of bytes needed for all blocks (a multiple of 64)
    totalBytes = fileBytes + paddingBytes + 8;
    totalBits = fileBytes * 8;
    rewind(filePtr);

    printf("File is %lld bytes long.\n", fileBytes);

    // read the entire file into a byte buffer
    ubyte *buffer = (ubyte *)malloc(totalBytes * sizeof(ubyte));
    fread(buffer, fileBytes, 1, filePtr);

    // start padding with a 1
    byteIndex = fileBytes;
    buffer[byteIndex++] = FIRST_PADDING_BYTE;

    // rest of the padding is 0s
    for (; byteIndex < totalBytes - 8; byteIndex++) {
        buffer[byteIndex] = 0;
    }

    // append 8 bytes describing input length (most significant byte first)
    shiftPlaces = 56;

    for (; byteIndex < totalBytes; byteIndex++) {
        buffer[byteIndex] = (totalBits >> shiftPlaces) & 0xffff;
        shiftPlaces -= 8;
    }

    // compute the number of blocks needed to store all of the data, including the padding and input length bytes
    int numBlocks = totalBytes / 64;

    // allocate the memory needed for the 2D array M
    // https://stackoverflow.com/a/14088911
    word **M = (word **)malloc(numBlocks * sizeof(word *));

    for (int i = 0; i < numBlocks; i++) {
        M[i] = (int *)malloc(16 * sizeof(word));
    }

    // initialise blocks struct containing the 2D words array and number of blocks
    Blocks *blocks = (Blocks *)malloc(sizeof(Blocks));
    blocks->words = M;
    blocks->numBlocks = numBlocks;

    // now convert the buffer array of bytes into a series of blocks,
    // each containing 16x 32 bit words (512 bits total)
    byteIndex = 3;
    currWord = 0;
    for (int i = 0; i < totalBytes; i++) {
        // combine byte into the current word (little-endian, as per the RFC)
        currWord = currWord | (buffer[i] << (byteIndex-- * 8));

        if (byteIndex == -1 || i == totalBytes - 1) {
            // write currWord to the next index of the current block
            M[i / 64][(i % 64) / 4] = currWord;
            currWord = 0;
            byteIndex = 3;
        }
    }

    // close file
    fclose(filePtr);

    // free buffer memory
    free(buffer);

    // return pointer to blocks
    return blocks;
}

void printWordBits(word w) {
    for (int i = 0; i < 32; i++) {
        word bit = (w & 0x80000000) != 0;
        printf("%" PRIu32, bit);
        w = w << 1;
    }

    printf("\n");
}

void printBlocks(struct Blocks *M) {
    word **words = M->words;
    int numBlocks = M->numBlocks;

    printf("\n");
    for (int i = 0; i < numBlocks; i++) {
        printf("BLOCK: %d\n", i);

        for (int j = 0; j < 16; j++) {
            printWordBits(words[i][j]);
        }

        printf("\n");
    }
    printf("\n");
}