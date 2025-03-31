About

Project for adding unlimited quantity of numbers from which each can have unlimited quantity of characters (both in boundary of INT_MAX) working for binary, octal, decimal and hexadecimal numbers (choice needs to be entered in #define SYSTEM in 7th line). The code is safe from errors, written in C. This is an upload of a project made in january 2024

It is advised to compile it on Linux using command:

gcc -g -Wall -pedantic main.c -o TestMain

And run it using command:
valgrind --leak-check=full ./TestMain infile outfile
