#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

// RFC: a "word" is a 32 bit quantity
typedef uint32_t word;

// integer constants
#define T_MULTIPLIER 4294967296

// function-like macros
#define MIN(a, b) (a < b ? a : b)

#define F(x, y, z) (x & y | (~x & z))
#define G(x, y, z) (x & y | (y & ~z)
#define H(x, y, z) (x ^ y ^ z)
#define I(x, y, z) (y ^ (x | ~z))

// rotate bits x left s places
// (shift left and add bits that wrapped around)
#define ROTL(x, s) ((x << s) + (x >> (32 - s)))

// structs
struct Block {
    word words[16];
    struct Block* next;
};

// function declarations
word * generateT();
struct Block * readFileAsBlocks(char *filePath);
void printWordBits(word w);
void printBlocks(struct Block *);

int main() {
    // entry point of the program
    word *T = generateT();

    struct Block *M = readFileAsBlocks("md5.c");
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

struct Block * readFileAsBlocks(char *filePath) {
    struct Block* head = (struct Block*)malloc(sizeof(struct Block));
    struct Block* currBlock = head;
    
    // Block sized buffer of bytes
    char buffer[64];
    int bytesRead;
    int wordIndex;
    word *currWords;
    int currWord;
    int byteIndex;
    bool isFirstBlock = true;

    FILE *filePtr = fopen(filePath, "rb");

    if (!filePtr) {
        printf("File not found: %s\n", filePath);
        exit(1);
    }

    // reading binary files: https://stackoverflow.com/questions/22059189/read-a-file-as-byte-array
    // seek to end of the file to find it's length
    fseek(filePtr, 0, SEEK_END);          
    long bytesRemaining = ftell(filePtr);            
    rewind(filePtr);

    printf("File is %ld bytes long.\n", bytesRemaining);

    while (bytesRemaining > 0) {
        // read 64 bytes, or however many are left
        bytesRead = MIN(bytesRemaining, 64);
        fread(buffer, bytesRead, 1, filePtr);
        
        if (isFirstBlock) {
            // first block already allocated
            isFirstBlock = false;
        }
        else {
            // allocate memory for another block and move pointer along
            currBlock->next = (struct Block*)malloc(sizeof(struct Block));
            currBlock = currBlock->next;
        }
        currWords = currBlock->words;

        for (int i = 0; i < bytesRead; i++) {
            // find current word in buffer
            wordIndex = i / 4;
            currWord = currWords[i / 4];
            // combine byte into the current word (little-endian, as per the RFC)
            byteIndex = 3 - (i % 4);
            currWord = currWord | (buffer[i] << (byteIndex * 8));
            currWords[i / 4] = currWord;
        }

        bytesRemaining -= bytesRead;
    }

    // close file
    fclose(filePtr);

    // return head of linkedlist of Blocks
    return head;
}

void printWordBits(word w) {
    for (int i = 0; i < 32; i++) {
        word bit = (w & 0x80000000) != 0;
        printf("%" PRIu32, bit);
        w = w << 1;
    }

    printf("\n");
}

void printBlocks(struct Block *M) {
    struct Block *currBlock = M;
    word *words = currBlock->words;
    int blockCounter = 0;

    while (currBlock != NULL) {
        printf("BLOCK: %d\n", blockCounter++);

        for (int i = 0; i < 16; i++) {
            printWordBits(words[i]);
        }

        currBlock = currBlock->next;
        words = currBlock->words;

        printf("\n");
    }
}