#include "code.h"
#include "vm_jit.h"

int vm_jit (std::vector<Insn> & bytecode)
{
    Code code;
    for (unsigned int i = 0; i < bytecode.size (); ++i)
    {
        switch (bytecode[i].opcode)
        {
            case Insn::ADD:
            {
                // pop rbx ; 5b
                code.save_byte (0x5b);
                // pop rax ; 58
                code.save_byte (0x58);
                // add eax, ebx ; 01 d8
                code.save_byte (0x01);
                code.save_byte (0xd8);
                // push rax ; 50
                code.save_byte (0x50);
                break;
            }
            case Insn::SUB:
            {
                // pop rbx ; 5b
                code.save_byte (0x5b);
                // pop rax ; 58
                code.save_byte (0x58);
                // sub eax, ebx ; 29 d8
                code.save_byte (0x29);
                code.save_byte (0xd8);
                // push rax ; 50
                code.save_byte (0x50);
                break;
            }
            case Insn::MUL:
            {
                // pop rbx ; 5b
                code.save_byte (0x5b);
                // pop rax ; 58
                code.save_byte (0x58);
                // imul ebx ; f7 eb
                code.save_byte (0xf7);
                code.save_byte (0xeb);
                // push rax ; 50
                code.save_byte (0x50);
                break;
            }
            case Insn::DIV:
            {
                // pop rbx ; 5b
                code.save_byte (0x5b);
                // pop rax ; 58
                code.save_byte (0x58);
                // cdq ; 99, sign-extend edx:eax of eax
                code.save_byte (0x99);
                // idiv ebx ; f7 fb
                code.save_byte (0xf7);
                code.save_byte (0xfb);
                // push rax ; 50
                code.save_byte (0x50);
                break;
            }
            case Insn::PUSH:
            {
                // push <imm32> ; 68 <byte1> <byte2> <byte3> <byte4> (little-endian)
                unsigned char byte1 = (bytecode[i].number >> (8 * 0)) & 0xFF;
                unsigned char byte2 = (bytecode[i].number >> (8 * 1)) & 0xFF;
                unsigned char byte3 = (bytecode[i].number >> (8 * 2)) & 0xFF;
                unsigned char byte4 = (bytecode[i].number >> (8 * 3)) & 0xFF;
                code.save_byte (0x68);
                code.save_byte (byte1);
                code.save_byte (byte2);
                code.save_byte (byte3);
                code.save_byte (byte4);
                break;
            }
        }
    }
    // pop rax ; 58
    code.save_byte (0x58);
    // ret ; c3
    code.save_byte (0xc3);

    return code.exec ();
}
