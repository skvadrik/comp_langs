#ifndef __VM_BYTECODE__
#define __VM_BYTECODE__

#include <vector>

#include "parser.h"

struct Insn
{
    enum Opcode {ADD, SUB, MUL, DIV, NUMBER} opcode;
    int number;

    Insn (Opcode op)
        : opcode (op)
        , number (0)
    { }

    Insn (int n)
        : opcode (NUMBER)
        , number (n)
    { }
};

void vm_bytecode (AST * ast, std::vector<Insn> & bytecode);

#endif // __VM_BYTECODE__
