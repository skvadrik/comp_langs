#include "compiler.h"

void compile (AST * ast, std::vector<unsigned char> & code)
{
    switch (ast->type)
    {
        case AST::NUMBER:
        {
            // mov eax, <imm32> ; b8 <byte1> <byte2> <byte3> <byte4> (little-endian)
            unsigned char byte1 = (ast->value.number >> (8 * 0)) & 0xFF;
            unsigned char byte2 = (ast->value.number >> (8 * 1)) & 0xFF;
            unsigned char byte3 = (ast->value.number >> (8 * 2)) & 0xFF;
            unsigned char byte4 = (ast->value.number >> (8 * 3)) & 0xFF;
            code.push_back (0xb8);
            code.push_back (byte1);
            code.push_back (byte2);
            code.push_back (byte3);
            code.push_back (byte4);
            return;
        }
        case AST::ADD:
        {
            compile_operands (ast, code);
            // add eax, ebx ; 01 d8
            code.push_back (0x01);
            code.push_back (0xd8);
            return;
        }
        case AST::SUB:
        {
            compile_operands (ast, code);
            // sub eax, ebx ; 29 d8
            code.push_back (0x29);
            code.push_back (0xd8);
            return;
        }
        case AST::MUL:
        {
            compile_operands (ast, code);
            // imul ebx ; f7 eb
            code.push_back (0xf7);
            code.push_back (0xeb);
            return;
        }
        case AST::DIV:
        {
            compile_operands (ast, code);
            // cdq ; 99, sign-extend edx:eax of eax
            code.push_back (0x99);
            // idiv ebx ; f7 fb
            code.push_back (0xf7);
            code.push_back (0xfb);
            return;
        }
    }
}

void compile_operands (AST * ast, std::vector<unsigned char> & code)
{
    compile (ast->value.operands.right, code);
    // push rax ; 50
    code.push_back (0x50);
    compile (ast->value.operands.left, code);
    // pop rbx ; 5b
    code.push_back (0x5b);
}
