#include "vm_bytecode.h"

void vm_bytecode (AST * ast, std::vector<Insn> & bytecode)
{
    switch (ast->type)
    {
        case AST::NUMBER:
            bytecode.push_back (Insn (ast->value.number));
            return;
        case AST::ADD:
        {
            vm_bytecode (ast->value.operands.left, bytecode);
            vm_bytecode (ast->value.operands.right, bytecode);
            bytecode.push_back (Insn (Insn::ADD));
            return;
        }
        case AST::SUB:
        {
            vm_bytecode (ast->value.operands.left, bytecode);
            vm_bytecode (ast->value.operands.right, bytecode);
            bytecode.push_back (Insn (Insn::SUB));
            return;
        }
        case AST::MUL:
        {
            vm_bytecode (ast->value.operands.left, bytecode);
            vm_bytecode (ast->value.operands.right, bytecode);
            bytecode.push_back (Insn (Insn::MUL));
            return;
        }
        case AST::DIV:
        {
            vm_bytecode (ast->value.operands.left, bytecode);
            vm_bytecode (ast->value.operands.right, bytecode);
            bytecode.push_back (Insn (Insn::DIV));
            return;
        }
    }
}
