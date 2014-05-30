#include "vm_bytecode.h"

void vm_bytecode (Node * node, std::vector<Insn> & bytecode)
{
    switch (node->type)
    {
        case Node::OP:
        {
            vm_bytecode (node->value.op->left, bytecode);
            vm_bytecode (node->value.op->right, bytecode);
            switch (node->value.op->type)
            {
                case Op::ADD:
                    bytecode.push_back (Insn (Insn::ADD));
                    return;
                case Op::SUB:
                    bytecode.push_back (Insn (Insn::SUB));
                    return;
                case Op::MUL:
                    bytecode.push_back (Insn (Insn::MUL));
                    return;
                case Op::DIV:
                    bytecode.push_back (Insn (Insn::DIV));
                    return;
            }
        }
        case Node::NUMBER:
            bytecode.push_back (Insn (node->value.number));
            return;
    }
}
