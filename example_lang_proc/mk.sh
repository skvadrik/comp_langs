#!/bin/sh

g++ -O2 -o all \
    parser.cpp \
    interpreter.cpp \
    compiler.cpp \
    vm_bytecode.cpp \
    vm_run.cpp \
    vm_jit.cpp \
    main.cpp

./all "$1"

chmod +x code
./code
echo "c> $?"
