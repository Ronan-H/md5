# MD5
MD5 implementation in C (4th year Theory of Algorithms assignment)

## Table of Contents

Fill in shortcuts to each section here.

## Files
**input/**: Test input files. Contains 7 different files of different sizes in an attempt to cover as many edge cases as possible.

**.gitignore**: Gitignore file, ignoring IDE and compilation files.

**README.md**: This README file.

**main.c**: Program entry point, containing *main()*. Runs all test cases from *input/*, and then allows the user to enter any string they like to see it's corresponding hash value, in a loop.

**md5.c**: MD5 implementation in C. Also contains functions to convert a byte array into blocks, display 32bit *word* values as bits, display blocks, etc.

**md5.h**: Header file containing definitions for both *md5.c* and *main.c*.p

## How to Compile and Run

*These installation instructions are for **linux based** operating systems using the **aptitute** (apt) package manager. Instructions for other platforms may differ. See [here](https://docs.microsoft.com/en-us/cpp/build/walkthrough-compile-a-c-program-on-the-command-line?view=vs-2019) for instructions for compiling C programs on a Windows based machine.*

After cloning:

1. Install prerequisites (git and gcc)
```shell
sudo apt install git build-essential
```

2. Clone this repo
```shell
git clone https://github.com/Ronan-H/md5.git
cd md5
```

3. Compile
```shell
gcc md5.c main.c -o md5 -lm
```

4. Run
```shell
./md5
```

5. (Optional) Compile and run can be combined for convenience
```shell
gcc md5.c main.c -o md5 -lm && ./md5
```

## What it Does, and Testing
The purpose of what running the application does is to demonstrate that it can produce the correct md5 hash of any input. As such, running the program will first run the md5 algorithm against all 7 test files in *input/*:

```
TESTING FILE:  ./input/0_bytes.txt
EXPECTED HASH: d41d8cd98f00b204e9800998ecf8427e
ACTUAL HASH:   d41d8cd98f00b204e9800998ecf8427e
MATCHES? -- YES --

TESTING FILE:  ./input/7_bytes.txt
EXPECTED HASH: a30647b9afd8edab046dc999d5005745
ACTUAL HASH:   a30647b9afd8edab046dc999d5005745
MATCHES? -- YES --

TESTING FILE:  ./input/70_bytes.txt
EXPECTED HASH: 2ee1d2ac4ca54921803e3e561ca7160e
ACTUAL HASH:   2ee1d2ac4ca54921803e3e561ca7160e
MATCHES? -- YES --

TESTING FILE:  ./input/119_bytes.txt
EXPECTED HASH: aae4424f064da59e945d090f68086e3d
ACTUAL HASH:   aae4424f064da59e945d090f68086e3d
MATCHES? -- YES --

TESTING FILE:  ./input/120_bytes.txt
EXPECTED HASH: 07863b54ce694ca5e7e8a15c2fbdad59
ACTUAL HASH:   07863b54ce694ca5e7e8a15c2fbdad59
MATCHES? -- YES --

TESTING FILE:  ./input/121_bytes.txt
EXPECTED HASH: 456ba7f594dc57df3ab8015b959b3917
ACTUAL HASH:   456ba7f594dc57df3ab8015b959b3917
MATCHES? -- YES --

TESTING FILE:  ./input/128_bytes.txt
EXPECTED HASH: 2bcf219635c03d2db06f516b2d605fb0
ACTUAL HASH:   2bcf219635c03d2db06f516b2d605fb0
MATCHES? -- YES --
```

As shown, my implementation produces the correct hash for all 7 test files. The *EXPECTED* hash value for each test case was found by running the ```md5sum``` command (included in most "Unix-like" operating systems) for all files in the *input/* directory, like so: ```md5sum input/*```, which produces the following output:

```
d41d8cd98f00b204e9800998ecf8427e  input/0_bytes.txt
aae4424f064da59e945d090f68086e3d  input/119_bytes.txt
07863b54ce694ca5e7e8a15c2fbdad59  input/120_bytes.txt
456ba7f594dc57df3ab8015b959b3917  input/121_bytes.txt
2bcf219635c03d2db06f516b2d605fb0  input/128_bytes.txt
2ee1d2ac4ca54921803e3e561ca7160e  input/70_bytes.txt
a30647b9afd8edab046dc999d5005745  input/7_bytes.txt
```

After that, the user is free to enter any string they want into the console (arbitrarily limited to 100 characters) to produce the md5 hash of that string. Entering *EXIT* exits the application.

```
Enter a string to hash, or EXIT to exit: Hello, World!

Hash value: 65a8e27d8879283831b664bd8b7f0ad4

Enter a string to hash, or EXIT to exit: password123

Hash value: 482c811da5d5b4bc6d497ffa98491e38

Enter a string to hash, or EXIT to exit: EXIT

Hash value: a42b2fb0e720a080e79a92f4ca97d927

ronan@ronan-desktop:~/code/md5$
```

## How it Works

MD5 *(message digest 5)* is a hash function, taking an arbitrary length (max 2^64 bits) input and converting it into a 128 bit output, usually represent as 32 characters of hexadecimal. MD5 was used heavily for storing the hash of passwords to authenticate users without storing their actual password, and for verifying the integrity of files by pairing them with their hash. These use cases relied on three different properties of MD5:

* Collision resistance: computationally infeasible to find two different inputs which produce the same hash
* Preimage resistance: computationally infeasible to find the original message from it's produced hash

Collisions have been successfully found for MD5, and a theoretical preimage attack has also been found. As such, MD5 is now considered insecure, and the SHA-2 set of cryptographic functions should be use instead.

## How I Wrote it

I mostly followed the published [RFC 1321](https://www.ietf.org/rfc/rfc1321.txt) which specifies the MD5 algorithm to write it. I usually used the same variable and type names as the RFC too, to help me follow it. Instead of storing the input bytes M in one big array, I stored them in a 2D array of *word* blocks. This is probably slightly less efficient but in my view it is a more elegant solution.
