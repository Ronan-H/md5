# MD5 - Rescoped

## Table of Contents
Table of contents here

## Introduction


## Command line options
I have included four different command line options for this project. To see what they are and how to use them, run the program with the ```--help``` option:

```shell
ronan@ronan-desktop:~/code/md5$ ./md5 --help

Usage: ./md5 [--test] [--help] [--hash | --crack]
  Default argument if no option is specified: --hash

Options:
  --help:  displays this help message
  --test:  runs the suite of tests against the MD5 algorithm
  --hash:  hashes user input in a loop (DEFAULT OPTION)
  --crack: runs the MD5 cracking utility

Note: only one of --help, --hash, or --crack will be used, in that order
      (--test and --help can be used alongside any other option, or on their own)

Exiting...
```



## Option: --test
The purpose of this option is to demonstrate that the program can produce the correct MD5 hash of any input. As such, running the program with this argument supplied will run the MD5 algorithm against all 9 test files in *test_input/*:

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

As shown, the implementation produces the correct hash for all 9 test files. It also produces the correct hash for the test cases provided in the RFC, but I wanted my own test cases to test the boundaries of the different padding lengths.

The *EXPECTED* hash value for each test case was found by running the ```md5sum``` command (included in most "Unix-like" operating systems) for all files in the *test_input/* directory, like so: ```md5sum test_input/*```, which produces the following output:

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

## Option: --hash
If the ```--hash``` option is given, or if no option is given, the user can enter any string they want into the console (arbitrarily limited to 1000 characters) to produce the MD5 hash of that string. Entering *EXIT* exits the application.

```shell
Enter a string to hash, or EXIT to exit: Hello, World!
                         MD5 Hash value: 65a8e27d8879283831b664bd8b7f0ad4

Enter a string to hash, or EXIT to exit: password123
                         MD5 Hash value: 482c811da5d5b4bc6d497ffa98491e38

Enter a string to hash, or EXIT to exit: EXIT
                         MD5 Hash value: a42b2fb0e720a080e79a92f4ca97d927

ronan@ronan-desktop:~/code/md5$
```

## Option --crack
As a talking point for this document, I have also added the ```--crack``` option. When this option is given, the user can enter any MD5 hash, and the program will attempt to reverse it. The time taken to brute-force every permutation of characters for a certain length is shown, to demonstrate that the time taken to crack hashes in this fashion grows exponentially with the size of the input.

```shell
ronan@ronan-desktop:~/code/md5$ ./md5 --crack
Expected hash input format: 32 lowercase hex characters, E.g.: 5d41402abc4b2a76b9719d911017c592
Expected plaintext alpahbet: [a-z]*
Trying up to plaintext length: 5

Enter a reference MD5 hash to crack: 5d41402abc4b2a76b9719d911017c592

Cracking...

Trying all permutations of length 0...
  No match, took 42 microseconds

Trying all permutations of length 1...
  No match, took 197 microseconds

Trying all permutations of length 2...
  No match, took 4831 microseconds

Trying all permutations of length 3...
  No match, took 55555 microseconds

Trying all permutations of length 4...
  No match, took 1233622 microseconds

Trying all permutations of length 5...

Match found!
  Result: 'hello'

Exiting...
```

## Algorithms used
Producing an MD5 hash can be broken down into two separate algorithms: one to manipulate the input to be of the format that the hashing algorithm expects, and the MD5 hashing algorithm itself. MD5 expects the input to be in the form of fixed length **blocks**, where each block is exactly 512 bits long. To fill up any extra space, and to ensure that doing so does not compromise the security of the hash, a padding scheme is used to fill up the rest of the space in the blocks (I.e., appending bits set to 0 for the remainder of the output is **not good enough**).

Here is a breakdown of how I converted a simple array of input bytes into a format which is usable for the MD5 hashing algorithm:

### Constructing Blocks, Step by Step
1. Compute the exact number of padding bytes needed to append to the message.
   * For this I came up with the formula ```paddingBytes = 65 - ((length + 8) % 64 + 1);```
2. Compute the total number of bytes needed for the byte buffer: ```totalBytes = length + paddingBytes + 8;```
   * 8 bytes are needed at the end to represent the input length; a 64 bit unsigned integer. This is specified in the padding scheme, outlined in the RFC.
   * ```totalBytes``` is now guaranteed to be evenly divisible by 64, i.e. full blocks can be constructed with no bytes left over.
3. Create a byte array ```buffer``` with length ```totalBytes```. Read the entire file into this array. There will be space left over for padding and input length bytes.
4. Append the first bit of padding, a 1. This is easy now, we just write the integer 128 to the above array. 128 represents 1 followed by 7 zeroes in binary.
5. Write all the remaining 0's of padding. Again, this is pretty easy. We're just filling the rest of the array with 0's, **up until we reach the 8 bytes of input length at the end**. We could write 0's here but we're just about to write the input length there anyway.
6. Use bitwise operations to represent the input length in the last 8 bytes.
7. Create a 2D *word* array, and read each group of 4 bytes from the ```buffer``` array into each *word* value. Again, this is pretty straight forward, because we have already guaranteed that the array can be divided into blocks evenly. It's important to remember here that **bits** are grouped in **high-order**, and bytes are grouped in **low-order**, as the RFC specifies. This was one of the most confusing aspects of the assignment to get right.

Once the input has been processed into blocks, or at least into a format that can easily be interpreted as blocks of bytes (the RFC represents blocks using a single dimensions array of bytes, where each chunk of 64 bytes can be viewed as an invividual block), it now has to be processed using the actual MD5 hashing algorithm. This is actually the easiest part of the process, since the RFC outlines exactly what to do. It is not actually necessary to understand what each step in the algorithm is trying to achieve, in order to implement it.

### Producing the hash value using MD5
1. Generate T[]. *"Let T[i] denote the i-th element of the table, which
   is equal to the integer part of 4294967296 times abs(sin(i)), where i
   is in radians*". Some implementations of MD5 hard code these values, but I prefer to generate them in a loop.
2. Initialise the "MD Buffer": four 32-bit "word" values A, B, C, and D, where each variable gets a specific, prefefined value. Values will be added to these after each round, and afterwards, these values make up the hash value itself.
3. Process each block in turn. In my implementation, this is simply a loop through Each block X, from the array of blocks M.
   1. Initialise variables AA, BB, CC, and DD, initialised to A, B , C and D respectively.
   2. Manipulate A, B, C, and D, in a series of four "rounds", each with sixteen individual operations. These incorporate bytes from the current block X, values from T in ascending order of index, and bitwise logic defined in the auxiliary functions F(), G(), H(), and I. In these steps, the input bytes get scrambled in a way that is irreversible.
   3. Add AA to A, BB to B, and so on. Here, hashed data from the current round accumulates with hashed data from all previous rounds.

## Complexity of MD5

## Complexity of algorithms that can reverse MD5

## References