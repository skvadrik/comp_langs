#!/bin/sh

g++ -O2 -W -Wall -Wextra -o all \
    ast.cpp \
    code.cpp \
    compiler.cpp \
    gen_elf64.cpp \
    interpreter.cpp \
    main.cpp \
    parser.cpp \
    vm_bytecode.cpp \
    vm_jit.cpp \
    vm_run.cpp

./all "$1"

chmod +x code
./code
echo "c> $?"
