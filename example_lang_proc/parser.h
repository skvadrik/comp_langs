#ifndef __PARSER__
#define __PARSER__

// forward declarations
struct Node;
struct AST;

void parse (const char * & s, AST & ast);
Node * parse_expr (const char * & s);
Node * parse_term (const char * & s);
Node * parse_factor (const char * & s);
Node * parse_prim (const char * & s);

struct Op
{
    enum OpType {ADD, SUB, MUL, DIV} type;
    Node * left;
    Node * right;

    Op (OpType t, Node * l, Node * r) : type (t), left (l), right (r) {}
};

struct Node
{
    enum NodeType {OP, NUMBER} type;
    union NodeValue
    {
        Op * op;
        int number;

        NodeValue (Op * o) : op (o) {}
        NodeValue (int n) : number (n) {}

    } value;

    Node (NodeType t, Op * o) : type (t), value (o) { }
    Node (int n) : type (NUMBER), value (n) { }
    ~Node ()
    {
        if (type == OP)
        {
            delete value.op->left;
            delete value.op->right;
            delete value.op;
        }
    }
};

struct AST
{
    Node * root;
    ~AST () {delete root;}
};

#endif // __PARSER__
