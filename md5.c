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
void printWordBits(word w);
word * generateT();
struct Block * readFileAsBlocks(char *filePath);

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

    struct Block *M = readFileAsBlocks("md5.c");

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
    int isFirstBlock = 0;

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
            isFirstBlock = 1;
        }
        else {
            // allocate memory for another block and move pointer along
            currBlock->next = (struct Block*)malloc(sizeof(struct Block));
            currBlock = currBlock->next;
            currWords = currBlock->words;
        }

        for (int i = 0; i < bytesRead; i++) {
            // find current word in buffer
            wordIndex = i / 4;
            currWord = currWords[i / 4];
            // combine byte into the current word (little-endian, as per the RFC)
            byteIndex = 3 - (i % 4);
            currWord = currWord | (buffer[i] << (byteIndex * 8));
        }

        bytesRemaining -= bytesRead;
    }

    // close file
    fclose(filePtr);

    // return head of linkedlist of Blocks
    return head;
}