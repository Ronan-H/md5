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

## How it Works

How it works here.

## How I Wrote it

How I wrote it here.
