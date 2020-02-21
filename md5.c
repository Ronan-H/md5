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
#define FIRST_PADDING_BYTE 1;

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
    //struct Blocks *blocks = readFileAsBlocks("./.gitignore");

    // test files for edge cases
    // empty file
    struct Blocks *blocks = readFileAsBlocks("./input/0_bytes.txt");
    // all bytes (including padding and input length) should fit in one block
    //struct Blocks *blocks = readFileAsBlocks("./input/7_bytes.txt");
    // all bytes (including padding and input length) should fit in two blocks
    //struct Blocks *blocks = readFileAsBlocks("./input/70_bytes.txt");
    // all bytes (including padding and input length) should just about fit in two blocks
    //struct Blocks *blocks = readFileAsBlocks("./input/119_bytes.txt");
    // padding space requirements forces creation of an extra block
    //struct Blocks *blocks = readFileAsBlocks("./input/120_bytes.txt");
    // same as above
    //struct Blocks *blocks = readFileAsBlocks("./input/121_bytes.txt");
    // input fills exactly 2 blocks, 3rd block is entirely padding and input length
    //struct Blocks *blocks = readFileAsBlocks("./input/128_bytes.txt");

    // large file (~90 MB)
    //struct Blocks *M = readFileAsBlocks("/home/ronan/Videos/video-project.mp4");

    int numBlocks = blocks->numBlocks;
    word **M = blocks->words;

    printBlocks(blocks);

    // initialise MD buffer
    word A = 0x67452301;
    word B = 0xefcdab89;
    word C = 0x98badcfe;
    word D = 0x10325476;

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
        R2_OP(A, B, C, D, X[1], 5, T[17]);
        R2_OP(D, A, B, C, X[6], 9, T[18]);
        R2_OP(C, D, A, B, X[11], 14, T[19]);
        R2_OP(B, C, D, A, X[0], 20, T[20]);

        R2_OP(A, B, C, D, X[5], 5, T[21]);
        R2_OP(D, A, B, C, X[10], 9, T[22]);
        R2_OP(C, D, A, B, X[15], 14, T[23]);
        R2_OP(B, C, D, A, X[4], 20, T[24]);

        R2_OP(A, B, C, D, X[9], 5, T[25]);
        R2_OP(D, A, B, C, X[14], 9, T[26]);
        R2_OP(C, D, A, B, X[3], 14, T[27]);
        R2_OP(B, C, D, A, X[8], 20, T[28]);

        R2_OP(A, B, C, D, X[13], 5, T[29]);
        R2_OP(D, A, B, C, X[2], 9, T[30]);
        R2_OP(C, D, A, B, X[7], 14, T[31]);
        R2_OP(B, C, D, A, X[12], 20, T[32]);

        // Round 3
        R3_OP(A, B, C, D, X[5], 4, T[33]);
        R3_OP(D, A, B, C, X[8], 11, T[34]);
        R3_OP(C, D, A, B, X[11], 16, T[35]);
        R3_OP(B, C, D, A, X[14], 23, T[36]);

        R3_OP(A, B, C, D, X[1], 4, T[37]);
        R3_OP(D, A, B, C, X[4], 11, T[38]);
        R3_OP(C, D, A, B, X[7], 16, T[39]);
        R3_OP(B, C, D, A, X[10], 23, T[40]);

        R3_OP(A, B, C, D, X[13], 4, T[41]);
        R3_OP(D, A, B, C, X[0], 11, T[42]);
        R3_OP(C, D, A, B, X[3], 16, T[43]);
        R3_OP(B, C, D, A, X[6], 23, T[44]);

        R3_OP(A, B, C, D, X[9], 4, T[45]);
        R3_OP(D, A, B, C, X[12], 11, T[46]);
        R3_OP(C, D, A, B, X[15], 16, T[47]);
        R3_OP(B, C, D, A, X[2], 23, T[48]);

        // Round 4
        R4_OP(A, B, C, D, X[0], 6, T[49]);
        R4_OP(D, A, B, C, X[7], 10, T[50]);
        R4_OP(C, D, A, B, X[14], 15, T[51]);
        R4_OP(B, C, D, A, X[5], 21, T[52]);

        R4_OP(A, B, C, D, X[12], 6, T[53]);
        R4_OP(D, A, B, C, X[3], 10, T[54]);
        R4_OP(C, D, A, B, X[10], 15, T[55]);
        R4_OP(B, C, D, A, X[1], 21, T[56]);

        R4_OP(A, B, C, D, X[8], 6, T[57]);
        R4_OP(D, A, B, C, X[15], 10, T[58]);
        R4_OP(C, D, A, B, X[6], 15, T[59]);
        R4_OP(B, C, D, A, X[13], 21, T[60]);

        R4_OP(A, B, C, D, X[4], 6, T[61]);
        R4_OP(D, A, B, C, X[11], 10, T[62]);
        R4_OP(C, D, A, B, X[2], 15, T[63]);
        R4_OP(B, C, D, A, X[9], 21, T[64]);

        A += AA;
        B += BB;
        C += CC;
        D += DD;
    }

    printf("Hash: %x%x%x%x\n\n", A, B, C, D);

    // TODO free allocated memory before exit?

    return 0;
}

// generate the 64 element array T, as per the RFC
word * generateT() {
    // use static so T can be used when out of scope
    static word T[65];

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
    byteIndex = 0;
    currWord = 0;
    for (int i = 0; i < totalBytes; i++) {
        // combine byte into the current word (little-endian, as per the RFC)
        currWord = currWord | (buffer[i] << (byteIndex++ * 8));

        if (byteIndex == 4 || i == totalBytes - 1) {
            // write currWord to the next index of the current block
            M[i / 64][(i % 64) / 4] = currWord;
            currWord = 0;
            byteIndex = 0;
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