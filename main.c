#include "md5.h"

int main() {
    // entry point of the program
    word *T = generateT();

    // test files for edge cases
    // empty file
    testFileHash("./input/0_bytes.txt", "d41d8cd98f00b204e9800998ecf8427e");
    // all bytes (including padding and input length) should fit in one block
    testFileHash("./input/7_bytes.txt", "a30647b9afd8edab046dc999d5005745");
    // all bytes (including padding and input length) should fit in two blocks
    testFileHash("./input/70_bytes.txt", "2ee1d2ac4ca54921803e3e561ca7160e");
    // all bytes (including padding and input length) should just about fit in two blocks
    testFileHash("./input/119_bytes.txt", "aae4424f064da59e945d090f68086e3d");
    // padding space requirements forces creation of an extra block
    testFileHash("./input/120_bytes.txt", "07863b54ce694ca5e7e8a15c2fbdad59");
    // same as above
    testFileHash("./input/121_bytes.txt", "456ba7f594dc57df3ab8015b959b3917");
    // input fills exactly 2 blocks, 3rd block is entirely padding and input length
    testFileHash("./input/128_bytes.txt", "2bcf219635c03d2db06f516b2d605fb0");

    return 0;
}