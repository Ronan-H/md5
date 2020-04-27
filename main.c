#include "md5.h"
#include <getopt.h>
#include <sys/time.h>

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
        runCrackUtility(5);
    }

    puts("Exiting...\n");

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

    puts("Note: only one of --hash or --crack will be used if both are specified, in that order");
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
        // finished generating string; hash string and compare
        Blocks *blocks = makeBlocks(buffer, length);
        char *hash = md5(blocks);

        // check for a match
        bool match = isHashEqual(hash, refHash);
        if (match) {
            // append null terminator to result
            buffer[length] = '\0';
        }
        // propagate match result
        return match;
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

void runCrackUtility(int maxLength) {
    char refHash[33];
    char buffer[maxLength + 1];
    struct timeval stop, start;
    long timeTaken;

    // get reference hash from user
    puts("Expected hash input format: 32 lowercase hex characters, E.g.: 5d41402abc4b2a76b9719d911017c592");
    puts("Expected plaintext alpahbet: [a-z]*");
    printf("Trying up to plaintext length: %d\n\n", maxLength);
    printf("Enter a reference MD5 hash to crack: ");
    fgets(refHash, 33, stdin);
    puts("\nCracking...\n");

    // try all permutations for all lengths of string, up to maxLength
    for (int len = 0; len <= maxLength; len++) {
        printf("Trying all permutations of length %d...\n", len);
        // checking system time in C: https://stackoverflow.com/a/10192994
        gettimeofday(&start, NULL);
        bool matchFound = bruteForcePermutations(len, len - 1, buffer, refHash);
        gettimeofday(&stop, NULL);

        if (matchFound) {
            printf("\nMatch found!\n  Result: '%s'\n\n", buffer);
            return;
        }
        else {
            // print time taken to exhaust all permutations of length 'len'
            timeTaken = (stop.tv_sec - start.tv_sec) * 1000000 + (stop.tv_usec - start.tv_usec);
            printf("  No match, took %lu microseconds\n\n", timeTaken); 
        }
    }

    // no match found for any string of length 0 to maxLength
    puts("No match found!");
}