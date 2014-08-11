#include <stack>

#include "vm_run.h"

int vm_run (std::vector<Insn> & bytecode)
{
    std::stack<int> stack;
    for (unsigned int i = 0; i < bytecode.size (); ++i)
    {
        switch (bytecode[i].opcode)
        {
            case Insn::ADD:
            {
                int l = stack.top (); stack.pop ();
                int r = stack.top (); stack.pop ();
                stack.push (l + r);
                break;
            }
            case Insn::SUB:
            {
                int l = stack.top (); stack.pop ();
                int r = stack.top (); stack.pop ();
                stack.push (r - l);
                break;
            }
            case Insn::MUL:
            {
                int l = stack.top (); stack.pop ();
                int r = stack.top (); stack.pop ();
                stack.push (l * r);
                break;
            }
            case Insn::DIV:
            {
                int l = stack.top (); stack.pop ();
                int r = stack.top (); stack.pop ();
                stack.push (r / l);
                break;
            }
            case Insn::NUMBER:
                stack.push (bytecode[i].number);
                break;
        }
    }
    return stack.top ();
}
