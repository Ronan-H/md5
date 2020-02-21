#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

// RFC: a "word" is a 32 bit quantity
typedef uint32_t word;

// integer constants
// 2^32 (constant from RFC to generate T)
#define T_MULTIPLIER 4294967296
// 1 followed by 7 zeroes in binary
#define FIRST_PADDING_BYTE 128;

// function-like macros
#define MIN(a, b) (a < b ? a : b)

#define F(x, y, z) (x & y | (~x & z))
#define G(x, y, z) (x & y | (y & ~z)
#define H(x, y, z) (x ^ y ^ z)
#define I(x, y, z) (y ^ (x | ~z))

// rotate bits x left s places
// (shift left and add bits that wrapped around)
#define ROTL(x, s) ((x << s) + (x >> (32 - s)))

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

    struct Blocks *M = readFileAsBlocks("md5.c");
    printBlocks(M);

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
    int bytesRead;
    int byteIndex;
    word currWord;

    FILE *filePtr = fopen(filePath, "rb");

    if (!filePtr) {
        printf("File not found: %s\n", filePath);
        exit(1);
    }

    // reading binary files: https://stackoverflow.com/a/22059317
    // seek to end of the file to find it's length
    fseek(filePtr, 0, SEEK_END);
    // TODO: typedef this, its silly
    unsigned long long bytesRemaining = ftell(filePtr);
    unsigned long long totalBytes = bytesRemaining;
    unsigned long long totalBits = totalBytes * 8;
    rewind(filePtr);

    printf("File is %lld bytes long.\n", bytesRemaining);

    // read the entire file into a char buffer
    char buffer[totalBytes];
    fread(buffer, totalBytes, 1, filePtr);

    // compute the number of blocks needed to store the data, including the padding and input length bytes
    // (at least 1 block regardless of input length, +1 block for every 64 bytes, +1 extra if needed to fit padding and input length bytes)
    int numBlocks = 1 + (totalBytes / 64) + (totalBytes % 64 > 13 ? 1 : 0);
    printf("Num blocks: %d\n", numBlocks);

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

    byteIndex = 3;
    currWord = 0;
    for (int i = 0; i < totalBytes; i++) {
        // combine byte into the current word (little-endian, as per the RFC)
        currWord = currWord | (buffer[i] << (byteIndex-- * 8));

        if (byteIndex == -1 || i == totalBytes - 1) {
            M[i / 64][(i % 64) / 4] = currWord;
            currWord = 0;
            byteIndex = 3;
        }
    }
    
    // -= PADDING =-
    // pad after the last byte
    int paddingIndex = totalBytes / 4;
    int lastBlockIndex = numBlocks - 1;

    M[paddingIndex / 16][paddingIndex % 16] = FIRST_PADDING_BYTE;

    while (++paddingIndex % 16 != 14) {
        M[paddingIndex / 16][paddingIndex % 16] = 0;
    }

    // append input length (most significant byte first?)
    // TODO: this probably isn't right.
    M[lastBlockIndex][14] = totalBits >> 16;
    M[lastBlockIndex][15] = totalBits & 0xffff;

    // close file
    fclose(filePtr);

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

    for (int i = 0; i < numBlocks; i++) {
        printf("BLOCK: %d\n", i++);

        for (int j = 0; j < 16; j++) {
            printWordBits(words[i][j]);
        }

        printf("\n");
    }
}