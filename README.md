# MD5
MD5 implementation in C (4th year Theory of Algorithms assignment)

## Table of Contents

Fill in shortcuts to each section here.

## Files
**md5.c:** MD5 implementation in C.

**README.md:** This README file.

**.gitignore:** Gitignore file, ignoring IDE and compilation files.

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