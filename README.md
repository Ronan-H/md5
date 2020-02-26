# MD5

## Table of Contents
 * [Files](#files)
 * [How to Compile and Run](#how-to-compile-and-run)
 * [What it Does, and Testing](#what-it-does-and-testing)
 * [How it Works](#how-it-works)
 * [How I Wrote it](#how-i-wrote-it)
 * [Simplification: Using Bytes](#simplification-using-bytes)

## Files
**input/**: Test input files. Contains 7 different files of different sizes in an attempt to cover as many edge cases as possible.

**.gitignore**: Gitignore file, ignoring IDE and compilation files.

**README.md**: This README file.

**main.c**: Program entry point, containing *main()*. Runs all test cases from *input/*, and then allows the user to enter any string they like to see it's corresponding hash value, in a loop.

**md5.c**: MD5 implementation in C. Also contains functions to convert a byte array into blocks, display 32bit *word* values as bits, display blocks, etc.

**md5.h**: Header file containing definitions for both *md5.c* and *main.c*.

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
The purpose of the application is to demonstrate that it can produce the correct MD5 hash of any input. As such, running the program will first run the MD5 algorithm against all 7 test files in *input/*:

```
TESTING FILE:  ./test_input/0_bytes.txt
EXPECTED HASH: d41d8cd98f00b204e9800998ecf8427e
ACTUAL HASH:   d41d8cd98f00b204e9800998ecf8427e
MATCHES? -- YES --

TESTING FILE:  ./test_input/7_bytes.txt
EXPECTED HASH: a30647b9afd8edab046dc999d5005745
ACTUAL HASH:   a30647b9afd8edab046dc999d5005745
MATCHES? -- YES --

TESTING FILE:  ./test_input/70_bytes.txt
EXPECTED HASH: 2ee1d2ac4ca54921803e3e561ca7160e
ACTUAL HASH:   2ee1d2ac4ca54921803e3e561ca7160e
MATCHES? -- YES --

TESTING FILE:  ./test_input/119_bytes.txt
EXPECTED HASH: aae4424f064da59e945d090f68086e3d
ACTUAL HASH:   aae4424f064da59e945d090f68086e3d
MATCHES? -- YES --

TESTING FILE:  ./test_input/120_bytes.txt
EXPECTED HASH: 07863b54ce694ca5e7e8a15c2fbdad59
ACTUAL HASH:   07863b54ce694ca5e7e8a15c2fbdad59
MATCHES? -- YES --

TESTING FILE:  ./test_input/121_bytes.txt
EXPECTED HASH: 456ba7f594dc57df3ab8015b959b3917
ACTUAL HASH:   456ba7f594dc57df3ab8015b959b3917
MATCHES? -- YES --

TESTING FILE:  ./test_input/128_bytes.txt
EXPECTED HASH: 2bcf219635c03d2db06f516b2d605fb0
ACTUAL HASH:   2bcf219635c03d2db06f516b2d605fb0
MATCHES? -- YES --

TESTING FILE:  ./test_input/md5.png
EXPECTED HASH: aee5aa4f28909f16b4924fa5903efcdd
ACTUAL HASH:   aee5aa4f28909f16b4924fa5903efcdd
MATCHES? -- YES --

TESTING FILE:  ./test_input/smb_coin.wav
EXPECTED HASH: a36764134107d0fe6c80bc7fa696fb16
ACTUAL HASH:   a36764134107d0fe6c80bc7fa696fb16
MATCHES? -- YES --
```

As shown, my implementation produces the correct hash for all 7 test files. The *EXPECTED* hash value for each test case was found by running the ```md5sum``` command (included in most "Unix-like" operating systems) for all files in the *input/* directory, like so: ```md5sum test_input/*```, which produces the following output:

```
d41d8cd98f00b204e9800998ecf8427e  test_input/0_bytes.txt
aae4424f064da59e945d090f68086e3d  test_input/119_bytes.txt
07863b54ce694ca5e7e8a15c2fbdad59  test_input/120_bytes.txt
456ba7f594dc57df3ab8015b959b3917  test_input/121_bytes.txt
2bcf219635c03d2db06f516b2d605fb0  test_input/128_bytes.txt
2ee1d2ac4ca54921803e3e561ca7160e  test_input/70_bytes.txt
a30647b9afd8edab046dc999d5005745  test_input/7_bytes.txt
aee5aa4f28909f16b4924fa5903efcdd  test_input/md5.png
a36764134107d0fe6c80bc7fa696fb16  test_input/smb_coin.wav
```

After that, the user is free to enter any string they want into the console (arbitrarily limited to 100 characters) to produce the md5 hash of that string. Entering *EXIT* exits the application.

```shell
Enter a string to hash, or EXIT to exit: Hello, World!
                         MD5 Hash value: 65a8e27d8879283831b664bd8b7f0ad4

Enter a string to hash, or EXIT to exit: password123
                         MD5 Hash value: 482c811da5d5b4bc6d497ffa98491e38

Enter a string to hash, or EXIT to exit: EXIT
                         MD5 Hash value: a42b2fb0e720a080e79a92f4ca97d927

ronan@ronan-desktop:~/code/md5$
```

## How it Works

MD5 *(message digest 5)* is a hash function, taking an arbitrary length (max 2<sup>64</sup> bits) input and converting it into a 128 bit output, usually represented as 32 characters of hexadecimal. MD5 was used heavily for storing the hash of passwords, to authenticate users without storing their actual password, and for verifying the integrity of files by pairing them with their hash. These use cases mainly relied on two different properties of MD5:

* Collision resistance: it is computationally infeasible to find two different inputs which produce the same hash
* Preimage resistance: it is computationally infeasible to find the original message from it's produced hash

MD5 collisions have been found and used as an attack vector, and a theoretical preimage attack has also been found. As such, MD5 is now considered insecure, and the SHA-2 set of cryptographic functions should be used instead.

## How I Wrote it

### Getting Started
I mostly followed the published [RFC 1321](https://www.ietf.org/rfc/rfc1321.txt) which specifies the MD5 algorithm. I usually used the same variable and type names as the RFC too, to help me follow it. Instead of storing the input bytes *M* in one big array, I stored them in a 2D array of *word* blocks. This is probably slightly less efficient but in my view it is a more elegant solution. I began by defining some easy *funtion-like* macros, such as F(), G(), H() and I(). These were pretty easy to implement, as it was just a matter of translating the mathematical functions given in the RFC into C code.

### Running into Difficulties
After that, though, it got a lot more difficult. I decided I would use a file as input to the hashing functon. I had to look up how to read an entire file into memory in C, stored in a byte array. Originally, I decided to store blocks in a linked list. This worked, however writing the padding code proved to be difficult to do elegantly and efficiently. Another mistake I made was not calculating how many blocks were needed ahead of time. This makes inserting the padding a lot harder, since you have to dymanically add blocks as you read the file buffer, and figuring out the padding was also hard.

### Making Things Easier
Eventually, I decided that a 2 dimensional array of *word*s would be the best way of representing a series of blocks. This makes it easier to iterate over the values in each block. Also, I decided that the padding bits should be added to the input message *before* converting the input into blocks, not after.

Using these ideas makes the file reading, padding, and block building steps fairly straight forward.

### Constructing Blocks, Step by Step
Here is how it works all together:
1. Compute the exact number of padding bytes needed to append to the message.
   * For this I came up with the formula ```paddingBytes = 65 - ((length + 8) % 64 + 1);```
2. Compute the total number of bytes needed for the byte ```buffer```: ```totalBytes = length + paddingBytes + 8;```
   * 8 bytes are needed at the end to represent the input length; a 64 bit unsigned integer.
   * ```totalBytes``` is now guaranteed to be evenly divisible by 64, ie. full blocks can be constructed with no bytes left over.
3. Create a byte array ```buffer``` with length ```totalBytes```. Read the entire file into this array. There will be space left over for padding and input length bytes.
4. Append the first bit of padding, a 1. This is easy now, we just write the integer 128 to the above array. 128 represents 1 followed by 7 zeroes in binary.
5. Write all the remaining 0's of padding. Again, this is pretty easy. We're just filling the rest of the array with 0's, **up untill we reach the 8 bytes of input length at the end**. We could write 0's here but we're just about to write the input length there anyway.
6. Use bitwise operations to represent the input length in the last 8 bytes.
7. Create a 2D *word* array, and read each group of 4 bytes from the ```buffer``` array into each *word* value. Again, this is pretty straight forward, because we have already guaranteed that the array can be divided into blocks evenly. It's important to remember here that **bits** are grouped in **high-order**, and bytes are grouped in **low-order**, as the RFC specifies. This was one of the most confusing aspects of the assignment to get right.

From there, the blocks are fed straight into the md5 algorithm. There's not really much to say about this, it's pretty much exactly what the RFC says to do in pseudocode, I just had to translate it into C code.

### Fixing Bugs and Finishing Up
Once I was able to get a hash value out, the last step to the algorithm was fixing all the bugs. These were mostly typos, which isn't surprising, since there were so many values to copy from the RFC. Because of how hashing works, the hash value is *completely different* to the expected value if *anything* is wrong, even if a single bit is out of place. This is because of the [avalanche effect](https://en.wikipedia.org/wiki/Avalanche_effect). I had also gotten the *endianness* wrong when constructing a *word* value from 4 bytes. Eventually though, the hash came out right, and from there it was just a case of cleaning up and refactoring the code, allowing the user to enter their own input to be hashed, etc.

## Simplification: Using Bytes
For my implementation of the algorithm, I made a simplification in one area: the input to the algorithm can only be given in **bytes**. The RFC describes an algorithm which can process an exact number of **bits**. However, in practice, it would be rare for someone to want to get the hash value of a specific pattern of bits. In the vast majority of cases (passwords or files, for example), **bytes** are the lowest format of data that is used. Because of this, I think it's acceptable to make this simplification.
