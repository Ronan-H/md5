#include "md5.h"

int main() {
    runTestSuite();

    char inputStr[1002];
    char inputStrCopy[1100];
    char *pos;
    char *hash;
    int strLen = 0;
    Blocks *blocks;

    do {
        // get string input from user
        printf("Enter a string to hash, or EXIT to exit: ");
        fgets(inputStr, 1002, stdin);

        // find new line character so it can be ignored
        for (int i = 0; i < 1002; i++) {
            if (inputStr[i] == '\n') {
                strLen = i;
                break;
            }
            else if (i == 1001) {
                printf("                         Input too long! Limit is 1000 characters.\n\n");
            }
        }

        // copy input since makeBlocks() changes it
        strcpy(inputStrCopy, inputStr);
        blocks = makeBlocks(inputStrCopy, strLen);
        // generate hash and display to the user
        // (even if it was EXIT, maybe they wanted to know what the hash of EXIT is before exiting)
        hash = md5(blocks);
        printf("                         MD5 Hash value: %s\n\n", hash);
    } while(strcmp(inputStr, "EXIT\n") != 0);

    // free allocated memory
    // (blocks etc. have already been freed in md5())
    free(hash);

    return 0;
}