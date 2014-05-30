#include "vm.h"

void vm_bytecode (Node * node, std::vector<Insn> & bytecode)
{
    switch (node->type)
    {
        case NODE_OP:
        {
            vm_bytecode (node->value.op->left, bytecode);
            vm_bytecode (node->value.op->right, bytecode);
            switch (node->value.op->type)
            {
                case OP_ADD:
                    bytecode.push_back (Insn (VM_ADD));
                    return;
                case OP_SUB:
                    bytecode.push_back (Insn (VM_SUB));
                    return;
                case OP_MUL:
                    bytecode.push_back (Insn (VM_MUL));
                    return;
                case OP_DIV:
                    bytecode.push_back (Insn (VM_DIV));
                    return;
            }
        }
        case NODE_NUMBER:
            bytecode.push_back (Insn (node->value.number));
            return;
    }
}
