#include "md5.h"
#include <getopt.h>

void displayHelp();
void runHashInputLoop();
void runCrackUtility();
bool bruteForcePermutations(int length, int index, char *buffer, char *refHash);

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
    // restriction: only one of 'hash' and 'crack' can be used
    // precedence: hash, crack
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

    // displays a help message if the 'help' option was given
    if (helpFlag) {
        displayHelp();
    }

    // run test suite if the 'test' flag option given
    if (testFlag) {
        runTestSuite();
    }

    // run hash input loop if the 'hash' option was given
    if (hashFlag) {
        runHashInputLoop();
    }

    // run crack utility if the 'crack' option was given
    if (crackFlag) {
        runCrackUtility();
    }

    return 0;
}

void displayHelp() {
    // usage based on the NetBSD source code style guide
    puts("\nUsage: ./md5 [--test] [--help] [--hash | --crack]");

    puts("  Default argument if no option is specified: --hash\n");
    puts("Options:");
    puts("  --help:  displays this help message");
    puts("  --test:  runs the suite of tests against the MD5 algorithm");
    puts("  --hash:  hashes user input in a loop (DEFAULT OPTION)");
    puts("  --crack: runs the MD5 cracking utility\n");
    puts("Note: only one of --help, --hash, or --crack will be used, in that order");
    puts("      (--test and --help can be used alongside any other option, or on their own)\n");
}

void runHashInputLoop() {
    char inputStr[1002];
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

        blocks = makeBlocks(inputStr, strLen);
        // generate hash and display to the user
        // (even if it was EXIT, maybe they wanted to know what the hash of EXIT is before exiting)
        hash = md5(blocks);
        printf("                         MD5 Hash value: %s\n\n", hash);
    } while(strcmp(inputStr, "EXIT\n") != 0);

    // free allocated memory
    // (blocks etc. have already been freed in md5())
    free(hash);
}

bool bruteForcePermutations(int length, int index, char *buffer, char *refHash) {
    if (index < 0) {
        // hash string and compare
        Blocks *blocks = makeBlocks(buffer, length);
        char *hash = md5(blocks);

        // propagate match result
        // (if there's a match, the result is in the buffer)
        return isHashEqual(hash, refHash);
    }

    // try all permutations of character at the next index
    for (char c = 'a'; c <= 'z'; c++) {
        buffer[index] = c;
        bool match = bruteForcePermutations(length, index - 1, buffer, refHash);

        if (match) {
            // propagate match found
            return true;
        }
    }

    // tried all permutations at this index without a match
    return false;
}

void runCrackUtility() {
    char buffer[15];
}