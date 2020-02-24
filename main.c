#include "md5.h"

int main() {
    runTestSuite();

    char inputStr[100];
    char *pos;
    int strLen = 0;

    do {
        // get string input from user
        printf("Enter a string to hash, or EXIT to exit: ");
        fgets(inputStr, 100, stdin);

        // find new line character so it can be ignored
        for (int i = 0; i < 100; i++) {
            if (inputStr[i] == '\n') {
                strLen = i;
                break;
            }
        }

        // copy input since makeBlocks() changes it
        char inputStrCopy[100];
        strcpy(inputStrCopy, inputStr);
        Blocks *blocks = makeBlocks(inputStrCopy, strLen);
        // generate hash and display to the user
        // (even if it was EXIT, maybe they wanted to know what the hash of EXIT is before exiting)
        char *hash = md5(blocks);
        printf("\nHash value: %s\n\n", hash);
    } while(strcmp(inputStr, "EXIT\n") != 0);

    return 0;
}