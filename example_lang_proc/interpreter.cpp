#include "interpreter.h"

int interpret (AST * ast)
{
    switch (ast->type)
    {
        case AST::NUMBER:
            return ast->value.number;
        case AST::ADD:
        {
            int l = interpret (ast->value.operands.left);
            int r = interpret (ast->value.operands.right);
            return l + r;
        }
        case AST::SUB:
        {
            int l = interpret (ast->value.operands.left);
            int r = interpret (ast->value.operands.right);
            return l - r;
        }
        case AST::MUL:
        {
            int l = interpret (ast->value.operands.left);
            int r = interpret (ast->value.operands.right);
            return l * r;
        }
        case AST::DIV:
        {
            int l = interpret (ast->value.operands.left);
            int r = interpret (ast->value.operands.right);
            return l / r;
        }
    }
}
