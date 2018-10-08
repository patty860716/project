#!/bin/bash
gcc opcode.c -c -o opcode.o
ar rcs opcode.a opcode.o
gcc project.c opcode.a
./a.out

