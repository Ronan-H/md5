#include "md5.h"

int main() {
    runTestSuite();

    char buf[200];
    strcpy(buf, "Hello World!");
    Blocks *blocks = makeBlocks(buf, 12);
    char *hash = md5(blocks);
    printf("%s\n", hash);

    return 0;
}