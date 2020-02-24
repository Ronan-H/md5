#include "md5.h"

int main() {
    // entry point of the program
    word *T = generateT();

    // gitignore file
    struct Blocks *blocks = readFileAsBlocks("./.gitignore");

    // test files for edge cases
    // empty file
    //struct Blocks *blocks = readFileAsBlocks("./input/0_bytes.txt");
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
    //struct Blocks *blocks = readFileAsBlocks("/home/ronan/Videos/video-project.mp4");

    char *hash = md5(blocks);
    printf(hash);

    return 0;
}