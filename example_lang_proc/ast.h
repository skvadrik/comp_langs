#ifndef __AST__
#define __AST__

struct AST
{
    enum ASTType
    {
        NUMBER,
        ADD,
        SUB,
        MUL,
        DIV
    } type;
    union
    {
        int number;
        struct
        {
            AST * left;
            AST * right;
        } operands;
    } value;

    AST (ASTType t, AST * l, AST * r);
    AST (int n);
    ~AST ();
};

#endif // __AST__
