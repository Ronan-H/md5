# MD5 - Rescoped

## Table of Contents:
 * [Introduction](#introduction)
 * [Repo Contents](#repo-contents)
 * [How to Compile and Run](#how-to-compile-and-run)
 * [Command Line Options](#command-line-options)
 * [Algorithms Used](#algorithms-used)
 * [Complexity of MD5](#complexity-of-md5)
 * [Complexity of Algorithms that can Reverse MD5](#complexity-of-algorithms-that-can-reverse-md5)
 * [Trading Space for Time](#trading-space-for-time)
 * [Taking Shortcuts](#taking-shortcuts)
 * [References](#references)

## Introduction
In the original version of this project, I implemented the MD5 algorithm in C, and proved it's accuracy using a suite of tests. In the rescoped version of this project, I have expanded on the implementation, adding command line options, and a cracking utility. In this document, I go into greater detail to describe the MD5 algorithm, and demonstrate how MD5 hashes can be cracked using the approaches of brute-force by trialling all the possible permutations of the input space, and lookup tables. I also explain how those approaches are infeasible for larger sizes of input, and break down both MD5 and those reversal algorithms to show the worst-case time and space complexity when different approaches are used.

As part of this document being *"pitched at students  in  the  year  below"*, I have included some question and answer blocks. The question should be thought about before opening the answer and reading on.

## Repo Contents
**resources/**: Resources for use in this document. This directory can be ignored.

**test_input/**: Test input files. Contains 9 different files of different sizes and types in an attempt to cover as many edge cases as possible.

**.gitignore**: Gitignore file, ignoring IDE and compilation files.

**README.md**: The README file, originally written for the 30% version of this project.

**main.c**: Program entry point, containing *main()*. Processes command line options, and implements the hashing, cracking, and testing suites. Makes use of *md5.c* for producing hash values.

**md5.c**: MD5 implementation in C. Also contains functions to convert a byte array into blocks, display 32-bit *word* values as bits, display blocks, etc.

**md5.h**: Header file containing definitions for both *md5.c* and *main.c*.

**overview.md *(this file)***: Documentation added for the completion of the 70% rescoped portion of the project.

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
```
./md5 [options, read the next section for details]
```

## Command Line Options
I have included four different command line options for this project. To see what they are and how to use them, run the program with the ```--help``` option:

```
ronan@ronan-desktop:~/code/md5$ ./md5 --help

Usage: ./md5 [--test] [--help] [--hash | --crack]
  Default argument if no option is specified: --hash

Options:
  --help:  displays this help message
  --test:  runs the suite of tests against the MD5 algorithm
  --hash:  hashes user input in a loop (DEFAULT OPTION)
  --crack: runs the MD5 cracking utility

Note: only one of --hash or --crack will be used if both are specified, in that order
      (--test and --help can be used alongside any other option, or on their own)

Exiting...
```

### Option: --test
The testing of software is an important part of evaluation, and can also be used to drive the development of a project if the tests are written **before the code** (test-driven development). Running the program with the ```--test``` option will run the MD5 algorithm against all nine of the test files in *test_input/*:

```
ronan@ronan-desktop:~/code/md5$ ./md5 --test

----------------- TEST SUITE ------------------

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

---------------- END OF TESTS -----------------

Exiting...
```

As shown, the implementation produces the correct hash for all 9 test files. It also produces the correct hash for the test cases provided in the RFC, but I wanted my own test cases to test the boundaries of the different padding lengths. Notice that I included different file formats for some of the tests as well, including *WAV*, an audio format, and *PNG*, an image format. It is important to test your code against as wide a range of inputs as possible, without making any assumptions about inputs that "should work". Remember, purposely writing tests that you know will pass **are of no use**. Failing tests highlight flaws in your code, which you can fix.

The *EXPECTED* hash value for each test case was found by running the ```md5sum``` command (included in most "Unix-like" operating systems) for all files in the *test_input/* directory, like so: ```md5sum test_input/*```, which produces the following output:

```
ronan@ronan-desktop:~/code/md5$ md5sum test_input/*
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

### Option: --hash
If the ```--hash``` option is given, or if no option is given, the user can enter any string they want into the console (arbitrarily limited to 1000 characters) to produce the MD5 hash of that string. Entering *EXIT* exits the application. I decided to also print the hash value of "EXIT" before exiting, otherwise it would be impossible for the user to get the hash of that particular string.

```
ronan@ronan-desktop:~/code/md5$ ./md5 --hash
Enter a string to hash, or EXIT to exit: Hello, World!
                         MD5 Hash value: 65a8e27d8879283831b664bd8b7f0ad4

Enter a string to hash, or EXIT to exit: password123
                         MD5 Hash value: 482c811da5d5b4bc6d497ffa98491e38

Enter a string to hash, or EXIT to exit: EXIT
                         MD5 Hash value: a42b2fb0e720a080e79a92f4ca97d927

Exiting...
```

### Option: --crack
As a talking point for this document, I have also added the ```--crack``` option. When this option is given, the user can enter any MD5 hash, and the program will attempt to reverse it. The time taken to brute-force every permutation of characters for a certain length is shown, to demonstrate that the time taken to crack hashes in this fashion grows exponentially with the size of the input. As shown when the option is used, the crack utility exhausts the permutations of all strings containing lowercase alphabet characters, including the empty string, up to a maximum length of 5 characters. If a match is found, or no match is found after exhausting the entire search space, the program exits.

```
ronan@ronan-desktop:~/code/md5$ ./md5 --crack
Expected hash input format: 32 lowercase hex characters, E.g.: 5d41402abc4b2a76b9719d911017c592
Expected plaintext alphabet: [a-z]*
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

To generate all the permutations for a string of a given length, I made use of recursion.

Why did I choose to use recursion instead of using nested loops, and what are the issues with using nested loops?

<details>
  <summary>My Answer</summary>
  
  To generate all of the permutations for a string of length *n*, *n* nested loops are needed. Having 5 nested loops would be outrageous, and that's a pretty small string. Also, what if you decided that you don't want the program to exit until it finds the matching hash, or you wanted to let the user specify the max size of *n*, making *n* a variable length? In those cases, nested loops cannot used, since you can't have an arbitrary number of them. This is where a recursive solution comes in handy: it's short, tidy, and flexible.
  
</details>

## Algorithms Used
The process of producing an MD5 hash can be broken down into two separate algorithms: one to manipulate the input to be of the format that the hashing algorithm expects, and the MD5 hashing algorithm itself. MD5 expects the input to be in the form of fixed length **blocks**, where each block is exactly 512 bits long. To fill up any extra space, and to ensure that doing so does not compromise the security of the hash, a padding scheme is used to fill up the rest of the space in the blocks (I.e., appending bits set to 0 for the remainder of the output is **not good enough**).

Here is a breakdown of how I converted a simple array of input bytes into a format which is usable for the MD5 hashing algorithm:

### Constructing Blocks, Step by Step
1. Copy the input into a new byte array ```buffer```, allocating some extra space at the end for the padding scheme. Copying the array also prevents the original array being passed in from being modified. This is important because the crack utility repeatedly makes use of the same input byte buffer.
2. Compute the exact number of padding bytes needed to append to the message.
   * For this I came up with the formula ```paddingBytes = 65 - ((length + 8) % 64 + 1)```
3. Compute the total number of bytes needed for all blocks: ```totalBytes = length + paddingBytes + 8```
   * 8 bytes are needed at the end to represent the input length; a 64-bit unsigned integer. This is specified in the padding scheme, outlined in the RFC.
   * ```totalBytes``` is now guaranteed to be evenly divisible by 64, I.e. full blocks can be constructed with no bytes left over.
4. Append the first bit of padding, a 1. This is easy now, we just write the integer 128 to the above array. 128 represents 1 followed by 7 zeroes in binary.
5. Write all the remaining 0's of padding. Again, this is pretty easy. We're just filling the rest of the array with 0's, **up until we reach the 8 bytes of input length at the end**. We could write 0's there if we wanted, but they would just be overridden in the next step.
6. Use bitwise operations to represent the input length in the last 8 bytes.
7. Create a 2D *word* array, and read each group of 4 bytes from the ```buffer``` array into each *word* value. Again, this is pretty straight forward, because we have already guaranteed that the array can be divided into blocks evenly. It's important to remember here that **bits** are grouped in **high-order**, and bytes are grouped in **low-order**, as the RFC specifies. For me, this was one of the most confusing aspects of the assignment to get right.

<div align="center"><kbd><img style="border: thin solid black" src="./resources/block-building-diagram.jpeg" alt="Plot of logarithmic time to brute-force" width="500px" align="center"></kbd></div>

Once the input has been processed into blocks, or at least into a format that can easily be interpreted as blocks of bytes (the RFC represents blocks using a single dimension array of bytes, where each chunk of 64 bytes can be viewed as an individual block), it now has to be processed using the actual MD5 hashing algorithm. This is actually the easiest part of the process, since the RFC outlines exactly what to do. It is not actually necessary to understand what each step in the algorithm is trying to achieve, in order to implement it.

### Producing the MD5 Hash Value
Here are the steps involved in generating an MD5 hash value, based on the input format created above:

1. Generate T[]. *"Let T[i] denote the i-th element of the table, which
   is equal to the integer part of 4294967296 times abs(sin(i)), where i
   is in radians*". Some implementations of MD5 hard code these values, but I prefer to generate them in a loop, storing them in an array for use in the following steps.
2. Initialise the "MD Buffer": four 32-bit *word* values A, B, C, and D, where each variable gets a specific, predefined value. Values will be added to this buffer after each round, and afterwards, these values make up the hash value itself.
3. Process each block in turn. In my implementation, this is simply a loop through each block X, from the array of blocks M.
   1. Initialise variables AA, BB, CC, and DD, initialised to A, B, C, and D respectively.
   2. Manipulate A, B, C, and D, in a series of four "rounds", each with sixteen individual operations. These incorporate bytes from the current block X, values from T in ascending order of index, and bitwise logic defined in the auxiliary functions F(), G(), H(), and I. In these steps, the input bytes get scrambled in a way that is not directly reversible.
   3. Add AA to A, BB to B, and so on. Here, hashed data from the current round accumulates with hashed data from all previous rounds.
4. Print ABCD in low-byte order, expressed as 32 lowercase hexadecimal characters, where each series of 8 characters represents A, B, C, and D. In my implementation, this string is actually returned from the function instead of being printed, so that the hash can be used in other ways. This string of characters is the final MD5 hash value.

## Complexity of MD5
What order of time and space complexity should be expected for any hashing algorithm?

<details>
<summary>My Answer</summary>
  
Hashing algorithms are useful in a variety of ways, including password hashing, which will be a focus in this document, and hashing files to verify that they have remained unchanged. In the latter use case mentioned, input to the MD5 algorithm could potentially be *very large*. For example, consider somebody downloading a copy of Linux Mint. The ISO file they download would be about 2 gigabytes in size. In the case of downloading operating systems, it is strongly advised to verify the integrity of the ISO file before installing, as installing a compromised OS would be catastrophic.

We will call the number of bytes which the MD5 algorithm has to process *n*. So, in this case, the input is 31,250,000 bytes long, so *n* is 31,250,000. As we can see, this an enormous number, so for MD5 to be useful as a hashing algorithm, we would want it to run with a time and space complexity of **O(n) time or better**. Any complexity worse than this would surely be too slow, or take up too much memory for larger inputs.

With that in mind, let's take a look at the time and space complexities for my implementation of MD5.

</details>

### Building Blocks
1. Copying the input array, leaving some extra space for padding: every extra byte needed to be copied takes some fixed amount of time to copy, and one extra byte of space. As such, the time and space complexities for this operation must be **O(n)**. The extra space allocated is constant for all sizes of inputs, and therefore is not relevant to the time and space complexities.
2. Computing the number of padding bytes needed, appending the first bit of padding, etc...: this is a **constant time** operation, running in **O(1)** time and space. In other words, no matter what the input is, these operations take the same amount of time, and the same amount of space.
3. Appending the padding bits: this is difficult to classify, since the number of padding bits needed **changes depending on *n***, and is not proportional to the size of the input. However, since there is a small, fixed upper bound on the number of padding bits which are needed with varying lengths of input, I would say this step could be approximated as **O(1)** time and space.
4. Copying the input array into a series of *word* blocks: every byte of input must be individually copied to a block, and doing so uses array accesses and bitwise operations which run in constant time, so the overall time taken for this operation is **O(n)**. In a similar way, one byte of extra space is needed for every byte of input, so this also uses **O(n)** space.

The overall runtime of building blocks is the **worst** time and space complexity from the whole process, meaning a time and space complexity of **O(n)** overall.

### Hashing
1. Computing T[]: calculates T[i] for **64** indices regardless of input, using 4 bytes of memory for each index. This means this is a **constant time and space** operation, or **O(1)** for both.
2. Initialising the MD buffer A, B, C, D: again, this is the same regardless of input, so it must be an **O(1)** operation for both time and space.
3. For every *block*, runs 4 rounds of 16 operations. This runs a *fixed number* of operations *per block*. Therefore, it runs in **O(n)**, where *n* denotes the number of **blocks** this time, not **input bytes**. The implication of this is that in cases where the input is longer but the number of blocks remains the same, this part of the MD5 algorithm runs in the same amount of time. Also, no extra memory is needed in this loop, it just uses existing data structures. Therefore, it uses "constant extra space" for all iterations of the loop, meaning **O(1) space complexity**.

The overall running time of the hashing algorithm is then the worst complexity of those steps, so it runs in **O(n)** time, but uses **O(1)** extra space, since the blocks are already supplied from the previous step

As predicted above, the algorithm as a whole runs in **O(n)** time, and **O(n)** space. It also seems to me that **a secure hashing algorithm can not run better than *O(n)* time**, since every byte has to be used in some operation. If any input byte was ignored, a change to those bytes would **not change the output of the algorithm**, producing a **collision**, making the hash insecure and effectively unusable. Surely, **a hashing algorithm can not run better than *O(n)* space** either, since the entire input has to be held in memory. Based on that, I would guess that the time and space complexities of my implementation of MD5 cannot be improved upon. That does not mean that it can't be optimized, though. For example, hard coding the values of T[] instead of generating the array each time the hash function is run, and representing blocks as a 2D array instead of a 1D array, would probably improve the efficiency of the algorithm. In these cases, I chose **readability** and **maintainability** over **optimization/efficiency**. This trade-off is important to recognise as a software developer, and care should be taken when deciding which side should be sacrificed.

## Complexity of Algorithms that can Reverse MD5
One of the main features of hashing algorithms is that they are *one-way*: for some value *x*, computing *f(x)* is "easy", I.e. it can be done in a feasible amount of time. However, given a value *f(x)*, directly reversing the function to compute *x* is impossible. This can be seen because the hash *f(x)* is of fixed length, and the input can be *any length*, so some *x* values must map to the same value *f(x)*. As such, how can you directly reverse *f(x)* if there are multiple values of *x* which hash to that same *f(x)*?

Although directly reversing a hash function is impossible, you can still repeatedly hash different inputs until you find a hash that matches the one that you're trying to reverse. If a match is found, the input you hashed is likely to be the same as the reference hash's input, since there are a huge number of possible hash values (2<sup>128</sup>: all permutations of bits for MD5's 128-bit output, assuming all hashes are a possible result). This method of reversal is known as a *brute-force* attack, and is the simplest form of attack, akin to trying all the combinations of someone's luggage lock (000, 001, 002, ... 999).

Although this attack is simple, the time complexity of it makes it infeasible in a lot of situations. Every extra bit which you are brute-forcing through *doubles* the number of hashes needed to exhaust all of the permutations. This implies a time complexity in the order of **O(2<sup>n</sup>)**. This is one of the **worst** time complexities you can have. Just to brute-force input to the MD5 algorithm that produces a single block (512 bits), at worst you would have to generate and compare 2<sup>512</sup> hash values, which is a massive number and completely infeasible.

However, in the area of **password cracking**, bit permutations do not need to be exhausted completely. This is because only some permutations of bits actually represent a string of characters. In the case of the simple cracking utility that I created for this project, only the lowercase letters *a to z* are tested. As such, the complexity is reduced to the order of **O(26<sup>n</sup>)**, where *26* is the size of the alphabet being used, and *n* is the *length of the string, in characters*. For example, to brute-force through all the strings of length 5 from an alphabet a-z, at worst it could take **26<sup>5</sup> = 11,881,376** iterations. Since each hash can be computed quite quickly, strings of this length can actually be cracked in a short amount of time. Reversing a string of unknown length up to a length *n* takes 26<sup>0</sup> + 26<sup>1</sup> + ... + 26<sup>n</sup> hash generations at worst, but this would still be represented as having a worst-case time complexity of **O(26<sup>n</sup>)**.

A plot can be created, using the timer values shown by the **--crack** option, to show the exponential relationship between the length of a string and the time it takes to exhaust all the permutations for strings of that length, in this case to reverse a hash:

<div align="center"><kbd><img style="border: thin solid black" src="./resources/brute-force-time.png" alt="Plot of time to brute-force" width="500px" align="center"></kbd></div>

As shown, the time taken to brute-force through all strings of length 5 completely overshadows the time taken to brute-force strings of a smaller length, even if combined. This shows why many websites require passwords to **be at least 8** characters in length: just adding a few extra characters to a smaller password can make it infeasible to crack with this method.

As a further confirmation of the exponential running time of this cracking function, we can we can use log<sub>26</sub> for values on the y-axis:

<div align="center"><kbd><img style="border: thin solid black" src="./resources/log-brute-force-time.png" alt="Plot of logarithmic time to brute-force" width="500px" align="center"></kbd></div>

The approximate straight line confirms the exponential nature of the function.

It is important to note that the irregularities in the timer values, especially for shorter string lengths, are because **timing code execution speed is not reliable**.

Do you know some of the reasons why this might be?

<details>
  <summary>My Answer</summary>

  * You don't know the optimizations that the compiler, or runtime environment in the case of languages like Java, might be making.
  * You don't know what other operations your CPU might be performing at the same time that your code is running, E.g. the operations of other programs, or the OS itself.
  * You might not realise that you are timing more or less operations than you actually intend to, E.g. initialisation code.
  * In the case of timing the same code on multiple different systems, any differences in hardware, operating system, or configuration etc. can cause wild differences in timer values.
  * And more, similar reasons...

  As such, code execution times should be taken with a pinch of salt. Big O notation should be used instead, which provides a method of measuring code performance that is unaffected by all of the issues mentioned above. For the purposes of this document, though, I think the plots offer a great visualisation of this algorithms performance over varying values of *n*.

</details>

That's just one method of reversing a hashed string, though. Specific to the area of password cracking, there are more powerful methods.

### Trading Space for Time
In software, there is a constant battle between **space** (I.e. memory) constraints, and **time** constraints. In many cases, **space** can be "traded" for faster execution **time**, and vice versa. In the above example of an algorithm which can reverse an MD5 hash, the time taken to reverse a hashed string of length *n* grows *exponentially* with increasing values of *n*. However, it can be seen that the algorithm actually uses **very little space**. For a string length of *n*, a buffer of size *n* to store and manipulate the characters which it's trying to hash against the reference hash is needed, giving a space complexity of **O(n)** with a tiny overhead for increasing values of *n*. In practice, the space used by the algorithm is tiny, so space should gladly be sacrificed to reduce the exponential time complexity.

#### Simple Lookup Tables
In many areas of computing, lookup tables can be used to "precompute" the result of a computation, allowing the result to be looked up in a data structure during runtime instead of having to compute the result again. One example of this is caching, which is prevalent in the areas of DNS lookups and web page requests, to name a few. In the area of password cracking, lookup tables can be formed by mapping passwords to their corresponding hash value.

Why is this useful?

<details>
  <summary>My Answer</summary>

  Because the time needed to retrieve an MD5 hash value for a certain input from a data structure is much, much less than the time it would take to generate the hash for that value. In general, this is the reason that caching is useful, too.

</details>

**Time and Space Complexity**

The time complexity of retrieving elements from a lookup table depends on the data structure used:

* For a simple array of passwords and corresponding hash values in no particular order, every password must be iterated through until the matching password is found. In this case, lookup time is of the order **O(n)**, where *n* is the number of password-hash pairs in the table. This is assuming the worst-case, where the password wasn't actually in the table at all, so all passwords had to be checked for a match. The space complexity is also clearly **O(n)**, where every extra password and hash pair add a certain amount of memory overhead.

* If the above array is **sorted** by how **common** each password is, on average, passwords can be cracked much quicker. A match for common passwords would be found sooner. However, the worst-case time and space complexity remains the same.

* If the above data structure is **sorted** with passwords in **lexicographical order**, a [binary search](https://en.wikipedia.org/wiki/Binary_search_algorithm) can be performed to lookup passwords, reducing the time complexity to the order of **O(log n)** in the worst-case. Since the array was just reordered, the space complexity remains the same.

* Using a **hash table** instead can reduce the time complexity down to **O(1)**, but in reality, a huge lookup table could cause collisions, causing the time complexity to approach a worst-case of **O(n)**. Space complexity remains **O(n)** as with the other data structures, but again in reality hash tables use more memory than traditional arrays, with the overhead of buckets and collision chains.

As shown, it is possible for passwords to be cracked in **O(1)** time, if space is sacrificed, with the drawback that some passwords may not be in the table, and so can not be cracked by this method. This method is useful in cracking individual passwords. Using a similar technique, large portions of a **database** of password hashes can be cracked much more quickly, if the process is **reversed:**

#### Reverse Lookup Tables
Above, we started with a password hash, and searched for it in a table mapping hashes to plaintext passwords. Instead, we can start with the hash of a common password, and lookup the **database** of password hashes to ask the question *"I have this hash; who in the database has a password who hashes to this value?*. In this fashion, *many* password hashes can be cracked with a *single* pass through the database. In a large database, a large percentage of user passwords could be cracked by just performing a reverse lookup of the top 10 most commonly used passwords, for example. Another useful property of this idea is that less time is wasted trying to lookup a user's password who's hash isn't in the table at all.

**Time and Space Complexity**

Using this method, passwords for **all users** whose corresponding password hash is in the lookup table can be cracked in **O(n)** time, where *n* is the number of users in the database. Notably, comparing every password hash to every user's hash takes **O(nm)** at worst, where *n* is the number of users and *m* is the number of passwords in the database, for lookup tables **and** reverse lookup tables. Space complexity remains the same for both as well, having to store both the database and lookup table in memory.

### Taking Shortcuts
The above methods of reversing a hash assumed that MD5 holds the following properties:

* **Collision resistance:** it is computationally infeasible to find two different inputs which produce the same hash
* **Preimage resistance:** it is computationally infeasible to find the original message from it's produced hash

MD5 collisions have been found and used as an attack vector, and a theoretical preimage attack has also been found. That's not to say that MD5 hashes can be directly reversed, but being able to produce collisions can be just as bad. As such, MD5 is now considered insecure, and it is advised that the SHA-2 set of cryptographic functions should be used instead.

## References
[Hashing Algorithms and Security - Computerphile](https://youtu.be/b4b8ktEV4Bg)

* General information about hashing algorithms, including their uses and potential vulnerabilities, with some focus on MD5 in particular.

[Password Cracking - Computerphile](https://youtu.be/7U-RbOKanYs)

* Great information about what password cracking is, and the various methods that can be used, such as brute-forcing through all the permutations of characters, and lookup tables, which I talked about a lot in this document. Talks about the approximate time it takes to crack passwords of a certain length using those methods, and shows how longer passwords can be exponentially harder to crack. Also demonstrates how powerful these methods can be when paired with a ridiculous server machine with 4x NVIDIA Titan X graphics cards installed.

[MD5 - Wikipedia](https://en.wikipedia.org/wiki/MD5)

* Detailed information on the MD5 algorithm. This article was particularly useful in helping me write about the vulnerabilities of MD5, for example. I read several other Wikipedia articles related to hashing, too.

[Hash table - Wikipedia](https://en.wikipedia.org/wiki/Hash_table)

* Used to double check what we were taught about the time and space complexities of hash tables in 2<sup>nd</sup> and 3<sup>rd</sup> year of the course. Used this resource to help me talk about using a hash table as a lookup table, mapping MD5 hash values to passwords.
