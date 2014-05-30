#include <vector>

#include "parser.h"

enum Opcode
    { VM_ADD
    , VM_SUB
    , VM_MUL
    , VM_DIV
    , VM_PUSH
    };

struct Insn
{
    Opcode opcode;
    int number;

    Insn (Opcode op)
        : opcode (op)
        , number (0)
    { }

    Insn (int n)
        : opcode (VM_PUSH)
        , number (n)
    { }
};

void vm_bytecode (Node * node, std::vector<Insn> & bytecode);
int vm_run (std::vector<Insn> & bytecode);
int vm_jit (std::vector<Insn> & bytecode);
