#include "md5.h"
#include <getopt.h>

void runHashInputLoop();

// command line options
// ref: https://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Options.html
// ref: https://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html
// (I also used other similar getopt manuals on gnu.org)
static int helpFlag;
static int testFlag;
static int hashFlag;
static int crackFlag;

static struct option long_options[] =
{
    // all three of these options just set a flag
    {"help", no_argument, &helpFlag, 1},
    {"test", no_argument, &testFlag, 1},
    {"hash", no_argument, &hashFlag, 1},
    {"crack", no_argument, &crackFlag, 1},
    // "terminate the array with an element containing all zeros"
    {0, 0, 0, 0}
};

int main(int argc, char **argv) {
    // parse command line arguments
    // (just settings flags, so there's not much to do here)
    int optionIndex;
    int c = 0;

    // TODO print help if invalid option is used
    while (c != -1) {
        c = getopt_long_only(argc, argv, "", long_options, &optionIndex);
    }

    // command line argument precedence and restrictions
    // restriction: 'hash' and 'crack' can not both be used
    // precedence: 'hash' will be used if both are specified
    if (hashFlag) {
        crackFlag = 0;
    }

    // default if no options are used: run hash input loop
    if (!(helpFlag || testFlag || hashFlag || crackFlag)) {
        hashFlag = 1;
    }

    if (helpFlag) puts("Help flag was set");
    if (testFlag) puts("Test flag was set");
    if (hashFlag) puts("Hash flag was set");
    if (crackFlag) puts("Crack flag was set");

    // run test suite if 'test' flag was given
    if (testFlag) {
        runTestSuite();
    }

    // run hash input loop if 'hash' flag was given
    if (hashFlag) {
        runHashInputLoop();
    }

    return 0;
}

void runHashInputLoop() {
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
}