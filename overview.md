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
The purpose of the application is to demonstrate that it can produce the correct MD5 hash of any input. As such, running the program will first run the MD5 algorithm against all 9 test files in *test_input/*:

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

As shown, my implementation produces the correct hash for all 9 test files. It also produces the correct hash for the test cases provided in the RFC, but I wanted my own test cases to test the boundaries of the different padding lengths.

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

After that, the user is free to enter any string they want into the console (arbitrarily limited to 1000 characters) to produce the MD5 hash of that string. Entering *EXIT* exits the application.

```shell
Enter a string to hash, or EXIT to exit: Hello, World!
                         MD5 Hash value: 65a8e27d8879283831b664bd8b7f0ad4

Enter a string to hash, or EXIT to exit: password123
                         MD5 Hash value: 482c811da5d5b4bc6d497ffa98491e38

Enter a string to hash, or EXIT to exit: EXIT
                         MD5 Hash value: a42b2fb0e720a080e79a92f4ca97d927

ronan@ronan-desktop:~/code/md5$
```

