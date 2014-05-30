#include <stack>

#include "vm.h"

int vm_run (std::vector<Insn> & bytecode)
{
    std::stack<int> stack;
    for (unsigned int i = 0; i < bytecode.size (); ++i)
    {
        switch (bytecode[i].opcode)
        {
            case VM_ADD:
            {
                int l = stack.top (); stack.pop ();
                int r = stack.top (); stack.pop ();
                stack.push (l + r);
                break;
            }
            case VM_SUB:
            {
                int l = stack.top (); stack.pop ();
                int r = stack.top (); stack.pop ();
                stack.push (r - l);
                break;
            }
            case VM_MUL:
            {
                int l = stack.top (); stack.pop ();
                int r = stack.top (); stack.pop ();
                stack.push (l * r);
                break;
            }
            case VM_DIV:
            {
                int l = stack.top (); stack.pop ();
                int r = stack.top (); stack.pop ();
                stack.push (r / l);
                break;
            }
            case VM_PUSH:
                stack.push (bytecode[i].number);
                break;
        }
    }
    return stack.top ();
}
