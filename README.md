# MD5

## Notice about the Project's Rescope
Since the completion of the COVID-19 rescoped version to this project, some content from this document has been changed or moved to [overview.md](overview.md) to satisfy the additional requirements and changes made to the code. To view the state of this project when it was originally completed before the rescope, you can view the project from [this commit](https://github.com/Ronan-H/md5/tree/529d4e4dd5e3a538c371f1900ecb9e99d03659eb).

---

## Table of Contents:
 * [Repo Contents](#repo-contents)
 * [How to Compile and Run](#how-to-compile-and-run)
 * [How it Works](#how-it-works)
 * [How I Wrote it](#how-i-wrote-it)
 * [Simplification: Using Bytes](#simplification-using-bytes)

## Repo Contents
**resources/**: Resources for use in this document. This directory can be ignored.

**test_input/**: Test input files. Contains 9 different files of different sizes and types in an attempt to cover as many edge cases as possible.

**.gitignore**: Gitignore file, ignoring IDE and compilation files.

**README.md *(this file)***: The README file, originally written for the 30% version of this project.

**main.c**: Program entry point, containing *main()*. Processes command line options, and implements the hashing, cracking, and testing suites. Makes use of *md5.c* for producing hash values.

**md5.c**: MD5 implementation in C. Also contains functions to convert a byte array into blocks, display 32-bit *word* values as bits, display blocks, etc.

**md5.h**: Header file containing definitions for both *md5.c* and *main.c*.

**overview.md**: Documentation added for the completion of the 70% rescoped portion of the project.

## How to Compile and Run

*These installation instructions are for **Linux based** operating systems using the **aptitude** (apt) package manager. Instructions for other platforms may differ. See [here](https://docs.microsoft.com/en-us/cpp/build/walkthrough-compile-a-c-program-on-the-command-line?view=vs-2019) for instructions for compiling C programs on a Windows based machine.*

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

## How it Works

MD5 *(message digest 5)* is a hash function, taking an arbitrary number of bits as input and converting it into 128 bits of output, represented as 32 characters of hexadecimal. MD5 was used heavily for storing the hash of passwords, to authenticate users without storing their actual password, and for verifying the integrity of files by pairing them with their hash.

Additional information about how MD5 works, and about the suite of tests that I created to help me develop my implementation, can be found in [overview.md](overview.md).

## How I Wrote it

### Research
I was mostly able to just follow the published [RFC 1321](https://www.ietf.org/rfc/rfc1321.txt) to complete the assignment, which fully specifies the MD5 algorithm. I usually used the same variable and type names as the RFC too, to make it easier for me to follow along. I found I didn't really need to do too much research to complete the assignment, as the C concepts I used were taught to us across two modules (*Procedural Programming* and *Advanced Procedural Programming*) in second year. There were a few things I didn't know how to do in C, or had forgotten how to do, such as allocating memory with ```malloc()```, and using ```sprintf()``` to store a formatted string in a *char* array. Any time I needed help like this, I included a comment in my code to the web page I used to help me along (usually a Stack Overflow answer). This was the first time I had to consider *endianness* in a project, which I found very confusing, but again I was able to just read the RFC to make sure that the bits/bytes were being ordered correctly.

### Getting Started, and Running into Difficulties
I began by defining some easy *function-like* macros, such as F(), G(), H() and I(). These were pretty easy to implement, as it was just a matter of translating the mathematical functions given in the RFC into C code.

After that, though, it got a lot more difficult. I decided I would use a file as input to the hashing function. I had to look up how to read an entire file into memory in C, stored in a byte array. Originally, I decided to store blocks in a linked list. This worked, however writing the padding code proved to be difficult to do elegantly and efficiently. Another mistake I made was not calculating how many blocks were needed ahead of time. This makes inserting the padding a lot harder, since you have to dynamically add blocks as you read the file buffer.

### Making Things Easier
Eventually, I decided that a 2 dimensional array of *word*s would be the best way of representing a series of blocks. This makes it easier to iterate over the values in each block. Also, I decided that the padding bits should be added to the input message *before* converting the input into blocks, not after.

Using these ideas makes the file reading, padding, and block building steps fairly straight forward.

From there, the blocks are fed straight into the MD5 algorithm. There's not really much to say about this, it's pretty much exactly what the RFC says to do in pseudocode, I just had to translate it into C code.

### Fixing Bugs and Finishing Up
Once I was able to get a hash value out, the last step to the algorithm was fixing all the bugs. These were mostly typos, which isn't surprising, since there were so many values to copy from the RFC. Because of how hashing works, the hash value is *completely different* to the expected value if *anything* is wrong, even if a single bit is out of place. This is because of the [avalanche effect](https://en.wikipedia.org/wiki/Avalanche_effect). I had also gotten the *endianness* wrong when constructing a *word* value from 4 bytes. Eventually though, the hash came out right, and from there it was just a case of cleaning up and refactoring the code, allowing the user to enter their own input to be hashed, etc.

## Simplification: Using Bytes
For my implementation of the algorithm, I made a simplification in one area: the input to the algorithm can only be given in **bytes**. The RFC describes an algorithm which can process an exact number of **bits**. However, in practice, it would be rare for someone to want to get the hash value of a specific pattern of bits. In the vast majority of cases (passwords or files, for example), **bytes** are the lowest format of data that is used. Because of this, I think it's acceptable to use this simplification.
