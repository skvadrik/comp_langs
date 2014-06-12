#include "parser.h"

AST * expr (const char * & s)
{
    AST * l = term (s);
    return expr1 (s, l);
}

AST * expr1 (const char * & s, AST * l)
{
    switch (*s)
    {
        case '+':
        {
            AST * r = term (++s);
            l = new AST (AST::ADD, l, r);
            return expr1 (s, l);
        }
        case '-':
        {
            AST * r = term (++s);
            l = new AST (AST::SUB, l, r);
            return expr1 (s, l);
        }
        default:
            return l;
    }
}

AST * term (const char * & s)
{
    AST * l = factor (s);
    return term1 (s, l);
}

AST * term1 (const char * & s, AST * l)
{
    switch (*s)
    {
        case '*':
        {
            AST * r = factor (++s);
            l = new AST (AST::MUL, l, r);
            return term1 (s, l);
        }
        case '/':
        {
            AST * r = factor (++s);
            l = new AST (AST::DIV, l, r);
            return term1 (s, l);
        }
        default:
            return l;
    }
}

AST * factor (const char * & s)
{
    switch (*s)
    {
        case '(':
        {
            AST * e = expr (++s);
            ++s; // skip ')'
            return e;
        }
        default:
            return new AST (number (s, 0));
    }
}

unsigned int number (const char * & s, unsigned int n)
{
    unsigned char d = digit (s);
    if (d == 0xFF)
        return n;
    else
        return number (s, n * 10 + d);
}

unsigned char digit (const char * & s)
{
    switch (*s)
    {
        case '0': ++s; return 0;
        case '1': ++s; return 1;
        case '2': ++s; return 2;
        case '3': ++s; return 3;
        case '4': ++s; return 4;
        case '5': ++s; return 5;
        case '6': ++s; return 6;
        case '7': ++s; return 7;
        case '8': ++s; return 8;
        case '9': ++s; return 9;
        default:  return 0xFF;
    }
}
