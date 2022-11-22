#!/bin/bash

clear 

gcc ../src/main.c -o main.o -c
gcc ../src/ascii_viewport.c -o ascii_viewport.o -c
gcc ../src/terminal_functions.c -o terminal_functions.o -c

gcc -o a.out terminal_functions.o ascii_viewport.o main.o

rm *.o