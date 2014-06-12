#include "ast.h"
#include "parser.h"

AST::AST (ASTType t, AST * l, AST * r)
    : type (t)
    , value ()
{
    value.operands.left = l;
    value.operands.right = r;
}

AST::AST (int n)
    : type (NUMBER)
    , value ()
{
    value.number = n;
}

AST::~AST ()
{
    if (type != NUMBER)
    {
        delete value.operands.left;
        delete value.operands.right;
    }
}
