#ifndef __PARSER__
#define __PARSER__

enum NodeType
    { NODE_OP
    , NODE_NUMBER
    };

struct Op; // forward
union NodeValue
{
    Op * op;
    int number;

    NodeValue (Op * o)
        : op (o)
    { }

    NodeValue (int n)
        : number (n)
    { }
};

struct Node
{
    NodeType type;
    NodeValue value;

    Node (NodeType t, Op * o)
        : type (t)
        , value (o)
    { }

    Node (NodeType t, int n)
        : type (t)
        , value (n)
    { }
};

enum OpType
    { OP_ADD
    , OP_SUB
    , OP_MUL
    , OP_DIV
    };

struct Op
{
    OpType type;
    Node * left;
    Node * right;

    Op (OpType t, Node * l, Node * r)
        : type (t)
        , left (l)
        , right (r)
    { }
};

Node * parse_expr (const char * & s);
Node * parse_term (const char * & s);
Node * parse_factor (const char * & s);
Node * parse_prim (const char * & s);

#endif // __PARSER__
