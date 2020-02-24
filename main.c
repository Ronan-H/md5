#include "md5.h"

int main() {
    runTestSuite();

    char inputStr[100];
    char *pos;
    int strLen = 0;

    do {
        printf("Enter a string to hash, or EXIT to exit: ");
        fgets(inputStr, 100, stdin);

        // find new line character so it can be ignored
        for (int i = 0; i < 100; i++) {
            if (inputStr[i] == '\n') {
                strLen = i;
                break;
            }
        }

        char inputStrCopy[100];
        strcpy(inputStrCopy, inputStr);
        Blocks *blocks = makeBlocks(inputStrCopy, strLen);
        char *hash = md5(blocks);
        printf("\nHash value: %s\n\n", hash);
    } while(strcmp(inputStr, "EXIT\n") != 0);

    return 0;
}