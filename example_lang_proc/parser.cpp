#include "parser.h"

void parse (const char * & s, AST & ast)
{
    ast.root = parse_expr (s);
}

Node * parse_expr (const char * & s)
{
    Node * l = parse_term (s);
    for (;;)
    {
        switch (*s)
        {
            case '+':
            {
                Node * r = parse_term (++s);
                l = new Node (Node::OP, new Op (Op::ADD, l, r));
                break;
            }
            case '-':
            {
                Node * r = parse_term (++s);
                l = new Node (Node::OP, new Op (Op::SUB, l, r));
                break;
            }
            default:
                return l;
        }
    }
}

Node * parse_term (const char * & s)
{
    Node * l = parse_factor (s);
    for (;;)
    {
        switch (*s)
        {
            case '*':
            {
                Node * r = parse_term (++s);
                l = new Node (Node::OP, new Op (Op::MUL, l, r));
                break;
            }
            case '/':
            {
                Node * r = parse_term (++s);
                l = new Node (Node::OP, new Op (Op::DIV, l, r));
                break;
            }
            default:
                return l;
        }
    }
}

Node * parse_factor (const char * & s)
{
    switch (*s)
    {
        case '(':
        {
            Node * e = parse_expr (++s);
            ++s; // skip ')'
            return e;
        }
        default:
            return parse_prim (s);
    }
}

Node * parse_prim (const char * & s)
{
    int n = 0;
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
                n = n * 10 + (*s - '0');
                ++s;
                break;
            }
            default:
                return new Node (n);
        }
    }
}
