#ifndef __VM_BYTECODE__
#define __VM_BYTECODE__

#include <vector>

#include "parser.h"

struct Insn
{
    enum Opcode {ADD, SUB, MUL, DIV, PUSH} opcode;
    int number;

    Insn (Opcode op)
        : opcode (op)
        , number (0)
    { }

    Insn (int n)
        : opcode (PUSH)
        , number (n)
    { }
};

void vm_bytecode (AST * ast, std::vector<Insn> & bytecode);

#endif // __VM_BYTECODE__
