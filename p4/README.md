# Instructions

This directory contains the patch DIF file that allows for access via any passwords.

The patcher directory contains a program to patch the target binary with whatever password you supply. To compile and run:

```shell
$  cd patcher
$  make
```

```shell
$  ./patcher -i <PATH_TO_BINARY> -p <NEW PASSWORD>
```

This program will takes your password and compute a SHA-1 hash which it will then use to replace the relevant portions of the binary you supply with the new hash. You will then be able to access the program using your new password.

Note: Max password length is 20

