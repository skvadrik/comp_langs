#include "parser.h"

Node * parse_expr (const char * & s)
{
    Node * left = parse_term (s);
    for (;;)
    {
        switch (*s)
        {
            case '+':
            {
                Node * right = parse_term (++s);
                Op * op = new Op (OP_ADD, left, right);
                left = new Node (NODE_OP, op);
                break;
            }
            case '-':
            {
                Node * right = parse_term (++s);
                Op * op = new Op (OP_SUB, left, right);
                left = new Node (NODE_OP, op);
                break;
            }
            default:
                return left;
        }
    }
}

Node * parse_term (const char * & s)
{
    Node * left = parse_factor (s);
    for (;;)
    {
        switch (*s)
        {
            case '*':
            {
                Node * right = parse_term (++s);
                Op * op = new Op (OP_MUL, left, right);
                left = new Node (NODE_OP, op);
                break;
            }
            case '/':
            {
                Node * right = parse_term (++s);
                Op * op = new Op (OP_DIV, left, right);
                left = new Node (NODE_OP, op);
                break;
            }
            default:
                return left;
        }
    }
}

Node * parse_factor (const char * & s)
{
    switch (*s)
    {
        case '(':
        {
            Node * expr = parse_expr (++s);
            ++s; // skip ')'
            return expr;
        }
        default:
            return parse_prim (s);
    }
}

Node * parse_prim (const char * & s)
{
    int result = 0;
    for (;;)
    {
        switch (*s)
        {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            {
                result = result * 10 + (*s - '0');
                ++s;
                break;
            }
            default:
                return new Node (NODE_NUMBER, result);
        }
    }
}
